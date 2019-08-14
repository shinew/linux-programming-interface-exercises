#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void handler(int sig) { printf("got signal %d\n", sig); }

int main() {
  struct sigaction act;
  act.sa_flags = 0;
  sigset_t empty;
  sigemptyset(&empty);
  act.sa_mask = empty;
  act.sa_flags = 0;
  act.sa_handler = handler;
  sigaction(SIGUSR1, &act, NULL);
  sigaction(SIGRTMIN, &act, NULL);
  sigset_t blocked;
  sigemptyset(&blocked);
  sigaddset(&blocked, SIGUSR1);
  sigaddset(&blocked, SIGRTMIN);
  sigprocmask(SIG_BLOCK, &blocked, NULL);
  int foo;
  scanf("%d", &foo);
  sigprocmask(SIG_UNBLOCK, &blocked, NULL);
  return 0;
}
