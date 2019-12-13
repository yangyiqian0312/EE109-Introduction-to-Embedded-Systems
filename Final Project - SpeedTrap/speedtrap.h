unsigned int DIST = 2800;
unsigned int answer;
unsigned int isTooSlow;
unsigned int isOK;
unsigned short frequency = 440;
unsigned int t;	// max speed from last run
unsigned int isComplete;	// 	Have you had a complete trap without being too slow?
unsigned int testspeed = 0; 
unsigned int mmspeed = 0;	// speed in mm/sec

volatile unsigned int hasStarted;	// Have you started timer by blocking the left buzzer?
volatile unsigned int buzzCounter;	// Use to stop buzzer 1.5s
volatile unsigned int leftsensorflag = 0;
volatile unsigned int buzzerOn = 0;	

void start_timer();
void stop_timer();
void init_timer();
void init_sensorinterrupt();
void start_buzzer();
void stop_buzzer();