/* 银行家算法，操作系统概念(OS concepts Six Edition)
reedit by Johnny hagen，SCAU，run at vc6.0
*/
#include <cstdio>
#include <cstdlib>
#include "malloc.h"

#define alloclen sizeof(struct allocation)
#define maxlen sizeof(struct max)
#define avalen sizeof(struct available)
#define needlen sizeof(struct need)
#define finilen sizeof(struct finish)
#define pathlen sizeof(struct path)
struct allocation {
    int value;
    struct allocation *next;
};
struct max {
    int value;
    struct max *next;
};
struct available /*可用资源数*/
{
    int value;
    struct available *next;
};
struct need /*需求资源数*/
{
    int value;
    struct need *next;
};
struct path {
    int value;
    struct path *next;
};
struct finish {
    int stat;
    struct finish *next;
};

int main() {
    int row, colum, status = 0, i, j, t, temp, processtest;
    struct allocation *allochead, *alloc1, *alloc2, *alloctemp;
    struct max *maxhead, *maxium1, *maxium2, *maxtemp;
    struct available *avahead, *available1, *available2, *workhead, *work1, *work2, *worktemp, *worktemp1;
    struct need *needhead, *need1, *need2, *needtemp;
    struct finish *finihead, *finish1, *finish2, *finishtemp;
    struct path *pathhead, *path1, *path2;
    printf("\n请输入系统资源的种类数:");
    scanf("%d", &colum);
    printf("请输入现时内存中的进程数:");
    scanf("%d", &row);
    printf("请输入已分配资源矩阵:\n");
    for (i = 0; i < row; i++) {
        for (j = 0; j < colum; j++) {
            printf("请输入已分配给进程 p%d 的 %c 种系统资源:", i, 'A' + j);
            if (status == 0) {
                allochead = alloc1 = alloc2 = (struct allocation *) malloc(alloclen);
                alloc1->next = alloc2->next = NULL;
                scanf("%d", &allochead->value);
                status++;
            } else {
                alloc2 = (struct allocation *) malloc(alloclen);
                scanf("%d,%d", &alloc2->value);
                if (status == 1) {
                    allochead->next = alloc2;
                    status++;
                }
                alloc1->next = alloc2;
                alloc1 = alloc2;
            }
        }
    }
    alloc2->next = NULL;
    status = 0;
    printf("请输入最大需求矩阵:\n");
    for (i = 0; i < row; i++) {
        for (j = 0; j < colum; j++) {
            printf("请输入进程 p%d 种类 %c 系统资源最大需求:", i, 'A' + j);
            if (status == 0) {
                maxhead = maxium1 = maxium2 = (struct max *) malloc(maxlen);
                maxium1->next = maxium2->next = NULL;
                scanf("%d", &maxium1->value);
                status++;
            } else {
                maxium2 = (struct max *) malloc(maxlen);
                scanf("%d,%d", &maxium2->value);
                if (status == 1) {
                    maxhead->next = maxium2;
                    status++;
                }
                maxium1->next = maxium2;
                maxium1 = maxium2;
            }
        }
    }
    maxium2->next = NULL;
    status = 0;
    printf("请输入现时系统剩余的资源矩阵:\n");
    for (j = 0; j < colum; j++) {
        printf("种类 %c 的系统资源剩余:", 'A' + j);
        if (status == 0) {
            avahead = available1 = available2 = (struct available *) malloc(avalen);
            workhead = work1 = work2 = (struct available *) malloc(avalen);
            available1->next = available2->next = NULL;
            work1->next = work2->next = NULL;
            scanf("%d", &available1->value);
            work1->value = available1->value;
            status++;
        } else {
            available2 = (struct available *) malloc(avalen);
            work2 = (struct available *) malloc(avalen);
            scanf("%d,%d", &available2->value);
            work2->value = available2->value;
            if (status == 1) {
                avahead->next = available2;
                workhead->next = work2;
                status++;
            }
            available1->next = available2;
            available1 = available2;
            work1->next = work2;
            work1 = work2;
        }
    }
    available2->next = NULL;
    work2->next = NULL;
    status = 0;
    alloctemp = allochead;
    maxtemp = maxhead;
    for (i = 0; i < row; i++)
        for (j = 0; j < colum; j++) {
            if (status == 0) {
                needhead = need1 = need2 = (struct need *) malloc(needlen);
                need1->next = need2->next = NULL;
                need1->value = maxtemp->value - alloctemp->value;
                status++;
            } else {
                need2 = (struct need *) malloc(needlen);
                need2->value = (maxtemp->value) - (alloctemp->value);
                if (status == 1) {
                    needhead->next = need2;
                    status++;
                }
                need1->next = need2;
                need1 = need2;
            }
            maxtemp = maxtemp->next;
            alloctemp = alloctemp->next;
        }
    need2->next = NULL;
    status = 0;
    for (i = 0; i < row; i++) {
        if (status == 0) {
            finihead = finish1 = finish2 = (struct finish *) malloc(finilen);
            finish1->next = finish2->next = NULL;
            finish1->stat = 0;
            status++;
        } else {
            finish2 = (struct finish *) malloc(finilen);
            finish2->stat = 0;
            if (status == 1) {
                finihead->next = finish2;
                status++;
            }
            finish1->next = finish2;
            finish1 = finish2;
        }
    }
    finish2->next = NULL; /*Initialization compleated*/
    status = 0;
    processtest = 0;
    for (temp = 0; temp < row; temp++) {
        alloctemp = allochead;
        needtemp = needhead;
        finishtemp = finihead;
        worktemp = workhead;
        for (i = 0; i < row; i++) {
            worktemp1 = worktemp;
            if (finishtemp->stat == 0) {
                for (j = 0; j < colum; j++, needtemp = needtemp->next, worktemp = worktemp->next)
                    if (needtemp->value <= worktemp->value)
                        processtest++;
                if (processtest == colum) {
                    for (j = 0; j < colum; j++) {
                        worktemp1->value += alloctemp->value;
                        worktemp1 = worktemp1->next;
                        alloctemp = alloctemp->next;
                    }
                    if (status == 0) {
                        pathhead = path1 = path2 = (struct path *) malloc(pathlen);
                        path1->next = path2->next = NULL;
                        path1->value = i;
                        status++;
                    } else {
                        path2 = (struct path *) malloc(pathlen);
                        path2->value = i;
                        if (status == 1) {
                            pathhead->next = path2;
                            status++;
                        }
                        path1->next = path2;
                        path1 = path2;
                    }
                    finishtemp->stat = 1;
                } else {
                    for (t = 0; t < colum; t++)
                        alloctemp = alloctemp->next;
                    finishtemp->stat = 0;
                }
            } else
                for (t = 0; t < colum; t++) {
                    needtemp = needtemp->next;
                    alloctemp = alloctemp->next;
                }
            processtest = 0;
            worktemp = workhead;
            finishtemp = finishtemp->next;
        }
    }
    path2->next = NULL;
    finishtemp = finihead;
    for (temp = 0; temp < row; temp++) {
        if (finishtemp->stat == 0) {
            printf("\n系统处于非安全状态!\n");
            exit(0);
        }
        finishtemp = finishtemp->next;
    }
    printf("\n系统处于安全状态.\n");
    printf("\n安全序列为: \n");
    do {
        printf("p%d ", pathhead->value);
    } while (pathhead = pathhead->next);
    printf("\n");
    return 0;
}