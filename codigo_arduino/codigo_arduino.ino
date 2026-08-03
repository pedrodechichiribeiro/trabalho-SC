#include <Wire.h>

#define AS5600_ADDR 0x36      // Endereço I2C do sensor
#define ANGLE_REG_MSB 0x0E    // Registrador de leitura do ângulo

// motor Nenma 17

#define DIR_PIN 8             // pino de direcao
#define STEP_PIN 6            // pino pulso de passo
#define EN_PIN 4              // pino que liga/desliga o driver do motor

// A maioria dos motores NEMA 17 tem 1.8º por passo (360 / 1.8 = 200 passos)
#define PASSOS_POR_VOLTA 200


float anguloInicial = 0.0;


float lerAnguloBruto() {
  Wire.beginTransmission(AS5600_ADDR);
  Wire.write(ANGLE_REG_MSB);
  Wire.endTransmission(false);
  Wire.requestFrom(AS5600_ADDR, 2);
  
  if (Wire.available() == 2) {
    uint16_t highByte = Wire.read();
    uint16_t lowByte  = Wire.read();
    uint16_t valorBruto = ((highByte & 0x0F) << 8) | lowByte;
    return (valorBruto / 4095.0) * 360.0;
  }
  return -1.0;
}

void setup() {
  // 1. Inicializa a comunicação serial
  Serial.begin(9600);
  Serial.println("Iniciando Sistema: Motor NEMA 17 + Sensor de Angulo...");

  //pinos do Motor
  pinMode(DIR_PIN, OUTPUT);
  pinMode(STEP_PIN, OUTPUT);
  pinMode(EN_PIN, OUTPUT);

  // ativa o motor, o Enable é ativo em nível BAIXO)
  digitalWrite(EN_PIN, LOW); 
  
  // Define uma direção inicial padrão
  digitalWrite(DIR_PIN, HIGH);  // MUDAR DEPOIS 

  // 3. Configuração do Sensor de Ângulo (I2C)
  Wire.begin(); 
  delay(500); // Aguarda o barramento e o sensor estabilizarem

  // calibração do Ponto Zero do Sensor
  anguloInicial = lerAnguloBruto();
  
  if (anguloInicial == -1.0) {
    Serial.println("ERRO: Sensor magnetico nao detectado. Verifique os pinos A4 e A5.");
  } else {
    Serial.print("Angulo inicial lido: ");
    Serial.print(anguloInicial);
    Serial.println(" graus.");
    Serial.println("Sistema calibrado. Assumindo esta posicao como 0 graus.");
  }
  
}


void loop() {
  float anguloAtual = lerAnguloBruto();

  if (anguloAtual != -1.0) {
    // Calcula o ângulo assumindo a posição inicial como zero
    float anguloRelativo = anguloAtual - anguloInicial;

    // Normaliza o ângulo para garantir que ele fique sempre no formato 0 a 360.
    // Exemplo: Se o resultado for -10 graus, ele corrige para 350 graus.
    if (anguloRelativo < 0) {
      anguloRelativo += 360.0;
    }

    // Imprime o resultado no Serial Monitor com 2 casas decimais
    Serial.print("Angulo: ");
    Serial.print(anguloRelativo, 2); 
    Serial.println(" graus");
  } else {
    Serial.println("Erro na leitura I2C.");
  }

  // Atraso de 100ms para não poluir demais o terminal (10 leituras por segundo)
  delay(100); 
}