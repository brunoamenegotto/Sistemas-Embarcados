#define TRIGGER_PIN  5 //D1
#define ECHO_PIN     4 //D2

long lerDistancia(int triggerPin, int echoPin)
{
  // Limpa o trigger
  pinMode(triggerPin, OUTPUT);  
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  // Mantem o sinal em HIGH por 10 microseconds
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);
  pinMode(echoPin, INPUT);
  // Le o echo pin, e retorna o tempo de retorno em microseconds
  return pulseIn(echoPin, HIGH);
}

void setup()
{
  Serial.begin(9600);
}

void loop()
{
  // Mede o tempo e converte para centimetros conforme datasheet
  float cm = lerDistancia(TRIGGER_PIN, ECHO_PIN) / 58.0;
  Serial.print("Distancia = ");
  Serial.print(cm);
  Serial.println("cm");
  delay(1000);  
}
