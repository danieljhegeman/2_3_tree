#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct node * makeNode(int valOne, int valTwo);
struct files * setInputAndOutput(int argc, char *args[]);
struct node * expand(struct node * root, struct node * returnedNode);
void insert(struct node * root, struct node * node);
int findLargestNodeVal(struct node * node);
void valOneSwap(struct node * treeNode, struct node * newNode);

struct node {
  int valOne;
  int valTwo;
  struct node *left;
  struct node *middle;
  struct node *right;
};

struct files {
  FILE *input;
  FILE *output;
};

static int count = 0;

int main(int argc, char *argv[])
{
  struct node *root = makeNode(NULL, NULL);
  struct node *node = makeNode(NULL, NULL);
  struct node *returnedNode = NULL;
  struct node *newNode = NULL;
  struct node *newRoot = NULL;
  struct files *io = setInputAndOutput(argc, argv);
  char *input = NULL;
  size_t max = 10;
  while (getline(&input, &max, io->input) != -1) {
    node = makeNode(atoi(input), NULL);
    fprintf(stderr, "%d\n", node->valOne);
    if (count < 3) {
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
    } else {
      printf("Count is >= 3\n");
      printf("Root valOne: %d\n", root->valOne);
      printf("Root left->valOne: %d\n", root->left->valOne);
      printf("Root valTwo: %d\n", root->valTwo);
      printf("Root middle->valOne: %d\n", root->middle->valOne);
      printf("Root right->valOne: %d\n", root->right->valOne);
//    if (io->output == stdout) {
//      printf("Entered %d\n", atoi(input));
//    } else {
//      fprintf(io->output, "Entered %d\n", atoi(input));
      returnedNode = insert(root, node);
      if (returnedNode) {
        newNode = expand(root, returnedNode);
        newRoot = makeNode(root->valTwo, returnedNode->valTwo);
        newRoot->left = root;
        newRoot->middle = returnedNode;
        root = newRoot;
      }
    }
    ++count;
  }
}

struct node * expand(struct node * root, struct node * returnedNode) {
  struct node * newNode = NULL;
  if (returnedNode->valOne < root->middle->valOne) {
    newNode = makeNode(root->valTwo, findLargestNodeVal(root->right));
    newNode->left = root->middle;
    newNode->middle = root->right;
    root->right = NULL;
    root->middle = returnedNode;
    root->valTwo = returnedNode->valTwo;
  } else if (returnedNode->valOne < root->right->valOne) {
    newNode = makeNode(returnedNode->valTwo, findLargestNodeVal(root->right));
    newNode->left = returnedNode;
    newNode->middle = root->right;
    root->right = NULL;
  } else {
    newNode = makeNode(findLargestNodeVal(root->right), returnedNode->valTwo);
    newNode->left = root->right;
    root->right = NULL;
    newNode->middle = returnedNode;
  }
  return newNode;
}

void insert(struct node * root, struct node * node) {
  struct node * newNode = NULL;
  struct node * returnedNode = NULL;
  int insertPosition;
  if (!root->left->left) { // root has leaves
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
    } else {
      newNode = makeNode(node->valOne, root->right->valOne);
      newNode->left = node;
      newNode->middle = root->right;
      root->right = NULL;
    }
    return newNode;
  }
  if (node->valOne < root->valOne) {
    insertPosition = 0;
    returnedNode = insert(root->left, node);
  } else if (node->valOne < root->valTwo) {
    insertPosition = 1;
    returnedNode = insert(root->middle, node);
  } else {
    insertPosition = 2;
    returnedNode = insert(root->right, node);
  }
  if (returnedNode) {
    if (root->right) {
      newNode = expand(root, returnedNode);
//      if (insertPosition == 0) {
//        newNode = makeNode(root->valTwo, findLargestNodeVal(root->right));
//        newNode->left = root->middle;
//        newNode->middle = root->right;
//        root->right = NULL;
//        root->middle = returnedNode;
//        root->valTwo = returnedNode->valTwo;
//      } else if (insertPosition == 1) { 
//        newNode = makeNode(returnedNode->valTwo, findLargestNodeVal(root->right));
//        newNode->left = returnedNode;
//        newNode->middle = root->right;
//        root->right = NULL;
//      } else {
//        newNode = makeNode(findLargestNodeVal(root->right), returnedNode->valTwo);
//        newNode->left = root->right;
//        root->right = NULL;
//        newNode->middle = returnedNode;
//      }
    } else if (insertPosition == 0) {
      root->right = root->middle;
      root->middle = returnedNode;
      root->valTwo = returnedNode->valTwo;
    } else (insertPosition == 1) {
      root->right = returnedNode;
    }
  }
  return newNode;
}

int findLargestNodeVal(struct node * node) {
  while (node->left) {
    if (node->right) {
      node = node->right;
    } else {
      node = node->middle;
    }
  }
  return node->valOne;
}

void valOneSwap(struct node * treeNode, struct node * newNode) {
  int tempVal = treeNode->valOne;
  treeNode->valOne = newNode->valOne;
  newNode->valOne = tempVal;
}

struct node * makeNode(int valOne, int valTwo)
{
  struct node * node = malloc(sizeof(*node));
  node->isLeaf = 0;
  node->valOne = valOne;
  node->valTwo = valTwo;
  node->left = NULL;
  node->middle = NULL;
  node->right = NULL;
  return node;
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
