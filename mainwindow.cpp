#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QMouseEvent>
#include <QPushButton>
#include <QVBoxLayout>
#include <QTableWidgetItem>
#include <QTableWidget>
#include <QDebug>
#include <QVector>

using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
     ui->centralwidget->setStyleSheet("QWidget#centralwidget { border-image: url(/Users/kirilldrapeza/Documents/lab9/androidphones-2048px-0803.jpg) 0 0 0 0 stretch stretch; }");
    connect(ui->loadButton, &QPushButton::clicked, this, &MainWindow::onLoadButtonClicked);

    QPushButton *addItemButton = new QPushButton("Add", this);
    QPushButton *removeItemButton = new QPushButton("Remove", this);
    QPushButton *sortListButton = new QPushButton("Standart Sort", this);
    QPushButton *countSortButton = new QPushButton("Hoara Sort", this);

    ui->verticalLayout->addWidget(addItemButton);
    ui->verticalLayout->addWidget(removeItemButton);
    ui->verticalLayout->addWidget(sortListButton);
    ui->verticalLayout->addWidget(countSortButton);

    connect(addItemButton, &QPushButton::clicked, this, &MainWindow::onAddItemButtonClicked);
    connect(removeItemButton, &QPushButton::clicked, this, &MainWindow::onRemoveItemButtonClicked);
    connect(sortListButton, &QPushButton::clicked, this, &MainWindow::onSortListButtonClicked);
    connect(countSortButton, &QPushButton::clicked, this, &MainWindow::onCountSortButtonClicked);

    // Обработка событий мыши для синего виджета
    ui->loadButton->installEventFilter(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onLoadButtonClicked()
{
    loadPhoneNumbers("/Users/kirilldrapeza/Downloads/lab2Kirill/build/Qt_6_7_2_for_macOS-Debug/phonenumbers.txt");
}

void MainWindow::loadPhoneNumbers(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Error", "Cannot open file: " + fileName);
        return;
    }

    QTextStream in(&file);
    QString line = in.readLine();
    int numcount = line.toInt();

    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(numcount);
    ui->tableWidget->setColumnCount(3);
    QStringList headers = {"Address", "Phone Number", "Surname"};
    ui->tableWidget->setHorizontalHeaderLabels(headers);

    int row = 0;
    while (!in.atEnd() && row < numcount) {
        line = in.readLine();
        QStringList fields = line.split(",");
        if (fields.size() == 3) {
            QString address = fields[0];
            qint64 phonenumber = fields[1].toLongLong();
            QString surname = fields[2];

            ui->tableWidget->setItem(row, 0, new QTableWidgetItem(address));
            ui->tableWidget->setItem(row, 1, new QTableWidgetItem(QString::number(phonenumber)));
            ui->tableWidget->setItem(row, 2, new QTableWidgetItem(surname));
        }
        ++row;
    }

    file.close();
}

void MainWindow::onAddItemButtonClicked()
{
    // Вставляем новую строку в QTableWidget
    int newRow = ui->tableWidget->rowCount();
    ui->tableWidget->insertRow(newRow);

    // Добавляем редактируемые ячейки в новую строку
    for (int col = 0; col < ui->tableWidget->columnCount(); ++col) {
        QTableWidgetItem *newItem = new QTableWidgetItem();
        newItem->setFlags(newItem->flags() | Qt::ItemIsEditable); // Устанавливаем флаг, чтобы ячейка была редактируемой
        ui->tableWidget->setItem(newRow, col, newItem);
    }

    // Добавляем элемент в QListWidget
    ui->listWidget->addItem("new line");
}


void MainWindow::onRemoveItemButtonClicked()
{
    // Получаем индекс текущей выбранной строки в QTableWidget
    int currentRow = ui->tableWidget->currentRow();

    // Проверяем, что строка действительно выбрана
    if (currentRow != -1) {
        // Спрашиваем пользователя, хочет ли он удалить содержимое ячейки или всю строку
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Удаление", "Вы хотите удалить содержимое ячейки (Yes) или всю строку (No)?",
                                      QMessageBox::Yes|QMessageBox::No);

        if (reply == QMessageBox::Yes) {
            // Получаем индекс текущей выбранной ячейки
            QModelIndex currentIndex = ui->tableWidget->currentIndex();
            if (currentIndex.isValid()) {
                // Очищаем содержимое ячейки в QTableWidget
                ui->tableWidget->setItem(currentIndex.row(), currentIndex.column(), new QTableWidgetItem(""));
            }
        } else {
            // Полностью удаляем строку
            ui->tableWidget->removeRow(currentRow);

            // Удаляем элемент из QListWidget, если он связан со строкой
            if (currentRow < ui->listWidget->count()) {
                delete ui->listWidget->takeItem(currentRow);
            }
        }
    } else {
        QMessageBox::warning(this, "Ошибка", "Пожалуйста, выберите строку для удаления");
    }
}


void MainWindow::onSortListButtonClicked()
{
    ui->tableWidget->sortItems(0, Qt::AscendingOrder);
}

void MainWindow::onCountSortButtonClicked()
{
    HoaresortTable();
}

void MainWindow::HoaresortTable()
{
    int rowCount = ui->tableWidget->rowCount();
    if (rowCount == 0)
        return;

    QVector<QVector<QTableWidgetItem*>> rows;
    for (int row = 0; row < rowCount; ++row) {
        QVector<QTableWidgetItem*> rowItems;
        for (int col = 0; col < ui->tableWidget->columnCount(); ++col) {
            rowItems.append(ui->tableWidget->takeItem(row, col));
        }
        rows.append(rowItems);
    }

    quickSort(rows, 0, rows.size() - 1);

    for (int row = 0; row < rowCount; ++row) {
        for (int col = 0; col < ui->tableWidget->columnCount(); ++col) {
            ui->tableWidget->setItem(row, col, rows[row][col]);
        }
    }
}

void MainWindow::quickSort(QVector<QVector<QTableWidgetItem*>> &rows, int low, int high)
{
    if (low < high) {
        int pi = partition(rows, low, high);
        quickSort(rows, low, pi - 1);
        quickSort(rows, pi + 1, high);
    }
}

int MainWindow::partition(QVector<QVector<QTableWidgetItem*>> &rows, int low, int high)
{
    QString pivot = rows[high][0]->text();
    int i = (low - 1);

    for (int j = low; j < high; ++j) {
        if (rows[j][0]->text() < pivot) {
            ++i;
            swap(rows[i], rows[j]);
        }
    }
    swap(rows[i + 1], rows[high]);
    return (i + 1);
}


bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == ui->loadButton && event->type() == QEvent::MouseButtonPress) {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
        if (mouseEvent->button() == Qt::RightButton) {
            ui->loadButton->hide();
            return true;
        }
    }
    return QMainWindow::eventFilter(watched, event);
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
    qDebug() << event;
    QMainWindow::keyPressEvent(event);
}
