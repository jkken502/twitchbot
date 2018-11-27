#include <iostream>
#include <vector>
#include<string.h>
#include"json.h"
using std::string;
using std::cout;


class PAIR
{

	struct F {
		int i; bool b; string s;
	};
	struct FP {
		string key;
		F v;
	};

protected:
	string PAIR::key;
	struct FF {
		int i; bool b; string s; FP p;
	};
	struct FFP {
		string key;
		FF v;
	};

	
public:

	//idc about this operation it was just to silence an error
	PAIR operator=(int &idc){
		PAIR tmp;
		 return tmp;
	}
	PAIR::PAIR()
	{
		type = "unknown";
	}
	string type;
	virtual void setkey(string k){
		key = k;
	}
	string getkey()
	{
		return key;
	}
	 

};

template <class T>
class pair : public PAIR
{

private:
	void getType(int x)
	{
		type = "int";
		v.i = value;
	}
	void getType(bool x)
	{
		type = "bool";
		v.b = value;
	}
	void getType(string x)
	{
		type = "string";
		v.s = value;
	}
	void getType(PAIR x)
	{
		type = "pair";
		p.key = x.key;
		p.v = x.v;
		
	}
	
protected:
	T value;
public:

	bool operator==(pair<decltype(value)>& other)
	{
		
		if (this->value == other.value&&this->key == other.key)
			return true;
		else
			return false;
		if (this->type != other.type || this->type=="unknown"||this->type!=other.type)
		throw std::bad_cast();
	
			
	}
	


	void setval(decltype(value) v)
	{
		value = v;
		getType(v);
		
	}
	T getval(){
		if (type != "pair")
		{
			return value;			
		}
		else
		{
			if (type == "pair")
			{
				
				return value;
			}
			
		}
		
	}


};

//class s: public pair
//{
//
//public:
//	
//	string value;
//
//	void setval(decltype(value) v)
//	{
//		value = v;
//		type = "string";
//	}
//
//	decltype(value) getval(){
//		return value;
//	}
//
//
//};
//class i: public pair
//{
//
//public:
//	
//	int value;
//	
//	
//	void setval(decltype(value) v)
//	{
//		value = v;
//		type = "int";
//	}
//
//	decltype(value) getval(){
//		return value;
//	}
//
//
//};
//class b: public pair
//{
//
//public:
//	
//	bool value;
//
//	void setkey(string k){
//		key = k;
//	}
//	void setval(decltype(value) v)
//	{
//		value = v;
//		type = "bool";
//	}
//
//	decltype(value) getval(){
//		return value;
//	}
//
//};
//class p : public pair
//{
//
//public:
//
//	pair value;
//
//	void setkey(string k){
//		key=k;
//	}
//	void setval(decltype(value) v)
//	{
//		value = v;
//		type = "pair";
//	}
//
//	decltype(value) getval(){
//		return value;
//	}
//
//};

//void test()
//{
//	string test = "{\"name\":\"steve\",\"args\":{\"connection\":\"true\", \"chatbody\" : \"I am the body\" }}";
//	int quote = 0;
//	int arrays = 0;
//	int arrayends = 0;
//	int colons = 0;
//	int comma = 0;
//	
//	using std::endl;
//	//auto test = "test";
//	pair * t;
//	i tmp1;
//
//	tmp1.setkey("key");
//	tmp1.setval(100);
//
//	t = new i(tmp1);
//	cout << t[0].getkey() << endl;
//	tmp1.value = 0;
//	//if(t[0].type
//	s *tmp = (s*)t;
//	t[0].type;
//	cout << tmp[0].type << endl;
//	
//	
//	
//	
//
//	
//	for (int i = 0; i < test.length(); i++)
//	{
//		switch (test[i])
//		{
//		case '"':quote++;
//		case ',':comma++;
//		case ':':colons++;
//		case '{':arrays++;
//		case '}':arrayends++;
//
//		}
//	}
//	if (arrays != arrayends)
//	{
//		return void();
//	}
//
//}
class json
{
public:
	
	std::vector<pair<PAIR>> pjson;
	int size = 0;
	int current = 0;
	json::json()
	{
		pair<PAIR> newp;
		newp.setkey("_DEFAULT_");
		pjson.push_back(newp);
		size = 1;
		current = 0;
		
	}
	void addpair(string key, int value)
	{
		pair<PAIR> newp;
		pair<decltype(value)> tmp;
		pair<decltype(value)> tmp2;
		tmp.setkey(key);
		tmp.setval(value);
		pjson[current].setval(tmp);
		newp.setkey("_NEXT_PAIR_");
		pjson.push_back(newp);
		current++;
	}
	void addpair(string key, string value)
	{
		pair<PAIR> newp;
		pair<decltype(value)> tmp;
		pair<decltype(value)> tmp2;
		tmp.setkey(key);
		tmp.setval(value);
		pjson[current].setval(tmp);
		newp.setkey("_NEXT_PAIR_");
		pjson.push_back(newp);
		current++;
	}
	void addpair(string key, bool value)
	{
		pair<PAIR> newp;
		pair<decltype(value)> tmp;
		pair<decltype(value)> tmp2;
		tmp.setkey(key);
		tmp.setval(value);
		pjson[current].setval(tmp);
		newp.setkey("_NEXT_PAIR_");
		pjson.push_back(newp);
		current++;
	}
	void embedpair(string key, int value)
	{
		/*_PAIR<pair> newp;
		_PAIR<decltype(value)> tmp;
		_PAIR<decltype(value)> tmp2;
		tmp.setkey(key);
		tmp.setval(value);
		pjson[current-1].getval().type*/
	}
	void embedpair(string key, string value)
	{

	}
	void embedpair(string key, bool value)
	{

	}
};
int main(){
	
	json test;
	auto value=1;
	string key = "";
	test.addpair(key, value);
	pair<PAIR> newp;
	pair<decltype(value)> tmp;
	PAIR *tmp2;
	tmp.setkey(key);
	tmp.setval(value);
	

		system("pause");


	}
//void test2(){
//	class pair
//	{
//
//	public:
//		//template <class T>
//		//T test;
//		string pair::key;
//		pair::pair()
//		{
//		}
//		string type;
//		void setkey(string k){
//			key = k;
//		}
//
//
//	};
//	template <class T>
//	class _PAIR : public pair
//	{
//		void getType(int x)
//		{
//			type = "int";
//		}
//		void getType(bool x)
//		{
//			type = "bool";
//		}
//		void getType(string x)
//		{
//			type = "string";
//		}
//		void getType(pair x)
//		{
//			type = "pair";
//		}
//	public:
//
//		T value;
//
//
//		void setval(decltype(value) v)
//		{
//			value = v;
//			getType(v);
//		}
//		decltype(value) getval(){
//			if (type == "pair")
//				return value.value;
//			else
//				return value;
//
//		}
//
//
//	};
//	class s : public pair
//	{
//
//	public:
//
//		string value;
//
//		void setval(decltype(value) v)
//		{
//			value = v;
//			type = "string";
//		}
//
//		decltype(value) getval(){
//			return value;
//		}
//
//
//	};
//	class i : public pair
//	{
//
//	public:
//
//		int value;
//
//
//		void setval(decltype(value) v)
//		{
//			value = v;
//			type = "int";
//		}
//
//		decltype(value) getval(){
//			return value;
//		}
//
//
//	};
//	class b : public pair
//	{
//
//	public:
//
//		bool value;
//
//		void setkey(string k){
//			key = k;
//		}
//		void setval(decltype(value) v)
//		{
//			value = v;
//			type = "bool";
//		}
//
//		decltype(value) getval(){
//			return value;
//		}
//
//	};
//	class p : public pair
//	{
//
//	public:
//
//		pair value;
//
//		void setkey(string k){
//			value.key = k;
//		}
//		void setval(decltype(value) v)
//		{
//
//			type = "pair";
//		}
//
//		decltype(value) getval(){
//			return value;
//		}
//
//	};
//	using std::endl;
//	auto test = "test";
//	pair * t;
//	s tmp1;
//
//	tmp1.setkey("key");
//	tmp1.setval(test);
//
//	t = new s(tmp1);
//	cout << t[0].key << endl;
//	//tmp1.value = 0;
//	s *tmpstr = (s*)t;
//	i *tmpint = (i*)t;
//	b *tmpbool = (b*)t;
//	p *tmppair = (p*)t;
//	if (t[0].type == "string")
//		cout << tmpstr[0].value << endl;
//	if (t[0].type == "int")
//		cout << tmpint[0].value << endl;
//	if (t[0].type == "bool")
//		cout << tmpbool[0].value << endl;
//	system("pause");
//}
