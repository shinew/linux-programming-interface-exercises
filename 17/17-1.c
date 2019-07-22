#include <acl/libacl.h>
#include <grp.h>
#include <pwd.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <sys/acl.h>
#include <sys/stat.h>
#include <sys/types.h>

bool get_mask(acl_t acl, acl_permset_t *perms) {
  int entryId;
  for (entryId = ACL_FIRST_ENTRY;; entryId = ACL_NEXT_ENTRY) {
    acl_entry_t entry;
    if ((acl_get_entry(acl, entryId, &entry)) != 1) {
      break;
    }
    acl_tag_t type;
    acl_get_tag_type(entry, &type);
    if (type == ACL_MASK) {
      acl_get_permset(entry, perms);
      return true;
    }
  }
  return false;
}

void get_file_owner_and_group(const char *file, uid_t *owner, gid_t *group) {
  struct stat stats;
  stat(file, &stats);
  *owner = stats.st_uid;
  *group = stats.st_gid;
}

void print_perms(acl_permset_t perms, acl_permset_t *mask_perms) {
  if (mask_perms == NULL) {
    char read = acl_get_perm(perms, ACL_READ) ? 'r' : '-';
    char write = acl_get_perm(perms, ACL_WRITE) ? 'w' : '-';
    char execute = acl_get_perm(perms, ACL_EXECUTE) ? 'x' : '-';
    printf("%c%c%c\n", read, write, execute);
  } else {
    char read =
        acl_get_perm(perms, ACL_READ) & acl_get_perm(*mask_perms, ACL_READ)
            ? 'r'
            : '-';
    char write =
        acl_get_perm(perms, ACL_WRITE) & acl_get_perm(*mask_perms, ACL_WRITE)
            ? 'w'
            : '-';
    char execute = acl_get_perm(perms, ACL_EXECUTE) &
                           acl_get_perm(*mask_perms, ACL_EXECUTE)
                       ? 'x'
                       : '-';
    printf("%c%c%c\n", read, write, execute);
  }
}

int main(int argc, char *argv[]) {
  if (argc < 4) {
    printf("wrong # of args");
    return 1;
  }

  const char *user_or_group = argv[1];
  bool user_else_group = strcmp(user_or_group, "u") == 0;
  const char *name = argv[2];
  uid_t user;
  gid_t group;
  if (user_else_group) {
    user = getpwnam(name)->pw_uid;
  } else {
    group = getgrnam(name)->gr_gid;
  }

  const char *file = argv[3];
  uid_t file_owner;
  gid_t file_group;
  get_file_owner_and_group(file, &file_owner, &file_group);

  const acl_t acl = acl_get_file(file, ACL_TYPE_ACCESS);

  acl_permset_t mask_perms;
  bool has_mask = get_mask(acl, &mask_perms);
  acl_permset_t *mask = has_mask ? &mask_perms : NULL;

  int entryId;
  for (entryId = ACL_FIRST_ENTRY;; entryId = ACL_NEXT_ENTRY) {
    acl_entry_t entry;
    if ((acl_get_entry(acl, entryId, &entry)) != 1) {
      break;
    }
    acl_tag_t tag;
    acl_get_tag_type(entry, &tag);

    acl_permset_t perms;
    uid_t *this_user;
    gid_t *this_group;
    switch (tag) {
    case ACL_USER_OBJ:
      if (user_else_group && file_owner == user) {
        acl_get_permset(entry, &perms);
        print_perms(perms, NULL);
        return 0;
      }
      break;
    case ACL_USER:
      this_user = acl_get_qualifier(entry);
      if (user_else_group && (*this_user == user)) {
        acl_get_permset(entry, &perms);
        print_perms(perms, mask);
        return 0;
      }
      break;
    case ACL_GROUP_OBJ:
      if (!user_else_group && file_group == group) {
        acl_get_permset(entry, &perms);
        print_perms(perms, mask);
        return 0;
      }
      break;
    case ACL_GROUP:
      this_group = acl_get_qualifier(entry);
      if (!user_else_group && *this_group == group) {
        acl_get_permset(entry, &perms);
        print_perms(perms, mask);
        return 0;
      }
      break;
    }
  }
  return 0;
}
