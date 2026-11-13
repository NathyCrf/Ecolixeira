#include <ESP32Servo.h>
#include "BluetoothSerial.h"

// ==========================================
// 1. CONFIGURAÇÃO DAS FERRAMENTAS
// ==========================================
BluetoothSerial SerialBT; // Ativa a comunicação Bluetooth para o celular
Servo servoTampa;         // Cria o controlador virtual do micro servo motor

// Pinos do Sensor da Mão (Responsável por abrir a tampa)
const int trigMao = 5;    // Emite a onda de som
const int echoMao = 19;   // Ouve o retorno do som

// Pinos do Sensor do Lixo (Responsável por medir a profundidade interna)
const int trigLixo = 12;  // Emite a onda de som
const int echoLixo = 14;  // Ouve o retorno do som

// Pino do Motor da Tampa
const int pinoMotor = 18; // Envia o sinal elétrico para o servo motor

void setup() {
  // Inicia a comunicação com o computador para testes no Monitor Serial
  Serial.begin(115200);

  // Inicia o Bluetooth e define o nome do dispositivo
  SerialBT.begin("Lixeira_Inteligente");

  // Configura a direção da energia de cada pino
  pinMode(trigMao, OUTPUT);
  pinMode(echoMao, INPUT);
  pinMode(trigLixo, OUTPUT);
  pinMode(echoLixo, INPUT);

  // Conecta o motor ao pino D18 e força a tampa a iniciar fechada (ângulo 0)
  servoTampa.attach(pinoMotor);
  servoTampa.write(0);
}

// ==========================================
// 2. FUNÇÃO DE LEITURA ULTRASSÔNICA
// ==========================================
int medirDistancia(int pinoTrig, int pinoEcho) {
  digitalWrite(pinoTrig, LOW);
  delayMicroseconds(2);
  digitalWrite(pinoTrig, HIGH);
  delayMicroseconds(10);
  digitalWrite(pinoTrig, LOW);

  // Conta o tempo de retorno do som (timeout de 30000 us evita travamentos)
  long tempo = pulseIn(pinoEcho, HIGH, 30000);

  if (tempo == 0) {
    return -1;
  }

  // Converte o tempo da velocidade do som em centímetros
  int distancia = (tempo * 0.034) / 2;
  return distancia;
}

// ==========================================
// 3. CICLO PRINCIPAL
// ==========================================
void loop() {
  int distanciaMao = medirDistancia(trigMao, echoMao);
  int distanciaLixo = medirDistancia(trigLixo, echoLixo);

  Serial.print("Mao: ");
  Serial.print(distanciaMao);
  Serial.print(" cm | Lixo: ");
  Serial.print(distanciaLixo);
  Serial.println(" cm");

  // --- AÇÃO 1: ABRIR A TAMPA ---
  if (distanciaMao > 0 && distanciaMao <= 10) {
    Serial.println(">>> ABRINDO TAMPA <<<");
    servoTampa.write(90);
    delay(10000);
    servoTampa.write(0);
    Serial.println("<<< FECHANDO TAMPA >>>");
  }

  // --- AÇÃO 2: AVISO DE LIXEIRA CHEIA ---
  if (distanciaLixo > 0 && distanciaLixo <= 10) {
    SerialBT.println("ALERTA: A lixeira está cheia!");
  }

  delay(500);
}
