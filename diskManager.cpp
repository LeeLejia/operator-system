#include<iostream>
using namespace std;

typedef struct node
{
    int data;
    struct node *next;
}Node;
int main() {
    void fcfs(Node *,int,int);  //声明先来先服务函数FCFS
    void sstf(Node *,int,int);  //声明最短寻道时间优先函数SSTF
    void scan(Node *,int,int);  //声明扫描函数SCAN
    void print(Node *);         //输出链表函数
    Node *head,*p,*q;           //建立一个链表
    int it,c=0,f,s;             //c为链表长度,f是开始的磁道号,s是选择哪个算法
    head=(Node *)malloc(sizeof(Node));
    head->next=NULL;
    q=head;
    cout<<"<======|磁盘调度算法|======>"<<endl;
    cout<<"输入一个磁道序列，以0为结束：";
    cin>>it;
    while(it!=0) {
        p=(Node *)malloc(sizeof(Node));
        p->next=NULL;
        p->data=it;
        q->next=p;
        q=p;
        cin>>it;
        c++;
    }
    cout<<"输入磁针起始位置：";
    cin>>f;                    //f为磁道号
    print(head);
    cout<<"序列长度:"<<c<<endl;

    cout<<"***********************************************"<<endl;
    cout<<"先来先服务算法FCFS"<<endl;
    fcfs( head,c,f);
    cout<<"电梯调度算法"<<endl;
    scan( head,c,f);
    cout<<"最短寻道时间优先算法SSTF"<<endl;
    sstf( head,c,f);
    cout<<"***********************************************"<<endl;
}
//先来先服务算法
void fcfs(Node *head,int c,int f) {
    void print(Node *);
    Node *l;//*m,*n;
    float num=0;             //num为平均寻道长度
    l=head->next;
    for(int i=0;i<c;i++)
    {
        num+=abs(l->data-f);
        f=l->data;
        l=l->next;
    }
    num=num/c;
    cout<<"先来先服务的寻道顺序是:"<<endl;
    print(head);
    cout<<"平均寻道长度:"<<num<<endl;
}
//最短寻道时间优先算法
void sstf(Node *head,int c,int f) {
    void print(Node *);
    Node *p,*q,*r,*s,*l,*m;
    l=(Node *)malloc(sizeof(Node));
    l->next=NULL;
    m=l;
    q=head;
    p=head->next;
    s=head;
    r=head->next;
    float num=0;
    for(int i=0;i<c;i++) {
        int min=abs(f-r->data);
        for(int j=0;j<c-i-1;j++)
        {
            p=p->next;
            q=q->next;
            if(abs(f-p->data)<min) {
                min=abs(f-p->data);
                r=p;
                s=q;
            }
        }
        num+=abs(f-r->data);
        f=r->data;
        s->next=r->next;
        r->next=NULL;
        m->next=r;
        m=r;
        q=head;
        p=head->next;
        s=head;
        r=head->next;
    }
    num=num/c;
    print(l);
    cout<<"平均寻道长度:"<<num<<endl;
}
//扫描算法（电梯调度算法）
void scan(Node *head,int c,int f) {
    void print(Node *);
    int min,max,i=0,j=0;
    float num=0;
    Node *p,*q,*r,*s,*m,*n,*x,*y;
    r=(Node *)malloc(sizeof(Node));//存放比开始磁道小的磁道
    r->next=NULL;
    s=r;
    m=(Node *)malloc(sizeof(Node));//存放比开始磁道大的磁道
    m->next=NULL;
    n=m;
    x=(Node *)malloc(sizeof(Node));
    x->next=NULL;
    y=x;
    q=head;
    p=head->next;
    if(p== nullptr)
        return;
    while(p->next!=NULL) {
        if(p->data-f>0)
        {
            q->next=p->next;
            p->next=NULL;
            n->next=p;
            n=p;
            p=q->next;
            i++;
        }
        else
        {
            q->next=p->next;
            p->next=NULL;
            s->next=p;
            s=p;
            p=q->next;
            j++;
        }
    }
    if(p->data>=f)
    {
        n->next=p;
        n=p;
        i++;
    }
    else
    {
        s->next=p;
        s=p;
        j++;
    }
    q=r;                    //对比开始磁道小的磁道排序
    p=r->next;
    while(q->next->next!=NULL)
    {
        q=q->next;
        p=q->next;
        max=q->data;
        while(p->next!=NULL)
        {
            if(p->data>max)
            {
                max=p->data;
                p->data=q->data;
                q->data=max;
                max=q->data;
            }
            p=p->next;
        }
        if(p->data>max)
        {
            max=p->data;
            p->data=q->data;
            q->data=max;
            max=q->data;
        }
    }
    //print(r);
    q=m;
    p=m->next;
    while(q->next->next!=NULL)
    {
        q=q->next;
        p=q->next;
        min=q->data;
        while(p->next!=NULL)
        {
            if(p->data<min)
            {
                min=p->data;
                p->data=q->data;
                q->data=min;
                min=q->data;
            }
            p=p->next;
        }
        if(p->data<min)
        {
            min=p->data;
            p->data=q->data;
            q->data=min;
            min=q->data;
        }
    }
    //print(m);
    x=m;
    p->next=r->next;
    y=x->next;
    while(y->next!=NULL)
    {
        num+=abs(f-y->data);
        f=y->data;
        y=y->next;
    }
    num+=abs(f-y->data);
    num=num/c;
    cout<<"扫描算法的顺序是:"<<endl;
    print(x);
    cout<<"平均寻道长度为:"<<num<<endl;
}
void print(Node *head){
    Node *p;
    p=head->next;
    cout<<"磁道序列:";
    if(p==NULL) {
        cout<<"磁道序列为空！";
    } else if(p->next==NULL) {
        cout<<p->data;
    } else {
        while(p->next!=NULL) {
            cout<<p->data<<"->";
            p=p->next;
        }
        cout<<p->data<<endl;
    }
}


// 5 44 845 487 85 154 8 87 846 26 263 2 15 456 56 5897 487 216 0 80