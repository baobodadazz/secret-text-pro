#include <iostream>
#include <string>
#include <fstream>
#include "bocode64.h"

#define FILE_ENCODE 10001
#define FILE_DECODE 10002
#define ENCODE_BUFFER_SIZE 6144

using string = std::string;

class FileSecret
{
private:
    string file_dir;    //文件路径
    std::fstream in_stream;     //文件输入流
    std::fstream out_stream;    //文件输出流
    int mode;//10001编码，10002解码

    long file_length;

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
        //缓冲池
        char* chs = new char[ENCODE_BUFFER_SIZE];
        //处理池
        char *outch = new char[ENCODE_BUFFER_SIZE * 4 / 3];
        //剩余未读大小
        int unread_size;
        //done为已经处理的数据，每次循环处理buffer字节的数据，
        for(long done = 0; done < file_length; done += ENCODE_BUFFER_SIZE)
        {
            unread_size = file_length - done;
            if(unread_size < ENCODE_BUFFER_SIZE)
            {
                //如果小于buffer则不一定是整三的倍数，补零处理（以后再写）
                if(unread_size % 3 == 0)
                {
                    in_stream.read(chs, unread_size);
                    for(int j = 0; j < unread_size / 3; j++)
                    {
                        byte2char(chs + (3 * j), outch + (4 * j));
                    }
                    out_stream.write(outch, unread_size * 4 / 3);
                }
                else
                {
                    in_stream.read(chs, unread_size);
                    for(int j = 0; j < 3- (unread_size % 3); j++)
                    {
                        chs[unread_size + j] = 0;//后面的两位置零
                    }
                    long v_size = unread_size + 3 - (unread_size % 3);//虚拟大小，后部补到3的整数位
                    for(int j = 0; j < v_size / 3; j++)
                    {
                        byte2char(chs + (3 * j), outch + (4 * j));
                    }
                    out_stream.write(outch, v_size * 4 / 3);
                }
            }
            else
            {
                in_stream.read(chs, ENCODE_BUFFER_SIZE);
                //三个字节的头指针
                for(int j = 0; j < ENCODE_BUFFER_SIZE / 3; j++)
                {
                    byte2char(chs + (3 * j), outch + (4 * j));
                }
                out_stream.write(outch, ENCODE_BUFFER_SIZE * 4 / 3);
            }
        }
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
            in_stream.seekg(0, in_stream.end);
	        file_length = in_stream.tellg();
	        in_stream.seekg(0, in_stream.beg);
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