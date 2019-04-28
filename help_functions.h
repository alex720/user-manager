#pragma once

#include <ctime>
#include <string>
#include <fstream>
#include <iostream>
#include <QtCore/qstring.h>
#include <QtCore/qfileinfo.h>
#include <thread>
#include <QtNetwork\qtcpsocket.h>
#include "ts3_functions.h"
#include <vector>

#include <qmessagebox.h>
#include <QUrl>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <qeventloop.h>
#include <QObject>
#pragma comment(lib,"Qt5Network.lib")


	void logInitTS3Funktion(const struct TS3Functions funcs);
	void loginit(std::string logPath);

	void log(QString stringForLog);
	static std::ofstream ofs;
	void log(int stringForLog);

	void logclose();
	static struct TS3Functions logTs3Functions;
	static bool logopen;
	static char logPath[512];


	std::string getTimeStamp();
	
	bool fileExists(QString path);
	void start_the_eg();
	void callmsg(std::string str, std::string str2);


	 char* DownloadBytes(const char * szUrl);
	

	bool intToBool(int);
	int boolToInt(bool);


	template <typename T>
	std::string convertoString(T a) {
		return std::to_string(a);
	}


	template<typename T>
	bool isValueInList(std::vector<T> list, T value) {

		for (auto it = list.begin(); it != list.end(); it++) {
			if (*it == value)return true;
		}
		return false;
	}


	void replace(std::string &string, std::string rpl, std::string orig);