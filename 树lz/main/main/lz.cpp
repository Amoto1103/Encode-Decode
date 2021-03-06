#include "lz.h"
#include "pch.h"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <bitset>

using namespace std;

void LZ::code(string input) {
	ifstream read;
	read.open(input, ios::in | ios::binary);
	if (!read)
	{
		cout << "源文件读取失败。" << endl;
		return;
	}
	ofstream write;
	write.open(input + ".lz", ios::out | ios::binary);
	if (!write)
	{
		cout << "压缩文件创建失败。" << endl;
		return;
	}
	bool flag = 0;
	Dict* temp = root;
	lastChar = read.get();
	while (read.good())
	{
		index++;
		do
		{
			if (read.fail())
			{
				for (int i = 1; i < 33; i++)
				{
					if (index <= pow(2, i))
					{
						output(i, 1, write);
						break;
					}
				}
				output(0, 0, write);
				read.close();
				write.close();
				cout << "压缩完成。" << endl;
				return;
			}
			else
			{
				for (int i = 0; i < 256; i++)
				{
					if (temp->son[i] == NULL)
					{
						temp->son[i] = new Dict{ index,lastChar,{} };
						temp = root;
						for (int i = 1; i < 33; i++)
						{
							if (index <= pow(2, i))
							{
								output(i, 1, write);
								break;
							}
						}
						output(8, 0, write);
						preIndex = 0;
						flag = 0;
						break;
					}
					if (lastChar == temp->son[i]->lastChar)
					{
						preIndex = temp->son[i]->index;
						temp = temp->son[i];
						flag = 1;
						lastChar = read.get();
						break;
					}
				}
			}
		} while (flag);
		lastChar = read.get();
	}
	output(0, 0, write);
	read.close();
	write.close();
	cout << "压缩完成。" << endl;
	return;
}

void LZ::decode(string srcAdd, string dstAdd) {
	ifstream read;
	read.open(srcAdd, ios::in | ios::binary);
	if (!read)
	{
		cout << "压缩文件读取失败。" << endl;
		return;
	}
	ofstream write;
	write.open(dstAdd, ios::out | ios::binary);
	if (!write)
	{
		cout << "解压文件创建失败。" << endl;
		return;
	}
	map[0] = {};
	lastChar = read.get();
	while (read.good())
	{
		read.unget();
		index++;
		for (int i = 1; i < 33; i++)
		{
			if (index <= pow(2, i))
			{
				input(i, 0, read);
				vec = map[preIndex];
				lastChar = read.get();
				if (read.good())
				{
					input(8, 1, read);
					vec.push_back(lastChar);
					map[index] = vec;
				}
				else
					read.unget();
				for (int i = 0; i < vec.size(); i++)
				{
						write.put(vec[i]);
				}
				break;
			}
		}
		lastChar = read.get();
	}
	if (index < 127)
	{
		preIndex = stack.to_ulong();
		if (preIndex != 0)
		{
			vec = map[preIndex];
			for (int i = 0; i < vec.size(); i++)
			{
				write.put(vec[i]);
			}
		}
	}
	read.close();
	write.close();
	cout << "解压完成。" << endl;
	return;
}

void LZ::output(int width, bool func, ofstream &write)
{
	switch (width)
	{
	case 0:
	{
		if (length == 0)
			return;
		text = stack.to_ulong();
		write.put(text);
		return;
	}
	default:
	{
		if (func)
		{
			bitset<32> temp(preIndex);
			for (int i = 0; i < width; i++)
				stack[length + i] = temp[i];
			length += width;
			for (int j = 4; j > 0; j--)
			{
				if (length > 8 * j - 1)
				{
					unsigned long word = stack.to_ulong();
					for (int k = 0; k < j; k++)
					{
						text = word;
						write.put(text);
						word >>= 8;
					}
					length -= 8 * j;
					stack.reset();
					stack = word;
					return;
				}
			}
		}
		else
		{
			bitset<8> temp(lastChar);
			for (int i = 0; i < width; i++)
				stack[length + i] = temp[i];
			unsigned long word = stack.to_ulong();
			text = word;
			write.put(text);
			word >>= 8;
			stack.reset();
			stack = word;
			return;
		}
	}
	}
}

void LZ::input(int width, bool func, ifstream &read)
{
	switch (func)
	{
	case 0:
	{
		bitset<8> temp;
		while (length < width)
		{
			lastChar = read.get();
			temp = lastChar;
			for (int i = 0; i < 8; i++)
				stack[length + i] = temp[i];
			length += 8;
		}
		unsigned long word = stack.to_ulong();
		preIndex = word % (int(pow(2, width)));
		length -= width;
		word >>= width;
		stack.reset();
		stack = word;
		return;
	}
	case 1:
	{
		bitset<8> temp(lastChar);
		for (int i = 0; i < 8; i++)
			stack[length + i] = temp[i];
		unsigned long word = stack.to_ulong();
		lastChar = word % (256);
		word >>= 8;
		stack.reset();
		stack = word;
		return;
	}
	}
}