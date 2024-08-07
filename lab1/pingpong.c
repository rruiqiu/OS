// #include "kernel/types.h"
// #include "kernel/stat.h"
// #include "user/user.h"

// #define READEND 0
// #define WRITEEND 1


// int
// main(int argc, char *argv[])
// {
//   int p1[2];
//   int p2[2];
//   pipe(p1);
//   pipe(p2);
//   int pid = fork();
//   char buffer[1];
//   int n = 1;
//   //The parent should send a byte to the child; the child should print "<pid>: received ping", where <pid> is its process ID, write the byte on the pipe to the parent, and exit;
//   // the parent should read the byte from the child, print "<pid>: received pong", and exit. Your solution should be in the file user/pingpong.c.

//   if(pid == 0){ //on child process
//     close(p1[WRITEEND]);
//     read(p1[READEND],buffer,n);
//     printf("%d: received ping\n",getpid());
//     close(p1[READEND]);
//     close(p2[READEND]);
//     write(p2[WRITEEND],buffer,n);
//     close(p2[WRITEEND]);

//   }else{
//     //first write to the byte then read it from the child
//     close(p1[READEND]);
//     write(p1[WRITEEND],"A",n);
//     close(p1[WRITEEND]);
//     close(p2[WRITEEND]);
//     read(p2[READEND],buffer,n);
//     printf("%d: received pong\n",getpid());
//     close(p2[READEND]);
//   }

//   exit(0);
// }

//another way to implment

#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char const *argv[])
{
	int pid;
	int p[2];
	pipe(p);

	if (fork() == 0) // child (receive -> send)
	{
		pid = getpid();
		char buf[2];
		if (read(p[0], buf, 1) != 1)
		{
			fprintf(2, "failed to read in child\n");
			exit(1);
		}
		close(p[0]);
		printf("%d: received ping\n", pid);
		if(write(p[1], buf, 1) != 1)
		{
			fprintf(2, "failed to write in child\n");
			exit(1);
		}
		close(p[1]);
		exit(0);
	}else{			// parent (send -> receive)
		pid = getpid();
		char info[2] = "a";
		char buf[2];
		buf[1] = 0;
		if (write(p[1], info, 1) != 1)
		{
			fprintf(2, "failed to write in parent\n");
			exit(1);
		}
		// wait for child to receive ping
		close(p[1]);
		if(read(p[0], buf, 1) != 1){
			fprintf(2, "failed to read in parent\n");
			exit(1);
		}
		printf("%d: received pong\n", pid);
		close(p[0]);
		exit(0);
	}
}