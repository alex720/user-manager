#pragma once
#include "ui_UserWidget.h"
#include <qwidget.h>
#include <QTableWidget>
#include "sqlwrapper.h"
#include <QtCore/QObject>
#include <qdesktopwidget.h>
#include "help_functions.h"
#include <string>


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
	
	void buddyItemClicked(int row, int column);
	void blockedItemClicked(int row, int column);
	void blockedNameItemClicked(int row, int column);

private:
	Ui::UserWidget *ui;

	QTableWidgetItem *currentItem;

	sqlw* UserManager;
	QTableWidget* buddytablewidget;
	QTableWidget* blocktablewidget;
	QTableWidget* blocknametablewidget;

	void fillBuddyTable(std::string UID);
	void fillBlockedTable(std::string UID);
	void fillBlockedNameTable();

	bool initphase = false;

};
