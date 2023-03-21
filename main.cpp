/*
    仿照base64的原理制作的一个能加密且能将文件转换为字符的程序
    用于文件文字型存储，加密，传输等方向
    还能拿来练手
*/
#include <iostream>
#include <string>
#include "FileSecret.h"

int main(int argc, char* argv[])
{
    //先要求输入相对位置，方便使用，和开源项目SCC一样，后期再支持参数等方式
    // std::cout << "输入文件名（在同文件夹输入相对位置）：";
    // string file_dir;
    // std::cin >> file_dir;

    FileSecret fs("encode_bg.jpg", FILE_DECODE);
    fs.run();

    return 0;
}