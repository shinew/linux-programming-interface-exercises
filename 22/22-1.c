#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void handler(int sig) { printf("got signal %d\n", sig); }

int main() {
  sigset_t signals;
  sigemptyset(&signals);
  sigaddset(&signals, SIGCONT);
  sigprocmask(SIG_BLOCK, &signals, NULL);
  struct sigaction act;
  act.sa_flags = 0;
  sigset_t blocked;
  sigemptyset(&blocked);
  act.sa_mask = blocked;
  act.sa_flags = 0;
  act.sa_handler = handler;
  sigaction(SIGCONT, &act, NULL);
  sigaction(SIGUSR1, &act, NULL);
  pause();
  sigprocmask(SIG_UNBLOCK, &signals, NULL);
  pause();
  return 0;
}
