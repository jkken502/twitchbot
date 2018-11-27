#include "twitch.h"
#include "mysql.h"


int main(){
	//setup variables
	string channel = "channel";
	twitch twitchbot;
	twitchbot.award = false;
	string mysql_ip = "tcp://127.0.0.1:3306";
	string mysql_user = "";
	string mysql_pass = "";
	string twitchbot_oauth = "";
	string twitchbot_twitch_username = "";
	string ADMIN = ""; //what is the twitch username of the bot administrator 
	//end setup

	twitchbot.botName = twitchbot_twitch_username;
	twitchbot.mysql.botName = twitchbot_twitch_username;
	twitchbot.mysql.ADMIN = ADMIN;
	twitchbot.ADMIN = ADMIN;
	twitchbot.mysql.connect(mysql_ip, mysql_user, mysql_pass, channel);


	while (twitchbot.connected() == false)
		{
			twitchbot.connect(twitchbot_oauth, twitchbot_twitch_username, channel);
		}
			while (true)
			{
				
				twitchbot.check();
				Sleep(500);
			}
			main();
		system("pause");
	return 0;
}
