#include <grp.h>
#include <shadow.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

int initgroups(const char *user, gid_t group) {
  struct group *g;
  int num_groups = 1;
  setgrent();
  while ((g = getgrent()) != NULL) {
    for (char *member = *g->gr_mem; member != NULL; member++) {
      if (strcmp(member, user) == 0) {
        num_groups++;
        break;
      }
    }
  }
  gid_t *groups = malloc(num_groups);
  groups[0] = group;
  int group_index = 1;
  setgrent();
  while ((g = getgrent()) != NULL) {
    for (char *member = *g->gr_mem; member != NULL; member++) {
      if (strcmp(member, user) == 0) {
        groups[group_index++] = g->gr_gid;
        break;
      }
    }
  }
  endgrent();
  setgroups(num_groups, groups);
  free(groups);
  return 0;
}

int main() { return 0; }
