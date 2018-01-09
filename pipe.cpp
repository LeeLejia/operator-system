#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <cstdlib>

int pid1, pid2;

int main() {
    int fd[2];
    char outpipe[100], inpipe[100];
    /*创建一个管道*/
    pipe(fd);
    while ((pid1 = fork()) == -1);
    if (pid1 == 0) {
        lockf(fd[1], 1, 0);
        /*把串放入数组outpipe中*/
        sprintf(outpipe, "child 1 process is sending message!");
        /*向管道写长为50字节的串*/
        write(fd[1], outpipe, 50);
        /*自我阻塞*/
        sleep(2);
        lockf(fd[1], 0, 0);
        exit(0);
    } else {
        while ((pid2 = fork()) == -1);
        if (pid2 == 0) {
            /*互斥*/
            lockf(fd[1], 1, 0);
            sprintf(outpipe, "child 2 process is sending message!");
            write(fd[1], outpipe, 50);
            sleep(2);
            lockf(fd[1], 0, 0);
            exit(0);
        } else {
            /*同步*/
            wait();
            /*从管道中读长为50字节的串*/
            read(fd[0], inpipe, 50);
            printf("read:%s\n", inpipe);
            wait();
            read(fd[0], inpipe, 50);
            printf("read:%s\n", inpipe);
            exit(0);
        }
    }
}

// sleep 会解除一个wait状态

/*
管道的类型：
    有名管道
    一个可以在文件系统中长期存在的、具有路径名的文件。用系统调用mknod( )建立。
    它克服无名管道使用上的局限性，可让更多的进程也能利用管道进行通信。
    因而其它进程可以知道它的存在，并能利用路径名来访问该文件。
    对有名管道的访问方式与访问其他文件一样，需先用open( )打开。
无名管道
    一个临时文件。利用pipe( )建立起来的无名文件（无路径名）。
    只用该系统调用所返回的文件描述符来标识该文件，故只有调用pipe( )的进程及其子孙进程才能识别此文件描述符
    才能利用该文件（管道）进行通信。当这些进程不再使用此管道时，核心收回其索引结点。
    二种管道的读写方式是相同的，本文只讲无名管道。


建立一无名管道
int  pipe(filedes);
参数定义
    int  filedes[2];
    其中，filedes[0]是读出端,filedes[1]是写入端。
该函数使用头文件如下：
    #include <unistd.h>
    #inlcude <signal.h>
    #include <stdio.h>

int  read(fd,buf,nbyte);
参数定义
 int  fd;
 功能：从fd所指示的文件中读出nbyte个字节的数据，并将它们送至由指针buf所指示的缓冲区中。如该文件被加锁，等待，直到锁打开为止。
 参数定义
    char *buf;
    unsigned  nbyte;
 read(fd,buf,nbyte)
    功能：把nbyte 个字节的数据，从buf所指向的缓冲区写到由fd所指向的文件中。如文件加锁，暂停写入，直至开锁。
    参数定义同read( )。
 */