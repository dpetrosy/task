#include <QSqlQuery>
#include <QSqlError>
#include <QInputDialog>
#include <QSql>

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    database = QSqlDatabase::addDatabase("QPSQL");
    database.setHostName("host");
    database.setDatabaseName("database");
    database.setUserName("username");
    database.setPassword("password");

    if (!database.open())
    {
        qDebug() << "Database connection error:" << database.lastError().text();
    }
    readTreeFromDatabase();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::readTreeFromDatabase()
{
    QSqlQuery query("SELECT id, name, parent_id FROM tree_table", database);

    while (query.next())
    {
        int id = query.value(0).toInt();
        QString name = query.value(1).toString();
        int parent_id = query.value(2).toInt();

        QTreeWidgetItem *item = new QTreeWidgetItem(ui->treeWidget);
        item->setData(0, Qt::UserRole, id);
        item->setText(0, name);

        if (parent_id != -1)
        {
            QList<QTreeWidgetItem *> parentItems = ui->treeWidget->findItems(QString::number(parent_id), Qt::MatchExactly, 0);
            if (!parentItems.isEmpty())
            {
                QTreeWidgetItem *parentItem = parentItems.first();
                parentItem->addChild(item);
            }
        }
    }
}

void MainWindow::saveTreeToDatabase()
{
    QSqlQuery clearQuery("DELETE FROM tree_table", database);
    if (!clearQuery.exec())
    {
        qDebug() << "Failed to clear the tree_table:" << clearQuery.lastError().text();
        return;
    }

    QSqlQuery insertQuery(database);
    insertQuery.prepare("INSERT INTO tree_table (id, name, parent_id) VALUES (?, ?, ?)");

    for (int i = 0; i < ui->treeWidget->topLevelItemCount(); ++i)
    {
        QTreeWidgetItem *item = ui->treeWidget->topLevelItem(i);
        saveNodeToDatabase(item, -1, insertQuery);
    }
}

void MainWindow::saveNodeToDatabase(QTreeWidgetItem *node, int parent_id, QSqlQuery &query)
{
    QString nodeName = node->text(0);
    int nodeId = node->data(0, Qt::UserRole).toInt();

    query.bindValue(0, nodeId);
    query.bindValue(1, nodeName);
    query.bindValue(2, parent_id);
    if (!query.exec())
    {
        qDebug() << "Failed to insert node:" << query.lastError().text();
        return;
    }

    for (int i = 0; i < node->childCount(); ++i)
    {
        saveNodeToDatabase(node->child(i), nodeId, query);
    }
}

void MainWindow::on_addNodeButton_clicked()
{
    QString nodeName = QInputDialog::getText(this, "Add Node", "Enter node name:");

    if (!nodeName.isEmpty())
    {
        QTreeWidgetItem *selectedItem = ui->treeWidget->currentItem();
        QTreeWidgetItem *item = new QTreeWidgetItem(ui->treeWidget);
        item->setText(0, nodeName);

        if (selectedItem)
        {
            item->setData(0, Qt::UserRole, -1);
            selectedItem->addChild(item);
        }
        else
        {
            item->setData(0, Qt::UserRole, -1);
            ui->treeWidget->addTopLevelItem(item);
        }
    }
}

void MainWindow::on_removeNodeButton_clicked()
{
    QTreeWidgetItem *selectedItem = ui->treeWidget->currentItem();
    if (selectedItem)
    {
        delete selectedItem;
    }
}

void MainWindow::on_editNodeButton_clicked()
{
    QTreeWidgetItem *selectedItem = ui->treeWidget->currentItem();

    if (selectedItem)
    {
        QString nodeName = QInputDialog::getText(this, "Edit Node", "Enter new node name:", QLineEdit::Normal, selectedItem->text(0));
        if (!nodeName.isEmpty())
        {
            selectedItem->setText(0, nodeName);
        }
    }
}
