#ifndef LZCode
#define LZCode

#include <unordered_map>
#include <vector>
#include <string>
#include <bitset>
#include <fstream>

using namespace std;

struct vecHash {
	size_t operator()(vector<char> vec) const
	{
		unsigned long __h = 0;
		for (int i = 0; i < vec.size(); i++)
		{
			__h += vec[i];
			__h <<= 8;
		}
		return hash<unsigned long>()(__h);
	}
};

struct vecCmp {
	bool operator()(vector<char> vec1, vector<char> vec2) const
	{
		if (vec1.size() != vec2.size())
			return false;
		for (int i = 0; i < vec1.size(); i++)
		{
			if (vec1[i] != vec2[i])
				return false;
		}
		return true;
	}
};

typedef unordered_map<vector<char>,unsigned long,vecHash,vecCmp> LZMap;
typedef unordered_map<unsigned long, vector<char>> Map;

class LZ 
{
public:
	LZ() {};
	void code(string input);
	void decode(string srcAdd, string dstAdd);

private:
	LZMap lzmap;
	Map map;
	unsigned long index = 0;
	unsigned long preIndex = 0;
	int length = 0;
	char lastChar = '\0';
	vector<char> vec;
	bitset<40> stack;
	char text;
	void output(int width, bool func, ofstream &write);
	void input(int width, bool func, ifstream &read);
};

#endif 
