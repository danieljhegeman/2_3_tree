#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct node * makeNode(int valOne, int valTwo);
struct nodeReturner * makeNodeReturner(int inserted);
struct files * setInputAndOutput(int argc, char *args[]);
struct node * expand(struct node *root, struct node *returnedNode);
struct nodeReturner * insert(struct node *root, struct node *node);
int findLargestNodeVal(struct node *node);
void valOneSwap(struct node *treeNode, struct node *newNode);
void printTree(struct node *root);
struct listNode * makeListNode(struct node *treeNode, int row);

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
  int leafCount = 0;
  struct node *root = makeNode(0, 0);
  struct node *node = makeNode(0, 0);
  struct nodeReturner *nodeReturner = NULL;
  struct node *newRoot = NULL;
  struct files *io = setInputAndOutput(argc, argv);
  char *input = NULL;
  size_t max = 10;
  while (getline(&input, &max, io->input) != -1) {
    node = makeNode(atoi(input), 0);
    if (leafCount < 3) {
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
      ++leafCount;
    } else {
      nodeReturner = insert(root, node);
      if (nodeReturner->node) {
        newRoot = makeNode(root->valTwo, nodeReturner->node->valTwo);
        newRoot->left = root;
        newRoot->middle = nodeReturner->node;
        root = newRoot;
      }
      if (nodeReturner->inserted) {
        ++leafCount;
      }
    }
    printTree(root);
  }
}

struct node * expand(struct node *root, struct node *returnedNode) {
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

struct nodeReturner * insert(struct node *root, struct node *node) {
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
    return nodeReturner;
  }
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
  return nodeReturner;
}

int findLargestNodeVal(struct node *node) {
  while (node->left) {
    if (node->right) {
      node = node->right;
    } else {
      node = node->middle;
    }
  }
  return node->valOne;
}

void valOneSwap(struct node *treeNode, struct node *newNode) {
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
