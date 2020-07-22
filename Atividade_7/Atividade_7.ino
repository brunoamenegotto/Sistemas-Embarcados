#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <SSD1306Wire.h>
#include <AM2320.h>
#include <WiFiUdp.h>
#include <Wire.h>

#define SDA_PIN       4 //D2
#define SCL_PIN       5 //D1
#define ENDERECO_OLED 0x3C
#define ENDERECO_MPU  0x68
#define TAMANHO       GEOMETRY_128_32
#define TIME_ZONE    -3
// Configuracoes iniciais
#define pinoBotao     0
#define pinoLED       2

// Variveis de controle
int           statusLED = LOW;
int           statusBotao;
int           statusAnteriorBotao = HIGH;
unsigned long tempoAnteriorDebounce = 0;
unsigned long debounceDelay = 50;   // tempo do debounce time; aumentar se saida oscila

// WiFi network info.
const char*   ssid = "Carlos";
const char*   senha = "Heineken1986";

unsigned long contador;         // the debounce time; increase if the output flickers
unsigned long intervalo = 1000; // Tempo em ms do intervalo a ser utilizado

float         temperatura;
float         umidade;

bool mudouDirecao, mudouDirecaoAnterior;
//Variaveis para armazenar valores dos sensores
int AcX, AcY, AcZ, Tmp, GyX, GyY, GyZ;

SSD1306Wire display(ENDERECO_OLED, SDA_PIN, SCL_PIN, TAMANHO); // SDA, SCL -> Configuracao do display SSD1306
AM2320      sensor; // Cria uma instancia do sensor AM2320
WiFiUDP     ntpUDP; // Cliente UDP para o NTP
NTPClient   horaCliente(ntpUDP, "pool.ntp.org", TIME_ZONE * 3600, 60000); // Configuracao do Cliente NTP

void setup() 
{
  Wire.begin(SDA_PIN, SCL_PIN);
  Wire.beginTransmission(ENDERECO_MPU);
  Wire.write(0x6B);
  //Inicializa o MPU-6050
  Wire.write(0);
  Wire.endTransmission(true);

  // Conexao to Wi-Fi
  display.init();
  if (lePosicao()) display.flipScreenVertically();
  display.setTextAlignment(TEXT_ALIGN_CENTER_BOTH);
  display.setFont(ArialMT_Plain_24);
  display.drawString(display.getWidth() / 2, display.getHeight() / 2, "Conectando");
  display.display();

  Serial.begin(115200);
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
  Serial.println("Use este endereço para conectar ao ESP8266");
  Serial.println();

  // Mostra o IP da conexao no display OLED
  display.clear();
  display.setFont(ArialMT_Plain_10);
  display.drawString(display.getWidth() / 2, display.getHeight() / 2, "IP: " + WiFi.localIP().toString());
  display.display();

  sensor.begin(SDA_PIN, SCL_PIN); // Conecta o sensor AM2320 ao barramento I2C - SDA, SCL
  contador = millis(); // Inicializa o contador para o intervalo

  horaCliente.begin();
  contador = millis();

  pinMode(pinoBotao, INPUT);
  pinMode(pinoLED, OUTPUT);

  digitalWrite(pinoLED, statusLED);
}

void loop() 
{
  String infoDisplay; // Variavel que armazena a informacao a ser mostrada no display ou porta serial
  char strDisplay[30]; // Variavel auxiliar para armazenar infoDisplay formatada
  mudouDirecao = lePosicao();
  int reading = digitalRead(pinoBotao);
  // verifica o botao foi pressionado (entrada mudou de LOW para HIGH)
  // e se foi tempo suficiente para ignorar qualquer ruído:
  // VErifica a mudanca de status por ruído ou presionar o botao:
  if (reading != statusAnteriorBotao) {
    // reset
    tempoAnteriorDebounce = millis();
  }

  if ((millis() - tempoAnteriorDebounce) > debounceDelay) {
    // seja qual for a leitura, verifica se esta ocorreu a mais tempo que o debounce
    // se isto ocorrer então passa a ser o estado atual:
    // verifica se o estado do botão foi alterado:
    if (reading != statusBotao) {
      statusBotao = reading;
      // somente inverte o LED se o novo estado fo botao for HIGH
      if (statusBotao == HIGH) {
        statusLED = !statusLED;
      }
    }
  }
  digitalWrite(pinoLED, statusLED);
  // armazena a leitura e na próxima passagem pelo loop sera o statusAnteriorBotao:
  statusAnteriorBotao = reading;
  if(statusLED == LOW)
  {
    if (millis() - contador > intervalo)
    {
      if (sensor.measure())
      { // Verifica se o sensor esta operacional para leitura dos valores 
        temperatura = sensor.getTemperature(); // Obtem o valor de temperatura
        umidade = sensor.getHumidity(); // Obtem o valor da umidade relativa
        sprintf(strDisplay, "%.1fºC  -  %.0f%%", temperatura, umidade); // Formata a saida para ser mostrada no display
      } else {
        sprintf(strDisplay, "Erro leitura");
      }
      infoDisplay = strDisplay; // Atualiza o conteudo da informacao para String infoDisplay
      Serial.println(infoDisplay); // Imprime informacao formatada na serial
      // Mostra informacao atualizada da hora no display OLED
      display.clear();
      display.drawRect(0, 0, display.getWidth() - 1, display.getHeight() - 1);
      display.setTextAlignment(TEXT_ALIGN_CENTER);
      display.setFont(ArialMT_Plain_16);
      display.drawString(display.getWidth() / 2, display.getHeight() / 2 - 16, infoDisplay);
      display.setFont(ArialMT_Plain_10);
      display.drawString(display.getWidth() / 2, display.getHeight() / 2 + (TAMANHO == GEOMETRY_128_64 ? 5 : 0), "IP: " + WiFi.localIP().toString());
      display.display();

      contador = millis();
    }
  }
  else
  {
    if (millis() - contador > intervalo) 
    {
      horaCliente.update(); // Atualiza a hora no sistema utilizando o servidor NTP
      infoDisplay = horaCliente.getFormattedTime();
      // Mostra hora na porta Serial
      Serial.println(infoDisplay); // Imprime informacao formatada na serial
      // Mostra hora no display OLED
      display.clear();
      display.drawRect(0, 0, display.getWidth() - 1, display.getHeight() - 1);
      display.setTextAlignment(TEXT_ALIGN_CENTER);
      display.setFont(ArialMT_Plain_16);
      display.drawString(display.getWidth() / 2,  0, infoDisplay);
      display.setFont(ArialMT_Plain_10);
      display.drawString(display.getWidth() / 2, 16, "IP: " + WiFi.localIP().toString());
      display.display();

      contador = millis();
    }
  }  
  // Mostra informacao atualizada da hora no display OLED
  if (mudouDirecao != mudouDirecaoAnterior) {
    if (mudouDirecao) {
      display.init();
      display.flipScreenVertically();
    } else {
      display.init();
    }
    mudouDirecaoAnterior = mudouDirecao;
  }
}

bool lePosicao() {
  Wire.beginTransmission(ENDERECO_MPU);
  Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  //Solicita os dados do sensor
  Wire.requestFrom(ENDERECO_MPU, 14, 1);
  //Armazena o valor dos sensores nas variaveis correspondentes
  AcX = Wire.read() << 8 | Wire.read(); //0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)
  AcY = Wire.read() << 8 | Wire.read(); //0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  AcZ = Wire.read() << 8 | Wire.read(); //0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
  Tmp = Wire.read() << 8 | Wire.read(); //0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
  GyX = Wire.read() << 8 | Wire.read(); //0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
  GyY = Wire.read() << 8 | Wire.read(); //0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
  GyZ = Wire.read() << 8 | Wire.read(); //0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)

  AcX = map(AcX, 0, 65535, 0, 200);
  AcY = map(AcY, 0, 65535, 0, 200);
  AcZ = map(AcZ, 0, 65535, 0, 200);

  GyX = map(GyX, 0, 65535, 0, 200);
  GyY = map(GyY, 0, 65535, 0, 200);
  GyZ = map(GyZ, 0, 65535, 0, 200);

  return AcY > 145;
}
