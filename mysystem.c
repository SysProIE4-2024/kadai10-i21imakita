/*
 *  mysystem.c : system 関数のクローン
 */

#include <stdio.h>
#include <stdlib.h>    // exit, system のため
#include <unistd.h>    // fork, execXX のため
#include <sys/wait.h>  // wait のため
#include "mysystem.h"  // インタフェース

// system関数のクローン
char *execpath="/bin/sh";
int mysystem(char *command) {
  int status = 0;
  
  // ここにプログラムを書く
  int pid;
  if(command==NULL) status=1;
  if((pid=fork())<0){
    status=-1;
  }
  if(pid==0){
    execl(execpath, "sh", "-c", command, NULL);
    perror(execpath);
    exit(127);
  } else {
    wait(&status);
  }
  return status;
}

/* 実行例

ここに実行例を書く
% make
cc -Wall -std=c99 -o mysysmain mysysmain.c  mysystem.c
 % ./mysysmain "ls -l README.md"
mysystem:
-rw-r--r--  1 imakitayuuma  staff  2795  7  4 09:45 README.md
retval = 00000000
system:
-rw-r--r--  1 imakitayuuma  staff  2795  7  4 09:45 README.md
retval = 00000000
 % ./mysysmain ls -l README.md  
使い方 : ./mysysmain コマンド文字列
 % ./mysysmain "cat a"          
mysystem:
cat: a: No such file or directory
retval = 00000100
system:
cat: a: No such file or directory
retval = 00000100
*/
