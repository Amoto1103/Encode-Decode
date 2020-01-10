#include <iostream>
#include <fstream>
#include <cstdlib>
#include <algorithm>
#include <string>
#include <vector>
#include <cstring>

#include <bitset>

using namespace std;
struct word
{
	int fre;
	int word;
};
struct tnode {
	int word;
	int fre;
	char code[256];
	struct tnode *left;
	struct tnode *right;
};
bool complare(word a, word b);
void huff_produce(struct tnode* root, struct tnode huffnum[]);//编码函数声明


int main()
//压缩实现
{
	ifstream fin;
	cout << "Type (your input file) (your output file) to compress" << endl;
	string filename;
	getline(cin, filename);
	word words[256];
	for (int i = 0; i < 256; i++)
	{
		words[i] = { 0,0 };
	}//初始化字频统计数组

	fin.open(filename.c_str(), ios::binary);
	if (fin.fail())
	{
		cout << "fail\n";
		exit(1);
	}
	int next;
	cout << "Beginning ccing the frequency\n";
	while (true)
	{
		next = (int)fin.get();
		if (fin.eof()) break;
		int i = 0;
		while ((words[i].word != next) && (words[i].word != 0))
			i++;
		if (words[i].word == next)
		{
			words[i].fre++;
		}
		else if (words[i].word == 0)
		{
			words[i].word = next;
			words[i].fre++;
		}
	}

	sort(words, words + 256, complare);

	int kind = 0;
	while (words[kind].fre == 0)
		kind++;
	long int kindnum = 256 - kind;

	tnode* root = NULL;
	tnode *huff[256];
	for (int i = 0; i < kindnum; i++)
	{
		tnode *elem;
		elem = new tnode;
		elem->word = words[i + kind].word;
		elem->fre = words[i + kind].fre;
		for (int j = 0; j < 256; j++)
			elem->code[j] = '0';
		elem->left = NULL;
		elem->right = NULL;
		huff[i] = elem;
	}//构造一个名为huff的指针数组，每个指针指向一个节点，顺序按频率升序

	for (int i = 0; i < kindnum - 1; i++)
	{
		tnode *elem;
		elem = new tnode;
		elem->left = huff[i];
		elem->right = huff[i + 1];
		for (int j = 0; j < 256; j++)
			elem->code[j] = '0';
		elem->word = -2;
		elem->fre = elem->left->fre + elem->right->fre;
		//建立一个新父节点节点包含频率最小的两个子节点(其频率为和，word为0)
		for (int j = i + 2; j < kindnum; j++)
		{
			if (elem->fre >= huff[j]->fre)
			{
				huff[j - 1] = huff[j];
				huff[j] = elem;
			}
			else
			{
				huff[j - 1] = elem;
				break;
			}
		}//将elem插入入到huff(逐个比较，比elem小的前移一格)
		root = elem;
	}//构造huffman树(根节点为最后一个elem(root的最终值))

	root->code[0] = '\0';
	struct tnode huffnum[256];//编码表
	huff_produce(root, huffnum);//前缀码生成


	/*生成压缩文件*/
	/*fin.clear();
	fin.seekg(0, ios::beg);*/

	fin.close();
	fin.open(filename.c_str(), ios::binary);
	ofstream fout;
	fout.open(filename+".huff",ios::binary);

	//先写入文件的压缩信息
	//文件总词数+总种类数+(词+编码长+编码)
	long int filelen = root->fre;//文件总词数

	fout<<filelen;//写入总词数
	fout<<kindnum;//写入总种类
	for (int i = 0; i < kindnum; i++)
	{
		fout<<huffnum[i].word;
		int codelen = strlen(huffnum[i].code);

		fout<<codelen;
		int ij = 0;
		while (huffnum[i].code[ij] != '\0')
		{
			if (huffnum[i].code[ij] == 48) fout.put(0);
			if (huffnum[i].code[ij] == 49) fout.put(1);
			ij++;
		}
	}
	//开始文本写入
	cout<<"Beginning compressing\n";
	char pro[256];//临时存放前缀码
	pro[0] = '\0';
	int gch;
	while (true)
	{
		gch = (int)fin.get();

		if (fin.eof()) break;
		int i = 0;
		while (i < kindnum)
		{
			if (gch == huffnum[i].word)
				break;
			else i++;
		}//找到树中的词对应编码
		strcat(pro, huffnum[i].code);  //放入临时列表
		while (strlen(pro) >= 8)     //满8位写入
		{
			char c = 0;//使用移位操作符，将8位的前缀码信息储存在c中
			for (int i = 0; i < 8; i++)
			{
				if (pro[i] == '1')
					c = (c << 1) | 1;
				else c = c << 1;
			}
			fout.put(c);
			/*bitset<8> a(c);
			cout << a;*/
			strcpy(pro, pro + 8);
		}
	}



	if (strlen(pro) > 0)//最后不足的用0补全
	{
		char c = 0;
		strcat(pro, "00000000");
		for (int i = 0; i < 8; i++)
		{
			if (pro[i] == '1')
				c = (c << 1) | 1;
			else c = c << 1;
		}
		fout.put(c);
	}
	else;

	for (int i=0;i<kindnum;i++)
	{
		cout << huffnum[i].word << " " << huffnum[i].code << endl;
	}

	cout<<("Finish\n");






}

bool complare(word a, word b)
{
	return a.fre < b.fre;
}


void huff_produce(struct tnode* root, struct tnode huffnum[])
{
	static int count = 0;
	if (root->left) {
		strcpy(root->left->code, root->code);
		strcpy(root->right->code, root->code);
		strcat(root->left->code, "0");
		strcat(root->right->code, "1");
		huff_produce(root->left, huffnum);
		huff_produce(root->right, huffnum);
	}
	else
	{
		huffnum[count] = *root;
		count++;
	}
}

