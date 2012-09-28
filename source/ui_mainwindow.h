/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created: Wed Sep 26 00:04:00 2012
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QProgressBar>
#include <QtGui/QPushButton>
#include <QtGui/QSlider>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *glLayout;
    QSlider *subdivisionSlider;
    QLabel *label;
    QLabel *subdLevelLabel;
    QLabel *label_2;
    QLabel *faceCountLabel;
    QPushButton *loadObjButton;
    QPushButton *loadLstButton;
    QLabel *countLabel;
    QPushButton *exportObjButton;
    QPushButton *subDivPlus;
    QPushButton *subDivPlus_2;
    QLabel *label_3;
    QPushButton *loadLstButton_2;
    QPushButton *loadLstButton_3;
    QCheckBox *texCheckBox;
    QCheckBox *meshCheckBox;
    QCheckBox *meshCheckBox_2;
    QCheckBox *wireFrameCheckBox;
    QSlider *segmentSlider;
    QLabel *label_4;
    QSlider *countSlider;
    QCheckBox *meshCheckBox_3;
    QCheckBox *simplifyCheckBox;
    QCheckBox *simplifyCheckBox_2;
    QSlider *segmentSlider_2;
    QLabel *label_5;
    QProgressBar *progressBar;
    QLabel *label_6;
    QLabel *label_7;
    QLabel *label_8;
    QCheckBox *simplifyCheckBox_3;
    QCheckBox *simplifyCheckBox_4;
    QCheckBox *simplifyCheckBox_5;
    QPushButton *pushButton;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(979, 572);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        verticalLayoutWidget = new QWidget(centralWidget);
        verticalLayoutWidget->setObjectName(QString::fromUtf8("verticalLayoutWidget"));
        verticalLayoutWidget->setGeometry(QRect(20, 0, 541, 471));
        glLayout = new QVBoxLayout(verticalLayoutWidget);
        glLayout->setSpacing(6);
        glLayout->setContentsMargins(11, 11, 11, 11);
        glLayout->setObjectName(QString::fromUtf8("glLayout"));
        glLayout->setContentsMargins(0, 0, 0, 0);
        subdivisionSlider = new QSlider(centralWidget);
        subdivisionSlider->setObjectName(QString::fromUtf8("subdivisionSlider"));
        subdivisionSlider->setGeometry(QRect(630, 190, 271, 29));
        subdivisionSlider->setMaximum(5);
        subdivisionSlider->setOrientation(Qt::Horizontal);
        subdivisionSlider->setTickPosition(QSlider::TicksBelow);
        label = new QLabel(centralWidget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(720, 160, 41, 31));
        subdLevelLabel = new QLabel(centralWidget);
        subdLevelLabel->setObjectName(QString::fromUtf8("subdLevelLabel"));
        subdLevelLabel->setGeometry(QRect(770, 160, 31, 31));
        label_2 = new QLabel(centralWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(820, 160, 41, 31));
        faceCountLabel = new QLabel(centralWidget);
        faceCountLabel->setObjectName(QString::fromUtf8("faceCountLabel"));
        faceCountLabel->setGeometry(QRect(870, 160, 101, 31));
        loadObjButton = new QPushButton(centralWidget);
        loadObjButton->setObjectName(QString::fromUtf8("loadObjButton"));
        loadObjButton->setGeometry(QRect(570, 380, 131, 31));
        loadLstButton = new QPushButton(centralWidget);
        loadLstButton->setObjectName(QString::fromUtf8("loadLstButton"));
        loadLstButton->setGeometry(QRect(840, 380, 131, 31));
        countLabel = new QLabel(centralWidget);
        countLabel->setObjectName(QString::fromUtf8("countLabel"));
        countLabel->setGeometry(QRect(920, 321, 41, 21));
        exportObjButton = new QPushButton(centralWidget);
        exportObjButton->setObjectName(QString::fromUtf8("exportObjButton"));
        exportObjButton->setGeometry(QRect(700, 380, 141, 31));
        subDivPlus = new QPushButton(centralWidget);
        subDivPlus->setObjectName(QString::fromUtf8("subDivPlus"));
        subDivPlus->setGeometry(QRect(910, 190, 41, 27));
        subDivPlus_2 = new QPushButton(centralWidget);
        subDivPlus_2->setObjectName(QString::fromUtf8("subDivPlus_2"));
        subDivPlus_2->setGeometry(QRect(580, 190, 41, 27));
        label_3 = new QLabel(centralWidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(580, 160, 171, 31));
        loadLstButton_2 = new QPushButton(centralWidget);
        loadLstButton_2->setObjectName(QString::fromUtf8("loadLstButton_2"));
        loadLstButton_2->setGeometry(QRect(770, 420, 201, 51));
        loadLstButton_3 = new QPushButton(centralWidget);
        loadLstButton_3->setObjectName(QString::fromUtf8("loadLstButton_3"));
        loadLstButton_3->setGeometry(QRect(570, 420, 201, 51));
        texCheckBox = new QCheckBox(centralWidget);
        texCheckBox->setObjectName(QString::fromUtf8("texCheckBox"));
        texCheckBox->setGeometry(QRect(570, 0, 171, 22));
        meshCheckBox = new QCheckBox(centralWidget);
        meshCheckBox->setObjectName(QString::fromUtf8("meshCheckBox"));
        meshCheckBox->setGeometry(QRect(700, 0, 111, 22));
        meshCheckBox->setChecked(true);
        meshCheckBox_2 = new QCheckBox(centralWidget);
        meshCheckBox_2->setObjectName(QString::fromUtf8("meshCheckBox_2"));
        meshCheckBox_2->setGeometry(QRect(570, 30, 111, 22));
        meshCheckBox_2->setChecked(true);
        wireFrameCheckBox = new QCheckBox(centralWidget);
        wireFrameCheckBox->setObjectName(QString::fromUtf8("wireFrameCheckBox"));
        wireFrameCheckBox->setGeometry(QRect(830, 0, 111, 22));
        wireFrameCheckBox->setChecked(false);
        segmentSlider = new QSlider(centralWidget);
        segmentSlider->setObjectName(QString::fromUtf8("segmentSlider"));
        segmentSlider->setGeometry(QRect(580, 260, 361, 29));
        segmentSlider->setMinimum(0);
        segmentSlider->setMaximum(20);
        segmentSlider->setValue(10);
        segmentSlider->setTracking(true);
        segmentSlider->setOrientation(Qt::Horizontal);
        segmentSlider->setTickPosition(QSlider::TicksBelow);
        segmentSlider->setTickInterval(1);
        label_4 = new QLabel(centralWidget);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(580, 240, 171, 31));
        countSlider = new QSlider(centralWidget);
        countSlider->setObjectName(QString::fromUtf8("countSlider"));
        countSlider->setGeometry(QRect(580, 320, 331, 29));
        countSlider->setMinimum(0);
        countSlider->setMaximum(150);
        countSlider->setValue(50);
        countSlider->setOrientation(Qt::Horizontal);
        meshCheckBox_3 = new QCheckBox(centralWidget);
        meshCheckBox_3->setObjectName(QString::fromUtf8("meshCheckBox_3"));
        meshCheckBox_3->setGeometry(QRect(700, 30, 111, 22));
        meshCheckBox_3->setChecked(true);
        simplifyCheckBox = new QCheckBox(centralWidget);
        simplifyCheckBox->setObjectName(QString::fromUtf8("simplifyCheckBox"));
        simplifyCheckBox->setGeometry(QRect(830, 30, 131, 22));
        simplifyCheckBox->setChecked(true);
        simplifyCheckBox_2 = new QCheckBox(centralWidget);
        simplifyCheckBox_2->setObjectName(QString::fromUtf8("simplifyCheckBox_2"));
        simplifyCheckBox_2->setGeometry(QRect(580, 300, 221, 31));
        simplifyCheckBox_2->setChecked(true);
        segmentSlider_2 = new QSlider(centralWidget);
        segmentSlider_2->setObjectName(QString::fromUtf8("segmentSlider_2"));
        segmentSlider_2->setGeometry(QRect(580, 110, 361, 29));
        segmentSlider_2->setMinimum(3);
        segmentSlider_2->setMaximum(17);
        segmentSlider_2->setValue(8);
        segmentSlider_2->setTracking(true);
        segmentSlider_2->setOrientation(Qt::Horizontal);
        segmentSlider_2->setTickPosition(QSlider::TicksBelow);
        segmentSlider_2->setTickInterval(1);
        label_5 = new QLabel(centralWidget);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(580, 90, 261, 31));
        progressBar = new QProgressBar(centralWidget);
        progressBar->setObjectName(QString::fromUtf8("progressBar"));
        progressBar->setGeometry(QRect(20, 480, 951, 16));
        progressBar->setValue(100);
        label_6 = new QLabel(centralWidget);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setGeometry(QRect(130, 500, 171, 31));
        label_7 = new QLabel(centralWidget);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setGeometry(QRect(450, 500, 171, 31));
        label_8 = new QLabel(centralWidget);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setGeometry(QRect(270, 500, 171, 31));
        simplifyCheckBox_3 = new QCheckBox(centralWidget);
        simplifyCheckBox_3->setObjectName(QString::fromUtf8("simplifyCheckBox_3"));
        simplifyCheckBox_3->setGeometry(QRect(570, 60, 131, 22));
        simplifyCheckBox_3->setChecked(false);
        simplifyCheckBox_4 = new QCheckBox(centralWidget);
        simplifyCheckBox_4->setObjectName(QString::fromUtf8("simplifyCheckBox_4"));
        simplifyCheckBox_4->setGeometry(QRect(830, 60, 221, 22));
        simplifyCheckBox_4->setChecked(true);
        simplifyCheckBox_5 = new QCheckBox(centralWidget);
        simplifyCheckBox_5->setObjectName(QString::fromUtf8("simplifyCheckBox_5"));
        simplifyCheckBox_5->setEnabled(true);
        simplifyCheckBox_5->setGeometry(QRect(700, 60, 121, 22));
        simplifyCheckBox_5->setChecked(false);
        pushButton = new QPushButton(centralWidget);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(20, 500, 98, 27));
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 979, 25));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("MainWindow", "level :", 0, QApplication::UnicodeUTF8));
        subdLevelLabel->setText(QApplication::translate("MainWindow", "0", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("MainWindow", "Faces:", 0, QApplication::UnicodeUTF8));
        faceCountLabel->setText(QApplication::translate("MainWindow", "0", 0, QApplication::UnicodeUTF8));
        loadObjButton->setText(QApplication::translate("MainWindow", "Load .obj...", 0, QApplication::UnicodeUTF8));
        loadLstButton->setText(QApplication::translate("MainWindow", "Load .lst...", 0, QApplication::UnicodeUTF8));
        countLabel->setText(QApplication::translate("MainWindow", "0", 0, QApplication::UnicodeUTF8));
        exportObjButton->setText(QApplication::translate("MainWindow", "Export .obj...", 0, QApplication::UnicodeUTF8));
        subDivPlus->setText(QApplication::translate("MainWindow", "+", 0, QApplication::UnicodeUTF8));
        subDivPlus_2->setText(QApplication::translate("MainWindow", "-", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("MainWindow", "Surface Subdivision", 0, QApplication::UnicodeUTF8));
        loadLstButton_2->setText(QApplication::translate("MainWindow", "Next .lst >>", 0, QApplication::UnicodeUTF8));
        loadLstButton_3->setText(QApplication::translate("MainWindow", "<< Previous .lst", 0, QApplication::UnicodeUTF8));
        texCheckBox->setText(QApplication::translate("MainWindow", "Show Texture", 0, QApplication::UnicodeUTF8));
        meshCheckBox->setText(QApplication::translate("MainWindow", "Show Mesh", 0, QApplication::UnicodeUTF8));
        meshCheckBox_2->setText(QApplication::translate("MainWindow", "Show Graph", 0, QApplication::UnicodeUTF8));
        wireFrameCheckBox->setText(QApplication::translate("MainWindow", "Wire Frame", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("MainWindow", "Subdivision control", 0, QApplication::UnicodeUTF8));
        meshCheckBox_3->setText(QApplication::translate("MainWindow", "Tight Joints", 0, QApplication::UnicodeUTF8));
        simplifyCheckBox->setText(QApplication::translate("MainWindow", "Simplify Graph", 0, QApplication::UnicodeUTF8));
        simplifyCheckBox_2->setText(QApplication::translate("MainWindow", "Algorithm Progress Control", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("MainWindow", "Number of Sides", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("MainWindow", "Left Click: Pivot", 0, QApplication::UnicodeUTF8));
        label_7->setText(QApplication::translate("MainWindow", "Right Click: Pan", 0, QApplication::UnicodeUTF8));
        label_8->setText(QApplication::translate("MainWindow", "Mouse Wheel: Zoom", 0, QApplication::UnicodeUTF8));
        simplifyCheckBox_3->setText(QApplication::translate("MainWindow", "Retriangulate", 0, QApplication::UnicodeUTF8));
        simplifyCheckBox_4->setText(QApplication::translate("MainWindow", "Alternate Tris", 0, QApplication::UnicodeUTF8));
        simplifyCheckBox_5->setText(QApplication::translate("MainWindow", "Shade Smooth", 0, QApplication::UnicodeUTF8));
        pushButton->setText(QApplication::translate("MainWindow", "Reset Camera", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
