/* 
  LoRa Half-Duplex
  CEFAST Aerospace
  Autor: Matheus Guilherme Henriques
  Atençao: Código instável no arduino! (Provavel motivo: pinos digitais enviam 5V ao inves de 3.3V)
  Fiação:
    VCC  -> 3.3 (NUNCA 5V)
    GND  -> GND
    MOSI -> ESP32: 23
    MISO -> ESP32: 19
    SCK  -> ESP32: 18
    NSS  -> ESP32: 5
    RESET-> ESP32: 14
    DIO0 -> ESP32: 26
*/

#include <SPI.h>
#include <LoRa.h>

const long LORA_FREQ = 433E6; //433 MHz, frequencia alocada com o LASC e a permitida para uso de acordo com a Licença da ANATEL
const int LORA_SF = 7; //Spreading Factor (7 a 12), quanto menor mais rápido mas menor alcance
const long LORA_BW = 125E3; //125 KHz, de acordo com a Licença da ANATEL
const int LORA_CR = 5; //Coding rate (5 a 8), quanto maior, mais robustez contra erros na transmissao, mas mais lenta é a transmissao
const int LORA_TXPOWER = 17; //Intensidade do sinal, máximo do módulo é 18 dBm
const unsigned long PING_INTERVAL_MS = 5000UL; //Intervalo entre pings em milisegundos

const int LORA_SCK_PIN = 18;
const int LORA_MISO_PIN = 19;
const int LORA_MOSI_PIN = 23;
const int LORA_CS_PIN = 5; //NSS
const int LORA_RST_PIN = 14;
const int LORA_DIO0_PIN = 26;

unsigned long lastPingMs = 0;
unsigned long pingCounter = 0;

void sendMessage(const String &msg){
  Serial.print(F("Enviando: ")); //Debug
  Serial.println(msg); //Debug

  LoRa.idle(); //Sai de receptor e entra para transmissor
  LoRa.beginPacket(); 
  LoRa.print(msg);
  LoRa.endPacket(); 

  LoRa.receive(); //Volta para receptor
  Serial.println(F("Tx ok. Voltando a RX.")); //Debug
}

void readMessage(){
  int packetSize = LoRa.parsePacket(); //Verifica se há novo pacote
  if(packetSize){
    String incoming = "";
    while(LoRa.available()){
      incoming += (char)LoRa.read(); //Concatena mensagem
    }
    incoming.trim();
    if(incoming.length()){
      Serial.print(F("Recebido: ")); //Debug
      Serial.println(incoming); //Debug
    }
  }
}

void sendPing(){
  unsigned long now = millis(); //Tempo atual
  if(now - lastPingMs >= PING_INTERVAL_MS){ //Verifica se há o tempo definido entre os pings
    lastPingMs = now;
    String ping = String("PING #") + String(pingCounter++);
    sendMessage(ping);
  }
}

void manualSendMessageViaSerial(){
  if(Serial.available()){
    String line = Serial.readStringUntil('\n'); //Le entrada digitada no serial
    line.trim();
    if (line.length()) {
      sendMessage(line); //Envia o que foi digitado no serial
    }
  }
}


void setup() {
  Serial.begin(115200);

  SPI.begin(LORA_SCK_PIN, LORA_MISO_PIN, LORA_MOSI_PIN, LORA_CS_PIN); //Init SPI
  LoRa.setPins(LORA_CS_PIN, LORA_RST_PIN, LORA_DIO0_PIN); //Define pinagem do LoRa

  if (!LoRa.begin(LORA_FREQ)) {
    Serial.println(F("Erro: falha ao inicializar o módulo! Verifique alimentaçao e conexoes."));
    while (true) {
      delay(1000); //Trava execuçao se houver erro
    }
  }

  //Definiçao de parametros
  LoRa.setSpreadingFactor(LORA_SF);
  LoRa.setSignalBandwidth(LORA_BW);
  LoRa.setCodingRate4(LORA_CR);
  LoRa.setTxPower(LORA_TXPOWER);

  //Iniciar em modo receptor
  LoRa.receive();

  //Debug
  Serial.println(F("LoRa inicializado com sucesso."));
  Serial.println(F("Digite uma mensagem no Monitor Serial e pressione Enter para enviar."));
  Serial.println();
}

void loop() {
  readMessage();
  sendPing(); //Debug
  manualSendMessageViaSerial();
}
