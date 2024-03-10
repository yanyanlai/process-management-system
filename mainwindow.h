#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include "dispatcher.h"
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_create_But_released();

    void on_begin_But_released();

    void on_delete_But_released();

    void on_clear_released();

    void on_block_But_clicked();

    void on_stop_But_released();

    void on_look_But_released();

    void on_wake_But_released();

    void on_certain_1_released();

    void on_Cancle_clicked();

    void on_create_But_clicked();

    void on_Auto_clicked();
    void autoAddProcess();

    void on_Stop_Create_clicked();

private:
    Ui::MainWindow *ui;

public:
    dispatcher d;
    QTimer* timer;
    QTimer* resouce_timer;
    int choose;
};
#endif // MAINWINDOW_H
