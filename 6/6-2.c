#include <setjmp.h>
#include <stdio.h>

static jmp_buf env;

int foo() {
  setjmp(env);
  return 42;
}

int main() {
  int result = foo();
  printf("%d\n", result);
  longjmp(env, 1);
  return 0;
}
