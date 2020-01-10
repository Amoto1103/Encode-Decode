#include <iostream>
#include <string>
#include <fstream>
#include "string.h"
#include<cstdlib>
#include<cstdio>

using namespace std;

struct word
{
	int word;
	int codelen;
	char code[256];
};

int main()
{
	string filename;
	cout << "Please input filename to decompress:\n";
	getline(cin, filename);
	long sum;
	int kind;
	ifstream fin;
	ofstream fout;
	fin.open(filename.c_str(), ios::binary);
	fin.read((char *)&sum, 4);
	fin.read((char *)&kind, 4);
	struct word Huffnode[256];
	for (int i = 0; i < kind; i++)
	{
		fin.read((char *)&Huffnode[i].word, 4);
		fin.read((char *)&Huffnode[i].codelen, 4);
		Huffnode[i].code[0] = '\0';
		for (int j = 0; j < Huffnode[i].codelen; j++)
		{
			if (fin.get() == 48)
				strcat(Huffnode[i].code, "0");
			else
				strcat(Huffnode[i].code, "1");
		}

	}
	fout.open("news.txt", ios::binary);
	char buff[256]; //临时存放读入的字符
	buff[0] = '\0';

	int num;


	while (1)
	{

		char buff1[256];
		char buff2[9] = "00000000";
		num = (int)fin.get();

		snprintf(buff1, sizeof(buff1), "%02x",num );
		for (int k = 0; k < strlen(buff1); k++)
		{
			buff2[k + 8 - strlen(buff1)] = buff1[k];
		}
		strcat(buff, buff2);//codes中此时存的为一串01编码

		bool flag = 1;
		while ((flag == 1)&&(sum>0))
		{
			flag = 0;
			int count = 0;
			for (int i = 0; i < kind; i++)
			{
				if (memcmp(Huffnode[i].code, buff, (unsigned int)Huffnode[i].codelen) == 0)
				{
					count = i;
					/*char c = Huffnode[count].word;  //将对应的字符保存在c中
					fout.put(c);*/
					fout.write((char *)&Huffnode[count].word, 1);
					strcpy(buff, buff + Huffnode[count].codelen);//把codes的字符串前移，前移位数=该叶子结点编码的长度
					sum--;
					flag = 1;
					break;
				}
			}

		}  //若找到则跳出for循环

		if (sum == 0)
			break;
	}
	fin.close();
	fout.close();
	return 0;
}
