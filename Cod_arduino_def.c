// BIBLIOTECAS

// Inclui a biblioteca da Adafruit
#include <Adafruit_LiquidCrystal.h>

// DECLARAÇÃO DE VARIAVEIS

// Cria um objeto 'lcd_1' do tipo Adafruit_LiquidCrystal
// O número (0) representa o endereço do canal de comunicação
Adafruit_LiquidCrystal lcd_1(0);


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
  
  // Inicializa o display LCD com modelo padrão 16x2
  lcd_1.begin(16, 2);
  
  
  // Led's
  
  pinMode(nv1, INPUT_PULLUP);
  pinMode(nv2, INPUT_PULLUP);
  pinMode(nv3, INPUT_PULLUP);

  
  pinMode(encher, OUTPUT);
  pinMode(molho, OUTPUT);
  pinMode(motor, OUTPUT);
  pinMode(bater, OUTPUT);
  pinMode(pronto, OUTPUT);
  
  
  // Botões pullup's
  
  pinMode(ligar, INPUT_PULLUP);
  for (int i = 0; i < qtdLeds; i++) {
    pinMode(leds[i], OUTPUT);
  } 
  pinMode(bnv1, INPUT_PULLUP);
  pinMode(bnv2, INPUT_PULLUP);
  pinMode(bnv3, INPUT_PULLUP);

}





void loop() {   

////////////////////////LIGAR E DESLIGAR//////////////////////////
  
  // Leitura do botão (LOW = pressionado, HIGH = solto, pois usamos INPUT_PULLUP)
  on = digitalRead(ligar);

  // Detecta borda de descida (quando botão foi pressionado)
  if (on == LOW && off == HIGH) {
    // Inverte o estado do LED
    ligado = !ligado;
  }
  
  
    // Se efeito estiver ligado → LEDs correm
  if (ligado) {
    
    lcd_1.setBacklight(1);
    lcd_1.display();
    
      for (int i = 0; i < qtdLeds; i++) {
      digitalWrite(leds[i], HIGH);  // Acende LED atual
      delay(500);                   // Espera
      digitalWrite(leds[i], LOW);   // Apaga LED atual
    }
    
   
    lcd_1.setCursor(0, 0);
    lcd_1.print("Projeto MVN");
    delay(1000);
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
  // Salva o estado atual do botão
  off = on;  

  
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

















void processoEncher(){     
     
  // Seleciona o nível da água baseado nos botões (botões com INPUT_PULLUP, LOW = pressionado)
    if (digitalRead(bnv1) == LOW) {
    
    lcd_1.clear();   
    lcd_1.setCursor(0, 0);  
    lcd_1.print("Nv. 1 escolhido"); 
    delay(1000);
    lcd_1.setCursor(0, 1);
    lcd_1.print("Enchendo...");  
    digitalWrite(encher, HIGH);
      
    nivelSelecionado = 1;
    
    sensor1();
      
    } else if (digitalRead(bnv2) == LOW) {
    
    lcd_1.clear();   
    lcd_1.setCursor(0, 0);
    lcd_1.print("Nv. 2 escolhido "); 
    delay(1000);
    lcd_1.setCursor(0, 1);
    lcd_1.print("Enchendo...");  
    digitalWrite(encher, HIGH);
      
    nivelSelecionado = 2;
      
    sensor2();  
    
    } else if (digitalRead(bnv3) == LOW) {
    
    lcd_1.clear();   
    lcd_1.setCursor(0, 0);
    lcd_1.print("Nv. 3 escolhido ");
    delay(1000);
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
  
      // 2. Ligar bater
        
    lcd_1.clear();   
    lcd_1.setCursor(0, 0);
    lcd_1.print("Motor ativo");
    digitalWrite(bater, HIGH);  
    delay(1000);
    lcd_1.setCursor(0, 1);
    lcd_1.print("Batendo...");
    digitalWrite(motor, 1); 
      
      delay(10000);  // Tempo de bater (exemplo 5 segundos)
     
	digitalWrite(motor, 0); 
    digitalWrite(bater, LOW);
} 
  


void processoMolho(){       
      
  // 3. Ligar molho
      
    lcd_1.clear();   
    lcd_1.setCursor(0, 0);
    lcd_1.print("Motor inativo");
    delay(1000);
    lcd_1.setCursor(0, 1);
    lcd_1.print("Molho...");  
    digitalWrite(molho, HIGH);
      
      delay(7000);  // Tempo de molho (exemplo 4 segundos)
      
    digitalWrite(molho, LOW);
}
      
      

void processoPronto(){      
      
      // 4. Ligar pronto
      
	lcd_1.clear();   
    lcd_1.setCursor(0, 0);
    lcd_1.print("Motor inativo");
    delay(1000);
    lcd_1.setCursor(0, 1);
    lcd_1.print("Esta pronto!!!!");
      
    digitalWrite(pronto, HIGH);
      
    // Tempo pronto (exemplo 10 segundos)  
    lcd_1.setBacklight(0);
    delay(500);  
  	lcd_1.setBacklight(1);
    delay(500); 
	lcd_1.setBacklight(0);
    delay(500);  
  	lcd_1.setBacklight(1);
    delay(500); 
	lcd_1.setBacklight(0);
    delay(500);  
  	lcd_1.setBacklight(1);
    delay(500); 
	lcd_1.setBacklight(0);
    delay(500);  
  	lcd_1.setBacklight(1);
    delay(500); 
	lcd_1.setBacklight(0);
    delay(500);  
  	lcd_1.setBacklight(1);
    delay(500); 
	lcd_1.setBacklight(0);
    delay(500);  
  	lcd_1.setBacklight(1);
    delay(500);  
  	lcd_1.setBacklight(1);
    delay(500); 
	lcd_1.setBacklight(0);
    delay(500);  
  	lcd_1.setBacklight(1);
    delay(500); 
	lcd_1.setBacklight(0);
    delay(500);  
  	lcd_1.setBacklight(1);
    delay(500); 
	lcd_1.setBacklight(0);
    delay(500);  
  	lcd_1.setBacklight(1);
    delay(500);
    lcd_1.noDisplay();  
	lcd_1.setBacklight(0);  
    lcd_1.clear();  
      digitalWrite(pronto, LOW);
      
}      
 
void finalizado(){

  // Após o processo, desliga tudo e reseta nível
      for (int i = 0; i < qtdLeds; i++) {
        digitalWrite(leds[i], LOW);
      }
      
      nivelSelecionado = 0;
      ligado = false;  // Desliga o sistema após o ciclo completo

}







void sensor1() {
  while (digitalRead(nv1) == HIGH) {
    // HIGH significa "não pressionado" (por causa do INPUT_PULLUP)
    delay(50); // evita leitura instável
  }
  digitalWrite(encher, LOW);
  
}




void sensor2() {
  while (digitalRead(nv2) == HIGH) {
    // HIGH significa "não pressionado" (por causa do INPUT_PULLUP)
    delay(50); // evita leitura instável
  }
  digitalWrite(encher, LOW);
  
}




void sensor3() {
  while (digitalRead(nv3) == HIGH) {
    // HIGH significa "não pressionado" (por causa do INPUT_PULLUP)
    delay(50); // evita leitura instável
  }
  digitalWrite(encher, LOW);
  
}
  
  
