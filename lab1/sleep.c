#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  if(argc == 1){
    printf("Please input an argument for sleep");
    exit(1);
  }
  int sleepTime = atoi(argv[1]);
  //accept 1 argument string, convert it to int, then sleep for int s
  sleep(sleepTime);
  printf("(nothing happens for a little while)");
  exit(0);
}
