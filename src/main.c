#include "debug.c"
#include "files.h"
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

int main(int argc, char *argv[]) {
  checkArgs(argc, argv);

  // Note: Enable for dev mode.
  if (false) {
    return debugRun(argc, argv);
  }

  struct Args args = parseArgs(argc, argv);
  struct FilePathRule *rules = generateFilePathRules(args.pathsLen, args.paths);
  int len = args.pathsLen;
  if (len == 0) {
    len = 1;
  }

  char **replaced = forEachRule(rules, len, &args, replace);
  for (int i = 0; replaced[i] != NULL; i++) {
   for(int i = 0; i < args.action_sz; i++){
   //  modifiedContent = replaceXY(args.action[i].target, args.action[i].replace, args.wordMatch, modifiedContent);
     printf("Replaced: '%s' with '%s' from %s\n", args.action[i].target, args.action[i].replace, replaced[i]);
   }
  }
  free(replaced);
  free(rules);

  return EXIT_SUCCESS;
}
