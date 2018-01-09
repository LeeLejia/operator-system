#include <sys/types.h>
#include <sys/msg.h>
#include <cstdlib>
#include <cstdio>
#include <zconf.h>

#define MSGKEY 75
struct msgform {
    long mtype;
    int id;
    char mtext[1000];
} msg;
int msgqid;

void client() {
    int i;
    /*打开75#消息队列*/
    msgqid = msgget(MSGKEY, 0777);
    for (i = 10; i >= 1; i--) {
        msg.mtype = i;
        msg.id = i;
        sprintf(msg.mtext,"hello!\n");
        printf("(client)sent!%d\n",i);
        msgsnd(msgqid, &msg, 1024, 0);     /*发送消息*/
    }
    exit(0);
}
void server() {
    msgqid = msgget(MSGKEY, 0777 | IPC_CREAT);  /*创建75#消息队列*/
    do {
        msgrcv(msgqid, &msg, 1030, 0, 0);   /*接收消息*/
        printf("[server]received:id=%d,msg=%s\n",msg.id,msg.mtext);
    } while (msg.mtype != 1);
    msgctl(msgqid, IPC_RMID, 0);  /*删除消息队列，归还资源*/
    exit(0);
}

int main() {
   if (fork()>0){
       if(fork()==0) {
           //client();
       }
       else{
           wait();
           wait();
           printf("ok,finish!\n");
       }
    } else{
       server();
   }
}
// Q:随机结果产生的原因？？
// 异步，有缓存？缓存大小？