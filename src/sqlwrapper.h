#pragma once
#include <QtCore/qsettings.h>
#include <QtCore/QString>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <list>
#include <string>
#include <fstream>
#include <iostream>
#include "help_functions.h"
#include <mutex>
#include "ServerObject.h"
#include "User.h"
#include "ConfigData.h"
#include <queue>

#define _CRT_SECURE_NO_WARNINGS 
#define PATH_BUFSIZE 512
#define BUDDYTABLE QString("buddytable")
#define BLOCKTABLE QString("blocktable")
#define NAMEBLOCKTABLE QString("nameblocktable")
#define BLOCKEDCOUNTRYTABLE QString("blockedCountrytable")
#define SERVERTABLE QString("EnabledServers")
/*
enum Task {
	NONE = 0,
	REMOVE,
	INSERT,
	UPDATE
};


struct InsertTaskObject {
	Task task;
	BuddyUser buddyUser;
	BlockedUser blockedUser;
	BlockedName blockedName;
};
*/

class sqlw {
public:

	sqlw(const std::string Path, const std::string PathTsDB, ConfigData *Datas);
	~sqlw();

	void newTableForServer(const QString &SUID, const QString &name); // f�r jeden server wird eine neue Tabelle angelegt
	void removeTableForServer(const QString &SUID);
	void updateChannelGroupID(const QString &SUID, const int key, const int newGroupID);
	int readChannelGroupID(const QString &SUID, const int key);
	
	bool isForServerEnabled(const QString &SUID);
	BuddyUser isBuddy(const std::string &UID, bool importmode = false);
	BlockedUser isBlocked(const std::string &UID, bool importmode = false);
	BlockedName isNameBlocked(const std::string &Name);
	BlockedCountry isCountryBlocked(const std::string &countryTag);

	void updateBlocked(const BlockedUser& blockedUser);
	void updateBuddy(const BuddyUser& buddyUser);
	void updateName(const BlockedName& blockedname);
	void UpdateCountry(const BlockedCountry& blockedCountry);

	void addNameList(const BlockedName& blockedName);
	void addBlockedList(const BlockedUser& blockedUser);
	void addBuddyList(const BuddyUser& buddyUser);
	void addCountryList(const BlockedCountry& blockedCountry);

	void removeNameList(const BlockedName& blockedName);
	void removeBlockedList(const BlockedUser& blockedUser);
	void removeBuddyList(const BuddyUser& buddyUser);
	void removeCountryList(const BlockedCountry& blockedCountry);

	bool getshoudwork();

	std::string PathTsDB;
	std::string PathUserDB;

	BuddyUser getBuddybyUID(std::string UID);
	BlockedUser getBlockedbyUID(std::string UID);
	BlockedName getBlockedNamebyNAME(std::string name);
	BlockedCountry getBlockedCountrybyCountryTag(std::string countryTag);

	std::list<BuddyUser> buddyList;
	std::list<BlockedName> nameBlockList;
	std::list<BlockedUser> blockList;
	std::list<BlockedCountry> countryList;

	void blocked_import();
	void buddys_import();
private:
	void openDB();
	void closeDB();

	//std::queue<InsertTaskObject> dataBaseWriter;
	//void DataBaseWriterThreadLoop();
	//bool DataBaseWriterThreadShouldWorking;


	ConfigData *Datas;

	void CreateFirstDB();
	void CreateColums();

	bool shoudwork = false;

	std::list<std::string> stdBuddy;
	std::list<std::string> stdBlocked;
	std::vector<ServerCache> ServerCaches;

	std::mutex dbprotect;
	QSqlDatabase *UserDB;
	bool open;
	bool initcomplete = false;
	std::mutex tsdbProtect;
	


	void loadGlobalLists();
	void loadAllLists();
	void loadBuddyList();
	void loadBlocklist();
	void loadNameBlockList();
	void loadBlockCountryList();
	
	void addBuddyToTable( BuddyUser buddyUser);
	void addBlockedToTable( BlockedUser blockedUser);
	void addBlockedNameToTable( BlockedName blockedName);
	void addBlockedCountryToTable( BlockedCountry blockedCountry);


	void updateBlockedInTabled( BlockedUser blockedUser);
	void updateBuddyInTabled( BuddyUser buddyUser);
	void updateNameInTabled( BlockedName blockedname);
	void updateBlockedCountryToTable( BlockedCountry blockedCountry);
	
	void removeUserofTable(const QString &UID,const int friendN);

	void readFillServerObjectCache();

};


//SUID = SERVERUID
//UID = CLIENTUID
