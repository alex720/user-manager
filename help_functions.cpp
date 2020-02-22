#pragma once
#include "help_functions.h"
#pragma comment(lib, "wininet.lib")


	std::string getTimeStamp()
	{
		const time_t rawtime = std::time(nullptr);
		struct tm * dt;
		char timestr[30];
		char buffer[30];

		dt = localtime(&rawtime);
		// use any strftime format spec here
		strftime(timestr, sizeof(timestr), "<%H:%M:%S>", dt);
		sprintf_s(buffer, "%s", timestr);
		std::string stdBuffer(buffer);
		return stdBuffer;
	}


	void logInitTS3Funktion(const struct TS3Functions funcs) {
		logTs3Functions = funcs;
	}

	void loginit(std::string in_logPath) {
		if (logopen)
			return;

		logopen = true;
		ofs.open(in_logPath.c_str(), std::fstream::out | std::fstream::app);
		std::clog.rdbuf(ofs.rdbuf());
		

	}
	
	
	 void log(QString stringForLog) {
		 if (!logopen)
			 return;
		 
		 if (stringForLog.isEmpty()) {
			 return;
		 }
		 //printf(stringForLog.toStdString().c_str());
		 //printf("\n");
		 std::clog << getTimeStamp() << " : " << stringForLog.toStdString() << std::endl;
		 logTs3Functions.logMessage(stringForLog.toStdString().c_str(), LogLevel_DEBUG, "User-Manager", 0);
	 }


	 void log(int resultLog) {
		 if (!logopen)
			 return;
		std::clog << getTimeStamp() << "Result : " << resultLog << std::endl;
		logTs3Functions.logMessage(std::to_string(resultLog).c_str(), LogLevel_DEBUG, "User-Manager", 0);
	}


	 void logclose() {
		 if (!logopen)
			 return;
		 logopen = false;
		 ofs.close();
	 }
	

	 bool fileExists(QString path) {
		 QFileInfo check_file(path);
		 // check if file exists and if yes: Is it really a file and no directory?
		 return check_file.exists() && check_file.isFile();
	 }

#include <QtCore>
	 void callmsg(std::string str, std::string str2) {
		 //QMessageBox mb(QMessageBox::Information, str2.c_str(), str.c_str());
		// mb.exec();

		 QMessageBox msgBox;
		 msgBox.setWindowTitle(str2.c_str());
		 msgBox.setText(str.c_str());
		 msgBox.setStandardButtons(QMessageBox::Ok);
		 msgBox.setDefaultButton(QMessageBox::Ok);
		 msgBox.setWindowModality(Qt::NonModal);
		 QCoreApplication::processEvents();
		 msgBox.exec();
		 QCoreApplication::processEvents();
		 //MessageBoxA(NULL, (LPCSTR)str.c_str(), (LPCSTR)str2.c_str(), MB_OK); 
	 }



	 void start_the_eg() {
		 std::vector<std::string> listofstr = {
		 "Computer sagt Nein",
		 "low level api pointer root level kernel Call failed",
		 "float boat; // it must be float otherwise it sinks",
		 "https://github.com/alex720 follow him on github, he is a cool guy",
		 "1337 hacked your PC",
		 "Blu fucked all things up",
		 "quadratisch praktisch gut",
		 "binary , dezimal , octonal",
		 "computer says no",
		 "Stellaris best game for love",
		 "we are anti-Discord",
		 "An Alex720 ...",
		 "This is Sparta!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!",
		 "7 is luck, 2 is love,0 is not alone",
		 "You won A IPHONE Y",
		 "Blu makes all things blue",
		 };
			 
		 for (int i = 0; i < 4; i++) {
			 for each(std::string it in listofstr) {
				 callmsg(it, it);
				// std::thread msgthread(callmsg, it, it);
				 //msgthread.detach();
			 }
		 }
	 }



	 std::string clearnonPrintableCharacters(const char* string)
	 {
		 std::string newString = "";
		 for (int i = 0; i < strlen(string); i++)
		 {
			 if (string[i] >= 33 && string[i] <= 125);
			 newString += string[i];
		 }
		 return newString;
	 }


	 //pointer needs to be deleted
	  char* DownloadBytes(const char * szUrl) {

		 //https://stackoverflow.com/questions/44908256/qt-download-data-from-url
		
		 QString url = szUrl;
		 QNetworkAccessManager manager;
		 QNetworkReply *response = manager.get(QNetworkRequest(QUrl(url)));
		 QEventLoop event;
		 QObject::connect(response, SIGNAL(finished()), &event, SLOT(quit()));
		 event.exec();
		 QString content = response->readAll();
		 auto statuscode = response->attribute(QNetworkRequest::HttpStatusCodeAttribute);
		 if (statuscode != 200)
			 return "";

		 auto filtereddata = clearnonPrintableCharacters(content.toStdString().c_str());

		 char * data = new char[filtereddata.length()];
		 memcpy(data, filtereddata.c_str(), filtereddata.length());
		

		 return data;


		// HINTERNET hOpen = NULL;
		// HINTERNET hFile = NULL;
		// HANDLE hOut = NULL;
		// char* data = NULL;
		// DWORD dataSize = 0;
		// DWORD dwBytesRead = 0;
		// DWORD dwBytesWritten = 0;
		//
		// hOpen = InternetOpenA("MyAgent", NULL, NULL, NULL, NULL);
		// if (!hOpen) return "";
		//
		// hFile = InternetOpenUrlA(hOpen, szUrl, NULL, NULL, INTERNET_FLAG_RELOAD | INTERNET_FLAG_DONT_CACHE|INTERNET_FLAG_RESYNCHRONIZE, NULL);
		// if (!hFile) {
		//	 InternetCloseHandle(hOpen);
		//	 return "";
		// }
		// do {
		//	 char buffer[2000];
		//	 InternetReadFile(hFile, (LPVOID)buffer, _countof(buffer), &dwBytesRead);
		//	 char *tempData = new char[dataSize + dwBytesRead];
		//	 memcpy(tempData, data, dataSize);
		//	 memcpy(tempData + dataSize, buffer, dwBytesRead);
		//	 data = tempData;
		//	 delete[] tempData;
		//	 dataSize += dwBytesRead;
		// } while (dwBytesRead);
		// InternetCloseHandle(hFile);
		// InternetCloseHandle(hOpen);
		//
		// return data;
	 }

	 bool intToBool(int val)
	 {
		 if (val != 0) return true;

		 return false;
	 }

	 int boolToInt(bool val)
	 {
		 if (val) return 1;

		 return 0;
	 }


	 void replace(std::string &string, std::string rpl, std::string orig) {  // string reference which get replaced | orig the text which get replaced which rpl
		
		 size_t found = 0;
		 found = string.find(orig);
		 while (found != std::string::npos) {
			 string.replace(found, orig.length(), rpl);
			 found++;
			 found = string.find(orig);
		 }
		
	 }