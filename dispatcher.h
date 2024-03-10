#ifndef DISPATCHER_H
#define DISPATCHER_H

#include "pcb.h"
#include <vector>
#include <QTimer>
#include <mutex>
using namespace std;

class dispatcher {
private:
    void FIFO();
    void HPF();
    void RR();
    void SF();

public:
    int A;
    int B;
    int C;
    int start_;
    int r_flag;
    int cnt;
    mutex resourceMutex;
    vector<PCB * > PCB_queue;
    vector<PCB * > ready_queue;
    vector<PCB * > block_queue;
    QTimer* PCB_Timer;

public:
    dispatcher();
    ~dispatcher();
    void add(int total_time, int priority, int need_A, int need_B, int need_C);
    void Cancel(int pid);
    void block();
    void run(int type);
    int P(PCB* b);          //申请资源
    int V(PCB* b);          //释放资源
};

#endif // DISPATCHER_H
