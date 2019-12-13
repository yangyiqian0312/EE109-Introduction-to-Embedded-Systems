volatile unsigned char isStarted, isValid;
volatile char data[5];
volatile unsigned int count;
void init_serial(unsigned short ubrr_value);
void serial_txchar(char ch);
void serial_stringout(char* s);