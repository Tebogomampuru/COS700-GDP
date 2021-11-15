#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QHBoxLayout *hLayout3 = new QHBoxLayout;
    QHBoxLayout *hLayout4 = new QHBoxLayout;
    QHBoxLayout *hLayout5 = new QHBoxLayout;

    hspinBox1 = new QSpinBox;
    hspinBox2 = new QSpinBox;

    okbutton = new QPushButton;
    okbutton->setText("OK");

    pmlbutton = new QPushButton;
    pmlbutton->setText("Generate PML");

    QLabel * num_phil = new QLabel("Number of dining philosophers");
    QLabel * num_res = new QLabel("Number of available resources");

    hLayout3->addWidget(num_phil);
    hLayout3->addWidget(hspinBox1);
    hLayout4->addWidget(num_res);
    hLayout4->addWidget(hspinBox2);
    hLayout5->addWidget(okbutton);

    ui->verticalLayout_2->addLayout(hLayout3);
    ui->verticalLayout_2->addLayout(hLayout4);
    ui->verticalLayout_2->addLayout(hLayout5);

    QObject::connect(okbutton,SIGNAL(clicked()),this, SLOT(changeGUI()));



}


void MainWindow::changeGUI()
{
    okbutton->setEnabled(false);
    hspinBox1->setEnabled(false);
    hspinBox2->setEnabled(false);

    for(int k = 0; k<hspinBox1->value(); k++)
    {
        QHBoxLayout *hLayout = new QHBoxLayout;
        QHBoxLayout *hLayoutHeading = new QHBoxLayout;
        QHBoxLayout *hLayout2 = new QHBoxLayout;
        QSpinBox *hspinBox3 = new QSpinBox;

        QLabel * res_needed = new QLabel(QString("Minimum number of resources needed by philosopher_%1\t").arg(k+1));
        hLayout2->addWidget(res_needed);
        hLayout2->addWidget(hspinBox3);
        qspinbox.append(hspinBox3);
        ui->verticalLayout_2->addLayout(hLayout2);


        if (hspinBox1->value() > 0 && hspinBox2->value() >0 )
        {
            QHBoxLayout *hLayout6 = new QHBoxLayout;
            hLayout6->addWidget(pmlbutton);
            ui->gridLayout->addLayout(hLayout6,hspinBox2->value() + 1, 1);
            QObject::connect(pmlbutton,SIGNAL(clicked()),this, SLOT(changeGUI2()));

        }

        QLabel * heading = new QLabel("Philosophers \t\t\t\t Resources accessable by Philosopher");
        if (k == 0)
        {
            hLayoutHeading->addWidget(heading);
            ui->gridLayout->addLayout(hLayoutHeading,hspinBox2->value()+2,1);
        }

        for(int i = 0; i<hspinBox2->value(); i++)
        {
            QCheckBox * box = new QCheckBox;
            QLabel * jj = new QLabel(QString("Resources accessable by Philosophers_%1\t").arg(k+1));
            if (i == 0)
                hLayout->addWidget(jj);
            hLayout->addWidget(box);

            qcheckbox.append(box);
        }
        ui->gridLayout->addLayout(hLayout,hspinBox2->value() + k+3,1);
    }

    if (hspinBox1->value() == 0 || hspinBox2->value() == 0 )
    {
        QPlainTextEdit *outputScreen1 = new QPlainTextEdit;
        outputScreen1->setReadOnly(true);
        outputScreen1->appendPlainText("The number of dining Philosphers and available resources should be greater than zero");
        QHBoxLayout *hLayout6 = new QHBoxLayout;
        hLayout6->addWidget(outputScreen1);
        ui->gridLayout->addLayout(hLayout6,hspinBox2->value() + 1, 1);

    }
}

void MainWindow::changeGUI2()
{
    pmlbutton->setEnabled(false);
    QPlainTextEdit *outputScreen2 = new QPlainTextEdit;
    outputScreen2->setReadOnly(true);

    QHBoxLayout *hLayout7 = new QHBoxLayout;
    hLayout7->addWidget(outputScreen2);
    ui->gridLayout->addLayout(hLayout7,hspinBox2->value() + 2, 1);

    QVector<QString> accessablity_matrix;

    QString entry;
    for (int k = 0; k <hspinBox1->value(); k++ )
    {
        entry.clear();

        for (int j = hspinBox2->value()*k; j <(hspinBox2->value()+k*hspinBox2->value()); j++ )
        {
            if (j == hspinBox2->value()*k && qcheckbox.at(j)->isChecked() == true)
                entry.append("{1");
            if (j == hspinBox2->value()*k && qcheckbox.at(j)->isChecked() == false)
                entry.append("{0");

            if (j > hspinBox2->value()*k && qcheckbox.at(j)->isChecked() == true)
            {
                entry.append(",    1");
            }
            if (j > hspinBox2->value()*k && qcheckbox.at(j)->isChecked() == false)
            {
                entry.append(",    0");
            }
        }

        entry.append("}");


        accessablity_matrix.append(entry);

    }



        //Create global variables
        outputScreen2->appendPlainText("mtype = {release_all, idle");
        for (int k=0; k<hspinBox2->value(); k++)
        {

            outputScreen2->moveCursor(QTextCursor::End);
            outputScreen2->insertPlainText(QString(", req%1").arg(k));
            outputScreen2->moveCursor(QTextCursor::End);
        }

        for (int k=0; k<hspinBox2->value(); k++)
        {
            outputScreen2->moveCursor(QTextCursor::End);
            outputScreen2->insertPlainText(QString(", rel%1").arg(k));
            outputScreen2->moveCursor(QTextCursor::End);
        }

        outputScreen2->moveCursor(QTextCursor::End);
        outputScreen2->insertPlainText(" };\n\n//Channels");
        outputScreen2->moveCursor(QTextCursor::End);

        for (int k=1; k<=hspinBox1->value(); k++)
        {
            outputScreen2->appendPlainText(QString("chan action_%1 = [0] of {mtype};").arg(k));
        }

        outputScreen2->appendPlainText("");
        for (int k=1; k<=hspinBox1->value(); k++)
        {
            outputScreen2->appendPlainText(QString("mtype action%1;").arg(k));
        }

        outputScreen2->appendPlainText(QString("\nint maxRes = %1;                                                                                                    //Resourses available for use")
                                       .arg(hspinBox2->value()));
        outputScreen2->appendPlainText("int resource_tracker [maxRes];                                                                                     //Initialise the list of resources as unassigned");
        outputScreen2->appendPlainText("mtype request [maxRes]                = {req0");

        for (int k=1; k<hspinBox2->value(); k++)
        {
            outputScreen2->moveCursor(QTextCursor::End);
            outputScreen2->insertPlainText(QString(", req%1").arg(k));
            outputScreen2->moveCursor(QTextCursor::End);
        }
        outputScreen2->moveCursor(QTextCursor::End);
        outputScreen2->insertPlainText("};               //List of requests that can be made\nmtype release [maxRes]                = {rel0");
        outputScreen2->moveCursor(QTextCursor::End);

        for (int k=1; k<hspinBox2->value(); k++)
        {
            outputScreen2->moveCursor(QTextCursor::End);
            outputScreen2->insertPlainText(QString(", rel%1").arg(k));
            outputScreen2->moveCursor(QTextCursor::End);
        }
        outputScreen2->moveCursor(QTextCursor::End);
        outputScreen2->insertPlainText("};               //List of resources that can be released\n");
        outputScreen2->moveCursor(QTextCursor::End);

        outputScreen2->appendPlainText("");

        int y=0;
        for (QString val : accessablity_matrix)
        {
            y = y+1;
            outputScreen2->appendPlainText(QString("int Res_Accessable_by_phil_%1 [maxRes] = %2;            //Resources accessable by agent-%3. NB: 0 means the agent cannot access that resource")
                                       .arg(y).arg(val)
                                       .arg(y));
        }


        outputScreen2->appendPlainText("\nint i, phil_1_gained_res");
        for (int k=2; k<=hspinBox1->value(); k++)
        {
            outputScreen2->moveCursor(QTextCursor::End);
            outputScreen2->insertPlainText(QString(", phil_%1_gained_res;").arg(k));
            outputScreen2->moveCursor(QTextCursor::End);
        }


        outputScreen2->appendPlainText("");
        for (int k=0; k<hspinBox1->value(); k++)
        {
            outputScreen2->appendPlainText(QString("int Min_res_for_phil_%1 = %2;").arg(k+1).arg(qspinbox[k]->value()));
        }

        outputScreen2->appendPlainText("int roundStartFlag=1;\n\n");
        //Create Philosopher process
        for (int k=0; k<hspinBox1->value(); k++)
        {
            outputScreen2->appendPlainText(QString("active proctype Phil_%1()\n{\n\tRoundStarts_%1:\n\tdo\n\t::  phil_%1_gained_res < Min_res_for_phil_%1\n\t    do").arg(k+1));

            for (int i=0; i<hspinBox2->value(); i++)
            {
                outputScreen2->appendPlainText(QString("\t    ::  (Res_Accessable_by_phil_%1[%2] == 1 && resource_tracker[%2] == 0) -> action_%1!request[%2] -> break;")
                                               .arg(k+1)
                                               .arg(i));
            }

            outputScreen2->appendPlainText("");
            for (int i=0; i<hspinBox2->value(); i++)
            {
                outputScreen2->appendPlainText(QString("\t    ::  (resource_tracker[%2] == %1) -> action_%1!release[%2] -> break;")
                                                       .arg(k+1)
                                                       .arg(i));
            }
            outputScreen2->appendPlainText(QString("\t    ::  action_%1!idle -> break;\n\t    od\n").arg(k+1));
            outputScreen2->appendPlainText(QString("\t::  (phil_%1_gained_res == Min_res_for_phil_%1) -> action_%1!release_all -> break;\n\tod\n").arg(k+1));

            outputScreen2->appendPlainText(QString("\tdo\n\t\t:: (roundStartFlag==1)-> goto RoundStarts_%1\n\tod\n\n}\n\n").arg(k+1));
        }

        //Create Environment process

        outputScreen2->appendPlainText("active proctype Env()\n{\n\tRoundStarts:\n\n\t\troundStartFlag = 1\n\t\tatomic\n\t\t{");
        for (int k=1; k<=hspinBox1->value(); k++)
        {
            outputScreen2->appendPlainText(QString("\t\t\taction_%1?action%1").arg(k));
        }

        outputScreen2->appendPlainText("\n\t\t\troundStartFlag = 0\n\t\t}\n\n""\n\t\tif\n\t\t::  (true)\n\t\t    for (i : 0.. maxRes - 1 ){\n  \t\t\tif");
        QString operand_k;
        for (int k=1; k<=hspinBox1->value(); k++)
        {
            if (k == 1)
                outputScreen2->appendPlainText("\n  \t\t\t\t:: (action1 == request[i]");
            if (k != 1)
                outputScreen2->appendPlainText("\n  \t\t\t\t:: (action1 != request[i]");

            for (int z=2; z<=hspinBox1->value(); z++)
            {
                if (z == k)
                    operand_k = " == ";
                else
                    operand_k = " != ";
                outputScreen2->moveCursor(QTextCursor::End);
                outputScreen2->insertPlainText(QString(" && action%1%2request[i]")
                                               .arg(z).arg(operand_k));
                outputScreen2->moveCursor(QTextCursor::End);
             }

            outputScreen2->moveCursor(QTextCursor::End);
            outputScreen2->insertPlainText(QString(" && phil_%1_gained_res < Min_res_for_phil_%1)\n\t\t\t\t   resource_tracker[i] = %1;\n\t\t\t\t   phil_%1_gained_res = phil_%1_gained_res + 1;\n\t\t\t\t   printf\(\"Philosopher-%1 gained %e\\n\", release[i]);\n\t\t\t\t   break;")
                                           .arg(k));
            outputScreen2->moveCursor(QTextCursor::End);

        }

        for (int k=1; k<=hspinBox1->value(); k++)
        {
            outputScreen2->appendPlainText(QString("\n  \t\t\t\t:: (action%1 == release[i] && resource_tracker[i] == %1)\n\t\t\t\t   printf\(\"Philosopher-%1 released %e\\n\", release[i]);\n\t\t\t\t   resource_tracker[i] = 0;\n\t\t\t\t   phil_%1_gained_res = phil_%1_gained_res - 1;\n\t\t\t\t   break;")
                                           .arg(k));
        }

        outputScreen2->appendPlainText("\n  \t\t\t\t:: else -> skip;\n  \t\t\tfi\n\t\t    }\n\t\tfi\n\n");

        for (int k=1; k<=hspinBox1->value(); k++)
        {
            outputScreen2->appendPlainText(QString("\t\tif\n\t\t::  action%1 == release_all\n\t\t    for (i : 0.. maxRes - 1 ){\n\t\t    \tif\n\t\t    \t:: resource_tracker[i] == %1 -> resource_tracker[i] = 0;\n\t\t    \t:: else -> skip;\n\t\t    \tfi\n\t\t    }\n\t\t    phil_%1_gained_res = 0;\n")
                                           .arg(k));
            outputScreen2->appendPlainText("\t\t::  else -> skip;\n\t\tfi\n\n");
        }


        outputScreen2->appendPlainText("\n\t\tgoto RoundStarts\n}\n\nltl notLive {<>([](phil_1_gained_res < Min_res_for_phil_1))");
        for (int k=2; k<=hspinBox1->value(); k++)
        {
            outputScreen2->moveCursor(QTextCursor::End);
            outputScreen2->insertPlainText(QString(" || <>([](phil_%1_gained_res == Min_res_for_phil_%1))").arg(k));
            outputScreen2->moveCursor(QTextCursor::End);
        }
        outputScreen2->moveCursor(QTextCursor::End);
        outputScreen2->insertPlainText(" }\n\n");
        outputScreen2->moveCursor(QTextCursor::End);


}

MainWindow::~MainWindow()
{
    delete ui;
}
