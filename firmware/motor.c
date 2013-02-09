#include "motors.h"
#include "msp430g2553.h"

unsigned int PWMPeriod = 100-1; //in uS, aka clock ticks for SMCLK
unsigned char motor_enable = 0;

void config_motor_timers(void)
{
	TA1CTL = TASSEL_2 + MC_1;   // SMCLK, up mode
	TA0CTL = TASSEL_2 + MC_1;   // SMCLK, up mode

	TA1CCTL1 = OUTMOD_7;
	TA0CCTL1 = OUTMOD_7;// CCR1 reset/set

	TA0CCR0 = PWMPeriod;
	TA1CCR0 = PWMPeriod;
}

//Function to smoothly change the speed of a motor observing the max_ramp rate
void changeSpeed(motor* Motor, unsigned int speed)
{
	unsigned char i;
	if (speed > Motor->speed) //if we are speeding up
	{
		for(i=Motor->speed; i<=speed; i++)
		{
			*Motor->TCCR[1] = i;
			//_delay_cycles(Motor->max_ramp);
		}
	}else{  //otherwise we are slowing down
		for(i=Motor->speed; i>=speed; i--)
				{
					*Motor->TCCR[1] = i;
					//_delay_cycles(Motor->max_ramp);
				}
	}

	Motor->speed = speed;
}


void set_pin_OUT(unsigned char port, unsigned char pin, unsigned char value)
{

	unsigned volatile char *DIR;
	unsigned volatile char *POUT;
	unsigned volatile char *REN;
	unsigned volatile char *SEL;


	switch (port)
	{
	case 1 :
	   DIR = &P1DIR;
	   POUT = &P1OUT;
	   REN = &P1REN;
	   SEL = &P1SEL;
	   break;
	case 2 :
		DIR = &P2DIR;
		POUT = &P2OUT;
		REN = &P2REN;
		SEL = &P2SEL;
		break;
	}

	*SEL &= ~pin;    //Turn off special feature (turn into GPIO)
	*REN |= pin;		//Enable pull up resistor
	*DIR |= pin;		//Set pin as output

	//Set pin as 0 or 1
	switch (value)
	{
	case 0:
		*POUT &= ~pin;
		break;
	case 1:
		*POUT |= pin;
		break;
	}
}


void set_pin_TIMER(unsigned char port, unsigned char pin)
{
	unsigned volatile char *DIR;
	//int *OUT;
	//int *REN;
	unsigned volatile char *SEL;


	switch (port)
	{
	case 1 :
	   DIR = &P1DIR;
	  // OUT = &P1OUT;
	  // REN = &P1REN;
	   SEL = &P1SEL;
	   break;
	case 2 :
		DIR = &P2DIR;
		//OUT = &P2OUT;
		//REN = &P2REN;
		SEL = &P2SEL;
		break;
	}

	*SEL |= pin;
	*DIR |= pin;

}

//Function to change the direction a motor is turning.
void changeDirection(motor* Motor, char direction)
{
	    Motor->direction = direction;  //store direction in struct
	    int old_speed = Motor->speed;

	    changeSpeed(Motor,0);		   //Bring motor to zero speed safely before changing dir

	   	if (Motor->direction)   //if going forward
	   	{
	   		set_pin_OUT(Motor->PORT[0], Motor->PIN[0], 1);
	   		set_pin_TIMER(Motor->PORT[1], Motor->PIN[1]);

	   	}else{
	   		set_pin_OUT(Motor->PORT[0], Motor->PIN[0], 0);
	   		set_pin_TIMER(Motor->PORT[1], Motor->PIN[1]);
	}

	   	changeSpeed(Motor,old_speed);

}

void motorState(unsigned char state)
{
	P2SEL &= ~BIT5;
	P2DIR |= BIT5;
	P2REN |= BIT5;

	if(state)
	{
	   P2OUT |= BIT5;
	}else{
	   P2OUT &= ~BIT5;
	}

}




