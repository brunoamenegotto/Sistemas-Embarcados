#include <SoftwareSerial.h>
#define pin_tx 1 // TX
#define pin_rx 3 // RX
#define verde 16 // D0
#define amarelo 5 // D1
#define azul 4 // D2
#define vermelho 0 // D3

SoftwareSerial blue(pin_tx, pin_rx);

int led_verde = LOW;
int led_amarelo = LOW;
int led_azul = LOW;
int led_vermelho = LOW;

void setup() {
  Serial.begin(9600);
  blue.begin(9600);
  pinMode(verde, OUTPUT);
  pinMode(amarelo, OUTPUT);
  pinMode(azul, OUTPUT);
  pinMode(vermelho, OUTPUT);
}

void loop() {
  if (blue.available()) 
  {
    char c = blue.read();
    if (c == '1') led_verde = HIGH;
    if (c == '2') led_amarelo = HIGH;
    if (c == '3') led_azul = HIGH;
    if (c == '4') led_vermelho = HIGH;
    if (c == '0') 
    {
      led_verde = LOW;
      led_amarelo = LOW;
      led_azul = LOW;
      led_vermelho = LOW;
    }
  }
  digitalWrite(verde, led_verde);
  digitalWrite(amarelo, led_amarelo);
  digitalWrite(azul, led_azul);
  digitalWrite(vermelho, led_vermelho);
}
