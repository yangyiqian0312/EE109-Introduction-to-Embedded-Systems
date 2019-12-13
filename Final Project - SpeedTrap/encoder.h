volatile unsigned char new_state, old_state, temp;
volatile unsigned char changed;  // Flag for state change
volatile unsigned char a, b;
volatile unsigned int maxspeed;

void init_encoder();
ISR(PCINT1_vect);