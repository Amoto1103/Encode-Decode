#include "pch.h"
#include <string>
#include <iostream>

#include <fstream>
using namespace std;

int main()
{
	string filename;
	cout << "Please input filename to decompress:\n";
	getline(cin, filename);
	char a[6];
	ifstream fin;
	fin.open(filename.c_str(), ios::binary);
	fin.read(a, 6);
	unsigned int x = (int)a;
	cout << a;
}
