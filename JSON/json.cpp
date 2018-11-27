#include "json.h"
JSON:: JSON(string test)
{
	
	ret = { &string(" ", " ") };
	int start = -1;
	int end = -1;
	size = 0;
	int current_array = 0;
	//string test = "{\"name\":\"steve\",\"args\":[{\"connection\":\"true\"}, { \"chatbody\" : \"I am the body\" }]}";
	for (int i = 0; i < strlen(test.c_str()); i++)
	{
		switch (test[i])
		{
		case '"':
			size++;

		}
	}
	if (size > 0)
	{
		{

			
			using std::endl;
			size = size / 2;
			//cout << "size: " << size<< endl;
		}
		ret = new string[size];

		for (int i = 0; i < strlen(test.c_str()); i++)
		{
			switch (test[i])
			{
			case '"':
			{
				if (start < 0)
					start = i + 1;
				else if (end < 0)
				{
					end = i;
					ret[current_array] = "";
					for (int tmp = 0; tmp < end - start; tmp++)
					{
						ret[current_array] += test[start + tmp];
					}
					//cout << std::endl;
					start = -1;
					end = -1;
					current_array++;
				}

			}
			case ':':
			{}
			}
		}


	}
}
string * JSON::str()
{
	return ret;
}
int JSON::str_size()
{
	return size;
}