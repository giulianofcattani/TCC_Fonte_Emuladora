#include <Arduino.h>
#ifdef ESP8266
#include <ESP8266WiFi.h>
#else // ESP32
#include <WiFi.h>
#endif
#include <ModbusIP_ESP8266.h>

ModbusIP mb;

// Constantes dos registradores Modbus
const uint16_t SENSOR_HREG = 0;    // Endereço do primeiro registrador
const uint16_t valorInicial = 0;   // Valor inicial dos registradores
const uint16_t quantRegist = 10;   // Número de registradores

// Variáveis de dados
uint16_t tensaoRand1, correnteRand1, potRand1;
uint16_t tensaoRand2, correnteRand2, potRand2;
uint16_t tensaoRand3, correnteRand3, potRand3;
uint16_t tempAmb;
uint16_t valorLido;
long ts;

void setup()
{
  Serial.begin(115200);
  
  // Conectar à rede Wi-Fi
  WiFi.begin("login", "senha");

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi conectado");
  Serial.println("Endereço IP: ");
  Serial.println(WiFi.localIP());

  // Inicializar o servidor Modbus e adicionar os registradores
  mb.server();
  mb.addHreg(SENSOR_HREG, valorInicial, quantRegist);

  ts = millis();
}

void loop()
{
  // Lidar com a comunicação Modbus
  mb.task();

  // Ler o valor atual do registrador Modbus: Controle da fonte
  // Constante passada pelo supervisório para incrementar o decrementar a tensão da fonte
  // Seria o valor a ser escrito
  valorLido = mb.Hreg(SENSOR_HREG);

  // Emula os valores de tensão lidos pela fonte emuladora e soma/diminui
  tensaoRand1 = random(210, 220) + valorLido;
  tensaoRand2 = random(210, 220) + valorLido;
  tensaoRand3 = random(210, 220) + valorLido;

  // Emula os valores de corrente lidos pela fonte emuladora
  correnteRand1 = random(5, 10);
  correnteRand2 = random(5, 10);
  correnteRand3 = random(5, 10);

  // Emula os valores de potência lidos pela fonte emuladora
  potRand1 = random(2150, 2200);
  potRand2 = random(2150, 2200);
  potRand3 = random(2150, 2200);
  
  // Gerar temperatura ambiente aleatória
  tempAmb = random(20, 36);

  // Atualizar os registradores Modbus com os novos valores
  mb.Hreg(1, tensaoRand1);
  mb.Hreg(2, tensaoRand2);
  mb.Hreg(3, tensaoRand3);

  mb.Hreg(4, correnteRand1);
  mb.Hreg(5, correnteRand2);
  mb.Hreg(6, correnteRand3);

  mb.Hreg(7, potRand1);
  mb.Hreg(8, potRand2);
  mb.Hreg(9, potRand3);

  mb.Hreg(10, tempAmb);

}