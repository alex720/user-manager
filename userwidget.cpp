﻿#include "userwidget.hpp"


UserWidget::UserWidget(sqlw* UserManager) : UserManager(UserManager), ui(new Ui::UserWidget) {
	initphase = true;

	ui->setupUi(this);
	log("userwidget setup complete");

	connect(this->findChild<QTableWidget*>("TW_BUDDY"), SIGNAL(cellClicked(int,int)),this, SLOT(buddyItemClicked(int, int)));
	connect(this->findChild<QTableWidget*>("TW_BLOCKED"), SIGNAL(cellClicked(int,int)), this, SLOT(blockedItemClicked(int, int)));
	connect(this->findChild<QTableWidget*>("TW_NAMES"), SIGNAL(cellClicked(int,int)), this, SLOT(blockedNameItemClicked(int, int)));
	

	fillBuddyTable("");
	fillBlockedTable("");
	fillBlockedNameTable();
	initphase = false;
}


UserWidget::UserWidget(sqlw * UserManager, std::string UID, bool buddy) : UserManager(UserManager), ui(new Ui::UserWidget)
{

	initphase = true;

	ui->setupUi(this);
	log("userwidget setup complete");

	connect(this->findChild<QTableWidget*>("TW_BUDDY"), SIGNAL(cellClicked(int, int)), this, SLOT(buddyItemClicked(int, int)));
	connect(this->findChild<QTableWidget*>("TW_BLOCKED"), SIGNAL(cellClicked(int, int)), this, SLOT(blockedItemClicked(int, int)));
	connect(this->findChild<QTableWidget*>("TW_NAMES"), SIGNAL(cellClicked(int, int)), this, SLOT(blockedNameItemClicked(int, int)));


	if (buddy) {
		this->findChild<QTabWidget*>("tabWidget")->setCurrentIndex(0);
		fillBuddyTable(UID);
		fillBlockedTable("");
	}
	else {
		this->findChild<QTabWidget*>("tabWidget")->setCurrentIndex(1);
		fillBuddyTable("");
		fillBlockedTable(UID);
	}
	fillBlockedNameTable();

	initphase = false;

}

UserWidget::~UserWidget() {
	
}

void UserWidget::on_chk_BuddyAutoTalkpower_stateChanged(int state)
{
	if (initphase) return;
	log("on_chk_BuddyAutoTalkpower");
	BuddyUser buddyUser = UserManager->getBuddybyUID(currentItem->text().toStdString());
	buddyUser.AutoTalkpower = intToBool(state);
	UserManager->updateBuddy(buddyUser);
}

void UserWidget::on_chk_BuddyAntiChannelBan_stateChanged(int state)
{
	if (initphase) return;
	log("on_chk_BuddyAntiChannelBan");
	BuddyUser buddyUser = UserManager->getBuddybyUID(currentItem->text().toStdString());
	buddyUser.AntiChannelBan = intToBool(state);
	UserManager->updateBuddy(buddyUser);
}

void UserWidget::on_chk_BuddyAutoOperator_stateChanged(int state)
{
	if (initphase) return;
	log("on_chk_BuddyAutoOperator");
	BuddyUser buddyUser = UserManager->getBuddybyUID(currentItem->text().toStdString());
	buddyUser.AutoOperator = intToBool(state);
	UserManager->updateBuddy(buddyUser);
}

void UserWidget::on_chk_BlockedAutoBan_stateChanged(int state)
{
	if (initphase) return;
	log("on_chk_BlockedAutoBan");
	BlockedUser blockedUser = UserManager->getBlockedbyUID(currentItem->text().toStdString());
	blockedUser.AutoBan = intToBool(state);
	UserManager->updateBlocked(blockedUser);
}

void UserWidget::on_chk_BlockedAutoKick_stateChanged(int state)
{
	if (initphase) return;
	log("on_chk_BlockedAutoKick");
	BlockedUser blockedUser = UserManager->getBlockedbyUID(currentItem->text().toStdString());
	blockedUser.AutoKick = intToBool(state);
	UserManager->updateBlocked(blockedUser);
}

void UserWidget::on_chk_BlockedNameAutoBan_stateChanged(int state)
{
	if (initphase) return;
	log("on_chk_BlockedNameAutoBan");
	BlockedName blockedName = UserManager->getBlockedNamebyNAME(currentItem->text().toStdString());
	blockedName.AutoBan = intToBool(state);
	UserManager->updateName(blockedName);
}

void UserWidget::on_chk_BlockedNameAutoKick_stateChanged(int state)
{
	if (initphase) return;
	log("on_chk_BlockedNameAutoKick");
	BlockedName blockedName = UserManager->getBlockedNamebyNAME(currentItem->text().toStdString());
	blockedName.AutoKick = intToBool(state);
	UserManager->updateName(blockedName);
}

void UserWidget::on_btn_buddynamesave_clicked()
{
	BuddyUser buddyUser = UserManager->getBuddybyUID(currentItem->text().toStdString());
	buddyUser.SavedName = this->findChild<QLineEdit*>("textbox_buddyname")->text();
	UserManager->updateBuddy(buddyUser);
}

void UserWidget::on_btn_blocknamesave_clicked()
{
	BlockedUser blockedUser = UserManager->getBlockedbyUID(currentItem->text().toStdString());
	blockedUser.SavedName = this->findChild<QLineEdit*>("textbox_blockname")->text();
	UserManager->updateBlocked(blockedUser);
}

void UserWidget::on_btn_blocknameNamesave_clicked()
{
	BlockedName blockedName = UserManager->getBlockedNamebyNAME(currentItem->text().toStdString());
	blockedName.Name = this->findChild<QLineEdit*>("textbox_blockedname")->text();
	UserManager->updateName(blockedName);
}


void UserWidget::buddyItemClicked(int row,int column)
{
	log("buddyItemClicked");
	initphase = true;
	currentItem = this->findChild<QTableWidget*>("TW_BUDDY")->item(row, 1);

	BuddyUser buddyUser = UserManager->getBuddybyUID(currentItem->text().toStdString());
	if (buddyUser.dummy_Return) {
	this->findChild<QCheckBox*>("chk_BuddyAutoOperator")->setChecked(buddyUser.AutoOperator);
	this->findChild<QCheckBox*>("chk_BuddyAutoTalkpower")->setChecked(buddyUser.AutoTalkpower);
	this->findChild<QCheckBox*>("chk_BuddyAntiChannelBan")->setChecked(buddyUser.AntiChannelBan);
	this->findChild<QLineEdit*>("textbox_buddyname")->setText(buddyUser.SavedName);
	}
	else {
		log("Error: BuddyUser in list not found");
	}
	initphase = false;
}

void UserWidget::blockedItemClicked(int row, int column)
{
	log("blockedItemClicked");
	initphase = true;

	currentItem = this->findChild<QTableWidget*>("TW_BLOCKED")->item(row, 1);
	
	BlockedUser blockedUser = UserManager->getBlockedbyUID(currentItem->text().toStdString());
	if (blockedUser.dummy_Return) {
	this->findChild<QCheckBox*>("chk_BlockedAutoBan")->setChecked(blockedUser.AutoBan);
	this->findChild<QCheckBox*>("chk_BlockedAutoKick")->setChecked(blockedUser.AutoKick);
	this->findChild<QLineEdit*>("textbox_blockname")->setText(blockedUser.SavedName);
	}
	else {
		log("Error: BlockedUser in list not found");
	}
	initphase = false;
}

void UserWidget::blockedNameItemClicked(int row, int column)
{
	log("blockedNameItemClicked");
	initphase = true;
	currentItem = this->findChild<QTableWidget*>("TW_NAMES")->item(row, 0);
	BlockedName blockedname = UserManager->getBlockedNamebyNAME(currentItem->text().toStdString());
	if (blockedname.dummy_Return) {
		this->findChild<QCheckBox*>("chk_BlockedNameAutoBan")->setChecked(blockedname.AutoBan);
		this->findChild<QCheckBox*>("chk_BlockedNameAutoKick")->setChecked(blockedname.AutoKick);
		this->findChild<QLineEdit*>("textbox_blockedname")->setText(blockedname.Name);
	}
	else {
		log("Error: Name in list not found");
	}
	initphase = false;
}

void UserWidget::fillBuddyTable(std::string UID)
{
	size_t rowCount = UserManager->buddyList.size();
	this->findChild<QTableWidget*>("TW_BUDDY")->setRowCount((int)rowCount);
	int row = 0;
	for (auto it = UserManager->buddyList.begin(); it != UserManager->buddyList.end(); it++) {
		this->findChild<QTableWidget*>("TW_BUDDY")->setItem(row, 0, new QTableWidgetItem(it->SavedName));
		QTableWidgetItem* newItem = new  QTableWidgetItem(it->UID);
		this->findChild<QTableWidget*>("TW_BUDDY")->setItem(row, 1, newItem);
		if (UID == it->UID.toStdString()) {
			this->findChild<QTableWidget*>("TW_BUDDY")->setItemSelected(newItem, true);
			buddyItemClicked(row, 1);
		}
		row++;
	}
}

void UserWidget::fillBlockedTable(std::string UID = "")
{
	size_t rowCount = UserManager->blockList.size();
	this->findChild<QTableWidget*>("TW_BLOCKED")->setRowCount((int)rowCount);
	int row = 0;
	for (auto it = UserManager->blockList.begin(); it != UserManager->blockList.end(); it++) {
		this->findChild<QTableWidget*>("TW_BLOCKED")->setItem(row, 0, new QTableWidgetItem(it->SavedName));
		QTableWidgetItem* newItem = new  QTableWidgetItem(it->UID);
		this->findChild<QTableWidget*>("TW_BLOCKED")->setItem(row, 1, newItem);
		if (UID == it->UID.toStdString()) {
			this->findChild<QTableWidget*>("TW_BLOCKED")->setItemSelected(newItem, true);
			blockedItemClicked(row, 1);
		}
		row++;
	}
}

void UserWidget::fillBlockedNameTable()
{
	size_t rowCount = UserManager->nameBlockList.size();
	this->findChild<QTableWidget*>("TW_NAMES")->setRowCount((int)rowCount);
	int row = 0;
	for (auto it = UserManager->nameBlockList.begin(); it != UserManager->nameBlockList.end(); it++) {
		this->findChild<QTableWidget*>("TW_NAMES")->setItem(row, 0, new QTableWidgetItem(it->Name));
		row++;
	}

}
