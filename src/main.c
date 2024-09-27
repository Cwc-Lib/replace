#include "args.c"
#include "errors.c"
#include "files.c"
#include "files.h"
#include "replace.c"
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>


int main(int argc, char *argv[]) {
  checkArgs(argc, argv);

  struct Args args = parseArgs(argc, argv);
  struct FilePathRule *rules = generateFilePathRules(args.pathsLen, args.paths);
  int len = args.pathsLen;
  if (len == 0) {
    len = 1;
  }

  char **replaced = forEachRule(rules, len, &args, replace);
  for (int i = 0; replaced[i] != NULL; i++) {
   for(int j = 0; j < args.action_sz; j++){
     
   //  modifiedContent = replaceXY(args.action[i].target, args.action[i].replace, args.wordMatch, modifiedContent);
     printf("Replaced[%d]: '%s' with '%s' from %s\n",  args.action[j].replace_count, args.action[j].target, args.action[j].replace, replaced[i] );
   }
  }
  free(replaced);
  free(rules);

  return EXIT_SUCCESS;
}
