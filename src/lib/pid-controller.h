#ifndef __PID_CONTROLLER__
#define __PID_CONTROLLER__ 

//Define parameter 
#define epsilon 0.01 
#define dt 0.01 //100ms loop time     

#define MAX 50 //For Current Saturation 
#define MIN -50
#define Kp  0.1 
#define Kd  0.01 
#define Ki  0.005

extern struct PID
{
    float error;
    float integral;
    float output;
};

void pid_init(struct PID *pid);

void pid_cal(float setpoint,float actual_value, struct PID *pid);

#endif