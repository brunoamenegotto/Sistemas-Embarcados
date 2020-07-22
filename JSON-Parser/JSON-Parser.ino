#include <ArduinoJson.h>

String recebido = "{\"sensor\":\"AM2320\",\"hora\":\"23:59:59\",\"info\":[28.75, 72.30]}";

void setup() 
{
 Serial.begin(115200);
 //Serial.println(" ");
 //Serial.println(recebido);
}

String leStringSerial()
{
  String conteudo;
  char caractere;
  
  while(Serial.available() > 0) 
  {
    caractere = Serial.read();
    if (caractere != '\n')
    {
      conteudo.concat(caractere);
    }
    delay(10);
  }
    
  Serial.print("Recebi: ");
  Serial.println(conteudo);
  return conteudo;
}
  
void loop() 
{
  if (Serial.available() > 0)
  {
    recebido = leStringSerial();
  }
  
  StaticJsonDocument<200> doc;
  Serial.println(" ");
  Serial.println("Copie essa String, modifique e cole no Serial Arduino :");
  Serial.println(recebido);
  
  String json = recebido;

  DeserializationError erro = deserializeJson(doc, json);

  if (erro) 
  {
    Serial.print("deserializeJson() failed: ");
    Serial.println(erro.c_str());
    return;
  } 
  else 
  {
    // Obtem o root do docuemento JSON
    JsonObject root = doc.as<JsonObject>();

    // Processamento dos valores
    const char* sensor = root["sensor"];
    const char* hora = root["hora"];
    double temperatura = root["info"][0];
    double umidade = root["info"][1];

    // Mostra os valores
    Serial.println(" ");
    Serial.println(sensor);
    Serial.println(hora);
    Serial.println(temperatura, 6);
    Serial.println(umidade, 6);
  }
  delay(5000);
}
