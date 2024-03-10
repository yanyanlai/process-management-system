#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTextEdit>
#include <QDebug>
#include "pcb.h"
#include "config.h"

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    timer = new QTimer(this);

    //全局
    QListWidget* list = ui->listWidget;
    my_list = new MyList(list);
    my_timer = timer;
    my_timer->setInterval(TIME_OUT);

    resouce_timer = new QTimer();

    QObject::connect(resouce_timer, &QTimer::timeout, [&]() {
        QLabel* a = ui->A;
        QLabel* b = ui->B;
        QLabel* c = ui->C;
        a->setText(QString("系统资源A还剩余：%1").arg(d.A));
        b->setText(QString("系统资源B还剩余：%1").arg(d.B));
        c->setText(QString("系统资源C还剩余：%1").arg(d.C));
    });

    resouce_timer->start(100);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_create_But_released()
{
    ui->stackedWidget->setCurrentWidget(ui->page_2);
}

void MainWindow::on_begin_But_released()
{
    //获取调度类型
    QComboBox* box = ui->comboBox;
    QStringList type = box->currentText().split(".");
    qDebug() << type[0].toInt() << " " << type[1];

    if (my_timer->isActive()) {
       my_list->add_and_show(QString("调度正在进行"));
       return;
    }

    if (!d.ready_queue.empty()) d.run(type[0].toInt());
    else {
        my_list->add_and_show(QString("就绪队列为空"));
    }
}

void MainWindow::on_delete_But_released()
{
    QTextEdit* pid = ui->pid__;
    QString pid_str = pid->toPlainText();

    d.Cancel(pid_str.toInt());
}

void MainWindow::on_clear_released()
{
    my_list->clear();
}

void MainWindow::on_block_But_clicked()
{
    d.block();
}

void MainWindow::on_stop_But_released()
{
    if (my_timer->isActive()) {
        my_timer->stop();
        if (!d.ready_queue.empty()) {
            d.ready_queue[0]->state = READY;
        }
        my_list->add_and_show(QString("调度已经停止了"));
        d.cnt = 0;
    }
    else {
        my_list->add_and_show(QString("调度还未开始"));
    }
}

void MainWindow::on_look_But_released()
{
    QTextEdit* pid = ui->pid__;
    QString pid_str = pid->toPlainText();

    bool f = false;
    PCB* b;
    for (int i = 0; i < d.PCB_queue.size(); i++) {
        b = d.PCB_queue[i];
        if (b->pid == pid_str.toInt()) {
            f = true;
            break;
        }
    }

    if (f) my_list->add_and_show(b->resouce_show());
    else my_list->add_and_show(QString("未找到相关进程，PID为：%1").arg(pid_str));

}

void MainWindow::on_wake_But_released()
{
    // 获取界面中的进程ID输入框
    QTextEdit* pid = ui->pid__;
    // 获取输入框中的进程ID字符串
    QString pid_str = pid->toPlainText();

    // 初始化标志和状态
    bool f = false; // 标记是否找到对应进程
    int state = 0; // 标记进程唤醒后的状态

    // 遍历调度器的 PCB 队列
    for (int i = 0; i < d.PCB_queue.size(); i++) {
        PCB* b = d.PCB_queue[i];
        // 检查当前 PCB 是否与输入的 PID 匹配
        if (b->pid == pid_str.toInt()) {
            // 调用调度器的 P 函数唤醒进程
            state = d.P(b);
            // 重置调度器的计数器
            d.cnt = 0;
            // 设置找到标志为 true
            f = true;
            // 跳出循环
            break;
        }
    }

    // 根据查找结果输出相应的信息
    if (f) {
        if (state == READY)
            // 如果状态为 READY，输出进程已经被唤醒的信息
            my_list->add_and_show(QString("进程已经被唤醒，PID为：%1").arg(pid_str));
        else if (state == BLOCK)
            // 如果状态为 BLOCK，输出资源不足无法唤醒的信息
            my_list->add_and_show(QString("进程所需资源不足，无法唤醒，PID为：%1").arg(pid_str));
    }
    else
        // 如果没有找到相关进程，输出未找到的信息
        my_list->add_and_show(QString("找不到相关进程，PID为：%1").arg(pid_str));
}


void MainWindow::on_certain_1_released()
{
    //获取数据
    QTextEdit* t = ui->time__;
    QTextEdit* p = ui->priority__;
    QTextEdit* a = ui->need_A;
    QTextEdit* b = ui->need_B;
    QTextEdit* c = ui->need_C;
    QString t_str = t->toPlainText();
    QString p_str = p->toPlainText();
    QString a_str = a->toPlainText();
    QString b_str = b->toPlainText();
    QString c_str = c->toPlainText();

    qDebug() << t_str << p_str;
    //添加进程
    d.add(t_str.toInt(), p_str.toInt(), a_str.toInt(), b_str.toInt(), c_str.toInt());

    ui->stackedWidget->setCurrentWidget(ui->page_1);
}

void MainWindow::on_Cancle_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_1);
}

void MainWindow::on_Auto_clicked()
{
    // 创建一个 QTimer 对象
        QTimer* timer = new QTimer(this);

        // 连接定时器的 timeout 信号到槽函数
        connect(timer, &QTimer::timeout, this, &MainWindow::autoAddProcess);

        // 设置定时器间隔为1-10秒之间的随机值（毫秒为单位）
        int interval = qrand() % 6001 + 5000;  // 5000-10000 毫秒之间的随机值
        timer->start(interval);
}
void MainWindow::autoAddProcess()
{
    // 生成1-20之间的随机值
    int total_time = qrand() % 20 + 1;

    // 生成1-10之间的随机值
    int priority = qrand() % 10 + 1;
    int need_A = qrand() % 3 + 1;
    int need_B = qrand() % 3 + 1;
    int need_C = qrand() % 3 + 1;

    // 调用 add 函数添加进程
    d.add(total_time, priority, need_A, need_B, need_C);
}

void MainWindow::on_Stop_Create_clicked()
{

}
