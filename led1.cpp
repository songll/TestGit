/*
 *�����ƣ��ڱ仯���ߵ�б��Ϊ0.5�Ļ����ϣ������������ʵĵ���
 */
#include <stdio.h>
#include <stdlib.h>

#define BREATHLEVEL 			(12*256)
#define LED_ADDR    			(0x23ff)
#define FPGA_BASE    			(0x100000) 			//FPGA����ַ 

//���ض���ַ�����ݶ������͸�����ֵ
unsigned int IORD(unsigned int address, unsigned int offset)
{
	return (*(volatile unsigned *)(((address)|0x80000000)+4*(offset))); 
}

//��ֵvalueд���ض��ĵ�ַ��
void IOWR(unsigned int address, unsigned int offset, unsigned int value)
{
	(*(volatile unsigned *)(((address)|0x80000000)+4*(offset))) = (value);
}


//������dataд��fpga�ĵ�ַaddr��
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
			wr_fpga(LED_ADDR, (unsigned int)upgrade_led);    //led����
			
		}
		else
		{
			upgrade_led = 0;			
			wr_fpga(LED_ADDR, (unsigned int)upgrade_led);    //led����
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


