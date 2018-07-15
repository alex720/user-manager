#include "configDialog.h"
#define _CRT_SECURE_NO_WARNINGS 

ConfigDialog::ConfigDialog(sqlw* UserManagerPoint, const struct TS3Functions funcs,ConfigData *datas,QWidget *parent): UserManager(UserManagerPoint),QDialog(parent),datas(datas), ui(new Ui::ConfigDialog)
{

	initphase = true;

	log("configDialog setup start");
	ts3Functions = funcs;

	ui->setupUi(this);
	log("setup ui");
	this->reloadOptions();
	log("configDialog reloadinifile ");


	ts3Functions.printMessageToCurrentTab("Konfiguration geoeffnet");

	//get current current schid
	uint64 schid = ts3Functions.getCurrentServerConnectionHandlerID();
	char* serverName = "";
	ts3Functions.getServerVariableAsString(schid, VIRTUALSERVER_NAME, &serverName);
	char* uid = "";
	ts3Functions.getServerVariableAsString(schid,VIRTUALSERVER_UNIQUE_IDENTIFIER, &uid);
	this->findChild<QLabel*>("L_NAME")->setText(QString(serverName));
	this->findChild<QLabel*>("L_UID")->setText(QString(uid));

	if (UserManager->isForServerEnabled(uid)) {
		this->findChild<QCheckBox*>("chkcurrent_server")->setChecked(true);
		this->findChild<QSpinBox*>("speChannelGroupAdmin")->setEnabled(true);
		this->findChild<QSpinBox*>("speChannelGroupBan")->setEnabled(true);
		this->findChild<QSpinBox*>("speChannelGroupOperator")->setEnabled(true);


		this->findChild<QSpinBox*>("speChannelGroupAdmin")->setValue(UserManager->readChannelGroupID(uid, 1));
		this->findChild<QSpinBox*>("speChannelGroupBan")->setValue(UserManager->readChannelGroupID(uid, 3));
		this->findChild<QSpinBox*>("speChannelGroupOperator")->setValue(UserManager->readChannelGroupID(uid, 2));
	
	}
	
	initphase = false;
	log("ConfigDialog setup complete");
}
ConfigDialog::~ConfigDialog()
{
	
	delete ui;
}


void ConfigDialog::reloadOptions() {

	if (datas->getAutoTP()) {
		this->findChild<QCheckBox*>("chkAutoTalkpower")->setChecked(true);
	}
	else {
		this->findChild<QCheckBox*>("chkAutoTalkpower")->setChecked(false);
	}

	if (datas->getAutoBan()) {
		this->findChild<QCheckBox*>("chkAutoBan")->setChecked(true);
	}
	else {
		this->findChild<QCheckBox*>("chkAutoBan")->setChecked(false);
	}

	if (datas->getAutoKick()) {
		this->findChild<QCheckBox*>("chkAutoKick")->setChecked(true);
	}
	else {
		this->findChild<QCheckBox*>("chkAutoKick")->setChecked(false);
	}

	if (datas->getAutoOperator()) {
		this->findChild<QCheckBox*>("chkAutoOperator")->setChecked(true);
	}
	else {
		this->findChild<QCheckBox*>("chkAutoOperator")->setChecked(false);
	}

	if (datas->getAntiChannelBan()) {
		this->findChild<QCheckBox*>("chkAntiChannelBan")->setChecked(true);
	}
	else {
		this->findChild<QCheckBox*>("chkAntiChannelBan")->setChecked(false);
	}

	if (datas->getuseTSList()) {
		useTSListcheck = true;
		this->findChild<QCheckBox*>("chkUseTSList")->setChecked(true);
	}
	else {
		this->findChild<QCheckBox*>("chkUseTSList")->setChecked(false);
	}

	if (datas->getwantannoucments()) {
		this->findChild<QCheckBox*>("chkNotifikations")->setChecked(true);
	}
	else {
		this->findChild<QCheckBox*>("chkNotifikations")->setChecked(false);
	}

	if (datas->getlogging()) {
		this->findChild<QCheckBox*>("chkLog")->setChecked(true);
	}
	else {
		this->findChild<QCheckBox*>("chkLog")->setChecked(false);
	}

	if (datas->getWorking()) {
		this->findChild<QCheckBox*>("chkWorking")->setChecked(true);
	}
	else {
		this->findChild<QCheckBox*>("chkWorking")->setChecked(false);
	}

	if (datas->getchannelAnnouncments()) {
		this->findChild<QCheckBox*>("chkChannelNotifikations")->setChecked(true);
	}
	else {
		this->findChild<QCheckBox*>("chkChannelNotifikations")->setChecked(false);
	}

	if (datas->getautomute()){
		this->findChild<QCheckBox*>("chkAutoMute")->setChecked(true);
	}
	else {
		this->findChild<QCheckBox*>("chkAutoMute")->setChecked(false);
	}
	this->findChild<QLineEdit*>("kickmessage")->setText(datas->getkickMessage().c_str());
}

void ConfigDialog::on_chkAntiChannelBan_stateChanged(int state) 
{
	if (initphase)return;
	log("AntiChannelBan toggled");
	datas->setAntiChannelBan(state);
}

void ConfigDialog::on_chkAutoKick_stateChanged(int state)
{
	if (initphase)return;
	log("AutoKick toggled");
	datas->setAutoKick(state);
}
void ConfigDialog::on_chkAutoBan_stateChanged(int state)
{
	if (initphase)return;
	log("AutoBan toggled");
	datas->setAutoBan(state);
}
void ConfigDialog::on_chkAutoMute_stateChanged(int state) 
{
	if (initphase)return;
	log("AutoMute toggled");
	datas->setautomute(state);
}

void ConfigDialog::on_chkAutoOperator_stateChanged(int state) 
{
	if (initphase)return;
	log("AutoOperator toggled");
	static int eg = 0;
	eg += 1;
	if (eg == 10) {
		eg = 0;
		log("Blu macht alles kaputt");
	}
	datas->setAutoOperator(state);
}

void ConfigDialog::on_chkAutoTalkpower_stateChanged(int state) 
{
	if (initphase)return;
	log("AutoTalkpower toggled");
	datas->setAutoTP(state);
}

void ConfigDialog::on_chkLog_stateChanged(int state)
{
	if (initphase)return;
	log("Log toggled");
	datas->setlogging(state);
}

void ConfigDialog::on_chkNotifikations_stateChanged(int state)
{
	if (initphase)return;
	log("Notifikations toggled");
	datas->setwantannoucments(state);
}


void ConfigDialog::on_btnImport_Blocked_clicked() {
	blocked_import();
}

void ConfigDialog::buddys_import() {
	ts3Functions.printMessageToCurrentTab("Beginne mit der Transferierung der Buddys");
	Sleep(500);
	QSqlDatabase tsdb = QSqlDatabase::addDatabase("QSQLITE");
	tsdb.setDatabaseName(QString(UserManager->PathTsDB.c_str()));
	tsdb.open();

	QSqlQuery queryout = tsdb.exec(QString("SELECT * FROM Contacts WHERE value LIKE '%Friend=0%'"));

	while (queryout.next()) {


		QString buffer = queryout.value("value").toString(); //buffer = string of the "value" column

		std::string sBuffer = buffer.toStdString(); //sBUffer std::string von buffer
		std::string searchstring("IDS=");
		size_t pos = sBuffer.find(searchstring);

		size_t pos2 = sBuffer.find("Friend=");
		std::string Name = sBuffer.substr(9, pos2 - 10);

		std::string cur = sBuffer.substr(pos + 4, 28);
		if (!UserManager->isBlocked(cur.c_str()).dummy_Return && !UserManager->isBuddy(cur.c_str()).dummy_Return) {
			
			BuddyUser cache = {};
			cache.AntiChannelBan = datas->getAntiChannelBan();
			cache.AutoOperator = datas->getAutoOperator();
			cache.AutoTalkpower = datas->getAutoTP();
			cache.SavedName = Name.c_str();
			cache.UID = cur.c_str();
			
			UserManager->addBuddyList(cache);
		}
	}

	tsdb.close();
	ts3Functions.printMessageToCurrentTab("Transferierung der Buddys beendet!");
}

void ConfigDialog::blocked_import() {
	ts3Functions.printMessageToCurrentTab("Beginne mit der Transferierung der blockierten User");
	Sleep(500);
	QSqlDatabase tsdb = QSqlDatabase::addDatabase("QSQLITE");
	tsdb.setDatabaseName(QString(UserManager->PathTsDB.c_str()));
	tsdb.open();

	QSqlQuery queryout = tsdb.exec(QString("SELECT * FROM Contacts WHERE value LIKE '%Friend=1%'"));

	while (queryout.next()) {

		QString buffer = queryout.value("value").toString(); //buffer = string of the "value" column

		std::string sBuffer = buffer.toStdString(); //sBUffer std::string von buffer
		std::string searchstring("IDS=");
		size_t pos = sBuffer.find(searchstring);

		size_t pos2 = sBuffer.find("Friend=");
		std::string Name = sBuffer.substr(9, pos2 - 10);
		std::string cur = sBuffer.substr(pos + 4, 28);
		if (!UserManager->isBlocked(cur.c_str()).dummy_Return && !UserManager->isBuddy(cur.c_str()).dummy_Return) {
			ts3Functions.printMessageToCurrentTab("test");
			BlockedUser cache = {};
			cache.AutoBan = datas->getAutoBan();
			cache.AutoKick = datas->getAutoKick();
			cache.UID = cur.c_str();
			cache.SavedName = Name.c_str();
			UserManager->addBlockedList(cache);   
		}
	}
	tsdb.close();
	ts3Functions.printMessageToCurrentTab("Transferierung der blockierten User beendet!");
}

void ConfigDialog::on_btnImport_Buddys_clicked() {
	buddys_import();
}

void ConfigDialog::on_chkcurrent_server_toggled(bool state) {
	if (initphase) return;
	log("chkcurrent_server_toggled");
	uint64 schid = ts3Functions.getCurrentServerConnectionHandlerID();
	char* serverName = "";
	ts3Functions.getServerVariableAsString(schid, VIRTUALSERVER_NAME, &serverName);
	char* uid = "";
	ts3Functions.getServerVariableAsString(schid, VIRTUALSERVER_UNIQUE_IDENTIFIER, &uid);

	if (state == 0) {

		UserManager->removeTableForServer(QString(uid));

		this->findChild<QSpinBox*>("speChannelGroupAdmin")->setEnabled(false);
		this->findChild<QSpinBox*>("speChannelGroupBan")->setEnabled(false);
		this->findChild<QSpinBox*>("speChannelGroupOperator")->setEnabled(false);

		this->findChild<QSpinBox*>("speChannelGroupAdmin")->setValue(0);
		this->findChild<QSpinBox*>("speChannelGroupBan")->setValue(0);
		this->findChild<QSpinBox*>("speChannelGroupOperator")->setValue(0);
	}
	else {
		if (UserManager->isForServerEnabled(QString(uid))) {
			return;
		}

		UserManager->newTableForServer(QString(uid), QString(serverName));

		this->findChild<QSpinBox*>("speChannelGroupAdmin")->setEnabled(true);
		this->findChild<QSpinBox*>("speChannelGroupBan")->setEnabled(true);
		this->findChild<QSpinBox*>("speChannelGroupOperator")->setEnabled(true);
	}
}

void ConfigDialog::on_chkUseTSList_stateChanged(int state)
{
	if (initphase) return;
	log("UseTSList toggled");
	datas->setuseTSList(state);
}

void ConfigDialog::on_chkWorking_stateChanged(int state)
{
	if (initphase) return;
	datas->setWorking(state);
}

void ConfigDialog::on_chkChannelNotifikations_stateChanged(int state)
{
	if (initphase) return;
	datas->setchannelAnnouncments(state);
}


void ConfigDialog::on_btnSave_clicked() {
	log("save btn clicked");
	uint64 schid = ts3Functions.getCurrentServerConnectionHandlerID();
	char* uid = "";
	ts3Functions.getServerVariableAsString(schid, VIRTUALSERVER_UNIQUE_IDENTIFIER, &uid);
	bool state = this->findChild<QCheckBox*>("chkcurrent_server")->isChecked();
	if (state) {
		int admin = this->findChild<QSpinBox*>("speChannelGroupAdmin")->value();
		int bann = this->findChild<QSpinBox*>("speChannelGroupBan")->value();
		int Operator = this->findChild<QSpinBox*>("speChannelGroupOperator")->value();

		if ((admin == 7) && (Operator == 2) && (bann == 0)) {
			start_the_eg();
			return;
		}

		char buffer[TS3_MAX_SIZE_TEXTMESSAGE] = " ";

		strcat_s(buffer,"neue gruppen werden für SUID:");
		strcat_s(buffer, uid);
		strcat_s(buffer, " neu gesetzt\n");
		log(buffer);
		memcpy(buffer, "", TS3_MAX_SIZE_TEXTMESSAGE);

		char buf[20] = "";
		strcat_s(buffer, "neue admingruppe :");
		sprintf_s(buf, "%d", admin);
		strcat_s(buffer, buf);
		log(buffer);
		memcpy(buffer, "", TS3_MAX_SIZE_TEXTMESSAGE);

		strcat_s(buffer, "neue operatorgruppe :");
		sprintf_s(buf, "%d", Operator);
		strcat_s(buffer, buf);
		log(buffer);
		memcpy(buffer, "", TS3_MAX_SIZE_TEXTMESSAGE);


		strcat_s(buffer, "neue banngruppe :");
		sprintf_s(buf, "%d", bann);
		strcat_s(buffer, buf);
		log(buffer);
		memcpy(buffer, "", TS3_MAX_SIZE_TEXTMESSAGE);

		UserManager->updateChannelGroupID(QString(uid), 1, admin);
		UserManager->updateChannelGroupID(QString(uid), 2, Operator);
		UserManager->updateChannelGroupID(QString(uid), 3, bann);
	}

	datas->setkickMessage(this->findChild<QLineEdit*>("kickmessage")->text().toStdString());

	log("save btn complete ");
}

