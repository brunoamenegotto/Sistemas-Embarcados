#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 21
#define RST_PIN 35
#define Led_alerta 26
#define Led_sucesso 25
#define pinoRele 27
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Cria instância com MFRC522
 

// --- Variáveis Globais --- 
char st[20];


// --- Configurações Iniciais ---
void setup() 
{
  Serial.begin(115200);   // Inicia comunicação Serial em 9600 baud rate
  SPI.begin();          // Inicia comunicação SPI bus
  mfrc522.PCD_Init();   // Inicia MFRC522
  pinMode(Led_sucesso, OUTPUT);
  pinMode(Led_alerta, OUTPUT);
  pinMode(pinoRele, OUTPUT);
  digitalWrite(Led_sucesso, LOW);
  digitalWrite(Led_alerta, HIGH);
  digitalWrite(pinoRele, HIGH);
  Serial.println("Aproxime o seu cartao do leitor...");
  Serial.println();
}

void loop() 
{
  // Verifica novos cartões
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  // Seleciona um dos cartões
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  
  // Mostra UID na serial
  Serial.print("UID da tag :");
  String conteudo= "";
  byte letra;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     conteudo.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     conteudo.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Mensagem : ");
  conteudo.toUpperCase();
  
  if (conteudo.substring(1) == "92 29 86 B5")
  {
    digitalWrite(Led_alerta, LOW);
    delay(500);
    digitalWrite(Led_alerta, HIGH);
    delay(500);
    digitalWrite(Led_alerta, LOW);
    delay(500);
    digitalWrite(Led_alerta, HIGH);
    delay(500);
    digitalWrite(Led_alerta, LOW);
    digitalWrite(Led_sucesso, HIGH);
    Serial.println("Acesso Liberado!");
    digitalWrite(pinoRele, LOW);
    delay(1500);
    digitalWrite(pinoRele, HIGH);
    Serial.println();
  }
  else if (conteudo.substring(1) == "93 5E EE E2")
  {
    digitalWrite(Led_alerta, LOW);
    delay(500);
    digitalWrite(Led_alerta, HIGH);
    delay(500);
    digitalWrite(Led_alerta, LOW);
    delay(500);
    digitalWrite(Led_alerta, HIGH);
    delay(500);
    digitalWrite(Led_alerta, LOW);
    digitalWrite(Led_sucesso, HIGH);
    Serial.println("Acesso Liberado!");
    digitalWrite(pinoRele, LOW);
    Serial.println(digitalRead(pinoRele));
    delay(1500);
    digitalWrite(pinoRele, HIGH);
    Serial.println();
  }
  else
  {
    digitalWrite(Led_alerta, LOW);
    delay(500);
    digitalWrite(Led_alerta, HIGH);
    delay(500);
    digitalWrite(Led_alerta, LOW);
    delay(500);
    Serial.println("Acesso Negado");
    Serial.println();
  }
  digitalWrite(Led_sucesso, LOW);
  digitalWrite(Led_alerta, HIGH);
  
}
 
 
