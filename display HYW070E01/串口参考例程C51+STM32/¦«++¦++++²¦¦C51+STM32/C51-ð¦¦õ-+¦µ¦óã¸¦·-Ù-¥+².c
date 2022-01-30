//把数值0x0001写入变量存储器0x0010单元
// 
#include <reg51.h>
#include <string.h>

void init_serialcom( void ) //串口通信初始设定
{
TMOD=0x20;     //定时器1工作于8位自动重载模式, 用于产生波特率
TH1=0xFD;     //波特率9600
TL1=0xFD;
SCON=0x50;     //设定串行口工作方式   
TR1=1;  
}

//向串口发送一个字符
void send_char_com( unsigned char ch)
{SBUF=ch;
while (TI== 0);
TI= 0 ;
}


main()
{
init_serialcom(); //初始化串口
//把数值0x0001写入变量存储器0x0010单元
send_char_com(0x5A);
send_char_com(0xA5);
send_char_com(0x05);
send_char_com(0x82);
send_char_com(0x00);
send_char_com(0x10);
send_char_com(0x00);
send_char_com(0x01);

while(1);
}