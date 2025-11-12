// PROJETO MVN by Carlos Silva


// BIBLIOTECAS

// Inclui a biblioteca da Adafruit
#include <LiquidCrystal_I2C.h>



// RemoteXY seleciona o modo de conexão e inclui biblioteca
#define REMOTEXY_MODE__ESP8266_HARDSERIAL_POINT


// RemoteXY configurações de conexão
#define REMOTEXY_SERIAL Serial
#define REMOTEXY_SERIAL_SPEED 115200
#define REMOTEXY_WIFI_SSID "PROJETO MVN"
#define REMOTEXY_WIFI_PASSWORD "12345678"
#define REMOTEXY_SERVER_PORT 6377


#include <RemoteXY.h>

// RemoteXY configuração GUI   
#pragma pack(push, 1)  
uint8_t RemoteXY_CONF[] =   // 106 bytes
  { 255,4,0,0,0,99,0,19,0,0,0,0,6,1,106,200,1,1,5,0,
  1,36,61,36,36,8,188,16,79,78,32,92,32,79,70,70,0,1,9,126,
  24,24,0,176,31,78,105,118,101,108,32,49,0,1,40,126,24,24,0,176,
  31,78,195,173,118,101,108,32,50,0,1,71,126,24,24,0,176,31,78,195,
  173,118,101,108,32,51,0,129,12,30,82,12,64,16,80,82,79,74,69,84,
  79,32,77,86,78,0 };
  
// Esta estrutura define todas as variáveis e eventos que tem no controle remoto
struct {

    // input variaveis
  uint8_t ligar; 
  uint8_t bnv1; 
  uint8_t bnv2; 
  uint8_t bnv3; 

    // outra variavel para conexão
  uint8_t connect_flag;  // =1 if wire connected, else =0

} RemoteXY;   
#pragma pack(pop)


// DECLARAÇÃO DE VARIAVEIS

// Cria um objeto 'lcd_1' do tipo LiquidCrystal_I2C
// Inicializa o display LCD com modelo padrão 16x2
LiquidCrystal_I2C lcd_1(0x27, 16, 2);



// Declaração de Pinos

int bnv1 = 2; 
int bnv2 = 3;
int bnv3 = 4;

int nv1 = 5;
int nv2 = 6;
int nv3 = 7;

int ligar = 8;

int encher = 9;
int molho = 10;
int bater = 11;
int motor = 12;
int pronto = 13;


// Variáveis de estado

//LCD
// Variável para guardar o estado da luz de fundo (1 = ligada, 0 = desligada)
int backlightState = 1;


// LIGAR E DESLIGAR
const int leds[] = {13, 11, 10, 9};  
const int qtdLeds = 4; 
int on = 0;
int off = 0;
bool ligado = 0;

// NIVEIS
int nivelSelecionado = 0;  // 0 = nenhum, 1 = nv1, 2 = nv2, 3 = nv3

  

void setup() {
  
  
  RemoteXY_Init (); 

  // Inicializa o LCD
  lcd_1.init();          
  
  // Sensores pullup's
  pinMode(nv1, INPUT_PULLUP);
  pinMode(nv2, INPUT_PULLUP);
  pinMode(nv3, INPUT_PULLUP);

  // Led's
  pinMode(encher, OUTPUT);
  pinMode(molho, OUTPUT);
  pinMode(bater, OUTPUT);
  pinMode(pronto, OUTPUT);
  
  // Motor
  pinMode(motor, OUTPUT);

  // Botão de ligar/desligar
  pinMode(ligar, INPUT_PULLUP);
  for (int i = 0; i < qtdLeds; i++) {
    pinMode(leds[i], OUTPUT);
  } 

  // Botões pullup's
  pinMode(bnv1, INPUT_PULLUP);
  pinMode(bnv2, INPUT_PULLUP);
  pinMode(bnv3, INPUT_PULLUP);
}





void loop() {   

  RemoteXY_Handler ();

  
////////////////////////LIGAR E DESLIGAR//////////////////////////

// Lê o botão físico e o botão do app
bool botaoFisico = (digitalRead(ligar) == LOW);  //  LOW = pressionado
bool botaoApp = (RemoteXY.ligar == 1);           // 1 = pressionado no app

// Combina os dois (se qualquer um for pressionado, "on" será true)
bool onPressionado = botaoFisico || botaoApp;

// Detecta transição: quando o botão foi pressionado (borda de descida)
if (onPressionado && !off) {
  ligado = !ligado;  // Inverte o estado do sistema
}

// Comportamento quando ligado
if (ligado) {

  lcd_1.setBacklight(1);
  lcd_1.display();

  // Animação dos LEDs
  for (int i = 0; i < qtdLeds; i++) {
    digitalWrite(leds[i], HIGH);
    RemoteXY_delay(500);
    digitalWrite(leds[i], LOW);
  }

  lcd_1.setCursor(0, 0);
  lcd_1.print("Projeto MVN");
  RemoteXY_delay(1000);
  lcd_1.setCursor(0, 1);
  lcd_1.print("Escolha Nv. Agua");

} else {

  lcd_1.setBacklight(0);
  lcd_1.noDisplay();

  // Garante que todos LEDs fiquem apagados quando desligado
  for (int i = 0; i < qtdLeds; i++) {
    digitalWrite(leds[i], LOW);
  }
}

// Atualiza o estado anterior
off = onPressionado;


/////////////////////////PROCESSO TANQUINHO////////////////////////////////     
 
 if (ligado) {

    processoEncher();
     
    if (nivelSelecionado != 0) {
      
processoBater();  
processoMolho();
processoBater();
processoPronto();          
finalizado();        
               
    }



  }

}      






/////////////////////////VOID'S//////////////////////////////// 

void processoEncher(){   
 
     RemoteXY_Handler ();

  // Seleciona o nível da água baseado nos botões (botões com INPUT_PULLUP, LOW = pressionado)
    if (digitalRead(bnv1) == LOW || RemoteXY.bnv1 == 1 ) {
    
    lcd_1.clear();   
    lcd_1.setCursor(0, 0);  
    lcd_1.print("Nv. 1 escolhido"); 
    RemoteXY_delay(1000);
    lcd_1.setCursor(0, 1);
    lcd_1.print("Enchendo...");  
    digitalWrite(encher, HIGH);
      
    nivelSelecionado = 1;
    
    sensor1();
      
    } else if (digitalRead(bnv2) == LOW || RemoteXY.bnv2 == 1) {
    
    lcd_1.clear();   
    lcd_1.setCursor(0, 0);
    lcd_1.print("Nv. 2 escolhido "); 
    RemoteXY_delay(1000);
    lcd_1.setCursor(0, 1);
    lcd_1.print("Enchendo...");  
    digitalWrite(encher, HIGH);
      
    nivelSelecionado = 2;
      
    sensor2();  
    
    } else if (digitalRead(bnv3) == LOW || RemoteXY.bnv3 == 1) {
    
    lcd_1.clear();   
    lcd_1.setCursor(0, 0);
    lcd_1.print("Nv. 3 escolhido ");
    RemoteXY_delay(1000);
    lcd_1.setCursor(0, 1);
    lcd_1.print("Enchendo...");  
    digitalWrite(encher, HIGH);
      
    nivelSelecionado = 3;
      
    sensor3();  
    
    } else {
      
      nivelSelecionado = 0; // Nenhum selecionado
    }
  
      
   
}
      
      

void processoBater(){ 

  RemoteXY_Handler ();
  
      // 2. Ligar bater
        
    lcd_1.clear();   
    lcd_1.setCursor(0, 0);
    lcd_1.print("Motor ativo");
    digitalWrite(bater, HIGH);  
    RemoteXY_delay(1000);
    lcd_1.setCursor(0, 1);
    lcd_1.print("Batendo...");
    digitalWrite(motor, 1); 
      
      RemoteXY_delay(20000);  // Tempo de bater (exemplo 20 segundos)
     
	digitalWrite(motor, 0); 
    digitalWrite(bater, LOW);
} 
  


void processoMolho(){ 

  RemoteXY_Handler ();      
      
  // 3. Ligar molho
      
    lcd_1.clear();   
    lcd_1.setCursor(0, 0);
    lcd_1.print("Motor inativo");
    RemoteXY_delay(1000);
    lcd_1.setCursor(0, 1);
    lcd_1.print("Molho...");  
    digitalWrite(molho, HIGH);
      
      RemoteXY_delay(10000);  // Tempo de molho (exemplo 10 segundos)
      
    digitalWrite(molho, LOW);
}
      
      

void processoPronto(){   

  RemoteXY_Handler ();   
      
      // 4. Ligar pronto
      
	lcd_1.clear();   
    lcd_1.setCursor(0, 0);
    lcd_1.print("Motor inativo");
    RemoteXY_delay(1000);
    lcd_1.setCursor(0, 1);
    lcd_1.print("Esta pronto!!!!");
      
    digitalWrite(pronto, HIGH);
      
    // Tempo pronto (exemplo 12 segundos)  
    lcd_1.setBacklight(0);
    RemoteXY_delay(1000);  
  	lcd_1.setBacklight(1);
    RemoteXY_delay(2000); 
	  lcd_1.setBacklight(0);
    RemoteXY_delay(1000);  
  	lcd_1.setBacklight(1);
    RemoteXY_delay(2000); 
	  lcd_1.setBacklight(0);
    RemoteXY_delay(1000);  
  	lcd_1.setBacklight(1);
    RemoteXY_delay(5000); 
	  lcd_1.setBacklight(0); 
	
    lcd_1.clear();  
      digitalWrite(pronto, LOW);
      
}      
 
void finalizado(){
 
 RemoteXY_Handler ();

  // Após o processo, desliga tudo e reseta nível
      for (int i = 0; i < qtdLeds; i++) {
        digitalWrite(leds[i], LOW);
      }
      
      nivelSelecionado = 0;
      ligado = false;  // Desliga o sistema após o ciclo completo

}







void sensor1() {

RemoteXY_Handler ();

  while (digitalRead(nv1) == HIGH) {
    // HIGH significa "não pressionado" (por causa do INPUT_PULLUP)
    RemoteXY_delay(50); // evita leitura instável
  }
  digitalWrite(encher, LOW);
  
}




void sensor2() {

RemoteXY_Handler ();

  while (digitalRead(nv2) == HIGH) {
    // HIGH significa "não pressionado" (por causa do INPUT_PULLUP)
    RemoteXY_delay(50); // evita leitura instável
  }
  digitalWrite(encher, LOW);
  
}




void sensor3() {

RemoteXY_Handler ();

  while (digitalRead(nv3) == HIGH) {
    // HIGH significa "não pressionado" (por causa do INPUT_PULLUP)
    RemoteXY_delay(50); // evita leitura instável
  }
  digitalWrite(encher, LOW);
  
}
  
  
