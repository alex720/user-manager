#pragma once

#include "ui_UserWidget.h"
#include <QTWidgets/qwidget.h>
#include <QTWidgets/QTableWidget>
#include "sqlwrapper.h"
#include <QtCore/QObject>
#include <QTWidgets/qwidget.h>
#include "help_functions.h"
#include <string>
#include <QTWidgets/qlineedit.h>


namespace Ui {
	class UserWidget;
}

class UserWidget : public QWidget {
	Q_OBJECT

public:
	 UserWidget(sqlw* UserManager);
	 UserWidget(sqlw* UserManager,std::string UID,bool buddy); // buddy = true buddy table öffnen | buddy == false blocktable öffnen
	~UserWidget();

	private slots:
	void on_chk_BuddyAutoOperator_stateChanged(int state);
	void on_chk_BuddyAutoTalkpower_stateChanged(int state);
	void on_chk_BuddyAntiChannelBan_stateChanged(int state);
	void on_chk_BlockedAutoBan_stateChanged(int state);
	void on_chk_BlockedAutoKick_stateChanged(int state);
	void on_chk_BlockedNameAutoBan_stateChanged(int state);
	void on_chk_BlockedNameAutoKick_stateChanged(int state);
	void on_btn_buddynamesave_clicked();
	void on_btn_blocknamesave_clicked();
	void on_btn_blocknameNamesave_clicked();

	void on_chk_BlockedCountryAutoBan_stateChanged(int state);
	void on_chk_BlockedCountryAutoKick_stateChanged(int state);

	void on_btn_buddyRemove_clicked();
	void on_btn_blockedRemove_clicked();
	void on_btn_NameRemove_clicked();
	void on_btn_countryRemove_clicked();


	void buddyItemClicked(int row, int column);
	void blockedItemClicked(int row, int column);
	void blockedNameItemClicked(int row, int column);
	void blockedCountryItemClicked(int row, int column);

private:
	Ui::UserWidget *ui;

	QTableWidgetItem *currentItemBuddy = nullptr;
	QTableWidgetItem *currentItemBlocked = nullptr;
	QTableWidgetItem *currentItemName = nullptr;
	QTableWidgetItem *currentItemCountry = nullptr;

	sqlw* UserManager;
	QTableWidget* buddytablewidget;
	QTableWidget* blocktablewidget;
	QTableWidget* blocknametablewidget;
	QTableWidget* blockCountrytablewidget;

	void fillBuddyTable(std::string UID);
	void fillBlockedTable(std::string UID);
	void fillBlockedNameTable();
	void fillBlockedCountryTable();

	bool initphase = false;

};
