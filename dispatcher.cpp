#include "dispatcher.h"
#include "config.h"
#include <bits/stdc++.h>
#include <QApplication>
#include <QTimer>

using namespace std;

#include <QDebug>

dispatcher::dispatcher() {
    // 初始化资源标志和计数器
    r_flag = 0;
    cnt = 0;

    // 初始化系统资源 A、B、C
    A = 10;
    B = 10;
    C = 10;

    // 创建一个 QTimer 对象
    PCB_Timer = new QTimer();

    // 连接定时器的 timeout 信号到 lambda 函数
    QObject::connect(PCB_Timer, &QTimer::timeout, [&]() {
        // 遍历 PCB 队列中的每个进程
        for (int i = 0; i < PCB_queue.size(); i++) {
            PCB* b = PCB_queue[i];
            // 如果进程状态需要修改
            if (b->Revise == 1) {
                // 在就绪队列中查找并移除相应进程
                for (int j = 0; j < ready_queue.size(); j++) {
                    if (ready_queue[j]->pid == b->pid) {
                        ready_queue.erase(ready_queue.begin() + j);
                        break;
                    }
                }
                // 在阻塞队列中查找并移除相应进程
                for (int j = 0; j < block_queue.size(); j++) {
                    if (block_queue[j]->pid == b->pid) {
                        block_queue.erase(block_queue.begin() + j);
                        break;
                    }
                }
                // 重置进程状态修改标志
                b->Revise = 0;
                // 根据进程状态将进程重新放入相应队列
                if (b->state == READY)
                    ready_queue.push_back(b);
                else if (b->state == BLOCK)
                    block_queue.push_back(b);
                else if (b->state == DEAD) {
                    // 如果进程状态为 DEAD，释放资源并移除进程
                    V(b);
                    PCB_queue.erase(PCB_queue.begin() + i);
                    delete b;
                }
            }
            // 执行 P 操作，模拟进程运行
            P(b);
        }
    });

    // 启动定时器，每隔100毫秒执行一次上述 lambda 函数中的内容
    PCB_Timer->start(100);
}


dispatcher::~dispatcher() {

}

int dispatcher::P(PCB *b)
{
    lock_guard<std::mutex> lock(resourceMutex); // 使用互斥锁保护共享资源的访问
    int f_A = 0; // 标记A资源是否被成功获取
    int f_B = 0; // 标记B资源是否被成功获取
    int f_C = 0; // 标记C资源是否被成功获取

    if (b->state == DEAD) {
        return DEAD; // 如果进程状态为DEAD，直接返回DEAD
    }

    if (b->get_A < b->need_A) {
        int need = b->need_A - b->get_A; // 计算A资源还需要多少
        if (need <= A) {
            b->get_A += need; // 获取A资源
            A -= need; // 更新系统A资源
            f_A = 1; // 设置A资源获取标记为1
        }
        else {
            // 如果A资源不足，不执行额外操作（这部分可能需要添加错误处理逻辑）
        }
    }
    else {
        f_A = 1; // 如果A资源已经满足，设置A资源获取标记为1
    }

    if (b->get_B < b->need_B) {
        int need = b->need_B - b->get_B; // 计算B资源还需要多少
        if (need <= B) {
            b->get_B += need; // 获取B资源
            B -= need; // 更新系统B资源
            f_B = 1; // 设置B资源获取标记为1
        }
    }
    else {
        f_B = 1; // 如果B资源已经满足，设置B资源获取标记为1
    }

    if (b->get_C < b->need_C) {
        int need = b->need_C - b->get_C; // 计算C资源还需要多少
        if (need <= C) {
            b->get_C += need; // 获取C资源
            C -= need; // 更新系统C资源
            f_C = 1; // 设置C资源获取标记为1
        }
    }
    else {
        f_C = 1; // 如果C资源已经满足，设置C资源获取标记为1
    }

    if (f_A && f_B && f_C) {
        if (b->state == BLOCK) {
            b->state = READY; // 将进程状态设置为READY
            b->Revise = 1; // 标记进程状态修改
        }
        return READY; // 返回READY状态
    }

    b->state = BLOCK; // 如果资源获取不满足，将进程状态设置为BLOCK
    b->Revise = 1; // 标记进程状态修改
    return BLOCK; // 返回BLOCK状态
}

int dispatcher::V(PCB *b)
{
    lock_guard<std::mutex> lock(resourceMutex); // 使用互斥锁保护共享资源的访问
    my_list->add_and_show(QString("进程资源已经释放，PID为：%1").arg(b->pid)); // 输出释放资源的信息
    A += b->get_A; // 归还A资源
    B += b->get_B; // 归还B资源
    C += b->get_C; // 归还C资源

    b->get_A = b->get_B = b->get_C = 0; // 将进程获取的资源数量重置为0
}


void dispatcher::add(int total_time, int priority, int need_A, int need_B, int need_C) {
    PCB* b = new PCB(total_time, priority, need_A, need_B, need_C);
    P(b);
    PCB_queue.push_back(b);

    my_list->add_and_show(b->to_string(QString("一个进程进入了就绪队列，")));
}

void dispatcher::Cancel(int pid)
{
    // 初始化标志，表示是否找到指定的进程
    bool f = false;
    // 遍历调度器的 PCB 队列
    for (int i = 0; i < PCB_queue.size(); i++) {
        PCB *b = PCB_queue[i];
        // 检查当前 PCB 是否与输入的 PID 匹配
        if (pid == b->pid) {
            // 设置进程状态为 DEAD
            b->state = DEAD;
            // 标记进程状态修改
            b->Revise = 1;
            // 设置找到标志为 true
            f = true;
        }
    }

    // 根据查找结果输出相应的信息
    if (f)
        // 如果找到指定进程，输出进程已撤销的信息
        my_list->add_and_show(QString("进程已经撤销了, pid=%1").arg(pid));
    else
        // 如果没有找到指定进程，输出未找到的信息
        my_list->add_and_show(QString("没有找到指定进程, pid=%1").arg(pid));
}

void dispatcher::block()
{
    // 检查调度器的定时器是否处于活动状态
    if (my_timer->isActive()) {
        // 停止定时器
        my_timer->stop();
        // 重置计数器
        cnt = 0;

        PCB* b;
        // 检查就绪队列是否为空
        if (!ready_queue.empty())
            b = ready_queue.front(); // 获取就绪队列的第一个进程
        else {
            // 如果就绪队列为空，输出相应信息并返回
            my_list->add_and_show(QString("就绪队列为空，无可用的进程进行阻塞"));
            return;
        }

        // 设置当前进程状态为 BLOCK
        b->state = BLOCK;
        // 标记进程状态修改
        b->Revise = 1;

        // 输出当前进程已阻塞的信息
        my_list->add_and_show(QString("当前进程已经阻塞了, pid=%1").arg(b->pid));

        // 重新启动定时器
        my_timer->start();
    }
    else {
        // 如果调度未开始，输出相应信息
        my_list->add_and_show(QString("调度未开始"));
    }
}


void dispatcher::run(int type) {
    switch (type) {
    case 1:
        FIFO();      // 先来先服务
        break;
    case 2:
        HPF();       // 高优先级
        break;
    case 3:
        RR();        // 时间片轮转
        break;
    case 4:
        SF();        // 短作业优先
    }

    my_list->add_and_show(QString("就绪队列里剩%1个进程").arg(ready_queue.size())); // 输出就绪队列中的进程数量

    if (start_ != 0) return; // 如果已经开始调度，直接返回
    start_ = 1;

    QObject::connect(my_timer, &QTimer::timeout, [&]() {
        int flag = 0;
        PCB* b;

        if (!ready_queue.empty())
            b = ready_queue.front(); // 获取就绪队列的第一个进程
        else {
            my_list->add_and_show(QString("就绪队列为空,本次调度结束"));
            my_timer->stop();
            r_flag = 0;
            cnt = 0;
            return; // 如果就绪队列为空，输出信息，停止定时器，重置标志，结束函数
        }

        b->state = RUNNING; // 设置当前进程状态为运行中
        flag = b->run(); // 执行进程运行函数，返回运行状态

        if (r_flag == 1) cnt++; // 如果 r_flag 为1，递增计数器

        if (flag == 1) {
            b->state = DEAD; // 如果运行状态为1，将进程状态设置为DEAD
            b->Revise = 1; // 标记进程状态修改
            cnt = 0; // 重置计数器
            my_list->add_and_show(b->to_string(QString("进程运行时间结束了，"))); // 输出进程结束信息
        }
        else if (r_flag == 1 && cnt == TIME_SLICE) {
            my_list->add_and_show(QString("时间片到了"));
            b->state = READY; // 如果 r_flag 为1且计数器等于时间片数，将进程状态设置为READY
            b->Revise = 1; // 标记进程状态修改
            cnt = 0; // 重置计数器
        }
    });
}

void dispatcher::FIFO() {
    my_list->add_and_show(QString("开始调度进程, 调度算法为先来先服务算法"));
    sort(ready_queue.begin(), ready_queue.end(), cmp_PID);
    my_timer->start();
}

void dispatcher::HPF() {
    my_list->add_and_show(QString("开始调度进程, 调度算法为高优先级优先算法"));
    sort(ready_queue.begin(), ready_queue.end(), cmp_HPF);
    my_timer->start();
}

void dispatcher::RR() {
    my_list->add_and_show(QString("开始调度进程, 调度算法为时间片轮询算法"));
    r_flag = 1;
    my_timer->start();
}

void dispatcher::SF() {
    my_list->add_and_show(QString("开始调度进程, 调度算法为短进程优先算法"));
    sort(ready_queue.begin(), ready_queue.end(), cmp_SF);
    my_timer->start();
}
