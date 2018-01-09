#include <sys/types.h>
#include <sys/shm.h>
#include <cstdio>
#include <cstdlib>
#include <zconf.h>

#define  SHMKEY  75
int  shmid,i;
int  *addr;

void client() {
    int i;
    /*打开共享存储区*/
    shmid=shmget(SHMKEY,1024,0777);
    /*获得共享存储区首地址*/
    addr= static_cast<int *>(shmat(shmid, 0, 0));
    for (i=9;i>=0;i--) {
        /*等读*/
        while (*addr!=-1);
        /*往该地址写入数据*/
        sprintf(reinterpret_cast<char *>(addr), "hello!%d\n",i);
        printf("(client) sent:%d\n",i);
        /*通知可读*/
        *addr=i;
    }
    exit(0);
}

void server()
{
    shmid=shmget(SHMKEY,1024,0777|IPC_CREAT);       /*创建共享存储区*/
    addr= static_cast<int *>(shmat(shmid, 0, 0));   /*获取首地址*/
    do {
        *addr=-1;
        /*等写*/
        while (*addr==-1);/*往该地址读出数据*/
            printf("(server) received:%d\n",*addr);
    }while (*addr);
    /*撤消共享存储区，归还资源*/
    shmctl(shmid,IPC_RMID,0);
    exit(0);
}

int main()
{
    while ((i=fork())==-1);
    if (!i) server(); /*子进程1 服务*/
//    system("ipcs  -m");
    while ((i=fork())==-1);
    if (!i) client(); /*子进程2 客户端*/
    wait();
    wait();
    return 0;
}

/*Q：打印两次？？*/
/*
 Q:发现每当client发送一次数据后，server要等待大约0.1秒才有响应。同样，之后client又需要等待大约0.1秒才发送下一个数据。

 A: 出现上述应答延迟的现象是程序设计的问题。
    当client端发送了数据后，并没有任何措施通知server端数据已经发出，需要由client的查询才能感知。
    此时，client端并没有放弃系统的控制权，仍然占用CPU的时间片。
    只有当系统进行调度时，切换到了server进程，再进行应答。
    这个问题，也同样存在于server端到client的应答过程中。
 */

/*
 F:
    比较两种消息通信机制中数据传输的时间
    由于两种机制实现的机理和用处都不一样，难以直接进行时间上的比较。如果比较其性能，应更加全面的分析。
    （1）消息队列的建立比共享区的设立消耗的资源少。
        前者只是一个软件上设定的问题，后者需要对硬件的操作，实现内存的映像，当然控制起来比前者复杂。
        如果每次都重新进行队列或共享的建立，共享区的设立没有什么优势。
    （2）当消息队列和共享区建立好后，共享区的数据传输，受到了系统硬件的支持，不耗费多余的资源；
        而消息传递，由软件进行控制和实现，需要消耗一定的cpu的资源。
        从这个意义上讲，共享区更适合频繁和大量的数据传输。
    （3）消息的传递，自身就带有同步的控制。当等到消息的时候，进程进入睡眠状态，不再消耗cpu资源。
        而共享队列如果不借助其他机制进行同步，接收数据的一方必须进行不断的查询，白白浪费了大量的cpu资源。
        可见，消息方式的使用更加灵活。
 */

/**
创建、获得一个共享存储区。
系统调用格式：
    int  shmget(key,size,flag);
该函数使用头文件如下：
    #include<sys/types.h>
    #include<sys/ipc.h>
    #include<sys/shm.h>
参数定义
    key_t  key;
    int  size,flag;
    其中，key是共享存储区的名字；size是其大小（以字节计）；flag是用户设置的标志，如IPC_CREAT。
    IPC_CREAT表示若系统中尚无指名的共享存储区，则由核心建立一个共享存储区；若系统中已有共享存储区，便忽略IPC_CREAT。
    附：
        操作允许权                  八进制数
         用户可读                     00400
         用户可写                     00200
         小组可读                     00040
         小组可写                     00020
         其它可读                     00004
         其它可写                     00002

    控制命令                    值
    IPC_CREAT                0001000
    IPC_EXCL                 0002000
    例：shmid=shmget(key,size,(IPC_CREAT|0400))
       创建一个关键字为key，长度为size的共享存储区

共享存储区的附接。从逻辑上将一个共享存储区附接到进程的虚拟地址空间上。
系统调用格式：
    char* shmat(shmid,addr,flag);
该函数使用头文件如下：
    #include<sys/types.h>
    #include<sys/ipc.h>
    #include<sys/shm.h>
参数定义
         int  shmid,flag;
         char  * addr;
        其中，shmid是共享存储区的标识符；addr是用户给定的，将共享存储区附接到进程的虚地址空间；
        flag规定共享存储区的读、写权限，以及系统是否应对用户规定的地址做舍入操作。
        其值为SHM_RDONLY时，表示只能读；其值为0时，表示可读、可写；其值为SHM_RND（取整）时，表示操作系统在必要时舍去这个地址。
        该系统调用的返回值是共享存储区所附接到的进程虚地址viraddr。

把一个共享存储区从指定进程的虚地址空间断开。
系统调用格式：
    int  shmdt(addr);
该函数使用头文件如下：
    #include<sys/types.h>
    #include<sys/ipc.h>
    #include<sys/shm.h>
参数定义
    char  addr;
    其中，addr是要断开连接的虚地址，亦即以前由连接的系统调用shmat( )所返回的虚地址。调用成功时，返回0值，调用不成功，返回-1。


共享存储区的控制，对其状态信息进行读取和修改。
系统调用格式：
    int  shmctl(shmid,cmd,buf);
该函数使用头文件如下：
    #include<sys/types.h>
    #include<sys/ipc.h>
    #include<sys/shm.h>
参数定义
    int  shmid,cmd;
    struct  shmid_ds  *buf;
    其中，buf是用户缓冲区地址，cmd是操作命令。命令可分为多种类型：
    （1）用于查询有关共享存储区的情况。如其长度、当前连接的进程数、共享区的创建者标识符等；
    （2）用于设置或改变共享存储区的属性。如共享存储区的许可权、当前连接的进程计数等；
    （3）对共享存储区的加锁和解锁命令；
    （4）删除共享存储区标识符等。
    上述的查询是将shmid所指示的数据结构中的有关成员，放入所指示的缓冲区中；而设置是用由buf所指示的缓冲区内容来设置由shmid所指示的数据结构中的相应成员。
 */