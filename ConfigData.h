#pragma once
#include <qsettings.h>
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
	std::string getkickMessage();

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
	void setkickMessage(std::string val);

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

	std::string kickMessage;
	

};

