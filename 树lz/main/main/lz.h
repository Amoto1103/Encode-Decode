#ifndef LZCode
#define LZCode

#include <unordered_map>
#include <vector>
#include <string>
#include <bitset>
#include <fstream>

using namespace std;

typedef unordered_map<unsigned long, vector<char>> Map;

struct Dict
{
	unsigned long index = 0;
	char lastChar = '\0';
	struct Dict *son[256] = {};
};

class LZ 
{
public:
	LZ() {};
	void code(string input);
	void decode(string srcAdd, string dstAdd);

private:
	Dict* root = new Dict;
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
