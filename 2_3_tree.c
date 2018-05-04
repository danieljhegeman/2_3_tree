#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct files * setInputAndOutput(int argc, char *args[]);
struct node * newNode(int val);
void valOneSwap(struct node * treeNode, struct node * newNode);
void insert(struct node * root, int val);

struct node {
  int isLeaf;
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
  struct node *root = newNode(0);
  struct node *node = newNode(0);
  struct files *io = setInputAndOutput(argc, argv);
  char *input = NULL;
  size_t max = 10;
  while (getline(&input, &max, io->input) != -1) {
    node = newNode(atoi(input));
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
    }
    ++count;
  }
}

void valOneSwap(struct node * treeNode, struct node * newNode) {
  int tempVal = treeNode->valOne;
  treeNode->valOne = newNode->valOne;
  newNode->valOne = tempVal;
}

void insert(struct node * root, int val) {
  if (val < root->valOne) {
    if (root->middle) {
      
    } else {
      
    }
  }
}

struct node * newNode(int val)
{
  struct node * node = malloc(sizeof(*node));
  node->isLeaf = 0;
  node->valOne = val;
  node->valTwo = 0;
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
