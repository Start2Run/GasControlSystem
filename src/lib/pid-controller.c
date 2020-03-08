#include <stdio.h>
#include "pid-controller.h"

void pid_init(struct PID* pid) {
    // set prev and integrated error to zero
    pid->integral = 0;
    pid->error = 0;
}

void pid_cal(float setpoint,float actual_value, struct PID* pid) 
{    
    float error;    
    float derivative;    
    float output;
    //Caculate P,I,D    
    error = setpoint - actual_value;
    //In case of error too small then stop integration    
    if(abs(error) > epsilon)    
    {        
        pid->integral = pid->integral + error*dt;    
    }    
    derivative = (error - pid->error)/dt;    
    output = Kp*error + Ki* pid->integral + Kd*derivative;
    //Saturation Filter    
    if(output > MAX)    
    {        
        output = MAX;    
    }    
    else if(output < MIN)    
    {        
        output = MIN;    
    }        
    //Update error        
    pid->error = error;
    pid->output = output; 
 } 