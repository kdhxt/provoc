// provoc.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <tchar.h>
#include <Windows.h>
#include <conio.h>  
#include <vector>
#include <algorithm>

#pragma warning(disable:4996)
using namespace std;

#define foreach(i,n) for(int i=0;i<n;i++)
int last_position = 0;//记录当前已经处理掉的文件位置
int flags[10] = { 0 };

const TCHAR* pStrPipeName = L"\\\\.\\pipe\\Name_pipe_demon_get";
const TCHAR* pStrPipeNames = L"\\\\.\\pipe\\Name_pipe_demon_gets";
string path = "";
string pathst = "";
const int BUFFER_MAX_LEN = 1024;
char buf[BUFFER_MAX_LEN];
HANDLE hPipe = NULL;
HANDLE hPipes = NULL;

struct node {
    int f, r;//row-col  | first rear
    string nm;//name | errors
    node(int ff = -1, int rr = -1, string nmi = "") {
        this->f = ff;
        this->r = rr;
        this->nm = nmi;
    }
    bool operator < (const node& a)const {
        return f < a.f;
    }
};
vector<node>sp;
vector<node>errmsg;
vector<node>exterrmsg;

string maps[10] = { "0","1","2","3","4","5","6","7","8","9" };
string Str2Cent(int k) {
    string ret = "";
    int tmp = k;
    while (1) {
        ret += maps[(tmp % 10)];
        tmp /= 10;
        if (!tmp)break;
    }
    int l = ret.size();
    for (int i = 0; i < l / 2; i++) {
        ret[i] ^= ret[l - i - 1] ^= ret[i] ^= ret[l - i - 1];
    }
    return ret;
}

int Cnet2Str(string str) {
    int l = str.size();
    int ret = 0;
    for (int i = 0; i < l; i++) {
        ret = ret * 10 + (str[i] - '0');
    }
    return ret;
}

void swrite(char c) {
    DWORD dwLen = 0;
    buf[0] = c;
    //向服务端发送数据
    if (WriteFile(hPipe, buf, 1, &dwLen, NULL)) {
        printf("数据写入完毕共%d字节 %c\n", dwLen,c);
    }
    else{
        printf("数据写入失败\n");
    }
}

void swrites(char c) {
    DWORD dwLen = 0;
    buf[0] = c;
    //向服务端发送数据
    if (WriteFile(hPipes, buf, 1, &dwLen, NULL)) {
        printf("数据写入完毕共%d字节 %c\n", dwLen, c);
    }
    else {
        printf("数据写入失败\n");
    }
}

void swriterr(string err) {
    int le = err.size();
    for (int i = 0; i < le; i++) {
        buf[i] = err[i];
    }
    buf[le] = '\0';
    DWORD dwLen = 0;
    if (WriteFile(hPipe, buf, le, &dwLen, NULL)) {
        printf("数据写入完毕共%d字节 %s\n", dwLen, buf);
    }
    else {
        printf("数据写入失败\n");
    }
}

int pflag = 0;
int prost = 1e9+7;
void splitST() {
    ifstream finst(pathst);
    //按行 | first串 follow串
    string lineo;
    int f, r;
    int cnt = 0;
    string cn = "";
    while (getline(finst, lineo)) {
        cnt++;
        int ls = lineo.size();
        //END_VAR
        if (lineo.substr(0, 7) == "END_VAR") {
            f = cnt;
            continue;
        }

        //"PROGRAM"
        if (ls < 9)continue;
        if (lineo.substr(0, 7) == "PROGRAM") {
            if (!pflag) {
                prost = cnt;
                pflag = 1;
            }
            f = cnt;
            cn = lineo.substr(8, ls - 8);
            continue;
        }
        //END_PROGRAM
        if (lineo.substr(0, 11) == "END_PROGRAM") {
            r = cnt;
            sp.push_back(node(f, r, cn));
            f = r = 0;
            cn = "";
            continue;
        }

        //"FUNCTION_BLOCK"
        if (ls < 15)continue;
        if (lineo.substr(0, 14) == "FUNCTION_BLOCK") {
            f = cnt;
            cn = lineo.substr(8, ls - 8);
            continue;
        }
        //END_FUNCTION_BLOCK
        if (lineo.substr(0, 18) == "END_FUNCTION_BLOCK") {
            r = cnt;
            sp.push_back(node(f, r, cn));
            f = r = 0;
            cn = "";
            continue;
        }
    }
    cout << "startss" << endl;
    for (int i = 0; i < sp.size(); i++) {
        cout << sp[i].f << ' ' << sp[i].r << ' ' << sp[i].nm << endl;
    }
    cout << "endss" << endl;
}

void remakerr(string path) {
    ifstream fin(path);
    string lineo;
    string lastline;
    int sw = 1;
    while (getline(fin, lineo)) {
        int ls = lineo.size();
        int comma[10];
        int bar = 0;
        int dot = 0;
        int op = 0; 
        int i = 0;
        for (i = 2; i < ls; i++) {
            if (lineo[i] == ':') {
                comma[op++] = i;
                if (op == 2)break;
            }
            if (lineo[i] == '-' && op == 1 && (!bar)) {
                bar = i;
            }
            if (lineo[i] == '.' && op == 1 && (!dot)) {
                dot = i;
            }
        }
        //' '
        if (op == 2 && dot && bar) {
            int row = Cnet2Str(lineo.substr(comma[0] + 1, bar - 1 - comma[0] - 1 + 1));
            int col = Cnet2Str(lineo.substr(bar + 1, dot - 1 - bar - 1 + 1));
            int fe = i - 1;
            for (; i < ls; i++) {
                if (lineo[i] == ' ') {
                    fe = i;
                    break;
                }
            }
            node ins = node(row, col, lineo.substr(fe + 1, ls - 1 - fe - 1 + 1));
            errmsg.push_back(ins);
            continue;
        }
        else{//无行，无列的信息
            node ins = node(-1, -1, lineo);
            exterrmsg.push_back(ins);
        }
        lastline = lineo;
    }
    cout << "START" << endl;
    for (int i = 0; i < errmsg.size(); i++) {
        cout << errmsg[i].f << ' ' << errmsg[i].r << ' ' << errmsg[i].nm << endl;
    }
    cout << "=============" << endl;
    for (int i = 0; i < exterrmsg.size(); i++) {
        cout << exterrmsg[i].f << ' ' << exterrmsg[i].r << ' ' << exterrmsg[i].nm << endl;
    }
    cout << "END" << endl;
}

void printerr() {
    //二分 | 已经保证sp单调
    int spl = sp.size();
    int emgl = errmsg.size();
    int eemgl = exterrmsg.size();
    for (int i = 0; i < emgl; i++) {
        int ite = lower_bound(sp.begin(), sp.end(), errmsg[i]) - sp.begin();
        if (ite != spl) {
            string errors = "ERROR#";
            if (ite == 0 || errmsg[i].f >= sp[ite - 1].r) {//首行或者是非首行定义
                if (ite == 0 && errmsg[i].f < prost) {//首行且不是定义
                    errors = "";
                    errors += errmsg[i].nm;
                }
                else {
                    errors += (sp[ite].nm + "#");//首行定义后者非首行定义
                    errors += errmsg[i].nm;
                }
            }
            else if (errmsg[i].f < sp[ite - 1].r) {
                errors += (sp[ite - 1].nm + "#");
                string rows = Str2Cent(errmsg[i].f - sp[ite - 1].f);
                string cols = Str2Cent(errmsg[i].r);
                errors += "(" + rows;
                errors += ",";
                errors += cols + ")#Description of ";
                errors += errmsg[i].nm;
            }
            //管道通信
            swriterr(errors);
        }
        else {
            string errors = "ERROR#";
            if (ite > 0 && errmsg[i].f < sp[ite - 1].r) {//ite>0防止空情况出现异常
                errors += (sp[ite - 1].nm + "#");
                string rows = Str2Cent(errmsg[i].f - sp[ite - 1].f);
                string cols = Str2Cent(errmsg[i].r);
                errors += "(" + rows;
                errors += ",";
                errors += cols + ")#Description of ";
                errors += errmsg[i].nm;
            }
            else {
                errors = "";
                errors += errmsg[i].nm;
            }
            //管道通信
            swriterr(errors);
        }
    }
    for (int i = 0; i < eemgl; i++) {
        //管道通信
        swriterr(exterrmsg[i].nm);
    }
    swriterr("66");
}

void find_last_line()
{   
    //setvbuf(stdout, (char*)NULL, _IONBF, 0);
    //printf("loadSucess6");
    if (!WaitNamedPipe(pStrPipeName, NMPWAIT_WAIT_FOREVER))
    {
        printf("Error! 连接Get失败\n");
        return;
    }
    hPipe = CreateFile(pStrPipeName, GENERIC_WRITE, 0,
        NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    int cnts = 0;
    while (1) {
        //last_position = 0;
        ifstream infile("ac.txt",ios::app);
        ifstream infilerr(path, ios::app);
        /*infile.seekg(0, ios::end);
        long long filesize = infile.tellg();
        if (filesize == 0 || filesize == -1) {
            continue;
        }*/
        char cur='#';
        //infile.seekg(last_position, ios::beg);//缓冲区误我
        if (infile >> cur);
        if (cur == '1' && flags[1] == 0) {
            flags[1] = 1;
            printf("10x\n");
            swrite(cur);
        }
        if (cur == '2' && flags[2] == 0) {
            flags[2] = 1;
            printf("20x\n");
            swrite(cur);
        }
        if (cur == '3' && flags[3] == 0) {
            flags[3] = 1;
            printf("30x\n");
            swrite(cur);
        }
        if (cur == '4' && flags[4] == 0) {
            flags[4] = 1;
            printf("40x\n");
            swrite(cur);
        }
        if (cur == '5' && flags[5] == 0) {
            flags[5] = 1;
            printf("50x\n");
            swrite(cur);
            break;
        }
        infilerr.seekg(0, ios::end);
        long long filesize = infilerr.tellg();
        if (filesize>=2) {
            flags[6] = 1;
            //打印错误开始符号
            swrite('6');
            printf("60x\n");
            //读.st文件tmp
            splitST();
            //分析eception.log文件
            infilerr.seekg(0, ios::beg);
            infilerr.close();
            Sleep(999);
            remakerr(path);
            //重定位错误信息并管道打印错误信息
            printerr();
            break;
        }
        //last_position = filesize;
        infile.close();
    }
    CloseHandle(hPipe);
}

void ReadSync() {
    find_last_line();
}

void util(wstring pr,wstring p)
{
    SHELLEXECUTEINFO   sei;
    ZeroMemory(&sei, sizeof(SHELLEXECUTEINFO));
    sei.cbSize = sizeof(SHELLEXECUTEINFO);
    sei.fMask = SEE_MASK_NOCLOSEPROCESS;
    sei.hwnd = NULL;
    sei.lpVerb = NULL;
    sei.lpFile = pr.c_str();
    sei.lpParameters = p.c_str();
    sei.lpDirectory = NULL;
    sei.nShow = SW_HIDE;
    sei.hInstApp = NULL;
    ShellExecuteEx(&sei);
    WaitForSingleObject(sei.hProcess, INFINITE);
    TerminateProcess(sei.hProcess, 0);
    //可以用CreateProcess函数去完成                                                     
}

wstring string2wstring(string str)
{
    wstring result;
    //获取缓冲区大小，并申请空间，缓冲区大小按字符计算  
    int len = MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.size(), NULL, 0);
    TCHAR* buffer = new TCHAR[len + 1];
    //多字节编码转换成宽字节编码  
    MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.size(), buffer, len);
    buffer[len] = '\0';             //添加字符串结尾  
    //删除缓冲区并返回值  
    result.append(buffer);
    delete[] buffer;
    return result;
}

//将wstring转换成string  
string wstring2string(wstring wstr)
{
    string result;
    //获取缓冲区大小，并申请空间，缓冲区大小事按字节计算的  
    int len = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), wstr.size(), NULL, 0, NULL, NULL);
    char* buffer = new char[len + 1];
    //宽字节编码转换成多字节编码  
    WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), wstr.size(), buffer, len, NULL, NULL);
    buffer[len] = '\0';
    //删除缓冲区并返回值  
    result.append(buffer);
    delete[] buffer;
    return result;
}

int main(int argc, char** argv)
{   

    //setvbuf(stdout, (char*)NULL, _IONBF, 0);
    //m1
    if (argc == 1) {
        ifstream fin("comaddr.log");
        string line;
        getline(fin, line);
        //获取输出路径
        int len = line.size();
        for (int i = len - 1; i >= 0; i--) {
            if (line[i] == ' ') {
                path = line.substr(i + 1, len - 1 - i - 1 + 1)+"/EXCEPTION.log";
                string tpath = line.substr(i + 1, len - 1 - i - 1 + 1);
                int lens = tpath.size();
                string suf = "";
                for (int j = lens - 1; j >= 0; j--) {
                    if (tpath[j] == '/') {
                        tpath = tpath.substr(0, j);
                        break;
                    }
                }
                lens = tpath.size();
                for (int j = lens - 1; j >= 0; j--) {
                    if (tpath[j] == '/') {
                        suf = tpath.substr(j + 1, lens - 1 - j - 1 + 1);
                        break;
                    }
                }
                pathst = line.substr(i + 1, len - 1 - i - 1 + 1) + "/../OutputXml/"+suf+".st";
                cout << pathst << endl;
                break;
            }
        }
        ofstream pfout("ac.txt");//清空
        pfout.close();//清空
        ofstream pfouterr(path);//清空
        pfouterr.close();//清空
        WinExec(line.c_str(), 0);
        ReadSync();//等同与阻塞的方式调起进程同时读取共享文件输出
    }
    else if (argc == 2) {
        if (!WaitNamedPipe(pStrPipeNames, NMPWAIT_WAIT_FOREVER))
        {
            printf("Error! 连接Gets失败\n");
            return 0;
        }
        hPipes = CreateFile(pStrPipeNames, GENERIC_WRITE, 0,
            NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
        ifstream fin("comaddrs.log");
        string line;
        wstring lines;
        getline(fin, line);
        lines = string2wstring(line);
        util(L"gcc", lines);//阻塞调起
        swrites('7');

        getline(fin, line);
        lines = string2wstring(line);
        util(L"ld", lines);//阻塞调起
        swrites('8');

        getline(fin, line);
        lines = string2wstring(line);
        util(L"objcopy", lines);//阻塞调起
        swrites('9'); 
        CloseHandle(hPipes);//close pipeh
    }
    else if (argc == 3) {//必须保证是CreatePipe动作已完成比如父子进程调用
        if (!WaitNamedPipe(pStrPipeNames, NMPWAIT_WAIT_FOREVER))
        {
            printf("Error! 连接Gets失败\n");
            return 0;
        }
        hPipes = CreateFile(pStrPipeNames, GENERIC_WRITE, 0,
            NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
        ifstream fin("comaddrs.log");
        string line;
        wstring lines;
        getline(fin, line);
        lines = string2wstring(line);
        util(L"mips64el-ps2-elf-gcc", lines);//阻塞调起
        swrites('7');

        getline(fin, line);
        lines = string2wstring(line);
        util(L"mips64el-ps2-elf-ld", lines);//阻塞调起
        swrites('8');

        getline(fin, line);
        lines = string2wstring(line);
        util(L"mips64el-ps2-elf-objcopy", lines);//阻塞调起
        swrites('9');
        CloseHandle(hPipes);//close pipeh
    }
    //system("pause");
    return 0;
}


