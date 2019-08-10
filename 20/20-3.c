#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

void handler(int signal) {
  for (int i = 0; i < 1000000; i++) {
    printf("%d %d\n", i, signal);
  }
}

int main() {
  struct sigaction s;
  s.sa_handler = &handler;
  sigemptyset(&s.sa_mask);
  s.sa_flags = 0;
  /* s.sa_flags = SA_NODEFER; */
  /* s.sa_flags = SA_RESETHAND; */
  sigaction(SIGTERM, &s, NULL);
  pause();
  pause();
  return 0;
}
