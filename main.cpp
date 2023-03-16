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

string QUEEN = "`1234567890-=~!@#$%^&*()_+qwertyuiop[]asdfghjkl;zxcvbnmQWERTYUIO";

//传入数据data，r是一个3个字节的指针，为改函数的返回值
void byte2char(char data[3], char *r)//返回数组大小为4
{
    unsigned int a = (unsigned int)((int)data[0] + 128) + (unsigned int)((int)data[1] + 128) * 256 + (unsigned int)((int)data[2] + 128) * 256 * 256;
	unsigned int b1 = a % 64;   //得到4个0-63的数字
	unsigned int b2 = ((a - b1) / 64) % 64;
	unsigned int b3 = (a - b1 - 64 * b2) / (64 * 64) % 64;
	unsigned int b4 = (a - b1 - 64 * b2 - 64 * 64 * b3) / (64 * 64 * 64) % 64;
    //char r[4];
    r[0] = QUEEN[b1];
    r[1] = QUEEN[b2];
    r[2] = QUEEN[b3];
    r[3] = QUEEN[b4];

}

//传入数据data，r是一个3个字节的指针，为改函数的返回值(还没增加代码健壮性)
void char2byte(char data[4], char *r)//返回数组大小为3
{
    unsigned int b1 = QUEEN.find(data[0]);
    unsigned int b2 = QUEEN.find(data[1]);
    unsigned int b3 = QUEEN.find(data[2]);
    unsigned int b4 = QUEEN.find(data[3]);
    unsigned a = b1 + b2 * 64 + b3 * 64 * 64 + b4 * 64 * 64 * 64;
    int a1 = a % 256;
	int a2 = ((a - a1) / 256) % 256;
	int a3 = (a - a1 - 256 * a2) / (256 * 256) % 256;

    //char r[3];
    r[0] = a1 - 128;
    r[1] = a2 - 128;
    r[2] = a3 - 128;
}

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