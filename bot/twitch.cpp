#include "twitch.h"



void twitch::resetMysql()
{
	
	::mysql tmp;
	tmp.connect(IP, USERNAME, PASSWORD, TABLE);
	tmp.ADMIN = ADMIN;	
	d = tmp;
	
	
}

void twitch::commands(string test)
{
	
	string channel = CHANNEL;
		if (Strcmp(test, "PRIVMSG #" + channel + " :!"+botName+" reset"))
		{
			if (getUser(test) == ADMIN)
			{
				resetMysql();
			}
		}
		if (commandGen == false)
		{
			d.genCommands();
			commandGen = true;
		}
		string * mysqlCommands = d.getCommands();
		for (int i = 0; i < d.commandSize(); i++)
		{
			if (Strcmp(test, getUser(test) + ".tmi.twitch.tv PRIVMSG #" + channel + " :" + mysqlCommands[i]) && d.isBanned(getUser(test)) == false)
			{
				string prize = d.getPrize(mysqlCommands[i]);
				string info = d.binfo(prize);
				Send(server, ("PRIVMSG #" + channel + " :" + info).c_str());
			}
			if (Strcmp(test, getUser(test) + ".tmi.twitch.tv PRIVMSG #" + channel + " :" + mysqlCommands[i] + " info") && d.isBanned(getUser(test)) == false)
			{
				string prize = d.getPrize(mysqlCommands[i]);
				string info = d.info(prize);
				Send(server, ("PRIVMSG #" + channel + " :" + info).c_str());
			}
			if (Strcmp(test, getUser(test) + ".tmi.twitch.tv PRIVMSG #" + channel + " :" + mysqlCommands[i] + " redeem") && d.isBanned(getUser(test)) == false)
			{
				string prize = d.getPrize(mysqlCommands[i]);
				string info = d.info(prize);

				Send(server, ("PRIVMSG #" + channel + " :" + d.redeem(getUser(test), prize)).c_str());
			}

		}


		if (Strcmp(test, "PING :tmi.twitch.tv"))
		{
			Send(server, "PONG :tmi.twitch.tv");
		}
		if (Strcmp(test, "PRIVMSG #" + channel + " :!points") && d.isBanned(getUser(test)) == false)
		{

			if (d.getPoints(getUser(test)) >= 0){
				Send(server, ("PRIVMSG #" + channel + " :" + getUser(test) + " has " + tostr(d.getPoints(getUser(test))) + " Points.").c_str());
				cout << getUser(test) << " has " << d.getPoints(getUser(test)) << " Points." << endl;
			}
			else
			{
				Send(server, ("PRIVMSG #" + channel + " :" + getUser(test) + " has 0 Points.").c_str());
				cout << getUser(test) << " has 0 " << "Points." << endl;
			}

		}
		if (Strcmp(test, "PRIVMSG #" + channel + " :!leaderboard") && d.isBanned(getUser(test)) == false)
		{
			Send(server, ("PRIVMSG #" + channel + " :" + d.leaderboard()).c_str());
		}

		if (Strcmp(test, "PRIVMSG #" + channel + " :!bonusall"))
		{
			if (d.isAdmin(getUser(test)) > 0)
			{
				Send(server, ("PRIVMSG #" + channel + " :Usage: !bonusall <amount>").c_str());
			}
		}
		else if (test.find("PRIVMSG #" + channel + " :!bonusall ") != string::npos)
		{
			if (d.isAdmin(getUser(test)) > 0)
			{
				string testfor = ":!bonusall";
				//INPUT
				twitch_input::INPUT in(test, testfor, channel);
				in.test();
				if (in.size() == 1)
				{
					if (isInt(in.getInput()[0])){
						//bonusall(
						//cout<<"toINT(): "<<toInt(in.getInput()[1])<<endl;
						//in.test();
						cout << "Everyone has been given " << in.getInput()[0] << " points.";
						//bonusall(toInt(in.getInput()[0]));
						Send(server, ("PRIVMSG #" + channel + " :" + bonusall(toInt(in.getInput()[0]))).c_str());
					}
					else
					{
						cout << "notint" << endl;
						//notint
						//Send(server, ("PRIVMSG #" + channel + " :Usage: !bonusall <amount>").c_str());
					}


				}
				else
				{
					cout << "wrong size" << endl;
					//more than one
					//Send(server, ("PRIVMSG #" + channel + " :Usage: !bonusall <amount>").c_str());
				}
			}
		}



		if (Strcmp(test, "PRIVMSG #" + channel + " :!bonus"))
		{
			if (d.isAdmin(getUser(test)) > 0)
			{
				Send(server, ("PRIVMSG #" + channel + " :Usage: !bonus <user> <amount>").c_str());
			}
		}
		else if (test.find("PRIVMSG #" + channel + " :!bonus ") != std::string::npos)
		{
			if (d.isAdmin(getUser(test)) > 0)
			{
				string testfor = ":!bonus";
				//INPUT
				twitch_input::INPUT in(test, testfor, channel);
				//in.test();
				if (in.size() == 2)
				{
					if (isInt(in.getInput()[1])){
						if (d.isUser(in.getInput()[0])){
							//d.givePoints(in.getInput()[0], toInt(in.getInput()[1]));
							//cout<<"toINT(): "<<toInt(in.getInput()[1])<<endl;
							//in.test();
							cout << in.getInput()[0] << " has been given " << in.getInput()[1] << " points.";
							Send(server, ("PRIVMSG #" + channel + " :" + d.givePoints(in.getInput()[0], toInt(in.getInput()[1]))).c_str());
							d.updateBonus(in.getInput()[0], toInt(in.getInput()[1]));
						}
						else
						{
							//notuser
							Send(server, ("PRIVMSG #" + channel + " :" + in.getInput()[0] + " is not in your chat.").c_str());
						}
					}
					else
					{
						//notint
						Send(server, ("PRIVMSG #" + channel + " :Usage: !bonus <user> <amount>").c_str());
					}
				}
				else
				{
					//more than two
					Send(server, ("PRIVMSG #" + channel + " :Usage: !bonus <user> <amount>").c_str());
				}
			}
		}


		if (Strcmp(test, "PRIVMSG #" + channel + " :!tracking") && d.isBanned(getUser(test)) == false)
		{
			if (d.isAdmin(getUser(test)) > 1)
			{
				if (award)
				{
					Send(server, ("PRIVMSG #" + channel + " :"+botName+" is tracking loyalty").c_str());
				}
				else
				{
					Send(server, ("PRIVMSG #" + channel + " :"+botName+" is not tracking loyalty").c_str());
				}
			}
		}
		if (Strcmp(test, "PRIVMSG #" + channel + " :!start"))
		{
			if (d.isAdmin(getUser(test)) > 1){
				Send(server, ("PRIVMSG #" + channel + " :"+botName+" is now tracking loyalty.").c_str());
				award = true;
				//last = SOCK::timeGetTime();

			}
		}
		if (Strcmp(test, "PRIVMSG #" + channel + " :!stop"))
		{
			if (d.isAdmin(getUser(test)) > 1){
				Send(server, ("PRIVMSG #" + channel + " :"+botName+" is no longer tracking loyalty.").c_str());
				award = false;
			}
		}

		if (Strcmp(test, "PRIVMSG #" + channel + " :!"+botName+" add"))
		{
			//if (getUser(test)==ADMIN)
			if (d.isAdmin(getUser(test)) > 6)
			{
				Send(server, ("PRIVMSG #" + channel + " :Usage: !"+botName+" add <user>").c_str());
			}
		}
		else if (test.find("PRIVMSG #" + channel + " :!"+botName+" add ") != std::string::npos)
		{
			//if (getUser(test)==ADMIN)
			if (d.isAdmin(getUser(test)) > 6)
			{
				string testfor = ":!"+botName+" add";
				//INPUT
				twitch_input::INPUT in(test, testfor, channel);
				//in.test();
				if (in.size() == 1)
				{
					if (!d.isUser(in.getInput()[0])){
						if (d.addUser(in.getInput()[0]))
						{
							Send(server, ("PRIVMSG #" + channel + " :Operation Successful.").c_str());
						}
						else
						{
							Send(server, ("PRIVMSG #" + channel + " :Operation Failure.").c_str());
						}
					}
					else
					{
						//notuser
						Send(server, ("PRIVMSG #" + channel + " :Operation Failure: Duplicate entry.").c_str());
					}
				}

			}

		}
		if (Strcmp(test, "PRIVMSG #" + channel + " :!"+botName+" delete"))
		{
			if (getUser(test) == ADMIN)
				//if (d.isAdmin(getUser(test))>6)
			{
				Send(server, ("PRIVMSG #" + channel + " :Usage: !"+botName+" delete <user>").c_str());
			}
		}
		else if (test.find("PRIVMSG #" + channel + " :!"+botName+" delete ") != std::string::npos)
		{
			if (getUser(test) == ADMIN)
				//if (d.isAdmin(getUser(test))>6)
			{
				string testfor = ":!"+botName+" delete";
				//INPUT
				twitch_input::INPUT in(test, testfor, channel);
				//in.test();
				if (in.size() == 1)
				{
					if (d.isUser(in.getInput()[0])){
						if (d.deleteUser(in.getInput()[0]))
						{
							Send(server, ("PRIVMSG #" + channel + " :Operation Successful.").c_str());
						}
						else
						{
							Send(server, ("PRIVMSG #" + channel + " :Operation Failure.").c_str());
						}
					}
					else
					{
						//notuser
						Send(server, ("PRIVMSG #" + channel + " :Operation Failure: User not found.").c_str());
					}
				}

			}

		}
		if (Strcmp(test, "PRIVMSG #" + channel + " :!"+botName+" mod"))
		{
			if (getUser(test) == ADMIN || d.isAdmin(getUser(test)) > 4)
			{
				Send(server, ("PRIVMSG #" + channel + " :Usage: !"+botName+" mod <user>").c_str());
			}
		}
		else if (test.find("PRIVMSG #" + channel + " :!"+botName+" mod ") != std::string::npos)
		{
			if (getUser(test) == ADMIN || d.isAdmin(getUser(test)) > 4)
			{
				string testfor = ":!"+botName+" mod";
				//INPUT
				twitch_input::INPUT in(test, testfor, channel);
				//in.test();
				if (in.size() == 1)
				{
					if (d.isUser(in.getInput()[0])){
						if (d.addAdmin(in.getInput()[0]))
						{
							Send(server, ("PRIVMSG #" + channel + " :Operation Successful.").c_str());
						}
						else
						{
							Send(server, ("PRIVMSG #" + channel + " :Operation Failure.").c_str());
						}
					}
					else
					{
						//notuser
						Send(server, ("PRIVMSG #" + channel + " :Operation Failure: User not found.").c_str());
					}
				}

			}
		}
		if (Strcmp(test, "PRIVMSG #" + channel + " :!"+botName+" unmod"))
		{
			if (getUser(test) == ADMIN || d.isAdmin(getUser(test)) > 4)
			{
				Send(server, ("PRIVMSG #" + channel + " :Usage: !"+botName+" unmod <user>").c_str());
			}
		}
		else if (test.find("PRIVMSG #" + channel + " :!"+botName+" unmod ") != std::string::npos)
		{
			if (getUser(test) == ADMIN || d.isAdmin(getUser(test)) > 4)
			{
				string testfor = ":!"+botName+" unmod";
				//INPUT
				twitch_input::INPUT in(test, testfor, channel);
				//in.test();
				if (in.size() == 1)
				{
					if (d.isUser(in.getInput()[0])){
						if (d.removeAdmin(in.getInput()[0], getUser(test)))
						{
							Send(server, ("PRIVMSG #" + channel + " :Operation Successful.").c_str());
						}
						else
						{
							Send(server, ("PRIVMSG #" + channel + " :Operation Failure.").c_str());
						}
					}
					else
					{
						//notuser
						Send(server, ("PRIVMSG #" + channel + " :Operation Failure: User not found.").c_str());
					}
				}

			}

		}

		if (Strcmp(test, "PRIVMSG #" + channel + " :!"+botName+" userlevel"))
		{
			if (d.isAdmin(getUser(test)) > 0)
			{
				Send(server, ("PRIVMSG #" + channel + " :" + getUser(test) + ", has a moderation level of " + tostr(d.isAdmin(getUser(test))) + ".").c_str());
			}
		}

		if (Strcmp(test, "PRIVMSG #" + channel + " :!"+botName+" setuserlevel"))
		{
			if (d.isAdmin(getUser(test)) > 5)
			{
				Send(server, ("PRIVMSG #" + channel + " :Usage: !"+botName+" setuserlevel <user> <level>").c_str());
			}
		}
		else if (test.find("PRIVMSG #" + channel + " :!"+botName+" setuserlevel ") != std::string::npos)
		{
			if (d.isAdmin(getUser(test)) > 5 || getUser(test) == ADMIN)
			{
				string testfor = ":!"+botName+" setuserlevel";
				//INPUT
				twitch_input::INPUT in(test, testfor, channel);
				//in.test();
				if (in.size() == 2)
				{
					if (isInt(in.getInput()[1])){
						if (d.isUser(in.getInput()[0])){
							//d.givePoints(in.getInput()[0], toInt(in.getInput()[1]));
							//cout<<"toINT(): "<<toInt(in.getInput()[1])<<endl;
							//in.test();

							if (d.setUserLevel(in.getInput()[0], toInt(in.getInput()[1]), getUser(test)))
								Send(server, ("PRIVMSG #" + channel + " :Operation Successful.").c_str());
							else
								Send(server, ("PRIVMSG #" + channel + " :Operation Failure.").c_str());

						}
						else
						{
							//notuser
							Send(server, ("PRIVMSG #" + channel + " :Operation Failure: User not found.").c_str());
						}
					}
					else
					{
						//notint
						Send(server, ("PRIVMSG #" + channel + " :Usage: !!"+botName+" setuserlevel <user> <level>").c_str());
					}
				}
				else
				{
					//more than two
					Send(server, ("PRIVMSG #" + channel + " :Usage: !!"+botName+" setuserlevel <user> <level>").c_str());
				}
			}
		}
		//contest commands

		if (Strcmp(test, "PRIVMSG #" + channel + " :!contest create"))
		{
			if (getUser(test) == ADMIN || d.isAdmin(getUser(test)) > 2)
			{
				Send(server, ("PRIVMSG #" + channel + " :Usage: !contest create <question>").c_str());
			}
		}
		else if (test.find("PRIVMSG #" + channel + " :!contest create ") != std::string::npos)
		{
			if (getUser(test) == ADMIN || d.isAdmin(getUser(test)) > 2)
			{
				string testfor = ":!contest create";
				//INPUT
				twitch_input::INPUT in(test, testfor, channel);
				//in.test();
				if (in.size() > 0)
				{
					Send(server, ("PRIVMSG #" + channel + " :" + d.createContest(in.getString(), 300)).c_str());

				}
			}
		}
		if (Strcmp(test, "PRIVMSG #" + channel + " :!contest 1v1"))
		{
			if (getUser(test) == ADMIN || d.isAdmin(getUser(test)) > 2)
			{
				Send(server, ("PRIVMSG #" + channel + " :Usage: !contest 1v1 <player1> <player2>").c_str());
			}
		}
		else if (test.find("PRIVMSG #" + channel + " :!contest 1v1 ") != std::string::npos)
		{
			if (getUser(test) == ADMIN || d.isAdmin(getUser(test)) > 2)
			{
				string testfor = ":!contest 1v1";
				//INPUT
				twitch_input::INPUT in(test, testfor, channel);
				//in.test();
				if (in.size() == 2)
				{
					Send(server, ("PRIVMSG #" + channel + " :" + d.createContest("Who will win this next round?", 300, in.getInput()[0], in.getInput()[1])).c_str());

				}
			}
		}
		if (Strcmp(test, "PRIVMSG #" + channel + " :!contest result"))
		{
			if (getUser(test) == ADMIN || d.isAdmin(getUser(test)) > 3)
			{
				Send(server, ("PRIVMSG #" + channel + " :Set the winning option. Usage: !contest result <option>").c_str());
			}
		}
		else if (test.find("PRIVMSG #" + channel + " :!contest result ") != std::string::npos)
		{
			if (getUser(test) == ADMIN || d.isAdmin(getUser(test)) > 3)
			{
				string testfor = ":!contest result";
				//INPUT
				twitch_input::INPUT in(test, testfor, channel);
				//in.test();
				if (in.size() == 1)
				{
					if (d.setResult(in.getInput()[0][0]))
					{
						Send(server, ("PRIVMSG #" + channel + " :" + in.getInput()[0][0] + " was chosen as the winning option.").c_str());
					}
					else
					{
						Send(server, ("PRIVMSG #" + channel + " :" + "Error.").c_str());
					}
				}
			}
		}
		if (Strcmp(test, "PRIVMSG #" + channel + " :!contest close"))
		{
			if (getUser(test) == ADMIN || d.isAdmin(getUser(test)) > 2)
			{
				if (d.closeContest())
				{
					Send(server, ("PRIVMSG #" + channel + " :The contest has now closed for betting stay tuned to see the results.").c_str());
				}
				else
				{
					Send(server, ("PRIVMSG #" + channel + " :There was an error closing the bets.").c_str());
				}
			}
		}
		if (Strcmp(test, "PRIVMSG #" + channel + " :!contest"))
		{
			Send(server, ("PRIVMSG #" + channel + " :" + d.viewContest()).c_str());
		}
		if (Strcmp(test, "PRIVMSG #" + channel + " :!bet"))
		{

			{
				Send(server, ("PRIVMSG #" + channel + " :Usage: !bet <option>(a or b) <amount>(1-15)").c_str());
			}
		}
		else if (test.find("PRIVMSG #" + channel + " :!bet ") != std::string::npos)
		{

			{
				string testfor = ":!bet";
				//INPUT
				twitch_input::INPUT in(test, testfor, channel);
				//in.test();
				if (in.size() == 2)
				{
					string bet = in.getInput()[0];
					int amount = toInt(in.getInput()[1]);
					if (bet == "a" || bet == "b")
					{
						if (amount > 0 && amount < 16)
						{
							if (d.getPoints(getUser(test)) >= amount)
							{
								if (d.bet(getUser(test), in.getInput()[0][0], toInt(in.getInput()[1])))
								{
									Send(server, ("PRIVMSG #" + channel + " :" + getUser(test) + " Your bet has been placed.").c_str());
									d.givePoints(getUser(test), amount * (-1));
									d.updateWinnings(getUser(test), amount * (-1));
								}
								else
								{
									//Send(server, ("PRIVMSG #" + channel + " :" + getUser(test) + " You may only bet once.").c_str());
								}
							}
							else
							{
								Send(server, ("PRIVMSG #" + channel + " :" + getUser(test) + " You do not have enough points.").c_str());
							}
						}
						else
						{
							//Send(server, ("PRIVMSG #" + channel + " :"+getUser(test)+" Your bet must be between 1 and 15.").c_str());
						}


					}
					else
					{

						//Send(server, ("PRIVMSG #" + channel + " :"+getUser(test)+" Use !bet a for yes, or !bet b for no.").c_str());
					}
				}

			}

		}


		//end contest commands
		if (Strcmp(test, "PRIVMSG #" + channel + " :!contest refund"))
		{
			if (d.isAdmin(getUser(test)) > 2)
			{
				if (d.refundContest())
				{
					Send(server, ("PRIVMSG #" + channel + " :The contest has been refunded.").c_str());
				}
				else
				{
					Send(server, ("PRIVMSG #" + channel + " :The contest could not be refunded").c_str());
				}
			}
		}

		if (test.find("PRIVMSG #" + channel + " :!ban") != string::npos && getUser(test) == ADMIN)
		{
			//todo
		}
		if (test.find("PRIVMSG #" + channel + " :!unban") != string::npos && getUser(test) == ADMIN)
		{
			//todo
		}
}
int twitch::Send(SOCKET s, const char * buff){
	string temp;
	temp = buff;
	
	temp = temp + "\r\n";

	
	send(s, temp.c_str(), strlen(temp.c_str()), 0);
	
	Sleep(1000);
	return 0;
}
bool twitch::Strcmp(string a, string b)
{
	int aLen = strlen(a.c_str());
	int bLen = strlen(b.c_str());
	bool r = true;
	if (a.find(b) != std::string::npos)
	{
		if (aLen < bLen)
		{
			int bskip = 0;
			for (int i = 0; i < aLen + 2; i++)
			{
				if (a[aLen - i - 1] == '\n' || a[aLen - i - 1] == '\r')
				{
					bskip++;
					continue;
				}
				if (b[bLen - i - bskip + 1] != a[aLen - i + 1])
				{
					
					r = false;
				}
				else
				{
					
				}
			}

		}
		if (aLen > bLen)
		{
			int askip = 0;
			for (int i = 0; i < bLen + 2; i++)
			{
				if (a[aLen - i - 1] == '\n' || a[aLen - i - 1] == '\r')
				{
					askip++;
					continue;
				}
				if (a[aLen - i - askip + 1] != b[bLen - i + 1])
				{
					
					r = false;
				}
				else
				{
					
				}
			}

		}

		return r;

	}
	else
	{
		return false;
	}
}


void twitch::connect(string oauth, string nick, string channel)
{
	OAUTH = oauth;
	NICK = nick;

	
	
	string mystr;
	CHANNEL = channel;
	d = mysql;
	d.ADMIN = ADMIN;
	IP = d.IP;
	USERNAME = d.USERNAME;
	PASSWORD = d.PASSWORD;
	TABLE = d.table;
	SOCKADDR_IN SockAddr;
	SockAddr.sin_family = AF_INET;
	SockAddr.sin_port = htons(80);
	string ipstr;
	{

		struct addrinfo *result = NULL;
		struct addrinfo *ptr = NULL;
		struct addrinfo hints;
		struct sockaddr_in *ip;
		WSADATA wsadata;
		int error = WSAStartup(0x0202, &wsadata);
		ZeroMemory(&hints, sizeof(hints));
		hints.ai_family = AF_UNSPEC;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;
		getaddrinfo("irc.chat.twitch.tv", "6667", &hints, &result);
		if (result != NULL)
		{
			ip = (sockaddr_in *)result->ai_addr;
			ipstr = inet_ntoa(ip->sin_addr);
			cout << "IP addr: " << inet_ntoa(ip->sin_addr) << endl;
		}
		else
		{
			cout << "Could not connect." << endl;
			CONNECTED = false;
			return;
		}
	}
	SockAddr.sin_addr.s_addr = inet_addr(ipstr.c_str());
	WSADATA wsadata;
	int error = WSAStartup(0x0202, &wsadata);
	//Did something happen?
	if (error){
		cout << "winstock.h error";
		CONNECTED = false;
		return;
	}
	if (wsadata.wVersion != 0x0202)
	{
		cout << "invalid version";
		WSACleanup(); //Clean up Winsock
		CONNECTED = false;
		return;
	}
	server = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	{
		if (server == INVALID_SOCKET)
		{
			cout << "invalid socket" << endl;
			CONNECTED = false;
			return;
		}
	}
	
	if ((::connect(server, (SOCKADDR *)&SockAddr, sizeof(SockAddr))))
	{
		cout << "WINSOCK ERROR: Failed to connect to server. Error code: " << WSAGetLastError() << endl;
		CONNECTED = false;
		return;
	}
	STOP = false;
	CONNECTED = true;
	mystr = "PASS oauth:" + oauth;
	mystr = mystr + "\r\n";

	send(server, mystr.c_str(), strlen(mystr.c_str()), 0);
	Sleep(2);
	mystr = "NICK " + nick;
	mystr = mystr + "\r\n";
	send(server, mystr.c_str(), strlen(mystr.c_str()), 0);

	Sleep(2);
	mystr = "JOIN #" + channel;
	mystr = mystr + "\r\n";
	send(server, mystr.c_str(), strlen(mystr.c_str()), 0);
	Sleep(2);
	mystr = "CAP REQ :twitch.tv/membership";
	mystr = mystr + "\r\n";
	send(server, mystr.c_str(), strlen(mystr.c_str()), 0);
	

	clast = timeGetTime();
}

bool twitch::stop(){
	return STOP;
}
string twitch::getUser(string test)
{
	string tmp = "";
	string check = "";
	string user = "";
	int t;
	if (Strcmp(test, "!") == false || Strcmp(test, "@") == false)
	{
		//return "false";
	}
	for (int i = 1; i < strlen(test.c_str()); i++)
	{
		if (test[i] == '!'){
			t = i;
			break;
		}

		tmp += test[i];
	}
	for (int i = t + 1; i < strlen(test.c_str()); i++)
	{
		if (test[i] == '@')
		{
			t = i;
			break;
		}
		check += test[i];
	}
	if (tmp == check)
	{
		user = tmp;
		if (Strcmp(test, ":" + user + "!" + user + "@" + user + ".tmi.twitch.tv PRIVMSG #" + CHANNEL + " :!"))
		{
			//return tmp;
		}
		//return "false";
		return tmp;

	}
	return "end";

}

string twitch::tostr(int in)
{
	if (in != 0){
		stringstream ss;
		ss << in;
		return ss.str();
	}
	else return "0";
}
int twitch::toInt(string str){
	try{
		int neg = 1;
		std::string::size_type sz;
		int ret = 0;
		if (str.length() > 0)
		{
			ret = stoi(str, &sz);
			if (str[0] == '-')
			{
				//neg = -1;
			}
		}
		//cout << "J" << endl;

		return ret*neg;
	}
	catch (...)
	{
		return 0;
	}
}
bool twitch::isInt(string str){
	{
		int ret = toInt(str);

		return (tostr(ret) == str);
	}
}
void twitch::check()
{
	
	if (STOP)
	{
		while (CONNECTED == false)
		{
			connect(OAUTH, NICK, CHANNEL);
			Sleep(5000);
		}
		
		return;
	}
	memset(server_reply, '\0', 600);
	//using namespace SOCK;

	string channel = CHANNEL;


	TIMER();

	

	
	if (CONNECTED)
	{
		csec = (timeGetTime() - clast) / 1000;
		if (csec > 300){
			Send(server, "PING :tmi.twitch.tv");
			clast = timeGetTime();
			cout << "PING SENT" << endl;
			CONNECTED = false;
		}
	}
	if (!CONNECTED)
	{
		csec = (timeGetTime() - clast) / 1000;
		if (csec > 30)
		{
			clast = timeGetTime();
			cout << "Server stopped responding" << endl;
			STOP = true;
		}
	}
	ioctlsocket(server, FIONREAD, &l);
	if (l > 0)
	{

		result = recv(server, server_reply, sizeof(server_reply), 0);

		if (result > 0)
		{
			if (!CONNECTED)
			{
				CONNECTED = true;
			}
			clast = timeGetTime();
			test = server_reply;
			memset(server_reply, '\0', 600);
			cout << test << endl;
				string *line = nullptr;
				int authorization_string_check1 = 0;
				for (int i = 0; i < test.length(); i++)
				{
					if (test[i] == '\n')
					{
						//cout << "endl found at: " << i << endl;
						authorization_string_check1++;
					}
				}

				if (authorization_string_check1 > 0)
				{
					if (authorization_string_check1 > 0)
					{
						line = new string[authorization_string_check1];
						size_t last_pos = -1;
						for (int i = 0; i < authorization_string_check1; i++)
						{
							size_t pos = test.find("\n", last_pos + 1);
							if (pos != string::npos)
							{
								line[i] = test.substr(last_pos + 1, pos + 1);
								//cout << test.substr(0, 10);
								last_pos = pos;
							}
							else
								break;
						}
					}

				}
				for (int i = 0; i < authorization_string_check1; i++)
				{
					
					commands(line[i]);

				}
				delete[] line;

		}
		else if (result == 0)
		{
			cout << "connection closed"<<endl;
			STOP = true;
		}
		else
		{
			cout << "There was an error while reading recv()" << endl;
			STOP = true;
		}
	}

}
string twitch::bonusall(int amount)
{
	VIEWERS viewers(twitch::CHANNEL);
	string *str = viewers.getviewers();
	int size = viewers.size();
	for (int i = 0; i < size; i++){
		//cout << str[i]<<endl;
		if (!d.isUser(str[i]))
			d.addUser(str[i]);

		d.givePoints(str[i], amount);
		d.updateBonus(str[i], amount);
		
		
	}
	
	cout << endl<<endl<<endl<<"bonusall function completed()"<<endl<<endl<<endl;
	if (amount < 0)
		return ("Everyone has had " + tostr(amount*(-1)) + " Points taken away.");
	else
 		return "Everyone has been given " + tostr(amount) + " Points.";
}
void twitch::TIMER(){



	
		if (award == true){
			seconds = (timeGetTime() - last) / 1000;
			if (seconds > 600){
				cout << "Given points" << endl;
				VIEWERS viewers(CHANNEL);
				string *str = viewers.getviewers();
				int size = viewers.size();
				for (int i = 0; i < size; i++){
					//cout << str[i]<<endl;
					if (!d.isUser(str[i]))
						d.addUser(str[i]);
					d.givePoints(str[i], 1, true);
				};
				last = timeGetTime();
			}
		}
		if (award == true){
			msec = (timeGetTime() - mlast) / 1000;
			if (msec > 1800){
				Send(server, ("PRIVMSG #" + CHANNEL + " :You earn 1 Point for every 10 minutes you watch while " + CHANNEL + " is live. Check your Points by typing !points.").c_str());
				cout << "Message" << endl;
				mlast = timeGetTime();
			}
		}
}
bool twitch::connected()
{
	return CONNECTED;
}



