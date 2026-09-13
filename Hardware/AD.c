#include "stm32f10x.h"                  // Device header

uint16_t ad_ww[4];

void AD_Init()
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
    RCC_ADCCLKConfig(RCC_PCLK2_Div6);//分频
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
	
	GPIO_InitTypeDef GPIO_INITS;
	GPIO_INITS.GPIO_Mode=GPIO_Mode_AIN;//GPIO无效化
	GPIO_INITS.GPIO_Pin=GPIO_Pin_3|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_6;
	GPIO_INITS.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_INITS);
	
	ADC_RegularChannelConfig(ADC1,ADC_Channel_1,1,ADC_SampleTime_71Cycles5);
	ADC_RegularChannelConfig(ADC1,ADC_Channel_6,2,ADC_SampleTime_71Cycles5);
	ADC_RegularChannelConfig(ADC1,ADC_Channel_2,3,ADC_SampleTime_71Cycles5);
	ADC_RegularChannelConfig(ADC1,ADC_Channel_3,4,ADC_SampleTime_71Cycles5);
	
	ADC_InitTypeDef qqq;
	qqq.ADC_ContinuousConvMode=ENABLE;//打开连续
	qqq.ADC_DataAlign=ADC_DataAlign_Right;
	qqq.ADC_ExternalTrigConv=ADC_ExternalTrigConv_None;//外部中断选择
	qqq.ADC_Mode=ADC_Mode_Independent;
	qqq.ADC_NbrOfChannel=4;
	qqq.ADC_ScanConvMode=ENABLE;
	ADC_Init(ADC1,&qqq);
	
	DMA_InitTypeDef sss;
	sss.DMA_PeripheralBaseAddr=(uint32_t)&ADC1->DR;//从哪转运
	sss.DMA_PeripheralDataSize=DMA_PeripheralDataSize_HalfWord;//以16位转运
	sss.DMA_PeripheralInc=DMA_PeripheralInc_Disable;//只从dr寄存器转运
	sss.DMA_MemoryBaseAddr=(uint32_t)ad_ww;
	sss.DMA_MemoryDataSize=DMA_MemoryDataSize_HalfWord;
	sss.DMA_MemoryInc=DMA_MemoryInc_Enable;
	sss.DMA_DIR=DMA_DIR_PeripheralSRC;//传输方向
	sss.DMA_BufferSize=4; //缓存大小
	sss.DMA_M2M=DMA_M2M_Disable;//硬件出发
	sss.DMA_Mode=DMA_Mode_Circular;//使用自动重装
	sss.DMA_Priority=DMA_Priority_VeryHigh;//优先级
	DMA_Init(DMA1_Channel1,&sss);
	
	
	DMA_Cmd(DMA1_Channel1,ENABLE);
    ADC_DMACmd(ADC1,ENABLE);
	ADC_Cmd(ADC1,ENABLE);
	///校准配置
	ADC_ResetCalibration(ADC1);
	while(ADC_GetResetCalibrationStatus(ADC1)==SET);
	ADC_StartCalibration(ADC1);
	while(ADC_GetCalibrationStatus(ADC1)==SET);
	
	ADC_SoftwareStartConvCmd(ADC1,ENABLE);// 启动adc软件转换
	//初始化后adc会一直运行
}

//void AD_Get()
//{
//	DMA_Cmd(DMA1_Channel1,DISABLE);
//	DMA_SetCurrDataCounter(DMA1_Channel1,4);
//	DMA_Cmd(DMA1_Channel1,ENABLE);
//	
//	
//	
//	while(DMA_GetFlagStatus(DMA1_FLAG_TC1)==RESET);
//		DMA_ClearFlag(DMA1_FLAG_TC1);//清除标志位
//}
