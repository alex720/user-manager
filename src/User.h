#pragma once
#include <QtCore/qstring.h>


struct BuddyUser {
	QString UID = "";
	QString SavedName = "";
	bool AntiChannelBan = false;
	bool AutoOperator = false;
	bool AutoTalkpower = false;
	bool dummy_Return = false;

	
	bool compare(std::string toComp) const {
		if (this->UID.compare(toComp.c_str())) return true;
		return false;
	}

	bool operator==(const BuddyUser& vgl) const {
		if (UID == vgl.UID) return true;
		return false;
	}
};

struct BlockedUser {
	QString UID = "";
	QString SavedName = "";
	bool AutoBan = false;;
	bool AutoKick = false;;
	bool dummy_Return = false;

	
	bool compare(std::string toComp) const {
		if (this->UID.compare(toComp.c_str())) return true;
		return false;
	}

	bool operator==(const BlockedUser& vgl) const {
		if (UID == vgl.UID) return true;
		return false;
	}
};

struct BlockedName {
	QString UID = "";
	QString Name = "";
	bool AutoBan = false;;
	bool AutoKick = false;;
	bool dummy_Return = false;

	bool compare(std::string toComp) const {
		if (this->Name.compare(toComp.c_str())) return true;
		return false;
	}

	bool operator==(const BlockedName& vgl) const {
		if (Name == vgl.Name) return true;
		return false;
	}
};


struct BlockedCountry {
	QString UID = "";
	QString CountryTag = "";
	bool AutoBan = false;;
	bool AutoKick = false;;
	bool dummy_Return = false;

	bool compare(std::string toComp) const {
		if (this->CountryTag.compare(toComp.c_str())) return true;
		return false;
	}

	bool operator==(const BlockedCountry& vgl) const {
		if (CountryTag == vgl.CountryTag) return true;
		return false;
	}
};

