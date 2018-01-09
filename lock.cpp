#include <stdio.h>
#include <unistd.h>

int main() {
    int p1, p2, i;
    while ((p1 = fork()) == -1);
    if (p1 == 0) {
        /*加锁，这里第一个参数为stdout（标准输出设备的描述符）*/
        lockf(1, 1, 0);
        for (i = 0; i < 3; i++){
            printf("daughter %d\n", i);
            sleep(1);
        }
        lockf(1, 0, 0);/*解锁*/
    } else {
        while ((p2 = fork()) == -1);/*创建子进程p2*/
        if (p2 == 0) {
            lockf(1, 1, 0); /*加锁*/
            for (i = 0; i < 3; i++){
                printf("son %d\n", i);
                sleep(1);
            }
            lockf(1, 0, 0);/*解锁*/
        } else {
            lockf(1, 1, 0);/*加锁*/
            for (i = 0; i < 3; i++){
                printf(" parent %d\n", i);
                sleep(1);
            }
            lockf(1, 0, 0); /*解锁*/
        }
    }
}
// 进程结束自动解锁？？
// 上述程序执行时，不同进程之间不存在共享临界资源（其中打印机的互斥性已由操作系统保证）问题，所以加锁与不加锁效果相同。

/**
lockf(files,function,size):
    用作锁定文件的某些段或者整个文件。

头文件:
    #include "unistd.h"
        参数定义：
参数：
    int  lockf(files,function,size)
    int  files,function;
    long  size;
其中：
     files是文件描述符；
     function是锁定和解锁：1表示锁定，0表示解锁。
     size是锁定或解锁的字节数，0表示从文件的当前位置到文件尾。
 */


