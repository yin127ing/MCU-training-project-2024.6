#ifndef  UART_H
#define  UART_H
#define uchar unsigned char
/*
void uart_init(void);
void uart_send_char(uchar i);  
void uart_send_str(char *zifu);        //发送 字符串
*/


/*
晶振11.0592M,波特率9600
定时器模式：T1八位自动重装
串口工作模式：1
中断：开串口中断，关定时器1中断
 */
void uart_init(void)
{
	TMOD&=0x0f;
	TMOD|=0x20;    //设置T1为八位自动重装 SM0=1，SM1=0！，【GATE=0（不受外部中断限制）】，【C/T=0(定时器模式)】
	TH1=0xfd;	//晶振11.0592M,波特率9600    =fs/12/(256-TH1)/32
	TL1=0xfd;		
	TR1=1;	//启动定时器1
 
	SM0=0;	//设置串口工作模式为模式1
	SM1=1;	
	EA=1;	//打开总总中断
	ES=1;	//打开串口中断
	REN=1;	//允许串口接收
}
void uart_send_char(uchar i)  
{
	ES=0;  //关闭串口中断
	SBUF=i;
	while(!TI);	//等待发送完毕
	TI=0;
	ES=1;
}
void uart_send_str(char *zifu)        //发送 字符串
{
	int i=0;
	while(zifu[i]!='\0')
	{
		uart_send_char(zifu[i]);
		i++;
	}
}
#endif 