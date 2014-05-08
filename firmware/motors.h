//Motors
typedef struct{
	unsigned int speed;
	unsigned char direction;  //1 = forward, 0 = reverse
	unsigned char decay;   //1 = fast, 0 = slow
    unsigned char error;  //Motor error flag
    unsigned int  max_ramp;  //Cycles to wait between speed updates

    volatile unsigned int  *TCCR[2];
    unsigned char  PORT[2];
    unsigned char   PIN[2];
} motor;

void config_motor_timers(void);
void changeSpeed(motor* Motor, unsigned int speed);
void changeDirection(motor* Motor, char direction);
void set_pin_OUT(unsigned char port, unsigned char pin, unsigned char value);
void set_pin_TIMER(unsigned char port, unsigned char pin);
void motorState(unsigned char state);
