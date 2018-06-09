/********************************************************************************
** Form generated from reading UI file 'chatDlgBB.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CHATDLGBB_H
#define UI_CHATDLGBB_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Dialog
{
public:
    QWidget *layoutWidget;
    QHBoxLayout *hboxLayout;
    QSpacerItem *spacerItem;
    QPushButton *okButton;
    QPushButton *cancelButton;
    QWidget *gridLayoutWidget;
    QGridLayout *gridLayout;
    QLabel *label_ip;
    QLabel *label_port;
    QLineEdit *lineEdit_ip;
    QLineEdit *lineEdit_port;
    QGroupBox *groupBox_server;
    QPushButton *pushButton_listen;
    QGroupBox *groupBox;
    QPushButton *pushButton_conn;

    void setupUi(QDialog *Dialog)
    {
        if (Dialog->objectName().isEmpty())
            Dialog->setObjectName(QStringLiteral("Dialog"));
        Dialog->resize(470, 344);
        layoutWidget = new QWidget(Dialog);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        layoutWidget->setGeometry(QRect(100, 300, 351, 33));
        hboxLayout = new QHBoxLayout(layoutWidget);
        hboxLayout->setSpacing(6);
        hboxLayout->setObjectName(QStringLiteral("hboxLayout"));
        hboxLayout->setContentsMargins(0, 0, 0, 0);
        spacerItem = new QSpacerItem(131, 31, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout->addItem(spacerItem);

        okButton = new QPushButton(layoutWidget);
        okButton->setObjectName(QStringLiteral("okButton"));

        hboxLayout->addWidget(okButton);

        cancelButton = new QPushButton(layoutWidget);
        cancelButton->setObjectName(QStringLiteral("cancelButton"));

        hboxLayout->addWidget(cancelButton);

        gridLayoutWidget = new QWidget(Dialog);
        gridLayoutWidget->setObjectName(QStringLiteral("gridLayoutWidget"));
        gridLayoutWidget->setGeometry(QRect(30, 0, 381, 131));
        gridLayout = new QGridLayout(gridLayoutWidget);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        label_ip = new QLabel(gridLayoutWidget);
        label_ip->setObjectName(QStringLiteral("label_ip"));

        gridLayout->addWidget(label_ip, 0, 1, 1, 1);

        label_port = new QLabel(gridLayoutWidget);
        label_port->setObjectName(QStringLiteral("label_port"));

        gridLayout->addWidget(label_port, 1, 1, 1, 1);

        lineEdit_ip = new QLineEdit(gridLayoutWidget);
        lineEdit_ip->setObjectName(QStringLiteral("lineEdit_ip"));

        gridLayout->addWidget(lineEdit_ip, 0, 2, 1, 1);

        lineEdit_port = new QLineEdit(gridLayoutWidget);
        lineEdit_port->setObjectName(QStringLiteral("lineEdit_port"));

        gridLayout->addWidget(lineEdit_port, 1, 2, 1, 1);

        groupBox_server = new QGroupBox(Dialog);
        groupBox_server->setObjectName(QStringLiteral("groupBox_server"));
        groupBox_server->setGeometry(QRect(10, 150, 211, 101));
        pushButton_listen = new QPushButton(groupBox_server);
        pushButton_listen->setObjectName(QStringLiteral("pushButton_listen"));
        pushButton_listen->setGeometry(QRect(50, 40, 100, 40));
        groupBox = new QGroupBox(Dialog);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(240, 160, 190, 90));
        pushButton_conn = new QPushButton(groupBox);
        pushButton_conn->setObjectName(QStringLiteral("pushButton_conn"));
        pushButton_conn->setGeometry(QRect(40, 30, 120, 40));

        retranslateUi(Dialog);
        QObject::connect(okButton, SIGNAL(clicked()), Dialog, SLOT(accept()));
        QObject::connect(cancelButton, SIGNAL(clicked()), Dialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(Dialog);
    } // setupUi

    void retranslateUi(QDialog *Dialog)
    {
        Dialog->setWindowTitle(QApplication::translate("Dialog", "Dialog", Q_NULLPTR));
        okButton->setText(QApplication::translate("Dialog", "OK", Q_NULLPTR));
        cancelButton->setText(QApplication::translate("Dialog", "Cancel", Q_NULLPTR));
        label_ip->setText(QApplication::translate("Dialog", "ip:", Q_NULLPTR));
        label_port->setText(QApplication::translate("Dialog", "port:", Q_NULLPTR));
        groupBox_server->setTitle(QApplication::translate("Dialog", "server", Q_NULLPTR));
        pushButton_listen->setText(QApplication::translate("Dialog", "Listening", Q_NULLPTR));
        groupBox->setTitle(QApplication::translate("Dialog", "client", Q_NULLPTR));
        pushButton_conn->setText(QApplication::translate("Dialog", "Connect", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class Dialog: public Ui_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CHATDLGBB_H
