#include <string>
#include <iostream>
using namespace std;
#include <winsock2.h>
#include <ws2tcpip.h>
//#pragma comment(lib,"release/input.lib")
class VIEWERS
{
public:
	VIEWERS::VIEWERS(string channel);
	VIEWERS::~VIEWERS();
	string * VIEWERS::getviewers();
	int VIEWERS::size();
private:
	string * VIEWERS::viewer_str;
	int VIEWERS::_size;

};