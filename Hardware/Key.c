#include "stm32f10x.h"                  // Device header
#include "Delay.h"

uint16_t Key_Num;

void Key_Inti()
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	GPIO_InitTypeDef GPIO_AAA;
	GPIO_AAA.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_AAA.GPIO_Pin=GPIO_Pin_2|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14;
	GPIO_AAA.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_AAA);
	
	GPIO_AAA.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_AAA.GPIO_Pin=GPIO_Pin_15;
	GPIO_AAA.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_AAA);
	
}

uint8_t Key_Getnum()
{
	uint16_t temp;
	if(Key_Num)
	{
		temp=Key_Num;
	    Key_Num=0;
	    return temp;
	}
	return 0;
	
}

void Open()
{
	GPIO_ResetBits(GPIOB,GPIO_Pin_15);
	Delay_ms(55);
	GPIO_SetBits(GPIOB,GPIO_Pin_15);
}

uint8_t Key_Getstat()
{
	if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_2)==0)
	{
		return 1;
	}
	if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_10)==0)
	{
		return 2;
	}
	if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_11)==0)
	{
		return 3;
	}
	if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_12)==0)
	{
		return 4;
	}
	if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_14)==0)
	{
		return 5;
	}
	if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_13)==0)
	{
		return 6;
	}
	
	return 0;
}

void Key_Tick()//
{
	static uint16_t qq;//静态变量，函数退出后值不消失
	static uint16_t shanggci,benci;
	qq++;
	if(qq>=20)
	{
		qq=0;
		shanggci=benci;
		benci=Key_Getstat();
		
		
		if(shanggci==0&&benci!=0)
		{
			Key_Num=Key_Getstat();
		}
	}
	
	
}
