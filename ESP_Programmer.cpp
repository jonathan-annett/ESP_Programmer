//#include <Arduino.h>
#include <activeLoopSwitch.h>

#include "ESP_Programmer.h"


int ESP8266Programmer_program_pin = 12;
int ESP8266Programmer_enable_pin = 13;

void (*AfterESP8266Programmer_loop)(unsigned long) = main_loop;
int AfterESP8266Programmer_SerialBaud = 57600;
int AfterESP8266Programmer_Serial1Baud = 9600;
unsigned long AbortESP8266ProgrammerAfter = 0;

unsigned long LED_FLASH_INTERVAL = PRE_PROGRAM_INTERVAL;
unsigned long PRE_PROGRAM_INTERVAL = 1000;

int lit = 0;
unsigned long tick = 0;
int flash_state = LOW;





void ESP8266Programmer_flash_led() {
    if (lit==0) {
       lit = 1;
       tick=0;
    } else {
        unsigned long when = millis();
        if (tick==0) {
             tick = when;
             flash_state = LOW;
             pinMode(LED_PIN, OUTPUT);
          } else {
            if (( when-tick) < LED_FLASH_INTERVAL) {
              return;
            }
            tick=when;
            if (flash_state==LOW) {
              flash_state=HIGH;
            } else {
              flash_state=LOW;
            }
        
          }
          digitalWrite(LED_PIN,flash_state); 
    }
}

void ESP8266Programmer_loop(unsigned long when) {
    
  // pass data from ESP to host, if any
  while (Serial1.available()) {
    Serial.write((uint8_t)Serial1.read());
  }

  // pass data from host to ESP, if any
  if (Serial.available()) {
      
    if (LED_FLASH_INTERVAL==PRE_PROGRAM_INTERVAL) LED_FLASH_INTERVAL = PROGRAMING_INTERVAL;
    AbortESP8266ProgrammerAfter=0;
    while (Serial.available())
    {
      Serial1.write((uint8_t)Serial.read());
    }
    ESP8266Programmer_flash_led();
    
  } else {
      if ( 
          
           (tick > 0) 
           
           && 
           
           (
               ( (lit==1) &&  ((when-tick) > 5000) )
                || 
                ((AbortESP8266ProgrammerAfter !=0) && (tick > AbortESP8266ProgrammerAfter) )  
            )
            
        ) {
          
          LED_FLASH_INTERVAL=POST_PROGRAM_INTERVAL;
          ESP8266Programmer_end();
          
      } else {
         //if (LED_FLASH_INTERVAL!=PROGRAMING_INTERVAL) 
         ESP8266Programmer_flash_led(); 
      }
  }

}

void ESP8266Programmer_begin() {
  Serial1.end();
  Serial.end();
   
  pinMode(ESP8266Programmer_enable_pin, OUTPUT);
  pinMode(ESP8266Programmer_program_pin, OUTPUT);
  
  digitalWrite(ESP8266Programmer_program_pin, HIGH);
  digitalWrite(ESP8266Programmer_enable_pin, LOW);
  delay(500);
  Serial1.begin(115000);
  Serial.begin(115000);
  delay(500);

  digitalWrite(ESP8266Programmer_program_pin, LOW);
  digitalWrite(ESP8266Programmer_enable_pin, HIGH);
  Serial.println("ESP8266 programmer ready.");
  
  unsigned long LED_FLASH_INTERVAL = PRE_PROGRAM_INTERVAL;
  
  lit = 0;
  tick = 0;
  flash_state = LOW;
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  
  active_loop=ESP8266Programmer_loop;
}

int serialLog_start=31;
byte serialLog [32]= {
    NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL, 
    NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL 
};


void monitorProgrammer() {
    int i;
    if (Serial.available())
    {
        for (i=serialLog_start;i<31;i++) {
            serialLog[i]=serialLog[i+1];
        }
        serialLog[31] = (byte)Serial.read();
        if (serialLog_start>0) serialLog_start--;
    }
}

bool detectProgrammer() {
    monitorProgrammer();
    if ( (serialLog_start == 0) && (serialLog [31]==0xc0)) {
        for (int i = 0; i < 31; i++) {
            if (serialLog[i]!=0x55) return false;
        }   
        serialLog_start =31; 
        for (int i = 0; i < 31; i++) {
            serialLog[i]=NULL;
        }   
        ESP8266Programmer_begin();
        return true;
    }
    return false;
}


void ESP8266Programmer_end () {
    Serial1.end();
    Serial.end();
    delay (500);
    Serial.begin(AfterESP8266Programmer_SerialBaud);
    Serial1.begin(AfterESP8266Programmer_Serial1Baud);
    digitalWrite(ESP8266Programmer_program_pin, LOW);
    digitalWrite(ESP8266Programmer_enable_pin, HIGH);
    delay(500);
    digitalWrite(ESP8266Programmer_program_pin, HIGH);
    digitalWrite(ESP8266Programmer_enable_pin, LOW);
    active_loop=AfterESP8266Programmer_loop;
}

