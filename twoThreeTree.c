#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Python.h>

struct tree * makeTree(void);
int contains(struct tree *tree, int val);
int addNodeToTree(struct tree *tree, int val);
int removeNodeFromTree(struct tree *tree, int val);
int leafCount(struct tree *tree);
struct node * makeNode(int valOne, int valTwo);
struct nodeReturner * makeNodeReturner(int inserted);
struct files * setInputAndOutput(int argc, char *args[]);
struct node * expand(struct node *root, struct node *returnedNode);
struct nodeReturner * insert(struct node *root, struct node *node);
struct nodeRemover * findNode(struct node *root, int target);
int findLargestNodeVal(struct node *node);
void valOneSwap(struct node *treeNode, struct node *newNode);
void printTree(struct node *root);
struct listNode * makeListNode(struct node *treeNode, int row);
struct nodeRemover * makeNodeRemover(void);

struct tree {
  struct node *root;
  int leafCount;
};

struct node {
  int valOne;
  int valTwo;
  struct node *left;
  struct node *middle;
  struct node *right;
};

struct nodeReturner {
  int inserted;
  struct node *node;
};

struct nodeRemover {
  int removed;
  int restructure;
  int newVal;
};

struct files {
  FILE *input;
  FILE *output;
};

struct listNode {
  int row;
  struct node *treeNode;
  struct listNode *next;
};

struct tree * makeTree() 
{
  struct tree *tree = malloc(sizeof(*tree));
  tree->root = makeNode(0, 0);
  tree->leafCount = 0;
  return tree;
}

static PyObject * twoThreeTree(PyObject *self, PyObject *args)
{
  return Py_BuildValue("k", makeTree());
}

static PyObject * removeNode(PyObject *self, PyObject *args)
{
  struct tree *tree;
  int val;
  if (!PyArg_ParseTuple(args, "ki", &tree, &val)) {
    return NULL;
  } else {
    return Py_BuildValue("i", removeNodeFromTree(tree, val));
  }
}

static PyObject * addNode(PyObject *self, PyObject *args)
{
  struct tree *tree; 
  int val; 
  if (!PyArg_ParseTuple(args, "ki", &tree, &val)) {
    return NULL;
  } else {
    return Py_BuildValue("i", addNodeToTree(tree, val));
  }
}

static PyObject * hasVal(PyObject *self, PyObject *args)
{
  struct tree *tree;
  int val;
  if (!PyArg_ParseTuple(args, "ki", &tree, &val)) {
    return NULL;
  } else {
    return Py_BuildValue("i", contains(tree, val));
  }
}

static PyObject * count(PyObject *self, PyObject *args)
{
  struct tree *tree;
  if (!PyArg_ParseTuple(args, "k", &tree)) {
    return NULL;
  } else {
    return Py_BuildValue("i", leafCount(tree));
  }
}

static PyObject * version(PyObject *self)
{
  return Py_BuildValue("s", "Version 1.0");
}

static PyMethodDef myMethods[] = {
  {"twoThreeTree", twoThreeTree, METH_VARARGS, "Creates a 2-3 tree instance."},
  {"addNode", addNode, METH_VARARGS, "Adds a node to the tree."},
  {"removeNode", removeNode, METH_VARARGS, "Removes a node from the tree."},
  {"hasVal", hasVal, METH_VARARGS, "Checks to see if the tree contains a value."},
  {"count", count, METH_VARARGS, "Returns the number of items in the tree."},
  {"version", (PyCFunction) version, METH_NOARGS, "returns the version."},
  {NULL, NULL, 0, NULL}
};

static struct PyModuleDef Tree = {
  PyModuleDef_HEAD_INIT,
  "Tree",
  "2-3 Tree Module",
  -1,
  myMethods
};

PyMODINIT_FUNC PyInit_Tree(void)
{
  return PyModule_Create(&Tree);
}

int leafCount(struct tree *tree)
{
  return tree->leafCount;
}

int contains(struct tree *tree, int val) 
{
  struct node *root = tree->root;
  while (root->left) {
    if (val <= root->valOne) {
      root = root->left;
    } else if (val <= root->valTwo) {
      root = root->middle;
    } else if (root->right) {
      root = root->right;
    }
  }
  if (val == root->valOne) {
    return 1;
  }
  return 0;
}

int addNodeToTree(struct tree *tree, int val)
{
  struct node *root = tree->root;
  struct node *node = makeNode(val, 0);
  struct nodeReturner *nodeReturner = NULL;
  struct node *newRoot = NULL;
  if (tree->leafCount < 3) {
    if (root->left) {
      if (node->valOne <= root->valOne) {
        if (node->valOne == root->valOne) {
          return 0;
        }
        root->valOne = node->valOne;
        valOneSwap(root->left, node);
      }
      if (root->middle) {
        if (node->valOne <= root->valTwo) {
          if (node->valOne == root->valTwo) {
            return 0;
          }
          root->valTwo = node->valOne;
          valOneSwap(root->middle, node); 
        }
        root->right = node;
      } else {
        root->valTwo = node->valOne;
        root->middle = node;
      }
    } else {
      root->valOne = node->valOne;
      root->left = node;
    }
    ++tree->leafCount;
  } else {
    nodeReturner = insert(root, node);
    if (nodeReturner->node) {
      newRoot = makeNode(root->valTwo, nodeReturner->node->valTwo);
      newRoot->left = root;
      newRoot->middle = nodeReturner->node;
      tree->root = newRoot;
    }
    if (nodeReturner->inserted) {
      ++tree->leafCount;
    } else {
      return 0;
    } 
  }
  return 1;
}

int removeNodeFromTree(struct tree *tree, int val)
{
  int removedFlag = 0;
  struct node *root = tree->root;
  struct nodeRemover *nodeRemover = findNode(root, val);
  if (nodeRemover->removed) {
    --tree->leafCount;
    removedFlag = 1;
  }
  if (nodeRemover->restructure && tree->leafCount > 1) {
    tree->root = root->left;
  }
  return removedFlag;
}

struct nodeRemover * findNode(struct node *root, int target)
{
  struct nodeRemover *nodeRemover = makeNodeRemover();
  int removedPosition = 0;
  if (!root->left->left) {
    if (root->right) {
      if (root->left->valOne == target) {
        free(root->left);
        root->left = root->middle;
        root->valOne = root->valTwo;
        root->middle = root->right;
        root->valTwo = root->right->valOne;
        root->right = NULL;
      } else if (root->middle->valOne == target) {
        free(root->middle); 
        root->middle = root->right;
        root->valTwo = root->right->valOne;
        root->right = NULL;
      } else if (root->right->valOne == target) {
        free(root->right);
        nodeRemover->newVal = root->valTwo;
        root->right = NULL;
      } else {
        nodeRemover->removed = 0;
      } 
    } else {
      if (root->left->valOne == target) {
        free(root->left);
        root->left = root->middle;
        root->valOne = root->valTwo;
        root->middle = NULL;
        nodeRemover->restructure = 1;
      } else if (root->middle->valOne == target) {
        free(root->middle);
        root->middle = NULL;
        nodeRemover->newVal = root->valOne;
        nodeRemover->restructure = 1;
      } else {
        nodeRemover->removed = 0;
      }
    }
  } else {
    if (target <= root->valOne) {
      removedPosition = 1;
      nodeRemover = findNode(root->left, target);
      if (target == root->valOne) {
        root->valOne = nodeRemover->newVal;
      }
    } else if (target <= root->valTwo) {
      removedPosition = 2;
      nodeRemover = findNode(root->middle, target);
      if (target == root->valTwo) {
        root->valTwo = nodeRemover->newVal;
      }
    } else if (root->right) {
      removedPosition = 3;
      nodeRemover = findNode(root->right, target);
    } else {
      nodeRemover->removed = 0;
    }
    if (nodeRemover->restructure) {
      if (removedPosition == 1) {
        root->left->middle = root->middle->left;
        root->left->valTwo = root->middle->valOne;
        root->valOne = root->middle->valOne;
        if (root->middle->right) {
          root->middle->left = root->middle->middle;
          root->middle->valOne = root->middle->valTwo;
          root->middle->middle = root->middle->right; 
          root->middle->valTwo = root->valTwo;
          root->middle->right = NULL;
          nodeRemover->restructure = 0;
        } else {
          root->left->right = root->middle->middle;
          root->valOne = root->middle->valTwo;
          free(root->middle);
          if (root->right) {
            root->middle = root->right;
            root->valTwo = findLargestNodeVal(root->middle);
            root->right = NULL;
            nodeRemover->restructure = 0;
          } else {
            root->middle = NULL;
          }
        }
      } else if (removedPosition == 2) {
        if (root->right) {
          root->middle->middle = root->right->left;
          root->middle->valTwo = root->right->valOne;
          root->valTwo = root->right->valOne;
          if (root->right->right) {
            root->right->left = root->right->middle;
            root->right->valOne = root->right->valTwo;
            root->right->middle = root->right->right;
            root->right->valTwo = findLargestNodeVal(root->right);
            root->right->right = NULL;
          } else {
            root->middle->right = root->right->middle;
            root->valTwo = root->right->valTwo;
            free(root->right);
            root->right = NULL;
          }
          nodeRemover->restructure = 0;
        } else if (root->left->right) {
          root->middle->middle = root->middle->left;
          root->middle->valTwo = root->middle->valOne;
          root->middle->left = root->left->right;
          root->middle->valOne = root->valOne;
          root->left->right = NULL;
          root->valOne = root->left->valTwo;
          nodeRemover->restructure = 0;
        } else {
          root->left->right = root->middle->left;
          root->valOne = root->middle->valOne;
          free(root->middle);  
          root->middle = NULL;
        }
      } else if (removedPosition == 3) {
        if (root->middle->right) {
          root->right->middle = root->right->left;
          root->right->valTwo = root->right->valOne;
          root->right->left = root->middle->right;
          root->right->valOne = root->valTwo;
          root->middle->right = NULL;
          root->valTwo = root->middle->valTwo;
        } else {
          root->middle->right = root->right->left;
          root->valTwo = root->right->valOne;
          free(root->right);
          root->right = NULL;
        }
        nodeRemover->restructure = 0;
      } 
    }
  }
  return nodeRemover;
}

struct node * expand(struct node *root, struct node *returnedNode)
{
  struct node *newNode = NULL;
  if (returnedNode->valOne < root->valOne) {
    newNode = makeNode(root->valTwo, findLargestNodeVal(root->right));
    newNode->left = root->middle;
    newNode->middle = root->right;
    root->right = NULL;
    root->middle = returnedNode;
    root->valTwo = returnedNode->valTwo;
    root->valOne = root->left->valTwo;
  } else if (returnedNode->valOne < root->valTwo) {
    newNode = makeNode(root->valTwo, findLargestNodeVal(root->right));
    newNode->left = returnedNode;
    newNode->middle = root->right;
    root->right = NULL;
    root->valTwo = root->middle->valTwo;
  } else {
    newNode = makeNode(findLargestNodeVal(root->right), returnedNode->valTwo);
    newNode->left = root->right;
    root->right = NULL;
    newNode->middle = returnedNode;
  }
  return newNode;
}

struct nodeReturner * insert(struct node *root, struct node *node)
{
  struct nodeReturner *nodeReturner = makeNodeReturner(1);
  int insertPosition = 0;
  if (!root->left->left) { // root has leaves
    if (node->valOne == root->left->valOne || node->valOne == root->middle->valOne || (root->right && node->valOne == root->right->valOne)) {
      // value already present in tree
      nodeReturner->inserted = 0;
      return nodeReturner;
    }
    if (node->valOne < root->left->valOne) {
      root->valOne = node->valOne;
      valOneSwap(root->left, node);
    }
    if (node->valOne < root->middle->valOne) {
      root->valTwo = node->valOne;
      valOneSwap(root->middle, node);
    }
    if (!root->right) {
      root->right = node;
    } else if (node->valOne < root->right->valOne) {
      nodeReturner->node = makeNode(node->valOne, root->right->valOne);
      nodeReturner->node->left = node;
      nodeReturner->node->middle = root->right;
      root->right = NULL;
    } else if (node->valOne > root->right->valOne) {
      nodeReturner->node = makeNode(root->right->valOne, node->valOne);
      nodeReturner->node->left = root->right;
      root->right = NULL;
      nodeReturner->node->middle = node;
    }
  } else {
    if (node->valOne <= root->valOne) {
      insertPosition = 1;
      nodeReturner = insert(root->left, node);
    } else if (node->valOne <= root->valTwo) {
      insertPosition = 2;
      nodeReturner = insert(root->middle, node);
    } else {
      if (!root->right) {
        nodeReturner = insert(root->middle, node); 
        if (!nodeReturner->node && nodeReturner->inserted) {
          root->valTwo = node->valOne;
        }
      } else {
        insertPosition = 3;
        nodeReturner = insert(root->right, node);
      }
    }
    if (nodeReturner->node) {
      if (root->right) {
        nodeReturner->node = expand(root, nodeReturner->node);
        // Can return expansion logic to here; not generalizable to handling at root
      } else if (insertPosition == 1) {
        root->valOne = root->left->valTwo;
        root->right = root->middle;
        root->middle = nodeReturner->node;
        root->valTwo = nodeReturner->node->valTwo;
        nodeReturner->node = NULL;
      } else {
        root->right = nodeReturner->node;
        root->valTwo = root->middle->valTwo;
        nodeReturner->node = NULL;
      }
    }
  }
  return nodeReturner;
}

int findLargestNodeVal(struct node *node)
{
  while (node->left) {
    if (node->right) {
      node = node->right;
    } else {
      node = node->middle;
    }
  }
  return node->valOne;
}

void valOneSwap(struct node *treeNode, struct node *newNode)
{
  int tempVal = treeNode->valOne;
  treeNode->valOne = newNode->valOne;
  newNode->valOne = tempVal;
}

struct node * makeNode(int valOne, int valTwo)
{
  struct node *node = malloc(sizeof(*node));
  node->valOne = valOne;
  node->valTwo = valTwo;
  node->left = NULL;
  node->middle = NULL;
  node->right = NULL;
  return node;
}

struct nodeReturner * makeNodeReturner(int inserted)
{
  struct nodeReturner *nodeReturner = malloc(sizeof(*nodeReturner));
  nodeReturner->inserted = inserted;
  nodeReturner->node = NULL;
  return nodeReturner;
}

struct nodeRemover * makeNodeRemover() 
{
  struct nodeRemover *nodeRemover = malloc(sizeof(*nodeRemover));
  nodeRemover->removed = 1;
  nodeRemover->restructure = 0;
  return nodeRemover;
}

static PyObject * bfList(PyObject *nodes[], int length)
{
  PyObject *nodesPyList = PyList_New(length);
  int i;
  for (i = 0; i < length; ++i) {
    PyList_SET_ITEM(nodesPyList, i, PyInt_FromLong(nodes[i]));
  }
  return nodesPyList;
}

void printTree(struct node *root)
{
  int totalNodeCount = 1;
  int rowNodeCount = 0;
  int row = 0;
  int newRow = 0;
  struct listNode *node = makeListNode(root, row);
  struct listNode *tail = node;
  struct listNode *temp;
  while (node) {
    ++rowNodeCount;
    if (node->row > row) {
      ++row;
      newRow = 1;
      rowNodeCount = 1;
    }
    if (node->treeNode->left) { // not a leaf
      tail->next = makeListNode(node->treeNode->left, row + 1);
      tail = tail->next;
      ++totalNodeCount;
      if (node->treeNode->middle) {
        tail->next = makeListNode(node->treeNode->middle, row + 1);
        tail = tail->next;
        ++totalNodeCount;
      }
      if (node->treeNode->right) {
        tail->next = makeListNode(node->treeNode->right, row + 1);
        tail = tail->next;
        ++totalNodeCount;
      }
    }
    if (newRow) {
      if (row > 0) {
        printf("|\n|");
      }
      newRow = 0;
    } else {
      printf("|");
    }
    printf("%d", node->treeNode->valOne);
    if (node->treeNode->left) {
      printf(",%d", node->treeNode->valTwo);
    }
    temp = node;
    node = node->next;
    free(temp);
  }
  printf("|\n");
  printf("%d total nodes; %d leaf nodes\n", totalNodeCount, rowNodeCount);
}



struct listNode * makeListNode(struct node *treeNode, int row)
{
  struct listNode *entry = malloc(sizeof(*entry));
  entry->row = row;
  entry->treeNode = treeNode;
  entry->next = NULL;
  return entry;
}
