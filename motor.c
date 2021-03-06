
/************************************************
* File Name	: motor.c
* Version	: 2015.7.12 By DHP
* Device(s)	: R5F100LE
* Tool-Chain	: CA78K0R
* Description	: Motor Control
* API		: void Motor_Init()
		  void Motor_RateFlash(int16_t MOTO1, int16_t MOTO2, int16_t MOTO3, int16_t MOTO4);
		  // MOTO 1-4 输入范围均为 0 - MOTO_Max
		  
************************************************/

#include "include.h"

#define MOTO1_duty TDR01
#define MOTO2_duty TDR02
#define MOTO3_duty TDR03
#define MOTO4_duty TDR04

#define MOTO_Max 40000  // test percent: 40000 / 50000 = 80 %
float MOTO_PWM_ZeroOffset;
float MOTO_PWM_RatioOffset;

void Motor_Init()
{	
	// 因为设置了 PWM 输出频率为 100 Hz，周期为 10 ms， TDR00 = 0x9C3FU
	// 而电调要求控制油门为1-2ms ，在100Hz下，占空比是 10% - 20%
	// 故TDR的范围为 TDR00 * 0.05 ~ TDR00 * 0.10
	
	MOTO_PWM_ZeroOffset = 4000;	//(TDR00 + 1) * 0.05;
	MOTO_PWM_RatioOffset = 0.08;	//(TDR00 + 1) * (0.10 - 0.05) / 50000(MOTO_Max);
	Motor_RateFlash(0, 0, 0, 0);
	
	R_TAU0_Channel0_Start();  //方波输出，电机驱动	
}



/***********************************************
函数名:	Motor_RateFlash(int16_t MOTO1, int16_t MOTO2, int16_t MOTO3, int16_t MOTO4)
功能:	更新四路PWM值
输入:	MOTO1, MOTO2, MOTO3, MOTO4
输出:	无
描述：	#define MOTO_Max 50000
	MOTO 1-4 输入范围均为 0 - MOTO_Max
	
***********************************************/

void Motor_RateFlash(float MOTO1, float MOTO2, float MOTO3, float MOTO4)
{	
	if(MOTO1 > MOTO_Max) MOTO1 = MOTO_Max;
	if(MOTO2 > MOTO_Max) MOTO2 = MOTO_Max;
	if(MOTO3 > MOTO_Max) MOTO3 = MOTO_Max;
	if(MOTO4 > MOTO_Max) MOTO4 = MOTO_Max;
	
	if(MOTO1 < 0) MOTO1 = 0;
	if(MOTO2 < 0) MOTO2 = 0;
	if(MOTO3 < 0) MOTO3 = 0;
	if(MOTO4 < 0) MOTO4 = 0;
	
	MOTO1_duty = MOTO1 * MOTO_PWM_RatioOffset + MOTO_PWM_ZeroOffset;
	MOTO2_duty = MOTO2 * MOTO_PWM_RatioOffset + MOTO_PWM_ZeroOffset;
	MOTO3_duty = MOTO3 * MOTO_PWM_RatioOffset + MOTO_PWM_ZeroOffset;
	MOTO4_duty = MOTO4 * MOTO_PWM_RatioOffset + MOTO_PWM_ZeroOffset;
}