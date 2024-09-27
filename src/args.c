#include "args.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Args parseArgs(int argc, char *argv[]) {
   struct Args args={};
   setvbuf(stdout, NULL, _IONBF, 0);
   setvbuf(stderr, NULL, _IONBF, 0);


   args.pathsLen = 0;
   args.wordMatch = 1;

  if (argc < 3) {
    return args;
  }
  
   int ii = 1;
   printf("-----\n");
   while(ii<argc){
      ii++;
      printf("arg: %s\n", argv[ii]);
   }
   printf("-----\n");

    
   int idx = 1;
   while(idx<argc){

      if (strcmp("-w", argv[idx]) == 0 || strcmp("--word", argv[idx]) == 0) {
         args.wordMatch = 0;
         idx++;
      }

      if (strcmp("-f", argv[idx]) == 0 || strcmp("--file", argv[idx]) == 0) {
         idx++;
         //args.paths = (char **)malloc( * sizeof(file_t));
         while(idx<argc && argv[idx][0]!='-'){ 
           //args.paths[args.pathsLen] = (char *)malloc(strlen(argv[idx]) + 1);
           strcpy(args.paths[args.pathsLen].input, argv[idx]);
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
      
    //  if (strcmp("-r", argv[idx]) == 0)  {
      if (argv[idx][0]=='-' && argv[idx][1]=='r')  {
         if(argv[idx][2] == 'a'){
            args.action[args.action_sz].type = ACT_ADD;
         }else{
            args.action[args.action_sz].type = ACT_REPLACE;
         }
         idx++;
         args.action[args.action_sz].target = argv[idx];
        // printf("target: %s\n", args.action[args.action_sz].target);
         idx++;
         args.action[args.action_sz].replace = argv[idx];
         //printf("replace: %s\n", args.action[args.action_sz].replace);
         idx++;
         args.action_sz++;
      }
  }
  
  
  //   args.target = argv[idx];
  //   idx++;
  //   args.replace = argv[idx];
  //   idx++;

  if(args.pathsLen==0){
   printf("error, -f flag required");
   exit(0);
  }
  

  return args;
}
