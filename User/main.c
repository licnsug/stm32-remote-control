#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "Timer.h"
#include "Key.h"
#include "AD.h"
#include "math.h"
#include "String.h"
#include "Stdlib.h"
#include "NRF24L01.h"

uint8_t key_num,KEY;
uint8_t Flage;
uint8_t SendFlag,tastdadt;
uint16_t LA,LB,LC,LD;//接收数据
int8_t LAA,LBB,LCC,LDD;//处理完的数据

int8_t DataProcess(uint16_t ADvaul);

int main(void)
{
	OLED_Init();
	Timer_Init();
	Key_Inti();
	AD_Init();
	NRF24L01_Init();
	
	while (1)
	{ 
		key_num=Key_Getnum();
		if(key_num)
		{
			KEY=key_num;
			
		}
//处理数据
		LA=ad_ww[0];
		LB=ad_ww[1];
		LC=ad_ww[2];
		LD=ad_ww[3];
		
		LAA=DataProcess(LA);
		LBB=DataProcess(LB);
		LCC=DataProcess(LC);
		LDD=DataProcess(LD);
//发送数据		
		if(Flage)
		{
			NRF24L01_TxPacket[0]=0x00;
			NRF24L01_TxPacket[1]=LAA;
			NRF24L01_TxPacket[2]=LBB;
			NRF24L01_TxPacket[3]=LCC;
			NRF24L01_TxPacket[4]=LDD;
			NRF24L01_TxPacket[5]=key_num;
			
			SendFlag=NRF24L01_Send();
			if(SendFlag==1)
			{
				KEY=0;
			}
			Flage = 0;
		}
		
		tastdadt = NRF24L01_ReadReg(NRF24L01_STATUS); // NRF24L01_STATUS = 0x07
		
		OLED_ShowSignedNum(1,1,LAA,3);
		OLED_ShowHexNum(1,7,tastdadt,3);
		OLED_ShowSignedNum(2,1,LBB,3);
		OLED_ShowSignedNum(3,1,LCC,3);
		OLED_ShowSignedNum(4,1,LDD,3);
		OLED_ShowSignedNum(4,7,KEY,2);
		
		
		
		

		
		
		
		
	}
}


int8_t DataProcess(uint16_t ADvaul)
{
	int16_t vaul;
	vaul=ADvaul-2048;
	//-2048....2048
	if(vaul>100)
	{
		vaul-=100;
	}
	else if(vaul<-100)
	{
		vaul+=100;
	}
	else
		vaul=0;
	//-1948....1948
	vaul=vaul*101/1949;
	return vaul;
}



void TIM1_UP_IRQHandler(void)//1ms
{
	static uint16_t i,j;
	
	if(TIM_GetITStatus(TIM1,TIM_IT_Update) == SET)
	{   
		
		Key_Tick();
		
		i++;
		j++;
		if(i>=80)
		{
			i=0;
			Flage=1;
		}
		if(j>=5000)
		{
			Open();//定时开机防止tp4336进入低功耗
			j=0;
			
		}
		

		
		TIM_ClearITPendingBit(TIM1,TIM_IT_Update);
    }
			
}


