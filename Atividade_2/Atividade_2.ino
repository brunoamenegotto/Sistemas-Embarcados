#define PIN_FRENTE     5 //PIN D1
#define PIN_TRAZ    12 //PIN D6
#define PIN_ESQUERDA  14 //PIN 5
#define PIN_DIREITA   4 // PIN D2

int frenteLigado = LOW;           
int trazLigado = LOW;
int esquerdaLigado = LOW;
int direitaLigado = LOW;

void setup()
{
  pinMode(PIN_FRENTE, OUTPUT);
  pinMode(PIN_TRAZ, OUTPUT);
  pinMode(PIN_ESQUERDA, OUTPUT);
  pinMode(PIN_DIREITA, OUTPUT);

  Serial.begin(115200);
}

void loop()
{
  if (Serial.available()) {
    char c = Serial.read();
    if (c == '1') frenteLigado = HIGH;
    if (c == '3') trazLigado = HIGH;
    if (c == '4') esquerdaLigado = HIGH;
    if (c == '2') direitaLigado = HIGH;
    if (c == '0') {
    frenteLigado = LOW; 
    trazLigado = LOW; 
    esquerdaLigado = LOW; 
    direitaLigado = LOW;  
    }
  }
  digitalWrite(PIN_FRENTE, frenteLigado); 
  digitalWrite(PIN_TRAZ, trazLigado);
  digitalWrite(PIN_ESQUERDA, esquerdaLigado);
  digitalWrite(PIN_DIREITA, direitaLigado);
}
