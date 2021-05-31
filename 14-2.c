#include <signal.h>
#include <stdio.h>

// Хендлер по примеру
void myHandler(int nsig) {
  if(nsig == SIGINT) {
    printf("CTRL + Снажать");
  }
  else if(nsig == SIGQUIT) {
    printf("CTRL + 4 нажать");
  }
}
int main(void) {
  (void)signal(SIGINT, myHandler);
  (void)signal(SIGQUIT, myHandler);
  while(1);
  return 0;
}