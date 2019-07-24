#include "FreeRTOS.h"
#include "task.h"
#include "croutine.h"
#include "queue.h"
#include "list.h"
#include "portable.h"
#include "FreeRTOSConfig.h"
#include "stm32f10x.h"

#include "timers.h"

#define BAUDRATE9600		0
#define BAUDRATE115200		1
#define BAUDRATE19200		2


void NVIC_Set(u8 NVIC_PreemptionPriority,u8 NVIC_SubPriority,u8 NVIC_Channel,u8 NVIC_Group)	 
{ 
	u32 temp;	
	u8 IPRADDR=NVIC_Channel/4;  //每组只能存4个,得到组地址 
	u8 IPROFFSET=NVIC_Channel%4;//在组内的偏移
	IPROFFSET=IPROFFSET*8+4;    //得到偏移的确切位置
	temp=NVIC_PreemptionPriority<<(4-NVIC_Group);	  
	temp|=NVIC_SubPriority&(0x0f>>NVIC_Group);
	temp&=0xf;//取低四位

	if(NVIC_Channel<32)NVIC->ISER[0]|=1<<NVIC_Channel;//使能中断位(要清除的话,相反操作就OK)
	else NVIC->ISER[1]|=1<<(NVIC_Channel-32);    
	NVIC->IP[IPRADDR]|=temp<<IPROFFSET;//设置响应优先级和抢断优先级   	    	  				   
}


void UART5_Send(u8 tx_data)
{
	while((UART5->SR & (0x40)) == 0);//等待发送完毕
	UART5->DR = tx_data;
}

void UART5_SendString(u8* str)
{
    while(*str != '\0')
    {
        UART5_Send(*str);
        str ++;
    }
}
/*
PC12	TX
PD2		RX
*/
void UART5_Init(u32 baud_rate)
{
	RCC->APB2ENR |= 1 << 4;			//Enable PC clock
	RCC->APB2ENR |= 1 << 5;			//Enable PD clock
	
	GPIOC->CRH &= 0xfff0ffff;		//PC12 IO配置
	GPIOC->CRH |= 0x000b0000;		
	
	GPIOD->CRL &= 0xfffff0ff;		//PD2  IO配置
	GPIOD->CRL |= 0x00000400;
	
	RCC->APB1ENR	|= 1<<20; 		//串口时钟使能
	
    RCC->APB1RSTR	|= 1<<20;		//复位串口
    RCC->APB1RSTR	&= ~(1<<20);	
	
	switch(baud_rate)
	{
		case BAUDRATE9600: UART5->BRR=0x0EA;break;
		case BAUDRATE19200:	 UART5->BRR=0x753;break;
		case BAUDRATE115200: UART5->BRR=0x0138;break;
		default:
			 UART5->BRR=0x0138;break;//其它波特率设置为
	}
    UART5->CR1|=0x202c;//设置停止位,设置接受中断使能
	NVIC_Set(2,1,53,0);//设置串口中断
}



TaskHandle_t test_Task_Handle;

TimerHandle_t TimerDebug = NULL;

void Fn_Timer_Debug(xTimerHandle pxTimer)
{
    UART5_SendString("Timer is running !\r\n");
}

void Task_Function(void *param)
{

    TimerDebug =  xTimerCreate("timerdebuh",
                               1000/portTICK_RATE_MS,
                               pdTRUE,
                               (void*)0,
                               Fn_Timer_Debug);
    xTimerStart(TimerDebug,0);
    for(;;)
    {
        vTaskDelay(500/portTICK_RATE_MS);
        UART5_SendString("Welcome to FreeRTOS!\r\n");
    }
}

int main(void)
{
    UART5_Init(119200);
    UART5_Send(0xaa);
    
    xTaskCreate(Task_Function,
            "1213",
            1024,
            NULL,
            1,
            &test_Task_Handle);

    vTaskStartScheduler();
//while(1)
//{
//    u16 i,j;
//    for(i=0;i<1000;i++)
//        for(j=0;j<1000;j++);
//    static u8 cnt = 0;
//    UART5_Send(cnt);
//    cnt ++;
//}
}
