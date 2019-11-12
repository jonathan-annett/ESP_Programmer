/**************************************************
   Simple pass-through serial flash programmer
   programming the ESP8266 by an Arduino

   This resembles the comfortable DTS controlled
   programming mode one have with an FTDI or similiar
   serial connection cable, where no manual reset of
   the ESP is needed to upload code and run it.
   Unfortunately there is no RTS/DTS control on the
   Arduino Serial library, so we solely rely on timing.

   If the esptool does not wait or retry long enough,
   you have to press the reset button

   Transmissions from the ESP are passed without any
   modification.


 ***************************************************/


/*
  connection table:
   ESP8266  Cactus Micro Rev2
   GPIO0    12
   ENABLE   13
   RX       TX
   TX       RX
   GND      GND

*/