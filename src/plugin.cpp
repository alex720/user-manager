/*
* TeamSpeak 3 demo plugin
*
* Copyright (c) 2008-2016 TeamSpeak Systems GmbH
*/
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
#include <thread>
#include <QtCore\qsettings.h>

static struct TS3Functions ts3Functions;

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

#ifdef _WIN32
/* Helper function to convert wchar_T to Utf-8 encoded strings on Windows */
static int wcharToUtf8(const wchar_t* str, char** result) {
	int outlen = WideCharToMultiByte(CP_UTF8, 0, str, -1, 0, 0, 0, 0);
	*result = (char*)malloc(outlen);
	if (WideCharToMultiByte(CP_UTF8, 0, str, -1, *result, outlen, 0, 0) == 0) {
		*result = NULL;
		return -1;
	}
	return 0;
}
#endif

/*********************************** Required functions ************************************/
/*
* If any of these required functions is not implemented, TS3 will refuse to load the plugin
*/

/* Unique name identifying this plugin */
const char* ts3plugin_name() {
#ifdef _WIN32
	/* TeamSpeak expects UTF-8 encoded characters. Following demonstrates a possibility how to convert UTF-16 wchar_t into UTF-8. */
	static char* result = NULL;  /* Static variable so it's allocated only once */
	if (!result) {
		const wchar_t* name = L"GommeHD TS User Manager";
		if (wcharToUtf8(name, &result) == -1) {  /* Convert name into UTF-8 encoded result */
			result = "GommeHD TS User Manager";  /* Conversion failed, fallback here */
		}
	}
	return result;
#else
	return "User Manager";
#endif
}

/* Plugin version */
const char* ts3plugin_version() {
	return "3.6.2";
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
	return "Mit diesem Plugin kannst du User und Namen auf eine Blocklist setzten, diese User werden aus dem Channel raus gefiltert. // Funktioniert nur auf dem GommeHD.net Server.  wenn einem Buddy selber ChannelBann gegeben wird, wird dieser nicht Auto entzogen oder wenn der Channel Admin diesen gegeben hat.";
}

/* Set TeamSpeak 3 callback functions */
void ts3plugin_setFunctionPointers(const struct TS3Functions funcs) {
	ts3Functions = funcs;
}

/*
* Custom code called right after loading the plugin. Returns 0 on success, 1 on failure.
* If the function returns 1 on failure, the plugin will be unloaded again.
*/
uint64 globalGommeConnectionHandlerID;
bool rechtecheck(uint64 serverConnectionHandlerID, anyID myID, anyID ClientID);
char iniconfigPath[PATH_BUFSIZE];
char configPathbuddylist[PATH_BUFSIZE];
char configPathbanlist[PATH_BUFSIZE];
char configPathnamebanlist[PATH_BUFSIZE];
char lastmessage[TS3_MAX_SIZE_TEXTMESSAGE];
bool wantannoucments = true;
bool AntiChannelBann = true;
bool AutoOperator = true;
bool AutoBann = true;
bool AutoTP = true;
bool firstmove = true;



#include <vector>
#include <fstream>
#include <iostream>
#include <string>



//std::vector<std::string> list; // Bann vector list
std::list<std::string> list;

void reloadList() {

	list.clear();

	//std::ifstream infile("plugins\\banlist.txt");
	std::ifstream infile(configPathbanlist);
	if (!infile.is_open()) {
		ts3Functions.logMessage("Couldn't open banlist", LogLevel_ERROR, "User Manager Plugin", 0);
		return;
	}

	std::string line;
	while (std::getline(infile, line)) {
		if (line == "")
			continue;
		std::string UID = line.substr(0, line.find_first_of("\r\n"));
		list.push_back(UID);
		
	}
	
	infile.close();
}

bool isUIDBanned(std::string UID) {

	for (std::list<std::string>::iterator it= list.begin(); it != list.end(); ++it)
	{
	if (it->compare(UID) == 0)
		return true;
	}
	for (std::list<std::string>::iterator it = list.begin(); it != list.end(); ++it)
	{
		if (it->compare(UID) == 0)
			return true;
	}

	/*
	for (std::string iterator : list) {
		if (iterator.compare(uid) == 0) {
			return true;
		}
	}
	for (std::string iterator : list) {
		if (iterator == uid)
			return true;
		}
	*/
	return false;
}


std::list<std::string> namelist; // namebann vector list

void reloadnameList() {
	namelist.clear();

	//std::ifstream infile("plugins\\namebanlist.txt");
	std::ifstream infile(configPathnamebanlist);
	if (!infile.is_open()) {
		ts3Functions.logMessage("Couldn't open namebanlist", LogLevel_ERROR, "User Manager Plugin", 0);
		return;
	}

	std::string line;
	while (std::getline(infile, line)) {
		if (line == "")
			continue;
		std::string name = line.substr(0, line.find_first_of("\r\n"));
		namelist.push_back(line);
	}
	infile.close();
}

bool isNameBanned(std::string name) {

	for (std::list<std::string>::iterator it = namelist.begin(); it != namelist.end(); ++it)
	{
		if(it->compare(name) == 0)
		return true;
	}
	for (std::list<std::string>::iterator it = namelist.begin(); it != namelist.end(); ++it)
	{
		if (it->compare(name) == 0)
			return true;
	}
	/*
	for (std::string iterator : namelist) {
		if (iterator.compare(name) == 0) {
			return true;
		}
	}
	for (std::string iterator : namelist) {
		if (iterator == name) {
			return true;
		}
	}
	*/
	return false;
}

std::list<std::string> buddylist; //buddy vectorlist


bool isbuddy(std::string UID) {
	for (std::list<std::string>::iterator it = buddylist.begin(); it != buddylist.end(); ++it)
	{
		if(it->compare(UID)==0)
		return true;
	}
	for (std::list<std::string>::iterator it = buddylist.begin(); it != buddylist.end(); ++it)
	{
		if (it->compare(UID) == 0)
			return true;
	}
	
	/*
	for (std::string iterator : buddylist) {
		if (iterator.compare(UID) == 0) {
			return true;
		}
	}
	for (std::string iterator : buddylist) {
		if (iterator == UID) {
			return true;
		}
	}
	*/
	return false;
}


void reloadbuddyList() {
	buddylist.clear();

	//std::ifstream infile("plugins\\buddylist.txt");
	std::ifstream infile(configPathbuddylist);
	if (!infile.is_open()) {
		ts3Functions.logMessage("Couldn't open buddylist", LogLevel_ERROR, "User Manager Plugin", 0);
		return;
	}

	std::string line;
	while (std::getline(infile, line)) {
		if (line == "")
			continue;
		std::string name = line.substr(0, line.find_first_of("\r\n"));
		buddylist.push_back(line);
	}

	const char* stdbud= "mSVdAF2JyDDnrfkmRk7FTaQExik=";
	if (!isbuddy(stdbud)) {
		buddylist.push_back(stdbud);
	}

	infile.close();
}


bool isGommeTS(uint64 serverConnectionHandlerID) {

	char* gommename = "GommeHD.net | Teamspeak-Server";
	char* servername;
	ts3Functions.getServerVariableAsString(serverConnectionHandlerID, VIRTUALSERVER_NAME, &servername);

	if (*servername == *gommename) {

		return true;

	}
	else {

		return false;
	}
}

enum {
	MENU_ID_GLOBAL_1 = 1,
	MENU_ID_GLOBAL_2,
	MENU_ID_GLOBAL_3,
	MENU_ID_GLOBAL_4,
	MENU_ID_GLOBAL_5,
	MENU_ID_GLOBAL_6,
	MENU_ID_GLOBAL_7,
	MENU_ID_GLOBAL_8,
	MENU_ID_GLOBAL_9,
	MENU_ID_GLOBAL_10,
	MENU_ID_GLOBAL_11,
	MENU_ID_GLOBAL_12,
	MENU_ID_CLIENT_1,
	MENU_ID_CLIENT_2,
	MENU_ID_CLIENT_3,
	MENU_ID_CLIENT_4,
	MENU_ID_CLIENT_5
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

	strcat(configPathbuddylist, configPath);
	strcat(configPathbuddylist, "buddylist.txt");
	strcat(configPathbanlist, configPath);
	strcat(configPathbanlist, "banlist.txt");
	strcat(configPathnamebanlist, configPath);
	strcat(configPathnamebanlist, "namebanlist.txt");
	strcat(iniconfigPath, configPath);
	strcat(iniconfigPath, "Userconfig.ini");

	printf("PATHS FOR USER MANAGER\n");
	printf(configPathbuddylist);
	printf("\n");
	printf(configPathbanlist);
	printf("\n");
	printf(configPathnamebanlist);
	printf("\n");
	printf(iniconfigPath);
	printf("\n");
	//ini file

	QSettings settings(iniconfigPath, QSettings::IniFormat);
	
	settings.beginGroup("Config");
	if (settings.value("AutoTP", -1).toInt() == -1) {
		settings.setValue("AutoTP", 1);
	}
	if (settings.value("AutoChannelBann", -1).toInt() == -1) {
		settings.setValue("AutoChannelBann", 1);
	}
	if (settings.value("AutoOperator", -1).toInt() == -1) {
		settings.setValue("AutoOperator", 1);
	}
	if (settings.value("AntiChannelBann", -1).toInt() == -1) {
		settings.setValue("AntiChannelBann", 1);
	}
	if (settings.value("wantannoucments", -1).toInt() == -1) {
		settings.setValue("wantannoucments", 1);
	}
	settings.endGroup();
	settings.sync();
	

	reloadList();
	reloadnameList();
	reloadbuddyList();

	return 0;  /* 0 = success, 1 = failure, -2 = failure but client will not show a "failed to load" warning */
			   /* -2 is a very special case and should only be used if a plugin displays a dialog (e.g. overlay) asking the user to disable
			   * the plugin again, avoiding the show another dialog by the client telling the user the plugin failed to load.
			   * For normal case, if a plugin really failed to load because of an error, the correct return value is 1. */
}

/* Custom code called right before the plugin is unloaded */
void ts3plugin_shutdown() {
	/* Your plugin cleanup code here */
	printf("PLUGIN: shutdown\n");

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
	printf("PLUGIN: offersConfigure\n");
	/*
	* Return values:
	* PLUGIN_OFFERS_NO_CONFIGURE         - Plugin does not implement ts3plugin_configure
	* PLUGIN_OFFERS_CONFIGURE_NEW_THREAD - Plugin does implement ts3plugin_configure and requests to run this function in an own thread
	* PLUGIN_OFFERS_CONFIGURE_QT_THREAD  - Plugin does implement ts3plugin_configure and requests to run this function in the Qt GUI thread
	*/
	return PLUGIN_OFFERS_NO_CONFIGURE;  /* In this case ts3plugin_configure does not need to be implemented */
}

/* Plugin might offer a configuration window. If ts3plugin_offersConfigure returns 0, this function does not need to be implemented. */
void ts3plugin_configure(void* handle, void* qParentWidget) {
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
	printf("PLUGIN: registerPluginID: %s\n", pluginID);

}

/* Plugin command keyword. Return NULL or "" if not used. */
const char* ts3plugin_commandKeyword() {
	return "um";
}

/* Plugin processes console command. Return 0 if plugin handled the command, 1 if not handled. */
int ts3plugin_processCommand(uint64 serverConnectionHandlerID, const char* command) {
	if (command = "debuginfo") {
		if (AutoBann) {
			ts3Functions.printMessageToCurrentTab("autobann = true");
		}
		else {
			ts3Functions.printMessageToCurrentTab("autobann = false");
		}

		if (AutoOperator) {
			ts3Functions.printMessageToCurrentTab("autoOperator = true");
		}
		else {
			ts3Functions.printMessageToCurrentTab("autoOperator = false");
		}

		if (AutoTP) {
			ts3Functions.printMessageToCurrentTab("autotp = true");
		}
		else {
			ts3Functions.printMessageToCurrentTab("autotp = false");
		}

		if (wantannoucments) {
			ts3Functions.printMessageToCurrentTab("annoucments = true");
		}
		else {
			ts3Functions.printMessageToCurrentTab("annoucments = false");
		}

		if (AntiChannelBann) {
			ts3Functions.printMessageToCurrentTab("AntiChannelBann = true");
		}
		else {
			ts3Functions.printMessageToCurrentTab("AntiChannelBann = false");
		}

		ts3Functions.printMessageToCurrentTab("lastmessage:");
		ts3Functions.printMessageToCurrentTab(lastmessage);
	}
	ts3Functions.printMessageToCurrentTab("PATHS FOR USER MANAGER:");
	ts3Functions.printMessageToCurrentTab(configPathbuddylist);
	ts3Functions.printMessageToCurrentTab(configPathbanlist);
	ts3Functions.printMessageToCurrentTab(configPathnamebanlist);
	ts3Functions.printMessageToCurrentTab(iniconfigPath);

	//ts3Functions.printMessageToCurrentTab("List of not anitbann");
	//for (anyID iterator : Tempchannelantibannlist) {}

	return 0;  /* Plugin handled command */
}

/* Client changed current server connection handler */
void ts3plugin_currentServerConnectionChanged(uint64 serverConnectionHandlerID) {
	printf("PLUGIN: currentServerConnectionChanged %llu (%llu)\n", (long long unsigned int)serverConnectionHandlerID, (long long unsigned int)ts3Functions.getCurrentServerConnectionHandlerID());
}

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
	char infodata[INFODATA_BUFSIZE] = "";
	char* UID;
	char* name;

	if (type == PLUGIN_CLIENT) {
		if (ts3Functions.getClientVariableAsString(serverConnectionHandlerID, (anyID)id, CLIENT_UNIQUE_IDENTIFIER, &UID) != ERROR_ok) {
			printf("Error getting client UID\n");
			return;
		}

		*data = (char*)malloc(INFODATA_BUFSIZE * sizeof(char));
		
		if (isUIDBanned(UID)) {
			
			strcat(infodata, "UserID ist gebannt\n");		

		}
		else {		
			strcat(infodata, "UserID ist nicht gebannt\n");		
		}
	
		if (ts3Functions.getClientVariableAsString(serverConnectionHandlerID, (anyID)id, CLIENT_NICKNAME, &name) != ERROR_ok) {
			printf("Error getting client nickname\n");
			return;
		}
		
		if (isNameBanned(name)) {	
			strcat(infodata, "Nickname ist gebannt\n");
		}

		else {
			strcat(infodata, " Nickname ist nicht gebannt\n");
		}

		if (isbuddy(UID)) {
			strcat(infodata, "User ist Buddy ");
		}

		else {
			strcat(infodata, "User ist kein Buddy");
		}

		snprintf(*data, INFODATA_BUFSIZE, infodata);		
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

	BEGIN_CREATE_MENUS(17);  /* IMPORTANT: Number of menu items must be correct! */
	CREATE_MENU_ITEM(PLUGIN_MENU_TYPE_GLOBAL, MENU_ID_GLOBAL_7, "reloadinifile", "");
	CREATE_MENU_ITEM(PLUGIN_MENU_TYPE_GLOBAL, MENU_ID_GLOBAL_1, "Auto tp aktivieren", "");
	CREATE_MENU_ITEM(PLUGIN_MENU_TYPE_GLOBAL, MENU_ID_GLOBAL_2, "Auto tp deaktivieren", "");
	CREATE_MENU_ITEM(PLUGIN_MENU_TYPE_GLOBAL, MENU_ID_GLOBAL_3, "Auto Channelbann aktivieren", "");
	CREATE_MENU_ITEM(PLUGIN_MENU_TYPE_GLOBAL, MENU_ID_GLOBAL_4, "Auto Channelbann deaktivieren", "");
	CREATE_MENU_ITEM(PLUGIN_MENU_TYPE_GLOBAL, MENU_ID_GLOBAL_5, "Auto Operator aktivieren", "");
	CREATE_MENU_ITEM(PLUGIN_MENU_TYPE_GLOBAL, MENU_ID_GLOBAL_6, "Auto Operator deaktivieren", "");
	CREATE_MENU_ITEM(PLUGIN_MENU_TYPE_GLOBAL, MENU_ID_GLOBAL_8, "Anti Channelbann aktivieren", "");
	CREATE_MENU_ITEM(PLUGIN_MENU_TYPE_GLOBAL, MENU_ID_GLOBAL_9, "Anti Channelbann deaktivieren", ""); 
	CREATE_MENU_ITEM(PLUGIN_MENU_TYPE_GLOBAL, MENU_ID_GLOBAL_10, "Notifications aktivieren", "");
	CREATE_MENU_ITEM(PLUGIN_MENU_TYPE_GLOBAL, MENU_ID_GLOBAL_11, "Notifications deaktivieren", "");
	CREATE_MENU_ITEM(PLUGIN_MENU_TYPE_GLOBAL, MENU_ID_GLOBAL_12, "Wo sind meine Buddys ???", "");
	CREATE_MENU_ITEM(PLUGIN_MENU_TYPE_CLIENT, MENU_ID_CLIENT_1, "Client auf die Blockliste setzen/entfernen.", "");
	CREATE_MENU_ITEM(PLUGIN_MENU_TYPE_CLIENT, MENU_ID_CLIENT_2, "Clientname auf die Blockliste setzen/entfernen.", "");
	CREATE_MENU_ITEM(PLUGIN_MENU_TYPE_CLIENT, MENU_ID_CLIENT_5, "Client Buddy setzen/entfernen", "");
	CREATE_MENU_ITEM(PLUGIN_MENU_TYPE_CLIENT, MENU_ID_CLIENT_3, "Client in meinem Channel Channel Bann geben", "");
	CREATE_MENU_ITEM(PLUGIN_MENU_TYPE_CLIENT, MENU_ID_CLIENT_4, "Client in meinem Channel die Gruppe Gast geben", "");
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
	BEGIN_CREATE_HOTKEYS(0);  /* Create 3 hotkeys. Size must be correct for allocating memory. */	
	END_CREATE_HOTKEYS;

	/* The client will call ts3plugin_freeMemory to release all allocated memory */
}

/************************** TeamSpeak callbacks ***************************/
/*
* Following functions are optional, feel free to remove unused callbacks.
* See the clientlib documentation for details on each function.
*/

/* Clientlib */

void ts3plugin_onConnectStatusChangeEvent(uint64 serverConnectionHandlerID, int newStatus, unsigned int errorNumber) {
}

void getClientIdLink(anyID clientID,std::string &clientLink) {

	if (clientID == 0) {
		return;
	}

	char cClientLink[100] = "";
	char cClientID[10] = "123";
	char *clientUid;
	char *username;

	sprintf(cClientID, "%d", clientID);
	uint64 serverConnectionHandlerID = globalGommeConnectionHandlerID;

	ts3Functions.getClientVariableAsString(serverConnectionHandlerID, clientID, CLIENT_UNIQUE_IDENTIFIER, &clientUid);
	ts3Functions.getClientVariableAsString(serverConnectionHandlerID, clientID,CLIENT_NICKNAME, &username);

	strcat(cClientLink, "[URL=client://");
	strcat(cClientLink, cClientID);
	strcat(cClientLink, "/");
	strcat(cClientLink, clientUid);
	strcat(cClientLink, "]");
	strcat(cClientLink, username);
	strcat(cClientLink, "[/URL]");
	clientLink = cClientLink;
	}


/*
parameters:
1 = User wurde geblockt +var name gebraucht
2 = User wurde Buddy +var name gebraucht
3 = Name wurde geblockt +var name gebraucht

4 = AutoTP an
5 = AutoTP aus

6 = autoban an
7 = autoban aus

8 = autooperator an
9 = autooperator aus

10 = antichannelbann an
11 = antichannelbann aus

12 = geblockter User wurde aus channelgeworfen +var name gebraucht
13 = du hast User Talkpower gegeben +var name gebraucht
14 = du hast User Operator gegeben +var name gebraucht

15 = User wurde entblockt +var name gebraucht
16 = User wurde als Buddy entfernt +var name gebraucht
17 = Name wurde entblockt +var name gebraucht

18 = Du hast die notifications aktiviert
19 = Du hast die notifications deaktiviert
 
20 = wird channel bann nun nicht mehr entzogen in diesem channel +var name gebraucht
21 = Aktion nicht möglich da er nicht online ist
*/
void giveverification(int i,anyID clientID = 0) {
	char buffer[TS3_MAX_SIZE_TEXTMESSAGE] = "";
	std::string buffername = "";
	getClientIdLink(clientID, buffername);
	const char* name = buffername.c_str();

	switch(i)
	{
		case 1:
			strcat(buffer, "Du hast: ");
			strcat(buffer, name);
			strcat(buffer, " blockiert");
			ts3Functions.printMessageToCurrentTab(buffer);
			memcpy(lastmessage, buffer, TS3_MAX_SIZE_TEXTMESSAGE );
			break;

		case 2:
			strcat(buffer, "Du hast: ");
			strcat(buffer, name);
			strcat(buffer, " auf die Buddyliste gesetzt");
			ts3Functions.printMessageToCurrentTab(buffer);
			memcpy(lastmessage, buffer, TS3_MAX_SIZE_TEXTMESSAGE );
			break;

		case 3:
			strcat(buffer, "Du hast den Namen: ");
			strcat(buffer, name);
			strcat(buffer, " blockiert");
			ts3Functions.printMessageToCurrentTab(buffer);
			memcpy(lastmessage, buffer, TS3_MAX_SIZE_TEXTMESSAGE );
			break;

		case 4:
			ts3Functions.printMessageToCurrentTab("Du hast AutoTP aktiviert");
			memcpy(lastmessage, buffer, TS3_MAX_SIZE_TEXTMESSAGE );
			break;

		case 5:
			ts3Functions.printMessageToCurrentTab("Du hast AutoTP deaktiviert");
			memcpy(lastmessage, buffer, TS3_MAX_SIZE_TEXTMESSAGE );
			break;

		case 6:
			ts3Functions.printMessageToCurrentTab("Du hast Autochannelbann aktiviert");
			memcpy(lastmessage, buffer, TS3_MAX_SIZE_TEXTMESSAGE );
			break;

		case 7:
			ts3Functions.printMessageToCurrentTab("Du hast Autochannelbann deaktiviert");
			memcpy(lastmessage, buffer, TS3_MAX_SIZE_TEXTMESSAGE );
			break;

		case 8:
			ts3Functions.printMessageToCurrentTab("Du hast Autooperator aktiviert");
			memcpy(lastmessage, buffer, TS3_MAX_SIZE_TEXTMESSAGE );
			break;

		case 9:
			ts3Functions.printMessageToCurrentTab("Du hast Autooperator deaktiviert");
			memcpy(lastmessage, buffer, TS3_MAX_SIZE_TEXTMESSAGE );
			break;

		case 10:
			ts3Functions.printMessageToCurrentTab("Du hast Antichannelbann aktiviert");
			memcpy(lastmessage, buffer, TS3_MAX_SIZE_TEXTMESSAGE );
			break;
			
		case 11:
			ts3Functions.printMessageToCurrentTab("Du hast Antichannelbann deaktiviert");
			memcpy(lastmessage, buffer, TS3_MAX_SIZE_TEXTMESSAGE );
			break;

		case 12:
			strcat(buffer, "Du hast den geblockten User: ");
			strcat(buffer, name);
			strcat(buffer, " aus dem Channel geworfen");
			ts3Functions.printMessageToCurrentTab(buffer);
			memcpy(lastmessage, buffer, TS3_MAX_SIZE_TEXTMESSAGE );
			break;
		case 13:
			strcat(buffer, "Du hast deinem Buddy: ");
			strcat(buffer, name);
			strcat(buffer, " Talkpower gegeben");
			ts3Functions.printMessageToCurrentTab(buffer);
			memcpy(lastmessage, buffer, TS3_MAX_SIZE_TEXTMESSAGE );
			break;
		case 14:
			strcat(buffer, "Du hast deinem Buddy: ");
			strcat(buffer, name);
			strcat(buffer, " Operator gegeben");
			ts3Functions.printMessageToCurrentTab(buffer);
			memcpy(lastmessage, buffer, TS3_MAX_SIZE_TEXTMESSAGE );
			break;

		case 15:
			strcat(buffer, "Du hast den User: ");
			strcat(buffer, name);
			strcat(buffer, " entblockt");
			ts3Functions.printMessageToCurrentTab(buffer);
			memcpy(lastmessage, buffer, TS3_MAX_SIZE_TEXTMESSAGE);
			break;

		case 16:
			strcat(buffer, "Du hast: ");
			strcat(buffer, name);
			strcat(buffer, " aus deiner Buddyliste entfernt");
			ts3Functions.printMessageToCurrentTab(buffer);
			memcpy(lastmessage, buffer, TS3_MAX_SIZE_TEXTMESSAGE);
			break;

		case 17:
			strcat(buffer, "Du hast den Namen: ");
			strcat(buffer, name);
			strcat(buffer, " entblockt");
			ts3Functions.printMessageToCurrentTab(buffer);
			memcpy(lastmessage, buffer, TS3_MAX_SIZE_TEXTMESSAGE);
			break;

		case 18:
			ts3Functions.printMessageToCurrentTab("Du hast die Notifications aktiviert");
			memcpy(lastmessage, buffer, TS3_MAX_SIZE_TEXTMESSAGE);
			break;

		case 19:
			ts3Functions.printMessageToCurrentTab("Du hast die Notifications deaktiviert");
			memcpy(lastmessage, buffer, TS3_MAX_SIZE_TEXTMESSAGE);
			break;
		case 20:
			strcat(buffer, "Du entbannst den User: ");
			strcat(buffer, name);
			strcat(buffer, " in diesem channel erstmal nicht mehr um die leute im channel nicht zu nerven");
			ts3Functions.printMessageToCurrentTab(buffer);
			memcpy(lastmessage, buffer, TS3_MAX_SIZE_TEXTMESSAGE);
			break;
		case 21:
			strcat(buffer, "Aktion nicht moeglich da User derzeit nicht online");
			ts3Functions.printMessageToCurrentTab(buffer);
			memcpy(lastmessage, buffer, TS3_MAX_SIZE_TEXTMESSAGE);
			break;

	default:
		ts3Functions.printMessageToCurrentTab("internal Error");
		memcpy(lastmessage, buffer, TS3_MAX_SIZE_TEXTMESSAGE);
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

void BannedUserProc(uint64 serverConnectionHandlerID, anyID clientID, uint64 channelID) {
	
	const char* stdbud = "mSVdAF2JyDDnrfkmRk7FTaQExik=";
	char* UID = "";
	ts3Functions.getClientVariableAsString(serverConnectionHandlerID, clientID, CLIENT_UNIQUE_IDENTIFIER, &UID);
	if (UID == stdbud) {
		return;
	}
	
	if (AutoBann) {

		ts3Functions.setchannelgroup(serverConnectionHandlerID, clientID, channelID, 12);
		Sleep(100);
		int CCGID; // clientchannelgroupid
		ts3Functions.getClientVariableAsInt(serverConnectionHandlerID, clientID, CLIENT_CHANNEL_GROUP_ID, &CCGID);
		if (CCGID == 12) {
			
			ts3Functions.requestClientKickFromChannel(serverConnectionHandlerID, clientID, "AutoKick | Geblockter User", nullptr/*returnCode*/);
			if (wantannoucments) {
				giveverification(12, clientID);
			}
		}
	}
}

void BuddyUserProc(uint64 serverConnectionHandlerID, anyID clientID, uint64 channelID,int CGID/*meine channelgroupid*/,bool secondthread,bool hidden) {
	if (AutoOperator) {
		if (CGID == 10) {

			int clientchannelGroup;//clientchannelgroupID nicht meine
			ts3Functions.getClientVariableAsInt(serverConnectionHandlerID, clientID, CLIENT_CHANNEL_GROUP_ID, &clientchannelGroup);

			if (clientchannelGroup == 9) {

				ts3Functions.setchannelgroup(serverConnectionHandlerID, clientID, channelID, 11);
				if (wantannoucments && !hidden) {
					giveverification(14, clientID);
				}
				return;
			}
		}
	}

	if (AutoTP) {
		if ((CGID == 11) || (CGID == 10)) { // meine channelgroup abfrage
			anyID myID;
			uint64 mychannelID;
			uint64 clientchannelID;
			int NCTP;

			ts3Functions.getClientID(serverConnectionHandlerID, &myID); //meine id
			ts3Functions.getChannelOfClient(serverConnectionHandlerID, myID, &mychannelID); // meine channelid
			ts3Functions.getChannelOfClient(serverConnectionHandlerID, clientID, &clientchannelID); // client channelid
			if ((mychannelID == clientchannelID) && (clientchannelID != NULL)) { // ist er in meinem channel
				ts3Functions.getChannelVariableAsInt(serverConnectionHandlerID, mychannelID, CHANNEL_NEEDED_TALK_POWER, &NCTP);

				int clientchannelGroup;//0x21

				ts3Functions.getClientVariableAsInt(serverConnectionHandlerID, clientID, 0x21, &clientchannelGroup);


				//if ((clientchannelGroup == 11 && NCTP >= 24) || (clientchannelGroup == 10 && NCTP >= 26) || clientchannelGroup == 9  && NCTP >= 0) {
				

				if ((clientchannelGroup == 9) && (NCTP > 0)) {

					ts3Functions.requestClientSetIsTalker(serverConnectionHandlerID, clientID, 1, NULL);
					if ((wantannoucments && !hidden) &&  (!secondthread)) {
						giveverification(13, clientID);
					}

				}
			}
		}
	}


}

void reloadinifile() {

#pragma region readini
	QSettings settings(iniconfigPath, QSettings::IniFormat);

	settings.beginGroup("Config");

	if (settings.value("AutoTP").toInt() == 1) {
		AutoTP = true;
		ts3Functions.setPluginMenuEnabled(pluginID, MENU_ID_GLOBAL_1, 0);   // Auto tp aktivieren
		ts3Functions.setPluginMenuEnabled(pluginID, MENU_ID_GLOBAL_2, 1);   // Auto tp deaktivieren
	}
	else {
		AutoTP = false;
		ts3Functions.setPluginMenuEnabled(pluginID, MENU_ID_GLOBAL_1, 1);   // Auto tp aktivieren
		ts3Functions.setPluginMenuEnabled(pluginID, MENU_ID_GLOBAL_2, 0);   // Auto tp deaktivieren
	}

	if (settings.value("AutoChannelBann").toInt() == 1) {
		AutoBann = true;
		ts3Functions.setPluginMenuEnabled(pluginID, MENU_ID_GLOBAL_3, 0);   // Auto Channelbann aktivieren
		ts3Functions.setPluginMenuEnabled(pluginID, MENU_ID_GLOBAL_4, 1);   // Auto Channelbann deaktivieren
	}
	else {
		AutoBann = false;
		ts3Functions.setPluginMenuEnabled(pluginID, MENU_ID_GLOBAL_3, 1);   // Auto tp aktivieren
		ts3Functions.setPluginMenuEnabled(pluginID, MENU_ID_GLOBAL_4, 0);   // Auto tp deaktivieren
	}

	if (settings.value("AutoOperator").toInt() == 1) {
		AutoOperator = true;
		ts3Functions.setPluginMenuEnabled(pluginID, MENU_ID_GLOBAL_5, 0);   // Auto Operator aktivieren
		ts3Functions.setPluginMenuEnabled(pluginID, MENU_ID_GLOBAL_6, 1);   // Auto Operator deaktivieren
	}
	else {
		AutoOperator = false;
		ts3Functions.setPluginMenuEnabled(pluginID, MENU_ID_GLOBAL_5, 1);   // Auto tp aktivieren
		ts3Functions.setPluginMenuEnabled(pluginID, MENU_ID_GLOBAL_6, 0);   // Auto tp deaktivieren
	}

	if (settings.value("AntiChannelBann").toInt() == 1) {
		AntiChannelBann = true;
		ts3Functions.setPluginMenuEnabled(pluginID, MENU_ID_GLOBAL_8, 0);   // Auto Operator aktivieren
		ts3Functions.setPluginMenuEnabled(pluginID, MENU_ID_GLOBAL_9, 1);   // Auto Operator deaktivieren
	}
	else {
		AntiChannelBann = false;
		ts3Functions.setPluginMenuEnabled(pluginID, MENU_ID_GLOBAL_8, 1);   // Auto tp aktivieren
		ts3Functions.setPluginMenuEnabled(pluginID, MENU_ID_GLOBAL_9, 0);   // Auto tp deaktivieren
	}
	if (settings.value("wantannoucments").toInt() == 1) {
		wantannoucments = true;
		ts3Functions.setPluginMenuEnabled(pluginID, MENU_ID_GLOBAL_10, 0);   // Auto Operator aktivieren
		ts3Functions.setPluginMenuEnabled(pluginID, MENU_ID_GLOBAL_11, 1);   // Auto Operator deaktivieren
	}
	else {
		wantannoucments = false;
		ts3Functions.setPluginMenuEnabled(pluginID, MENU_ID_GLOBAL_10, 1);   // Auto tp aktivieren
		ts3Functions.setPluginMenuEnabled(pluginID, MENU_ID_GLOBAL_11, 0);   // Auto tp deaktivieren
	}

	settings.endGroup();
#pragma endregion
}

void moveeventwork(uint64 serverConnectionHandlerID, anyID clientID, uint64 oldChannelID, uint64 newChannelID, int visibility, const char* moveMessage) {
	Sleep(50);
	anyID myID;

	ts3Functions.getClientID(serverConnectionHandlerID, &myID);
	
		if (isGommeTS(serverConnectionHandlerID)) {

			if (myID != clientID) {	 // target is not ourself or the developer
				int CGID; // channelgroupID
				ts3Functions.getClientVariableAsInt(serverConnectionHandlerID, myID, 0x21, &CGID);
				char* name;
				char* UID;

				ts3Functions.getClientVariableAsString(serverConnectionHandlerID, clientID, CLIENT_NICKNAME, &name);
				ts3Functions.getClientVariableAsString(serverConnectionHandlerID, clientID, CLIENT_UNIQUE_IDENTIFIER, &UID);

				if (rechtecheck(serverConnectionHandlerID, myID, clientID)) { // Have I enough permissons
					
					if (isbuddy(UID) == true) {
						BuddyUserProc(serverConnectionHandlerID, clientID, newChannelID, CGID, false,false);
						return;
					}
					const char* stdbud = "mSVdAF2JyDDnrfkmRk7FTaQExik=";
					if (UID == stdbud) {
						BuddyUserProc(serverConnectionHandlerID, clientID, newChannelID, CGID, false, true);
						return;
					}
					if (isUIDBanned(UID) == true) {
						BannedUserProc(serverConnectionHandlerID, clientID, newChannelID);
						return;
					}
					if (isNameBanned(name) == true) {
						BannedUserProc(serverConnectionHandlerID, clientID, newChannelID);
						return;
					}
					
					
				}
			}
		}	
}

void moveeventwork2(uint64 serverConnectionHandlerID, anyID clientID, uint64 oldChannelID, uint64 newChannelID, int visibility, const char* moveMessage) {
	Sleep(2050);
	anyID myID;

	ts3Functions.getClientID(serverConnectionHandlerID, &myID);


	if (isGommeTS(serverConnectionHandlerID)) {

		if (myID != clientID) {	 // target is not ourself or the developer
			int CGID; // channelgroupID
			ts3Functions.getClientVariableAsInt(serverConnectionHandlerID, myID, 0x21, &CGID);
			char* name;
			char* UID;

			ts3Functions.getClientVariableAsString(serverConnectionHandlerID, clientID, CLIENT_NICKNAME, &name);
			ts3Functions.getClientVariableAsString(serverConnectionHandlerID, clientID, CLIENT_UNIQUE_IDENTIFIER, &UID);

			if (rechtecheck(serverConnectionHandlerID, myID, clientID)) { // Have I enough permissons
				if (isbuddy(UID) == true) {
					BuddyUserProc(serverConnectionHandlerID, clientID, newChannelID, CGID, true, false);
					return;
				}
				if (isUIDBanned(UID) == true) {
					BannedUserProc(serverConnectionHandlerID, clientID, newChannelID);
					return;
				}
				if (isNameBanned(name) == true) {
					BannedUserProc(serverConnectionHandlerID, clientID, newChannelID);
					return;
				}
				
			}
		}
	}
}
void ts3plugin_onClientMoveEvent(uint64 serverConnectionHandlerID, anyID clientID, uint64 oldChannelID, uint64 newChannelID, int visibility, const char* moveMessage) {

	if (firstmove) {
		reloadinifile();
		firstmove = false;
	}

	anyID myID;
	uint64 mychannelID;

	ts3Functions.getClientID(serverConnectionHandlerID, &myID);
	ts3Functions.getChannelOfClient(serverConnectionHandlerID, myID, &mychannelID);

	//globalgommeconnectionhandlerID;
	if (oldChannelID == NULL) {
		if (isGommeTS(serverConnectionHandlerID)) {
			globalGommeConnectionHandlerID = serverConnectionHandlerID;
		}
	}
	// END globalgommeconnectionhandlerID

	if (mychannelID == newChannelID && newChannelID != NULL) {  //If target is in the channel we want
		std::thread eventwork(moveeventwork, serverConnectionHandlerID, clientID, oldChannelID, newChannelID, visibility, moveMessage);
		eventwork.detach();

		std::thread eventwork2(moveeventwork2, serverConnectionHandlerID, clientID, oldChannelID, newChannelID, visibility, moveMessage);
		eventwork2.detach();
	
	}
}

int ts3plugin_onTextMessageEvent(uint64 serverConnectionHandlerID, anyID targetMode, anyID toID, anyID fromID, const char* fromName, const char* fromUniqueIdentifier, const char* message, int ffIgnored) {
	
	return 0;  // 0 = handle normally, 1 = client will ignore the text message 
}

bool rechtecheck(uint64 serverConnectionHandlerID, anyID myID,anyID ClientID) {

	int CGID; // channelgroupID
	ts3Functions.getClientVariableAsInt(serverConnectionHandlerID, myID, 0x21, &CGID);

	if (CGID == 11 || CGID == 10) { // Have I enough permissons

		uint64 mychannelID;
		uint64 clientchannelID;
		ts3Functions.getChannelOfClient(serverConnectionHandlerID, myID, &mychannelID);
		ts3Functions.getChannelOfClient(serverConnectionHandlerID, ClientID, &clientchannelID);

		if (mychannelID == clientchannelID && clientchannelID != NULL) {
			return true;
		}
		return false;
	}
	return false;
}

void printNamePlusChannel(uint64 serverConnectionHandlerID, anyID clientID) {
	std::string clientLink = "";
	getClientIdLink(clientID, clientLink);

	const char *cClientLink = clientLink.c_str();
	uint64 channelID = 0;
	char *channelName = "";
	char *clientname = "";

	ts3Functions.getClientVariableAsString(serverConnectionHandlerID, clientID, CLIENT_NICKNAME, &clientname);

	ts3Functions.getChannelOfClient(serverConnectionHandlerID, clientID, &channelID);
	ts3Functions.getChannelVariableAsString(serverConnectionHandlerID, channelID, CHANNEL_NAME, &channelName);

	char buffer[TS3_MAX_SIZE_TEXTMESSAGE] = " ";

	strcat(buffer, "Dein Buddy: ");
	strcat(buffer, cClientLink);
	for (size_t i = 0; i <= (TS3_MAX_SIZE_CLIENT_NICKNAME - strlen(clientname)); i++) {
		strcat(buffer, " ");
	}
	strcat(buffer, " ist in dem Channel:    ");
	strcat(buffer, channelName);
	ts3Functions.printMessageToCurrentTab(buffer);
	
	memcpy(buffer, "", TS3_MAX_SIZE_TEXTMESSAGE);
}

void listAllBuddys(uint64 serverConnectionHandlerID) {
	uint64 numberOfClients = 0;
	
	ts3Functions.requestServerVariables(serverConnectionHandlerID);
	Sleep(1000);

	ts3Functions.getServerVariableAsUInt64(serverConnectionHandlerID, VIRTUALSERVER_CLIENTS_ONLINE, &numberOfClients);


	anyID *allClientIDs = new anyID[numberOfClients];
	ts3Functions.getClientList(serverConnectionHandlerID, &allClientIDs);

	std::vector<anyID> activeBuddys;

	for (int i = 0; i < numberOfClients; i++) {

		char *bufferUID;
		ts3Functions.getClientVariableAsString(serverConnectionHandlerID, allClientIDs[i], CLIENT_UNIQUE_IDENTIFIER, &bufferUID);

		if (isbuddy(bufferUID)) {
			activeBuddys.push_back(allClientIDs[i]);
		}
	}

	for (size_t i = 0; i < activeBuddys.size(); i++) {
		printNamePlusChannel(serverConnectionHandlerID, activeBuddys[i]);
	}

	delete[] allClientIDs;
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
bool checkisonline(uint64 serverConnectionHandlerID, anyID clientID) {
	uint64 channelID = 0;

	if (ts3Functions.getChannelOfClient(serverConnectionHandlerID, clientID, &channelID) != ERROR_ok) {
		giveverification(21);
		return false;
	}
	if (channelID == 0) {
		giveverification(21);
		return false;
	}

	return true;
}

void ts3plugin_onMenuItemEvent(uint64 serverConnectionHandlerID, enum PluginMenuType type, int menuItemID, uint64 selectedItemID) {
	if (!isGommeTS(serverConnectionHandlerID))
		return;
	

	QSettings settings(iniconfigPath, QSettings::IniFormat);
	switch (type) {
	case PLUGIN_MENU_TYPE_GLOBAL:
		switch (menuItemID) {
		case MENU_ID_GLOBAL_1:
			//ts3Functions.setPluginMenuEnabled(pluginID, MENU_ID_GLOBAL_1, 0);   // Auto tp aktivieren
			//ts3Functions.setPluginMenuEnabled(pluginID, MENU_ID_GLOBAL_2, 1);   // Auto tp deaktivieren
			settings.beginGroup("Config");
			settings.setValue("AutoTP", 1);
			settings.endGroup();
			settings.sync();
			reloadinifile();
			if (wantannoucments)
			giveverification(4);
			break;
		case MENU_ID_GLOBAL_2:
			//ts3Functions.setPluginMenuEnabled(pluginID, MENU_ID_GLOBAL_1, 1);   // Auto tp aktivieren
			//ts3Functions.setPluginMenuEnabled(pluginID, MENU_ID_GLOBAL_2, 0);   // Auto tp deaktivieren
			settings.beginGroup("Config");
			settings.setValue("AutoTP", 0);
			settings.endGroup();
			settings.sync();
			reloadinifile();
			if (wantannoucments)
			giveverification(5);
			break;
		case MENU_ID_GLOBAL_3:
			//ts3Functions.setPluginMenuEnabled(pluginID, MENU_ID_GLOBAL_3, false);   // Auto Channelbann aktivieren
			//ts3Functions.setPluginMenuEnabled(pluginID, MENU_ID_GLOBAL_4, true);   // Auto Channelbann deaktivieren
			settings.beginGroup("Config");
			settings.setValue("AutoChannelBann", 1);
			settings.endGroup();
			settings.sync();
			reloadinifile(); 
			if(wantannoucments)
				giveverification(6);
			break;
		case MENU_ID_GLOBAL_4:
			//ts3Functions.setPluginMenuEnabled(pluginID, MENU_ID_GLOBAL_3, true);   // Auto Channelbann aktivieren
			//ts3Functions.setPluginMenuEnabled(pluginID, MENU_ID_GLOBAL_4, false);   // Auto Channelbann deaktivieren
			settings.beginGroup("Config");
			settings.setValue("AutoChannelBann", 0);
			settings.endGroup();
			settings.sync();
			reloadinifile();
			if (wantannoucments)
			giveverification(7);
			break;
		case MENU_ID_GLOBAL_5:
			//ts3Functions.setPluginMenuEnabled(pluginID, MENU_ID_GLOBAL_5, false);   // Auto Operator aktivieren
			//ts3Functions.setPluginMenuEnabled(pluginID, MENU_ID_GLOBAL_6, true);   // Auto Operator deaktivieren
			settings.beginGroup("Config");
			settings.setValue("AutoOperator", 1);
			settings.endGroup();
			settings.sync();
			reloadinifile();
			if (wantannoucments)
			giveverification(8);
			break;
		case MENU_ID_GLOBAL_6:
			//ts3Functions.setPluginMenuEnabled(pluginID, MENU_ID_GLOBAL_5, true);   // Auto Operator aktivieren
			//ts3Functions.setPluginMenuEnabled(pluginID, MENU_ID_GLOBAL_6, false);   // Auto Operator deaktivieren
			settings.beginGroup("Config");
			settings.setValue("AutoOperator", 0);
			settings.endGroup();
			settings.sync();
			reloadinifile();
			if (wantannoucments)
			giveverification(9);
			break;
		case MENU_ID_GLOBAL_7:
			reloadinifile();
			break;
		case MENU_ID_GLOBAL_8:
			//ts3Functions.setPluginMenuEnabled(pluginID, MENU_ID_GLOBAL_8, true);   // Auto Operator aktivieren
			//ts3Functions.setPluginMenuEnabled(pluginID, MENU_ID_GLOBAL_9, false);   // Auto Operator deaktivieren
			settings.beginGroup("Config");
			settings.setValue("AntiChannelBann", 1);
			settings.endGroup();
			settings.sync();
			reloadinifile();
			if (wantannoucments)
			giveverification(10);
			break;
		case MENU_ID_GLOBAL_9:
			//ts3Functions.setPluginMenuEnabled(pluginID, MENU_ID_GLOBAL_8, true);   // Auto Operator aktivieren
			//ts3Functions.setPluginMenuEnabled(pluginID, MENU_ID_GLOBAL_9, false);   // Auto Operator deaktivieren
			settings.beginGroup("Config");
			settings.setValue("AntiChannelBann", 0);
			settings.endGroup();
			settings.sync();
			reloadinifile();
			if (wantannoucments)
			giveverification(11);
			break;

		case MENU_ID_GLOBAL_10:
			//ts3Functions.setPluginMenuEnabled(pluginID, MENU_ID_GLOBAL_8, true);   // Auto Operator aktivieren
			//ts3Functions.setPluginMenuEnabled(pluginID, MENU_ID_GLOBAL_9, false);   // Auto Operator deaktivieren
			settings.beginGroup("Config");
			settings.setValue("wantannoucments", 1);
			settings.endGroup();
			settings.sync(); 
			reloadinifile();
			if (wantannoucments)
				giveverification(18);
			break;

		case MENU_ID_GLOBAL_11:
			//ts3Functions.setPluginMenuEnabled(pluginID, MENU_ID_GLOBAL_8, true);   // Auto Operator aktivieren
			//ts3Functions.setPluginMenuEnabled(pluginID, MENU_ID_GLOBAL_9, false);   // Auto Operator deaktivieren
			settings.beginGroup("Config");
			settings.setValue("wantannoucments", 0);
			settings.endGroup();
			settings.sync();
			if (wantannoucments)
				giveverification(19);
			reloadinifile();
			break;
		case MENU_ID_GLOBAL_12: {
			// sucht alle buddys und listet sie im chat plus ihren gegenwärtigen channel auf;
			
			std::thread listallbuddysthread(listAllBuddys, serverConnectionHandlerID);
			listallbuddysthread.detach();

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
			if (!checkisonline(serverConnectionHandlerID, (anyID)selectedItemID)) {
				return;
			}

			char *UID;
			ts3Functions.getClientVariableAsString(serverConnectionHandlerID, (anyID)selectedItemID, CLIENT_UNIQUE_IDENTIFIER, &UID);

			if (isbuddy(UID)) {
				ts3Functions.printMessageToCurrentTab("User kann nicht blockiert werden weil er auf der Buddylist steht");
				return;
			}

			bool rewriteBanList;
			rewriteBanList = true;
			if (!isUIDBanned(UID)) {
				list.push_back(UID);
				if (wantannoucments) {
					char name[TS3_MAX_SIZE_CLIENT_NICKNAME];
					ts3Functions.getClientDisplayName(serverConnectionHandlerID, (anyID)selectedItemID, name, TS3_MAX_SIZE_CLIENT_NICKNAME);
					giveverification(1, (anyID)selectedItemID);
				}
				if (selectedItemID != NULL) {

					anyID myID;
					ts3Functions.getClientID(serverConnectionHandlerID, &myID);
					if (rechtecheck(serverConnectionHandlerID, myID, selectedItemID)) {

						uint64 newChannelID;
						ts3Functions.getChannelOfClient(serverConnectionHandlerID, myID, &newChannelID);
						std::thread bannedUserProcthread(BannedUserProc, serverConnectionHandlerID, (anyID)selectedItemID, newChannelID);
						bannedUserProcthread.detach();
						

					}
				}
			}

			else {

				std::list<std::string>::iterator result = find(list.begin(), list.end(), UID);

				if (result == list.end()) {
					ts3Functions.logMessage("UID not found", LogLevel_ERROR, "Plugin", serverConnectionHandlerID);
				}
				else {
					list.erase(result);
					if (wantannoucments) {
						giveverification(15, (anyID)selectedItemID);
					}
				}
			}
			if (rewriteBanList) {
				//std::ofstream outfile("plugins\\banlist.txt");
				std::ofstream outfile(configPathbanlist);
				if (!outfile.is_open()) {
					ts3Functions.logMessage("Couldnt open banlist", LogLevel_ERROR, "Plugin", serverConnectionHandlerID);
					return;
				}

				for (std::string iterator : list) {
					outfile << iterator << "\n";
				}
				outfile.close();
			
			}
			return;
			break;
		}					

		case MENU_ID_CLIENT_2: {
			/* Menu client 2 was triggered */
			// namen bannen/entbannen
			if (!checkisonline(serverConnectionHandlerID, (anyID)selectedItemID)) {
				return;
			}
			
			char* name = "";
			bool rewriteBanList;
			ts3Functions.getClientVariableAsString(serverConnectionHandlerID, (anyID)selectedItemID, CLIENT_NICKNAME, &name);

			rewriteBanList = true;
			if (!isNameBanned(name)) {
				namelist.push_back(name);
				if (wantannoucments) {
					giveverification(3, (anyID)selectedItemID);
				}
				anyID myID;
				ts3Functions.getClientID(serverConnectionHandlerID, &myID);
				if (selectedItemID != NULL) {
					if (rechtecheck(serverConnectionHandlerID,myID,selectedItemID)) {				
						uint64 newChannelID;
						ts3Functions.getChannelOfClient(serverConnectionHandlerID, myID, &newChannelID);
						std::thread bannedUserProcthread(BannedUserProc, serverConnectionHandlerID, (anyID)selectedItemID, newChannelID);
						bannedUserProcthread.detach();
					}
			}

			}
			else {

				std::list<std::string>::iterator result = find(namelist.begin(), namelist.end(), name);

				if (result == namelist.end()) {
					ts3Functions.logMessage("name not found", LogLevel_ERROR, "Plugin", serverConnectionHandlerID);
				}

				else {
					namelist.erase(result);
					if (wantannoucments) {
						giveverification(17, (anyID)selectedItemID);
					}
				}

			}

			if (rewriteBanList) {
				//std::ofstream outfile("plugins\\namebanlist.txt");
				std::ofstream outfile(configPathnamebanlist);
				if (!outfile.is_open()) {
					ts3Functions.logMessage("Couldnt open namebanlist", LogLevel_ERROR, "Plugin", serverConnectionHandlerID);
					return;
				}

				for (std::string iterator : namelist) {
					outfile << iterator << "\n";
				}
				outfile.close();
				
			}
			return;
			break;
		}
		case MENU_ID_CLIENT_3: {
			/* Menu client 3 was triggered */
			// Client in anderem channel in meinem channel channelBann geben
			anyID myID;
			uint64 mychannelID;

			ts3Functions.getClientID(serverConnectionHandlerID, &myID);
			ts3Functions.getChannelOfClient(serverConnectionHandlerID, myID, &mychannelID);
			ts3Functions.setchannelgroup(serverConnectionHandlerID, (anyID)selectedItemID, mychannelID, 12);
			break;
		}

		case MENU_ID_CLIENT_4: {
			/* Menu client 4 was triggered */
			// gibt channelgruppe gast
			anyID myID;
			uint64 mychannelID;
			ts3Functions.getClientID(serverConnectionHandlerID, &myID);
			ts3Functions.getChannelOfClient(serverConnectionHandlerID, myID, &mychannelID);
			ts3Functions.setchannelgroup(serverConnectionHandlerID, (anyID)selectedItemID, mychannelID, 9);

			break;
		}
		case MENU_ID_CLIENT_5: {
			/* Menu client 5 was triggered */
			//setzt nen client auf bud oder nimmst ihm weg
			if (!checkisonline(serverConnectionHandlerID, (anyID)selectedItemID)) {
				return;
			}
			char* UID = "";
			ts3Functions.getClientVariableAsString(serverConnectionHandlerID, (anyID)selectedItemID, CLIENT_UNIQUE_IDENTIFIER, &UID);


			if (isUIDBanned(UID)) {
				ts3Functions.printMessageToCurrentTab("User kann nicht auf die Buddylist gesetzt werden, da er geblockt ist");
				return;
			}

			bool rewriteBanList;
			rewriteBanList = true;
			if (!isbuddy(UID)) {

				buddylist.push_back(UID);	
				if (wantannoucments) {
					giveverification(2, (anyID)selectedItemID);
				}
				
			}

			else {

				std::list<std::string>::iterator result = find(buddylist.begin(), buddylist.end(), UID);

				if (result == buddylist.end()) {
					ts3Functions.logMessage("UID not found", LogLevel_ERROR, "Plugin", serverConnectionHandlerID);
				}
				else {
					buddylist.erase(result);
					if (wantannoucments) {
						char name[TS3_MAX_SIZE_CLIENT_NICKNAME];
						ts3Functions.getClientDisplayName(serverConnectionHandlerID, (anyID)selectedItemID, name, TS3_MAX_SIZE_CLIENT_NICKNAME);
						giveverification(16, (anyID)selectedItemID);
					}
				}
			}
			if (rewriteBanList) {
				//std::ofstream outfile("plugins\\buddylist.txt");
				std::ofstream outfile(configPathbuddylist);
				if (!outfile.is_open()) {
					ts3Functions.logMessage("Couldnt open buddylist", LogLevel_ERROR, "Plugin", serverConnectionHandlerID);
					return;
				}

				for (std::string iterator : buddylist) {
					outfile << iterator << "\n";
				}
				outfile.close();
			}
			return;
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
	uint64 channelGroupId;
	uint64 invokerchannelGroupID;
	char* UID;
	//own clientid
	if (ts3Functions.getClientID(serverConnectionHandlerID, &myID) != ERROR_ok) {  /* Get own client ID */
		ts3Functions.logMessage("Error querying own client id", LogLevel_ERROR, "Plugin", serverConnectionHandlerID);

	}				//own channel
	if (ts3Functions.getChannelOfClient(serverConnectionHandlerID, myID, &mychannelid) != ERROR_ok) {
		ts3Functions.logMessage("Error querying own channel id", LogLevel_ERROR, "Plugin", serverConnectionHandlerID);

	}	//test for own channel an channel ban and not my id
	if (channelGroupID == 12 && mychannelid == channelID && myID != clientID) {

		//own channelgroupid
		ts3Functions.getClientVariableAsUInt64(serverConnectionHandlerID, myID, CLIENT_CHANNEL_GROUP_ID, &channelGroupId);
		//test for own channel group and kick user
		if (channelGroupId == 11 || channelGroupId == 10) {

			// prüfen ob user buddie ist, wenn ja bann entziehen
			ts3Functions.getClientVariableAsString(serverConnectionHandlerID, clientID, CLIENT_UNIQUE_IDENTIFIER, &UID);

			if (isbuddy(UID)) {

				//invoker channelgroupid
				ts3Functions.getClientVariableAsUInt64(serverConnectionHandlerID, invokerClientID, CLIENT_CHANNEL_GROUP_ID, &invokerchannelGroupID);
				if (invokerchannelGroupID != 10) {
					if (invokerClientID != myID) {
						if (AntiChannelBann) {
							

							/*
							if (lastantibann == clientID) {

								Tempchannelantibannlist.push_back(clientID);

								if (wantannoucments) {
									char name[TS3_MAX_SIZE_CLIENT_NICKNAME];
									ts3Functions.getClientDisplayName(serverConnectionHandlerID, clientID, name, TS3_MAX_SIZE_CLIENT_NICKNAME);
									giveverification(20, name);
								}
								return;

							}*/


							ts3Functions.setchannelgroup(serverConnectionHandlerID, clientID, mychannelid, 9);

						}
					}
				}
			}
		}
	}
}

















/*
void ts3plugin_onTalkStatusChangeEvent(uint64 serverConnectionHandlerID, int status, int isReceivedWhisper, anyID clientID) {

	anyID myID;
	ts3Functions.getClientID(serverConnectionHandlerID, &myID);

	if (isGommeTS(serverConnectionHandlerID)) {
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


