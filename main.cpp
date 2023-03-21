/*
    仿照base64的原理制作的一个能加密且能将文件转换为字符的程序
    用于文件文字型存储，加密，传输等方向
    还能拿来练手
*/
#include <iostream>
#include <string>
#include "FileSecret.h"

using namespace std;

int main(int argc, char *argv[])
{
    // 先要求输入相对位置，方便使用，和开源项目SCC一样，后期再支持参数等方式
    cout << "输入文件名（在同文件夹输入相对位置）：";
    string file_dir;
    cin >> file_dir;
    int func;
    cout << "请选择  1.编码   2.解码\n选择：";
    int s;

    switch (s)
    {
    case 1:
        func = FILE_ENCODE;
        break;
    case 2:
        func = FILE_DECODE;
        break;
    default:
        exit(2);
    }

    FileSecret fs(file_dir, func);
    fs.run();

    return 0;
}