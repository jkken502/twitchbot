#include <string>
using std::string;

class JSON{
public:
	JSON::JSON(string test);
	string*JSON::str();
	int JSON::str_size();
private:
	string *JSON::ret;
	int JSON :: size;
};

