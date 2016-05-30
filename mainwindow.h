#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDateTime>
#include <QTreeWidgetItem>
#include <QTableWidgetItem>
#include <QItemDelegate>
#include <QDateTimeEdit>

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
    void on_tableEvents_cellChanged(int row, int column);

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

class TaskPropertiesDateTimeEditDelegate : public QItemDelegate
{
public:
    TaskPropertiesDateTimeEditDelegate(QObject *parent = 0) : QItemDelegate(parent)
    {
    }

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem&, const QModelIndex&) const
    {
        QDateTimeEdit *editor = new QDateTimeEdit(parent);
        editor->setDisplayFormat(QString("dd/MM/yyyy  HH:mm:ss"));
        return editor;
    }

    void setEditorData(QWidget *editor, const QModelIndex &index) const
    {
        QDateTime dateTime = QDateTime::fromString(index.model()->data(index, Qt::DisplayRole).toString());
        QDateTimeEdit *dateTimeWidget = dynamic_cast<QDateTimeEdit*>(editor);
        dateTimeWidget->setDateTime(dateTime);
    }

    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
    {
        QDateTimeEdit *dateTimeWidget = dynamic_cast<QDateTimeEdit*>(editor);
        QDateTime dateTime = dateTimeWidget->dateTime();
        model->setData(index, dateTime.toString(), Qt::EditRole);
    }

    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex&) const
    {
        editor->setGeometry(option.rect);
    }
};

#endif // MAINWINDOW_H
