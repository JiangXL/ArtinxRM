#ifndef __CHASSIS_H__
#define __CHASSIS_H__

#include "mytype.h"
#include "stm32f4xx_hal.h"
#define CLOSEDLOOPRATIO -1
#ifdef EC60
#define Max_WheelSpeed 2500 //最大速度
#else
#define Max_WheelSpeed 850
#endif
#define Gyro_Ratio 1 // (1.651376146f)  //陀螺仪比例系数  如果不是1就不对啊
#define PI (3.1415926535898f)

typedef enum {
    Ctrl_Mode_RemoteControl      = 1, //只有遥控器模式
    Ctrl_Mode_KeyBoard_OpenLoop  = 3, //鼠标键盘 角速度开环
    Ctrl_Mode_KeyBoard_CloseLoop = 2, //鼠标键盘 角速度闭环

    CHASSIS_STOP, // stall
    CHASSIS_RELAX, // relax
    CHASSIS_OPEN_LOOP,
    CHASSIS_CLOSE_GYRO_LOOP,
    CHASSIS_FOLLOW_GIMBAL_ENCODER,

} eChassisMode;

typedef struct
{
    // float my_normal_speed;   //底盘合速度
    float           vx; // x分速度	forward/back
    float           vy; // y分速度	left/right
    float           omega; // w角速度  to calc matrix, target omega
    float           kb_vx;
    float           kb_vy;
    float           mouse_omega;
    eChassisMode    mode;
    eChassisMode    last_mode;
    float           target_angle; //地盘目标角度
    float           angle_from_gyro;
    float           omega_from_gyro; // measure omega
    float           last_angle;
    data_convert_ut angle_from_gimbal;
    int             is_snipe_mode; //	1 = yes 0 = no	// gimbal & chassis seperate
    /*new add*/
    data_convert_ut wheel_speed;
} chassis_t; //底盘状态

/**
        * @function 麦轮解算函数， 输入x和y方向速度， 旋转角度， 输出到4个轮子
        *	@bref omega: + = cw, - = ccw
        * @output speed[4];
        */
void mecanumCalc(float vx, float vy, float omega, const int each_max_spd,
                  s16 speed[]);
void reset_zgyro(void); // after call, at least wait 2s
void chassis_pid_init(void);
void chassis_task(void const*);
// extern float realtime_chassis_power;
// extern float realtime_chassis_current;
// extern float realtime_chassis_voltage;
extern vu32      latest_get_target_time;
extern chassis_t chassis;
#endif
