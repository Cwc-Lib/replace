#include "files.h"
#include "utils.c"
#include "utils.h"
#include <dirent.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

// Generate a FilePathRule from the provided path, separating the target and
// prefix if present.
struct FilePathRule generateFilePathRule(char *path) {
  struct FilePathRule fpr;
  fpr.path = NULL;
  fpr.prefix = NULL;

  char *lastPrefixIndicator = strrchr(path, '*');

  if (lastPrefixIndicator == NULL) {
    fpr.path = strdup(path);
    return fpr;
  }

  fpr.path = strndup(path, lastPrefixIndicator - path);
  if (strlen(fpr.path) == 0) {
    fpr.path = NULL;
  }

  char *ptr = lastPrefixIndicator;
  while (*ptr != '\0') {
    if (*ptr != '*') {
      ptr++;
      continue;
    }
    memmove(ptr, ptr + 1, strlen(ptr));
  }
  fpr.prefix = lastPrefixIndicator;
  return fpr;
}

// Generate a list of FilePathRules from the provided list of paths.
// Essentially, this function converts an Args structure into an array of
// FilePathRules.
struct FilePathRule *generateFilePathRules(int pathsLen, char **paths) {
  if (pathsLen == 0 || paths == NULL) {
    struct FilePathRule *rules =
        (struct FilePathRule *)malloc(sizeof(struct FilePathRule));
    if (rules == NULL) {
      // TODO: implement errors.
      perror("Memory allocation error");
      exit(EXIT_FAILURE);
    }
    struct FilePathRule rule;
    rule.path = NULL;
    rule.prefix = NULL;
    rules[0] = rule;
    return rules;
  }

  struct FilePathRule *rules =
      (struct FilePathRule *)malloc(pathsLen * sizeof(struct FilePathRule));

  if (rules == NULL) {
    // TODO: implement errors.
    perror("Memory allocation error");
    return NULL;
  }

  for (int i = 0; i < pathsLen; i++) {
    rules[i] = generateFilePathRule(paths[i]);
  }

  return rules;
}

// Recursively collects files and directories under the specified base path
// (bp), based on the given prefix (pr) and stores them in the result array.
void collectFiles(const char *bp, const char *pr, char ***result, int *count) {
  DIR *dir;
  struct dirent *entry;

  dir = opendir(bp);
  if (!dir) {
    fprintf(stderr, "Unable to open directory -> %s", bp);
    return;
  }

  while ((entry = readdir(dir)) != NULL) {
    if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
      continue;
    }

    char *path = malloc(strlen(bp) + strlen(entry->d_name) + 2);
    if (!path) {
      perror("Memory allocation error");
      exit(EXIT_FAILURE);
    }

    sprintf(path, "%s/%s", bp, entry->d_name);

    if (entry->d_type == DT_DIR) {
      collectFiles(path, pr, result, count);
      continue;
    }

    if (pr == NULL || (pr && strstr(entry->d_name, pr) != NULL)) {
      (*result)[(*count)++] = path;
      *result = (char **)realloc(*result, sizeof(char *) * (*count + 1));
      if (!(*result)) {
        perror("Memory allocation error");
        exit(EXIT_FAILURE);
      }
    }
  }

  closedir(dir);
}

// Generates full paths based on an array of FilePathRule structures.
char **generateFullPaths(struct FilePathRule *rules, int rulesCount) {
  char **result = (char **)malloc(sizeof(char *));
  if (!result) {
    perror("Memory allocation error");
    exit(EXIT_FAILURE);
  }

  int totalCount = 0;
  for (int i = 0; i < rulesCount; i++) {
    struct FilePathRule rule = rules[i];

    char *cwd = getcwd(NULL, 0);
    if (!cwd) {
      perror("Unable to get current working directory");
      exit(EXIT_FAILURE);
    }

    if (rule.path == NULL || strcmp(rule.path, "") == 0) {
      rule.path = strdup(cwd);
    } else {
      rule.path = strdup(concatenateStrings(cwd, rule.path));
    }
    free(cwd);

    struct stat path_stat;
    if (stat(rule.path, &path_stat) == 0 && S_ISDIR(path_stat.st_mode)) {
      collectFiles(rule.path, rule.prefix, &result, &totalCount);
    } else {
      result[totalCount++] = strdup(rule.path);
      result = (char **)realloc(result, sizeof(char *) * (totalCount + 1));
      if (!result) {
        // TODO: implement errors.
        perror("Memory allocation error");
        exit(EXIT_FAILURE);
      }
    }
  }

  result[totalCount] = NULL;
  return result;
}
