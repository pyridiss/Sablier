#include <QInputDialog>

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
}

void MainWindow::on_actionNewTask_triggered()
{
    //We first check if a project or a task is selected
    if (ui->treeWidget->selectedItems().empty()) return;

    QTreeWidgetItem* parent = ui->treeWidget->selectedItems().first();

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
    parent->addChild(item);
}
