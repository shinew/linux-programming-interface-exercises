#include <pwd.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>

struct passwd *getpwnam(const char *name) {
  setpwent();
  struct passwd *pw;
  while ((pw = getpwent()) != NULL) {
    if (strcmp(pw->pw_name, name) == 0) {
      break;
    }
  }
  endpwent();
  return pw;
}

int main() {
  struct passwd *me = getpwnam("shine");
  printf("uid = %d\n", me->pw_uid);
  return 0;
}
