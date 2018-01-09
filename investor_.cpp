/**
 * 银行家算法
 */
#include <cstdio>

/*资源链*/
struct SourceLink {
    int count;
    SourceLink *next;

    SourceLink() {
        count = 0;
        next = nullptr;
    }
};

/*进程*/
struct Process {
    // 进程号码
    int pid;
    // 占有的资源
    SourceLink *hold;
    // 需要的资源
    SourceLink *need;

    // 是否需要资源
    bool ok;

    Process() {
        pid = -1;
        hold = nullptr;
        need = nullptr;
        ok=false;
    }
};

/*进程链*/
struct ProcessLink {
    Process value;
    ProcessLink *next;

    ProcessLink() {
        next = nullptr;
    }
};
/*系统剩余*/
SourceLink * sysAvailable;
/*系统进程*/
ProcessLink *p;

int main() {
    int pcount, sourceType;
    printf("请输入线程数量：");
    scanf("%d", &pcount);
    if (pcount <= 0) {
        printf("输入错误，程序退出！");
        return -1;
    }
    printf("请输入系统资源种类数目：");
    if (pcount <= 0) {
        printf("输入错误，程序退出！");
        return -1;
    }
    scanf("%d", &sourceType);
    p = new ProcessLink;
    ProcessLink *ptmp = p;
    SourceLink *stmp;
    for (int i = 0; i < pcount; i++, ptmp = ptmp->next) {
        ptmp->next = new ProcessLink;
        ptmp->value.pid = i;
        printf("请依次输入%d个数值，代表P%d进程需要的资源数量:\n", sourceType, i);
        stmp = ptmp->value.need = new SourceLink;
        ptmp->value.ok=true;
        for (int j = 0, s = 0; j < sourceType; j++, stmp = stmp->next) {
            scanf("%d", &s);
            stmp->next = new SourceLink;
            stmp->count = s;
            if(s>0){
                ptmp->value.ok=false;
            }
        }
        printf("请依次输入%d个数值，代表P%d进程占有的资源数量:\n", sourceType, i);
        stmp = ptmp->value.hold = new SourceLink;
        for (int j = 0, s = 0; j < sourceType; j++, stmp = stmp->next) {
            scanf("%d", &s);
            stmp->next = new SourceLink;
            stmp->count = s;
        }
    }
    printf("请依次输入%d个数值，代表操作系统剩下的资源数量:\n", sourceType);
    stmp = sysAvailable = new SourceLink;
    for (int j = 0, s = 0; j < sourceType; j++, stmp = stmp->next) {
        scanf("%d", &s);
        stmp->next = new SourceLink;
        stmp->count = s;
    }
    int que[pcount],rsindex=0;
    // 计算序列
    while(true){
        bool can_assign,has_assign=false,assign_ok=true;
        ProcessLink *processLink=p;
        for(int j = 0;j<pcount;j++,processLink=processLink->next){
            // 检测改进程是否已经分配完毕
            if(processLink->value.ok)
                continue;
            SourceLink * syss = sysAvailable, *psneed=processLink->value.need, *pshold;
            can_assign = true;
            // 但前进程能否分配
            for(int i=0;i<sourceType;i++,psneed=psneed->next,syss=syss->next){
                if(psneed->count>syss->count){
                    can_assign=false;
                    break;
                }
            }
            if(can_assign){
                // 当前进程可以分配
                syss = sysAvailable;
                pshold=processLink->value.hold;
                psneed =processLink->value.need;
                for(int i=0;i<sourceType;i++,pshold=pshold->next,syss=syss->next,psneed=psneed->next){
                    syss->count+=pshold->count;
                    pshold->count=0;
                    psneed->count=0;
                    processLink->value.ok=true;
                }
                que[rsindex]=j;
                rsindex++;
                // 已分配完毕一个进程
                has_assign=true;
                break;
            }
        }
        // 检测是否完成全部分配
        processLink=p;
        for(int j = 0;j<pcount;j++,processLink=processLink->next){
           if(!processLink->value.ok){
               assign_ok=false;
               break;
           }
        }
        // 不可分配，并且未满足全部需求
        if(!has_assign && !assign_ok){
            printf("不存在安全序列！");
            return 0;
        }
        if(assign_ok){
            printf("已完成分配，安全序列为：");
            for(int i=0;i<rsindex;i++){
                printf("P%d ",que[i]);
            }
            printf("\n");
            return 0;
        }
    }
}