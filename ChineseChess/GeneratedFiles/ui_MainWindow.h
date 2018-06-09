/********************************************************************************
** Form generated from reading UI file 'MainWindow.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindowClass
{
public:
    QWidget *centralWidget;
    QTextEdit *textEdit_rMv;
    QTextEdit *textEdit_bMv;
    QLabel *label_red;
    QLabel *label_black;
    QPushButton *pushButton_undo;
    QComboBox *cb_gamePattern;
    QTextEdit *textEdit_msg;
    QPushButton *pushButton_send;
    QTextEdit *textEdit_send;
    QLabel *label_CR;
    QLabel *label_SM;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindowClass)
    {
        if (MainWindowClass->objectName().isEmpty())
            MainWindowClass->setObjectName(QStringLiteral("MainWindowClass"));
        MainWindowClass->resize(1242, 1024);
        centralWidget = new QWidget(MainWindowClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        textEdit_rMv = new QTextEdit(centralWidget);
        textEdit_rMv->setObjectName(QStringLiteral("textEdit_rMv"));
        textEdit_rMv->setGeometry(QRect(920, 30, 161, 330));
        textEdit_rMv->setUndoRedoEnabled(false);
        textEdit_rMv->setReadOnly(true);
        textEdit_bMv = new QTextEdit(centralWidget);
        textEdit_bMv->setObjectName(QStringLiteral("textEdit_bMv"));
        textEdit_bMv->setGeometry(QRect(1080, 30, 151, 330));
        textEdit_bMv->setUndoRedoEnabled(false);
        textEdit_bMv->setReadOnly(true);
        label_red = new QLabel(centralWidget);
        label_red->setObjectName(QStringLiteral("label_red"));
        label_red->setGeometry(QRect(970, 10, 72, 15));
        label_black = new QLabel(centralWidget);
        label_black->setObjectName(QStringLiteral("label_black"));
        label_black->setGeometry(QRect(1130, 10, 72, 15));
        pushButton_undo = new QPushButton(centralWidget);
        pushButton_undo->setObjectName(QStringLiteral("pushButton_undo"));
        pushButton_undo->setGeometry(QRect(1160, 930, 80, 30));
        cb_gamePattern = new QComboBox(centralWidget);
        cb_gamePattern->setObjectName(QStringLiteral("cb_gamePattern"));
        cb_gamePattern->setGeometry(QRect(920, 370, 310, 40));
        textEdit_msg = new QTextEdit(centralWidget);
        textEdit_msg->setObjectName(QStringLiteral("textEdit_msg"));
        textEdit_msg->setGeometry(QRect(920, 460, 310, 240));
        textEdit_msg->setUndoRedoEnabled(false);
        textEdit_msg->setReadOnly(true);
        pushButton_send = new QPushButton(centralWidget);
        pushButton_send->setObjectName(QStringLiteral("pushButton_send"));
        pushButton_send->setGeometry(QRect(920, 880, 310, 40));
        textEdit_send = new QTextEdit(centralWidget);
        textEdit_send->setObjectName(QStringLiteral("textEdit_send"));
        textEdit_send->setGeometry(QRect(920, 770, 310, 87));
        label_CR = new QLabel(centralWidget);
        label_CR->setObjectName(QStringLiteral("label_CR"));
        label_CR->setGeometry(QRect(920, 440, 160, 15));
        label_SM = new QLabel(centralWidget);
        label_SM->setObjectName(QStringLiteral("label_SM"));
        label_SM->setGeometry(QRect(920, 740, 150, 15));
        MainWindowClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindowClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1242, 26));
        MainWindowClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindowClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindowClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindowClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindowClass->setStatusBar(statusBar);

        retranslateUi(MainWindowClass);

        QMetaObject::connectSlotsByName(MainWindowClass);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindowClass)
    {
        MainWindowClass->setWindowTitle(QApplication::translate("MainWindowClass", "MainWindow", Q_NULLPTR));
        label_red->setText(QApplication::translate("MainWindowClass", "\347\272\242\346\226\271", Q_NULLPTR));
        label_black->setText(QApplication::translate("MainWindowClass", "\351\273\221\346\226\271", Q_NULLPTR));
        pushButton_undo->setText(QApplication::translate("MainWindowClass", "\346\202\224\346\243\213", Q_NULLPTR));
        cb_gamePattern->clear();
        cb_gamePattern->insertItems(0, QStringList()
         << QApplication::translate("MainWindowClass", "\350\207\252\346\210\221\345\215\232\345\274\210", Q_NULLPTR)
         << QApplication::translate("MainWindowClass", "\344\272\272\344\272\272\345\257\271\346\210\230", Q_NULLPTR)
         << QApplication::translate("MainWindowClass", "\344\272\272\346\234\272\345\257\271\346\210\230", Q_NULLPTR)
        );
        pushButton_send->setText(QApplication::translate("MainWindowClass", "SendMessage", Q_NULLPTR));
        label_CR->setText(QApplication::translate("MainWindowClass", "Chatting Records:", Q_NULLPTR));
        label_SM->setText(QApplication::translate("MainWindowClass", "Sending Message:", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MainWindowClass: public Ui_MainWindowClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
