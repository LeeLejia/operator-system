#include <ctime>
#include <cstdio>
#include <zconf.h>
#include <cstdlib>

int main() {
    time_t t; //建立time_t格式变量
    FILE *fp; //建立文件
    fp = fopen("/var/log/Mydaemon.log", "a"); //打开文件
    pid_t pid;                                //守护神
    pid = fork();
    if (pid > 0) {
        printf("Daemon on duty!\n");
        exit(0);
    } else if (pid < 0) {
        printf("Can't fork!\n");
        exit(-1);
    }
    while (1) {
        if (fp >= 0) {
            sleep(1);
            printf("Daemon on duty!\n");
            t = time(0);
            fprintf(fp, "The current time is %s\n", asctime(localtime(&t)));
        }
    }
}