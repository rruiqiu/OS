#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

void find(char *path, char *filename);

int
main(int argc, char *argv[])
{
  if(argc < 3){
    printf("please input the file name to find");
    exit(0);
  }
  char *path = argv[1];
  char *filename = argv[2];
  find(path,filename);
  exit(0);
}


void find(char *path, char *filename){
  char buf[512], *p;
  int fd;
  struct dirent de;
  struct stat st;
  if((fd = open(path, 0)) < 0){
    //open will create a fd to that specific path
    fprintf(2, "ls: cannot open %s\n", path);
    return;
  }

  if(fstat(fd, &st) < 0){
    fprintf(2, "ls: cannot stat %s\n", path);
    close(fd);
    return;
  }

  // read and the directory and place the read file into the de
  while(read(fd, &de, sizeof(de)) == sizeof(de)){
    //start - define the folder path and add the name to the path, put 0 as end string
    strcpy(buf, path);
    p = buf+strlen(buf);
    *p++ = '/';
    //if the directory is empty
    if(de.inum == 0)
      continue;
    memmove(p, de.name, DIRSIZ);
    p[DIRSIZ] = 0;
    if(stat(buf, &st) < 0){
      printf("find: cannot stat %s\n", buf);
      continue;
    }
    //end, buffer contains the name

    switch(st.type){
      case T_FILE:
        //if it is a file, compare with the input filename
        if(strcmp(filename,de.name) == 0){
          printf("%s\n",buf);
        }
        break;
      case T_DIR:
        //if the file is a directory, recursion

        //don't recurse into . and ..
        if ((strcmp(de.name, ".") != 0) && (strcmp(de.name, "..") != 0)){
          find(buf,filename);
        }

        break;
    }
  }
  close(fd);
}

// int
// main(int argc, char *argv[]){
//   //Write a simple version of the UNIX find program: find all the files in a directory tree with a specific name. Your solution should be in the file user/find.c.
//   /*
//   Some hints:
//   Look at user/ls.c to see how to read directories.
//   Use recursion to allow find to descend into sub-directories.
//   Don't recurse into "." and "..".
//   Changes to the file system persist across runs of qemu; to get a clean file system run make clean and then make qemu.
//   You'll need to use C strings. Have a look at K&R (the C book), for example Section 5.5.
//   Note that == does not compare strings like in Python. Use strcmp() instead.
//   Add the program to UPROGS in Makefile.
//   */

//   //recurse into subdirectory
//   //


//   exit(0);
// }