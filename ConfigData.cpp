#include "ConfigData.h"



ConfigData::ConfigData(std::string iniconfigPath) : SettingsFile(iniconfigPath.c_str())
{

	QSettings settings(SettingsFile, QSettings::IniFormat);

	settings.beginGroup("Config");
	if (settings.value("AutoTP", -1).toInt() == -1) {
		settings.setValue("AutoTP", 1);
	}
	if (settings.value("AutoBan", -1).toInt() == -1) {
		settings.setValue("AutoBan", 1);
	}
	if (settings.value("AutoKick", -1).toInt() == -1) {
		settings.setValue("AutoKick", 1);
	}
	if (settings.value("AutoOperator", -1).toInt() == -1) {
		settings.setValue("AutoOperator", 1);
	}
	if (settings.value("AntiChannelBan", -1).toInt() == -1) {
		settings.setValue("AntiChannelBan", 1);
	}
	if (settings.value("wantannoucments", -1).toInt() == -1) {
		settings.setValue("wantannoucments", 1);
	}
	if (settings.value("automute", -1).toInt() == -1) {
		settings.setValue("automute", 1);
	}
	if (settings.value("logging", -1).toInt() == -1) {
		settings.setValue("logging", 1);
		logclose();
	}
	if (settings.value("UseTSList", -1).toInt() == -1) {
		settings.setValue("UseTSList", 0);
	}
	if (settings.value("channelAnnouncments", -1).toInt() == -1) {
		settings.setValue("channelAnnouncments", 0);
	}
	if (settings.value("kickmessage", "").toString() == "") {
		settings.setValue("kickmessage", "Geblockter User");
	}

	if (settings.value("UseTSList", -1).toInt() == 1) {
		useTSList = true;
	}

	settings.endGroup();
	settings.sync();


	reload();

}


ConfigData::~ConfigData()
{
}

bool ConfigData::getWorking()
{
	return working;
}

bool ConfigData::getAutoTP()
{
	return AutoTP;
}

bool ConfigData::getAutoBan()
{
	return AutoBan;
}

bool ConfigData::getAutoOperator()
{
	return AutoOperator;
}

bool ConfigData::getAntiChannelBan()
{
	return AntiChannelBan;
}

bool ConfigData::getwantannoucments()
{
	return wantannoucments;
}

bool ConfigData::getautomute()
{
	return automute;
}

bool ConfigData::getlogging()
{
	return logging;
}

bool ConfigData::getuseTSList()
{
	return useTSList;
}

bool ConfigData::getAutoKick()
{
	return AutoKick;
}

bool ConfigData::getchannelAnnouncments()
{
	return channelAnnouncments;
}

std::string ConfigData::getkickMessage()
{
	return kickMessage;
}

void ConfigData::setWorking(bool val)
{
	working = val;
}

void ConfigData::setAutoTP(bool val)
{
	AutoTP = val;
	QSettings settings(SettingsFile, QSettings::IniFormat);
	settings.beginGroup("Config");
	if (val == 0) {
		settings.setValue("AutoTP", 0);
	}
	else {
		settings.setValue("AutoTP", 1);
	}
	settings.endGroup();
	settings.sync();
}

void ConfigData::setAutoBan(bool val)
{
	AutoBan = val;

	QSettings settings(SettingsFile, QSettings::IniFormat);
	settings.beginGroup("Config");
	if (val == 0) {
		settings.setValue("AutoChannelBan", 0);
	}
	else {
		settings.setValue("AutoChannelBan", 1);
	}
	settings.endGroup();
	settings.sync();
}

void ConfigData::setAutoKick(bool val)
{
	AutoKick = val;
	QSettings settings(SettingsFile, QSettings::IniFormat);
	settings.beginGroup("Config");
	if (val == 0) {
		settings.setValue("AutoChannelKick", 0);
	}
	else {
		settings.setValue("AutoChannelKick", 1);
	}
	settings.endGroup();
	settings.sync();
}

void ConfigData::setAutoOperator(bool val)
{
	AutoOperator = val;
	QSettings settings(SettingsFile, QSettings::IniFormat);
	settings.beginGroup("Config");
	if (val == 0) {
		settings.setValue("AutoOperator", 0);
	}
	else {
		settings.setValue("AutoOperator", 1);
	}
	settings.endGroup();
	settings.sync();
}

void ConfigData::setAntiChannelBan(bool val)
{
	AntiChannelBan = val;
	QSettings settings(SettingsFile, QSettings::IniFormat);
	settings.beginGroup("Config");
	if (val == 0) {
		settings.setValue("AntiChannelBan", 0);
	}
	else {
		settings.setValue("AntiChannelBan", 1);
	}
	settings.endGroup();
	settings.sync();
}

void ConfigData::setwantannoucments(bool val)
{
	wantannoucments = val;
	QSettings settings(SettingsFile, QSettings::IniFormat);
	settings.beginGroup("Config");
	if (val == 0) {
		settings.setValue("wantannoucments", 0);
	}
	else {
		settings.setValue("wantannoucments", 1);
	}
	settings.endGroup();
	settings.sync();
}

void ConfigData::setautomute(bool val)
{
	automute = val;
	QSettings settings(SettingsFile, QSettings::IniFormat);
	settings.beginGroup("Config");
	if (val == 0) {
		settings.setValue("automute", 0);
	}
	else {
		settings.setValue("automute", 1);
	}
	settings.endGroup();
	settings.sync();
}

void ConfigData::setlogging(bool val)
{
	logging = val;

	QSettings settings(SettingsFile, QSettings::IniFormat);
	settings.beginGroup("Config");
	if (val == 0) {
		settings.setValue("logging", 0);
		logclose();
	}
	else {
		settings.setValue("logging", 1);
		loginit(logPath);
	}
	settings.endGroup();
	settings.sync();
	
}

void ConfigData::setuseTSList(bool val)
{
	useTSList = val;
	QSettings settings(SettingsFile, QSettings::IniFormat);
	settings.beginGroup("Config");
	if (val == 0) {
		settings.setValue("UseTSList", 0);
	}
	else {
		settings.setValue("UseTSList", 1);
	}
	settings.endGroup();
	settings.sync();
}

void ConfigData::setchannelAnnouncments(bool val)
{
	channelAnnouncments = val;
	QSettings settings(SettingsFile, QSettings::IniFormat);
	settings.beginGroup("Config");
	if (val == 0) {
		settings.setValue("channelAnnouncments", 0);
	}
	else {
		settings.setValue("channelAnnouncments", 1);
	}
	settings.endGroup();
	settings.sync();
}

void ConfigData::setkickMessage(std::string val)
{
	kickMessage = val;
	QSettings settings(SettingsFile, QSettings::IniFormat);
	settings.beginGroup("Config");
	settings.setValue("kickmessage", val.c_str());
	settings.endGroup();
	settings.sync();
}

void ConfigData::reload()
{
	working = true;

	QSettings settings(SettingsFile, QSettings::IniFormat);

	settings.beginGroup("Config");

	if (settings.value("AutoTP").toInt() == 1) {
		AutoTP = true;
	}
	else {
		AutoTP = false;
	}
	if (settings.value("AutoBan").toInt() == 1) {
		AutoBan = true;
	}
	else {
		AutoBan = false;
	}
	if (settings.value("AutoKick").toInt() == 1) {
		AutoKick = true;
	}
	else {
		AutoKick = false;
	}
	if (settings.value("AutoOperator").toInt() == 1) {
		AutoOperator = true;
	}
	else {
		AutoOperator = false;
	}
	if (settings.value("AntiChannelBan").toInt() == 1) {
		AntiChannelBan = true;
	}
	else {
		AntiChannelBan = false;
	}
	if (settings.value("wantannoucments").toInt() == 1) {
		wantannoucments = true;
	}
	else {
		wantannoucments = false;
	}

	if (settings.value("automute").toInt() == 1) {
		automute = true;
	}
	else {
		automute = false;
	}

	if (settings.value("UseTSList").toInt() == 1) {
		useTSList = true;
	}
	else {
		useTSList = false;
	}

	if (settings.value("logging").toInt() == 1) {
		logging = true;
		loginit(logPath);
	}
	else {
		logging = false;
		logclose();
	}

	kickMessage = settings.value("kickmessage").toString().toStdString();

	settings.endGroup();

}
