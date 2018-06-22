#pragma once
#include "ui_User-Manager.h"
#include "ConfigData.h"
#include "sqlwrapper.h"
#include "ts3_functions.h"
#include <QDialog>
#include <iostream>
#include <qmessagebox.h>
#include <qdesktopwidget.h>
#include <QtCore/QObject>
#include "help_functions.h"

#define _CRT_SECURE_NO_WARNINGS



namespace Ui {
	class ConfigDialog;
}


class ConfigDialog : public QDialog
{
	Q_OBJECT

public:
	explicit ConfigDialog(sqlw* UserManager, const struct TS3Functions funcs,ConfigData *datas, QWidget *parent = Q_NULLPTR);

	~ConfigDialog();
	
	private slots:
	void on_btnSave_clicked();
	void on_btnImport_Blocked_clicked();
	void on_btnImport_Buddys_clicked();
	void on_chkAutoKick_stateChanged(int state);
	void on_chkAutoBan_stateChanged(int state);
	void on_chkAutoMute_stateChanged(int state);
	void on_chkNotifikations_stateChanged(int state);
	void on_chkAntiChannelBan_stateChanged(int state);
	void on_chkAutoOperator_stateChanged(int state);
	void on_chkAutoTalkpower_stateChanged(int state);
	void on_chkLog_stateChanged(int state);
	void on_chkcurrent_server_toggled(bool state);
	void on_chkUseTSList_stateChanged(int state);
	void on_chkWorking_stateChanged(int state);
	

private:
	Ui::ConfigDialog *ui;
	bool settingsLoaded;
	void reloadOptions();
	sqlw *UserManager;
	struct TS3Functions ts3Functions;
	ConfigData *datas;

	void buddys_import();
	void blocked_import();
	bool useTSListcheck = false;

	bool initphase = false;;
};