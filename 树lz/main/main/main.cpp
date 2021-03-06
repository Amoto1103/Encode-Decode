#include "lz.cpp"
#include "pch.h"
#include <iostream>
#include <string>

using namespace std;

int main()
{
	cout << "1压缩，2解压，其它退出。" << endl;
	int choice;
	cin >> choice;
	cin.ignore();
	switch (choice)
	{
	case 1:
	{
		cout << "输入文件路径及文件名。" << endl;
		string input;
		getline(cin, input);
		LZ lz;
		lz.code(input);
		return 0;
	}
	case 2:
	{
		cout << "输入压缩文件路径及文件名。" << endl;
		string srcAdd;
		getline(cin, srcAdd);
		cout << "输入解压文件路径及文件名。" << endl;
		string dstAdd;
		getline(cin, dstAdd);
		LZ lz;
		lz.decode(srcAdd, dstAdd);
		return 0;
	}
	default:
		return 0;
	}
}
