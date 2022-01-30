#include "stm32f10x.h"
#include "delay.h"
#include "usart.h"


//全局变量
#define array_length          100   //定义数组长度 100

void write_register_80_1byte(uint8_t address,uint8_t data);                                //0x80指令-写一个寄存器单元   
void write_register_80_2byte(uint8_t address,uint8_t data1,uint8_t data2);                 //0x80指令-连续写两个寄存器单元
void write_register_80_3byte(uint8_t address,uint8_t data1,uint8_t data2,uint8_t data3);   //0x80指令-连续写三个寄存器单元
void write_multiple_register_80(uint8_t address,uint8_t data_length,uint8_t *data);        //0x80指令-连续写多个寄存器单元

void write_variable_store_82_1word(uint16_t address,uint16_t data);                           //0x82指令-写一个变量存储单元
void write_variable_store_82_2word(uint16_t address,uint16_t data1,uint16_t data2);           //0x82指令-连续写两个变量存储单元
void write_multiple_variable_store_82(uint16_t address,uint8_t data_length,uint16_t *data);   //0x82指令-连续写多个变量存储单元


int main(void)
 {	
   uint8_t a1[array_length];
   uint16_t a2[array_length];  
	 
   delay_init();                                     //延时函数初始化	  
   NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);   //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
   uart_init(115200);                                //串口1初始化为115200
 while(1)
      {
      write_register_80_1byte(0x01,0x06);            //控制屏幕亮度，01单元写入06
      delay_ms(1000);  
      write_register_80_1byte(0x01,0x2f);            //控制屏幕亮度，01单元写入2F
      delay_ms(1000);  
      write_register_80_1byte(0x02,0xC8);            //控制蜂鸣器响2秒，02单元写入C8
      delay_ms(1000);
      write_register_80_2byte(0x03,0x00,0x02);      //切换到2号界面，03、04单元写入00、02
      delay_ms(1000);
      
      a1[0]=0x00;
      a1[1]=0x03;
      write_multiple_register_80(0x03,2,a1);        //切换到3号界面，03、04单元写入00、03
      delay_ms(1000);
      
      
      write_variable_store_82_1word(0x1000,0x55aa);          //向0x1000单元里写入数据0x55aa
      write_variable_store_82_2word(0x2000,0x1122,0x3344);   //向0x2000、0x2001单元里写入数据0x1122、0x3344
      
      a2[0]=0x0001;
      a2[1]=0x0002;
      write_multiple_variable_store_82(0x3000,2,a2);         //向0x3000、0x3001单元里写入数据0x0001、0x0002
	   
		delay_ms(1000);
      write_register_80_2byte(0x03,0x00,0x01);               //切换到1号界面，03、04单元写入00、01
      
     }  
 }

 
//0x80指令-写一个寄存器
//地址范围为0x00~0xff,所以address是8位的
//参考开发指南-2.2寄存器读写指令 0x80 、0x81
void write_register_80_1byte(uint8_t address,uint8_t data)    
 {
   u8 i;
   uint8_t array[array_length];
	 
   array[0]=USER_R3;
   array[1]=USER_RA;
   array[2]=0x03;               //指令长度
   array[3]=0x80;
   array[4]=address;
   array[5]=data;               //数值 

   for(i=0;i<6;i++)             //通过串口一发送指令
    {
      USART_SendData(USART1, array[i]);
      while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);
    } 
 }

//0x80指令-连续写两个寄存器单元 
void write_register_80_2byte(uint8_t address,uint8_t data1,uint8_t data2)                 
 {
   u8 i;
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
      USART_SendData(USART1, array[i]);
      while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);
    } 
 }


//0x80指令-连续写三个寄存器单元 
void write_register_80_3byte(uint8_t address,uint8_t data1,uint8_t data2,uint8_t data3)
 {
   u8 i;
   uint8_t array[array_length];
	 
   array[0]=USER_R3;           
   array[1]=USER_RA;          
   array[2]=0x05;              //指令长度
   array[3]=0x80;
   array[4]=address;
   array[5]=data1;             //数值1
   array[6]=data2;             //数值2
   array[7]=data3;             //数值3

	 
   for(i=0;i<8;i++)            //通过串口一发送指令
    {
      USART_SendData(USART1, array[i]);
      while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);
    } 
 } 
 
 
//0x80指令-写多个寄存器
//地址范围为0x00~0xff,所以address是8位的
//参考开发指南-2.2寄存器读写指令 0x80 、0x81
void write_multiple_register_80(uint8_t address,uint8_t data_length,uint8_t *data)    
 {
   u8 i,nDataLen;
   uint8_t array[array_length];
	 
   array[0]=USER_R3;
   array[1]=USER_RA;
   array[2]=1+1+data_length;   //命令占一字节，地址占一字节，再加上数据长度
   array[3]=0x80;
   array[4]=address;
	 
   for(i=0;i<data_length;i++)
    {
      array[5+i]=data[i]; 
    }
	 
   nDataLen=array[2]+3;        //有效命令的长度
	 
   for(i=0;i<nDataLen;i++)     //通过串口一发送指令
    {
      USART_SendData(USART1, array[i]);
      while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//等待发送结束
    } 
 }



//0x82指令-写一个变量存储单元
void write_variable_store_82_1word(uint16_t address,uint16_t data)     
{
   u8 i;
   uint8_t array[array_length];
	 
   array[0]=USER_R3;
   array[1]=USER_RA;
   array[2]=1+2+2;                   //命令占1字节，地址占2字节，再加上数据长度2字节
   array[3]=0x82;
   array[4]=(address&0xFF00)>>8;     //取地址的高8位
   array[5]=address&0x00FF;          //取地址的低8位
   array[6]=(data&0xFF00)>>8;        //取数据的高8位
   array[7]=data&0x00FF;             //取数据的低8位
	 
   for(i=0;i<8;i++)                  //通过串口发送指令
   {
     USART_SendData(USART1, array[i]);
     while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);
   } 
 }


//0x82指令-连续写两个变量存储单元                    
void write_variable_store_82_2word(uint16_t address,uint16_t data1,uint16_t data2)           
{
   u8 i;
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
   	 
   for(i=0;i<10;i++)                 //通过串口发送指令
   {
     USART_SendData(USART1, array[i]);
     while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);
   } 
 } 
 
  
//0x82指令-连续写多个变量存储单元
//地址范围为0x0000~0xffff,所以address是16位的
//数据内容是2N字节，因为每个变量存储单元包含2个字节。
//参考开发指南-2.3变量存储器读写指令 0x82 、0x83
void write_multiple_variable_store_82(uint16_t address,uint8_t data_length,uint16_t *data)    
 {
   u8 i,nDataLen;
   uint8_t array[array_length];
	 
   array[0]=USER_R3;
   array[1]=USER_RA;
   array[2]=1+2+data_length*2;  //命令占1字节，地址占2字节，再加上数据长度(数据是双字节)
   array[3]=0x82;
   array[4]=(address&0xFF00)>>8;         //取地址的高8位
   array[5]=address&0x00FF;              //取地址的低8位

   for(i=0;i<data_length;i++)
    {
      array[6+2*i]=(data[i]&0xFF00)>>8;  //取数据的高8位
      array[7+2*i]=data[i]&0x00FF;       //取数据的低8位
    }

   nDataLen=array[2]+3;                  //有效命令的长度
	 
   for(i=0;i<nDataLen;i++)               //通过串口一发送指令
   {
     USART_SendData(USART1, array[i]);
     while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);
   } 
 }

 
