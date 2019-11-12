

extern int ESP8266Programmer_program_pin;
extern int ESP8266Programmer_enable_pin;
#define LED_PIN 11
#define PROGRAMING_INTERVAL 100
#define POST_PROGRAM_INTERVAL 50
extern unsigned long LED_FLASH_INTERVAL;
extern unsigned long PRE_PROGRAM_INTERVAL;

extern void ESP8266Programmer_end ();

extern void ESP8266Programmer_begin();

extern void ESP8266Programmer_flash_led();

extern void ESP8266Programmer_loop(unsigned long tick);

extern void ESP8266Programmer_begin();

extern bool detectProgrammer();

extern void (*AfterESP8266Programmer_loop)(unsigned long);
extern int AfterESP8266Programmer_SerialBaud ;
extern int AfterESP8266Programmer_Serial1Baud ;
extern unsigned long AbortESP8266ProgrammerAfter; 