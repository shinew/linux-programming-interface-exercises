#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int setenv(const char *name, const char *value, int overwrite) {
  if (overwrite == 0 && getenv(name) != NULL) {
    return 0;
  }
  int name_len = strlen(name);
  int value_len = strlen(value);
  int total_len_with_equal_and_null = name_len + value_len + 2;
  char *buffer = malloc(total_len_with_equal_and_null);
  memcpy(buffer, name, name_len);
  memcpy(buffer + name_len, "=", 1);
  memcpy(buffer + name_len + 1, value, value_len);
  memcpy(buffer + name_len + 1 + value_len, "\0", 1);
  putenv(buffer);
  return 0;
}

extern char **environ;
bool has_name(const char *env_ptr, const char *name) {
  int name_len = strlen(name);
  for (int i = 0; i < name_len; i++) {
    if (name[i] != env_ptr[i]) {
      return false;
    }
  }
  return env_ptr[name_len] == '=';
}

int unsetenv(const char *name) {
  for (char **env_ptr = environ; *env_ptr != NULL; env_ptr++) {
    if (has_name(*env_ptr, name)) {
      *env_ptr = NULL;
    }
  }
  return true;
}

int main() {
  const char *name = "shine";
  setenv(name, "wang", 0);
  setenv(name, "huang", 0);
  printf("%s\n", getenv(name));
  setenv(name, "lol", 1);
  printf("%s\n", getenv(name));
  unsetenv(name);
  printf("%s\n", getenv(name));
  unsetenv(name);
  printf("%s\n", getenv(name));
  setenv(name, "lol", 0);
  printf("%s\n", getenv(name));
  return 0;
}
