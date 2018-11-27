#ifndef TWITCH_H
#define TWITCH_H
#include <iostream>
#include <string>
#include "input.h"
#include "json.h"
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment(lib,"JSON.lib")
#pragma comment(lib,"Winmm.lib")
#pragma comment(lib,"Ws2_32.lib")
using namespace std;
#include <winsock2.h>
#include <ws2tcpip.h>
#include <timeapi.h>
#pragma comment(lib,"User32.lib")
#include <Windows.h>
#include "mysql.h"
#include "viewers.h"
#include <sstream>


class twitch
{
private:
	bool twitch::commandGen=false;
	mysql d;
	bool twitch::STOP = false;
	int twitch::timer = 0;
	int twitch::result;
	string twitch::test;
	char twitch::server_reply[600];
	unsigned long twitch::l;
	const char twitch::key = '0x24';
	
	//void twitch::connect();
	string twitch::OAUTH;
	string twitch::NICK;
	int twitch::msec = 0;
	int mlast = timeGetTime();
	int seconds = 0;
	int twitch::last = timeGetTime();
	void twitch::responses();
	void twitch::TIMER();
	bool CONNECTED;
	int clast = timeGetTime();
	int csec = 0;
	std::string IP, USERNAME, PASSWORD, TABLE;
	void twitch::resetMysql();
public:
	void twitch::consoleCommands(string test);
	bool twitch::stop();
	bool twitch::connected();
	mysql twitch::mysql;
	string twitch::CHANNEL;
	bool twitch::award=false;
	string twitch::bonusall(int amount);
	SOCKET twitch::server, twitch::self;
	int twitch::Send(SOCKET s, const char * buff);
	bool twitch::Strcmp(string a, string b);
	void twitch::connect(string oauth, string nick, string channel);
	string twitch::getUser(string test);
	string twitch::tostr(int in);
	int twitch::toInt(string str);
	bool twitch::isInt(string str);
	void twitch::check();
	void twitch::commands(string test);
	void twitch::mysqlUpdate();
	std::string botName = "twitchbot";
	std::string ADMIN;
};

#endif