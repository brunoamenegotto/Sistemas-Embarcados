#include <ArduinoJson.h>
#include <AM2320.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <NTPClient.h>

#define SDA_PIN       4 //D2
#define SCL_PIN       5 //D1
#define TIME_ZONE    -3

// WiFi network info.
const char* ssid = "Carlos";
const char* senha = "Heineken1986";

unsigned long contador;         // the debounce time; increase if the output flickers
unsigned long intervalo = 1000; // Tempo em ms do intervalo a ser utilizado

float         temperatura;
float         umidade;

AM2320      sensor; // Cria uma instancia do sensor AM2320
WiFiUDP ntpUDP; // Cliente UDP para o NTP
NTPClient horaCliente(ntpUDP, "pool.ntp.org", TIME_ZONE * 3600, 60000);

void setup() 
{
  Serial.begin(115200);
  Serial.print("Conectando ");
  Serial.println(ssid);
  WiFi.begin(ssid, senha);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Mostra IP da conexao na porta Serial
  Serial.println("");
  Serial.println("WiFi conectado.");
  Serial.print("Endereço IP: ");
  Serial.println(WiFi.localIP());
  Serial.println("Use este endereço para conectar ao ESP8266");
  Serial.println();

  sensor.begin(SDA_PIN, SCL_PIN); // Conecta o sensor AM2320 ao barramento I2C - SDA, SCL
  horaCliente.begin();
  contador = millis();
}

void loop()
{
  String infoDisplay;
  char strDisplay[30];

  if (millis() - contador > intervalo)
  {
    horaCliente.update(); // Atualiza a hora no sistema utilizando o servidor NTP
    infoDisplay = horaCliente.getFormattedTime();
    if (sensor.measure())
    { 
      temperatura = sensor.getTemperature(); // Obtem o valor de temperatura
      umidade = sensor.getHumidity(); // Obtem o valor da umidade relativa
    } 
    else 
    {
      sprintf(strDisplay, "Erro leitura");
    }
    // Alocacao de espaco para o documento JSON
    // 200 é o tamanho em bytes que será alocado na memoria
    // Use arduinojson.org/assistant para computar a capacidade
    StaticJsonDocument<200> doc;
    
    // Transforma o documento (doc) em um objeto JSON (root)
    JsonObject root = doc.to<JsonObject>();
    
    // Adicionando valores na JSON
    root["sensor"] = "AM2320";
    root["hora"] = infoDisplay;
    
    // Adicionando arrays na JSON
    JsonArray data = root.createNestedArray("info");
    data.add(temperatura);
    data.add(umidade);
    
    // Serializa o objeto e mostra na Serial
    Serial.println();
    // Ou entao de forma formatada
    serializeJsonPretty(root, Serial);
    Serial.println();
    // Sera impresso:
    // {
    //   "sensor": "AM2320",
    //   "hora":"23:58:58",
    //   "info": [
    //     'temperatura',
    //     'umidade'
    //   ]
    // }
    
    // Tambem e possivel serializar para uma String ou char info[]
    String resultado;
    serializeJson(root, resultado);
    char info[200];
    serializeJsonPretty(root, info);
    Serial.println(resultado);
    Serial.println(info);
    
    contador = millis();
  }
}
