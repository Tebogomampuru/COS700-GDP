#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <qvector.h>
#include <qcheckbox.h>
#include <QLabel>
#include <QSpinBox>
#include <QPushButton>
#include <QPlainTextEdit>

using namespace std;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void changeGUI();
    void changeGUI2();

private:
    Ui::MainWindow *ui;
    QSpinBox *hspinBox1;
    QSpinBox *hspinBox2;
    QVector<QSpinBox*> qspinbox;
    QVector<QCheckBox*> qcheckbox;
    QPushButton *okbutton;
    QPushButton *pmlbutton;
};

#endif // MAINWINDOW_H
