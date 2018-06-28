/********************************************************************************
** Form generated from reading UI file 'UserWidget.ui'
**
** Created by: Qt User Interface Compiler version 5.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_USERWIDGET_H
#define UI_USERWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_UserWidget
{
public:
    QTabWidget *tabWidget;
    QWidget *tab;
    QHBoxLayout *horizontalLayout_3;
    QHBoxLayout *horizontalLayout;
    QTableWidget *TW_BUDDY;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_2;
    QTextEdit *textbox_buddyname;
    QPushButton *btn_buddynamesave;
    QCheckBox *chk_BuddyAutoOperator;
    QCheckBox *chk_BuddyAutoTalkpower;
    QCheckBox *chk_BuddyAntiChannelBan;
    QSpacerItem *verticalSpacer_2;
    QSpacerItem *verticalSpacer;
    QWidget *tab_2;
    QHBoxLayout *horizontalLayout_6;
    QHBoxLayout *horizontalLayout_4;
    QTableWidget *TW_BLOCKED;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout_5;
    QTextEdit *textbox_blockname;
    QPushButton *btn_blocknamesave;
    QCheckBox *chk_BlockedAutoBan;
    QCheckBox *chk_BlockedAutoKick;
    QSpacerItem *verticalSpacer_3;
    QSpacerItem *verticalSpacer_4;
    QWidget *tab_3;
    QHBoxLayout *horizontalLayout_9;
    QHBoxLayout *horizontalLayout_7;
    QTableWidget *TW_NAMES;
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *horizontalLayout_8;
    QTextEdit *textbox_blockedname;
    QPushButton *btn_buddynamesave_2;
    QCheckBox *chk_BlockedNameAutoBan;
    QCheckBox *chk_BlockedNameAutoKick;
    QSpacerItem *verticalSpacer_5;
    QSpacerItem *verticalSpacer_6;

    void setupUi(QWidget *UserWidget)
    {
        if (UserWidget->objectName().isEmpty())
            UserWidget->setObjectName(QStringLiteral("UserWidget"));
        UserWidget->resize(574, 406);
        UserWidget->setMaximumSize(QSize(1000, 1000));
        tabWidget = new QTabWidget(UserWidget);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        tabWidget->setGeometry(QRect(0, 10, 538, 391));
        tab = new QWidget();
        tab->setObjectName(QStringLiteral("tab"));
        horizontalLayout_3 = new QHBoxLayout(tab);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        TW_BUDDY = new QTableWidget(tab);
        if (TW_BUDDY->columnCount() < 2)
            TW_BUDDY->setColumnCount(2);
        TW_BUDDY->setObjectName(QStringLiteral("TW_BUDDY"));
        TW_BUDDY->setRowCount(0);
        TW_BUDDY->setColumnCount(2);
        TW_BUDDY->horizontalHeader()->setVisible(false);
        TW_BUDDY->verticalHeader()->setVisible(false);

        horizontalLayout->addWidget(TW_BUDDY);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        textbox_buddyname = new QTextEdit(tab);
        textbox_buddyname->setObjectName(QStringLiteral("textbox_buddyname"));
        textbox_buddyname->setMaximumSize(QSize(200, 23));
        textbox_buddyname->setLayoutDirection(Qt::LeftToRight);
        textbox_buddyname->setAutoFillBackground(false);
        textbox_buddyname->setInputMethodHints(Qt::ImhNone);
        textbox_buddyname->setFrameShadow(QFrame::Sunken);
        textbox_buddyname->setLineWidth(1);
        textbox_buddyname->setLineWrapMode(QTextEdit::NoWrap);
        textbox_buddyname->setAcceptRichText(false);

        horizontalLayout_2->addWidget(textbox_buddyname);

        btn_buddynamesave = new QPushButton(tab);
        btn_buddynamesave->setObjectName(QStringLiteral("btn_buddynamesave"));
        btn_buddynamesave->setMaximumSize(QSize(40, 40));
        btn_buddynamesave->setCheckable(false);

        horizontalLayout_2->addWidget(btn_buddynamesave);


        verticalLayout->addLayout(horizontalLayout_2);

        chk_BuddyAutoOperator = new QCheckBox(tab);
        chk_BuddyAutoOperator->setObjectName(QStringLiteral("chk_BuddyAutoOperator"));

        verticalLayout->addWidget(chk_BuddyAutoOperator);

        chk_BuddyAutoTalkpower = new QCheckBox(tab);
        chk_BuddyAutoTalkpower->setObjectName(QStringLiteral("chk_BuddyAutoTalkpower"));

        verticalLayout->addWidget(chk_BuddyAutoTalkpower);

        chk_BuddyAntiChannelBan = new QCheckBox(tab);
        chk_BuddyAntiChannelBan->setObjectName(QStringLiteral("chk_BuddyAntiChannelBan"));

        verticalLayout->addWidget(chk_BuddyAntiChannelBan);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_2);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);


        horizontalLayout->addLayout(verticalLayout);


        horizontalLayout_3->addLayout(horizontalLayout);

        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QStringLiteral("tab_2"));
        horizontalLayout_6 = new QHBoxLayout(tab_2);
        horizontalLayout_6->setObjectName(QStringLiteral("horizontalLayout_6"));
        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        TW_BLOCKED = new QTableWidget(tab_2);
        if (TW_BLOCKED->columnCount() < 2)
            TW_BLOCKED->setColumnCount(2);
        TW_BLOCKED->setObjectName(QStringLiteral("TW_BLOCKED"));
        TW_BLOCKED->setColumnCount(2);
        TW_BLOCKED->horizontalHeader()->setVisible(false);
        TW_BLOCKED->verticalHeader()->setVisible(false);

        horizontalLayout_4->addWidget(TW_BLOCKED);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        textbox_blockname = new QTextEdit(tab_2);
        textbox_blockname->setObjectName(QStringLiteral("textbox_blockname"));
        textbox_blockname->setMaximumSize(QSize(200, 23));

        horizontalLayout_5->addWidget(textbox_blockname);

        btn_blocknamesave = new QPushButton(tab_2);
        btn_blocknamesave->setObjectName(QStringLiteral("btn_blocknamesave"));
        btn_blocknamesave->setMaximumSize(QSize(40, 40));

        horizontalLayout_5->addWidget(btn_blocknamesave);


        verticalLayout_2->addLayout(horizontalLayout_5);

        chk_BlockedAutoBan = new QCheckBox(tab_2);
        chk_BlockedAutoBan->setObjectName(QStringLiteral("chk_BlockedAutoBan"));

        verticalLayout_2->addWidget(chk_BlockedAutoBan);

        chk_BlockedAutoKick = new QCheckBox(tab_2);
        chk_BlockedAutoKick->setObjectName(QStringLiteral("chk_BlockedAutoKick"));

        verticalLayout_2->addWidget(chk_BlockedAutoKick);

        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer_3);

        verticalSpacer_4 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer_4);


        horizontalLayout_4->addLayout(verticalLayout_2);


        horizontalLayout_6->addLayout(horizontalLayout_4);

        tabWidget->addTab(tab_2, QString());
        tab_3 = new QWidget();
        tab_3->setObjectName(QStringLiteral("tab_3"));
        horizontalLayout_9 = new QHBoxLayout(tab_3);
        horizontalLayout_9->setObjectName(QStringLiteral("horizontalLayout_9"));
        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setObjectName(QStringLiteral("horizontalLayout_7"));
        TW_NAMES = new QTableWidget(tab_3);
        if (TW_NAMES->columnCount() < 1)
            TW_NAMES->setColumnCount(1);
        TW_NAMES->setObjectName(QStringLiteral("TW_NAMES"));
        TW_NAMES->setColumnCount(1);
        TW_NAMES->horizontalHeader()->setVisible(false);
        TW_NAMES->verticalHeader()->setVisible(false);

        horizontalLayout_7->addWidget(TW_NAMES);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setObjectName(QStringLiteral("horizontalLayout_8"));
        textbox_blockedname = new QTextEdit(tab_3);
        textbox_blockedname->setObjectName(QStringLiteral("textbox_blockedname"));
        textbox_blockedname->setMaximumSize(QSize(200, 23));
        textbox_blockedname->setLayoutDirection(Qt::LeftToRight);
        textbox_blockedname->setAutoFillBackground(false);
        textbox_blockedname->setInputMethodHints(Qt::ImhNone);
        textbox_blockedname->setFrameShadow(QFrame::Sunken);
        textbox_blockedname->setLineWidth(1);
        textbox_blockedname->setLineWrapMode(QTextEdit::NoWrap);
        textbox_blockedname->setAcceptRichText(false);

        horizontalLayout_8->addWidget(textbox_blockedname);

        btn_buddynamesave_2 = new QPushButton(tab_3);
        btn_buddynamesave_2->setObjectName(QStringLiteral("btn_buddynamesave_2"));
        btn_buddynamesave_2->setMaximumSize(QSize(40, 40));
        btn_buddynamesave_2->setCheckable(false);

        horizontalLayout_8->addWidget(btn_buddynamesave_2);


        verticalLayout_3->addLayout(horizontalLayout_8);

        chk_BlockedNameAutoBan = new QCheckBox(tab_3);
        chk_BlockedNameAutoBan->setObjectName(QStringLiteral("chk_BlockedNameAutoBan"));

        verticalLayout_3->addWidget(chk_BlockedNameAutoBan);

        chk_BlockedNameAutoKick = new QCheckBox(tab_3);
        chk_BlockedNameAutoKick->setObjectName(QStringLiteral("chk_BlockedNameAutoKick"));

        verticalLayout_3->addWidget(chk_BlockedNameAutoKick);

        verticalSpacer_5 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_3->addItem(verticalSpacer_5);

        verticalSpacer_6 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_3->addItem(verticalSpacer_6);


        horizontalLayout_7->addLayout(verticalLayout_3);


        horizontalLayout_9->addLayout(horizontalLayout_7);

        tabWidget->addTab(tab_3, QString());

        retranslateUi(UserWidget);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(UserWidget);
    } // setupUi

    void retranslateUi(QWidget *UserWidget)
    {
        UserWidget->setWindowTitle(QApplication::translate("UserWidget", "Form", 0));
        btn_buddynamesave->setText(QApplication::translate("UserWidget", "save", 0));
        chk_BuddyAutoOperator->setText(QApplication::translate("UserWidget", "AutoOperator", 0));
        chk_BuddyAutoTalkpower->setText(QApplication::translate("UserWidget", "AutoTalkpower", 0));
        chk_BuddyAntiChannelBan->setText(QApplication::translate("UserWidget", "AntiChannelBan", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("UserWidget", "Buddys", 0));
        btn_blocknamesave->setText(QApplication::translate("UserWidget", "save", 0));
        chk_BlockedAutoBan->setText(QApplication::translate("UserWidget", "AutoBan", 0));
        chk_BlockedAutoKick->setText(QApplication::translate("UserWidget", "AutoKick", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("UserWidget", "Blocked", 0));
        btn_buddynamesave_2->setText(QApplication::translate("UserWidget", "save", 0));
        chk_BlockedNameAutoBan->setText(QApplication::translate("UserWidget", "AutoBan", 0));
        chk_BlockedNameAutoKick->setText(QApplication::translate("UserWidget", "AutoKick", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab_3), QApplication::translate("UserWidget", "Names", 0));
    } // retranslateUi

};

namespace Ui {
    class UserWidget: public Ui_UserWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_USERWIDGET_H
