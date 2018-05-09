#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct node * makeNode(int valOne, int valTwo);
struct nodeReturner * makeNodeReturner(int inserted);
struct files * setInputAndOutput(int argc, char *args[]);
struct node * expand(struct node *root, struct node *returnedNode);
struct nodeReturner * insert(struct node *root, struct node *node);
void addNodes(struct node **rootPtr, struct files *io, int *leafCountPtr);
void findAndRemoveNodes(struct node **rootPtr, int *leafCountPtr);
struct nodeRemover * findNode(struct node *root, int target);
int findLargestNodeVal(struct node *node);
void valOneSwap(struct node *treeNode, struct node *newNode);
void printTree(struct node *root);
struct listNode * makeListNode(struct node *treeNode, int row);
struct nodeRemover * makeNodeRemover();

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

int main(int argc, char *argv[])
{
  struct files *io = setInputAndOutput(argc, argv);
  int leafCount = 0;
  struct node *root = makeNode(0, 0);
  char *input = NULL;
  size_t max = 10;
  printf("Enter any key to add values\n");
  while (getline(&input, &max, stdin) != -1) {
    printf("Add values\n");
    addNodes(&root, io, &leafCount);
    printf("Remove values\n");
    findAndRemoveNodes(&root, &leafCount);
    printf("Enter any key to add values\n");
  }
}

void findAndRemoveNodes(struct node **rootPtr, int *leafCountPtr)
{
  struct nodeRemover *nodeRemover;
  char *input = NULL;
  size_t max = 10;
  while (getline(&input, &max, stdin) != -1) {
    nodeRemover = findNode(*rootPtr, atoi(input));
    if (nodeRemover->removed) {
      --*leafCountPtr;
    }
    if (nodeRemover->restructure && *leafCountPtr > 1) {
      *rootPtr = (*rootPtr)->left;
    }
    printf("Num leaf nodes after removing: %d\n", *leafCountPtr);
    printTree(*rootPtr);
  }
}

struct nodeRemover * findNode(struct node *root, int target)
{
  struct nodeRemover *nodeRemover = makeNodeRemover();
  int removedPosition;
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

void addNodes(struct node **rootPtr, struct files *io, int *leafCountPtr)
{
  struct node *root = *rootPtr;
  struct node *node = makeNode(0, 0);
  struct nodeReturner *nodeReturner = NULL;
  struct node *newRoot = NULL;
  char *input = NULL;
  size_t max = 10;
  while (getline(&input, &max, io->input) != -1) {
    node = makeNode(atoi(input), 0);
    if (*leafCountPtr < 3) {
      if (root->left) {
        if (node->valOne <= root->valOne) {
          if (node->valOne == root->valOne) {
            continue;
          }
          root->valOne = node->valOne;
          valOneSwap(root->left, node);
        }
        if (root->middle) {
          if (node->valOne <= root->valTwo) {
            if (node->valOne == root->valTwo) {
              continue;
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
      ++*leafCountPtr;
    } else {
      nodeReturner = insert(root, node);
      if (nodeReturner->node) {
        newRoot = makeNode(root->valTwo, nodeReturner->node->valTwo);
        newRoot->left = root;
        newRoot->middle = nodeReturner->node;
        root = newRoot;
        *rootPtr = root;
      }
      if (nodeReturner->inserted) {
        ++*leafCountPtr;
      }
    }
    printTree(root);
  }
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

struct files * setInputAndOutput(int argc, char *args[])
{
  struct files *io = malloc(sizeof(*io));
  if (argc > 1) {
    io->input = fopen(args[1], "r");
    if (argc > 2) { 
      io->output = fopen(args[2], "w");
    } else {
      io->output = stdout;
    }
  } else {
    io->input = stdin;
    io->output = stdout;
  }
  return io;
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
