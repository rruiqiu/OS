#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/param.h"

// #include <string.h>
#define MAXARGUSIZE 100

int main(int argc, char *argv[]){
  if(argc < 2){
    printf("Please input at least 2 parameters.");
    exit(0);
  }
  char each_arg[MAXARGUSIZE];
  int counter = 0;
  // char argument[MAXARG][MAXARGUSIZE]; //define an array of string
  char buf;
  char *argument[MAXARG];
  for(int i = 1; i < argc;i++){
    argument[i-1] = argv[i];
    counter++;
  }
  int arg_counter = counter;
  int p=0;
  while(read(0,&buf,sizeof(buf)) > 0){ //loop until it reaches the end of line
    //construct the exec arguments
    // printf("%c",buf);
    if(buf == '\n'){ //the case when the arguments has a newline
      //do something that fork a child to complete the exec argument
      
      //need to append arg first
      each_arg[p] = 0;
      // argument[arg_counter] = each_arg;
      argument[arg_counter] = malloc((p)*sizeof(char));
      memcpy(argument[arg_counter],each_arg,p);

      // printf("%s",argument[arg_counter]);
      arg_counter++;
      p = 0;

      argument[arg_counter] = 0;
      //assign an end string to the arg_counter
      arg_counter = counter;  //reset the counter to the init position
      //
      if(fork() == 0){//child process

        exec(argument[0],argument);
        exit(0);
      }else{
        wait((int *) 0);
      }
    }else if(buf == ' '){
      //copy each argument to the argument array
      each_arg[p] = 0;
      argument[arg_counter] = malloc((p)*sizeof(char));
      memcpy(argument[arg_counter],each_arg,p);
      // argument[arg_counter] = each_arg;
      // printf("%s %d\n",argument[arg_counter],arg_counter);
      arg_counter++;
      p = 0;
    }else{
      //each_arg[p] use to record each argument
      each_arg[p] = buf;
      p++;
    }
  }

  for(int i = counter;i<MAXARG;i++){
    //free the allocated space
    if(argument[i]== 0 ){
      // printf("%d",i);
      break;
    }
    free(argument[i]);
  }

  exit(0);
}