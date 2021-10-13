// provoc.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <tchar.h>
#include <Windows.h>
#include <conio.h>  
#pragma warning(disable:4996)
using namespace std;
int last_position = 0;//记录当前已经处理掉的文件位置
int flags[10] = { 0 };

const TCHAR* pStrPipeName = L"\\\\.\\pipe\\Name_pipe_demon_get";
const TCHAR* pStrPipeNames = L"\\\\.\\pipe\\Name_pipe_demon_gets";
string path = "";
const int BUFFER_MAX_LEN = 1024;
char buf[BUFFER_MAX_LEN];
HANDLE hPipe = NULL;
HANDLE hPipes = NULL;

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
            printf("60x\n");
            swrite('6');
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
    return 0;
}


