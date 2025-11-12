
int led = 2;
int leitura;


void setup() {
  
  Serial.begin(9600);
  pinMode(4, INPUT_PULLUP); 
  pinMode(led, OUTPUT);


}

void loop() {

  leitura = pinMode(4);

if(leitura == HIGH){ 

Serial.println("FUNCIONANDO");

} else {

Serial.println("DESATIVADO")

}



  



}
