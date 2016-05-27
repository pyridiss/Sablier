#include <QInputDialog>
#include <QUuid>

#include <libical/ical.h>
#include <libical/icalss.h>

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

QString MainWindow::createUid()
{
    QString uuid = QUuid::createUuid().toString();
    //Remove brackets from uuid
    uuid.replace('{', "");
    uuid.replace('}', "");
    return uuid;
}

QTreeWidgetItem* MainWindow::selectedTreeWidgetItem()
{
    if (ui->treeWidget->selectedItems().empty()) return NULL;

    return ui->treeWidget->selectedItems().first();

}

Task* MainWindow::selectedTask()
{
    QTreeWidgetItem* item = selectedTreeWidgetItem();
    if (item == NULL) return NULL;

    return (Task*)item->data(0, Qt::UserRole).value<void*>();
}

void MainWindow::on_actionNewProject_triggered()
{
    bool ok;
    QString name = QInputDialog::getText(this,
                                         tr("New project"),
                                         tr("Enter the name of the new project:"),
                                         QLineEdit::Normal,
                                         QString(),
                                         &ok);
    if (!ok || name.isEmpty()) return;

    ui->selectProject->addItem(name);

    QTreeWidgetItem *item = new QTreeWidgetItem();
    item->setText(0, name);
    item->setText(1, "00:00:00");
    ui->treeWidget->addTopLevelItem(item);

    Task* newTask = new Task();
    newTask->mName = name;
    newTask->mUID = createUid();
    mProjects.insert(newTask->mUID, newTask);

    item->setData(0, Qt::UserRole, qVariantFromValue((void*) newTask));
    createIcalTask(newTask);
}

void MainWindow::on_actionNewTask_triggered()
{
    QTreeWidgetItem* parentWidget = selectedTreeWidgetItem();
    Task* parentTask = selectedTask();

    bool ok;
    QString name = QInputDialog::getText(this,
                                         tr("New sub task"),
                                         tr("Enter the name of the new sub task:"),
                                         QLineEdit::Normal,
                                         QString(),
                                         &ok);
    if (!ok || name.isEmpty()) return;

    ui->selectProject->addItem(name);

    QTreeWidgetItem *item = new QTreeWidgetItem();
    item->setText(0, name);
    item->setText(1, "00:00:00");
    parentWidget->addChild(item);

    Task* newTask = new Task();
    newTask->mName = name;
    newTask->mUID = createUid();
    newTask->pParent = parentTask;
    parentTask->mChildren.push_back(newTask);

    item->setData(0, Qt::UserRole, qVariantFromValue((void*) newTask));
    createIcalTask(newTask);
}

void MainWindow::createIcalTask(Task* task)
{
    icalcomponent *todo;
    icalproperty  *prop;

    todo = icalcomponent_new(ICAL_VTODO_COMPONENT);

    prop = icalproperty_new_summary(task->icalSummary().c_str());
    icalcomponent_add_property(todo, prop);

    prop = icalproperty_new_uid(task->icalUid().c_str());
    icalcomponent_add_property(todo, prop);

    if (task->icalRelatedTo() != "")
    {
        prop = icalproperty_new_relatedto(task->icalRelatedTo().c_str());
        icalcomponent_add_property(todo, prop);
    }

    saveToIcsFile(todo);
}

void MainWindow::saveToIcsFile(icalcomponent *comp)
{
    icalset* file = icalfileset_new("/home/quentin/Public/test.ics");
    icalset_add_component(file, comp);
    icalfileset_commit(file);
    icalfileset_free(file);
}
