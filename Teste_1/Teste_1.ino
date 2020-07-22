#include <Wiegand.h>

WIEGAND wg;

#define beep 25
#define led 26

void setup() 
{
  Serial.begin(9600); 
  wg.begin();
  pinMode(beep, OUTPUT);
  pinMode(led, OUTPUT);
  digitalWrite(beep, HIGH);
  digitalWrite(led, HIGH);
}

void loop() 
{
  if(wg.available())
  {
    Serial.print("Wiegand HEX = ");
    Serial.print(wg.getCode(),HEX);
    Serial.print(", DECIMAL = ");
    Serial.print(wg.getCode());
    Serial.print(", Type W");
    Serial.println(wg.getWiegandType());    
    digitalWrite(beep, LOW);
    digitalWrite(led, LOW);
    delay(100);
    digitalWrite(beep, HIGH);
    digitalWrite(led, HIGH);
  }
  
}
