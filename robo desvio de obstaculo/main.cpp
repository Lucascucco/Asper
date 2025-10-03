/*
   Robô Desvio de Obstáculos (Sensor Fixo)
   
   - Adaptado para Robô Aspirador 4 Motores (M2/M3 Locomoção; M1/M4 Limpeza).
   - Sensor HC-SR04 fixo, sem Servo Motor para escanear.
   - Lógica de desvio simplificada: ao encontrar um obstáculo, recua e força um giro para a direita.
*/

// --- Bibliotecas Auxiliares ---
#include <AFMotor.h>         // Inclui biblioteca AF Motor
// A biblioteca Servo.h foi removida.
 

// --- Seleção e Mapeamento dos Motores ---
AF_DCMotor motor_limpeza1(1); // M1: Motor das Escovas/Cerdas (Limpeza 1)
AF_DCMotor motor_esquerda(2); // M2: Motor de Locomoção Esquerdo (Roda)
AF_DCMotor motor_direita(3);  // M3: Motor de Locomoção Direito (Roda)
AF_DCMotor motor_limpeza2(4); // M4: Motor das Escovas/Cerdas (Limpeza 2)



// --- Mapeamento de Hardware (Ultrassônico) ---

#define   trig        A4                 // Pino de trigger do sensor HC-SR04
#define   echo        A5                 // Pino de echo do sensor HC-SR04

// --- Sensores IR ---
#define ir_esq 7   // Sensor IR esquerdo
#define ir_dir 8   // Sensor IR direito

// --- Protótipo das Funções Auxiliares ---
float measureDistance();                // Mede e retorna a distância em cm
void trigPulse();                       // Gera o pulso de trigger
void decision_fixed();                  // Função para desvio de obstáculo (lógica fixa)
void robot_forward(unsigned char v);    // Move robô para frente
void robot_backward(unsigned char v);   // Move robô para trás
void robot_left(unsigned char v);       // Move robô para esquerda
void robot_right(unsigned char v);      // Move robô para direita
void robot_stop();                      // Para o robô (locomoção)


// --- Variáveis Globais ---
unsigned char velocidade_locomocao = 0xC8; // 200 em Hex (Velocidade das rodas)
unsigned char velocidade_limpeza = 180;    // Velocidade das cerdas (Fixo)

float dist_cm;                         // Distância em centímetros (frente)

// --- Configurações Iniciais ---
void setup()
{
  
  // 1. Configuração dos pinos do sensor
  pinMode(trig, OUTPUT);                 
  pinMode(echo, INPUT);                        
 
  pinMode(ir_esq, INPUT);
  pinMode(ir_dir, INPUT);
  
  digitalWrite(trig, LOW);
  delay(500);                                  
  
  // 2. LIGA OS MOTORES DE LIMPEZA (M1 e M4) E OS MANTÉM LIGADOS
  motor_limpeza1.setSpeed(velocidade_limpeza); 
  motor_limpeza1.run(FORWARD);
  motor_limpeza2.setSpeed(velocidade_limpeza); 
  motor_limpeza2.run(FORWARD);
  
} //end setup


// --- Loop Infinito ---
void loop()
{
  
     // 1. O robô avança e verifica a distância constantemente
     robot_forward(velocidade_locomocao);
     delay(80); 
  
     dist_cm = measureDistance();// Mede a distância à frente
     int irE = digitalRead(ir_esq);
     int irD = digitalRead(ir_dir);
  
     if(dist_cm < 20) // Se a distância for menor que 20 cm
     {
         decision_fixed(); // Entra na rotina de desvio
     }
     else if (irE == LOW) { 
          // Obstáculo na esquerda → gira direita
          robot_right(velocidade_locomocao);
          delay(500);
     }
     else if (irD == LOW) { 
          // Obstáculo na direita → gira esquerda
          robot_left(velocidade_locomocao);
          delay(500);
  }
} //end loop


// --- Desenvolvimento das Funções Auxiliares ---


float measureDistance()                       // Função que retorna a distância em centímetros
{
  float pulse;                                
        
  trigPulse();                                
  
  pulse = pulseIn(echo, HIGH);                
    
  // Cálculo da Conversão de µs para cm (34000 cm/s / 2)
  return (pulse/58.82);                      
  
} //end measureDistante


void trigPulse()                             // Gera o pulso de trigger
{
   digitalWrite(trig,HIGH);                  
   delayMicroseconds(10);                    
   digitalWrite(trig,LOW);                   

} //end trigPulse


void decision_fixed()                        // Rotina de desvio simplificada (sem escanear)
{
   robot_stop();                             // 1. PARA a locomoção (Limpeza continua)
   delay(500);                               
   
   robot_backward(velocidade_locomocao);     // 2. Recua para ganhar espaço
   delay(600);                            
   
   // 3. Força um giro para a DIREITA (pode ser LEFT se preferir)
   robot_right(velocidade_locomocao);     
   delay(800);                            
   
   robot_forward(velocidade_locomocao);      // 4. Volta a avançar (e o loop verifica a nova distância)

} //end decision_fixed

 
// --- FUNÇÕES DE LOCOMOÇÃO (APENAS M2 e M3) ---
// ** Lógica de Giro do motor_esquerda (M2) é INVERTIDA **

void robot_forward(unsigned char v)
{
     // AÇÃO: Reta para Frente
     motor_direita.setSpeed(v);
     motor_direita.run(FORWARD);   // Motor Direito: AVANÇA
     motor_esquerda.setSpeed(v);
     motor_esquerda.run(BACKWARD); // Motor Esquerdo: AVANÇA (Comando invertido)
} //end robot forward


void robot_backward(unsigned char v)
{
     // AÇÃO: Reta para Trás
     motor_direita.setSpeed(v);
     motor_direita.run(BACKWARD);  // Motor Direito: RECUA
     motor_esquerda.setSpeed(v);
     motor_esquerda.run(FORWARD);  // Motor Esquerdo: RECUA (Comando invertido)
} //end robot backward


void robot_left(unsigned char v)
{
     // AÇÃO: Giro no lugar para a ESQUERDA (Direita AVANÇA, Esquerda RECUA)
     motor_direita.setSpeed(v);
     motor_direita.run(FORWARD);   // Motor Direito: AVANÇA
     motor_esquerda.setSpeed(v);
     motor_esquerda.run(FORWARD);  // Motor Esquerdo: RECUA (Comando invertido)
} //end robot left


void robot_right(unsigned char v)
{
     // AÇÃO: Giro no lugar para a DIREITA (Esquerda AVANÇA, Direita RECUA)
     motor_direita.setSpeed(v);
     motor_direita.run(BACKWARD);  // Motor Direito: RECUA
     motor_esquerda.setSpeed(v);
     motor_esquerda.run(BACKWARD); // Motor Esquerdo: AVANÇA (Comando invertido)
} //end robot right


void robot_stop()
{
     // AÇÃO: Parar APENAS os motores de locomoção (M2 e M3)
     motor_direita.run(RELEASE);
     motor_esquerda.run(RELEASE);
} //end robot stop