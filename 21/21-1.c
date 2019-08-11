#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

void abort() {
  sigset_t sigs;
  sigemptyset(&sigs);
  sigaddset(&sigs, SIGABRT);
  sigprocmask(SIG_UNBLOCK, &sigs, NULL);
  raise(SIGABRT);
  while(true) {}
}

int main() {
  abort();
  return 0;
}
