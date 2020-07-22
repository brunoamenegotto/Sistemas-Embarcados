#include <DC_Motor.h>
#include <RCSwitch.h>

// Configuracoes iniciais
#define pinoRF      5  //D1
#define PWM_B       2  //D4
#define PWM_A       0  //D3
#define STBY        16 //D0
#define AI2         15 //D8
#define AI1         13 //D7
#define BI1         12 //D6
#define BI2         14 //D5

RCSwitch receptor = RCSwitch(); // Cria instancia de RCSwitch para recepção de informações
DC_Motor motorEsquerda(BI1, BI2, PWM_B);
DC_Motor motorDireita(AI1, AI2, PWM_A);

void setup() {
  Serial.begin(115200);
  receptor.enableReceive(pinoRF); // O receptor sera conectado ao pino "pinoRF"
  pinMode(STBY, OUTPUT);
  digitalWrite(STBY, HIGH);
}

void loop() {
  if (receptor.available()) { // Verifica se existe informação disponível
    int valor = receptor.getReceivedValue(); // Armazena o valor da informação
    if (valor == 0) {
      Serial.print("Código desconhecido.");
    } else {
      Serial.print("Recebido ");
      Serial.print(valor, HEX);
      Serial.print(" / ");
      Serial.print(receptor.getReceivedBitlength()); // Obtem o valor do tamanho da informação
      Serial.print("bit ");
      Serial.print("Protocolo: ");
      Serial.println(receptor.getReceivedProtocol()); // Obtem o valor do protocolo de transmissão
      if (valor == 0x82B2295) { // Testa o valor da informação recebida
        motorDireita.horario();
        motorEsquerda.horario();
        Serial.println("Motor Frente");
      }
      if (valor == 0x82B22A5) {
        motorDireita.parar();
        motorEsquerda.parar();
        Serial.println("Motor Parado");
      }
    }
    receptor.resetAvailable(); // Prepara receptor para receber nova informação
  }
}
