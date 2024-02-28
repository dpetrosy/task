#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QSqlDatabase>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_addNodeButton_clicked();
    void on_removeNodeButton_clicked();
    void on_editNodeButton_clicked();

private:
    Ui::MainWindow *ui;
    QSqlDatabase database;

    void readTreeFromDatabase();
    void saveTreeToDatabase();
    void saveNodeToDatabase(QTreeWidgetItem *node, int parent_id, QSqlQuery &query);
};

#endif // MAINWINDOW_H
