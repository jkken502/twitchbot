#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "viewers.h"
#include "json.h"
#pragma comment(lib,"JSON.lib")


VIEWERS::VIEWERS(string channel)
{
	char server_reply[6000];
	memset(server_reply, '\0', sizeof(server_reply));
	string mystr;
	SOCKET s;
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
		getaddrinfo("tmi.twitch.tv", "80", &hints, &result);
		if (result != NULL)
		{
			ip = (sockaddr_in *)result->ai_addr;
			ipstr = inet_ntoa(ip->sin_addr);
			//cout << "IP addr: " << inet_ntoa(ip->sin_addr) << endl;
		}
		else
		{
			cout << "Could not connect." << endl;
			return;
		}
	}
	SockAddr.sin_addr.s_addr = inet_addr(ipstr.c_str());
	WSADATA wsadata;
	int error = WSAStartup(0x0202, &wsadata);

	//Did something happen?
	if (error){
		cout << "winstock.h error";
	}
	if (wsadata.wVersion != 0x0202)
	{
		cout << "invalid version";
		WSACleanup(); //Clean up Winsock

	}
	s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (s == INVALID_SOCKET)
	{
		cout << "invalid socket" << endl;
	}
	if ((connect(s, (SOCKADDR *)&SockAddr, sizeof(SockAddr))))
	{
		cout << "VIEWERS.CPP:Failed to connect to server. Error code: " << WSAGetLastError() << endl;
	}

	mystr = "GET /group/user/" + channel + "/chatters HTTP/1.1\r\nHost: tmi.twitch.tv\r\nAccept: text/htmlr\nConnection: close\r\n\r\n";
	send(s, mystr.c_str(), strlen(mystr.c_str()), 0);





	int result;
	{

		result = recv(s, server_reply, sizeof(server_reply), 0);
		if (result > 0)
		{
			string str=server_reply;
			//str = "\"TEST\"";
			JSON test(str);
			int viewers = 0;
			for (int i = 0; i < test.str_size(); i++)
			{
				if (test.str()[i] != "_links" && test.str()[i] != "chatter_count" && test.str()[i] != "chatters" && test.str()[i] != "moderators"  && test.str()[i] != "staff" && test.str()[i] != "admins" && test.str()[i] != "global_mods" && test.str()[i] != "viewers")
				{
					viewers++;
				}
			}
			VIEWERS::_size = viewers;
			//cout << "size: "<<_size<<endl;
			
			viewer_str = new string[viewers];
			viewers = 0;
			for (int i = 0; i < test.str_size(); i++)
			{
				
				if (test.str()[i] != "_links" && test.str()[i] != "chatter_count" && test.str()[i] != "chatters" && test.str()[i] != "moderators"  && test.str()[i] != "staff" && test.str()[i] != "admins" && test.str()[i] != "global_mods" && test.str()[i] != "viewers")
				{
					
					//cout << viewers+1 << ": ";
					//cout << test.str()[i].c_str() << endl;
					viewer_str[viewers] = test.str()[i].c_str();
					viewers++;
					

				}
				
			}


			memset(server_reply, '\0', sizeof(server_reply));

		}
		else if (result == 0)
		{
			cout << "connection closed";

		}
		else
		{
			cout << "Error";

		}
	}





}
string* VIEWERS::getviewers()
{
	return viewer_str;
}
int VIEWERS::size()
{
	return _size;
}
