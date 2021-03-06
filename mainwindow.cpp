#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    arrayA = new IncrementalArray();
    arrayB = new BlockyArray();

    testArrays();
}

MainWindow::~MainWindow()
{
    delete ui;

    delete arrayA;
    delete arrayB;
}

void MainWindow::setUpInputs()
{
    auto intValidator = new QIntValidator(INT32_MIN, INT32_MAX, this);
    ui->elementEditA->setValidator(intValidator);
    ui->elementEditB->setValidator(intValidator);
}

void MainWindow::testArrays()
{
    IncrementalArray i_array1 = IncrementalArray();
    Array &array1 = i_array1;
    array1.add(-1);
    array1[0] = 2;
    array1.add(5);
    array1.print();

    IncrementalArray i_array2 = IncrementalArray(0);
    Array &array2 = i_array2;
    array2.add(8);
    array2.print();

    array2.addAll(array1);
    array2.print();

    array1.addAll(array2);
    array2.addAll(array1);
    array2.print();

    BlockyArray b_array3 = BlockyArray(2);
    Array &array3 = b_array3;

    array3.print();
    array3.add(2);
    (array3 += 2) += 5;
    array3.print();

    array3[0] = 0;
    array2.addAll(array2);
    array3 += array2 += -100;
    array3.print();

    array3.clear();
    array3.print();

    //std::ofstream output1;
    //output1.open("array.txt");
    //output1 << array1 << std::endl;
    //output1.close();

    std::ifstream input2;
    input2.open("array.txt");
    input2 >> array2;

    array2.print();
}


void MainWindow::on_addElementButtonA_clicked()
{
    if (!ui->elementEditA->hasAcceptableInput())
        return;
    const int value = ui->elementEditA->text().toInt();
    arrayA->add(value);
    addItemToList(value, ui->arrayViewA);
}

void MainWindow::on_addElementButtonB_clicked()
{
    if (!ui->elementEditB->hasAcceptableInput())
        return;
    const int value = ui->elementEditB->text().toInt();
    arrayB->add(value);
    addItemToList(value, ui->arrayViewB);
}

void MainWindow::onListItemAChanged(QListWidgetItem *item)
{
    handleListItemChanged(item, ui->arrayViewA, arrayA);
}

void MainWindow::onListItemBChanged(QListWidgetItem *item)
{
    handleListItemChanged(item, ui->arrayViewB, arrayB);
}

void MainWindow::handleListItemChanged(QListWidgetItem *item, QListWidget *view, Array *backingArray)
{
    const unsigned int row = view->row(item);
    int value = row < backingArray->getLength() ? (*backingArray)[row] : 0;

    if (!checkItemValue(item, &value)) {
        return;
    }

    handleArrayItemChanged(backingArray, row, value);
}

bool MainWindow::checkItemValue(QListWidgetItem *item, int *pValue)
{
    bool valueCorrect = false;
    int value = item->text().toInt(&valueCorrect);
    if (!valueCorrect) {
        item->setText(QString::number(*pValue));
    } else {
        *pValue = value;
    }
    return valueCorrect;
}

void MainWindow::handleArrayItemChanged(Array *array, size_t row, int value)
{
    if (row == array->getLength()) {
        array->add(value);
    } else if(row < array->getLength()) {
        (*array)[row] = value;
    }
}

void MainWindow::addItemToList(int item, QListWidget *list)
{
    auto listItem = new QListWidgetItem(QString::number(item));
    listItem->setFlags(listItem->flags() | Qt::ItemIsEditable);
    list->addItem(listItem);
}

QListWidget *operator <<(QListWidget *output, const Array &toOutput) {
    const size_t listSize = output->count(), arraySize = toOutput.getLength();
    for (size_t i = 0; i < listSize; ++i) {
        output->item(i)->setText(QString::number(toOutput[i]));
    }
    for (size_t i = listSize; i < arraySize; ++i) {
        output->addItem(QString::number(toOutput[i]));
        output->item(i)->setFlags(output->item(i)->flags() | Qt::ItemIsEditable);
    }

    return output;
}

void MainWindow::on_add_a_to_b_clicked()
{
    ui->arrayViewB << (*arrayB += *arrayA);
}

void MainWindow::on_add_b_to_a_clicked()
{
    ui->arrayViewA << (*arrayA += *arrayB);
}

void MainWindow::on_actionSave_triggered()
{
    QFileDialog dialog(this);
    initArrayOpenDialog(dialog, false);

    if (dialog.exec() == QDialog::Accepted) {
        auto file = getSelectedFile(dialog).toStdString();

        std::ofstream output;
        output.open(file);

        if (!output.is_open()) {
            QMessageBox::critical(this,
                                  "Error",
                                  "Could not open the file");
            return;
        }

        output << *arrayA;
        output << *arrayB;
        output.close();
    }
}

void MainWindow::on_actionOpen_triggered() {
    QFileDialog dialog(this);
    initArrayOpenDialog(dialog, true);

    if (dialog.exec() == QDialog::Accepted) {
        auto file = getSelectedFile(dialog).toStdString();

        std::ifstream input;
        input.open(file);

        if (!input.is_open()) {
            QMessageBox::critical(this,
                                  "Error",
                                  "Could not open the file");
            return;
        }

        input >> *arrayA;
        input >> *arrayB;
        input.close();

        ui->arrayViewA << *arrayA;
        ui->arrayViewB << *arrayB;
    }
}

void MainWindow::initArrayOpenDialog(QFileDialog &dialog, bool open) {
    static bool firstTextDialog = true;

    if (firstTextDialog) {
        firstTextDialog = false;
        const QStringList documentsLocations = QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation);
        dialog.setDirectory(documentsLocations.isEmpty() ? QDir::currentPath() : documentsLocations.last());
    }

    QStringList textFilters({
                                "text/plain",
                                "application/octet-stream"
                            });
    dialog.setMimeTypeFilters(textFilters);
    dialog.selectMimeTypeFilter("text/plain");
    dialog.setAcceptMode(open ? QFileDialog::AcceptOpen : QFileDialog::AcceptSave);
}

QString MainWindow::getSelectedFile(const QFileDialog &dialog) {
    auto selectedPaths = dialog.selectedFiles();
    return QDir::toNativeSeparators(selectedPaths.first());
}
