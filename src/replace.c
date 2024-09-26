#include "args.h"
#include "files.h"
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Replaces args.target with args.replace in zContent.
//char *replaceXY(struct Args args, const char *zContent) {
char* replaceXY(char* target, char* replace, int wordMatch, const char *zContent) {

printf("replaceXY : %s %s %d\n", target, replace, wordMatch);

  size_t xLen = strlen(target);
  size_t zLen = strlen(zContent);
  char *buffer = malloc((zLen + 1) * sizeof(char));

  if (buffer == NULL) {
    printf("Cannot allocate memory to replace x with y\n");
    return NULL;
  }

  memset(buffer, 0, zLen + 1);

  size_t pos = 0;
  while (pos < zLen) {
    char *match = strstr(zContent + pos, target);

    if (match == NULL) {
      strcat(buffer, zContent + pos);
      break;
    }

    if (wordMatch == 1) {
      strncat(buffer, zContent + pos, match - (zContent + pos));
      strcat(buffer, replace);
      pos = match - zContent + xLen;
      continue;
    }

    if ((match == zContent || isspace(*(match - 1))) && (match[xLen] == '\0' || isspace(match[xLen]))) {
      strncat(buffer, zContent + pos, match - (zContent + pos));
      strcat(buffer, replace);
      pos = match - zContent + xLen;
    } else {
      strncat(buffer, zContent + pos, match - (zContent + pos) + 1);
      pos = match - zContent + 1;
    }
  }

  return buffer;
}


int write_file(struct Args* args, const char *path,  char *zContent, char *modifiedContent) {
    
  if (!args->out_dir && strcmp(zContent, modifiedContent) == 0) {
    free(zContent);
    free(modifiedContent);
    return 1;
  }

  FILE *file = fopen(path, "w");
  if (file == NULL) {
    free(zContent);
    free(modifiedContent);
    return 1;
  }

  size_t len = strlen(modifiedContent);
  fwrite(modifiedContent, sizeof(char), len, file);


  fclose(file);
  free(zContent);
  free(modifiedContent);
  return 0;
}

// Replaces x with y in FILE of path.
// If something wents wrong, error will be logged in console
// appropriate error number will be returned.
int replace(struct Args* args, const char *path) {
  char *zContent = readFileContent(path);
  if (zContent == NULL) {
    return 1;
  }
    char *modifiedContent=zContent;
//char *replaceXY(char* target, char* replace, int wordMatch, const char *zContent) 
  
  for(int i = 0; i < args->action_sz; i++){
      modifiedContent = replaceXY(args->action[i].target, args->action[i].replace, args->wordMatch, modifiedContent);
      printf("-------------\n");
      printf("modifiedContent : %s \n", modifiedContent);
        printf("-------------\n");
  }
  
  // char *modifiedContent = replaceXY(args, zContent);
   if (modifiedContent == NULL) {
      free(zContent);
      return 1;
   }
   
   //////////////////
   //////////////////
   //////////////////
   printf("\nWrite file %s\n", path);
   if(args->out_dir){
      printf("\nWrite file %s\n", path);
      int sz = strlen(path)-1;
      while(sz>0 && !(path[sz]=='/' || path[sz]=='\\')){sz--;}   
      if(sz){
        char vla[sz+args->out_dirLen+1];
         sprintf(vla, "%.*s%s%s",sz+1, path, args->out_dir, &path[sz+1] );
         printf("\nOut file %s\n", vla);
         write_file(args, vla, zContent,modifiedContent);
         return 0;
      }
   }
     //////////////////
   //////////////////
 
  write_file(args, path, zContent,modifiedContent);
  return 0;
}
