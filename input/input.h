#ifndef INPUT_H
#define INPUT_H
#include <string>
//using namespace std;
namespace twitch_input
{
	class INPUT
	{
		int _size;
		std::string *ret;
	public:
		INPUT::INPUT(std::string test, std::string testfor, std::string channel);
		std::string * INPUT::getInput();
		int INPUT::size();
		void INPUT::test();
		std::string INPUT::getString();

	};
}
using namespace twitch_input;
#endif