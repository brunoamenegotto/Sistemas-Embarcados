#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// Configuracoes iniciais
#define pinoBotao 0
#define pinoLED   BUILTIN_LED

unsigned long tempoAnteriorDebounce = 0;
unsigned long debounceDelay = 50;   // tempo do debounce time; aumentar se saida oscila

const char* ssid = "MyASUS";
const char* senha = "9a4281138522";

const char* mqtt_server = "broker.mqtt-dashboard.com";
const char* topico = "Sistemas.Embarcados.Topico.17193526";

WiFiClient clienteWIFI;
PubSubClient clienteMQTT(clienteWIFI);
String strMacAddress;
char macAddress[6];

int botao;
int z;
int statusLED = HIGH;
int statusBotao = 0;

// Metodo que monitora o recebimento de mensagens do broker MQTT
void callback(char* topico, byte* payload, unsigned int tamanho) {
  Serial.print("[MSG RECEBIDA] Topico: ");
  Serial.print(topico);
  Serial.print(" / Mensagem: ");
  for (int i = 0; i < tamanho; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
  // Controla o LED conforme o valor recebido
  if ((char)payload[0] == '1') {
    digitalWrite(pinoLED, LOW);   // Liga o LED nivel baixo por saída ser PULL_UP
  } else {
    digitalWrite(pinoLED, HIGH);  // Desliga o LED
  }
  statusLED = !digitalRead(pinoLED);
}

void conectaMQTT() {
  // Loop ate conexão
  while (!clienteMQTT.connected()) {
    Serial.print("Aguardando conexao MQTT...");
    if (clienteMQTT.connect(macAddress)) {
      Serial.println("MQTT conectado");
      //faz subscribe automatico no topico
      clienteMQTT.subscribe(topico);
    } else {
      Serial.print("Falha, rc=");
      Serial.print(clienteMQTT.state());
      Serial.println(" tentando reconectar em 5 segundos.");
      delay(5000);
    }
  }
}

void setup() {
  pinMode(pinoLED, OUTPUT);
  pinMode(pinoBotao, INPUT);
  Serial.begin(115200);

  // Conexao to Wi-Fi
  Serial.print("Conectando ");
  Serial.println(ssid);
  WiFi.begin(ssid, senha);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Mostra IP do servidor
  Serial.println("");
  Serial.println("WiFi conectado.");
  Serial.print("Endereço IP: ");
  Serial.println(WiFi.localIP());
  Serial.println(WiFi.RSSI());
  Serial.println("Use este endereço para conectar ao ESP8266");
  Serial.println();
  strMacAddress = WiFi.macAddress();
  strMacAddress.toCharArray(macAddress,6);
  // Conexao com broker no servidor MQTT
  clienteMQTT.setServer(mqtt_server, 1883);
  // Definicao do procedimento de recebimento de mensagens
  clienteMQTT.setCallback(callback);
}

void loop() {
  if (!clienteMQTT.connected()) {
    conectaMQTT();
  }
  clienteMQTT.loop();
  
  const char* payload;
  
  z = analogRead(A0);
  //Serial.println(z);
  if (z > 100 && z < 135) botao = 1;                     
  else if (z > 150 && z < 190) botao = 2;                
  else if (z > 500 && z < 550) botao = 3;                
  else if (z > 230 && z < 280) botao = 4;             
  else botao = 0;    
  //Serial.println(z);
  switch (botao) {
    case 1:
      //Serial.println("VOL-");
      payload = "VOL_DOWN";
      clienteMQTT.publish(topico, payload);
      break;
    case 2:
      //Serial.println("CH-");
      payload = "CH_DOWN";
      clienteMQTT.publish(topico, payload);
      break;
    case 3:
      //Serial.println("VOL+");
      payload = "VOL_UP";
      clienteMQTT.publish(topico, payload);
      break;
    case 4:
      //Serial.println("CH+");
      payload = "CH_UP";
      clienteMQTT.publish(topico, payload);
      break;
  }
  statusBotao = digitalRead(pinoBotao);
  if (statusBotao == LOW)
  {
    digitalWrite(pinoLED, HIGH);
    payload = "POWER";
    clienteMQTT.publish(topico, payload);
  }
  else
  {
    digitalWrite(pinoLED, LOW);
  }
  delay(100);
}
