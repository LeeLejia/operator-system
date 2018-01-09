#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <cstdlib>

void waiting(), stop();

int wait_mark;

int main() {
    int p1, p2, stdout;
    while ((p1 = fork()) == -1);
    if (p1 > 0) {
        while ((p2 = fork()) == -1);
        if (p2 > 0) {
            wait_mark = 1;
            /*接收到^c信号，转stop*/
            signal(SIGINT, reinterpret_cast<__sighandler_t>(stop));
            waiting();
            kill(p1, 16);        /*向p1发软中断信号16*/
            kill(p2, 17);        /*向p2发软中断信号17*/
            wait(0);           /*同步*/
            wait(0);
            printf("Parent process is killed!\n");
            exit(0);
        } else {
            wait_mark = 1;
            signal(17, reinterpret_cast<__sighandler_t>(stop));   /*接收到软中断信号17，转stop*/
            waiting();
            lockf(stdout, 1, 0);
            printf("Child process 2 is killed by parent!\n");
            lockf(stdout, 0, 0);
            exit(0);
        }
    } else {
        wait_mark = 1;
        signal(16, reinterpret_cast<__sighandler_t>(stop));        /*接收到软中断信号16，转stop*/
        waiting();
        lockf(stdout, 1, 0);
        printf("Child process 1 is killed by parent!\n");
        lockf(stdout, 0, 0);
        exit(0);
    }
}

void waiting() {
    while (wait_mark != 0);
}

void stop() {
    wait_mark = 0;
}
// 不打印子进程的内容，因为^c同时会终止子进程活动。
/*
int  kill(pid,sig)
参数定义
    int  pid,sig;
    其中，pid是一个或一组进程的标识符，参数sig是要发送的软中断信号。
    （1）pid>0时，核心将信号发送给进程pid。
    （2）pid=0时，核心将信号发送给与发送进程同组的所有进程。
    （3）pid=-1时，核心将信号发送给所有用户标识符真正等于发送进程的有效用户标识号的进程。


预置对信号的处理方式，允许调用进程控制软中断信号。
系统调用格式
    signal(sig,function)
头文件
    #include <signal.h>
参数定义
    int  sig;
    void (*func) ()

    其中sig用于指定信号的类型，sig为0则表示没有收到任何信号，余者如下表：
        值	名  字	说          明
        01	SIGHUP	挂起（hangup）
        02	SIGINT	中断，当用户从键盘按^c键或^break键时
        03	SIGQUIT	退出，当用户从键盘按quit键时
        04	SIGILL	非法指令
        05	SIGTRAP	跟踪陷阱（trace trap），启动进程，跟踪代码的执行
        06	SIGIOT	IOT指令
        07	SIGEMT	EMT指令
        08	SIGFPE	浮点运算溢出
        09	SIGKILL	杀死、终止进程
        10	SIGBUS	总线错误
        11	SIGSEGV	段违例（segmentation  violation），进程试图去访问其虚地址空间以外的位置
        12	SIGSYS	系统调用中参数错，如系统调用号非法
        13	SIGPIPE	向某个非读管道中写入数据
        14	SIGALRM	闹钟。当某进程希望在某时间后接收信号时发此信号
        15	SIGTERM	软件终止（software  termination）
        16	SIGUSR1	用户自定义信号1
        17	SIGUSR2	用户自定义信号2
        18	SIGCLD	某个子进程死
        19	SIGPWR	电源故障
    function：在该进程中的一个函数地址，在核心返回用户态时，它以软中断信号的序号作为参数调用该函数，对除了信号SIGKILL，SIGTRAP和SIGPWR以外的信号，核心自动地重新设置软中断信号处理程序的值为SIG_DFL，一个进程不能捕获SIGKILL信号。
        function 的解释如下：
        （1）function=1时，进程对sig类信号不予理睬，亦即屏蔽了该类信号；
        （2）function=0时，缺省值，进程在收到sig信号后应终止自己；
        （3）function为非0，非1类整数时，function的值即作为信号处理程序的指针。
*/