#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDateTime>
#include <QTreeWidgetItem>
#include <QTableWidgetItem>

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
    void addEvent(Task* parentTask, QString uid);
    void createIcalTask(Task *task);
    void saveToIcsFile(icalcomponent *comp);
    void loadFromIcsFile();

private slots:
    void on_actionNewProject_triggered();
    void on_actionNewTask_triggered();
    void on_addEventButton_clicked();

private:
    Ui::MainWindow *ui;
    QMap<QString, Task*> mProjects;
};

class DateItem : public QTableWidgetItem
{
//This class will help sorting events by real date instead of QString
public:
    QDateTime mTime;

    DateItem() : QTableWidgetItem()
    {
    }

    virtual bool operator<(const QTableWidgetItem &other) const
    {
        const DateItem *right = dynamic_cast<const DateItem*>(&other);
        if (right != NULL)
        {
            return (mTime < right->mTime);
        }
        return (QTableWidgetItem::operator<(other));
    }
};

#endif // MAINWINDOW_H
