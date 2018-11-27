#include "mysql.h"
#include <sstream>

mysql::mysql()
{
    driver = get_driver_instance();
	disable_redeems = false;
	__COMMAND_SIZE = 0;
}
mysql::~mysql()
{
	delete[__COMMAND_SIZE] __COMMANDS;
	__COMMAND_SIZE = 0;
	IP = "";
	USERNAME = "";
	PASSWORD = "";
}

bool mysql::error(sql::SQLException &e)
{
	std::cout << "MYSQL Error: ";
	std::cout << e.what() << std::endl;
	if (e.getErrorCode() == 2013)
		return false;
	if (e.getErrorCode() == 2006)
	{
		std::cout << "testing connection failure" << std::endl;
		try {
			
		}
		catch (sql::SQLException &f){
			error(f);
			return false;
		}

	}
	return false;
}
void mysql::connect(std::string ip, std::string username, std::string password, std::string table){
	try {
		mysql::table = table;
		
		sql::Connection* con = driver->connect(ip, username, password);
		
		bool btrue = true;

		if (con->isValid())
		{
			loggedin = true;
			database = botName+"_data";
			IP = ip;
			USERNAME = username;
			PASSWORD = password;
			mysql::table = table;
			genCommands();
		}
		driver->threadEnd();
		con->close();
		delete con;
		//tcp://127.0.0.1:3306"
	}
	catch (sql::SQLException &e){
		loggedin = false;
		std::cout << "Error: ";
		std::cout << e.what() << std::endl;
	}
}
bool mysql::isUser(std::string user)
{
	if (loggedin){
		for (int i = 0; i < strlen(user.c_str()); i++)
		{
			if (user[i] == ' ' || user[i] == '\n' || user[i] == '\r' || user[i] == '\'' || user[i] == '"' || user[i] == '@')
				return false;
		}

		if (getPoints(user) < 0)
			return false;
		else return true;

	}
}
int mysql::isAdmin(std::string user)
{
	if (loggedin){

		try {
			
			sql::Connection* con = driver->connect(IP, USERNAME, PASSWORD);
			con->setSchema(database);
			sql::Statement* stmt;
			sql::ResultSet* res;
			stmt = con->createStatement();
			int result = 0;
			bool found = false;
			res = stmt->executeQuery("SELECT admin from " + table + "_points  where name='" + user + "'");
			while (res->next()) {
				found = true;
				//cout << "\t... MySQL replies: ";
				/* Access column data by alias or column name */
				//cout << res->getString(1) << endl;
				result = res->getInt(1);
				
				

			}
			
			stmt->close();
			delete stmt;
			res->close();
			delete res;
			con->close();
			driver->threadEnd();
			delete con;
			return result;

		}
		catch (sql::SQLException &e){
			std::cout << "Error: ";
			if (error(e))
				
				return isAdmin(user);
		}

	}
}
bool mysql::addAdmin(std::string user)
{
	if (loggedin){
		if (!isUser(user))
		{
			return false;
		}
		if (isAdmin(user)>0)
		{
			return false;
		}
		try {
			
			sql::Connection* con = driver->connect(IP, USERNAME, PASSWORD);
			con->setSchema(database);
			sql::Statement* stmt;
			bool temp = false;
			stmt = con->createStatement();
			if (stmt->executeUpdate("update " + table + "_points set admin=1 where name='" + user + "'") == 1)
			{
				temp = true;
			}
			stmt->close();
			con->close();
			delete stmt;
			driver->threadEnd();
			delete con;
			return temp;

		}
		catch (sql::SQLException &e){
			std::cout << "Error: ";
			if (error(e))
				return isAdmin(user);
		}

	}
}
bool mysql::removeAdmin(std::string user, std::string callingUser)
{
	if (loggedin){
		if (!isUser(user))
		{
			return false;
		}
		if (isAdmin(user)==0)
		{
			return false;	
		}
		if (callingUser != ADMIN)
		{
			if(user!=callingUser)
			if (isAdmin(user) >= isAdmin(callingUser))
			{
				return false;
			}
		}

		try {
			
			sql::Connection* con = driver->connect(IP, USERNAME, PASSWORD);
			con->setSchema(database);
			sql::Statement* stmt;
			
			stmt = con->createStatement();
			bool result = false;
			bool found = false;
			if (stmt->executeUpdate("update " + table + "_points set admin=0 where name='" + user + "'") == 1)
			{
				result = true;
			}
			stmt->close();
			con->close();
			delete stmt;
			driver->threadEnd();
			delete con;
			return result;
		}
		catch (sql::SQLException &e){
			std::cout << "Error: ";
			if (error(e))
				return removeAdmin(user,callingUser);
		}

	}
}
bool mysql::setUserLevel(std::string user, int level, std::string callingUser)
{
	if (loggedin){
		if (!isUser(user))
		{
			return false;
		}
		if (level < 1 || isAdmin(user) < 1)
		{
			return false;
		}

		if(callingUser!=ADMIN)
		{
			if (level < 1 || level>=isAdmin(callingUser))
			{
				return false;
			}
			if (user == callingUser)
			{
				return false;
			}
			if (isAdmin(user) > isAdmin(callingUser))
			{
				return false;
			}
		}
		try {
			
			sql::Connection* con = driver->connect(IP, USERNAME, PASSWORD);
			con->setSchema(database);
			sql::Statement* stmt;
			stmt = con->createStatement();
			bool result = false;
			if (stmt->executeUpdate("update " + table + "_points set admin="+tostr(level)+" where name='" + user + "'") == 1)
			{
				result = true;
			}
			stmt->close();
			con->close();
			delete stmt;
			driver->threadEnd();
			delete con;
			return result;

		}
		catch (sql::SQLException &e){
			std::cout << "Error: ";
			if (error(e))
				return isAdmin(user);
		}

	}
}

bool mysql::addUser(std::string user)
{
	if (loggedin){
		try{
			if (isUser(user))
				return false;
			
			sql::Connection* con = driver->connect(IP, USERNAME, PASSWORD);
			con->setSchema(database);
			sql::Statement* stmt;
			
			stmt = con->createStatement();
			(stmt->executeUpdate("insert into " + table + "_points (name) values ('" + user + "')"));
			stmt->close();
			con->close();
			delete stmt;
			driver->threadEnd();
			delete con;
			return true;

		}

		catch (sql::SQLException &e){
			if (error(e))
				return addUser(user);
		}
	}
	return false;
}
bool mysql::deleteUser(std::string user)
{
	if (loggedin){
		try{
			if (!isUser(user))
				return false;
			
			sql::Connection* con = driver->connect(IP, USERNAME, PASSWORD);
			con->setSchema(database);
			sql::Statement* stmt;
			
			stmt = con->createStatement();

			(stmt->executeUpdate("delete from " + table + "_points where name ='" + user + "'"));
			stmt->close();
			con->close();
			delete stmt;
			driver->threadEnd();
			delete con;
			return true;

		}

		catch (sql::SQLException &e){
			if (error(e))
				return deleteUser(user);
		}
	}
	return false;
}
std::string mysql::leaderboard()
{
	if (loggedin){
		std::string temp = "The top viewers on the leaderboard are: ";
		try{
			
			sql::Connection* con = driver->connect(IP, USERNAME, PASSWORD);
			con->setSchema(database);
			sql::Statement* stmt;
			sql::ResultSet* res;
			stmt = con->createStatement();
			//string user = ADMIN;

			res = stmt->executeQuery("SELECT name from " + table + "_points where bot=0 and banned=0 order by alltime desc limit 5");
			while (res->next()) {

				//cout << "\t... MySQL replies: ";
				/* Access column data by alias or column name */
				//cout << res->getString(1) << endl;

				temp += res->getString(1) + ", ";
			}
			temp.pop_back();
			temp.pop_back();
			temp += ".";
			stmt->close();
			con->close();
			delete stmt;
			delete res;
			driver->threadEnd();
			delete con;
			if (temp == "The top viewers on the leaderboard are: ")
			{
				return "false";

			}
			else
				return temp;
		}
		
		catch (sql::SQLException &e){
			if (error(e))
				return leaderboard();
		}
	}
	
}

int mysql::getPoints(std::string user)
{
	if (loggedin){
		try{
			//driver->threadInit();
			int points = 0;
			bool found = false;
			{
				std::unique_ptr<sql::Connection> con(driver->connect(IP, USERNAME, PASSWORD));
				con->setSchema(database);
				std::unique_ptr<sql::Statement> stmt(con->createStatement());;
				std::unique_ptr<sql::ResultSet> res(stmt->executeQuery("SELECT points from " + table + "_points where name='" + user + "' and banned=0"));
				//stmt (con->createStatement());
				
				//string user = ADMIN;
				
				//res = stmt->executeQuery("SELECT points from " + table + "_points where name='" + user + "' and banned=0");
				while (res->next()) {
					found = true;
					//cout << "\t... MySQL replies: ";
					/* Access column data by alias or column name */
					//cout << res->getString(1) << endl;
					points = res->getInt(1);

				}
				stmt->close();
				res->close();
				con->close();
			}
			//delete stmt;
			//delete res;
			//driver->threadEnd();
			//delete con;
			if (!found)
			{
				return -1;
			}
			return points;
		}

		catch (sql::SQLException &e){
			//if (error(e))
				std::cout << "error in mysql::getPoints: " << &e;

		}

	}
	return -1;
}
std::string mysql::tostr(int in)
{
	if (in != 0){
		std::stringstream ss;
		ss << in;
		return ss.str();
	}
	else return "0";
}
bool mysql::isBanned(std::string user)
{
	if (loggedin){
		try{
			
			sql::Connection* con = driver->connect(IP, USERNAME, PASSWORD);
			con->setSchema(database);
			sql::Statement* stmt;
			sql::ResultSet* res;
			stmt = con->createStatement();
			bool temp = false;
			//string user = ADMIN;
		
			res = stmt->executeQuery("SELECT banned from " + table + "_points where name='" + user + "'");
			while (res->next()) {
				
				//cout << "\t... MySQL replies: ";
				/* Access column data by alias or column name */
				//cout << res->getString(1) << endl;
				temp= (res->getInt(1)==1);
				
			}
			stmt->close();
			con->close();
			delete stmt;
			delete res;
			driver->threadEnd();
			delete con;
			return temp;
		}

		catch (sql::SQLException &e){
			if (error(e))
				return isBanned(user);
		}
	}
	return true;
}
int mysql::getAllTime(std::string user)
{
	if (loggedin){
		try{
			
			sql::Connection* con = driver->connect(IP, USERNAME, PASSWORD);
			con->setSchema(database);
			sql::Statement* stmt;
			sql::ResultSet* res;
			stmt = con->createStatement();
			//string user = ADMIN;
			
			int temp = 0;
			res = stmt->executeQuery("SELECT alltime from " + table + "_points where name='" + user + "' and banned=0");
			while (res->next()) {
				
				//cout << "\t... MySQL replies: ";
				/* Access column data by alias or column name */
				//cout << res->getString(1) << endl;
				temp= res->getInt(1);

			}
			stmt->close();
			con->close();
			delete stmt;
			delete res;
			driver->threadEnd();
			delete con;
			return temp;
		}

		catch (sql::SQLException &e){
			if (error(e))
				return getAllTime(user);
		}
	}
	return -1;
}
void mysql::updateBonus(std::string user, int points)
{
	if (loggedin){
		std::string pstr = tostr(points);
		try{
			
			sql::Connection* con = driver->connect(IP, USERNAME, PASSWORD);
			con->setSchema(database);
			sql::Statement* stmt;
			sql::ResultSet* res;
			stmt = con->createStatement();

			res = stmt->executeQuery("SELECT bonus + " + pstr + " from " + table + "_points where name='" + user + "' and banned=0");
			if (res->next()) {
				if (stmt->executeUpdate("update " + table + "_points set bonus=" + res->getString(1) + " where name='" + user + "' and banned=0"))
				{


					//cout << "\t... MySQL replies: ";
					/* Access column data by alias or column name */
					//cout << res->getString(1) << endl;


					
				}
			}
			stmt->close();
			con->close();
			delete stmt;
			delete res;
			driver->threadEnd();
			delete con;
		}
		catch (sql::SQLException &e){
			error(e);
		}
	}
}
void mysql::updateWinnings(std::string user, int points)
{
	if (loggedin){
		std::string pstr = tostr(points);
		try{
			
			sql::Connection* con = driver->connect(IP, USERNAME, PASSWORD);
			con->setSchema(database);
			sql::Statement* stmt;
			sql::ResultSet* res;
			stmt = con->createStatement();

			res = stmt->executeQuery("SELECT winnings + " + pstr + " from " + table + "_points where name='" + user + "' and banned=0");
			if (res->next()) {
				if (stmt->executeUpdate("update " + table + "_points set winnings=" + res->getString(1) + " where name='" + user + "' and banned=0"))
				{


					//cout << "\t... MySQL replies: ";
					/* Access column data by alias or column name */
					//cout << res->getString(1) << endl;



				}
			}
			stmt->close();
			res->close();
			con->close();
			delete stmt;
			delete res;
			driver->threadEnd();
			delete con;
		}
		catch (sql::SQLException &e){
			error(e);
		}
	}
}
std::string mysql::givePoints(std::string user, int points, bool all)
{
	if (loggedin){
		std::string pstr;
		std::string astr;
		std::string temp="";
		int apoints;
		if (all)
		{
			apoints = getAllTime(user);
			if (apoints < 0)
			{
				all = false;
			}
		}
		int cpoints = getPoints(user);
		if (cpoints + points == cpoints)
		{
			if (points < 0)
			{
				points = points*(-1);
				return user + " has had " + tostr(points) + " points taken from them.";
			}
			return user + " has been given " + tostr(points) + " points.";

		}

		cpoints = cpoints + points;

		if (cpoints < 0)
		{
			cpoints = 0;
		}
		pstr = tostr(cpoints);



		//pstr = "1";
		try{
			
			sql::Connection* con = driver->connect(IP, USERNAME, PASSWORD);
			con->setSchema(database);
			sql::Statement* stmt;
			
			stmt = con->createStatement();
			if (stmt->executeUpdate("update " + table + "_points set points=" + pstr + " where name='" + user + "' and banned=0"))
			{


				//cout << "\t... MySQL replies: ";
				/* Access column data by alias or column name */
				//cout << res->getString(1) << endl;


				if (points > 0)
				{
					if (all == true)
					{
						apoints = apoints + points;
						astr = tostr(apoints);
						if (stmt->executeUpdate("update " + table + "_points set alltime=" + astr + " where name='" + user + "' and banned=0"))
						{

						}
						else
						{
							std::cout << "error awarding alltime points" << std::endl;
						}
					}
					
					temp= user + " has been given " + tostr(points) + " points.";
				}
				if (points < 0)
				{
					points = points*(-1);
					temp = user + " has had " + tostr(points) + " points taken from them.";
				}

			}
			if (temp == "")
			{
				temp= "User not found";
			}
			stmt->close();
			con->close();
			delete stmt;
			driver->threadEnd();
			delete con;
			return temp;
		}

		catch (sql::SQLException &e){
			if (error(e))
				
				return givePoints(user, points, all);
		}
	}
	return "false";
}
int mysql::commandSize()
{
	__try{
		int i = 0;
		while (__COMMANDS[i] != "END_OF_ARRAY")
		{
			i++;
		}
		if (i == __COMMAND_SIZE)
		{
			return __COMMAND_SIZE;
		}
	}
	__except (0)
	{
		return 0;
	}
}
void mysql::genCommands()
{
	try{
		std::cout << "gencommands()" << std::endl;
		sql::Connection* con = driver->connect(IP, USERNAME, PASSWORD);
		con->setSchema(database);
		sql::Statement* stmt;
		sql::ResultSet* res;
		stmt = con->createStatement();
		res = stmt->executeQuery("SELECT count(*) 'count' from " + table + "_rewards");
		res->next();
		int len = res->getInt(1);
		__COMMAND_SIZE = len;
		if (len > 0 && len < 10);
		delete[] __COMMANDS;
		__COMMANDS = new std::string[len + 1];
		res = stmt->executeQuery("SELECT command from " + table + "_rewards");
		int i = 0;
		while (res->next()) {

			__COMMANDS[i] = res->getString(1);
			i++;
		}
		stmt->close();
		res->close();
		con->close();
		delete stmt;
		delete res;
		//driver->threadEnd();
		delete con;
		__COMMANDS[len] = "END_OF_ARRAY";
	}
	catch (sql::SQLException &e)
	{
		if (error(e))
			return genCommands();
	}
}
std::string *mysql::getCommands()
{

	return __COMMANDS;
}
std::string mysql::redeem(std::string user, std::string prize)
{
	std::string temp = "";
	
	if (!disable_redeems)
	{
		
		sql::Connection* con = driver->connect(IP, USERNAME, PASSWORD);
		con->setSchema(database);
		sql::Statement* stmt;
		sql::ResultSet* res;
		int cost;
		bool found = false;
		
		
		stmt = con->createStatement();
		res = stmt->executeQuery("SELECT reward, cost from " + table + "_rewards");
		if (prize == "reset"&&user != ADMIN)
		{
			temp= "'redeem' is not a prize, and can not be redeemed.";
		}
		while (res->next()) {

			if (res->getString(1) == prize)
			{
				cost = res->getInt(2);
				if (getPoints(user) >= cost)
				{//user can afford to pay for prize
					if (cost >= 0)
					{//make sure we don't give points by making sure the cost is a positive number
						givePoints(user, (cost)*-1);
						(stmt->executeUpdate("insert into " + table + "_redeems (name, prize,time) values ('" + user + "','" + prize + "',NOW())"));
						temp= user + " spent " + tostr(cost) + " Points to redeem '" + prize + "'.";
					}
					else
					{//the cost is less than 0
						temp= "Error: The cost of '" + prize + "can not be less than 0!";
					}
				}
				else
				{//user does not have enough points
					temp= user + ", you need " + tostr(cost) + " Points to redeem '" + prize + "'.";
				}
			}//prize was not found

		}
		if(temp=="")
		temp= "Error: Prize: '" + prize + "' was not found.";
		stmt->close();
		res->close();
		con->close();
		delete stmt;
		delete res;
		driver->threadEnd();
		delete con;
	}
	else
	{
		temp= "Redeems have been disabled for testing purposes.";
	}

	return temp;

}
std::string mysql::getPrize(std::string command)
{
	std::string temp="false";
	if (commandSize() > 0);
	for (int i = 0; i < commandSize(); i++)
	{
		if (command == __COMMANDS[i])
		{
			try
			{
				
				sql::Connection* con = driver->connect(IP, USERNAME, PASSWORD);
				con->setSchema(database);
				sql::Statement* stmt;
				sql::ResultSet* res;
				stmt = con->createStatement();
				res = stmt->executeQuery("SELECT command from " + table + "_rewards");
				while (res->next()) {

					if (res->getString(1) == command)
					{
						res = stmt->executeQuery("SELECT reward from " + table + "_rewards where command='" + command + "'");
						while (res->next()) {
							temp= res->getString(1);
						}

					}

				}
				stmt->close();
				res->close();
				con->close();
				delete stmt;
				delete res;
				driver->threadEnd();
				delete con;
			}
			catch (...)
			{

			}
		}
	}
	
	return temp;
}
std::string mysql::info(std::string prize)
{
	
	sql::Connection* con = driver->connect(IP, USERNAME, PASSWORD);
	con->setSchema(database);
	sql::Statement* stmt;
	sql::ResultSet* res;
	stmt = con->createStatement();
	std::string temp = "";
	res = stmt->executeQuery("SELECT reward, cost from " + table + "_rewards");
	while (res->next()) {

		if (res->getString(1) == prize)
		{
			res = stmt->executeQuery("SELECT info from " + table + "_rewards where reward='" + prize + "'");
			while (res->next()) {
				temp= res->getString(1);
			}

		}
	}
	if (temp == "")
	{
		temp = "Error: Prize " + prize + " not found.";
	}
	stmt->close();
	res->close();
	con->close();
	delete stmt;
	delete res;
	driver->threadEnd();
	delete con;
	return temp;
}
std::string mysql::binfo(std::string prize)
{
	
	sql::Connection* con = driver->connect(IP, USERNAME, PASSWORD);
	con->setSchema(database);
	sql::Statement* stmt;
	sql::ResultSet* res;
	stmt = con->createStatement();
	std::string temp="";
	res = stmt->executeQuery("SELECT reward, command, cost from " + table + "_rewards");
	while (res->next()) {

		if (res->getString(1) == prize)
		{
			temp= prize + ". Use '" + res->getString(2) + " redeem' to redeem with " + res->getString(3) + " Points.";

		}
	}
	if(temp=="")
	temp= "Error: Prize " + prize + " not found.";
	stmt->close();
	res->close();
	con->close();
	delete stmt;
	delete res;
	driver->threadEnd();
	delete con;
	return temp;
}
bool mysql::bet(std::string user, char vote, int points)//user "your bet has been placed."
{
	try{
		bool open = false;
		
		sql::Connection* con = driver->connect(IP, USERNAME, PASSWORD);
		con->setSchema(database);
		sql::Statement* stmt;
		sql::ResultSet* res;
		bool temp = false;
		stmt = con->createStatement();
		res = stmt->executeQuery("SELECT status from " + table + "_contest");
		while (res->next()) {

			if (res->getString(1) == "open")
				open = true;
		}

		if (open)
		{

			if (stmt->executeUpdate(("insert into " + table + "_votes (name,bet,amount,winnings) values ('" + user + "', '" + vote + "', " + tostr(points) + ", " + tostr(points*(-1)) + ")").c_str()) == 1)
			{
				temp= true;
			}
			
		}
		stmt->close();
		res->close();
		con->close();
		delete stmt;
		delete res;
		driver->threadEnd();
		delete con;
		return temp;
	}
	catch (...)
	{
		return false;
	}

}
bool mysql::setResult(char vote)//vote "was picked as the winning option."
{
	try{
		int test;
		
		sql::Connection* con = driver->connect(IP, USERNAME, PASSWORD);
		con->setSchema(database);
		sql::Statement* stmt;
		sql::ResultSet* res;
		stmt = con->createStatement();
		bool ret = false;
		res = stmt->executeQuery("select status from " + table + "_contest");
		if (res->next()) {
			if (res->getString(1) == "open" || res->getString(1) == "closed"){

				closeContest();


				std::string temp = "update " + table + "_contest set result='"; temp += vote; temp += "'";
				test = stmt->executeUpdate(temp);
				if (test == 1){

					finishContest();
					ret= true;
				}
			}
		}
		stmt->close();
		res->close();
		con->close();
		delete stmt;
		delete res;
		driver->threadEnd();
		delete con;
		return ret;
	}
	catch (sql::SQLException &e)
	{
		error(e);
		return false;
	}
}
bool mysql::refundContest()
{
	try{
		
		sql::Connection* con = driver->connect(IP, USERNAME, PASSWORD);
		con->setSchema(database);
		sql::Statement* stmt;
		sql::ResultSet* res;
		std::string result;
		double pool = 0;
		int winnings = 0;

		stmt = con->createStatement();
		res = stmt->executeQuery("select status from " + table + "_contest");
		res->next();
		if (res->getString(1) == "finished")
		{
			stmt->close();
			res->close();
			con->close();
			delete stmt;
			delete res;
			driver->threadEnd();
			delete con;
			return false;
		}
		stmt->executeUpdate("update " + table + "_contest set status='finished'");


		res = stmt->executeQuery("select name, amount from " + table + "_votes");
		int amount = 0;
		std::string user;
		while (res->next()) {
			amount = res->getInt(2);
			winnings = int(amount);
			user = res->getString(1);
			givePoints(user, winnings);
			updateWinnings(user, winnings);
			stmt->executeUpdate("update " + table + "_votes set winnings=0");
		 }
		stmt->close();
		res->close();
		con->close();
		delete stmt;
		delete res;
		driver->threadEnd();
		delete con;
		return true;
	}
	catch (sql::SQLException &e)
	{
		error(e);
		return false;
	}
}
void mysql::finishContest()
{
	try{
		
		sql::Connection* con = driver->connect(IP, USERNAME, PASSWORD);
		con->setSchema(database);
		sql::Statement* stmt;
		sql::ResultSet* res;
		std::string result;
		double pool = 0;
		int winnings = 0;

		stmt = con->createStatement();
		res = stmt->executeQuery("select result from "+table+"_contest");
		if (res->next()) {
			result = res->getString(1);

		}
		stmt->executeUpdate("update "+table+"_contest set status='finished'");
		res = stmt->executeQuery("select (select sum(amount) from " + table + "_votes), (select sum(amount) from " + table + "_votes where bet='" + result + "')");
		if (res->next()) {
			if (res->getInt(2) == 0)
			{
				return;
			}
			pool = double(res->getInt(1)) / double(res->getInt(2));

		}

		res = stmt->executeQuery("select name, amount from " + table + "_votes where bet='" + result + "'");
		int amount = 0;
		std::string user;
		while (res->next()) {
			amount = res->getInt(2);
			winnings = int(amount*pool);
			user = res->getString(1);
			givePoints(user, winnings);
			updateWinnings(user, winnings);
			stmt->executeUpdate("update " + table + "_votes set winnings=" + tostr(winnings - amount) + " where name='" + user + "'");
		}
		stmt->close();
		res->close();
		con->close();
		delete stmt;
		delete res;
		driver->threadEnd();
		delete con;
	}
	catch (sql::SQLException &e)
	{
		error(e);
		return;
	}
}
std::string mysql::viewContest(){
	try{
		
		sql::Connection* con = driver->connect(IP, USERNAME, PASSWORD);
		con->setSchema(database);
		sql::Statement* stmt;
		sql::ResultSet* res;
		stmt = con->createStatement();
		std::string temp = "";
		res = stmt->executeQuery("select status from " + table + "_contest");
		res->next();
		if (res->getString(1) == "finished")
		{
			delete stmt;
			delete res;
			return "No contest is running.";
		}
		res = stmt->executeQuery("select question, a, b from " + table + "_contest");
		while (res->next()) {

			temp = "A contest is running " + res->getString(1) + " For " + res->getString(2) + " !bet a <amount>, For " + res->getString(3) + " !bet b <amount>";
		}
		stmt->close();
		res->close();
		con->close();
				delete stmt;
				delete res;
				driver->threadEnd();
				delete con;
				return temp;
			
		
	}

	catch (sql::SQLException &e)
	{
		error(e);
	}
}
std::string mysql::viewResult()
{
	try{
		
		sql::Connection* con = driver->connect(IP, USERNAME, PASSWORD);
		con->setSchema(database);
		sql::Statement* stmt;
		sql::ResultSet* res;
		stmt = con->createStatement();
		std::string temp = "";
		res = stmt->executeQuery("select question, result from " + table + "_contest");
		while (res->next()) {

			temp= res->getString(1) + " The correct answer was: " + res->getString(2) + ".";
		}
		stmt->close();
		res->close();
		con->close();
		delete stmt;
		delete res;
		driver->threadEnd();
		delete con;
		return temp;
	}
	catch (sql::SQLException &e)
	{
		error(e);
	}
}
std::string mysql::createContest(std::string question, int seconds, std::string a, std::string b)
{
	try{
		
		sql::Connection* con = driver->connect(IP, USERNAME, PASSWORD);
		con->setSchema(database);
		sql::Statement* stmt;
		sql::ResultSet* res;
		stmt = con->createStatement();
		res = stmt->executeQuery("select status from " + table + "_contest");
		res->next();
		if (res->getString(1) != "finished"){
			stmt->close();
			res->close();
			con->close();
			delete stmt;
			delete res;
			driver->threadEnd();
			delete con;
			return "A contest is already running";
		}
		stmt->executeUpdate("truncate table " + table + "_contest");
		stmt->executeUpdate("truncate table " + table + "_votes");
		if (stmt->executeUpdate(("insert into " + table + "_contest (question, a, b, status) values ('" + question + "', '" + a + "', '" + b + "', 'open')").c_str()) == 1){
			stmt->close();
			res->close();
			con->close();
			delete stmt;
			delete res;
			driver->threadEnd();
			delete con;
			return "Contest started " + question + " for " + a + " !bet a <amount> OR for " + b + " !bet b <amount>.";
		}
		stmt->close();
		res->close();
		con->close();
		delete stmt;
		delete res;
		driver->threadEnd();
		delete con;
		return "There was an error and the contest could not be started, please try again later.";
	}
	catch (sql::SQLException &e)
	{
		error(e);
		
		return "There was an sql error";
	}
}
std::string mysql::getAnswer(char result)
{
	try{
		
		sql::Connection* con = driver->connect(IP, USERNAME, PASSWORD);
		con->setSchema(database);
		sql::Statement* stmt;
		sql::ResultSet* res;
		stmt = con->createStatement();
		std::string temp = "select " + result; temp += " from " + table + "_contest";
		std::string ret = "";
		res = stmt->executeQuery(temp);
		while (res->next()) {

			ret= res->getString(1);
		}
		stmt->close();
		res->close();
		con->close();
		delete stmt;
		delete res;
		driver->threadEnd();
		delete con;
		return ret;
	}
	catch (sql::SQLException &e)
	{
		error(e);
	}
}
bool mysql::closeContest()
{
	try{
		
		sql::Connection* con = driver->connect(IP, USERNAME, PASSWORD);
		con->setSchema(database);
		sql::Statement* stmt;
		sql::ResultSet* res;
		stmt = con->createStatement();
		bool temp = false;
		res = stmt->executeQuery("select status from " + table + "_contest");
		res->next();
		if (res->getString(1) == "finished")
		{
			stmt->close();
			res->close();
			con->close();
			delete stmt;
			delete res;
			driver->threadEnd();
			delete con;
			return false;
		}
		if (stmt->executeUpdate("update " + table + "_contest set status='closed'") == 1){
			temp= true;
		}
		stmt->close();
		res->close();
		con->close();
		delete stmt;
		delete res;
		driver->threadEnd();
		delete con;
		return temp;
	}
	catch (sql::SQLException &e)
	{
		return false;
		error(e);
	}
}
