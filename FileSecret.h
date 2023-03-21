#include <iostream>
#include <string>
#include <fstream>
#include "bocode64.h"

#define FILE_ENCODE 10001
#define FILE_DECODE 10002
#define ENCODE_BUFFER_SIZE 6144
#define DECODE_BUFFER_SIZE 8192

using string = std::string;

class FileSecret
{
private:
    string file_dir;         // 文件路径
    std::fstream in_stream;  // 文件输入流
    std::fstream out_stream; // 文件输出流
    int mode;                // 10001编码，10002解码

    long file_length;

    void init()
    {

        if (mode == FILE_ENCODE) // 二进制进，文本出
        {
            in_stream.open(file_dir, std::ios::in | std::ios::binary);
            out_stream.open("encode_" + file_dir, std::ios::out);
        }
        else // 文本进，二进制出
        {
            in_stream.open(file_dir, std::ios::in);
            out_stream.open("decode_" + file_dir, std::ios::out | std::ios::binary);
        }
    }

    void encode()
    {
        // 缓冲池
        char *chs = new char[ENCODE_BUFFER_SIZE];
        // 处理池
        char *outch = new char[ENCODE_BUFFER_SIZE * 4 / 3];
        //尾部数据,下表的位数表示文件在尾增加了几个零
        char t = getCharAt(0);
        // 剩余未读大小
        int unread_size;
        //写入文件头，8个字节，long型数据
        //out_stream.write((char*)&file_length, sizeof(file_length));
        //不能用上面这种方法写，又变成二进制文件了
        // done为已经处理的数据，每次循环处理buffer字节的数据，
        for (long done = 0; done < file_length; done += ENCODE_BUFFER_SIZE)
        {
            unread_size = file_length - done;
            if (unread_size < ENCODE_BUFFER_SIZE)
            {
                // 如果小于buffer则不一定是整三的倍数，补零处理
                if (unread_size % 3 == 0)
                {
                    in_stream.read(chs, unread_size);
                    for (int j = 0; j < unread_size / 3; j++)
                    {
                        byte2char(chs + (3 * j), outch + (4 * j));
                    }
                    out_stream.write(outch, unread_size * 4 / 3);
                    //得到尾部数据
                    t = getCharAt(0);
                }
                else
                {
                    in_stream.read(chs, unread_size);
                    //得到尾部数据
                    t = getCharAt(3 - (unread_size % 3));
                    for (int j = 0; j < 3 - (unread_size % 3); j++)
                    {
                        chs[unread_size + j] = 0; // 后面的位置零
                    }
                    long v_size = unread_size + 3 - (unread_size % 3); // 虚拟大小，后部补到3的整数位
                    for (int j = 0; j < v_size / 3; j++)
                    {
                        byte2char(chs + (3 * j), outch + (4 * j));
                    }
                    out_stream.write(outch, v_size * 4 / 3);

                }
            }
            else
            {
                in_stream.read(chs, ENCODE_BUFFER_SIZE);
                // 三个字节的头指针
                for (int j = 0; j < ENCODE_BUFFER_SIZE / 3; j++)
                {
                    byte2char(chs + (3 * j), outch + (4 * j));
                }
                out_stream.write(outch, ENCODE_BUFFER_SIZE * 4 / 3);
                //得到尾部数据
                t = getCharAt(0);
            }
            
        }
        out_stream.write(&t, 1);
    }

    void decode()
    {
        // 缓冲池
        char *chs = new char[DECODE_BUFFER_SIZE];
        // 处理池
        char *outch = new char[DECODE_BUFFER_SIZE * 3 / 4];
        // 尾部数据
        char *t = new char[1];
        //应该去掉的数据
        int desize = 0;
        // 剩余未读大小
        int unread_size;
        // done为已经处理的数据，每次循环处理buffer字节的数据，
        for (long done = 0; done < file_length; done += DECODE_BUFFER_SIZE)
        {
            unread_size = file_length - done;
            if (unread_size < DECODE_BUFFER_SIZE)
            {
                in_stream.read(chs, unread_size - 1);
                in_stream.read(t, 1);
                desize = getDesize(*t);
                for(int j = 0; j < unread_size / 4; j++)//整型除法吞掉最后一位
                {
                    char2byte(chs + (4 * j), outch + (3 * j));
                }
                out_stream.write(outch, (unread_size - 1) * 3 / 4 - desize);
            }
            else
            {
                in_stream.read(chs, DECODE_BUFFER_SIZE);
                // 三个字节的头指针
                for (int j = 0; j < DECODE_BUFFER_SIZE / 4; j++)
                {
                    char2byte(chs + (4 * j), outch + (3 * j));
                }
                if(unread_size == DECODE_BUFFER_SIZE + 1)
                {
                    in_stream.read(t, 1);
                    desize = getDesize(*t);
                    done++;
                }
                out_stream.write(outch, DECODE_BUFFER_SIZE * 3 / 4 - desize);
            }
        }
    }

public:
    FileSecret(string file_dir, int mode) // 10001编码，10002解码
    {
        if (mode != FILE_DECODE && mode != FILE_ENCODE)
            exit(1); // 错误代码1：模式错误
        this->file_dir = file_dir;
        this->mode = mode;
        init();
    }

    // 开始加密解密
    void run()
    {
        if (mode == FILE_ENCODE) // 二进制进，文本出
        {
            in_stream.seekg(0, in_stream.end);
            file_length = in_stream.tellg();
            in_stream.seekg(0, in_stream.beg);
            encode();
        }
        else // 文本进，二进制出
        {
            in_stream.seekg(0, in_stream.end);
            file_length = in_stream.tellg();
            in_stream.seekg(0, in_stream.beg);
            decode();
        }
    }

    ~FileSecret()
    {
        in_stream.close();
        out_stream.close();
    }
};