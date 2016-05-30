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

    TaskPropertiesDateTimeEditDelegate *widgetDelegate = new TaskPropertiesDateTimeEditDelegate(ui->tableEvents);
    ui->tableEvents->setItemDelegateForColumn(2, widgetDelegate);
    ui->tableEvents->setItemDelegateForColumn(3, widgetDelegate);

    ui->tableEvents->setColumnHidden(0, true);
    ui->tableEvents->setSortingEnabled(true);

    loadFromIcsFile();
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

    addTask(name, "", NULL, NULL);
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

    addTask(name, "", parentWidget, parentTask);
}

void MainWindow::on_addEventButton_clicked()
{
 //   QTreeWidgetItem* parentWidget = selectedTreeWidgetItem();
    Task* parentTask = selectedTask();

    if (parentTask == NULL) return;

    addEvent(parentTask, "");
}

void MainWindow::on_tableEvents_cellChanged(int row, int column)
{
    Event* event = (Event*)ui->tableEvents->item(row, 0)->data(Qt::UserRole).value<void*>();
    event->mName      = ui->tableEvents->item(row, 1)->text();
    event->mStartTime = QDateTime::fromString(ui->tableEvents->item(row, 2)->text());
    event->mEndTime   = QDateTime::fromString(ui->tableEvents->item(row, 3)->text());

    createIcalEvent(event);
}

void MainWindow::addEvent(Task* parentTask, QString uid)
{
    ui->tableEvents->blockSignals(true);

    Event* newEvent = new Event();
    newEvent->mName = parentTask->mName;
    if (uid == "") newEvent->mUID = createUid();
    else newEvent->mUID = uid;
    newEvent->pParent = parentTask;

    ui->tableEvents->insertRow(0);

    QTableWidgetItem *widgetUid = new QTableWidgetItem();
    widgetUid->setText(newEvent->mUID);
    widgetUid->setData(Qt::UserRole, qVariantFromValue((void*) newEvent));
    ui->tableEvents->setItem(0, 0, widgetUid);

    QTableWidgetItem *widgetName = new QTableWidgetItem();
    widgetName->setText(newEvent->mName);
    ui->tableEvents->setItem(0, 1, widgetName);

    // Columns 2 and 3 are DateItem's instead of QTableWidgetItem's to be correctly sorted

    DateItem *start = new DateItem();
    start->setText(newEvent->mStartTime.toString());
    start->mTime = newEvent->mStartTime;
    ui->tableEvents->setItem(0, 2, start);

    DateItem *end = new DateItem();
    end->setText(newEvent->mEndTime.toString());
    end->mTime = newEvent->mEndTime;
    ui->tableEvents->setItem(0, 3, end);

    ui->tableEvents->blockSignals(false);
}

void MainWindow::addTask(QString name, QString uid, QTreeWidgetItem* parentWidget, Task* parentTask)
{
    Task* newTask = new Task();
    newTask->mName = name;
    if (uid == "") newTask->mUID = createUid();
    else newTask->mUID = uid;
    newTask->pParent = parentTask;

    QTreeWidgetItem *item = new QTreeWidgetItem();
    item->setText(0, name);
    item->setText(1, "00:00:00");
    item->setData(0, Qt::UserRole, qVariantFromValue((void*) newTask));
    item->setData(1, Qt::UserRole, newTask->mUID);

    if (parentWidget == NULL)
        ui->treeWidget->addTopLevelItem(item);
    else
        parentWidget->addChild(item);

    if (parentTask == NULL)
        mProjects.insert(newTask->mUID, newTask);
    else
        parentTask->mChildren.push_back(newTask);

    if (uid == "")
        createIcalTask(newTask);
}

void MainWindow::addTask(QString name, QString uid, QString parentUid)
{
    QTreeWidgetItemIterator it(ui->treeWidget);

    if (parentUid != "")
    {
        while (*it)
        {
            if ((*it)->data(1, Qt::UserRole) == parentUid)
                    break;
            ++it;
        }

        addTask(name, uid, *it, (Task*)((*it)->data(0, Qt::UserRole).value<void*>()));
    }
    else
    {
        addTask(name, uid, NULL, NULL);
    }
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

void MainWindow::createIcalEvent(Event *event)
{
    icalcomponent *iEvent;
    icalproperty  *prop;

    iEvent = icalcomponent_new(ICAL_VEVENT_COMPONENT);

    prop = icalproperty_new_summary(event->icalSummary().c_str());
    icalcomponent_add_property(iEvent, prop);

    prop = icalproperty_new_uid(event->icalUid().c_str());
    icalcomponent_add_property(iEvent, prop);

    prop = icalproperty_new_relatedto(event->icalRelatedTo().c_str());
    icalcomponent_add_property(iEvent, prop);

    prop = icalproperty_new_dtstart(event->icalDtStart());
    icalcomponent_add_property(iEvent, prop);

    prop = icalproperty_new_dtend(event->icalDtEnd());
    icalcomponent_add_property(iEvent, prop);

    saveToIcsFile(iEvent);
}

void MainWindow::saveToIcsFile(icalcomponent *comp)
{
    icalset* file = icalfileset_new("/home/quentin/Public/test.ics");
    icalset_add_component(file, comp);
    icalfileset_commit(file);
    icalfileset_free(file);
}

char* read_stream(char *s, size_t size, void *d)
{
    char *c = fgets(s, size, (FILE*)d);

    return c;
}

void MainWindow::loadFromIcsFile()
{
    char* line;
    icalcomponent *c;
    icalparser *parser = icalparser_new();

    FILE* stream = fopen("/home/quentin/Public/test.ics", "r");

    icalparser_set_gen_data(parser, stream);

    do
    {
        line = icalparser_get_line(parser, read_stream);
        c = icalparser_add_line(parser, line);

        if (c != 0)
        {
            if (icalcomponent_isa(c) == ICAL_VTODO_COMPONENT)
            {
                const char* name   = icalcomponent_get_summary(c);
                const char* uid    = icalcomponent_get_uid(c);
                icalproperty *p    = icalcomponent_get_first_property(c, ICAL_RELATEDTO_PROPERTY);
                const char* parent = icalproperty_get_relatedto(p);
                addTask(name, uid, parent);
            }
            icalcomponent_free(c);
        }

    } while (line != 0);
}
