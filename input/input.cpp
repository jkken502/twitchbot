#include<iostream>
#include "input.h"

int LOCATION;
int ADD;
std::string TEST;
std::string TESTFOR;
std::string CHANNEL;

INPUT::INPUT(std::string test, std::string testfor, std::string channel)
{
	TEST = test;
	TESTFOR = testfor;
	CHANNEL = channel;
	_size = 0;
	int location = LOCATION = test.find(testfor);
	int add = ADD = testfor.length();
	for (int i = add + location; i < test.length(); i++)
	{
		if (test[i] == ' ')
		{
			_size++;
		}

	}
	if (_size == 0)
		return;
	ret = new std::string[_size + 1];
	int size = 0;
	for (int i = add + location; i < test.length(); i++)
	{
		if (test[i] == ' ')
		{
			size++;
			continue;
		}
		if (size>0)
			if (test[i] != '\n' && test[i] != '\r' && test[i] != '\b' && test[i] != '\'')
				ret[size - 1] += test[i];
	}

}
std::string *INPUT::getInput()
{
	return ret;
}
int INPUT::size()
{
	return _size;
}
void INPUT::test()
{
	std::string *input = ret;
	int size = _size;
	std::cout << "size: " << size << std::endl;
	for (int i = 0; i<size; i++)
	{
		std::cout << i << ": '" << input[i] << "'" << std::endl;
	}
}
std::string INPUT::getString()
{
	int add=ADD;
	int location=LOCATION;
	std::string test=TEST;
	std::string temp = "";
	for (int i = add + location; i < test.length(); i++)
	{
		
			if (test[i] != '\n' && test[i] != '\r' && test[i] != '\b' && test[i] != '\'')
				temp += test[i];
	}
	return temp;
}