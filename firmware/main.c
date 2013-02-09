#include "msp430g2553.h"
#include "hardware_description.h"
#include "motors.h"


//Control Registers
unsigned char initialized = 0;

//MCU - globals
unsigned char debug = 1;

//Motor structs
motor motors[2];


//Main state machine
void main(void) {
	if (!initialized)
	{
		config_motor_timers();

		//Set speed to 0 and set motor direction to initialize driver input channels
		motors[0].speed = 0;
		motors[0].TCCR[0] = &MOTOR1_in1CCR;
		motors[0].TCCR[1] = &MOTOR1_in2CCR;
		motors[0].PORT[0] = MOTOR1_in1PORT;
	    motors[0].PORT[1] = MOTOR1_in2PORT;
	    motors[0].PIN[0]  = MOTOR1_in1PIN;
	    motors[0].PIN[1]  = MOTOR1_in2PIN;

		motors[1].speed = 0;
		motors[1].TCCR[0] = &MOTOR2_in1CCR;
		motors[1].TCCR[1] = &MOTOR2_in2CCR;
		motors[1].PORT[0] = MOTOR2_in1PORT;
		motors[1].PORT[1] = MOTOR2_in2PORT;
		motors[1].PIN[0]  = MOTOR2_in1PIN;
	    motors[1].PIN[1]  = MOTOR2_in2PIN;

	    changeDirection(&motors[0],1);
	    changeDirection(&motors[1],1);

		initialized = 1;
	}
	
}
