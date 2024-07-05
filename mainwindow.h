#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QPushButton>
#include <QListWidget>
#include <QComboBox>
#include <QEvent>
#include <QKeyEvent>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
protected:
    void keyPressEvent(QKeyEvent *event) override;
private slots:
    void onLoadButtonClicked();
    void onAddItemButtonClicked();
    void onRemoveItemButtonClicked();
    void onSortListButtonClicked();
    void onCountSortButtonClicked();
    void HoaresortTable();
    void quickSort(QVector<QVector<QTableWidgetItem*>> &rows, int low, int high);
    int partition(QVector<QVector<QTableWidgetItem*>> &rows, int low, int high);
    bool eventFilter(QObject *watched, QEvent *event);

private:
    Ui::MainWindow *ui;
    void loadPhoneNumbers(const QString &fileName);
    QWidget *blueWidget;
};

#endif // MAINWINDOW_H
