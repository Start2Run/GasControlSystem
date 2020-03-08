#ifndef PID_H_ 
#define PID_H_ #include 
//Define parameter 
#define epsilon 0.01 
#define dt 0.01             
//100ms loop time 
#define MAX  100                   
//For Current Saturation 
#define MIN 0
#define Kp  0.1 #define Kd  0.01 #define Ki  0.005

float PIDcal(float setpoint,float actual_position);

#endif