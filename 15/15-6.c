#include <stdbool.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main(int __attribute__((unused)) argc, char *argv[]) {
  const char *file = argv[1];
  struct stat stats;
  stat(file, &stats);
  mode_t to_set = stats.st_mode;
  to_set |= S_IRUSR | S_IRGRP | S_IROTH;
  if ((stats.st_mode & S_IFMT) == S_IFDIR || (stats.st_mode & S_IXUSR) ||
      (stats.st_mode & S_IXGRP) || (stats.st_mode & S_IXOTH)) {
    to_set |= S_IXUSR | S_IXGRP | S_IXOTH;
  }
  chmod(file, to_set);
  return 0;
}
