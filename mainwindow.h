#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTreeWidgetItem>

#include <libical/ical.h>

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
    QTreeWidgetItem* selectedTreeWidgetItem();
    Task* selectedTask();

public:
    void addTask(QString name, QString uid, QTreeWidgetItem* parentWidget, Task* parentTask);
    void addTask(QString name, QString uid, QString parentUid);
    void createIcalTask(Task *task);
    void saveToIcsFile(icalcomponent *comp);
    void loadFromIcsFile();

private slots:
    void on_actionNewProject_triggered();
    void on_actionNewTask_triggered();

private:
    Ui::MainWindow *ui;
    QMap<QString, Task*> mProjects;
};

#endif // MAINWINDOW_H
