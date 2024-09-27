#ifndef ARGS_H
#define ARGS_H

/**
 * @brief Structure for storing command-line arguments.
 *
 * This structure is used to hold the command-line arguments parsed from the
 * input arguments of a program.
 */
enum Action{
  ACT_REPLACE,
  ACT_ADD,
};

typedef struct action_t{
  enum Action type;
  char* target;  /// The character to be replaced (X).
  char* replace; /// The character to replace 'target' with (Y).
  int replace_count;
  char* out_file;//malloc file out
  
}action_t;
 
 #define MAX_PATH 256
 #define MAX_FILE 20
 
typedef struct file_t{
   char input[MAX_PATH];
   char output[MAX_PATH];
   
   
   
   char* norm_path;
}file_t;
 
 
struct Args {
  //char* target;  /// The character to be replaced (X).
  //char* replace; /// The character to replace 'target' with (Y).
  action_t action[512];
  int action_sz;

  //char **paths;  /// An array of strings representing paths (Z).
 file_t paths[MAX_FILE];
 
 int pathsLen;  /// The number of paths in the 'paths' array.
  
  
  
  char* out_dir;  /// Directory output copy of all files with replcement, flag: -o.
  int out_dirLen;  /// The number of paths in the 'paths' array.
int wordMatch; /// This int flag determines whether the replacement
                 /// algorithm requires an exact, space-free match for
                 /// segments to be replaced.
                 
};

/**
 * @brief Parse command-line arguments and populate an Args structure.
 *
 * This function takes the command-line arguments (argc and argv) and parses
 * them to populate an Args structure with the provided values.
 *
 * @param argc The number of command-line arguments.
 * @param argv An array of strings representing the command-line arguments.
 * @return An Args structure containing the parsed arguments.
 */
struct Args parseArgs(int argc, char *argv[]);

#endif
