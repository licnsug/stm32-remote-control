#include "stm32f10x.h"                  // Device header

void Timer_Init()
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);
	
	TIM_InternalClockConfig(TIM1);//配置tim内部时钟
	
	TIM_TimeBaseInitTypeDef www;
	www.TIM_ClockDivision=TIM_CKD_DIV1;//时钟频率选择（没啥卵用）
	www.TIM_CounterMode=TIM_CounterMode_Up;//向上计数模式
	/*此处设置为1ms，mpu6050读取数据就要花费3ms！！！*/
	www.TIM_Period=100-1;//有公式
	www.TIM_Prescaler=720-1;//
	www.TIM_RepetitionCounter=0;//高级计时器才有，所以给0
	TIM_TimeBaseInit(TIM1,&www);//初始化TIMx时间基准单元外设
	
	TIM_ClearFlag(TIM1,TIM_FLAG_Update);//????????
	TIM_ITConfig(TIM1,TIM_IT_Update,ENABLE);//开启中断
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_InitTypeDef eee;
	eee.NVIC_IRQChannel=TIM1_UP_IRQn ;//指定要启用或禁用的IRQ通道
	eee.NVIC_IRQChannelCmd=ENABLE;//                                  
	eee.NVIC_IRQChannelPreemptionPriority=2;//指定IRQ通道的抢占优先级
	eee.NVIC_IRQChannelSubPriority=1;//
	NVIC_Init(&eee);
	
	TIM_Cmd(TIM1,ENABLE);//.启用或禁用指定外设
}

/*void TIM2_IRQHandler()//定时器2指定中断函数
{
	if(TIM_GetITStatus(TIM2,TIM_IT_Update))//获取中断标志位
	{
		
		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
	}
}*/


