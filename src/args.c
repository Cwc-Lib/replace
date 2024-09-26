#include "args.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Args parseArgs(int argc, char *argv[]) {
  struct Args args;
  args.target = NULL;
  args.replace = NULL;
  args.paths = NULL;
  args.pathsLen = 0;
  args.wordMatch = 1;

  if (argc < 3) {
    return args;
  }

   int idx = 1;
   if (strcmp("-w", argv[idx]) == 0 || strcmp("--word", argv[idx]) == 0) {
      args.wordMatch = 0;
      idx++;
   }

   if (strcmp("-f", argv[idx]) == 0 || strcmp("--file", argv[idx]) == 0) {
      idx++;
      args.paths = (char **)malloc((argc - idx) * sizeof(char *));
      while(idx<argc && argv[idx][0]!='-'){ 
        args.paths[args.pathsLen] = (char *)malloc(strlen(argv[idx]) + 1);
        strcpy(args.paths[args.pathsLen], argv[idx]);
        args.pathsLen++;
        idx++;
      }
   }
   
   
  if (strcmp("-o", argv[idx]) == 0 || strcmp("--out", argv[idx]) == 0) {
    args.wordMatch = 0;
    idx++;
    args.out_dir = argv[idx];
    args.out_dirLen = strlen(args.out_dir);
    idx++;
    printf("output files to: %s\n",  args.out_dir);
  }

  args.target = argv[idx];
  idx++;
  args.replace = argv[idx];
  idx++;
  
  /*
  int zexpect = idx;
  if (argc > idx) {
    args.paths = (char **)malloc((argc - idx) * sizeof(char *));
    if (args.paths == NULL) {
      fprintf(stderr, "Memory allocation failed for paths.\n");
      exit(1);
    }

    for (; idx < argc; idx++) {
      int pathidx = idx - zexpect;
      args.paths[pathidx] = (char *)malloc(strlen(argv[idx]) + 1);
      if (args.paths[pathidx] == NULL) {
        fprintf(stderr, "Memory allocation failed for path %d.\n", idx);
        exit(1);
      }
      strcpy(args.paths[pathidx], argv[idx]);
      args.pathsLen++;
    }
  }*/
  
  if(args.pathsLen==0){
   printf("error, -f flag required");
   exit(0);
  }
  

  return args;
}
