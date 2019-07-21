#include <stdbool.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int euidaccess(const char *file, int mode) {
  struct stat file_stats;
  int err;
  if ((err = stat(file, &file_stats)) != 0) {
    return err;
  }
  uid_t euid = geteuid();
  gid_t eguid = getegid();
  if (euid == 0) {
    return 0;
  }

  const mode_t file_mode = file_stats.st_mode;
  if (euid == file_stats.st_uid) {
    if ((mode & R_OK) && !(S_IRUSR & file_mode)) {
      return -1;
    }
    if ((mode & X_OK) && !(S_IXUSR & file_mode)) {
      return -1;
    }
    if ((mode & W_OK) && !(S_IWUSR & file_mode)) {
      return -1;
    }
    return 0;
  }
  if (eguid == file_stats.st_gid) {
    if ((mode & R_OK) && !(S_IRGRP & file_mode)) {
      return -1;
    }
    if ((mode & X_OK) && !(S_IXGRP & file_mode)) {
      return -1;
    }
    if ((mode & W_OK) && !(S_IWGRP & file_mode)) {
      return -1;
    }
    return 0;
  }
  if ((mode & R_OK) && !(S_IROTH & file_mode)) {
    return -1;
  }
  if ((mode & X_OK) && !(S_IXOTH & file_mode)) {
    return -1;
  }
  if ((mode & W_OK) && !(S_IWOTH & file_mode)) {
    return -1;
  }
  return 0;
}

int main(int __attribute__((unused)) argc, char *argv[]) {
  const char *file = argv[1];
  printf("%d\n", euidaccess(file, F_OK | W_OK | R_OK | X_OK));
  return 0;
}
