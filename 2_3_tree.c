#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct files * setInputAndOutput(int argc, char *args[]);

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

int main(int argc, char *argv[])
{
  struct node *head = malloc(sizeof(head));
  head->isLeaf = 1;
  struct files *io = setInputAndOutput(argc, argv);
  int count;
  char *input;
  size_t max = 10;
  for (count = 0; getline(&input, &max, io->input) != -1; ++count) {
    if (io->output == stdout) {
      printf("Entered %d\n", atoi(input));
    } else {
      fprintf(io->output, "Entered %d\n", atoi(input));
    }
  }
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
