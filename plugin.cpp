
#define _CRT_SECURE_NO_WARNINGS
#ifdef _WIN32
#pragma warning (disable : 4100)  /* Disable Unreferenced parameter warning */
#include <Windows.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "teamspeak/public_errors.h"
#include "teamspeak/public_errors_rare.h"
#include "teamspeak/public_definitions.h"
#include "teamspeak/public_rare_definitions.h"
#include "teamspeak/clientlib_publicdefinitions.h"
#include "ts3_functions.h"  
#include "plugin.h"
#include <string>
#include <thread>
#include <vector>
#include <fstream>
#include <iostream>
#include <QtSql/qsqldriver.h>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include "sqlwrapper.h"
#include <ctime>
#include "configDialog.h"
#include "userwidget.hpp"
#include <queue>
#include "help_functions.h"


#ifdef _WIN32
#define _strcpy(dest, destSize, src) strcpy_s(dest, destSize, src)
#define snprintf sprintf_s
#else
#define _strcpy(dest, destSize, src) { strncpy(dest, src, destSize-1); (dest)[destSize-1] = '\0'; }
#endif



#define PLUGIN_API_VERSION 22

#define PATH_BUFSIZE 512
#define COMMAND_BUFSIZE 128
#define INFODATA_BUFSIZE 128
#define SERVERINFO_BUFSIZE 256
#define CHANNELINFO_BUFSIZE 512
#define RETURNCODE_BUFSIZE 128

static char* pluginID = NULL;


/*********************************** Required functions ************************************/
/*
* If any of these required functions is not implemented, TS3 will refuse to load the plugin
*/

/* Unique name identifying this plugin */
const char* ts3plugin_name() {
	return "User Manager";
}

/* Plugin version */
const char* ts3plugin_version() {
	return "4.3.4";
}



/* Plugin API version. Must be the same as the clients API major version, else the plugin fails to load. */
int ts3plugin_apiVersion() {
	return PLUGIN_API_VERSION;
}

/* Plugin author */
const char* ts3plugin_author() {
	/* If you want to use wchar_t, see ts3plugin_name() on how to use */
	return "shitty720";
}

/* Plugin description */
const char* ts3plugin_description() {
	/* If you want to use wchar_t, see ts3plugin_name() on how to use */
	return "With this plugin you can put users and names on a blocklist, these users are filtered out of the channel. If a buddy himself is given channel ban, this will not be withdrawn car or if the channel admin has given this.";
}

/* Set TeamSpeak 3 callback functions */
void ts3plugin_setFunctionPointers(const struct TS3Functions funcs) {
	ts3Functions = funcs;
}

/*
* Custom code called right after loading the plugin. Returns 0 on success, 1 on failure.
* If the function returns 1 on failure, the plugin will be unloaded again.
*/
void unmuteuser(uint64 serverConnectionHandlerID, const anyID cClientID[]);
void muteuser(uint64 serverConnectionHandlerID, const anyID cClientID[]);
bool checkisonline(uint64 serverConnectionHandlerID, anyID clientID);
QString getSUID(uint64 serverConnectionHandlerID);
bool rechtecheck(uint64 serverConnectionHandlerID,int);
char iniconfigPath[PATH_BUFSIZE];
char configPathUserDB[PATH_BUFSIZE]; // Datenbank
char settingsDBpath[PATH_BUFSIZE];
char lastmessage[TS3_MAX_SIZE_TEXTMESSAGE];
void giveverification(uint64 serverconnectionhandlerid,int i, anyID clientID);
bool running = true; // variable for the thread Loop to stop the loop if the plugin get shutdown
void moveeventwork(uint64 serverConnectionHandlerID, anyID clientID, uint64 oldChannelID, uint64 newChannelID);
void ThreadLoop();

sqlw* UserManager;
ConfigData *Datas;
bool dbnotopen;

anyID currentUser = 0;

template <typename T>
void BannedUserProc(uint64 serverConnectionHandlerID, anyID clientID, uint64 channelID, T blockedUser) { // template for blockedUser and blockedName

	if (blockedUser.AutoBan) {

		anyID myID;
		int myCGID; // channelgroupID
		ts3Functions.getClientID(serverConnectionHandlerID, &myID); //meine id
		ts3Functions.getClientVariableAsInt(serverConnectionHandlerID, myID, 0x21, &myCGID);

		if ((myCGID == UserManager->readChannelGroupID(getSUID(serverConnectionHandlerID), 1)) || (myCGID == UserManager->readChannelGroupID(getSUID(serverConnectionHandlerID), 2))) {

			int CCGID; // clientchannelgroupid
			ts3Functions.getClientVariableAsInt(serverConnectionHandlerID, clientID, CLIENT_CHANNEL_GROUP_ID, &CCGID);
			int standertgroup;
			ts3Functions.getServerVariableAsInt(serverConnectionHandlerID, VIRTUALSERVER_DEFAULT_CHANNEL_GROUP, &standertgroup);
			if (CCGID == standertgroup) {

				ts3Functions.setchannelgroup(serverConnectionHandlerID, clientID, channelID, UserManager->readChannelGroupID(getSUID(serverConnectionHandlerID), 3));
				uint64 myChannelID = 0;
				uint64 clientChannelID = 0;
				ts3Functions.getChannelOfClient(serverConnectionHandlerID, myID, &myChannelID);
				ts3Functions.getChannelOfClient(serverConnectionHandlerID, clientID, &clientChannelID);
				if ((myChannelID == clientChannelID) && (blockedUser.AutoKick)) {
					ts3Functions.requestClientKickFromChannel(serverConnectionHandlerID, clientID, GetKickMessage().c_str(), nullptr/*returnCode*/);
					if (Datas->getwantannoucments()) {
						giveverification(serverConnectionHandlerID, 12, clientID);
						return;
					}
				}
			}

			if (CCGID == UserManager->readChannelGroupID(getSUID(serverConnectionHandlerID), 3)) {

				uint64 myChannelID = 0;
				uint64 clientChannelID = 0;
				ts3Functions.getChannelOfClient(serverConnectionHandlerID, myID, &myChannelID);
				ts3Functions.getChannelOfClient(serverConnectionHandlerID, clientID, &clientChannelID);
				if ((myChannelID == clientChannelID) && (blockedUser.AutoKick)) {
					ts3Functions.requestClientKickFromChannel(serverConnectionHandlerID, clientID, GetKickMessage().c_str(), nullptr/*returnCode*/);
					if (Datas->getwantannoucments()) {
						giveverification(serverConnectionHandlerID, 12, clientID);
					}
				}
			}
		}
	}
}

void BuddyUserProc(uint64 serverConnectionHandlerID, anyID clientID, uint64 channelID, BuddyUser buddyUser) {
	anyID myID;
	ts3Functions.getClientID(serverConnectionHandlerID, &myID); //meine id
	if (buddyUser.AutoOperator) {

		if (rechtecheck(serverConnectionHandlerID, 1)) {

			int clientchannelGroup;//clientchannelgroupID nicht meine
			ts3Functions.getClientVariableAsInt(serverConnectionHandlerID, clientID, CLIENT_CHANNEL_GROUP_ID, &clientchannelGroup);
			int standertgroup;
			ts3Functions.getServerVariableAsInt(serverConnectionHandlerID, VIRTUALSERVER_DEFAULT_CHANNEL_GROUP, &standertgroup);

			if (clientchannelGroup == standertgroup) {

				ts3Functions.setchannelgroup(serverConnectionHandlerID, clientID, channelID, UserManager->readChannelGroupID(getSUID(serverConnectionHandlerID), 2));
				if (Datas->getwantannoucments()) {
					giveverification(serverConnectionHandlerID, 14, clientID);
				}
				return;
			}
		}
	}

	if (buddyUser.AutoTalkpower) {

		if (rechtecheck(serverConnectionHandlerID, 2)) { // meine channelgroup abfrage
			uint64 mychannelID;
			uint64 clientchannelID;
			int NCTP;

			ts3Functions.getChannelOfClient(serverConnectionHandlerID, myID, &mychannelID); // meine channelid
			ts3Functions.getChannelOfClient(serverConnectionHandlerID, clientID, &clientchannelID); // client channelid
			if ((mychannelID == clientchannelID) && (clientchannelID != NULL)) { // ist er in meinem channel
				ts3Functions.getChannelVariableAsInt(serverConnectionHandlerID, mychannelID, CHANNEL_NEEDED_TALK_POWER, &NCTP);

				int clientchannelGroup;//0x21

				ts3Functions.getClientVariableAsInt(serverConnectionHandlerID, clientID, 0x21, &clientchannelGroup);

				//if ((clientchannelGroup == 11 && NCTP >= 24) || (clientchannelGroup == 10 && NCTP >= 26) || clientchannelGroup == 9  && NCTP >= 0) {
				int standertgroup;
				ts3Functions.getServerVariableAsInt(serverConnectionHandlerID, VIRTUALSERVER_DEFAULT_CHANNEL_GROUP, &standertgroup);

				if ((clientchannelGroup == standertgroup) && (NCTP > 0)) {

					ts3Functions.requestClientSetIsTalker(serverConnectionHandlerID, clientID, 1, NULL);
					if (Datas->getwantannoucments()) {
						giveverification(serverConnectionHandlerID, 13, clientID);
					}
				}
			}
		}
	}
}


#pragma region UID-BANLIST

void flipflopUserBanned(uint64 serverConnectionHandlerID, anyID clientID) {

	if (!checkisonline(serverConnectionHandlerID, clientID)) {
		return;
	}

	char *UID;
	ts3Functions.getClientVariableAsString(serverConnectionHandlerID, clientID, CLIENT_UNIQUE_IDENTIFIER, &UID);
	BuddyUser buddyUser = UserManager->isBuddy(UID);
	if (buddyUser.dummy_Return) {
		ts3Functions.printMessageToCurrentTab("User is not blockable because he is on the buddylist");
		return;
	}

	char* name = "";
	ts3Functions.getClientVariableAsString(serverConnectionHandlerID, clientID, CLIENT_NICKNAME, &name);



	BlockedUser blockedUser = UserManager->isBlocked(UID);
	if (!blockedUser.dummy_Return) {
		blockedUser.AutoBan = Datas->getAutoBan();
		blockedUser.AutoKick = Datas->getAutoKick();
		blockedUser.UID = UID;
		blockedUser.SavedName = name;


		

		UserManager->addBlockedList(blockedUser);

		if (clientID != NULL) {
			const anyID cClientID[] = { clientID };

			if (Datas->getautomute()) {
				muteuser(serverConnectionHandlerID,cClientID);
				
			}

			if (Datas->getwantannoucments()) {
				giveverification(serverConnectionHandlerID, 1, clientID);
			}

			if (rechtecheck(serverConnectionHandlerID, 2)) {
				anyID myID;
				ts3Functions.getClientID(serverConnectionHandlerID, &myID);
				uint64 newChannelID;
				ts3Functions.getChannelOfClient(serverConnectionHandlerID, myID, &newChannelID);
				if (Datas->getWorking())
					BannedUserProc(serverConnectionHandlerID, clientID, newChannelID, blockedUser);
			}
		}
	}
	else {
		const anyID cClientID[] = { clientID };
		if (Datas->getautomute()) {
			unmuteuser(serverConnectionHandlerID, cClientID);
			
		}
		UserManager->removeBlockedList(blockedUser);
		if (Datas->getwantannoucments()) {
			giveverification(serverConnectionHandlerID, 15, clientID);
		}
	}
}
#pragma endregion
#pragma region NAME_BANLIST
// list of banned names

void flipflopUserNameBanned(uint64 serverConnectionHandlerID, anyID clientID) {

	if (!checkisonline(serverConnectionHandlerID, clientID)) {
		return;
	}

	char* name = "";
	ts3Functions.getClientVariableAsString(serverConnectionHandlerID, clientID, CLIENT_NICKNAME, &name);

	BlockedName blockedName = UserManager->isNameBlocked(name);

	if (!blockedName.dummy_Return) {

		blockedName.Name = name;
		blockedName.AutoBan = Datas->getAutoBan();
		blockedName.AutoKick = Datas->getAutoKick();

		UserManager->addNameList(blockedName);

		if (clientID != NULL) {

			if (Datas->getwantannoucments()) {
				giveverification(serverConnectionHandlerID,3, clientID);
			}

			if (rechtecheck(serverConnectionHandlerID,2)) {
				anyID myID;
				ts3Functions.getClientID(serverConnectionHandlerID, &myID);
				uint64 newChannelID;
				ts3Functions.getChannelOfClient(serverConnectionHandlerID, myID, &newChannelID);
				if (Datas->getWorking())
				BannedUserProc(serverConnectionHandlerID, clientID, newChannelID,blockedName);

			}
		}
	}
	else {
		UserManager->removeNameList(blockedName);
		if (Datas->getwantannoucments()) {
			giveverification(serverConnectionHandlerID,17, clientID);
		}
	}
}

#pragma endregion
#pragma region Country_BANLIST


void flipflopCountryBanned(uint64 serverConnectionHandlerID, anyID clientID) {

	if (!checkisonline(serverConnectionHandlerID, clientID)) {
		return;
	}

	char* countryTag = "";
	ts3Functions.getClientVariableAsString(serverConnectionHandlerID, clientID, CLIENT_COUNTRY, &countryTag);

	BlockedCountry blockedCountry = UserManager->isCountryBlocked(countryTag);

	if (!blockedCountry.dummy_Return) {

		blockedCountry.CountryTag = countryTag;
		blockedCountry.AutoBan = Datas->getAutoBan();
		blockedCountry.AutoKick = Datas->getAutoKick();

		UserManager->addCountryList(blockedCountry);

		if (clientID != NULL) {

			if (Datas->getwantannoucments()) {
				giveverification(serverConnectionHandlerID, 25, clientID);
			}

			if (rechtecheck(serverConnectionHandlerID, 2)) {
				anyID myID;
				ts3Functions.getClientID(serverConnectionHandlerID, &myID);
				uint64 newChannelID;
				ts3Functions.getChannelOfClient(serverConnectionHandlerID, myID, &newChannelID);
				if (Datas->getWorking())
					BannedUserProc(serverConnectionHandlerID, clientID, newChannelID, blockedCountry);

			}
		}
	}
	else {
		UserManager->removeCountryList(blockedCountry);
		if (Datas->getwantannoucments()) {
			giveverification(serverConnectionHandlerID, 26, clientID);
		}
	}
}

#pragma endregion
#pragma region UID-BUDDYLIST


void flipflopBuddysUser(uint64 serverConnectionHandlerID,  anyID clientID) {
	
	if (!checkisonline(serverConnectionHandlerID, clientID)) {
		return;
	}

	char* UID = "";
	ts3Functions.getClientVariableAsString(serverConnectionHandlerID, clientID, CLIENT_UNIQUE_IDENTIFIER, &UID);

	if (UserManager->isBlocked(UID).dummy_Return) {
		ts3Functions.printMessageToCurrentTab("User is not buddyable because he is on the blocklist");
		return;
	}

	BuddyUser buddyUser = UserManager->isBuddy(UID);

	if (!buddyUser.dummy_Return) {
		char* name = "";
		ts3Functions.getClientVariableAsString(serverConnectionHandlerID, clientID, CLIENT_NICKNAME, &name);

		buddyUser.UID = UID;
		buddyUser.AntiChannelBan = Datas->getAntiChannelBan();
		buddyUser.AutoOperator = Datas->getAutoOperator();
		buddyUser.AutoTalkpower = Datas->getAutoTP();
		buddyUser.SavedName = name;

		UserManager->addBuddyList(buddyUser);
		if (clientID != NULL) {
			if (Datas->getwantannoucments()) {
				giveverification(serverConnectionHandlerID,2, clientID);
			}
		}
	}

	else {

		UserManager->removeBuddyList(buddyUser);
		if (clientID != NULL) {
			if (Datas->getwantannoucments()) {
				giveverification(serverConnectionHandlerID,16, clientID);
			}
		}
	}
}
#pragma endregion

bool isEnabledTS(uint64 serverConnectionHandlerID) {
	return UserManager->isForServerEnabled(getSUID(serverConnectionHandlerID));
}

void muteuser(uint64 serverConnectionHandlerID, const anyID cClientID[]) { 
	ts3Functions.requestMuteClients(serverConnectionHandlerID, cClientID, NULL);
}

void unmuteuser(uint64 serverConnectionHandlerID, const anyID cClientID[]) {
	ts3Functions.requestUnmuteClients(serverConnectionHandlerID, cClientID, NULL);
}

enum {
	MENU_ID_GLOBAL_1 = 1,
	MENU_ID_GLOBAL_2,
	MENU_ID_GLOBAL_3,
	MENU_ID_CLIENT_1,
	MENU_ID_CLIENT_2,
	MENU_ID_CLIENT_3,
	MENU_ID_CLIENT_4,
	MENU_ID_CLIENT_5,
	MENU_ID_CLIENT_6,
	MENU_ID_CLIENT_7
};



int ts3plugin_init() {
	char appPath[PATH_BUFSIZE];
	char resourcesPath[PATH_BUFSIZE];
	char configPath[PATH_BUFSIZE];
	char pluginPath[PATH_BUFSIZE];
	/* Your plugin init code here */
	


	/* Example on how to query application, resources and configuration paths from client */
	/* Note: Console client returns empty string for app and resources path */
	ts3Functions.getAppPath(appPath, PATH_BUFSIZE);
	ts3Functions.getResourcesPath(resourcesPath, PATH_BUFSIZE);
	ts3Functions.getConfigPath(configPath, PATH_BUFSIZE);
	ts3Functions.getPluginPath(pluginPath, PATH_BUFSIZE);

	strcat(iniconfigPath, configPath);
	strcat(iniconfigPath,"plugins\\UserManager\\Userconfig.ini");

	strcat(settingsDBpath, configPath);
	strcat(settingsDBpath, "settings.db");

	strcat(configPathUserDB, configPath);
	strcat(configPathUserDB, "plugins\\UserManager\\UserManager.db");

	strcat(logPath, configPath);
	strcat(logPath, "plugins\\UserManager\\log.txt");

	printf("PATHS FOR USER MANAGER\n");
	printf(settingsDBpath);
	printf("\n");
	printf(configPathUserDB);
	printf("\n");
	printf(iniconfigPath);
	printf("\n");
	printf(logPath);
	printf("\n");
	//ini file

	logInitTS3Funktion(ts3Functions);

	
	loginit(logPath);

	Datas = new ConfigData(iniconfigPath);

	UserManager = new sqlw(configPathUserDB, settingsDBpath,Datas);


	std::thread QueueWorkerThread(ThreadLoop);
	QueueWorkerThread.detach();


	return 0;  /* 0 = success, 1 = failure, -2 = failure but client will not show a "failed to load" warning */
			   /* -2 is a very special case and should only be used if a plugin displays a dialog (e.g. overlay) asking the user to disable
			   * the plugin again, avoiding the show another dialog by the client telling the user the plugin failed to load.
			   * For normal case, if a plugin really failed to load because of an error, the correct return value is 1. */
}

/* Custom code called right before the plugin is unloaded */
void ts3plugin_shutdown() {
	/* Your plugin cleanup code here */
	
	running = false;
	delete UserManager;
	delete Datas;
	logclose();

	/*
	* Note:
	* If your plugin implements a settings dialog, it must be closed and deleted here, else the
	* TeamSpeak client will most likely crash (DLL removed but dialog from DLL code still open).
	*/

	/* Free pluginID if we registered it */
	if (pluginID) {
		free(pluginID);
		pluginID = NULL;
	}
}

/****************************** Optional functions ********************************/
/*
* Following functions are optional, if not needed you don't need to implement them.
*/

/* Tell client if plugin offers a configuration window. If this function is not implemented, it's an assumed "does not offer" (PLUGIN_OFFERS_NO_CONFIGURE). */
int ts3plugin_offersConfigure() {
	//printf("PLUGIN: offersConfigure\n");
	/*
	* Return values:
	* PLUGIN_OFFERS_NO_CONFIGURE         - Plugin does not implement ts3plugin_configure
	* PLUGIN_OFFERS_CONFIGURE_NEW_THREAD - Plugin does implement ts3plugin_configure and requests to run this function in an own thread
	* PLUGIN_OFFERS_CONFIGURE_QT_THREAD  - Plugin does implement ts3plugin_configure and requests to run this function in the Qt GUI thread
	*/
	return PLUGIN_OFFERS_CONFIGURE_QT_THREAD;  /* In this case ts3plugin_configure does not need to be implemented */
}

/* Plugin might offer a configuration window. If ts3plugin_offersConfigure returns 0, this function does not need to be implemented. */
void ts3plugin_configure(void* handle, void* qParentWidget) {
	ConfigDialog* config = new ConfigDialog( UserManager,ts3Functions,Datas,(QWidget*)qParentWidget);
	config->setAttribute(Qt::WA_DeleteOnClose);
	config->setWindowFlags(config->windowFlags() & ~Qt::WindowContextHelpButtonHint);
	config->exec();
}

/*
* If the plugin wants to use error return codes, plugin commands, hotkeys or menu items, it needs to register a command ID. This function will be
* automatically called after the plugin was initialized. This function is optional. If you don't use these features, this function can be omitted.
* Note the passed pluginID parameter is no longer valid after calling this function, so you must copy it and store it in the plugin.
*/

void ts3plugin_registerPluginID(const char* id) {
	const size_t sz = strlen(id) + 1;
	pluginID = (char*)malloc(sz * sizeof(char));
	_strcpy(pluginID, sz, id);  /* The id buffer will invalidate after exiting this function */
}

/* Plugin command keyword. Return NULL or "" if not used. */
const char* ts3plugin_commandKeyword() {
	return "um";
}

/* Plugin processes console command. Return 0 if plugin handled the command, 1 if not handled. */
int ts3plugin_processCommand(uint64 serverConnectionHandlerID, const char* command) {
	if (command = "debuginfo") {
		if (Datas->getAutoBan()) {
			ts3Functions.printMessageToCurrentTab("AutoBan = true");
		}
		else {
			ts3Functions.printMessageToCurrentTab("AutoBan = false");
		}
		if (Datas->getAutoKick()) {
			ts3Functions.printMessageToCurrentTab("AutoKick = true");
		}
		else {
			ts3Functions.printMessageToCurrentTab("AutoKick = false");
		}

		if (Datas->getAutoOperator()) {
			ts3Functions.printMessageToCurrentTab("autoOperator = true");
		}
		else {
			ts3Functions.printMessageToCurrentTab("autoOperator = false");
		}

		if (Datas->getAutoTP()) {
			ts3Functions.printMessageToCurrentTab("autotp = true");
		}
		else {
			ts3Functions.printMessageToCurrentTab("autotp = false");
		}

		if (Datas->getwantannoucments()) {
			ts3Functions.printMessageToCurrentTab("annoucments = true");
		}
		else {
			ts3Functions.printMessageToCurrentTab("annoucments = false");
		}

		if (Datas->getAntiChannelBan()) {
			ts3Functions.printMessageToCurrentTab("AntiChannelBan = true");
		}
		else {
			ts3Functions.printMessageToCurrentTab("AntiChannelBan = false");
		}
		if (Datas->getautomute()) {
			ts3Functions.printMessageToCurrentTab("automute = true");
		}
		else {
			ts3Functions.printMessageToCurrentTab("automute = false");
		}

		if (Datas->getlogging()) {
			ts3Functions.printMessageToCurrentTab("logging = true");
		}
		else {
			ts3Functions.printMessageToCurrentTab("logging = false");
		}

		ts3Functions.printMessageToCurrentTab("lastmessage:");
		ts3Functions.printMessageToCurrentTab(lastmessage);
		ts3Functions.printMessageToCurrentTab("PATHS FOR USER MANAGER:");
		ts3Functions.printMessageToCurrentTab(settingsDBpath);
		ts3Functions.printMessageToCurrentTab(configPathUserDB);
		ts3Functions.printMessageToCurrentTab(iniconfigPath);
		ts3Functions.printMessageToCurrentTab(logPath);
	}
	
	

	return 0;  /* Plugin handled command */
}

/* Client changed current server connection handler */
void ts3plugin_currentServerConnectionChanged(uint64 serverConnectionHandlerID) {}

/*
* Implement the following three functions when the plugin should display a line in the server/channel/client info.
* If any of ts3plugin_infoTitle, ts3plugin_infoData or ts3plugin_freeMemory is missing, the info text will not be displayed.
*/

/* Static title shown in the left column in the info frame */
const char* ts3plugin_infoTitle() {
	return "User Manager";
}

/*
* Dynamic content shown in the right column in the info frame. Memory for the data string needs to be allocated in this
* function. The client will call ts3plugin_freeMemory once done with the string to release the allocated memory again.
* Check the parameter "type" if you want to implement this feature only for specific item types. Set the parameter
* "data" to NULL to have the client ignore the info data.
*/
void ts3plugin_infoData(uint64 serverConnectionHandlerID, uint64 id, enum PluginItemType type, char** data) {
	std::string infodata = "";
	char* UID="";
	char* name="";
	int userType = 0;  // 0 = nothing  |  1 = buddy | 2 = blocked | 3 = nameBlocked
	char* countryTag ="";

	if (type == PLUGIN_CLIENT) {
		currentUser = (anyID)id;
		if (ts3Functions.getClientVariableAsString(serverConnectionHandlerID, (anyID)id, CLIENT_UNIQUE_IDENTIFIER, &UID) != ERROR_ok) {
			printf("Error getting client UID\n");
			return;
		}
		if (ts3Functions.getClientVariableAsString(serverConnectionHandlerID, (anyID)id, CLIENT_NICKNAME, &name) != ERROR_ok) {
			printf("Error getting client Nickname\n");
			return;
		}
		if (ts3Functions.getClientVariableAsString(serverConnectionHandlerID, (anyID)id, CLIENT_COUNTRY, &countryTag) != ERROR_ok) {
			printf("Error getting client Country\n");
			return;
		}

		BlockedName blockedName = UserManager->isNameBlocked(name);
		BlockedUser blockedUser = UserManager->isBlocked(UID);
		BuddyUser	buddyUser = UserManager->isBuddy(UID);
		BlockedCountry blockedCountry = UserManager->isCountryBlocked(countryTag);

		if (buddyUser.dummy_Return) {
			infodata += "User is Buddy \n";
			userType = 1;
		}

		else {
			infodata += "User is not Buddy\n";
		}


		if (blockedUser.dummy_Return) {
		
			infodata += "UserID is blocked\n";		
			userType = 2;

		}
		else {		
			infodata += "UserID is not blocked\n";		
		}
	
		
		if (blockedName.dummy_Return) {
			infodata += "Nickname is blocked\n";
			userType = 3;
		}

		else {
			infodata += " Nickname is not blocked\n";
		}

		switch (userType) {
		case 1: 
			infodata += "The SavedName of the User is: ";
			infodata += buddyUser.SavedName.toStdString();
			infodata += "\n";
			break;
		case 2:
			infodata += "The SavedName of the User is: ";
			infodata += blockedUser.SavedName.toStdString();
			infodata += "\n";
			break;
		default:
			break;
		}

		if (blockedCountry.dummy_Return) {
			infodata += "Country is blocked\n";
			userType = 3;
		}

		else {
			infodata += " Country is not blocked\n";
		}

		*data = (char*)malloc((infodata.length() + 1) * sizeof(char));
		snprintf(*data, (infodata.length() + 1), infodata.c_str());
	}
	else {
		currentUser = 0;
	}
}

/* Required to release the memory for parameter "data" allocated in ts3plugin_infoData and ts3plugin_initMenus */
void ts3plugin_freeMemory(void* data) {
	free(data);
}

/*
* Plugin requests to be always automatically loaded by the TeamSpeak 3 client unless
* the user manually disabled it in the plugin dialog.
* This function is optional. If missing, no autoload is assumed.
*/
int ts3plugin_requestAutoload() {
	return 0;  /* 1 = request autoloaded, 0 = do not request autoload */
}

/* Helper function to create a menu item */
static struct PluginMenuItem* createMenuItem(enum PluginMenuType type, int id, const char* text, const char* icon) {
	struct PluginMenuItem* menuItem = (struct PluginMenuItem*)malloc(sizeof(struct PluginMenuItem));
	menuItem->type = type;
	menuItem->id = id;
	_strcpy(menuItem->text, PLUGIN_MENU_BUFSZ, text);
	_strcpy(menuItem->icon, PLUGIN_MENU_BUFSZ, icon);
	return menuItem;
}

/* Some makros to make the code to create menu items a bit more readable */
#define BEGIN_CREATE_MENUS(x) const size_t sz = x + 1; size_t n = 0; *menuItems = (struct PluginMenuItem**)malloc(sizeof(struct PluginMenuItem*) * sz);
#define CREATE_MENU_ITEM(a, b, c, d) (*menuItems)[n++] = createMenuItem(a, b, c, d);
#define END_CREATE_MENUS (*menuItems)[n++] = NULL; assert(n == sz);

/*
* Menu IDs for this plugin. Pass these IDs when creating a menuitem to the TS3 client. When the menu item is triggered,
* ts3plugin_onMenuItemEvent will be called passing the menu ID of the triggered menu item.
* These IDs are freely choosable by the plugin author. It's not really needed to use an enum, it just looks prettier.
*/



/*
* Initialize plugin menus.
* This function is called after ts3plugin_init and ts3plugin_registerPluginID. A pluginID is required for plugin menus to work.
* Both ts3plugin_registerPluginID and ts3plugin_freeMemory must be implemented to use menus.
* If plugin menus are not used by a plugin, do not implement this function or return NULL.
*/

void ts3plugin_initMenus(struct PluginMenuItem*** menuItems, char** menuIcon) {
	
	/*
	* Create the menus
	* There are three types of menu items:
	* - PLUGIN_MENU_TYPE_CLIENT:  Client context menu
	* - PLUGIN_MENU_TYPE_CHANNEL: Channel context menu
	* - PLUGIN_MENU_TYPE_GLOBAL:  "Plugins" menu in menu bar of main window
	*
	* Menu IDs are used to identify the menu item when ts3plugin_onMenuItemEvent is called
	*
	* The menu text is required, max length is 128 characters
	*
	* The icon is optional, max length is 128 characters. When not using icons, just pass an empty string.
	* Icons are loaded from a subdirectory in the TeamSpeak client plugins folder. The subdirectory must be named like the
	* plugin filename, without dll/so/dylib suffix
	* e.g. for "test_plugin.dll", icon "1.png" is loaded from <TeamSpeak 3 Client install dir>\plugins\test_plugin\1.png
	*/

	BEGIN_CREATE_MENUS(10);  /* IMPORTANT: Number of menu items must be correct! */
	CREATE_MENU_ITEM(PLUGIN_MENU_TYPE_GLOBAL, MENU_ID_GLOBAL_1, "Where are my Buddys ???", "");
	CREATE_MENU_ITEM(PLUGIN_MENU_TYPE_GLOBAL, MENU_ID_GLOBAL_2, "Open configuration", "");
	CREATE_MENU_ITEM(PLUGIN_MENU_TYPE_GLOBAL, MENU_ID_GLOBAL_3, "Open Contactlist", "");
	CREATE_MENU_ITEM(PLUGIN_MENU_TYPE_CLIENT, MENU_ID_CLIENT_1, "Set / remove client on the blocklist", "");
	CREATE_MENU_ITEM(PLUGIN_MENU_TYPE_CLIENT, MENU_ID_CLIENT_2, "Set / remove client name on the block list", "");
	CREATE_MENU_ITEM(PLUGIN_MENU_TYPE_CLIENT, MENU_ID_CLIENT_5, "Set / Remove Client Buddy", "");
	CREATE_MENU_ITEM(PLUGIN_MENU_TYPE_CLIENT, MENU_ID_CLIENT_7, "Set / Remove Country Blocked", "");
	CREATE_MENU_ITEM(PLUGIN_MENU_TYPE_CLIENT, MENU_ID_CLIENT_3, "Set client in my channel on the Channel Ban group", "");
	CREATE_MENU_ITEM(PLUGIN_MENU_TYPE_CLIENT, MENU_ID_CLIENT_4, "Set client in my channel on the Channel Guest group", "");
	CREATE_MENU_ITEM(PLUGIN_MENU_TYPE_CLIENT, MENU_ID_CLIENT_6, "Find him in Contactlist", "");
	END_CREATE_MENUS;  /* Includes an assert checking if the number of menu items matched */

					   /*
					   * Specify an optional icon for the plugin. This icon is used for the plugins submenu within context and main menus
					   * If unused, set menuIcon to NULL
					   */
	*menuIcon = (char*)malloc(PLUGIN_MENU_BUFSZ * sizeof(char));
	_strcpy(*menuIcon, PLUGIN_MENU_BUFSZ, "t.png");

	/*
	* Menus can be enabled or disabled with: ts3Functions.setPluginMenuEnabled(pluginID, menuID, 0|1);
	* Test it with plugin command: /test enablemenu <menuID> <0|1>
	* Menus are enabled by default. Please note that shown menus will not automatically enable or disable when calling this function to
	* ensure Qt menus are not modified by any thread other the UI thread. The enabled or disable state will change the next time a
	* menu is displayed.
	*/
	/* For example, this would disable MENU_ID_GLOBAL_2: */
	/* ts3Functions.setPluginMenuEnabled(pluginID, MENU_ID_GLOBAL_2, 0); */

	/* All memory allocated in this function will be automatically released by the TeamSpeak client later by calling ts3plugin_freeMemory */
}

/* Helper function to create a hotkey */
static struct PluginHotkey* createHotkey(const char* keyword, const char* description) {
	struct PluginHotkey* hotkey = (struct PluginHotkey*)malloc(sizeof(struct PluginHotkey));
	_strcpy(hotkey->keyword, PLUGIN_HOTKEY_BUFSZ, keyword);
	_strcpy(hotkey->description, PLUGIN_HOTKEY_BUFSZ, description);
	return hotkey;
}

/* Some makros to make the code to create hotkeys a bit more readable */
#define BEGIN_CREATE_HOTKEYS(x) const size_t sz = x + 1; size_t n = 0; *hotkeys = (struct PluginHotkey**)malloc(sizeof(struct PluginHotkey*) * sz);
#define CREATE_HOTKEY(a, b) (*hotkeys)[n++] = createHotkey(a, b);
#define END_CREATE_HOTKEYS (*hotkeys)[n++] = NULL; assert(n == sz);

/*
* Initialize plugin hotkeys. If your plugin does not use this feature, this function can be omitted.
* Hotkeys require ts3plugin_registerPluginID and ts3plugin_freeMemory to be implemented.
* This function is automatically called by the client after ts3plugin_init.
*/
void ts3plugin_initHotkeys(struct PluginHotkey*** hotkeys) {
	/* Register hotkeys giving a keyword and a description.
	* The keyword will be later passed to ts3plugin_onHotkeyEvent to identify which hotkey was triggered.
	* The description is shown in the clients hotkey dialog. */
	BEGIN_CREATE_HOTKEYS(3);  /* Create 3 hotkeys. Size must be correct for allocating memory. */	
	CREATE_HOTKEY("BlockUser", "Set / remove client on the blocklist");
	CREATE_HOTKEY("BuddyUser", "Set / Remove Client Buddy");
	CREATE_HOTKEY("BlockName", "Set / remove client name on the block list");
	END_CREATE_HOTKEYS;

	/* The client will call ts3plugin_freeMemory to release all allocated memory */
}

/************************** TeamSpeak callbacks ***************************/
/*
* Following functions are optional, feel free to remove unused callbacks.
* See the clientlib documentation for details on each function.
*/

/* Clientlib */
//returns an empty if failed
void getClientIdLink(uint64 serverConnectionHandlerID, anyID clientID, std::string &clientLink) {
	std::string cClientID = std::to_string(clientID);

	char *clientUid ="";
	char *username = "";

	if (clientID == 0) {
		ts3Functions.getServerVariableAsString(serverConnectionHandlerID, VIRTUALSERVER_NAME, &username);
		clientUid = "";
	}
	else {
		ts3Functions.getClientVariableAsString(serverConnectionHandlerID, clientID, CLIENT_UNIQUE_IDENTIFIER, &clientUid);
		ts3Functions.getClientVariableAsString(serverConnectionHandlerID, clientID, CLIENT_NICKNAME, &username);
	}

	if (username == "") {
		clientLink = "";
		return;
	}

	std::string friendname = username;
	//log("username");
	//log(username);
	replace(friendname, "%20", " ");

	clientLink = "[URL=client://" + cClientID + "/" + clientUid +"~"+ friendname + "]" + username + "[/URL]";
	return;;
}
QString getSUID(uint64 serverConnectionHandlerID) {

	char* uid;
	ts3Functions.getServerVariableAsString(serverConnectionHandlerID, VIRTUALSERVER_UNIQUE_IDENTIFIER, &uid);
	return QString(uid);

}


/*
parameters:
1 = User wurde geblockt +var clientID gebraucht
2 = User wurde Buddy +var clientID gebraucht
3 = Name wurde geblockt +var clientID gebraucht

4 = AutoTP an
5 = AutoTP aus

6 = autoban an
7 = autoban aus

8 = autooperator an
9 = autooperator aus

10 = AntiChannelBan an
11 = AntiChannelBan aus

12 = geblockter User wurde aus channelgeworfen +var clientID gebraucht
13 = du hast User Talkpower gegeben +var clientID gebraucht
14 = du hast User Operator gegeben +var clientID gebraucht

15 = User wurde entblockt +var clientID gebraucht
16 = User wurde als Buddy entfernt +var clientID gebraucht
17 = Name wurde entblockt +var clientID gebraucht

18 = Du hast die notifications aktiviert
19 = Du hast die notifications deaktiviert
 
20 = wird channel bann nun nicht mehr entzogen in diesem channel +var clientID gebraucht
21 = Aktion nicht möglich da er nicht online ist

22 = Automute beim blockieren der User wurde aktiviert
23 = Automute beim blockieren der User wurde deaktiviert
24 = sqlw manager shoulwork on false

25 = Country wurde gebannt +var clientID gebraucht
26 = Country wurde entbannt +var clientID gebraucht
*/
void giveverification(uint64 serverConnectionHandlerID,int i,anyID clientID = 0) {
	
	char buffer[TS3_MAX_SIZE_TEXTMESSAGE] = "";
	strcat(buffer,getTimeStamp().c_str());
	std::string buffername = "";
	getClientIdLink(serverConnectionHandlerID,clientID, buffername);
	const char* name = buffername.c_str();

	switch(i)
	{
		case 1:
			strcat(buffer, "You blocked: ");
			strcat(buffer, name);
			ts3Functions.printMessageToCurrentTab(buffer);
			memcpy(lastmessage, buffer, TS3_MAX_SIZE_TEXTMESSAGE );
			break;

		case 2:
			strcat(buffer, "You set: ");
			strcat(buffer, name);
			strcat(buffer, " on the Buddylist");
			ts3Functions.printMessageToCurrentTab(buffer);
			memcpy(lastmessage, buffer, TS3_MAX_SIZE_TEXTMESSAGE );
			break;

		case 3:
			strcat(buffer, "You have blocked the name: ");
			strcat(buffer, name);
			ts3Functions.printMessageToCurrentTab(buffer);
			memcpy(lastmessage, buffer, TS3_MAX_SIZE_TEXTMESSAGE );
			break;

		case 4:
			ts3Functions.printMessageToCurrentTab("You activated AutoTP");
			memcpy(lastmessage, buffer, TS3_MAX_SIZE_TEXTMESSAGE );
			break;

		case 5:
			ts3Functions.printMessageToCurrentTab("You deactivated AutoTP");
			memcpy(lastmessage, buffer, TS3_MAX_SIZE_TEXTMESSAGE );
			break;

		case 6:
			ts3Functions.printMessageToCurrentTab("You activated Autochannelban");
			memcpy(lastmessage, buffer, TS3_MAX_SIZE_TEXTMESSAGE );
			break;

		case 7:
			ts3Functions.printMessageToCurrentTab("You deactivated Autochannelban");
			memcpy(lastmessage, buffer, TS3_MAX_SIZE_TEXTMESSAGE );
			break;

		case 8:
			ts3Functions.printMessageToCurrentTab("You activated Autooperator");
			memcpy(lastmessage, buffer, TS3_MAX_SIZE_TEXTMESSAGE );
			break;

		case 9:
			ts3Functions.printMessageToCurrentTab("You deactivated Autooperator");
			memcpy(lastmessage, buffer, TS3_MAX_SIZE_TEXTMESSAGE );
			break;

		case 10:
			ts3Functions.printMessageToCurrentTab("You activated Antichannelban");
			memcpy(lastmessage, buffer, TS3_MAX_SIZE_TEXTMESSAGE );
			break;
			
		case 11:
			ts3Functions.printMessageToCurrentTab("You deactivated Antichannelban");
			memcpy(lastmessage, buffer, TS3_MAX_SIZE_TEXTMESSAGE );
			break;

		case 12:

			if (Datas->getchannelAnnouncments()) {

				anyID myID;
				uint64 myChannelID;
				ts3Functions.getClientID(serverConnectionHandlerID, &myID);
				ts3Functions.getChannelOfClient(serverConnectionHandlerID, myID, &myChannelID);
				strcat(buffer, "I removed the blocked User: ");
				strcat(buffer, name);
				strcat(buffer, " out of the channel");
				ts3Functions.requestSendChannelTextMsg(serverConnectionHandlerID, buffer, myChannelID, NULL);
			}
			else {
				strcat(buffer, "You removed the blocked User: ");
				strcat(buffer, name);
				strcat(buffer, " out of the channel");
				ts3Functions.printMessageToCurrentTab(buffer);
				memcpy(lastmessage, buffer, TS3_MAX_SIZE_TEXTMESSAGE);
			}
			break;
		case 13:
			strcat(buffer, "You gave your Buddy: ");
			strcat(buffer, name);
			strcat(buffer, " Talkpower");
			ts3Functions.printMessageToCurrentTab(buffer);
			memcpy(lastmessage, buffer, TS3_MAX_SIZE_TEXTMESSAGE);
			break;
		case 14:
			strcat(buffer, "You gave your Buddy: ");
			strcat(buffer, name);
			strcat(buffer, " Operator");
			ts3Functions.printMessageToCurrentTab(buffer);
			memcpy(lastmessage, buffer, TS3_MAX_SIZE_TEXTMESSAGE);
			break;
		case 15:
			strcat(buffer, "You unblocked the User: ");
			strcat(buffer, name);
			ts3Functions.printMessageToCurrentTab(buffer);
			memcpy(lastmessage, buffer, TS3_MAX_SIZE_TEXTMESSAGE);
			break;
		case 16:
			strcat(buffer, "You removed: ");
			strcat(buffer, name);
			strcat(buffer, " out of you Buddylist");
			ts3Functions.printMessageToCurrentTab(buffer);
			memcpy(lastmessage, buffer, TS3_MAX_SIZE_TEXTMESSAGE);
			break;

		case 17:
			strcat(buffer, "You unblocked the name: ");
			strcat(buffer, name);
			ts3Functions.printMessageToCurrentTab(buffer);
			memcpy(lastmessage, buffer, TS3_MAX_SIZE_TEXTMESSAGE);
			break;

		case 18:
			ts3Functions.printMessageToCurrentTab("You activated Notifications");
			memcpy(lastmessage, "You activated Notifications", TS3_MAX_SIZE_TEXTMESSAGE);
			break;

		case 19:
			ts3Functions.printMessageToCurrentTab("You deactivated Notifications");
			memcpy(lastmessage, "You deactivated Notifications", TS3_MAX_SIZE_TEXTMESSAGE);
			break;

		case 20:
			strcat(buffer, "Du entbannst den User: ");
			strcat(buffer, name);
			strcat(buffer, " in diesem channel erstmal nicht mehr um die leute im channel nicht zu nerven");
			ts3Functions.printMessageToCurrentTab(buffer);
			memcpy(lastmessage, buffer, TS3_MAX_SIZE_TEXTMESSAGE);
			break;

		case 21:
			ts3Functions.printMessageToCurrentTab("Action impossible because User is not online");
			memcpy(lastmessage, "Action impossible because User is not online", TS3_MAX_SIZE_TEXTMESSAGE);
			break;

		case 22:
			ts3Functions.printMessageToCurrentTab("Action impossible because User is not online");
			memcpy(lastmessage, "Action impossible because User is not online", TS3_MAX_SIZE_TEXTMESSAGE);
			strcat(buffer, "Automute when blocking the user was activated");
			break;

		case 23:
			ts3Functions.printMessageToCurrentTab("Automute when blocking the user was deactivated");
			memcpy(lastmessage, "Automute when blocking the user was deactivated", TS3_MAX_SIZE_TEXTMESSAGE);
			break;
		case 24:
			ts3Functions.printMessageToCurrentTab("An internal error appered wait a second or restart teamspeak");
			memcpy(lastmessage, "An internal error appered wait a second or restart teamspeak", TS3_MAX_SIZE_TEXTMESSAGE);
			break;
		case 25:
		{
			char* countryTag = "";
			ts3Functions.getClientVariableAsString(serverConnectionHandlerID, clientID, CLIENT_COUNTRY, &countryTag);
			strcat(buffer, "You blocked the CountryTag: ");
			strcat(buffer, countryTag);
			ts3Functions.printMessageToCurrentTab(buffer);
			memcpy(lastmessage, buffer, TS3_MAX_SIZE_TEXTMESSAGE); 
		}
			break;
		case 26:
		{
			char* countryTag = "";
			ts3Functions.getClientVariableAsString(serverConnectionHandlerID, clientID, CLIENT_COUNTRY, &countryTag);
			strcat(buffer, "You unblocked the CountryTag: ");
			strcat(buffer, countryTag);
			ts3Functions.printMessageToCurrentTab(buffer);
			memcpy(lastmessage, buffer, TS3_MAX_SIZE_TEXTMESSAGE);
		}
		break;
	default:
		ts3Functions.printMessageToCurrentTab("internal Error");
		memcpy(lastmessage, "internal Error", TS3_MAX_SIZE_TEXTMESSAGE);
		break;
	}

	
}

void ts3plugin_onNewChannelEvent(uint64 serverConnectionHandlerID, uint64 channelID, uint64 channelParentID) {
}

void ts3plugin_onNewChannelCreatedEvent(uint64 serverConnectionHandlerID, uint64 channelID, uint64 channelParentID, anyID invokerID, const char* invokerName, const char* invokerUniqueIdentifier) {
}

void ts3plugin_onDelChannelEvent(uint64 serverConnectionHandlerID, uint64 channelID, anyID invokerID, const char* invokerName, const char* invokerUniqueIdentifier) {
}

void ts3plugin_onChannelMoveEvent(uint64 serverConnectionHandlerID, uint64 channelID, uint64 newChannelParentID, anyID invokerID, const char* invokerName, const char* invokerUniqueIdentifier) {
}

void ts3plugin_onUpdateChannelEvent(uint64 serverConnectionHandlerID, uint64 channelID) {
}

void ts3plugin_onUpdateChannelEditedEvent(uint64 serverConnectionHandlerID, uint64 channelID, anyID invokerID, const char* invokerName, const char* invokerUniqueIdentifier) {
}

void ts3plugin_onUpdateClientEvent(uint64 serverConnectionHandlerID, anyID clientID, anyID invokerID, const char* invokerName, const char* invokerUniqueIdentifier) {
}

std::string GetKickMessage() {
	std::string buf = "AutoKick | ";
	buf.append(Datas->getkickMessage());
	return buf;
}

std::queue<InfoObjectQueue> UserWorker;


void ThreadLoop() {
	log("thread Started");
	while (running) {
		Sleep(10);
		while (!UserWorker.empty()) {
			InfoObjectQueue &cache = UserWorker.front();
			moveeventwork(cache.serverConnectionHandlerID, cache.clientID, cache.oldChannelID, cache.newChannelID);
			UserWorker.pop();	
		}
	}
	log("thread Closed");
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void moveeventwork(uint64 serverConnectionHandlerID, anyID clientID, uint64 oldChannelID, uint64 newChannelID) {



	if (isEnabledTS(serverConnectionHandlerID)) {

		
			char* name="";
			char* UID="";
			char* countryTag="";

			ts3Functions.getClientVariableAsString(serverConnectionHandlerID, clientID, CLIENT_NICKNAME, &name);
			ts3Functions.getClientVariableAsString(serverConnectionHandlerID, clientID, CLIENT_UNIQUE_IDENTIFIER, &UID);
			ts3Functions.getClientVariableAsString(serverConnectionHandlerID, clientID, CLIENT_COUNTRY, &countryTag);
				
			BuddyUser buddyUser = UserManager->isBuddy(UID);
			if (buddyUser.dummy_Return) {

				BuddyUserProc(serverConnectionHandlerID, clientID, newChannelID,buddyUser);
				return;
			}

			BlockedUser blockedUser = UserManager->isBlocked(UID);
			if (blockedUser.dummy_Return) {
				BannedUserProc(serverConnectionHandlerID, clientID, newChannelID, blockedUser);
				return;
			}

			BlockedName blockedName = UserManager->isNameBlocked(name);
			if (blockedName.dummy_Return) {
				BannedUserProc(serverConnectionHandlerID, clientID, newChannelID, blockedName);
				return;
			}

			BlockedCountry blockedCountry = UserManager->isCountryBlocked(countryTag);
			if (blockedCountry.dummy_Return) {
				BannedUserProc(serverConnectionHandlerID, clientID, newChannelID, blockedCountry);
				return;
			}

	}
}

void ts3plugin_onClientMoveEvent(uint64 serverConnectionHandlerID, anyID clientID, uint64 oldChannelID, uint64 newChannelID, int visibility, const char* moveMessage) {

	if (!Datas->getWorking()) return;

	if (!UserManager->getshoudwork()) {
		giveverification(serverConnectionHandlerID, 24);
		return;
	}

	anyID myID;
	uint64 mychannelID;

	ts3Functions.getClientID(serverConnectionHandlerID, &myID);
	ts3Functions.getChannelOfClient(serverConnectionHandlerID, myID, &mychannelID);

	if (mychannelID == newChannelID && newChannelID != NULL && clientID != myID) {  //If target is in the channel we want
		UserWorker.push({ serverConnectionHandlerID, clientID, oldChannelID, newChannelID });
	}
}

int ts3plugin_onTextMessageEvent(uint64 serverConnectionHandlerID, anyID targetMode, anyID toID, anyID fromID, const char* fromName, const char* fromUniqueIdentifier, const char* message, int ffIgnored) {
	return 0;  // 0 = handle normally, 1 = client will ignore the text message 
}
/*
parameter :
1 = check for Admin
2 = check for operator
*/
bool rechtecheck(uint64 serverConnectionHandlerID, int checkForParameter) {

	anyID myID;
	ts3Functions.getClientID(serverConnectionHandlerID, &myID);

	uint64 mychannelID;
	ts3Functions.getChannelOfClient(serverConnectionHandlerID, myID, &mychannelID);

	int CGID; // channelgroupID
	ts3Functions.getClientVariableAsInt(serverConnectionHandlerID, myID, 0x21, &CGID);

	if (CGID == UserManager->readChannelGroupID(getSUID(serverConnectionHandlerID), 1)) {
		return true;
	}
	if ((CGID == UserManager->readChannelGroupID(getSUID(serverConnectionHandlerID), 2)) && checkForParameter == 2){
		return true;
	}

	return false;
}

void printNamePlusChannel(uint64 serverConnectionHandlerID, anyID clientID) {

	if (clientID == 0) {
		return;
	}

	log((int)clientID);

	std::string clientLink = "";
	getClientIdLink(serverConnectionHandlerID,clientID, clientLink);


	if (clientLink == "") return;

	uint64 channelID = 0;
	char *channelName = "";
	char *clientname = "";


	

	ts3Functions.getClientVariableAsString(serverConnectionHandlerID, clientID, CLIENT_NICKNAME, &clientname);

	ts3Functions.getChannelOfClient(serverConnectionHandlerID, clientID, &channelID);
	ts3Functions.getChannelVariableAsString(serverConnectionHandlerID, channelID, CHANNEL_NAME, &channelName);
	


	std::string buffer = "";

	buffer+= "Your Buddy: ";
	buffer += clientLink;

	buffer += " is in the channel:    ";
	buffer += channelName;

	if (channelName != "") {
		ts3Functions.printMessageToCurrentTab(buffer.c_str());
	}
	
}

void listAllBuddys(uint64 serverConnectionHandlerID) {
	uint64 numberOfClients = 0;
	
	ts3Functions.requestServerVariables(serverConnectionHandlerID);
	Sleep(1000);

	ts3Functions.getServerVariableAsUInt64(serverConnectionHandlerID, VIRTUALSERVER_MAXCLIENTS, &numberOfClients);

	if (numberOfClients == 0) {
		log("error getting numberOfClients");
		ts3Functions.printMessageToCurrentTab("Error with getting numberOfClients, please try again");
		return;
	}

	anyID *allClientIDs = new anyID[numberOfClients];
	ts3Functions.getClientList(serverConnectionHandlerID, &allClientIDs);

	std::vector<anyID> activeBuddys;
	
	for (int i = 0; i < numberOfClients; i++) {

		char *bufferUID;
		ts3Functions.getClientVariableAsString(serverConnectionHandlerID, allClientIDs[i], CLIENT_UNIQUE_IDENTIFIER, &bufferUID);

		if (UserManager->isBuddy(bufferUID).dummy_Return) {
			if (!isValueInList(activeBuddys, allClientIDs[i])) {
				activeBuddys.push_back(allClientIDs[i]);
			}
		}
	}

	
	for (auto it = activeBuddys.begin(); it != activeBuddys.end();it++) {
		printNamePlusChannel(serverConnectionHandlerID, *it);
	}

	delete[] allClientIDs;

}



bool checkisonline(uint64 serverConnectionHandlerID, anyID clientID) {
	uint64 channelID = 0;

	if (ts3Functions.getChannelOfClient(serverConnectionHandlerID, clientID, &channelID) != ERROR_ok) {
		giveverification(serverConnectionHandlerID,21);
		return false;
	}
	if (channelID == 0) {
		giveverification(serverConnectionHandlerID,21);
		return false;
	}
	return true;
}

/*
* Called when a plugin menu item (see ts3plugin_initMenus) is triggered. Optional function, when not using plugin menus, do not implement this.
*
* Parameters:
* - serverConnectionHandlerID: ID of the current server tab
* - type: Type of the menu (PLUGIN_MENU_TYPE_CHANNEL, PLUGIN_MENU_TYPE_CLIENT or PLUGIN_MENU_TYPE_GLOBAL)
* - menuItemID: Id used when creating the menu item
* - selectedItemID: Channel or Client ID in the case of PLUGIN_MENU_TYPE_CHANNEL and PLUGIN_MENU_TYPE_CLIENT. 0 for PLUGIN_MENU_TYPE_GLOBAL.
*/
void ts3plugin_onMenuItemEvent(uint64 serverConnectionHandlerID, enum PluginMenuType type, int menuItemID, uint64 selectedItemID) {
	switch (type) {
	case PLUGIN_MENU_TYPE_GLOBAL:
		switch (menuItemID) {
		case MENU_ID_GLOBAL_1: {
			// sucht alle buddys und listet sie im chat plus ihren gegenwärtigen channel auf;
		
			
			
			std::thread listallbuddysthread(listAllBuddys, serverConnectionHandlerID);
			listallbuddysthread.detach();

			break;
		}
		case MENU_ID_GLOBAL_2: {
			// öffnet die konfiguration

			ConfigDialog* config = new ConfigDialog(UserManager, ts3Functions, Datas);
			config->setAttribute(Qt::WA_DeleteOnClose);
			config->setWindowFlags(config->windowFlags() & ~Qt::WindowContextHelpButtonHint);
			config->exec();
			

			break;
		}case MENU_ID_GLOBAL_3: {
			// öffnet die konfiguration

			UserWidget* userwidget = new UserWidget(UserManager);
			userwidget->setAttribute(Qt::WA_DeleteOnClose);
			userwidget->setWindowFlags(userwidget->windowFlags() & ~Qt::WindowContextHelpButtonHint);
			userwidget->show();

			break;
		}
		default:
			break;
		}
	case PLUGIN_MENU_TYPE_CLIENT:
		/* Client contextmenu item was triggered. selectedItemID is the clientID of the selected client */
		switch (menuItemID) {
		case MENU_ID_CLIENT_1: {
			/* Menu client 1 was triggered */
			// client uid bannen/entbannen
			std::thread flipflopUserBannedthread(flipflopUserBanned,serverConnectionHandlerID, (anyID)selectedItemID);
			flipflopUserBannedthread.detach();
			break;
		}					

		case MENU_ID_CLIENT_2: {
			/* Menu client 2 was triggered */
			// namen bannen/entbannen
			std::thread flipflopUserNameBannedthread(flipflopUserNameBanned,serverConnectionHandlerID, (anyID)selectedItemID);
			flipflopUserNameBannedthread.detach();
			break;
		
		}case MENU_ID_CLIENT_7: {
			/* Menu client 2 was triggered */
			// Country bannen/entbannen
			std::thread flipflopCountryBannedthread(flipflopCountryBanned, serverConnectionHandlerID, (anyID)selectedItemID);
			flipflopCountryBannedthread.detach();
			break;
		}
		case MENU_ID_CLIENT_3: {
			/* Menu client 3 was triggered */
			// Client in anderem channel in meinem channel channelBann geben
			anyID myID;
			uint64 mychannelID;

			ts3Functions.getClientID(serverConnectionHandlerID, &myID);
			ts3Functions.getChannelOfClient(serverConnectionHandlerID, myID, &mychannelID);
			ts3Functions.setchannelgroup(serverConnectionHandlerID, (anyID)selectedItemID, mychannelID, UserManager->readChannelGroupID(getSUID(serverConnectionHandlerID),3));
			break;
		}

		case MENU_ID_CLIENT_4: {
			/* Menu client 4 was triggered */
			// gibt channelgruppe gast
			anyID myID;
			uint64 mychannelID;
			ts3Functions.getClientID(serverConnectionHandlerID, &myID);
			ts3Functions.getChannelOfClient(serverConnectionHandlerID, myID, &mychannelID);
			int standertgroup;
			ts3Functions.getServerVariableAsInt(serverConnectionHandlerID, VIRTUALSERVER_DEFAULT_CHANNEL_GROUP, &standertgroup);
			ts3Functions.setchannelgroup(serverConnectionHandlerID, (anyID)selectedItemID, mychannelID, standertgroup);

			break;
		}

		case MENU_ID_CLIENT_5: {
			/* Menu client 5 was triggered */
			//setzt nen client auf bud oder nimmst ihm weg
		

			std::thread flipflopBuddysUserthread( flipflopBuddysUser,serverConnectionHandlerID, (anyID)selectedItemID);
			flipflopBuddysUserthread.detach();
			break;
		}
		case MENU_ID_CLIENT_6: {
			/* Menu client 6 was triggered */

			if (!checkisonline(serverConnectionHandlerID, (anyID)selectedItemID))break;

			bool buddy = false;
			char* UID;
			ts3Functions.getClientVariableAsString(serverConnectionHandlerID, (anyID)selectedItemID, CLIENT_UNIQUE_IDENTIFIER, &UID);
			BuddyUser buddyUser = UserManager->isBuddy(UID);
			if (buddyUser.dummy_Return) {
				buddy = true;
			}
			UserWidget* userwidget = new UserWidget(UserManager, UID, buddy); 
			userwidget->setAttribute(Qt::WA_DeleteOnClose);
			userwidget->setWindowFlags(userwidget->windowFlags() & ~Qt::WindowContextHelpButtonHint);
			userwidget->show();


			break;
		}
	}
		break;
	default:
		break;
	}
}

void ts3plugin_onClientChannelGroupChangedEvent(uint64 serverConnectionHandlerID, uint64 channelGroupID, uint64 channelID, anyID clientID, anyID invokerClientID, const char* invokerName, const char* invokerUniqueIdentity) {
	anyID myID;
	uint64 mychannelid;
	uint64 myChannelGroupID;
	uint64 invokerchannelGroupID;
	char* UID;

	//own clientid
	if (ts3Functions.getClientID(serverConnectionHandlerID, &myID) != ERROR_ok) {  /* Get own client ID */
		ts3Functions.logMessage("Error querying own client id", LogLevel_ERROR, "Plugin", serverConnectionHandlerID);

	}				//own channel
	if (ts3Functions.getChannelOfClient(serverConnectionHandlerID, myID, &mychannelid) != ERROR_ok) {
		ts3Functions.logMessage("Error querying own channel id", LogLevel_ERROR, "Plugin", serverConnectionHandlerID);

	}	//test for own channel an channel ban and not my id
	if (mychannelid == channelID && myID != clientID) {

		//own channelgroupid
		ts3Functions.getClientVariableAsUInt64(serverConnectionHandlerID, myID, CLIENT_CHANNEL_GROUP_ID, &myChannelGroupID);
		//test for own channel group and channelgroup of the user and kick user

		if (channelGroupID == UserManager->readChannelGroupID(getSUID(serverConnectionHandlerID), 3)) {

			// prüfen ob user buddie ist, wenn ja bann entziehen
			ts3Functions.getClientVariableAsString(serverConnectionHandlerID, clientID, CLIENT_UNIQUE_IDENTIFIER, &UID);

			BuddyUser buddyUser = UserManager->isBuddy(UID);
			if (buddyUser.dummy_Return) {

				//invoker channelgroupid
				ts3Functions.getClientVariableAsUInt64(serverConnectionHandlerID, invokerClientID, CLIENT_CHANNEL_GROUP_ID, &invokerchannelGroupID);
				if (invokerchannelGroupID != UserManager->readChannelGroupID(getSUID(serverConnectionHandlerID), 1)) {
					if (invokerClientID != myID) {
						if (buddyUser.AntiChannelBan) {
							if (rechtecheck(serverConnectionHandlerID, 1)) {

								ts3Functions.setchannelgroup(serverConnectionHandlerID, clientID, mychannelid, UserManager->readChannelGroupID(getSUID(serverConnectionHandlerID), 2));
								return;
							}
							if (rechtecheck(serverConnectionHandlerID, 2)) {
								int standertgroup;
								ts3Functions.getServerVariableAsInt(serverConnectionHandlerID, VIRTUALSERVER_DEFAULT_CHANNEL_GROUP, &standertgroup);
								ts3Functions.setchannelgroup(serverConnectionHandlerID, clientID, mychannelid, standertgroup);
								return;
							}
						}
					}
				}
			}
		}
		if (rechtecheck(serverConnectionHandlerID,2)) {

			// prüfen ob user buddie ist, wenn ja bann entziehen
			ts3Functions.getClientVariableAsString(serverConnectionHandlerID, clientID, CLIENT_UNIQUE_IDENTIFIER, &UID);
			BuddyUser buddyUser = UserManager->isBuddy(UID);
			if (buddyUser.dummy_Return) {

				//invoker channelgroupid
				ts3Functions.getClientVariableAsUInt64(serverConnectionHandlerID, invokerClientID, CLIENT_CHANNEL_GROUP_ID, &invokerchannelGroupID);
				if (invokerchannelGroupID != UserManager->readChannelGroupID(getSUID(serverConnectionHandlerID), 1)) {
					if (invokerClientID != myID) {
						if (channelGroupID == UserManager->readChannelGroupID(getSUID(serverConnectionHandlerID), 3)) {
							if (Datas->getAntiChannelBan()) {
								ts3Functions.setchannelgroup(serverConnectionHandlerID, clientID, mychannelid, UserManager->readChannelGroupID(getSUID(serverConnectionHandlerID), 2));
								return;
							}
						}
					}
				}
			}
		}
	}
}



void ts3plugin_onHotkeyEvent(const char* keyword) {

	uint64 serverConnectionHandlerID = ts3Functions.getCurrentServerConnectionHandlerID();

	if (currentUser == 0) return;



	if (!intToBool(strcmp(keyword, "BlockUser"))) {
		std::thread flipflopUserBannedthread(flipflopUserBanned, serverConnectionHandlerID, currentUser);
		flipflopUserBannedthread.detach();
	}

	else if (!intToBool(strcmp(keyword, "BuddyUser"))) {
		std::thread flipflopBuddysUserthread(flipflopBuddysUser, serverConnectionHandlerID, currentUser);
		flipflopBuddysUserthread.detach();
	}
	else if (!intToBool(strcmp(keyword, "BlockName"))) {
		std::thread flipflopUserNameBannedthread(flipflopUserNameBanned, serverConnectionHandlerID, currentUser);
		flipflopUserNameBannedthread.detach();
	}


	/* Identify the hotkey by keyword ("keyword_1", "keyword_2" or "keyword_3" in this example) and handle here... */
}



/*
void ts3plugin_onTalkStatusChangeEvent(uint64 serverConnectionHandlerID, int status, int isReceivedWhisper, anyID clientID) {

	anyID myID;
	ts3Functions.getClientID(serverConnectionHandlerID, &myID);

	if (isEnabledTS(serverConnectionHandlerID)) {
		//if (status == 0) {


			char* UID;
			uint64 mychannelID;
			uint64 clientchannelID;
			int NCTP;
			

			ts3Functions.getChannelOfClient(serverConnectionHandlerID, myID, &mychannelID);
			ts3Functions.getChannelOfClient(serverConnectionHandlerID, clientID, &clientchannelID);
			ts3Functions.getClientVariableAsString(serverConnectionHandlerID, clientID, CLIENT_UNIQUE_IDENTIFIER, &UID);
			ts3Functions.getChannelVariableAsInt(serverConnectionHandlerID, mychannelID, CHANNEL_NEEDED_TALK_POWER, &NCTP);
			

			if (mychannelID == clientchannelID && clientchannelID != NULL) {
				int mychannelGroup;//0x21
				int clientchannelGroup;

				ts3Functions.getClientVariableAsInt(serverConnectionHandlerID, myID, 0x21, &mychannelGroup);
				ts3Functions.getClientVariableAsInt(serverConnectionHandlerID, clientID, 0x21, &clientchannelGroup);

				int clienttp;

				ts3Functions.getClientVariableAsInt(serverConnectionHandlerID, clientID, CLIENT_TALK_POWER, &clienttp);


				if (mychannelGroup == 10 || mychannelGroup == 11) {

					if (((clientchannelGroup == 11 && NCTP >= 24) || (clientchannelGroup == 10 && NCTP >= 26)) && clienttp == NCTP){
						printf("test\n");

						if (clientID == myID) {

							ts3Functions.requestClientSetIsTalker(serverConnectionHandlerID, clientID, 1, NULL);

						}

					}
				}
			//}
		}
	}
}*/

/*

manchmal bekommt man kein operator

*/