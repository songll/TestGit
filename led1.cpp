/*
 *呼吸灯，在变化曲线的斜率为0.5的基础上，进行亮灯速率的调整
 */
#include <stdio.h>
#include <stdlib.h>

#define BREATHLEVEL 			(12*256)
#define LED_ADDR    			(0x23ff)
#define FPGA_BASE    			(0x100000) 			//FPGA基地址 

//把特定地址的数据读出来送给返回值
unsigned int IORD(unsigned int address, unsigned int offset)
{
	return (*(volatile unsigned *)(((address)|0x80000000)+4*(offset))); 
}

//把值value写入特定的地址内
void IOWR(unsigned int address, unsigned int offset, unsigned int value)
{
	(*(volatile unsigned *)(((address)|0x80000000)+4*(offset))) = (value);
}


//把数据data写入fpga的地址addr处
void wr_fpga( unsigned short addr, unsigned int data )
{
	IOWR( FPGA_BASE, addr, data );
}


int main(void)
{
	int in_count = 0,out_count = 0,temp = 0,boundary =1;
	unsigned short upgrade_led;
	upgrade_led = 1;


	while(1)
	{
		in_count++;
		if(in_count > out_count)
		{
			upgrade_led = 1;		
			wr_fpga(LED_ADDR, (unsigned int)upgrade_led);    //led灯灭
			
		}
		else
		{
			upgrade_led = 0;			
			wr_fpga(LED_ADDR, (unsigned int)upgrade_led);    //led灯亮
		}
		if(in_count >= BREATHLEVEL)
		{
			in_count = 0;
			if(boundary)
			{
				temp++;
				out_count = (temp + 1)/4;
				if(temp >=  BREATHLEVEL)
				{
					boundary = 0;
				}
			}
			else
			{
				temp--;
				out_count = (temp - 1)/4;
				if(out_count<1)
				{
					boundary = 1;
				}
			}
		}
	}
	
	return 0;
}


