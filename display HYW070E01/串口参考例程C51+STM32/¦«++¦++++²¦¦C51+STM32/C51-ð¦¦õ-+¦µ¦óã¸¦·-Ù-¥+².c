//����ֵ0x0001д������洢��0x0010��Ԫ
// 
#include <reg51.h>
#include <string.h>

void init_serialcom( void ) //����ͨ�ų�ʼ�趨
{
TMOD=0x20;     //��ʱ��1������8λ�Զ�����ģʽ, ���ڲ���������
TH1=0xFD;     //������9600
TL1=0xFD;
SCON=0x50;     //�趨���пڹ�����ʽ   
TR1=1;  
}

//�򴮿ڷ���һ���ַ�
void send_char_com( unsigned char ch)
{SBUF=ch;
while (TI== 0);
TI= 0 ;
}


main()
{
init_serialcom(); //��ʼ������
//����ֵ0x0001д������洢��0x0010��Ԫ
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