/********************************************************************************
** Form generated from reading UI file 'fmplayer.ui'
**
** Created by: Qt User Interface Compiler version 5.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FMPLAYER_H
#define UI_FMPLAYER_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_FmPlayer
{
public:
    QWidget *centralWidget;
    QLabel *label_Pl;

    void setupUi(QMainWindow *FmPlayer)
    {
        if (FmPlayer->objectName().isEmpty())
            FmPlayer->setObjectName(QStringLiteral("FmPlayer"));
        FmPlayer->resize(449, 383);
        centralWidget = new QWidget(FmPlayer);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        label_Pl = new QLabel(centralWidget);
        label_Pl->setObjectName(QStringLiteral("label_Pl"));
        label_Pl->setGeometry(QRect(70, 40, 1141, 531));
        FmPlayer->setCentralWidget(centralWidget);

        retranslateUi(FmPlayer);

        QMetaObject::connectSlotsByName(FmPlayer);
    } // setupUi

    void retranslateUi(QMainWindow *FmPlayer)
    {
        FmPlayer->setWindowTitle(QApplication::translate("FmPlayer", "FmPlayer", nullptr));
        label_Pl->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class FmPlayer: public Ui_FmPlayer {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FMPLAYER_H
