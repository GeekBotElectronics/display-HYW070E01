#include "reg52.h"			 //���ļ��ж����˵�Ƭ����һЩ���⹦�ܼĴ���
#include <string.h>

typedef unsigned int uint16_t;	  //���������ͽ�����������
typedef unsigned char uint8_t;

#define USER_R3               0xA5  //֡ͷ���ֽ�
#define USER_RA               0x5A  //֡ͷ���ֽ�
#define array_length          10   //�������鳤�� 00

void UsartInit();                      //���ڳ�ʼ��
void send_char_com(uint8_t ch);       //�򴮿ڷ���һ���ַ�
void delay(uint16_t ms_number);       // ms��ʱ����

void write_register_80_1byte(uint8_t address,uint8_t send_data);                               //0x80ָ��-дһ���Ĵ�����Ԫ   
void write_register_80_2byte(uint8_t address,uint8_t data1,uint8_t data2);                      //0x80ָ��-����д�����Ĵ�����Ԫ
void write_multiple_register_80(uint8_t address,uint8_t data_length,uint8_t *send_data);       //0x80ָ��-����д����Ĵ�����Ԫ

void write_variable_store_82_1word(uint16_t address,uint16_t send_data);                           //0x82ָ��-дһ�������洢��Ԫ
void write_variable_store_82_2word(uint16_t address,uint16_t data1,uint16_t data2);                //0x82ָ��-����д���������洢��Ԫ
void write_multiple_variable_store_82(uint16_t address,uint8_t data_length,uint16_t *send_data);   //0x82ָ��-����д��������洢��Ԫ

void main()
{	
   uint8_t a1[array_length];
   uint16_t a2[array_length];
	
	UsartInit();   //	���ڳ�ʼ��

	while(1)
   {   
      write_register_80_1byte(0X01,0x0F);            //������Ļ���ȣ�01��Ԫд��0F
      delay(1000);  
      write_register_80_1byte(0x01,0x3F);            //������Ļ���ȣ�01��Ԫд��3F
      delay(1000);  		
      write_register_80_1byte(0x02,0xC8);           //���Ʒ�������2�룬02��Ԫд��C8
      delay(1000);  	
      write_register_80_2byte(0x03,0x00,0x02);      //�л���2�Ž��棬03��04��Ԫд��00��02
      delay(1000);
		
      a1[0]=0x00;
      a1[1]=0x03;
      write_multiple_register_80(0x03,2,a1);        //�л���3�Ž��棬03��04��Ԫд��00��03
      delay(1000);
		
      write_variable_store_82_1word(0x1000,0x55aa);          //��0x1000��Ԫ��д������0x55aa
      write_variable_store_82_2word(0x2000,0x1122,0x3344);   //��0x2000��0x2001��Ԫ��д������0x1122��0x3344
      
      a2[0]=0x0001;
      a2[1]=0x0002;
      write_multiple_variable_store_82(0x3000,2,a2);         //��0x3000��0x3001��Ԫ��д������0x0001��0x0002
	   delay(1000);
      
      write_register_80_2byte(0x03,0x00,0x01);               //�л���1�Ž��棬03��04��Ԫд��00��01			
	   delay(1000);
    }	
}

////����Ϊ12MHz,����������Ϊ4800��ǧ��֮1.4
//void UsartInit()        //���ڳ�ʼ��
//{

//SCON=0x50;     //�趨���пڹ�����ʽ   
//TMOD=0x20;     //��ʱ��1������8λ�Զ�����ģʽ, ���ڲ���������
//PCON=0X80;		//SMOD=1�������ʼӱ�
//TH1=0xF3;     //������4800
//TL1=0xF3;
//TR1=1; 
//}

//����Ϊ11.0592MHz,����������Ϊ9600��
void UsartInit()        //���ڳ�ʼ��
{

SCON=0x50;     //�趨���пڹ�����ʽ   
TMOD=0x20;     //��ʱ��1������8λ�Զ�����ģʽ, ���ڲ���������
PCON=0X00;		//SMOD=0�������ʲ��ӱ�
TH1=0xFd;     //������9600
TL1=0xFd;
TR1=1; 
}


////�򴮿ڷ���һ���ַ�
void send_char_com( unsigned char ch)
{
	SBUF=ch;
   while (TI==0);
   TI=0;
}


//��ʱ���� 
 void delay(uint16_t ms_number) 
  {
   uint16_t i;
   uint16_t j;
   for(i=0;i<ms_number;i++)
    {
     for(j=0;j<200;j++);
    } 
  }

//0x80ָ��-дһ���Ĵ���
//��ַ��ΧΪ0x00~0xff,����address��8λ��
//�ο�����ָ��-2.2�Ĵ�����дָ�� 0x80 ��0x81
void write_register_80_1byte(uint8_t address,uint8_t send_data)    
 {
   uint8_t i;
   uint8_t array[array_length];
	 
   array[0]=USER_R3;
   array[1]=USER_RA; 
   array[2]=0x03;                //ָ���
   array[3]=0x80;
   array[4]=address;
   array[5]=send_data;          //��ֵ 

   for(i=0;i<6;i++)             //ͨ������һ����ָ��
    {
      send_char_com(array[i]);
    } 
 }

//0x80ָ��-����д�����Ĵ�����Ԫ 
void write_register_80_2byte(uint8_t address,uint8_t data1,uint8_t data2)                 
 {
   uint8_t i;
   uint8_t array[array_length];
	 
   array[0]=USER_R3;           
   array[1]=USER_RA;          
   array[2]=0x04;              //ָ���
   array[3]=0x80;
   array[4]=address;
   array[5]=data1;             //��ֵ1
   array[6]=data2;             //��ֵ2

	 
   for(i=0;i<7;i++)            //ͨ������һ����ָ��
    {
      send_char_com(array[i]);
    }
 }


//0x80ָ��-д����Ĵ���
//��ַ��ΧΪ0x00~0xff,����address��8λ��
//�ο�����ָ��-2.2�Ĵ�����дָ�� 0x80 ��0x81
void write_multiple_register_80(uint8_t address,uint8_t data_length,uint8_t *send_data)    
 {
   uint8_t i,nDataLen;
   uint8_t array[array_length];
	 
   array[0]=USER_R3;
   array[1]=USER_RA;
   array[2]=1+1+data_length;   //����ռһ�ֽڣ���ַռһ�ֽڣ��ټ������ݳ���
   array[3]=0x80;
   array[4]=address;
	 
   for(i=0;i<data_length;i++)
    {
      array[5+i]=send_data[i]; 
    }
	 
   nDataLen=array[2]+3;        //��Ч����ĳ���
	 
   for(i=0;i<nDataLen;i++)     //ͨ������һ����ָ��
    {
      send_char_com(array[i]);
    }
 }

//0x82ָ��-дһ�������洢��Ԫ
void write_variable_store_82_1word(uint16_t address,uint16_t send_data)     
{
   uint8_t i;
   uint8_t array[array_length];
	 
   array[0]=USER_R3;
   array[1]=USER_RA;
   array[2]=1+2+2;                   //����ռ1�ֽڣ���ַռ2�ֽڣ��ټ������ݳ���2�ֽ�
   array[3]=0x82;
   array[4]=(address&0xFF00)>>8;     //ȡ��ַ�ĸ�8λ
   array[5]=address&0x00FF;          //ȡ��ַ�ĵ�8λ
   array[6]=(send_data&0xFF00)>>8;   //ȡ���ݵĸ�8λ
   array[7]=send_data&0x00FF;        //ȡ���ݵĵ�8λ
	 
   for(i=0;i<8;i++)                  //ͨ������һ����ָ��
    {
      send_char_com(array[i]);
    }
 }


//0x82ָ��-����д���������洢��Ԫ                    
void write_variable_store_82_2word(uint16_t address,uint16_t data1,uint16_t data2)           
{
   uint8_t i;
   uint8_t array[array_length];
	 
   array[0]=USER_R3;
   array[1]=USER_RA;
   array[2]=1+2+4;                   //����ռ1�ֽڣ���ַռ2�ֽڣ��ټ������ݳ���4�ֽ�
   array[3]=0x82;
   array[4]=(address&0xFF00)>>8;     //ȡ��ַ�ĸ�8λ
   array[5]=address&0x00FF;          //ȡ��ַ�ĵ�8λ
   array[6]=(data1&0xFF00)>>8;       //ȡ���ݵĸ�8λ
   array[7]=data1&0x00FF;            //ȡ���ݵĵ�8λ
   array[8]=(data2&0xFF00)>>8;       //ȡ���ݵĸ�8λ
   array[9]=data2&0x00FF;            //ȡ���ݵĵ�8λ
   	 
   for(i=0;i<10;i++)                 //ͨ������һ����ָ��
    {
      send_char_com(array[i]);
    }
 } 
 
  
//0x82ָ��-����д��������洢��Ԫ
//��ַ��ΧΪ0x0000~0xffff,����address��16λ��
//����������2N�ֽڣ���Ϊÿ�������洢��Ԫ����2���ֽڡ�
//�ο�����ָ��-2.3�����洢����дָ�� 0x82 ��0x83
void write_multiple_variable_store_82(uint16_t address,uint8_t data_length,uint16_t *send_data)    
 {
   uint8_t i,nDataLen;
   uint8_t array[array_length];
	 
   array[0]=USER_R3;
   array[1]=USER_RA;
   array[2]=1+2+data_length*2;                //����ռ1�ֽڣ���ַռ2�ֽڣ��ټ������ݳ���(������˫�ֽ�)
   array[3]=0x82;
   array[4]=(address&0xFF00)>>8;              //ȡ��ַ�ĸ�8λ
   array[5]=address&0x00FF;                   //ȡ��ַ�ĵ�8λ

   for(i=0;i<data_length;i++)
    {
      array[6+2*i]=(send_data[i]&0xFF00)>>8;  //ȡ���ݵĸ�8λ
      array[7+2*i]=send_data[i]&0x00FF;       //ȡ���ݵĵ�8λ
    }

   nDataLen=array[2]+3;                       //��Ч����ĳ���
	 
   for(i=0;i<nDataLen;i++)                    //ͨ������һ����ָ��
    {
      send_char_com(array[i]);
    } 
 }
