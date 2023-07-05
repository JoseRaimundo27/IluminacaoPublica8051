#include <LiquidCrystal.h>
LiquidCrystal lcd(10,9,8,7,6,5);

//DEFINE PORTAS DOS SENSORES/LED/BOTAO:

#define ldr A0
#define temp A2
#define amp A3
#define volt A5
#define led 13
#define btn 11 // Int 0 = pino 2

//DEFINE SAIDA DOS SENSORES
int saida_ldr = 12;
int saidaSensores = 4;
int saida_temp = 10;
int saida_amp = 9;
int saida_volt = 8;


//VARIAVEIS AUXILIARES:
int valor_temp, valor_ldr, valor_amp, valor_volt; // Valores dos sensores
int status1=1; // status1 inicia em 1 ( led começa ligado )
int status2=1; // status2 inicia em 1 ( led começa ligado )

void setup() {
  //LCD:
  lcd.begin(16,2);
 // INTERRUPÇÕES:
  attachInterrupt(0,intBotaoLdr,CHANGE); // Configurando interrupção botao mudança de borda
  attachInterrupt(1,intSensores, CHANGE); // Configurando interrupção mudança de borda

  //PINOS: 
  pinMode(led, OUTPUT);//Pino 13 como saida do led
  pinMode(btn,INPUT);//Botao como entrada de dados
  pinMode(volt,INPUT);// Sensor de voltagem como entrada
  pinMode(ldr, INPUT); // Sensor de luz como entrada
  pinMode(saida_ldr, OUTPUT); // Sensor ldr como saída
  pinMode(saidaSensores, OUTPUT); // Saída sensores como saída
  pinMode(saida_temp, OUTPUT);
  pinMode(saida_amp, OUTPUT);
  pinMode(saida_volt, OUTPUT);
  
  // SERIAL:
  Serial.begin(9600);  
}
void ligaLed(){
  digitalWrite(led, HIGH);  
}
void apagaLed(){
  digitalWrite(led, LOW); 
}

//FUNÇÕES DAS INTERRUPÇÕES:
void intSensores(){
  if(status1 == 1){
    apagaLed(); // estava ligado, vou desligar
    status1 = 0;
  }
  else{
    ligaLed();
    status1= 1; //estava desligado, vou ligar
  }
}

void intBotaoLdr(){
  if(status2 == 1){
    apagaLed(); // estava ligado, vou desligar
    status2 = 0;
  }
  else{
    ligaLed();
    status2 = 1; //estava desligado, vou ligar
  }
}

//LOOP:
void loop() { 
  //Recebendo e convertendo valores analogicos:
  valor_temp = ((analogRead(temp))/2-1); //Convertendo valor temp
  valor_volt = (analogRead(volt)/2); // convertendo voltagem, n ficou exato
  valor_amp = (analogRead(amp)/2); // não converti amper

  analogWrite(5, valor_volt/723);
  analogWrite(saida_volt, valor_volt/4);
  analogWrite(saida_amp, valor_amp/512);
  
  //PLOTANDO SENSORES NO LCD:
  lcd.setCursor(0,0);
  lcd.print("T=");
  lcd.setCursor(2,0);
  lcd.print(valor_temp);
  lcd.setCursor(5,0);
  lcd.print("V=");
  lcd.setCursor(7,0);
  lcd.print(valor_volt);
  lcd.setCursor(11,0);
  lcd.print("I=");
  lcd.setCursor(13,0);
  lcd.print(valor_amp);
  

  
  //Serial:
  Serial.println(analogRead(5) ); 
  delay(100);

  //Rotina principal:
  if(status1 == 1 and status2 == 1){ //SE SENSORES E LDR/BOTAO ESTAO NA FAIXA
    ligaLed(); //Led ligado por default
  }

  if((valor_volt > 297) or (valor_amp > 360) or (valor_temp > 60)){ // SE OS SENSORES ULTRAPASSAREM O LIMITE
    digitalWrite(saidaSensores, HIGH); //ACIONA INTERRUPÇÃO DOS SENSORES
  }else{ //SE OS SENSORES ESTIVEREM NO LIMITE, TESTAR LUZ E BOTÃO:
      digitalWrite(saidaSensores, LOW); //Mantém ou alterna para LOW CASO SENSORES ESTEJAM NA FAIXA
      if(valor_ldr > 49){ // SE O LDR ULTRAPASSAR A FAIXA
      digitalWrite(saida_ldr, HIGH); //ACIONA A INTERRUPÇÃO DO LDR
      }else {
      digitalWrite(saida_ldr, LOW); //Mantém ou alterna para LOW CASO LDR ESTEJA NA FAIXA
    }   
  } 
}