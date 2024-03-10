#include "pcb.h"
#include "config.h"
#include <windows.h>
#include <iostream>
#include <QDebug>
#include <QApplication>
using namespace std;

PCB::PCB(int total, int p, int a, int b, int c){
    pid = pid_seq++;
    total_time = total;
    running_time = 0;
    priority = p;
    need_A = a;
    need_B = b;
    need_C = c;
    get_A = get_B = get_C = 0;
    state = BLOCK;
}

PCB::~PCB() {

}

int PCB::run() {
    // 检查进程的运行时间是否小于总运行时间
    if (running_time < total_time) {
        // 如果是，更新进程的运行时间，但不超过总运行时间
        running_time = min(total_time, running_time + TIME_OUT/1000);
        // 在界面上显示进程正在被调度中的信息
        my_list->add_and_show(to_string("进程正在被调度中，"));
    }

    // 检查进程的运行时间是否等于总运行时间
    if (running_time == total_time)
        // 如果是，返回1，表示进程的运行时间已经达到总运行时间
        return 1;
    // 如果不是，返回0，表示进程还需要继续运行
    return 0;
}

QString PCB::to_string(QString pre)
{
    QString s;
    if (state == RUNNING) s = "正在运行";
    else if (state == BLOCK) s = "阻塞";
    else if (state == READY) s = "就绪";
    else if (state == DEAD) s = "死亡";
    QString str = QString("%6"
                        "进程状态为：%5，"
                        "PID为：%1，"
                        "优先级为：%2，"
                        "已经运行了 %3 秒, 一共需要运行 %4 秒。")
            .arg(pid)
            .arg(priority)
            .arg(running_time)
            .arg(total_time)
            .arg(s)
            .arg(pre);
//arg 函数用于将参数插入到格式化字符串中的占位符位置
    return str;
}

QString PCB::resouce_show()
{
    QString s;
    if (state == RUNNING) s = "正在运行";
    else if (state == BLOCK) s = "阻塞";
    else if (state == READY) s = "就绪";
    else if (state == DEAD) s = "死亡";
    QString str = QString(
                        "进程状态为：%1，"
                        "PID为：%2，"
                        "优先级为：%3。\n"
                        "已经运行了 %10 秒, 一共需要运行 %11 秒\n"
                        "占有资源A的数量：%4，需要资源A的数量：%5\n"
                        "占有资源B的数量：%6，需要资源B的数量：%7\n"
                        "占有资源B的数量：%8，需要资源B的数量：%9")
            .arg(s)
            .arg(pid)
            .arg(priority)
            .arg(get_A).arg(need_A)
            .arg(get_B).arg(need_B)
            .arg(get_C).arg(need_C)
            .arg(running_time).arg(total_time)
            ;

    return str;
}
