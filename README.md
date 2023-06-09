# 加密文本转换
程序可以像base64一样对文件进行保密并将其显示为文本

![](https://img.shields.io/badge/release-1.0.0-green)

语言/language/語言：  [English](README-EN.md)  简体中文  [繁體中文](README-CHT.md)  [日本語](README-JP.md)

## 一、这是什么？

<font color="red">完善中</font>

## 二、原理

## 三、如何使用

## 三、过程探索

### 3.1 初期（2023.3.10-2023.3.21）

这段时间里，基本探索了程序的可行性，因为体量较小，没有做任何软件工程分析（*毕竟即时兴起*）。但是，有了之前开发SCC加密工具的经验，我想这次的文本转换应该也不会太难。

`3.13`：初步探索出算法，我将键盘上能轻松打出来的ASCII字符排成一排，一共94个字符，一开始我设想可以用这94个字符的组合表述多个字节，但显然这种方案虽然可行，但十分笨拙。虽然比起实际使用的方法，用94个字符共同编码生成的文件一定更小，但是94这个数字太不巧了，众所周知我们应当整个字节读入，为8个bit，能表示256个不同的值显然我们应该让最终情况数是2的n次方，因为这样更容易找到最小公倍数，方便数据之间进行转换。虽然94个字符也可以通过字符映射表来实现，但显然查表会浪费大量的时间，即便采用哈希表也不是明智的选择。而且显然这个映射表会十分庞大，因此读入内存会占据较多的内存，所以我们选择了$2^6=64$个字符来表示，这样的话可以用6和8的最小公倍数24个bit即3字节对应4个字符。

`3.15`：关于字符字典的选择，我们在最初版中给出了一个字符数组，后续我们可以修改这个数组以达到自定义字典的效果，可以通过这种方式，将字典视为密码，一并发送，也可以通过输入密码，由程序通过某种算法生成一个64位（ASCII）字符数组，来达到密码加密的效果。由于编码结果是纯ASCII字符，所以甚至还可以手动修改一部分。

`3.17`：写完了编码解码的核心部分，且基本完成了文件编码的内容，应该不会提供上层应用如CSS嵌入等，因为我也说过，这个就像base64一样，甚至我今天查了一下，base64也是选了64个字符，区别就是我们对于不整3个字节的文件的做法是补零处理，base64不是。也能对base64已经是一套成熟的体系了，我不认为我作为一个初出茅庐的大学生能比他们的代码更好用，更健壮，效率更高。（ps.==但后就不一定了==）

`3.20`：本来打算在文件头加上文件大小以此判断文件最后的补零是否为本来就有的，实际操作发现直接写入long型数据则文件就会变成二进制文件，就没有意义了。在仔细思考后发现，既然是编码生成的文件，那肯定是符合规范啊，我们可以在文件末尾加上1个字符带去是否补了零，补了几个零的信息。

