#include <zconf.h>
#include <cstdio>

int main() {
    int rs = fork();
    if(rs==0){
        while(true)
            printf("daughter  …");
    }else if(rs>0){
        rs = fork();
        if(rs==0){
            while(true)
                printf("son  …");
        }else if(rs>0){
            while(true)
                printf("parent  …");
        }else{
            printf("failed");
        }
    }else{
        printf("failed");
    }
    return 0;
}


/*
系统调用格式：
    pid=fork( )
返回值意义如下：
    0：返回值为0，表示当前进程是子进程。
    >0：返回值>0，返回值为子进程的id值（进程唯一标识符）。
    <0:创建失败
 */