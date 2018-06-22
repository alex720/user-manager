#pragma once
#include <qstring.h>




struct BuddyUser {

	QString UID = "";
	QString SavedName = "";
	bool AntiChannelBan = false;
	bool AutoOperator = false;
	bool AutoTalkpower = false;
	bool dummy_Return = false;

	
	bool compare(std::string toComp) {
		if (this->UID.compare(toComp.c_str())) return true;
		return false;
	}
	bool operator==(BuddyUser vgl) {
		if (UID == vgl.UID) return true;
		return false;
	}
};



struct BlockedUser {
	QString UID = "";
	QString SavedName = "";
	bool AutoBan = false;;
	bool AutoKick = false;;
	bool dummy_Return = false;;

	
	bool compare(std::string toComp) {
		if (this->UID.compare(toComp.c_str())) return true;
		return false;
	}

	bool operator==(BlockedUser vgl) {
		if (UID == vgl.UID) return true;
		return false;
	}
};


struct BlockedName {
	QString Name = "";
	bool AutoBan = false;;
	bool AutoKick = false;;
	bool dummy_Return = false;

	bool compare(std::string toComp) {
		if (this->Name.compare(toComp.c_str())) return true;
		return false;
	}

	bool operator==(BlockedName vgl) {
		if (Name == vgl.Name) return true;
		return false;
	}
};

/*

TODOLSIT
- write infos to database
- write name in infodata


*/