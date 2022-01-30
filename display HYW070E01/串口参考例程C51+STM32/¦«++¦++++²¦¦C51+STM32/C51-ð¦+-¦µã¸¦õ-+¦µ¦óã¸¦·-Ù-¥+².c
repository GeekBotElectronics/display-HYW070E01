#include "reg52.h"			 //此文件中定义了单片机的一些特殊功能寄存器
#include <string.h>

typedef unsigned int uint16_t;	  //对数据类型进行声明定义
typedef unsigned char uint8_t;

#define USER_R3               0xA5  //帧头首字节
#define USER_RA               0x5A  //帧头次字节
#define array_length          10   //定义数组长度 00

void UsartInit();                      //串口初始化
void send_char_com(uint8_t ch);       //向串口发送一个字符
void delay(uint16_t ms_number);       // ms延时函数

void write_register_80_1byte(uint8_t address,uint8_t send_data);                               //0x80指令-写一个寄存器单元   
void write_register_80_2byte(uint8_t address,uint8_t data1,uint8_t data2);                      //0x80指令-连续写两个寄存器单元
void write_multiple_register_80(uint8_t address,uint8_t data_length,uint8_t *send_data);       //0x80指令-连续写多个寄存器单元

void write_variable_store_82_1word(uint16_t address,uint16_t send_data);                           //0x82指令-写一个变量存储单元
void write_variable_store_82_2word(uint16_t address,uint16_t data1,uint16_t data2);                //0x82指令-连续写两个变量存储单元
void write_multiple_variable_store_82(uint16_t address,uint8_t data_length,uint16_t *send_data);   //0x82指令-连续写多个变量存储单元

void main()
{	
   uint8_t a1[array_length];
   uint16_t a2[array_length];
	
	UsartInit();   //	串口初始化

	while(1)
   {   
      write_register_80_1byte(0X01,0x0F);            //控制屏幕亮度，01单元写入0F
      delay(1000);  
      write_register_80_1byte(0x01,0x3F);            //控制屏幕亮度，01单元写入3F
      delay(1000);  		
      write_register_80_1byte(0x02,0xC8);           //控制蜂鸣器响2秒，02单元写入C8
      delay(1000);  	
      write_register_80_2byte(0x03,0x00,0x02);      //切换到2号界面，03、04单元写入00、02
      delay(1000);
		
      a1[0]=0x00;
      a1[1]=0x03;
      write_multiple_register_80(0x03,2,a1);        //切换到3号界面，03、04单元写入00、03
      delay(1000);
		
      write_variable_store_82_1word(0x1000,0x55aa);          //向0x1000单元里写入数据0x55aa
      write_variable_store_82_2word(0x2000,0x1122,0x3344);   //向0x2000、0x2001单元里写入数据0x1122、0x3344
      
      a2[0]=0x0001;
      a2[1]=0x0002;
      write_multiple_variable_store_82(0x3000,2,a2);         //向0x3000、0x3001单元里写入数据0x0001、0x0002
	   delay(1000);
      
      write_register_80_2byte(0x03,0x00,0x01);               //切换到1号界面，03、04单元写入00、01			
	   delay(1000);
    }	
}

////晶振为12MHz,波特率设置为4800，千分之1.4
//void UsartInit()        //串口初始化
//{

//SCON=0x50;     //设定串行口工作方式   
//TMOD=0x20;     //定时器1工作于8位自动重载模式, 用于产生波特率
//PCON=0X80;		//SMOD=1，波特率加倍
//TH1=0xF3;     //波特率4800
//TL1=0xF3;
//TR1=1; 
//}

//晶振为11.0592MHz,波特率设置为9600，
void UsartInit()        //串口初始化
{

SCON=0x50;     //设定串行口工作方式   
TMOD=0x20;     //定时器1工作于8位自动重载模式, 用于产生波特率
PCON=0X00;		//SMOD=0，波特率不加倍
TH1=0xFd;     //波特率9600
TL1=0xFd;
TR1=1; 
}


////向串口发送一个字符
void send_char_com( unsigned char ch)
{
	SBUF=ch;
   while (TI==0);
   TI=0;
}


//延时函数 
 void delay(uint16_t ms_number) 
  {
   uint16_t i;
   uint16_t j;
   for(i=0;i<ms_number;i++)
    {
     for(j=0;j<200;j++);
    } 
  }

//0x80指令-写一个寄存器
//地址范围为0x00~0xff,所以address是8位的
//参考开发指南-2.2寄存器读写指令 0x80 、0x81
void write_register_80_1byte(uint8_t address,uint8_t send_data)    
 {
   uint8_t i;
   uint8_t array[array_length];
	 
   array[0]=USER_R3;
   array[1]=USER_RA; 
   array[2]=0x03;                //指令长度
   array[3]=0x80;
   array[4]=address;
   array[5]=send_data;          //数值 

   for(i=0;i<6;i++)             //通过串口一发送指令
    {
      send_char_com(array[i]);
    } 
 }

//0x80指令-连续写两个寄存器单元 
void write_register_80_2byte(uint8_t address,uint8_t data1,uint8_t data2)                 
 {
   uint8_t i;
   uint8_t array[array_length];
	 
   array[0]=USER_R3;           
   array[1]=USER_RA;          
   array[2]=0x04;              //指令长度
   array[3]=0x80;
   array[4]=address;
   array[5]=data1;             //数值1
   array[6]=data2;             //数值2

	 
   for(i=0;i<7;i++)            //通过串口一发送指令
    {
      send_char_com(array[i]);
    }
 }


//0x80指令-写多个寄存器
//地址范围为0x00~0xff,所以address是8位的
//参考开发指南-2.2寄存器读写指令 0x80 、0x81
void write_multiple_register_80(uint8_t address,uint8_t data_length,uint8_t *send_data)    
 {
   uint8_t i,nDataLen;
   uint8_t array[array_length];
	 
   array[0]=USER_R3;
   array[1]=USER_RA;
   array[2]=1+1+data_length;   //命令占一字节，地址占一字节，再加上数据长度
   array[3]=0x80;
   array[4]=address;
	 
   for(i=0;i<data_length;i++)
    {
      array[5+i]=send_data[i]; 
    }
	 
   nDataLen=array[2]+3;        //有效命令的长度
	 
   for(i=0;i<nDataLen;i++)     //通过串口一发送指令
    {
      send_char_com(array[i]);
    }
 }

//0x82指令-写一个变量存储单元
void write_variable_store_82_1word(uint16_t address,uint16_t send_data)     
{
   uint8_t i;
   uint8_t array[array_length];
	 
   array[0]=USER_R3;
   array[1]=USER_RA;
   array[2]=1+2+2;                   //命令占1字节，地址占2字节，再加上数据长度2字节
   array[3]=0x82;
   array[4]=(address&0xFF00)>>8;     //取地址的高8位
   array[5]=address&0x00FF;          //取地址的低8位
   array[6]=(send_data&0xFF00)>>8;   //取数据的高8位
   array[7]=send_data&0x00FF;        //取数据的低8位
	 
   for(i=0;i<8;i++)                  //通过串口一发送指令
    {
      send_char_com(array[i]);
    }
 }


//0x82指令-连续写两个变量存储单元                    
void write_variable_store_82_2word(uint16_t address,uint16_t data1,uint16_t data2)           
{
   uint8_t i;
   uint8_t array[array_length];
	 
   array[0]=USER_R3;
   array[1]=USER_RA;
   array[2]=1+2+4;                   //命令占1字节，地址占2字节，再加上数据长度4字节
   array[3]=0x82;
   array[4]=(address&0xFF00)>>8;     //取地址的高8位
   array[5]=address&0x00FF;          //取地址的低8位
   array[6]=(data1&0xFF00)>>8;       //取数据的高8位
   array[7]=data1&0x00FF;            //取数据的低8位
   array[8]=(data2&0xFF00)>>8;       //取数据的高8位
   array[9]=data2&0x00FF;            //取数据的低8位
   	 
   for(i=0;i<10;i++)                 //通过串口一发送指令
    {
      send_char_com(array[i]);
    }
 } 
 
  
//0x82指令-连续写多个变量存储单元
//地址范围为0x0000~0xffff,所以address是16位的
//数据内容是2N字节，因为每个变量存储单元包含2个字节。
//参考开发指南-2.3变量存储器读写指令 0x82 、0x83
void write_multiple_variable_store_82(uint16_t address,uint8_t data_length,uint16_t *send_data)    
 {
   uint8_t i,nDataLen;
   uint8_t array[array_length];
	 
   array[0]=USER_R3;
   array[1]=USER_RA;
   array[2]=1+2+data_length*2;                //命令占1字节，地址占2字节，再加上数据长度(数据是双字节)
   array[3]=0x82;
   array[4]=(address&0xFF00)>>8;              //取地址的高8位
   array[5]=address&0x00FF;                   //取地址的低8位

   for(i=0;i<data_length;i++)
    {
      array[6+2*i]=(send_data[i]&0xFF00)>>8;  //取数据的高8位
      array[7+2*i]=send_data[i]&0x00FF;       //取数据的低8位
    }

   nDataLen=array[2]+3;                       //有效命令的长度
	 
   for(i=0;i<nDataLen;i++)                    //通过串口一发送指令
    {
      send_char_com(array[i]);
    } 
 }
