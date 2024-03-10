#ifndef PCB_H
#define PCB_H
#include <QString>

class PCB {
public:
    int pid;            //进程号
    int total_time;     //需要运行的总时间
    int running_time;   //已经运行了的时间
    int priority;       //优先级
    int state;          //进程的状态
    int Revise;         //进程状态的修改位
    int need_A;         //进程对资源A的需求
    int need_B;         //进程对资源B的需求
    int need_C;         //进程对资源C的需求
    int get_A;          //进程已经占有的资源A
    int get_B;          //进程已经占有的资源B
    int get_C;          //进程已经占有的资源C

public:
    PCB(int end, int p, int a, int b, int c);
    ~PCB();
    int run();
    QString to_string(QString pre);
    QString resouce_show();
};

#endif // PCB_H
