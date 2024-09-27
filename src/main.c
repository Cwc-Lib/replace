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

  forEachRule(args.paths, args.pathsLen, &args);
     printf("--\n");
   for (int i = 0; i< args.pathsLen; i++) {
      for(int j = 0; j < args.action_sz; j++){
         printf("%s:(%d) '%s' \n", args.paths[i].norm_path,  args.action[j].replace_count, args.action[j].target );
         printf("%s: '%s'\n", args.paths[i].output, args.action[j].replace );
         if(args.action[j].replace_count==0){
             printf("error, not found: ", args.action[j].target);
         }
         printf("--\n");
      }
   }
//  free(replaced);
 // free(rules);

  return EXIT_SUCCESS;
}
