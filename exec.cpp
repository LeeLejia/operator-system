#include<stdio.h>
#include<unistd.h>
#include <sys/wait.h>
#include <cstdlib>
int main()
{
    int pid;
    /*创建子进程*/
    pid=fork();
    switch(pid)
    {
        case  -1:
            /*创建失败*/
            printf("fork fail!\n");
            exit(1);
        case  0:
            /*子进程*/
            execl("/bin/ls","ls","-1","-color",NULL);
            /*子进程用exec( )装入命令ls后，代码被ls的代码取代
             * 这时子进程的PC指向ls的第1条语句，开始执行ls的命令代码。*/
            printf("exec fail!\n");
            exit(1);
        default:
            /*父进程*/
            /*同步*/
            wait(NULL);
            printf("ls completed !\n");
            exit(0);
    }
}
