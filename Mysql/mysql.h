#ifndef MYSQL_H
#define MYSQL_H
#include <c:/Program Files (x86)/MySQL/MySQL Connector C++ 1.1.9/include/cppconn/driver.h>
#include <c:/Program Files (x86)/MySQL/MySQL Connector C++ 1.1.9/include/cppconn/exception.h>
#include <c:/Program Files (x86)/MySQL/MySQL Connector C++ 1.1.9/include/cppconn/resultset.h>
#include <c:/Program Files (x86)/MySQL/MySQL Connector C++ 1.1.9/include/cppconn/statement.h>
#pragma comment(lib,"c:/Program Files (x86)/MySQL/MySQL Connector C++ 1.1.9/lib/opt/mysqlcppconn-static.lib")
#include <stdlib.h>
#include <iostream>
#include <string>
#include <cstdlib>
//using namespace std;
class mysql
{
public:
	mysql::mysql();
	mysql::~mysql();
	void mysql::testContest();//for testing only remove this after fixing contests payouts
	bool mysql::setUserLevel(std::string user, int level, std::string callingUser);
	void mysql::connect(std::string ip, std::string username, std::string password, std::string table);
	bool mysql::addAdmin(std::string user);
	int mysql::isAdmin(std::string user);
	bool mysql::removeAdmin(std::string user, std::string callingUser);
	void mysql::connect();
	void mysql::close();
	bool mysql::isUser(std::string user);
	bool mysql::isBanned(std::string user);
	bool mysql::addUser(std::string user);
	bool mysql::deleteUser(std::string user);
	int mysql::getPoints(std::string user);
	std::string mysql::leaderboard();
	int mysql::getAllTime(std::string user);
	std::string mysql::givePoints(std::string user, int points, bool all = false);
	void mysql::updateBonus(std::string user, int points);
	void mysql::updateWinnings(std::string user, int points);
	std::string mysql::redeem(std::string user, std::string prize);
	std::string mysql::info(std::string prize);
	std::string mysql::binfo(std::string prize);
	std::string mysql::getPrize(std::string command);
	std::string *mysql::getCommands();
	int mysql::commandSize();
	bool mysql::bet(std::string user, char vote, int points);
	bool mysql::setResult(char vote);
	std::string mysql::viewResult();
	std::string mysql::createContest(std::string question, int seconds, std::string a = "yes", std::string b = "no");
	bool mysql::closeContest();
	bool mysql::refundContest();
	std::string mysql::viewContest();
	std::string mysql::getAnswer(char result);
	void mysql::finishContest();
	bool mysql::lockedout();
	bool mysql::iscodevalid(std::string code);
	bool mysql::lockout(std::string code, std::string user, bool override=false);
	bool mysql::unlockout(std::string code, std::string user, bool override=false);
	std::string botName;
	std::string ADMIN;

private:
	sql::Driver* mysql::driver;
	std::string mysql::database;
	bool loggedin = false;
	bool mysql::disable_redeems;
	bool mysql::error(sql::SQLException &e);
	std::string mysql::table;
	std::string mysql::tostr(int in);
	//sql::Statement* mysql::stmt;
	//sql::ResultSet* mysql::res;
	std::string IP, USERNAME, PASSWORD;
	
	void mysql::genCommands();
	int __COMMAND_SIZE;
	std::string *mysql::__COMMANDS;
	friend class twitch;
};
#endif