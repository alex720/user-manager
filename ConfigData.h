#pragma once
#include <QtCore/qsettings.h>
#include "help_functions.h"


class ConfigData
{
public:
	ConfigData(std::string iniconfigPath);
	~ConfigData();

	bool getWorking();
	bool getAutoTP();
	bool getAutoBan();
	bool getAutoOperator();
	bool getAntiChannelBan();
	bool getwantannoucments();
	bool getautomute();
	bool getlogging();
	bool getuseTSList();
	bool getAutoKick();
	bool getchannelAnnouncments();
	std::string getkickMessage();
	QString getregExPattern();
	int getminimumOfTotalConnections();

	void setWorking(bool val);
	void setAutoTP(bool val);
	void setAutoBan(bool val);
	void setAutoKick(bool val);
	void setAutoOperator(bool val);
	void setAntiChannelBan(bool val);
	void setwantannoucments(bool val);
	void setautomute(bool val);
	void setlogging(bool val);
	void setuseTSList(bool val);
	void setchannelAnnouncments(bool val);
	void setkickMessage(std::string val);
	void setregExPattern(QString val);
	void setminimumOfTotalConnections(int val);

private:


	QString SettingsFile;

	void reload();

	bool working;
	bool AutoTP;
	bool AutoBan;
	bool AutoKick;
	bool AutoOperator;
	bool AntiChannelBan;
	bool wantannoucments;
	bool automute;
	bool logging;
	bool useTSList;
	bool channelAnnouncments;
	QString regExPattern;
	int minimumOfTotalConnections;

	std::string kickMessage;
	

};

