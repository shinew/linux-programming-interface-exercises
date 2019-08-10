#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int siginterrupt(int sig, int flag) {
  struct sigaction prev;
  int ret = sigaction(sig, NULL, &prev);
  if (ret != 0) {
    return ret;
  }
  if (flag) {
    prev.sa_flags |= SA_RESTART;
  } else {
    prev.sa_flags &= ~SA_RESTART;
  }
  sigaction(sig, &prev, NULL);
  return 0;
}

int main() { return 0; }
