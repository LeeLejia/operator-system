#include <zconf.h>
#include <cstdio>
#include <wait.h>

int main(){
   int rs = fork();
   switch (rs){
       case 0:
           printf("1\n");
           break;
       case -1:
           printf("failed!");
           return -1;
       default:
           rs = fork();
           if(rs<0){
               printf("failed!");
               return -1;
           }
           if(rs==0){
               wait(0);
               printf("2\n");
           }else{
               wait(0);
               wait(0);
               printf("3\n");
           }
           break;
   }
    return 0;
}

/**
系统调用格式：
    int  wait(status)　
    int  *status;
    其中，status是用户空间的地址。它的低8位反应子进程状态，为0表示子进程正常结束，非0则表示出现了各种各样的问题；高8位则带回了exit( )的返回值。exit( )返回值由系统给出。
    核心对wait( )作以下处理：
        （1）首先查找调用进程是否有子进程，若无，则返回出错码；
        （2）若找到一处于“僵死状态”的子进程，则将子进程的执行时间加到父进程的执行时间上，并释放子进程的进程表项；
        （3）若未找到处于“僵死状态”的子进程，则调用进程便在可被中断的优先级上睡眠，等待其子进程发来软中断信号时被唤醒。
 等待子进程运行结束。如果子进程没有完成，父进程一直等待。
 wait( )将调用进程挂起，直至其子进程因暂停或终止而发来软中断信号为止。
 如果在wait( )前已有子进程暂停或终止，则调用进程做适当处理后便返回。
*/