#include "stm32f10x.h"
#include "delay.h"
#include "usart.h"


//ȫ�ֱ���
#define array_length          100   //�������鳤�� 100

void write_register_80_1byte(uint8_t address,uint8_t data);                                //0x80ָ��-дһ���Ĵ�����Ԫ   
void write_register_80_2byte(uint8_t address,uint8_t data1,uint8_t data2);                 //0x80ָ��-����д�����Ĵ�����Ԫ
void write_register_80_3byte(uint8_t address,uint8_t data1,uint8_t data2,uint8_t data3);   //0x80ָ��-����д�����Ĵ�����Ԫ
void write_multiple_register_80(uint8_t address,uint8_t data_length,uint8_t *data);        //0x80ָ��-����д����Ĵ�����Ԫ

void write_variable_store_82_1word(uint16_t address,uint16_t data);                           //0x82ָ��-дһ�������洢��Ԫ
void write_variable_store_82_2word(uint16_t address,uint16_t data1,uint16_t data2);           //0x82ָ��-����д���������洢��Ԫ
void write_multiple_variable_store_82(uint16_t address,uint8_t data_length,uint16_t *data);   //0x82ָ��-����д��������洢��Ԫ


int main(void)
 {	
   uint8_t a1[array_length];
   uint16_t a2[array_length];  
	 
   delay_init();                                     //��ʱ������ʼ��	  
   NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);   //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
   uart_init(115200);                                //����1��ʼ��Ϊ115200
 while(1)
      {
      write_register_80_1byte(0x01,0x06);            //������Ļ���ȣ�01��Ԫд��06
      delay_ms(1000);  
      write_register_80_1byte(0x01,0x2f);            //������Ļ���ȣ�01��Ԫд��2F
      delay_ms(1000);  
      write_register_80_1byte(0x02,0xC8);            //���Ʒ�������2�룬02��Ԫд��C8
      delay_ms(1000);
      write_register_80_2byte(0x03,0x00,0x02);      //�л���2�Ž��棬03��04��Ԫд��00��02
      delay_ms(1000);
      
      a1[0]=0x00;
      a1[1]=0x03;
      write_multiple_register_80(0x03,2,a1);        //�л���3�Ž��棬03��04��Ԫд��00��03
      delay_ms(1000);
      
      
      write_variable_store_82_1word(0x1000,0x55aa);          //��0x1000��Ԫ��д������0x55aa
      write_variable_store_82_2word(0x2000,0x1122,0x3344);   //��0x2000��0x2001��Ԫ��д������0x1122��0x3344
      
      a2[0]=0x0001;
      a2[1]=0x0002;
      write_multiple_variable_store_82(0x3000,2,a2);         //��0x3000��0x3001��Ԫ��д������0x0001��0x0002
	   
		delay_ms(1000);
      write_register_80_2byte(0x03,0x00,0x01);               //�л���1�Ž��棬03��04��Ԫд��00��01
      
     }  
 }

 
//0x80ָ��-дһ���Ĵ���
//��ַ��ΧΪ0x00~0xff,����address��8λ��
//�ο�����ָ��-2.2�Ĵ�����дָ�� 0x80 ��0x81
void write_register_80_1byte(uint8_t address,uint8_t data)    
 {
   u8 i;
   uint8_t array[array_length];
	 
   array[0]=USER_R3;
   array[1]=USER_RA;
   array[2]=0x03;               //ָ���
   array[3]=0x80;
   array[4]=address;
   array[5]=data;               //��ֵ 

   for(i=0;i<6;i++)             //ͨ������һ����ָ��
    {
      USART_SendData(USART1, array[i]);
      while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);
    } 
 }

//0x80ָ��-����д�����Ĵ�����Ԫ 
void write_register_80_2byte(uint8_t address,uint8_t data1,uint8_t data2)                 
 {
   u8 i;
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
      USART_SendData(USART1, array[i]);
      while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);
    } 
 }


//0x80ָ��-����д�����Ĵ�����Ԫ 
void write_register_80_3byte(uint8_t address,uint8_t data1,uint8_t data2,uint8_t data3)
 {
   u8 i;
   uint8_t array[array_length];
	 
   array[0]=USER_R3;           
   array[1]=USER_RA;          
   array[2]=0x05;              //ָ���
   array[3]=0x80;
   array[4]=address;
   array[5]=data1;             //��ֵ1
   array[6]=data2;             //��ֵ2
   array[7]=data3;             //��ֵ3

	 
   for(i=0;i<8;i++)            //ͨ������һ����ָ��
    {
      USART_SendData(USART1, array[i]);
      while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);
    } 
 } 
 
 
//0x80ָ��-д����Ĵ���
//��ַ��ΧΪ0x00~0xff,����address��8λ��
//�ο�����ָ��-2.2�Ĵ�����дָ�� 0x80 ��0x81
void write_multiple_register_80(uint8_t address,uint8_t data_length,uint8_t *data)    
 {
   u8 i,nDataLen;
   uint8_t array[array_length];
	 
   array[0]=USER_R3;
   array[1]=USER_RA;
   array[2]=1+1+data_length;   //����ռһ�ֽڣ���ַռһ�ֽڣ��ټ������ݳ���
   array[3]=0x80;
   array[4]=address;
	 
   for(i=0;i<data_length;i++)
    {
      array[5+i]=data[i]; 
    }
	 
   nDataLen=array[2]+3;        //��Ч����ĳ���
	 
   for(i=0;i<nDataLen;i++)     //ͨ������һ����ָ��
    {
      USART_SendData(USART1, array[i]);
      while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//�ȴ����ͽ���
    } 
 }



//0x82ָ��-дһ�������洢��Ԫ
void write_variable_store_82_1word(uint16_t address,uint16_t data)     
{
   u8 i;
   uint8_t array[array_length];
	 
   array[0]=USER_R3;
   array[1]=USER_RA;
   array[2]=1+2+2;                   //����ռ1�ֽڣ���ַռ2�ֽڣ��ټ������ݳ���2�ֽ�
   array[3]=0x82;
   array[4]=(address&0xFF00)>>8;     //ȡ��ַ�ĸ�8λ
   array[5]=address&0x00FF;          //ȡ��ַ�ĵ�8λ
   array[6]=(data&0xFF00)>>8;        //ȡ���ݵĸ�8λ
   array[7]=data&0x00FF;             //ȡ���ݵĵ�8λ
	 
   for(i=0;i<8;i++)                  //ͨ�����ڷ���ָ��
   {
     USART_SendData(USART1, array[i]);
     while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);
   } 
 }


//0x82ָ��-����д���������洢��Ԫ                    
void write_variable_store_82_2word(uint16_t address,uint16_t data1,uint16_t data2)           
{
   u8 i;
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
   	 
   for(i=0;i<10;i++)                 //ͨ�����ڷ���ָ��
   {
     USART_SendData(USART1, array[i]);
     while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);
   } 
 } 
 
  
//0x82ָ��-����д��������洢��Ԫ
//��ַ��ΧΪ0x0000~0xffff,����address��16λ��
//����������2N�ֽڣ���Ϊÿ�������洢��Ԫ����2���ֽڡ�
//�ο�����ָ��-2.3�����洢����дָ�� 0x82 ��0x83
void write_multiple_variable_store_82(uint16_t address,uint8_t data_length,uint16_t *data)    
 {
   u8 i,nDataLen;
   uint8_t array[array_length];
	 
   array[0]=USER_R3;
   array[1]=USER_RA;
   array[2]=1+2+data_length*2;  //����ռ1�ֽڣ���ַռ2�ֽڣ��ټ������ݳ���(������˫�ֽ�)
   array[3]=0x82;
   array[4]=(address&0xFF00)>>8;         //ȡ��ַ�ĸ�8λ
   array[5]=address&0x00FF;              //ȡ��ַ�ĵ�8λ

   for(i=0;i<data_length;i++)
    {
      array[6+2*i]=(data[i]&0xFF00)>>8;  //ȡ���ݵĸ�8λ
      array[7+2*i]=data[i]&0x00FF;       //ȡ���ݵĵ�8λ
    }

   nDataLen=array[2]+3;                  //��Ч����ĳ���
	 
   for(i=0;i<nDataLen;i++)               //ͨ������һ����ָ��
   {
     USART_SendData(USART1, array[i]);
     while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);
   } 
 }

 
