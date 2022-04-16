/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.15.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QLabel *lblImg;
    QPlainTextEdit *edtConsole;
    QGroupBox *groupBox;
    QPushButton *btnGen;
    QSpinBox *spnSiz;
    QRadioButton *radAlgo2;
    QRadioButton *radAlgo3;
    QCheckBox *chkLops;
    QRadioButton *radAlgo1;
    QLabel *label;
    QGroupBox *groupBox_2;
    QPushButton *btnSolv;
    QGroupBox *grpClrSet;
    QPushButton *btnClrRst;
    QLabel *label_2;
    QPushButton *btnClrWall;
    QLabel *label_3;
    QPushButton *btnClrPath;
    QLabel *label_4;
    QPushButton *btnClrRecPath;
    QLabel *label_5;
    QPushButton *btnClrBfsPath;
    QPushButton *btnClrCros;
    QLabel *label_6;
    QPushButton *btnClrFadeS;
    QLabel *lblFadeS;
    QLabel *lblFadeE;
    QPushButton *btnClrFadeE;
    QRadioButton *radFadeN;
    QRadioButton *radFadeH;
    QRadioButton *radFadeV;
    QCheckBox *chkClrIns;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(1060, 980);
        MainWindow->setMinimumSize(QSize(1060, 980));
        MainWindow->setMaximumSize(QSize(1060, 980));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        lblImg = new QLabel(centralwidget);
        lblImg->setObjectName(QString::fromUtf8("lblImg"));
        lblImg->setGeometry(QRect(250, 10, 800, 800));
        lblImg->setFrameShape(QFrame::Box);
        lblImg->setFrameShadow(QFrame::Plain);
        lblImg->setAlignment(Qt::AlignCenter);
        edtConsole = new QPlainTextEdit(centralwidget);
        edtConsole->setObjectName(QString::fromUtf8("edtConsole"));
        edtConsole->setGeometry(QRect(250, 824, 801, 151));
        groupBox = new QGroupBox(centralwidget);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(10, 440, 231, 241));
        btnGen = new QPushButton(groupBox);
        btnGen->setObjectName(QString::fromUtf8("btnGen"));
        btnGen->setGeometry(QRect(10, 200, 211, 31));
        spnSiz = new QSpinBox(groupBox);
        spnSiz->setObjectName(QString::fromUtf8("spnSiz"));
        spnSiz->setGeometry(QRect(130, 160, 91, 31));
        spnSiz->setReadOnly(false);
        spnSiz->setMinimum(0);
        spnSiz->setMaximum(99);
        spnSiz->setSingleStep(1);
        radAlgo2 = new QRadioButton(groupBox);
        radAlgo2->setObjectName(QString::fromUtf8("radAlgo2"));
        radAlgo2->setGeometry(QRect(10, 60, 211, 25));
        radAlgo3 = new QRadioButton(groupBox);
        radAlgo3->setObjectName(QString::fromUtf8("radAlgo3"));
        radAlgo3->setGeometry(QRect(10, 130, 211, 25));
        chkLops = new QCheckBox(groupBox);
        chkLops->setObjectName(QString::fromUtf8("chkLops"));
        chkLops->setEnabled(false);
        chkLops->setGeometry(QRect(40, 90, 106, 25));
        radAlgo1 = new QRadioButton(groupBox);
        radAlgo1->setObjectName(QString::fromUtf8("radAlgo1"));
        radAlgo1->setGeometry(QRect(10, 30, 181, 25));
        radAlgo1->setChecked(true);
        label = new QLabel(groupBox);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(10, 160, 71, 31));
        groupBox_2 = new QGroupBox(centralwidget);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        groupBox_2->setGeometry(QRect(10, 690, 231, 81));
        btnSolv = new QPushButton(groupBox_2);
        btnSolv->setObjectName(QString::fromUtf8("btnSolv"));
        btnSolv->setGeometry(QRect(10, 40, 211, 31));
        grpClrSet = new QGroupBox(centralwidget);
        grpClrSet->setObjectName(QString::fromUtf8("grpClrSet"));
        grpClrSet->setGeometry(QRect(10, 10, 231, 421));
        btnClrRst = new QPushButton(grpClrSet);
        btnClrRst->setObjectName(QString::fromUtf8("btnClrRst"));
        btnClrRst->setGeometry(QRect(10, 380, 211, 28));
        label_2 = new QLabel(grpClrSet);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(10, 30, 141, 31));
        btnClrWall = new QPushButton(grpClrSet);
        btnClrWall->setObjectName(QString::fromUtf8("btnClrWall"));
        btnClrWall->setGeometry(QRect(180, 30, 41, 28));
        label_3 = new QLabel(grpClrSet);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(10, 60, 141, 31));
        btnClrPath = new QPushButton(grpClrSet);
        btnClrPath->setObjectName(QString::fromUtf8("btnClrPath"));
        btnClrPath->setGeometry(QRect(180, 60, 41, 28));
        label_4 = new QLabel(grpClrSet);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(10, 90, 141, 31));
        btnClrRecPath = new QPushButton(grpClrSet);
        btnClrRecPath->setObjectName(QString::fromUtf8("btnClrRecPath"));
        btnClrRecPath->setGeometry(QRect(180, 90, 41, 28));
        label_5 = new QLabel(grpClrSet);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(10, 120, 141, 31));
        btnClrBfsPath = new QPushButton(grpClrSet);
        btnClrBfsPath->setObjectName(QString::fromUtf8("btnClrBfsPath"));
        btnClrBfsPath->setGeometry(QRect(180, 120, 41, 28));
        btnClrCros = new QPushButton(grpClrSet);
        btnClrCros->setObjectName(QString::fromUtf8("btnClrCros"));
        btnClrCros->setGeometry(QRect(180, 150, 41, 28));
        label_6 = new QLabel(grpClrSet);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setGeometry(QRect(10, 150, 141, 31));
        btnClrFadeS = new QPushButton(grpClrSet);
        btnClrFadeS->setObjectName(QString::fromUtf8("btnClrFadeS"));
        btnClrFadeS->setEnabled(false);
        btnClrFadeS->setGeometry(QRect(180, 180, 41, 28));
        lblFadeS = new QLabel(grpClrSet);
        lblFadeS->setObjectName(QString::fromUtf8("lblFadeS"));
        lblFadeS->setEnabled(false);
        lblFadeS->setGeometry(QRect(10, 180, 141, 31));
        lblFadeE = new QLabel(grpClrSet);
        lblFadeE->setObjectName(QString::fromUtf8("lblFadeE"));
        lblFadeE->setEnabled(false);
        lblFadeE->setGeometry(QRect(10, 210, 141, 31));
        btnClrFadeE = new QPushButton(grpClrSet);
        btnClrFadeE->setObjectName(QString::fromUtf8("btnClrFadeE"));
        btnClrFadeE->setEnabled(false);
        btnClrFadeE->setGeometry(QRect(180, 210, 41, 28));
        radFadeN = new QRadioButton(grpClrSet);
        radFadeN->setObjectName(QString::fromUtf8("radFadeN"));
        radFadeN->setEnabled(false);
        radFadeN->setGeometry(QRect(10, 280, 141, 25));
        radFadeN->setChecked(true);
        radFadeH = new QRadioButton(grpClrSet);
        radFadeH->setObjectName(QString::fromUtf8("radFadeH"));
        radFadeH->setEnabled(false);
        radFadeH->setGeometry(QRect(10, 310, 151, 25));
        radFadeV = new QRadioButton(grpClrSet);
        radFadeV->setObjectName(QString::fromUtf8("radFadeV"));
        radFadeV->setEnabled(false);
        radFadeV->setGeometry(QRect(10, 340, 151, 25));
        chkClrIns = new QCheckBox(grpClrSet);
        chkClrIns->setObjectName(QString::fromUtf8("chkClrIns"));
        chkClrIns->setGeometry(QRect(10, 240, 106, 25));
        MainWindow->setCentralWidget(centralwidget);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        lblImg->setText(QCoreApplication::translate("MainWindow", "Image", nullptr));
        groupBox->setTitle(QCoreApplication::translate("MainWindow", "Generate", nullptr));
        btnGen->setText(QCoreApplication::translate("MainWindow", "Generate", nullptr));
        radAlgo2->setText(QCoreApplication::translate("MainWindow", "Growing tree backtracer", nullptr));
        radAlgo3->setText(QCoreApplication::translate("MainWindow", "Growing tree Prim", nullptr));
        chkLops->setText(QCoreApplication::translate("MainWindow", "Loops", nullptr));
        radAlgo1->setText(QCoreApplication::translate("MainWindow", "Eller's", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "Size:", nullptr));
        groupBox_2->setTitle(QCoreApplication::translate("MainWindow", "Solve", nullptr));
        btnSolv->setText(QCoreApplication::translate("MainWindow", "Solve", nullptr));
        grpClrSet->setTitle(QCoreApplication::translate("MainWindow", "Color set", nullptr));
        btnClrRst->setText(QCoreApplication::translate("MainWindow", "Reset", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "Walls", nullptr));
        btnClrWall->setText(QString());
        label_3->setText(QCoreApplication::translate("MainWindow", "Path", nullptr));
        btnClrPath->setText(QString());
        label_4->setText(QCoreApplication::translate("MainWindow", "Rec path", nullptr));
        btnClrRecPath->setText(QString());
        label_5->setText(QCoreApplication::translate("MainWindow", "Bfs path", nullptr));
        btnClrBfsPath->setText(QString());
        btnClrCros->setText(QString());
        label_6->setText(QCoreApplication::translate("MainWindow", "Crossover", nullptr));
        btnClrFadeS->setText(QString());
        lblFadeS->setText(QCoreApplication::translate("MainWindow", "Fade start", nullptr));
        lblFadeE->setText(QCoreApplication::translate("MainWindow", "Fade end", nullptr));
        btnClrFadeE->setText(QString());
        radFadeN->setText(QCoreApplication::translate("MainWindow", "No fade", nullptr));
        radFadeH->setText(QCoreApplication::translate("MainWindow", "Horizontal fade", nullptr));
        radFadeV->setText(QCoreApplication::translate("MainWindow", "Vertical fade", nullptr));
        chkClrIns->setText(QCoreApplication::translate("MainWindow", "Insanity", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
