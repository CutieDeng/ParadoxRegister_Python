#include <iostream>
#include <windows.h>
#include <shlobj.h>
#include<string>
#include<vector>
#include<direct.h>
#include<fstream>
#include<io.h>
#include <stdio.h>
/* run this program using the console pauser or add your own getch, system("pause") or input loop */
#pragma comment(lib, "shell32.lib")
using namespace std;
vector<string> modpath;
vector<string> modname;
vector<string> modactname;
string gamename;
string getDocument() {
	CHAR my_documents[MAX_PATH];
	HRESULT result = SHGetFolderPathA(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, my_documents);

	if (result != S_OK) {
		cout << "读取您的我的文档文件夹失败，程序恐怕完全无法运作，对不起！" << endl;
		system("pause");
		exit(0);
	}
	else {
		return my_documents;
	}
	return "0";
}
int checkpath(string path)
{
	std::string prefix = path;
	if (_access(prefix.c_str(), 0) == -1)	//如果文件夹不存在
	{
		cout << "检查" + path + "文件夹完毕，您没有创建" + path + "文件夹，已经为您创建" << endl;
		int code = _mkdir(prefix.c_str());//则创建
		return code;
	}	
	else
	{
		cout << "检测到了" << path <<"文件夹！正在尝试进行下一步操作……" << endl;
				}			
	return -1;
}
//CSDN抄来的得到Files的代码，稍作修改使用
void getFiles(string path)
{
	/*files存储文件的路径及名称(eg.   C:\Users\WUQP\Desktop\test_devided\data1.txt)
	 ownname只存储文件的名称(eg.     data1.txt)*/

	 //文件句柄
	long   hFile = 0;
	//文件信息
	struct _finddata_t fileinfo;
	string p;
	if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1)
	{
		do
		{
			//如果是目录,加入列表
			//如果不是,不管它
			if ((fileinfo.attrib & _A_SUBDIR))
			{
				//去除掉上一级和上上一级那两个的路径避免死循环
				if(strcmp(fileinfo.name, ".")&&strcmp(fileinfo.name,".."))
				{ 
				modpath.push_back(p.assign(path).append("\\").append(fileinfo.name));
				modname.push_back(fileinfo.name);
				}
			}
			else
			{

			}
		} while (_findnext(hFile, &fileinfo) == 0);
		_findclose(hFile);
	}
}
string& replace_all(string& str, const  string& old_value, const  string& new_value)
{
	while (true)
	{
		string::size_type   pos(0);
		if ((pos = str.find(old_value)) != string::npos)
		{
			str.replace(pos, old_value.length(), new_value);
		}
		else { break; }
	}
	return   str;
}
//CSDN上抓来的
//返回0：无中文，返回1：有中文
int IncludeChinese(char *str)
{
char c;
while(1)
{
        c=*str++;
        if (c==0) break;  //如果到字符串尾则说明该字符串没有中文字符
        if (c&0x80)        //如果字符高位为1且下一字符高位也是1则有中文字符
                if (*str & 0x80) return 1;
    }
    return 0;
}
void loadmod()
{
	//要判断我的文档里面的mod文件夹是否存在，不存在先创建再说
	//因为这货涉及多层，干脆直接创建得了，不就是失败吗！
	_mkdir((getDocument() + "\\Paradox Interactive\\"+gamename).c_str());
	string gamefile = getDocument() + "\\Paradox Interactive\\"+gamename+ "\\mod\\";
	checkpath(gamefile);
	//判断成功，接下来遍历整个mod文件夹，得到所有文件夹。
	//为了获取完整路径，首先得到程序目录
	char buffer[300];
	_getcwd(buffer, 300);
	string nowpath = buffer;
	
	//判断程序目录是否含有中文，若有，提示用户（懒得管性能损失，来回瞎转）
	string s1 = nowpath;
	char *data;
	int len = s1.length();
	data = (char *)malloc((len+1)*sizeof(char));
	s1.copy(data,len,0);
	//以上是从网上拿过来的
	if(IncludeChinese(data)==1)
	{
		cout << "经过多次验证，我们确定：当您的游戏目录含有中文时，模组将无法加载" << endl;
		cout << "劳烦您将游戏换到其他目录，或者，将该软件放到不含中文的目录" << endl;
		cout << "软件将会自动关闭！" << endl;
		system("pause");
		exit(0);
		
		
	}
	
	getFiles(nowpath + "\\mod");
	//此时已经得到了所有MOD的目录，接下来读取每一个的descriptor.mod
	for (auto iter = modpath.cbegin(); iter != modpath.cend(); iter++) {
		//这个iter就是我们获取的string
		string despath = (*iter) + "\\descriptor.mod";
		string modspath = (*iter) + "";
		//cout << "debug " + despath << endl;
		ifstream in(despath);
		string line;
		if (in) // 有该文件
		{
			while (getline(in, line)) // line中不包括每行的换行符
			{
				string::size_type idx;
				idx = line.find("name=");//查找名字,因为path我们直接用现在的就好了
				if (idx != line.npos)
				{
					//此时得到的是名字的那行，是我们需要的
					//这样就能切除出来名字并放到modname里
					modactname.push_back(line.substr(6, line.length() - 1));
					//拿到line了，准备生成mod文件，写入方式，删除原文件
					//生成的mod文件名和mod名相同。
					ofstream fout;
					fout.open(getDocument() + "\\Paradox Interactive\\"+gamename+"\\mod" + "\\" + line.substr(6, line.length() - 1 -6) + ".mod",ios::trunc);
					cout << getDocument() + "\\Paradox Interactive\\"+gamename+"\\mod" + "\\" + line.substr(6, line.length() - 1 -6) + ".mod" << endl;
					//生成之后，写入两行：
					fout << line << endl;//mod名那一行
					fout << "path=\""+ replace_all(modspath, "\\", "/") + "\"" << endl;
					//生成完毕，关闭文件。
					fout.close();
				}
			}
		}
		else // 没有该文件
		{
			cout << "读取失败，请检查您的descriptor.mod文件，有些MOD“创新”的把.mod文件改了名\n复制一份，然后改成descriptor.mod即可……" << endl;
		}

	}
		cout << "您的模组已经加载完成，应当加载了" << modactname.size() << "个模组" << endl;
		cout << "他们分别是：" << endl;
		for (unsigned int i = 0; i < modactname.size(); i++)
		{
			cout << modactname[i].substr(0,modactname[i].length() - 1) << endl;
		}





}
int main() {
	system("chcp 65001");
	cout << "Python版本因为有问题，故切换为C语言版本" << endl;
	cout << "本C语言版本由松子制作，不保证完全可用" << endl;
	cout << "☆接下来会启动启动器，在这个框下会显示新注册的模组！" << endl;
	cout << "目前暂时不支持压缩包内的模组。" << endl;
	cout << "……正在读取模组文件夹……请将MOD文件放到游戏目录的mod文件夹内" << endl;
	checkpath("mod");
	//简单加载一下，从而适配更多游戏
	fstream game;
	game.open("gamename.txt", ios::in);
	if (!game)
	{
		cout << "读取gamename.txt失败，默认为群星加载MOD" << endl;
		gamename = "Stellaris";
	}
	getline(game, gamename);
	cout << "当前为"<< gamename << "加载mod" << endl;
	game.close();
	//加载模组功能
	loadmod();
	system("pause");
	//	cout << getDocument() << endl;
	return 0;
}
