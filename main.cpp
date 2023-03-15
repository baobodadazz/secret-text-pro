/*
    仿照base64的原理制作的一个能加密且能将文件转换为字符的程序
    用于文件文字型存储，加密，传输等方向
    还能拿来练手
*/
#include <iostream>
#include <string>
#include <fstream>

#define FILE_ENCODE 10001
#define FILE_DECODE 10002

using string = std::string;

char char_set[] = {'a','s'};//正在求解算法

class FileSecret
{
private:
    string file_dir;    //文件路径
    std::fstream in_stream;     //文件输入流
    std::fstream out_stream;    //文件输出流
    int mode;//10001编码，10002解码

    void init()
    {
        

        if(mode == FILE_ENCODE)//二进制进，文本出
        {
            in_stream.open(file_dir, std::ios::in|std::ios::binary);
            out_stream.open("encode_" + file_dir, std::ios::out);
        }
        else//文本进，二进制出
        {
            in_stream.open(file_dir, std::ios::in);
            out_stream.open("decode_" + file_dir, std::ios::out|std::ios::binary);
        }

        
    }

    void encode()
    {

    }

    void decode()
    {

    }
public:
    FileSecret(string file_dir, int mode)//10001编码，10002解码
    {
        if(mode != FILE_DECODE && mode != FILE_ENCODE)
            exit(1);    //错误代码1：模式错误
        this->file_dir = file_dir;
        this->mode = mode;
        init();
    }

    //开始加密解密
    void run()
    {
        if(mode == FILE_ENCODE)//二进制进，文本出
        {
            encode();
        }
        else//文本进，二进制出
        {
            decode();
        }
    }

    ~FileSecret()
    {
        in_stream.close();
        out_stream.close();
    }

};

int main()
{
    //先要求输入相对位置，方便使用，和开源项目SCC一样，后期再支持参数等方式
    std::cout << "输入文件名（在同文件夹输入相对位置）：";
    string file_dir;
    std::cin >> file_dir;

    return 0;
}