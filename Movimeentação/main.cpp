// --- Bibliotecas ---
#include <AFMotor.h>

// --- Seleção dos Motores ---
AF_DCMotor motor1(2); // Seleção do Motor 1 (Ex: Lado Direito)
AF_DCMotor motor2(3); // Seleção do Motor 2 (Ex: Lado Esquerdo)

// --- Protótipo das Funções Auxiliares ---
void robot_forward(unsigned char v);  // Função movimenta robô frente
void robot_backward(unsigned char v); // Função movimenta robô trás
void robot_left(unsigned char v);     // Função movimenta robô esquerda
void robot_right(unsigned char v);    // Função movimenta robô direita
void robot_stop(unsigned char v);     // Função para para o robô

// --- Variáveis Globais ---
// Definindo uma velocidade mais lenta (150 é a metade da máxima 255)
unsigned char velocidade = 150; // Armazena velocidade motores (0 a 255)

// --- Configurações Iniciais ---
void setup()
{
  // A biblioteca configura as entradas e saídas pertinentes...  
  // O valor de "velocidade" (150) já está definido e será usado no loop
} // end setup


// --- Loop Infinito ---
void loop()
{
      robot_forward(velocidade);
      delay(500); // Avança por 0.5 segundos
  
      robot_backward(velocidade);
      delay(500); // Recua por 0.5 segundos
      
      robot_right(velocidade);
      delay(500); // Gira à direita por 0.5 segundos
      
      robot_left(velocidade);
      delay(500); // Gira à esquerda por 0.5 segundos
      
      robot_stop(velocidade);
      delay(500); // Para por 0.5 segundos
} // end loop

// --- Desenvolvimento das Funções Auxiliares ---

void robot_forward(unsigned char v)
{
      motor1.setSpeed(v);
      motor1.run(FORWARD);
      motor2.setSpeed(v);
      motor2.run(FORWARD);
      
} // end robot forward


void robot_backward(unsigned char v)
{
      motor1.setSpeed(v);
      motor1.run(BACKWARD);
      motor2.setSpeed(v);
      motor2.run(BACKWARD);

} // end robot backward

// FUNÇÕES DE GIRO CORRIGIDAS (Tank Steering)

void robot_left(unsigned char v)
{
      // Para girar à esquerda, um motor AVANÇA e o outro RECUA.
      motor1.setSpeed(v);
      motor1.run(FORWARD);  // Motor 1 (Direita) avança (ex.)
      motor2.setSpeed(v);
      motor2.run(BACKWARD); // Motor 2 (Esquerda) recua (ex.)
} // end robot left


void robot_right(unsigned char v)
{
      // Para girar à direita, a direção dos motores é invertida.
      motor1.setSpeed(v);
      motor1.run(BACKWARD); // Motor 1 (Direita) recua (ex.)
      motor2.setSpeed(v);
      motor2.run(FORWARD);  // Motor 2 (Esquerda) avança (ex.)
} // end robot right


void robot_stop(unsigned char v)
{
      // RELEASE desliga a corrente dos motores
      motor1.run(RELEASE);
      motor2.run(RELEASE);
} // end robot stop