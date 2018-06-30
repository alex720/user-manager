#include "sqlwrapper.h"
#define _CRT_SECURE_NO_WARNINGS 

#define DBCHECK2 if (!open) {\
		QSqlError err = UserDB->lastError();\
		log(err.databaseText());\
		log(err.text());\
		log("Error occured with opening database");\
		return bool(-1);\
		}

#define DBOPEN	dbprotect.lock();
									
#define DBCLOSE dbprotect.unlock();	

#define DBCHECK if (!open){ \
		QSqlError err = UserDB->lastError();\
		log(err.databaseText());\
		log(err.text());\
		log("Error occured with opening database");\
		dbprotect.unlock();\
		return;}

void sqlw::openDB() {
	UserDB = new QSqlDatabase;
	*UserDB = QSqlDatabase::addDatabase("QSQLITE");
	UserDB->setDatabaseName(QString(PathUserDB.c_str()));
	if (UserDB->open()) {
		open = true;
	}
}

void sqlw::closeDB() {
	
	open = false;
	UserDB->close();
	
	delete UserDB;
}


sqlw::sqlw(const std::string userDB, const std::string tsDB, ConfigData *Datas) : PathUserDB(userDB), PathTsDB(tsDB),Datas(Datas){

	if (!fileExists(userDB.c_str())) {
		CreateFirstDB();
	}

	openDB();

	//checkForOldDB();

	

	readFillServerObjectCache();

	loadAllLists();



	log("SQLW-Object created \n");
}

sqlw::~sqlw() {
	log("SQLW-Object deleted \n");
	closeDB();
}

void sqlw::addBlockedToTable(const BlockedUser blockedUser) {
	DBOPEN
	DBCHECK

	
	QString command = QString("INSERT INTO " + BLOCKTABLE + " (UID, AutoBan, AutoKick, SavedName) VALUES ('" + blockedUser.UID + "'" + ", " + "'" + convertoString(boolToInt(blockedUser.AutoBan)).c_str() + "'" + ", " + "'" + convertoString(boolToInt(blockedUser.AutoKick)).c_str() + "'" + ", " + "'" + blockedUser.SavedName + "')");
	log(command);
	QSqlQuery query(*UserDB);
	query.exec(command);
	QSqlError err = query.lastError();
	log(err.databaseText());


		DBCLOSE
}


void sqlw::addBlockedNameToTable(const BlockedName blockedName) {
	DBOPEN
		DBCHECK



	QString command = QString("INSERT INTO " + NAMEBLOCKTABLE + " (UID,AutoBan,AutoKick) VALUES ('" + blockedName.Name + "'" + ", " + "'" + convertoString(boolToInt(blockedName.AutoBan)).c_str() + "'" + ", " + "'" + convertoString(boolToInt(blockedName.AutoKick)).c_str() + "')");
	log(command);
	QSqlQuery query(*UserDB);
	query.exec(command);
	QSqlError err = query.lastError();
	log(err.databaseText());

		DBCLOSE
			
}

void sqlw::updateBlockedInTabled( BlockedUser blockedUser)
{

	DBOPEN
	DBCHECK
	QString command = QString("UPDATE " + BLOCKTABLE + " SET AutoBan='" + convertoString(boolToInt(blockedUser.AutoBan)).c_str() + "', AutoKick='" + convertoString(boolToInt(blockedUser.AutoKick)).c_str() + "', SavedName='" + blockedUser.SavedName + "' WHERE UID='" + blockedUser.UID + "'");
	log(command);
	QSqlQuery query(*UserDB);
	query.exec(command);
	QSqlError err = query.lastError();
	log(err.databaseText());
	DBCLOSE

}

void sqlw::updateBuddyInTabled( BuddyUser buddyUser)
{

	DBOPEN
	DBCHECK
	QString command = QString("UPDATE " + BUDDYTABLE + " SET AutoOperator='" + convertoString(boolToInt(buddyUser.AutoOperator)).c_str() + "', AntiChannelBan='" + convertoString(boolToInt(buddyUser.AntiChannelBan)).c_str() + "', AutoTalkpower='" + convertoString(boolToInt(buddyUser.AutoTalkpower)).c_str() + "', SavedName='" + buddyUser.SavedName + "'" + "WHERE UID='" + buddyUser.UID + "'");
	log(command);
	QSqlQuery query(*UserDB);
	query.exec(command);
	QSqlError err = query.lastError();
	log(err.databaseText());
	DBCLOSE

}

void sqlw::updateNameInTabled( BlockedName blockedname)
{

	DBOPEN
	DBCHECK
	QString command = QString("UPDATE " + NAMEBLOCKTABLE + " SET AutoBan='" + convertoString(boolToInt(blockedname.AutoBan)).c_str() + "', AutoKick='" + convertoString(boolToInt(blockedname.AutoKick)).c_str()  + "' WHERE UID='" + blockedname.Name + "'");
	log(command);
	QSqlQuery query(*UserDB);
	query.exec(command);
	QSqlError err = query.lastError();
	log(err.databaseText());
	DBCLOSE
}

void sqlw::addBuddyToTable(const BuddyUser buddyUser) {
	DBOPEN
		DBCHECK
			QString command = QString("INSERT INTO " + BUDDYTABLE + " (UID, AntiChannelBan, AutoOperator, AutoTalkpower, SavedName) VALUES ('" + buddyUser.UID + "'" + ", "+ "'" + convertoString(boolToInt(buddyUser.AntiChannelBan)).c_str() + "'" + ", " + "'" + convertoString(boolToInt(buddyUser.AutoOperator)).c_str() + "'" + ", " + "'" + convertoString(boolToInt(buddyUser.AutoTalkpower)).c_str() + "'" + ", " + "'" + buddyUser.SavedName + "')");
			log(command);
			QSqlQuery query(*UserDB);
			query.exec(command);
			QSqlError err = query.lastError();
			log(err.databaseText());
			
	DBCLOSE
}

void sqlw::removeUserofTable(const QString &UID, const int friendN) {
	DBOPEN
	DBCHECK

	switch (friendN)
	{
	case 0:
	{
		QString command = QString("DELETE FROM " + BLOCKTABLE + " WHERE UID = '" + UID + "'");
		log(command);
		QSqlQuery query(*UserDB);
		query.exec(command);
		QSqlError err = query.lastError();
		log(err.databaseText());
		break;
	}
	case 1:
		{
		QString command = QString("DELETE FROM " + NAMEBLOCKTABLE + " WHERE UID = '" + UID + "'");
		log(command);
		QSqlQuery query(*UserDB);
		query.exec(command);
		QSqlError err = query.lastError();
		log(err.databaseText());
		break;
	}
	case 2:
	{
		QString command = QString("DELETE FROM " + BUDDYTABLE + " WHERE UID = '" + UID + "'");
		log(command);
		QSqlQuery query(*UserDB);
		query.exec(command);
		QSqlError err = query.lastError();
		log(err.databaseText());
		break;
	}
	default:
		log("Error occured removeUserofTable");
		break;
	}
	DBCLOSE
}

void sqlw::newTableForServer(const QString &SUID, const QString &name) {
	DBOPEN
	DBCHECK
	QString command = QString("INSERT INTO " + SERVERTABLE + " (SUID,SERVERNAME) VALUES ('" + SUID + "','" + name + "');");
	log(command);
	QSqlQuery query(*UserDB);
	query.exec(command);
	QSqlError err = query.lastError();
	log(err.databaseText());
	DBCLOSE

	ServerCache cache;
	cache.SUID = SUID;
	cache.servername = name;
	cache.ACHG = 0;
	cache.OCHG = 0;
	cache.BCHG = 0;
	ServerCaches.push_back(cache);

}
/*
key 1 = ADMINCHANNELGROUP
key 2 = OPERATORCHANNELGROUP
key 3 = BANNCHANNELGROUP
*/
void sqlw::updateChannelGroupID(const QString &SUID, const int key, const int newGroupID) {

	DBOPEN
	DBCHECK
		switch (key)
		{
		case 1:
		{

			for (std::vector<ServerCache>::iterator it = ServerCaches.begin(); it != ServerCaches.end(); ++it) {
				if (it->SUID == SUID) {
					it->ACHG = newGroupID;
				}
			}

			char buf[20] = "";
			sprintf_s(buf, "%d", newGroupID);
			QString command = QString("UPDATE " + SERVERTABLE + " SET ACHG = " + QString(buf) + " WHERE SUID = '" + SUID + "' ");
			log(command);
			QSqlQuery query(*UserDB);
			query.exec(command);
			QSqlError err = query.lastError();
			log(err.databaseText());

			

			break;
		}
		case 2:
		{
			for (std::vector<ServerCache>::iterator it = ServerCaches.begin(); it != ServerCaches.end(); ++it) {
				if (it->SUID == SUID) {
					it->OCHG = newGroupID;
				}
			}

			char buf[20] = "";
			sprintf_s(buf, "%d", newGroupID);
			QString command = QString("UPDATE " + SERVERTABLE + " SET OCHG = " + QString(buf) + " WHERE SUID = '" + SUID + "' ");
			log(command);
			QSqlQuery query(*UserDB);
			query.exec(command);
			QSqlError err = query.lastError();
			log(err.databaseText());

			break;
		}
		case 3:
		{
			for (std::vector<ServerCache>::iterator it = ServerCaches.begin(); it != ServerCaches.end(); ++it) {
				if (it->SUID == SUID) {
					it->BCHG = newGroupID;
				}
			}

			char buf[20] = "";
			sprintf_s(buf, "%d", newGroupID);

			QString command = QString("UPDATE " + SERVERTABLE + " SET BCHG = " + QString(buf) + " WHERE SUID = '" + SUID + "' ");
			log(command);
			QSqlQuery query(*UserDB);
			query.exec(command);
			QSqlError err = query.lastError();
			log(err.databaseText());

			break;
		}
		default:
			log("Error occured updateChannelGroupID"); 
			break;

	}
	DBCLOSE
}
/*
key 1 = ADMINCHANNELGROUP
key 2 = OPERATORCHANNELGROUP
key 3 = BANNCHANNELGROUP

*/
int sqlw::readChannelGroupID(const QString &SUID, int key) {
	//log(QString("readchannelgroupID for " + SUID + QString("mit key: ") + convertoString(key).c_str()));
		switch (key)
		{
		case 1: 
		{
			for (std::vector<ServerCache>::iterator it = ServerCaches.begin(); it != ServerCaches.end(); ++it) {
				if (it->SUID == SUID) {
					return it->ACHG;
				}
			}
			break;
		}
		case 2:
		{
			for (std::vector<ServerCache>::iterator it = ServerCaches.begin(); it != ServerCaches.end(); ++it) {
				if (it->SUID == SUID) {
					return it->OCHG;
				}
			}
			break;
			
		}
		case 3:
		{
			for (std::vector<ServerCache>::iterator it = ServerCaches.begin(); it != ServerCaches.end(); ++it) {
				if (it->SUID == SUID) {
					return it->BCHG;
				}
			}
			break;
		}
		default:
			log("Error occured with readchannelGroupID");
			
			return -1;
			break;
		}
	return -1;
	log("readchannelgroup complete");
}

bool sqlw::isForServerEnabled(const QString &SUID) {
	log(ServerCaches.size());
	for (std::vector<ServerCache>::iterator it = ServerCaches.begin(); it != ServerCaches.end(); ++it) {
		if (it->SUID == SUID) {
			return true;
		}
	}
	return false;
}

void sqlw::removeTableForServer(const QString &SUID) {
	DBOPEN
	DBCHECK
		
	QString command = QString("DELETE FROM "+ SERVERTABLE + " WHERE SUID = '" + SUID+"'");
	log(command);
	QSqlQuery query(*UserDB);
	query.exec(command);
	QSqlError err = query.lastError();
	log(err.databaseText());
	DBCLOSE

		int whereiam = 0;

		for (std::vector<ServerCache>::iterator it = ServerCaches.begin(); it != ServerCaches.end(); ++it) {
			
			if (it->SUID == SUID) {
				break;
			}
			whereiam++;
		}
		ServerCaches.erase(ServerCaches.begin() + whereiam);
}

BuddyUser sqlw::isBuddy(const std::string &UID) {
	for (auto it = stdBuddy.begin(); it != stdBuddy.end(); ++it)
	{
		if (it->compare(UID) == 0) {

			BuddyUser cache = {};
			cache.AntiChannelBan = true;
			cache.AutoOperator = true;
			cache.AutoTalkpower = true;
			cache.UID = UID.c_str();
			cache.SavedName = "one of the coolest guys ever";
			cache.dummy_Return = true;
			return cache;
		}
	}
	
	for (auto it = buddyList.begin(); it != buddyList.end(); ++it)
	{
		if (it->compare(UID) == 0) {
			BuddyUser cache = *it;
			cache.dummy_Return = true;
			return cache;
		}	
	}
	if (Datas->getuseTSList()) {

		QSqlDatabase TsDB = QSqlDatabase::addDatabase("QSQLITE");
		TsDB.setDatabaseName(QString(PathTsDB.c_str()));
		if (TsDB.open()) {

			QSqlQuery queryout = TsDB.exec(QString(QString("SELECT * FROM Contacts WHERE value LIKE '%") + UID.c_str() +"'"));

			while (queryout.next()) {

				QString buffer = queryout.value("value").toString(); //buffer = string of the "value" column
				std::string sBuffer = buffer.toStdString();//sBUffer std::string von buffer
				std::string searchstring("Friend=");
				size_t pos = sBuffer.find(searchstring);
				std::string Name = sBuffer.substr(10, pos-10);


				std::string cur = sBuffer.substr(pos + 7, 1);
				if (cur == "0") {
					BuddyUser cache = {};
					cache.AntiChannelBan = Datas->getAntiChannelBan();
					cache.AutoOperator = Datas->getAutoOperator();
					cache.AutoTalkpower = Datas->getAutoTP();
					cache.UID = UID.c_str();
					cache.SavedName = Name.c_str();
					cache.dummy_Return = true;
					return cache;
				}
			}

			TsDB.close();
		}
		else {
			log("error with opening tsDatabase");
		}


	}

	BuddyUser cache;
	cache.dummy_Return = false;
	return cache;
}

BlockedName sqlw::isNameBlocked(const std::string &name) {
	for (auto it = nameBlockList.begin(); it != nameBlockList.end(); ++it)
	{
		if (it->compare(name) == 0) {
			BlockedName cache = *it;
			cache.dummy_Return = true;
			return cache;
		}
	}
	BlockedName cache;
	cache.dummy_Return = false;
	return cache;
}

void sqlw::updateBlocked(BlockedUser blockedUser)
{
	blockList.remove(blockedUser);
	updateBlockedInTabled(blockedUser);

	blockList.push_back(blockedUser);
}

void sqlw::updateBuddy(BuddyUser buddyUser)
{
	buddyList.remove(buddyUser);
	updateBuddyInTabled(buddyUser);

	buddyList.push_back(buddyUser);
}

void sqlw::updateName(BlockedName blockedname)
{
	nameBlockList.remove(blockedname);
	updateNameInTabled(blockedname);

	nameBlockList.push_back(blockedname);
}

BlockedUser sqlw::isBlocked(const std::string &UID) {
	for (auto it = stdBlocked.begin(); it != stdBlocked.end(); ++it)
	{
		if (it->compare(UID) == 0) {
			BlockedUser cache = {};
			cache.AutoBan = true;
			cache.AutoKick = true;
			cache.UID = UID.c_str();
			cache.dummy_Return = true;
			return cache;
		}
	}
	
	for (auto it = blockList.begin(); it != blockList.end(); ++it)
	{
		if (it->compare(UID) == 0) {
			BlockedUser cache = *it;
			cache.dummy_Return = true;
			return cache;
		}
	}

	if (Datas->getuseTSList()) {

		QSqlDatabase TsDB = QSqlDatabase::addDatabase("QSQLITE");
		TsDB.setDatabaseName(QString(PathTsDB.c_str()));
		if (TsDB.open()) {

			QSqlQuery queryout = TsDB.exec(QString(QString("SELECT * FROM Contacts WHERE value LIKE '%") + UID.c_str() + "'"));

			while (queryout.next()) {

				QString buffer = queryout.value("value").toString(); //buffer = string of the "value" column

				std::string sBuffer = buffer.toStdString(); //sBUffer std::string von buffer
				std::string searchstring("Friend=");
				size_t pos = sBuffer.find(searchstring);
				std::string Name = sBuffer.substr(10, pos - 10);
				std::string cur = sBuffer.substr(pos + 7, 1);
				if (cur == "1") {

					BlockedUser cache = {};
					cache.AutoBan = Datas->getAutoBan();
					cache.AutoKick = Datas->getAutoKick();
					cache.UID = UID.c_str();
					cache.dummy_Return = true;
					cache.SavedName = Name.c_str();
					return cache;
				}
			}
			TsDB.close();
		}
		else {
			log("error with opening tsDatabase");
		}
	}
	BlockedUser cache;
	cache.dummy_Return = false;
	return cache;
}

void sqlw::loadAllLists() {
	
	loadBuddyList();
	loadBlocklist();
	loadNameBlockList();

	std::thread _loadGlobalLists(std::bind(&sqlw::loadGlobalLists, this));
	_loadGlobalLists.detach();

}

void sqlw::loadBuddyList() {
	DBOPEN
	DBCHECK
	QString command = QString("SELECT * FROM " + BUDDYTABLE);
	log(command);
	QSqlQuery query(*UserDB);
	query.exec(command);
	QSqlError err = query.lastError();
	log(err.databaseText());
	while (query.next()) {
		
		BuddyUser Cache = {};

		Cache.UID = query.value("UID").toString();
		Cache.SavedName = query.value("SavedName").toString();
		Cache.AntiChannelBan = query.value("AntiChannelBan").toBool();
		Cache.AutoOperator = query.value("AutoOperator").toBool();
		Cache.AutoTalkpower = query.value("AutoTalkpower").toBool();

		if (!isBlocked(Cache.UID.toStdString()).dummy_Return) {
			buddyList.push_back(Cache);
		}
	}

	DBCLOSE
}

void sqlw::loadBlocklist() {
	DBOPEN
	DBCHECK
	QString command = QString("SELECT * FROM " + BLOCKTABLE);
	log(command);
	QSqlQuery query(*UserDB);
	query.exec(command);
	QSqlError err = query.lastError();
	log(err.databaseText());
	while (query.next()) {
		BlockedUser Cache = {};
		Cache.UID = query.value("UID").toString();
		Cache.SavedName = query.value("SavedName").toString();
		Cache.AutoBan = query.value("AutoBan").toBool();
		Cache.AutoKick = query.value("AutoKick").toBool();
		if (!isBuddy(Cache.UID.toStdString()).dummy_Return) {
			blockList.push_back(Cache);
		}
		
	}
	DBCLOSE
}

void sqlw::loadNameBlockList() {
	DBOPEN
	DBCHECK
	QString command = QString("SELECT * FROM " + NAMEBLOCKTABLE);
	log(command);
	QSqlQuery query(*UserDB);
	query.exec(command);
	QSqlError err = query.lastError();
	log(err.databaseText());
	while (query.next()) {

		BlockedName Cache = {};
		Cache.Name = query.value("UID").toString();
		Cache.AutoBan = query.value("AutoBan").toBool();
		Cache.AutoKick = query.value("AutoKick").toBool();
		nameBlockList.push_back(Cache);
	}
	DBCLOSE
}

void sqlw::addNameList(const BlockedName blockedName) {
	addBlockedNameToTable(blockedName);
	nameBlockList.push_back(blockedName);
}

void sqlw::addBlockedList(const BlockedUser blockedUser) {
	addBlockedToTable(blockedUser);
	blockList.push_back(blockedUser);
}

void sqlw::addBuddyList(const BuddyUser buddyUser) {
	addBuddyToTable(buddyUser);
	buddyList.push_back(buddyUser);
}

void sqlw::removeNameList(const BlockedName blockedName) {
	removeUserofTable(blockedName.Name, 1);
	nameBlockList.remove(blockedName);
	
}

void sqlw::removeBlockedList(const BlockedUser blockedUser) {
	
	removeUserofTable(QString(blockedUser.UID), 0);
	blockList.remove(blockedUser);
}

void sqlw::removeBuddyList(const BuddyUser buddyUser) {
	
	removeUserofTable(buddyUser.UID, 2);
	buddyList.remove(buddyUser);
}
//
//void sqlw::checkForOldDB() {
//	
//	DBOPEN
//		DBCHECK
//
//	//	QString command = QString("SELECT EXISTS(SELECT 1 FROM "+BUDDYTABLE+" WHERE u_tag = 'AutoOperator' LIMIT 1)");
//	QSqlQuery query(*UserDB);
//
//	QString command = QString("SELECT AutoOperator FROM " + BUDDYTABLE);
//	log(command);
//	QSqlError err = query.lastError();
//	bool returnval;
//
//	if (err.nativeErrorCode() == NOERROR) {
//		returnval = true;
//	}
//
//	log(err.databaseText());
//	query.exec(command);
//	
//	
//
//	if (!returnval) {
//		printf("old database detected\n");
//	}
//	if (returnval) {
//		printf("new database detected\n");
//	}
//
//	DBCLOSE
//	
//}

void sqlw::CreateFirstDB(){
	openDB();
	DBOPEN
	DBCHECK
	{
		QString command = QString("CREATE TABLE '" + SERVERTABLE + "' (`SUID` TEXT NOT NULL UNIQUE, `ACHG` INTEGER NOT NULL DEFAULT 0, `OCHG` INTEGER NOT NULL DEFAULT 0, `BCHG` INTEGER NOT NULL DEFAULT 0, `SERVERNAME` TEXT NOT NULL, PRIMARY KEY(SUID) ); ");
	log(command);
	QSqlQuery query(*UserDB);
	query.exec(command);
	QSqlError err = query.lastError();
	log(err.databaseText());
	}

	{
		QString command = QString("CREATE TABLE `"+BLOCKTABLE+"` (`UID` TEXT,`AutoBan` INTEGER,`AutoKick` INTEGER,`SavedName` TEXT,`Color` INTEGER)");
		log(command);
		QSqlQuery query(*UserDB);
		query.exec(command);
		QSqlError err = query.lastError();
		log(err.databaseText());
	}
	{
		QString command = QString("CREATE TABLE `" + BUDDYTABLE + "` (`UID` TEXT,`AntiChannelBan` INTEGER,`AutoOperator` INTEGER,`AutoTalkpower` INTEGER,`SavedName` TEXT,`Color` INTEGER)");
		log(command);
		QSqlQuery query(*UserDB);
		query.exec(command);
		QSqlError err = query.lastError();
		log(err.databaseText());
	}
	{
		QString command = QString("CREATE TABLE `" + NAMEBLOCKTABLE + "` (`UID` TEXT,`AutoBan` INTEGER,`AutoKick` INTEGER)");
		log(command);
		QSqlQuery query(*UserDB);
		query.exec(command);
		QSqlError err = query.lastError();
		log(err.databaseText());
	}

	DBCLOSE
	{
	closeDB();
	}
}


void sqlw::loadGlobalLists() {
	int bu = 0 , bo = 0;
	
	{
		
		char* blocked = DownloadBytes("https://gist.githubusercontent.com/alex720/3f13a69b05245c04a77b11532fbefc2a/raw/blocked.txt");
		char *s = "" ;
		s = strtok(blocked, ",");

		while (s != NULL) {
			bo += 1;
				stdBlocked.push_back(s);
				

			s = strtok(NULL, ",");
		}
	}

	{
		
		char* buddy = DownloadBytes("https://gist.githubusercontent.com/alex720/8980858597c4984fe00f34fec07c6ef5/raw/buddy.txt");
		char *b = "" ;
		b = strtok(buddy, ",");		
		while (b != NULL) {
			bu += 1;

				stdBuddy.push_back(b);
				

			b = strtok(NULL, ",");
		}
	}
	
	if (!((bu == 0) && (bo == 0))) {
		shoudwork = true;
	}
	
}


bool sqlw::getshoudwork() {
	return shoudwork;
}

BuddyUser sqlw::getBuddybyUID(std::string UID)
{
	for (auto it = buddyList.begin(); it != buddyList.end(); ++it)
	{
		if (it->compare(UID) == 0) {
			BuddyUser cache = *it;
			cache.dummy_Return = true;
			return cache;
		}
	}
	BuddyUser cache = {};
	return cache;
}

BlockedUser sqlw::getBlockedbyUID(std::string UID)
{
	for (auto it = blockList.begin(); it != blockList.end(); ++it)
	{
		if (it->compare(UID) == 0) {
			BlockedUser cache = *it;
			cache.dummy_Return = true;
			return cache;
		}
	}
	BlockedUser cache = {};
	return cache;
}

BlockedName sqlw::getBlockedNamebyNAME(std::string name)
{
	for (auto it = nameBlockList.begin(); it != nameBlockList.end(); ++it)
	{
		if (it->compare(name) == 0) {
			BlockedName cache = *it;
			cache.dummy_Return = true;
			return cache;
		}
	}
	BlockedName cache = {};
	return cache;
}



void sqlw::readFillServerObjectCache() {

	DBOPEN
	DBCHECK

	QString command = QString("SELECT * FROM " + SERVERTABLE);
	log(command);

	QSqlQuery query(*UserDB);
	query.exec(command);
	QSqlError err = query.lastError();
	log(err.databaseText());

	while (query.next()) {
		QString Sbuf = "";
		int Ibuf = 0;
		ServerCache Cache = {};

		Sbuf = query.value("SUID").toString();
		log(Sbuf);
		Cache.SUID = Sbuf;

		Ibuf = query.value("ACHG").toInt();
		log(Ibuf);
		Cache.ACHG = Ibuf;

		Ibuf = query.value("OCHG").toInt();
		log(Ibuf);
		Cache.OCHG = Ibuf;

		Ibuf = query.value("BCHG").toInt();
		log(Ibuf);
		Cache.BCHG = Ibuf;

		Sbuf = query.value("servername").toString();
		log(Sbuf);
		Cache.servername = Sbuf;

		ServerCaches.push_back(Cache);
	}

	DBCLOSE
		
}

