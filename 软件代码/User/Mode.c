#include "stm32f10x.h"                  // Device header
#include "stdlib.h"
#include "LED.h"
#include "Delay.h"
#include "OLED.h"
#include "Servo.h"
#include "PWM.h"
#include "Movement.h"
#include "usart1.h"
#include "usart2.h"
#include "usart3.h"
#include "stdio.h"
#include "Mode.h"
//***各种状态执行函数***//
//想让动作执行一次后停止，就在末尾加上“	move_mode = '0';”，否则动作持续反复执行，

extern uint8_t move_mode1 ;//用于接收蓝牙串口数据的变量
extern uint8_t move_mode3 ;//用于接收语音识别串口数据的变量
extern uint8_t move_mode ;//决定状态变量
extern uint8_t previous_mode ;//用于保存上一次接收串口数据的变量
extern uint16_t bz_flag;//避障标志位 ON/OFF 默认off
extern uint16_t hw_flag;//红外标志位 ON/OFF 默认off
extern int happiness; //开心指数
extern int stamina;  //体力值
extern uint16_t ff; //连续前进计数
extern uint16_t bb; //连续后退计数
extern uint16_t ll; //连续左转计数
extern uint16_t rr; //连续右转计数

/**************语音合成芯片设置命令*********************/
//选择背景音乐2。(0：无背景音乐  1-15：背景音乐可选)
//m[0~16]:0背景音乐为静音，16背景音乐音量最大
//v[0~16]:0朗读音量为静音，16朗读音量最大
//t[0~5]:0朗读语速最慢，5朗读语速最快

void mode_forward(void)//前进
{
			OLED_ShowImage(0, 0, 128, 64, BMP2); //前进脸	
			move_forward();
			LED13_Turn();//翻转电平状态，由1变为0，灯由灭变亮
			if(previous_mode == 'f')
			{
				ff++;
			}
			else
			{
				ff=0;
			}
			if(ff>=TIRED_N)
			{
				happiness -=1;
			}
			previous_mode = move_mode;// 把当前指令存成"上一次"
			stamina -= MOVE_COST;
	
}
void mode_behind(void)//后退
{
			OLED_ShowImage(0, 0, 128, 64, BMP2); //前进脸
			move_behind();
			LED13_Turn();
			if(previous_mode == 'b')
			{
				bb++;
			}
			else
			{
				bb=0;
			}
			if(bb>=TIRED_N)
			{
				happiness -=1;
			}
			previous_mode = move_mode;
			stamina -= MOVE_COST;
	
}
void mode_left(void)//左转
{
			OLED_ShowImage(0, 0, 128, 64, BMP3); //左转脸
			move_left();
			LED13_Turn();
			if(previous_mode == 'l')
			{
				ll++;
			}
			else
			{
				ll=0;
			}
			if(ll>=TIRED_N)
			{
				happiness -=1;
			}
			previous_mode = move_mode;
			stamina -= MOVE_COST;
	
	
}
void mode_right(void)//右转
{
			OLED_ShowImage(0, 0, 128, 64, BMP4); //右转脸
			move_right();
			LED13_Turn();
			if(previous_mode == 'r')
			{
				rr++;
			}
			else
			{
				rr=0;
			}
			if(rr>=TIRED_N)
			{
				happiness -=1;
			}
			previous_mode = move_mode;
			stamina -= MOVE_COST;
}

void mode_swing_qianhou(void)//前后摇摆
{
			OLED_ShowImage(0, 0, 128, 64, BMP11); //迷糊脸
			move_shake_qianhou();
			LED13_Turn();
			previous_mode = move_mode;
			stamina -= MOVE_COST;
			happiness +=1;
}
void mode_swing_zuoyou(void)//左右摇摆
{
			OLED_ShowImage(0, 0, 128, 64, BMP11); //迷糊脸
			move_shake_zuoyou();
			LED13_Turn();
			previous_mode = move_mode;
			stamina -= MOVE_COST;
			happiness +=1;
}

void mode_dance(void)//跳舞
{
			OLED_ShowImage(0, 0, 128, 64, BMP5); //特殊脸
			move_dance();
			LED13_Turn();
			previous_mode = move_mode;
			stamina -= MOVE_COST;
			happiness +=1;
	
}
void mode_stand(void)//立正
{
			OLED_ShowImage(0, 0, 128, 64, BMP1); //立正脸
			move_stand();
			LED13_Turn();
			previous_mode = move_mode;
			move_mode = '0';
			stamina -= MOVE_COST;
	
	
}
void mode_slowstand(void)//起身
{
			OLED_ShowImage(0, 0, 128, 64, BMP1); //立正脸
			move_slow_stand(previous_mode);
			LED13_Turn();
			previous_mode = move_mode;
			move_mode = '0';
			stamina -= MOVE_COST;
}

void mode_strech(void)//坐下擦脸
{
			OLED_ShowImage(0, 0, 128, 64, BMP1); //立正脸
			move_slow_stand(previous_mode);
			OLED_ShowImage(0, 0, 128, 64, BMP2);//前进脸
			move_stretch();
			OLED_ShowImage(0, 0, 128, 64, BMP12);//猫猫脸
			LED13_Turn();
			previous_mode = move_mode;
			move_mode = '0';
			stamina = STAMINA_MAX;
			happiness +=1;
}
void mode_hello(void)//打招呼
{
			if (previous_mode != '5' && previous_mode != 'D') {
				OLED_ShowImage(0, 0, 128, 64, BMP1); //立正脸
				move_slow_stand(previous_mode);
			}
	OLED_ShowImage(0, 0, 128, 64, BMP12);//猫猫脸
			//move_hello();
		int i;
	for(i=0;i<20;i++)//
	{
	Servo_SetAngle1(90-i);
	Servo_SetAngle2(90+i);
	//Servo_SetAngle3(90+i);
	Servo_SetAngle4(90-i);
		Delay_ms(7);
	}
	for(i=0;i<40;i++)//
	{
	Servo_SetAngle2(110+i);
	Servo_SetAngle4(70-i);
		Delay_ms(7);
	}
	for(i=0;i<60;i++)//右前足缓慢抬起
	{
		Servo_SetAngle1(70+i);
		Delay_ms(4);
	}
	Delay_ms(50);
	//下面是摇两次右前足
	Servo_SetAngle1(180);
	Delay_ms(400);
	Servo_SetAngle1(130);
	Delay_ms(400);
	Servo_SetAngle1(180);
	Delay_ms(400);
	Servo_SetAngle1(130);
	Delay_ms(400);
	Servo_SetAngle1(70);
	Delay_ms(500);
			LED13_Turn();
			previous_mode = move_mode;
			move_mode = '0';
			stamina -= MOVE_COST;
			happiness +=1;
}
void mode_twohands(void)//交替抬手
{
			OLED_ShowImage(0, 0, 128, 64, BMP1); //立正脸
			move_stand();
			move_two_hands();
			LED13_Turn();
			previous_mode = move_mode;
			move_mode = '0';
			stamina -= MOVE_COST;
			happiness +=1;
	
}
void mode_lanyao(void)//懒腰
{
			OLED_ShowImage(0, 0, 128, 64, BMP1); //立正脸
			move_slow_stand(previous_mode);
			OLED_ShowImage(0, 0, 128, 64, BMP9);//开心脸
			lan_yao();
			OLED_ShowImage(0, 0, 128, 64, BMP1);//立正脸
			LED13_Turn();
			previous_mode = move_mode;
			move_mode = '0';
			stamina = STAMINA_MAX;
			happiness +=1;
	
}
void mode_headup(void)//抬头
{
			OLED_ShowImage(0, 0, 128, 64, BMP1); //立正脸
			move_slow_stand(previous_mode);
			OLED_ShowImage(0, 0, 128, 64, BMP10);//调皮脸
			move_head_up();
			LED13_Turn();
			previous_mode = move_mode;
			move_mode = '0';
			stamina -= MOVE_COST;
	
}
void mode_sleeppa(void)//趴下睡觉
{
			if (previous_mode != '5' && previous_mode != 'q') {
				OLED_ShowImage(0, 0, 128, 64, BMP1); //立正脸
				move_slow_stand(previous_mode);
			}
			if (rand()%2) {//随机产生两种表情中的一种
				OLED_ShowImage(0, 0, 128, 64, BMP6); //普通睡觉脸
			}
			else{
				OLED_ShowImage(0, 0, 128, 64, BMP8); //酣睡脸
			}
			move_sleep_p();
			previous_mode = move_mode;
			move_mode = '0';
			stamina = STAMINA_MAX;
			happiness +=1;
}
void mode_sleepwo(void)//卧下睡觉
{
			if (previous_mode != '5' && previous_mode != 'q') {
				OLED_ShowImage(0, 0, 128, 64, BMP1); //立正脸
				move_slow_stand(previous_mode);
				Delay_s(1);
			}
			if (rand()%2) {//随机产生两种表情中的一种
				OLED_ShowImage(0, 0, 128, 64, BMP6); //普通睡觉脸
			}
			else{
				OLED_ShowImage(0, 0, 128, 64 , BMP8); //酣睡脸
			}
			move_sleep_w();
			previous_mode = move_mode;
			move_mode = '0';
			stamina = STAMINA_MAX;
			happiness +=1;
}
void mode_nanshou(void)//难受
{
	OLED_ShowImage(0, 0, 128, 64, BMP2); //前进脸	
	move_sleep_w();
	previous_mode = move_mode;
	move_mode = '0';
}
void mode_biaobai(void)//表白
{
	OLED_ShowImage(0, 0, 128, 64, BMP7); //love花痴脸	
	Servo_SetAngle1(135);//抬起右前脚
	OLED_ShowImage(0, 0, 128, 64, BMP11); //开心迷糊脸	
	Servo_SetAngle3(45);//抬起左前脚
	Servo_SetAngle1(90);//
	Servo_SetAngle3(90);//收回两只前脚
	OLED_ShowImage(0, 0, 128, 64, BMP9); //开心脸	
	OLED_ShowImage(0, 0, 128, 64, BMP7); //love花痴脸
	move_shake_qianhou();
	move_shake_qianhou();
	Delay_s(5);
	previous_mode = move_mode;
	move_mode = '0';
	stamina -= MOVE_COST;
	happiness +=1;
}
void mode_yuansu(void)//元素周期表
{
	OLED_ShowImage(0, 0, 128, 64, BMP5); //嘿嘿脸	
	Servo_SetAngle1(135);//抬起右前脚
	OLED_ShowImage(0, 0, 128, 64, BMP10); //调皮脸	
	Servo_SetAngle3(45);//抬起左前脚
	Servo_SetAngle2(45);//右后脚向后抬
	Servo_SetAngle4(135);//左后脚向后抬
	OLED_ShowImage(0, 0, 128, 64, BMP12); //猫猫脸	
	OLED_ShowImage(0, 0, 128, 64, BMP11); //开心迷糊脸
	move_shake_qianhou();
	move_shake_qianhou();
	move_shake_qianhou();
	Delay_s(2);
	previous_mode = move_mode;
	move_mode = '0';
	stamina -= MOVE_COST;
	happiness +=1;
}
void mode_xiaoxun(void)//校训
{
	OLED_ShowImage(0, 0, 128, 64, BMP5); //嘿嘿脸	
	Servo_SetAngle1(135);//抬起右前脚
	OLED_ShowImage(0, 0, 128, 64, BMP10); //调皮脸	
	Servo_SetAngle3(45);//抬起左前脚
	Servo_SetAngle2(45);//右后脚向后抬
	Servo_SetAngle4(135);//左后脚向后抬
	OLED_ShowImage(0, 0, 128, 64, BMP12); //猫猫脸	
	OLED_ShowImage(0, 0, 128, 64, BMP11); //开心迷糊脸
	move_shake_qianhou();
	move_shake_qianhou();
	Delay_s(2);
	previous_mode = move_mode;
	move_mode = '0';
	stamina -= MOVE_COST;
}
void mode_world(void)//世界之光
{
	OLED_ShowImage(0, 0, 128, 64, BMP5); //嘿嘿脸	
	Servo_SetAngle2(135);//右后脚向前抬
	Servo_SetAngle4(45);//左后脚向前抬
	Delay_s(2);
	previous_mode = move_mode;
	move_mode = '0';
}
void mode_manbo(void)//语音唤醒
{
	OLED_ShowImage(0, 0, 128, 64, BMP1); //立正脸
	previous_mode = move_mode;
	move_mode = '0';
}
void OLED_happiness(void)//OLED展示开心值
{
	OLED_ShowChinese(0,0,"开心值，");
	OLED_ShowNum(64,0,happiness,2,OLED_8X16);
	OLED_UpdateArea(0,0,128,16);
}
void mode_happiness(void)//展示开心值
{
	OLED_ShowImage(0, 8, 128, 48, BMP1); //立正脸
	OLED_happiness();
	if(happiness>=6){
		OLED_ShowImage(0, 8, 128, 48, BMP9); //开心脸
		OLED_happiness();
		move_shake_qianhou();
		move_shake_qianhou();Delay_s(4);
	}
	else if(happiness<=2){
		OLED_ShowImage(0, 8, 128, 48, BMP2);
		OLED_happiness();
		if (rand()%2) //随机产生两种动作中的一种
			{move_sleep_p();}
		else{move_sleep_w();}
	}Delay_s(3);
	previous_mode = move_mode;
	move_mode = '0';
}
void OLED_stamina(void)//OLED展示体力值
{
	OLED_ShowChinese(0,0,"体力值，");
	OLED_ShowNum(64,0,stamina,2,OLED_8X16);
	OLED_UpdateArea(0,0,128,16);
}
void mode_stamina(void)//展示体力值
{
	OLED_ShowImage(0, 8, 128, 48, BMP1); //立正脸
	OLED_stamina();
	if(stamina>=6){
		OLED_ShowImage(0, 8, 128, 48, BMP9); //开心脸
		OLED_stamina();
		move_shake_qianhou();
		move_shake_qianhou();Delay_s(6);
	}
	else if(stamina<=2){
		OLED_ShowImage(0, 8, 128, 48, BMP2);
		OLED_stamina();
		if (rand()%2) //随机产生两种动作中的一种
			{move_sleep_p();}
		else{move_sleep_w();}
	}Delay_s(4);
	previous_mode = move_mode;
	move_mode = '0';
}
void mode_index(void)//展示开心值和体力值
{
	OLED_Clear();
	OLED_ShowChinese(0,0,"开心值，");
	OLED_ShowNum(64,0,happiness,2,OLED_8X16);
	OLED_ShowChinese(0,16,"体力值，");
	OLED_ShowNum(64,16,stamina,2,OLED_8X16);
	OLED_UpdateArea(0,0,128,32);
	previous_mode = move_mode;
	move_mode = '0';
}

