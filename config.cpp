#include "config.h"
#include <QApplication>

int pid_seq = 1;
bool cmp_PID(PCB* a, PCB* b) {
    return a->pid < b->pid;
}

bool cmp_HPF(PCB* a, PCB* b) {
    return a->priority > b->priority;
}

bool cmp_SF(PCB* a, PCB* b) {
    return a->total_time < b->total_time;
}

MyList::MyList()
{

}

MyList::MyList(QListWidget *qList)
{
    this->qList = qList;
}

void MyList::add_and_show(QString str)
{
    qList->addItem(str);
    qList->update(); // 手动触发刷新
    QApplication::processEvents(); // 处理事件，确保刷新及时生效
}

void MyList::clear()
{
    if (qList != NULL) qList->clear();
}

MyList* my_list = new MyList(NULL);
QTimer* my_timer = NULL;
