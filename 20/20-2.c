#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main() {
  struct sigaction s;
  s.sa_handler = SIG_IGN;
  sigemptyset(&s.sa_mask);
  s.sa_flags = 0;
  sigaction(SIGTERM, &s, NULL);
  pause();
  return 0;
}
