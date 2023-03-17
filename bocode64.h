#include <string>
#include <iostream>

using namespace std;

string QUEEN = "`1234567890-=~!@#$%^&*()_+qwertyuiop[]asdfghjkl;zxcvbnmQWERTYUIO";

//传入数据data，r是一个3个字节的指针，为改函数的返回值
void byte2char(char *data, char *r)//返回数组大小为4
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
void char2byte(char *data, char *r)//返回数组大小为3
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