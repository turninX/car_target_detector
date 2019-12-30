/********************************************************************************
** Form generated from reading UI file 'qt_static.ui'
**
** Created by: Qt User Interface Compiler version 5.7.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QT_STATIC_H
#define UI_QT_STATIC_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_qt_staticClass
{
public:
    QWidget *centralWidget;
    QLabel *label;
    QWidget *widget;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_4;
    QLineEdit *lineEdit;
    QPushButton *read_file;
    QWidget *widget1;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label_2;
    QLabel *label_3;
    QPushButton *start;
    QPushButton *exit;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *qt_staticClass)
    {
        if (qt_staticClass->objectName().isEmpty())
            qt_staticClass->setObjectName(QStringLiteral("qt_staticClass"));
        qt_staticClass->resize(748, 572);
        centralWidget = new QWidget(qt_staticClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        label = new QLabel(centralWidget);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(10, 0, 641, 481));
        widget = new QWidget(centralWidget);
        widget->setObjectName(QStringLiteral("widget"));
        widget->setGeometry(QRect(20, 480, 631, 51));
        horizontalLayout_2 = new QHBoxLayout(widget);
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        label_4 = new QLabel(widget);
        label_4->setObjectName(QStringLiteral("label_4"));

        horizontalLayout_2->addWidget(label_4);

        lineEdit = new QLineEdit(widget);
        lineEdit->setObjectName(QStringLiteral("lineEdit"));

        horizontalLayout_2->addWidget(lineEdit);

        read_file = new QPushButton(widget);
        read_file->setObjectName(QStringLiteral("read_file"));

        horizontalLayout_2->addWidget(read_file);

        widget1 = new QWidget(centralWidget);
        widget1->setObjectName(QStringLiteral("widget1"));
        widget1->setGeometry(QRect(660, 120, 81, 281));
        verticalLayout = new QVBoxLayout(widget1);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        label_2 = new QLabel(widget1);
        label_2->setObjectName(QStringLiteral("label_2"));

        horizontalLayout->addWidget(label_2);

        label_3 = new QLabel(widget1);
        label_3->setObjectName(QStringLiteral("label_3"));

        horizontalLayout->addWidget(label_3);


        verticalLayout->addLayout(horizontalLayout);

        start = new QPushButton(widget1);
        start->setObjectName(QStringLiteral("start"));

        verticalLayout->addWidget(start);

        exit = new QPushButton(widget1);
        exit->setObjectName(QStringLiteral("exit"));

        verticalLayout->addWidget(exit);

        qt_staticClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(qt_staticClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 748, 23));
        qt_staticClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(qt_staticClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        qt_staticClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(qt_staticClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        qt_staticClass->setStatusBar(statusBar);

        retranslateUi(qt_staticClass);
        QObject::connect(start, SIGNAL(clicked()), qt_staticClass, SLOT(start_clicked()));
        QObject::connect(exit, SIGNAL(clicked()), qt_staticClass, SLOT(exit_clicked()));
        QObject::connect(read_file, SIGNAL(clicked()), qt_staticClass, SLOT(read_file_clicked()));

        QMetaObject::connectSlotsByName(qt_staticClass);
    } // setupUi

    void retranslateUi(QMainWindow *qt_staticClass)
    {
        qt_staticClass->setWindowTitle(QApplication::translate("qt_staticClass", "qt_static", Q_NULLPTR));
        label->setText(QString());
        label_4->setText(QApplication::translate("qt_staticClass", "\350\247\206\351\242\221\350\267\257\345\276\204\357\274\232", Q_NULLPTR));
        read_file->setText(QApplication::translate("qt_staticClass", "Read File", Q_NULLPTR));
        label_2->setText(QApplication::translate("qt_staticClass", "FPS:", Q_NULLPTR));
        label_3->setText(QString());
        start->setText(QApplication::translate("qt_staticClass", "Start", Q_NULLPTR));
        exit->setText(QApplication::translate("qt_staticClass", "Exit", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class qt_staticClass: public Ui_qt_staticClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QT_STATIC_H
