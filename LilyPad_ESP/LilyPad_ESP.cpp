#include <activeLoopSwitch.h>
#include <ESP_Programmer.h>


void main_loop (unsigned long tick) {
    
}

unsigned long run_tick = 0;


void run_loop (unsigned long tick) {
    if(detectProgrammer()) return;
    
    ESP8266Programmer_flash_led();
    unsigned long when = millis();
    if ( (when-run_tick) > 1000) {
        Serial.print("elapsed:");
        Serial.println((run_tick=when));
    }

}

void main_setup () {
    AfterESP8266Programmer_SerialBaud = 57600;
    AfterESP8266Programmer_Serial1Baud = 9600;
    AfterESP8266Programmer_loop = run_loop;
    ESP8266Programmer_begin();
}


void dec2hex (byte b, char *h) {
    byte B = (b >> 4) & 0x0f;
    h[0] = B < 0x0a ? '0'+B : 'a' + (B-0x0a);
    B = b & 0x0f;
    h[1] = B < 0x0a ? '0'+B : 'a' + (B-0x0a);
}



