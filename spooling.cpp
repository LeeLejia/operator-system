#include<stdlib.h>
#include<iostream>
#include<ctime>


using namespace std;

int *createFile(int len);

void request(int i);//i=1 表示用户进程1;i=2 表示用户进程2
void spooling();
void printBuffer(int pid);

enum PCD_STATUS {
    //可执行态、等待状态1（表示请求输出块用完）、为等待状态2（表示输出井空，SPOOLING输出进程等待）、结束态（进程执行完成）
            RUNNABLE, FULL_WAIT, EMPTY_WAIT, FINISH
};
// 进程控制块
struct pcb {
    int id;          /* 进程标识数 */
    int status;      /* 进程状态 */
} *PCB[3];
// 请求输出块
struct Reqblock {
    int reqname;         /* 请求进程名 */
    int length;          /* 本次输出信息长度，每个进程一个文件最多可占用输出井20个位置 */
    int addr;            /* 文件地址，标记在输出井中的下标 */
} reqblock[10];
// 将reqblock看成环形数组
int n_in = 0, n_out = 0;
// 两个进程需要输出的文件数量
int toOutCount[2] = {5, 5};

// 输出井
struct Buffer {
    int buffer[30];
    int end;
    int length;
} buffer[2];

// 调试的时候打印输出井中的缓存数据
int debug = false;

int main() {
    // 初始化pcb
    for (int i = 0; i < 3; i++) {
        PCB[i] = (pcb *) malloc(sizeof(pcb));
        PCB[i]->status = RUNNABLE;
        PCB[i]->id = i + 1;
    }
    srand((unsigned) time(NULL));
    while (true) {
        int r;
        r = rand() % 100;
        if (r <= 40 && toOutCount[0] > 0 && PCB[0]->status == RUNNABLE) {
            request(1);
            cout<<"转进程调度."<<endl;
        } else if (r > 40 && r <= 80 && toOutCount[1] > 0 && PCB[1]->status == RUNNABLE) {
            request(2);
            cout<<"转进程调度."<<endl;
        } else {
            spooling();
            cout<<"转进程调度."<<endl;
        }
        // 完成后跳出
        if ((toOutCount[0] == 0) && (toOutCount[1] == 0) && (n_in == n_out))
            break;
    }
    cout << "任务完成,作业进程进入FINISH完成状态." << endl;
}

void request(int pid) {
    if (n_in - n_out >= 10) {
        // 输出块用完,请求输出进程等待
        PCB[pid - 1]->status = FULL_WAIT;
        cout<<"请求输出块用完,进程"<<pid<<"进入FULL_WAIT等待状态."<<endl;
        return;
    }
    Reqblock *rq = &reqblock[n_in % 10];
    rq->reqname = pid;
    rq->length = rand() % 20 + 1;
    rq->addr = -1;// 标记在输出井中的下标
    // 开始计算内存，如果内存足够则拷贝数据到输出井
    if (30 - buffer[pid - 1].length >= rq->length) {
        rq->addr = buffer[pid - 1].end;
        int *data = createFile(rq->length);
        //开始拷贝
        printBuffer(pid);
        for (int ii = 0; ii < rq->length; ii++) {
            buffer[pid - 1].buffer[(buffer[pid - 1].end + ii) % 30] = data[ii];
        }
        buffer[pid - 1].end = (buffer[pid - 1].end + rq->length) % 30;
        buffer[pid - 1].length += rq->length;
        printBuffer(pid);
    }
    if (rq->addr != -1) {
        n_in++;
        if (pid == 1) {
            toOutCount[0]--;
        } else {
            toOutCount[1]--;
        }
        cout<<"进程"<<pid<<"将一个文件写入缓存."<<endl;
    } else {
        // 输出井为空（不够）
        PCB[pid - 1]->status = EMPTY_WAIT;
        cout<<"空余输出井为空,spooling处理进程进入EMPTY_WAIT等待状态"<<endl;
    }
}

void spooling() {
    if (n_out >= n_in) {
        cout << "spooling暂无任务！" << endl;
        return;
    }
    Reqblock *run = &reqblock[n_out % 10];
    cout << "spooling打印进程" << run->reqname << " | ";
    for (int i = 0; i < run->length; i++) {
        cout << buffer[run->reqname - 1].buffer[(run->addr + i) % 30] << " ";
        buffer[run->reqname - 1].buffer[(run->addr + i) % 30] = 0;
    }
    buffer[run->reqname - 1].length -= run->length;
    cout << endl;
    n_out++;
    for (int j = 0; j < 2; j++) {
        if (PCB[j]->status != RUNNABLE) {
            PCB[j]->status = RUNNABLE;
            cout<<"进程"<<j+1<<"进入可执行状态."<<endl;
        }
    }
}

/**
 * 生成文件
 * @param len 输出的字符串长度
 * @return 输出以0结尾的数字字符串
 */
int *createFile(int len) {
    // 每个要输出的文件内容为随机产生的数字0~9之间的一序列数，且0作为文件结束标志。
    // 当随机产生的数为0时，本次输出的文件结束，就形成一个请求输出信息块，申请并填入请求输出信息块reqblock结构中。
    int *data = static_cast<int *>(malloc(sizeof(int) * len));
    if(debug)
        cout << "raw" << " | ";
    for (int i = 0; i < len; i++) {
        int r = rand() % 9 + 1;
        data[i] = r;
        if(debug)
            cout << data[i];
    }
    if(debug)
        cout << endl;
    return data;
}
// 打印缓存
void printBuffer(int pid) {
    if(!debug)
        return;
    cout << "buffer[" << pid << "]: ";
    for (int i = 0; i < 30; i++) {
        cout << buffer[pid - 1].buffer[i];
    }
    cout << endl;
}