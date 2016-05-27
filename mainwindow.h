#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "task.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QString createUid();

private slots:
    void on_actionNewProject_triggered();
    void on_actionNewTask_triggered();

private:
    Ui::MainWindow *ui;
    QMap<QString, Task*> mProjects;
};

#endif // MAINWINDOW_H
