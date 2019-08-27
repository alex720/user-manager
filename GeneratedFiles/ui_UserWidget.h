/********************************************************************************
** Form generated from reading UI file 'UserWidget.ui'
**
** Created by: Qt User Interface Compiler version 5.12.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_USERWIDGET_H
#define UI_USERWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableWidget>
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
    QLineEdit *textbox_buddyname;
    QPushButton *btn_buddynamesave;
    QCheckBox *chk_BuddyAutoOperator;
    QCheckBox *chk_BuddyAutoTalkpower;
    QCheckBox *chk_BuddyAntiChannelBan;
    QPushButton *btn_buddyRemove;
    QSpacerItem *verticalSpacer_2;
    QSpacerItem *verticalSpacer;
    QWidget *tab_2;
    QHBoxLayout *horizontalLayout_6;
    QHBoxLayout *horizontalLayout_4;
    QTableWidget *TW_BLOCKED;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout_5;
    QLineEdit *textbox_blockname;
    QPushButton *btn_blocknamesave;
    QCheckBox *chk_BlockedAutoBan;
    QCheckBox *chk_BlockedAutoKick;
    QPushButton *btn_blockedRemove;
    QSpacerItem *verticalSpacer_3;
    QSpacerItem *verticalSpacer_4;
    QWidget *tab_3;
    QHBoxLayout *horizontalLayout_9;
    QHBoxLayout *horizontalLayout_7;
    QTableWidget *TW_NAMES;
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *horizontalLayout_8;
    QLineEdit *textbox_blockedname;
    QPushButton *btn_blocknameNamesave;
    QCheckBox *chk_BlockedNameAutoBan;
    QCheckBox *chk_BlockedNameAutoKick;
    QPushButton *btn_NameRemove;
    QSpacerItem *verticalSpacer_5;
    QSpacerItem *verticalSpacer_6;
    QWidget *tab_4;
    QWidget *layoutWidget;
    QHBoxLayout *horizontalLayout_19;
    QTableWidget *TW_COUNTRY;
    QVBoxLayout *verticalLayout_7;
    QCheckBox *chk_BlockedCountryAutoBan;
    QCheckBox *chk_BlockedCountryAutoKick;
    QPushButton *btn_countryRemove;
    QSpacerItem *verticalSpacer_13;

    void setupUi(QWidget *UserWidget)
    {
        if (UserWidget->objectName().isEmpty())
            UserWidget->setObjectName(QString::fromUtf8("UserWidget"));
        UserWidget->resize(512, 421);
        UserWidget->setMaximumSize(QSize(1000, 1000));
        tabWidget = new QTabWidget(UserWidget);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabWidget->setGeometry(QRect(10, 10, 491, 391));
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        horizontalLayout_3 = new QHBoxLayout(tab);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        TW_BUDDY = new QTableWidget(tab);
        if (TW_BUDDY->columnCount() < 2)
            TW_BUDDY->setColumnCount(2);
        TW_BUDDY->setObjectName(QString::fromUtf8("TW_BUDDY"));
        TW_BUDDY->setSortingEnabled(true);
        TW_BUDDY->setRowCount(0);
        TW_BUDDY->setColumnCount(2);
        TW_BUDDY->horizontalHeader()->setVisible(false);
        TW_BUDDY->verticalHeader()->setVisible(false);

        horizontalLayout->addWidget(TW_BUDDY);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        textbox_buddyname = new QLineEdit(tab);
        textbox_buddyname->setObjectName(QString::fromUtf8("textbox_buddyname"));

        horizontalLayout_2->addWidget(textbox_buddyname);

        btn_buddynamesave = new QPushButton(tab);
        btn_buddynamesave->setObjectName(QString::fromUtf8("btn_buddynamesave"));
        btn_buddynamesave->setMaximumSize(QSize(40, 40));
        btn_buddynamesave->setCheckable(false);

        horizontalLayout_2->addWidget(btn_buddynamesave);


        verticalLayout->addLayout(horizontalLayout_2);

        chk_BuddyAutoOperator = new QCheckBox(tab);
        chk_BuddyAutoOperator->setObjectName(QString::fromUtf8("chk_BuddyAutoOperator"));

        verticalLayout->addWidget(chk_BuddyAutoOperator);

        chk_BuddyAutoTalkpower = new QCheckBox(tab);
        chk_BuddyAutoTalkpower->setObjectName(QString::fromUtf8("chk_BuddyAutoTalkpower"));

        verticalLayout->addWidget(chk_BuddyAutoTalkpower);

        chk_BuddyAntiChannelBan = new QCheckBox(tab);
        chk_BuddyAntiChannelBan->setObjectName(QString::fromUtf8("chk_BuddyAntiChannelBan"));

        verticalLayout->addWidget(chk_BuddyAntiChannelBan);

        btn_buddyRemove = new QPushButton(tab);
        btn_buddyRemove->setObjectName(QString::fromUtf8("btn_buddyRemove"));
        btn_buddyRemove->setMaximumSize(QSize(80, 16777215));
        btn_buddyRemove->setCheckable(false);

        verticalLayout->addWidget(btn_buddyRemove);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_2);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);


        horizontalLayout->addLayout(verticalLayout);


        horizontalLayout_3->addLayout(horizontalLayout);

        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        horizontalLayout_6 = new QHBoxLayout(tab_2);
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        TW_BLOCKED = new QTableWidget(tab_2);
        if (TW_BLOCKED->columnCount() < 2)
            TW_BLOCKED->setColumnCount(2);
        TW_BLOCKED->setObjectName(QString::fromUtf8("TW_BLOCKED"));
        TW_BLOCKED->setColumnCount(2);
        TW_BLOCKED->horizontalHeader()->setVisible(false);
        TW_BLOCKED->verticalHeader()->setVisible(false);

        horizontalLayout_4->addWidget(TW_BLOCKED);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        textbox_blockname = new QLineEdit(tab_2);
        textbox_blockname->setObjectName(QString::fromUtf8("textbox_blockname"));

        horizontalLayout_5->addWidget(textbox_blockname);

        btn_blocknamesave = new QPushButton(tab_2);
        btn_blocknamesave->setObjectName(QString::fromUtf8("btn_blocknamesave"));
        btn_blocknamesave->setMaximumSize(QSize(40, 40));

        horizontalLayout_5->addWidget(btn_blocknamesave);


        verticalLayout_2->addLayout(horizontalLayout_5);

        chk_BlockedAutoBan = new QCheckBox(tab_2);
        chk_BlockedAutoBan->setObjectName(QString::fromUtf8("chk_BlockedAutoBan"));

        verticalLayout_2->addWidget(chk_BlockedAutoBan);

        chk_BlockedAutoKick = new QCheckBox(tab_2);
        chk_BlockedAutoKick->setObjectName(QString::fromUtf8("chk_BlockedAutoKick"));

        verticalLayout_2->addWidget(chk_BlockedAutoKick);

        btn_blockedRemove = new QPushButton(tab_2);
        btn_blockedRemove->setObjectName(QString::fromUtf8("btn_blockedRemove"));
        btn_blockedRemove->setMaximumSize(QSize(80, 16777215));
        btn_blockedRemove->setCheckable(false);

        verticalLayout_2->addWidget(btn_blockedRemove);

        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer_3);

        verticalSpacer_4 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer_4);


        horizontalLayout_4->addLayout(verticalLayout_2);


        horizontalLayout_6->addLayout(horizontalLayout_4);

        tabWidget->addTab(tab_2, QString());
        tab_3 = new QWidget();
        tab_3->setObjectName(QString::fromUtf8("tab_3"));
        horizontalLayout_9 = new QHBoxLayout(tab_3);
        horizontalLayout_9->setObjectName(QString::fromUtf8("horizontalLayout_9"));
        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        TW_NAMES = new QTableWidget(tab_3);
        if (TW_NAMES->columnCount() < 1)
            TW_NAMES->setColumnCount(1);
        TW_NAMES->setObjectName(QString::fromUtf8("TW_NAMES"));
        TW_NAMES->setColumnCount(1);
        TW_NAMES->horizontalHeader()->setVisible(false);
        TW_NAMES->verticalHeader()->setVisible(false);

        horizontalLayout_7->addWidget(TW_NAMES);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setObjectName(QString::fromUtf8("horizontalLayout_8"));
        textbox_blockedname = new QLineEdit(tab_3);
        textbox_blockedname->setObjectName(QString::fromUtf8("textbox_blockedname"));

        horizontalLayout_8->addWidget(textbox_blockedname);

        btn_blocknameNamesave = new QPushButton(tab_3);
        btn_blocknameNamesave->setObjectName(QString::fromUtf8("btn_blocknameNamesave"));
        btn_blocknameNamesave->setMaximumSize(QSize(40, 40));
        btn_blocknameNamesave->setCheckable(false);

        horizontalLayout_8->addWidget(btn_blocknameNamesave);


        verticalLayout_3->addLayout(horizontalLayout_8);

        chk_BlockedNameAutoBan = new QCheckBox(tab_3);
        chk_BlockedNameAutoBan->setObjectName(QString::fromUtf8("chk_BlockedNameAutoBan"));

        verticalLayout_3->addWidget(chk_BlockedNameAutoBan);

        chk_BlockedNameAutoKick = new QCheckBox(tab_3);
        chk_BlockedNameAutoKick->setObjectName(QString::fromUtf8("chk_BlockedNameAutoKick"));

        verticalLayout_3->addWidget(chk_BlockedNameAutoKick);

        btn_NameRemove = new QPushButton(tab_3);
        btn_NameRemove->setObjectName(QString::fromUtf8("btn_NameRemove"));
        btn_NameRemove->setMaximumSize(QSize(80, 16777215));
        btn_NameRemove->setCheckable(false);

        verticalLayout_3->addWidget(btn_NameRemove);

        verticalSpacer_5 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_3->addItem(verticalSpacer_5);

        verticalSpacer_6 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_3->addItem(verticalSpacer_6);


        horizontalLayout_7->addLayout(verticalLayout_3);


        horizontalLayout_9->addLayout(horizontalLayout_7);

        tabWidget->addTab(tab_3, QString());
        tab_4 = new QWidget();
        tab_4->setObjectName(QString::fromUtf8("tab_4"));
        layoutWidget = new QWidget(tab_4);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layoutWidget->setGeometry(QRect(10, 0, 451, 361));
        horizontalLayout_19 = new QHBoxLayout(layoutWidget);
        horizontalLayout_19->setObjectName(QString::fromUtf8("horizontalLayout_19"));
        horizontalLayout_19->setContentsMargins(0, 0, 0, 0);
        TW_COUNTRY = new QTableWidget(layoutWidget);
        if (TW_COUNTRY->columnCount() < 1)
            TW_COUNTRY->setColumnCount(1);
        TW_COUNTRY->setObjectName(QString::fromUtf8("TW_COUNTRY"));
        TW_COUNTRY->setColumnCount(1);
        TW_COUNTRY->horizontalHeader()->setVisible(false);

        horizontalLayout_19->addWidget(TW_COUNTRY);

        verticalLayout_7 = new QVBoxLayout();
        verticalLayout_7->setObjectName(QString::fromUtf8("verticalLayout_7"));
        chk_BlockedCountryAutoBan = new QCheckBox(layoutWidget);
        chk_BlockedCountryAutoBan->setObjectName(QString::fromUtf8("chk_BlockedCountryAutoBan"));

        verticalLayout_7->addWidget(chk_BlockedCountryAutoBan);

        chk_BlockedCountryAutoKick = new QCheckBox(layoutWidget);
        chk_BlockedCountryAutoKick->setObjectName(QString::fromUtf8("chk_BlockedCountryAutoKick"));

        verticalLayout_7->addWidget(chk_BlockedCountryAutoKick);

        btn_countryRemove = new QPushButton(layoutWidget);
        btn_countryRemove->setObjectName(QString::fromUtf8("btn_countryRemove"));
        btn_countryRemove->setMaximumSize(QSize(80, 16777215));
        btn_countryRemove->setCheckable(false);

        verticalLayout_7->addWidget(btn_countryRemove);

        verticalSpacer_13 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_7->addItem(verticalSpacer_13);


        horizontalLayout_19->addLayout(verticalLayout_7);

        tabWidget->addTab(tab_4, QString());

        retranslateUi(UserWidget);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(UserWidget);
    } // setupUi

    void retranslateUi(QWidget *UserWidget)
    {
        UserWidget->setWindowTitle(QApplication::translate("UserWidget", "Conactlist", nullptr));
        btn_buddynamesave->setText(QApplication::translate("UserWidget", "save", nullptr));
        chk_BuddyAutoOperator->setText(QApplication::translate("UserWidget", "AutoOperator", nullptr));
        chk_BuddyAutoTalkpower->setText(QApplication::translate("UserWidget", "AutoTalkpower", nullptr));
        chk_BuddyAntiChannelBan->setText(QApplication::translate("UserWidget", "AntiChannelBan", nullptr));
        btn_buddyRemove->setText(QApplication::translate("UserWidget", "Remove", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("UserWidget", "Buddys", nullptr));
        btn_blocknamesave->setText(QApplication::translate("UserWidget", "save", nullptr));
        chk_BlockedAutoBan->setText(QApplication::translate("UserWidget", "AutoBan", nullptr));
        chk_BlockedAutoKick->setText(QApplication::translate("UserWidget", "AutoKick", nullptr));
        btn_blockedRemove->setText(QApplication::translate("UserWidget", "Remove", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("UserWidget", "Blocked", nullptr));
        btn_blocknameNamesave->setText(QApplication::translate("UserWidget", "save", nullptr));
        chk_BlockedNameAutoBan->setText(QApplication::translate("UserWidget", "AutoBan", nullptr));
        chk_BlockedNameAutoKick->setText(QApplication::translate("UserWidget", "AutoKick", nullptr));
        btn_NameRemove->setText(QApplication::translate("UserWidget", "Remove", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_3), QApplication::translate("UserWidget", "Names", nullptr));
        chk_BlockedCountryAutoBan->setText(QApplication::translate("UserWidget", "AutoBan", nullptr));
        chk_BlockedCountryAutoKick->setText(QApplication::translate("UserWidget", "AutoKick", nullptr));
        btn_countryRemove->setText(QApplication::translate("UserWidget", "Remove", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_4), QApplication::translate("UserWidget", "Country", nullptr));
    } // retranslateUi

};

namespace Ui {
    class UserWidget: public Ui_UserWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_USERWIDGET_H
