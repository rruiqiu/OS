#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define READEND 0
#define WRITEEND 1


//Your goal is to use pipe and fork to set up the pipeline. The first process feeds the numbers 2 through 35 into the pipeline. For each prime number, you will arrange to create one process that reads from its left neighbor over a pipe and writes to its right neighbor over another pipe. Since xv6 has limited number of file descriptors and processes, the first process can stop at 35.

void child(int *p);

int main(){
  //recursion for every loop

  int p[2];
  pipe(p);
  int pid = fork();

  if(pid == 0){
    child(p);
  }else{
    //feed the number to the pipe
    close(p[READEND]);
    for(int i=2;i<=35;i++){
      write(p[WRITEEND],&i,sizeof(int));
    }
    close(p[WRITEEND]);
    wait((int *) 0);
  }
  exit(0);

}

void child(int *p){
  //print the first result
  int first_value;
  int read_result;
  close(p[WRITEEND]);
  int pr[2];
  pipe(pr);
  if(read(p[READEND],&first_value,sizeof(int)) == 0){ //read returns zero when the write-side of a pipe is closed.
    exit(0);
  }

  if(fork() == 0){
    child(pr);
  }else{
    close(pr[READEND]);
    printf("prime %d\n",first_value);
    while(read(p[READEND],&read_result,sizeof(int)) != 0){
      // printf("reading from readend\n");
      if(read_result % first_value != 0){
        write(pr[WRITEEND],&read_result,sizeof(int));
      }
    }

    close(pr[WRITEEND]);
    wait((int *) 0);
    exit(0);
  }
}