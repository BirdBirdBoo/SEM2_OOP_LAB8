#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QStandardPaths>
#include <QDebug>
#include <QIntValidator>
#include <QListWidget>
#include <array.h>
#include <fstream>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_addElementButtonA_clicked();

    void on_addElementButtonB_clicked();

    void onListItemAChanged(QListWidgetItem *item);
    void onListItemBChanged(QListWidgetItem *item);

    void handleListItemChanged(QListWidgetItem *item, QListWidget *view, Array *backingArray);
    bool checkItemValue(QListWidgetItem *item, int *value);
    void handleArrayItemChanged(Array *array, size_t row, int value);

    void on_add_a_to_b_clicked();

    void on_add_b_to_a_clicked();

    void on_actionSave_triggered();

    void on_actionOpen_triggered();

private:
    Ui::MainWindow *ui;

    Array *arrayA;
    Array *arrayB;

    void addItemToList(int item, QListWidget *list);

    void setUpInputs();

    void testArrays();
    void initArrayOpenDialog(QFileDialog &dialog, bool open);
    QString getSelectedFile(const QFileDialog &dialog);
};

QListWidget *operator<<(QListWidget *output, const Array &toOutput);
#endif // MAINWINDOW_H
