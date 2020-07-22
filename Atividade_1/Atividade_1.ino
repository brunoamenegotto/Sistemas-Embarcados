// Configuracoes iniciais
#define pinoBotaoMin   0
#define pinoLEDMin    12
#define pinoBotaoMax  4
#define pinoLEDMax    14
#define pinoLEDFadeD    5
#define pinoPotFadeA    A0
#define pinoLEDFadeA    16

#define maxBrilho   255
#define incremento  1

int brilho = 0;

void setup() {
  Serial.begin(9600);
  pinMode(pinoBotaoMin, INPUT);
  pinMode(pinoLEDMin, OUTPUT);
  pinMode(pinoBotaoMax, INPUT);
  pinMode(pinoLEDMax, OUTPUT);
  pinMode(pinoLEDFadeD, OUTPUT);
  pinMode(pinoPotFadeA, INPUT);
  pinMode(pinoLEDFadeA, OUTPUT);
}

void loop() {
  if (digitalRead(pinoBotaoMax) == HIGH) {
    if (brilho < maxBrilho) {
      brilho += incremento;
      Serial.println(brilho);
    } else {
      piscarLedMax(); 
    }
  }
  if (digitalRead(pinoBotaoMin) == HIGH) {
    if (brilho > 0) {
      brilho -= incremento;
      Serial.println(brilho);
    } else {
      piscarLedMin(); 
    }
  }
  int val = analogRead(pinoPotFadeA);
  val = map(val, 0, 1023, 0, 255);
  analogWrite(pinoLEDFadeA, val);
  analogWrite(pinoLEDFadeD, brilho);
  delay(10);
}

void piscarLedMax() {
  digitalWrite(pinoLEDMax, HIGH);
  delay(1000);
  digitalWrite(pinoLEDMax, LOW);
  delay(1000);  
}

void piscarLedMin() {
  digitalWrite(pinoLEDMin, HIGH);
  delay(500);
  digitalWrite(pinoLEDMin, LOW);
  delay(500);  
}
