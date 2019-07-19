#include <dirent.h>
#include <fcntl.h>
#include <fstream>
#include <iostream>
#include <pwd.h>
#include <string>
#include <vector>

using namespace std;

void list_processes_of(const char *username) {
  struct passwd *user = getpwnam(username);
  uid_t uid = user->pw_uid;

  DIR *proc = opendir("/proc");
  struct dirent *dir_entry;
  while ((dir_entry = readdir(proc)) != NULL) {
    string status_file = "/proc/";
    try {
      int pid = stoi(dir_entry->d_name);
      if (pid <= 0) {
        continue;
      }
      status_file += dir_entry->d_name;
      status_file += "/status";
      ifstream contents(status_file);
      if (contents.failbit) {
        continue;
      }
      string name;
      vector<uid_t> uids;
      for (string line; getline(contents, line);) {
        line.spli
      }
    } catch (const invalid_argument &_) {
    }

    closedir(proc);
  }
}

int main() {
  list_processes_of("shine");
  return 0;
}
