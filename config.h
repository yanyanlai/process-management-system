#ifndef CONFIG_H
#define CONFIG_H

#include "pcb.h"
#include <QListWidget>
#include <QTimer>
#include <vector>
using namespace std;

bool cmp_PID(PCB* a, PCB* b);
bool cmp_HPF(PCB* a, PCB* b);
bool cmp_SF(PCB* a, PCB* b);

#define TIME_SLICE 3        //时间片大小
#define TIME_OUT 1 * 1000   //进程的一次运行时间，即定时器的超时时间
#define READY 1             //进程的状态：就绪
#define BLOCK 2             //进程的状态：阻塞
#define RUNNING 3           //进程的状态：运行
#define DEAD 4              //进程的状态：撤销
#define BUTTON_CONNEL 1     //按钮：撤销进程
#define BUTTON_LOOK 2       //按钮：查看进程
#define BUTTON_WAKE 3       //按钮：唤醒进程

class MyList {
private:
    QListWidget* qList;
public:
    MyList();
    MyList(QListWidget* qList);
    void add_and_show(QString str);
    void clear();
};

extern int pid_seq;
extern MyList* my_list;
extern QTimer* my_timer;


#endif // CONFIG_H
