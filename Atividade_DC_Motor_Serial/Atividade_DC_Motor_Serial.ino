#include <Ultrasonic.h>
#include <DC_Motor.h>
#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>

#define PWM_B       2  //D4
#define PWM_A       0  //D3
#define STBY        16 //D0
#define AI2         15 //D8
#define AI1         13 //D7
#define BI1         12 //D6
#define BI2         14 //D5
#define trigger_pin 5  // D1
#define echo_pin    4  // D2
#define pin_tx      1  // TX
#define pin_rx      3  // RX

DC_Motor        motorEsquerda(BI1, BI2, PWM_B);
DC_Motor        motorDireita(AI1, AI2, PWM_A);
Ultrasonic      ultrasonic(trigger_pin, echo_pin);
SoftwareSerial  blue(pin_tx, pin_rx);

const char* ssid  = "wirelessROBOTICA";
const char* senha = "mecatronica";

int velocidade = 50;
WiFiServer server(80);

/*long lerDistancia(int triggerPin, int echoPin)
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
}*/

void setup()
{
  pinMode(STBY, OUTPUT);
  digitalWrite(STBY, HIGH);
  Serial.begin(115200);
  //blue.begin(9600);
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
  Serial.println("Use este endereço para conectar ao ESP8266");
  Serial.println();

  server.begin();
}

void loop()
{
  /*float cm = lerDistancia(trigger_pin, echo_pin) / 58.0;
  Serial.print("Distancia = ");
  Serial.print(cm);
  Serial.println("cm");
  delay(1000);
  if (blue.available()) 
  {
    char c = blue.read();
    if (c == 'a')
    {
      motorDireita.parar();
      motorEsquerda.horario();
    }
    if (c == 'd')
    {
      motorDireita.horario();
      motorEsquerda.parar();
    }
    if (c == 'w')
    {
      motorDireita.horario();
      motorEsquerda.horario();
    }
    if (c == 's')
    {
      motorDireita.antiHorario();
      motorEsquerda.antiHorario();
    }
    if (c == '0')
    {
      motorDireita.parar();
      motorEsquerda.parar(); 
    }
  }
  /*if(cm <= 20)
  {
      motorDireita.antiHorario();
      motorEsquerda.parar();
      delay(2000);
      motorDireita.antiHorario();
      motorEsquerda.antiHorario();
      delay(1000);
  }
  if (Serial.available()) 
  {
    char c = Serial.read();
    if (c == 'a')
    {
      motorDireita.parar();
      motorEsquerda.horario();
    }
    if (c == 'd')
    {
      motorDireita.horario();
      motorEsquerda.parar();
    }
    if (c == 'w')
    {
      motorDireita.horario();
      motorEsquerda.horario();
    }
    if (c == 's')
    {
      motorDireita.antiHorario();
      motorEsquerda.antiHorario();
    }
    if (c == '0')
    {
      motorDireita.parar();
      motorEsquerda.parar(); 
    }
  }*/
  WiFiClient client = server.available();   // Aguarda requisicoes de clientes
  if (client) {                             // Se cliente conectado,
    String cabec = ""; // Armazenamento HTTP request
    boolean linhaAtualVazia = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        cabec += c; // armazena conteudo do cabecalho para processamento
        // se é o final da linha e a linha está em branco,
        // significa que a solicitação http terminou,
        // desta forma e possível enviar uma resposta http
        if (c == '\n' && linhaAtualVazia) {
          processaGET(cabec);
          // envia um cabecalho de resposta http padrao
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");  // a conexao será fechada após a conclusão da resposta
          client.println();
          // Pagina HTML
          client.println("<!doctype html>");
          client.println("<html>");
          client.println("   <head>");
          client.println("      <meta charset=\"utf-8\">");
          client.println("      <link rel=\"stylesheet\" href=\"https://code.jquery.com/ui/1.12.1/themes/base/jquery-ui.css\">");
          client.println("      <link rel=\"stylesheet\" href=\"https://cdnjs.cloudflare.com/ajax/libs/font-awesome/4.7.0/css/font-awesome.min.css\">");
          client.println("      <link rel=\"shortcut icon\" href=\"https://www.google.com/s2/favicons?domain=www.uepg.br\">");
          client.println("      <script src=\"https://code.jquery.com/jquery-1.12.4.js\"></script>");
          client.println("      <script src=\"https://code.jquery.com/ui/1.12.1/jquery-ui.js\"></script>");
          client.println("      <style>");
          client.println("         .content {");
          client.println("         max-width: 280px;");
          client.println("         margin: auto;");
          client.println("         }");
          client.println("         .grid {");
          client.println("         display: grid;");
          client.println("         border-radius: 1em;");
          client.println("         grid-gap: 10px;");
          client.println("         grid-template-columns: 80px 80px 80px;");
          client.println("         background-color: LAVENDER;");
          client.println("         padding: 10px;");
          client.println("         }");
          client.println("         .grid-item {");
          client.println("         border: none;");
          client.println("         padding: 10px;");
          client.println("         background-color: POWDERBLUE;");
          client.println("         color: White;");
          client.println("         border-radius: 10px;");
          client.println("         font-size: 200%;");
          client.println("         text-align: center;");
          client.println("        }");
          client.println("      </style>");
          client.println("   </head>");
          client.println("   <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">");
          client.println("   <body>");
          client.println("      <div class=\"content\">");
          client.println("         <h3 align=\"center\">Click na seta correspondente ao movimento desejado, para parar click Home.</h3>");
          client.println("         <div class=\"grid\">");
          client.println("            <div class=\"grid-item\"></div>");
          client.println("            <div id=\"btnCima\" class=\"grid-item\"><span class=\"fa\">&#xf0aa;</span></div>");
          client.println("            <div class=\"grid-item\"></div>");
          client.println("            <div id=\"btnEsquerda\" class=\"grid-item\"><span class=\"fa\">&#xf0a8;</span></div>");
          client.println("            <div id=\"btnHome\" class=\"grid-item\"><span class=\"fa\">&#xf015;</span></div>");
          client.println("            <div id=\"btnDireita\" class=\"grid-item\"><span class=\"fa\">&#xf0a9;</span></div>");
          client.println("            <div class=\"grid-item\"></div>");
          client.println("            <div id=\"btnBaixo\" class=\"grid-item\"><span class=\"fa\">&#xf0ab;</span></div>");
          client.println("            <div id=\"btnBaixoDir\" class=\"grid-item\"><span class=\"fa\" style=\"transform: rotate(-45deg);\">&#xf0ab;</span></div>");
          client.println("         </div>");
          client.println("         <p>");
          client.println("           <label for=\"aceleracao\">Aceleração:</label>");
          client.println("           <input type=\"text\" id=\"aceleracao\" readonly style=\"border:0; color:#f6931f; font-weight:bold;\">");
          client.println("         </p>");
          client.println("         <div id=\"slider\"></div>");
          client.println("      </div>");
          client.println("      <script>");
          client.println("         $(document).ready(function(){");
          client.println("             var button = $(\".grid-item\");");
          client.println("             button.on(\"touchstart mousedown\", function(){");
          client.println("                 $(this).css(\"backgroundColor\", \"Blue\");");
          client.println("                 window.location.pathname = \"/\" + $(this)[0].id + \";\" + $(\"#slider\").slider(\"value\");");
          client.println("             });");
          client.println("             $(\"#slider\").slider({");
          client.println("               value:" + String(velocidade) + ",");
          client.println("               min: 0,");
          client.println("               max: 100,");
          client.println("               step: 10,");
          client.println("               slide: function(event, ui) {");
          client.println("                 $(\"#aceleracao\" ).val(ui.value);");
          client.println("               }");
          client.println("             });");
          client.println("             $(\"#aceleracao\").val($(\"#slider\").slider(\"value\"));");
          client.println("          });");
          client.println("      </script>");
          client.println("  </body>");
          client.println("</html>");
          // Envia a HTTP response
          client.println();
          break;
        }
        if (c == '\n') {  // inicio de uma nova linha
          linhaAtualVazia = true;
        } else if (c != '\r') {  // existe pelo menos um caracter na linha atual
          linhaAtualVazia = false;
        }
      }
    }
    // um atraso para o web browser processar a resposta
    delay(10);
    // Fecha conexao
    client.stop();
  }
}

void processaGET(String c) {
  if (c.indexOf("GET /") >= 0) {
    String linha = c.substring(c.indexOf("GET /") + 5);
    int ind = 0;
    String info = "";
    while (linha[ind] != '\n') {
      info += linha[ind];
      ind++;
    }
    String acao = info.substring(0, info.indexOf(" HTTP/1.1"));
    String vel = acao.substring(acao.indexOf(";") + 1);
    acao = acao.substring(0, acao.indexOf(";"));
    velocidade = vel.toInt();
    Serial.println(acao);
    Serial.println(velocidade);
    motorEsquerda.velocidade(velocidade);
    motorDireita.velocidade(velocidade);
    if (acao == "btnCimaEsq") {
    }
    else if (acao == "btnCima") {
      motorEsquerda.horario();
      motorDireita.horario();
    }
    else if (acao == "btnCimaDir") {
      //
    }
    else if (acao == "btnEsquerda") {
      motorEsquerda.parar();
      motorDireita.horario();
    }
    else if (acao == "btnHome") {
      motorEsquerda.parar();
      motorDireita.parar();
    }
    else if (acao == "btnDireita") {
      motorEsquerda.horario();
      motorDireita.parar();
    }
    if (acao == "btnBaixoEsq") {
      //
    }
    else if (acao == "btnBaixo") {
      motorEsquerda.antiHorario();
      motorDireita.antiHorario();
    }
  }/*
  motorDireita.parar();
  motorEsquerda.parar();
  delay(1000);*/
}
