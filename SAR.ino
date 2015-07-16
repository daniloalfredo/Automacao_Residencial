#include <IRremote.h>
#include <Servo.h>
#include <BitVoicer11.h>
#include <LiquidCrystal.h>
// O LED IR deve estar conectado ao pino PWM 3

#define LedQuarto 1
#define LedSala   2
#define LedBanheiro 3
#define VOL_UP    4
#define VOL_DOWN  5
#define CH_UP     6
#define CH_DOWN   7
#define POWER     8
#define GATE      9
#define TV        10
#define HEY_SAR   11
#define bits_RC5  12
#define bits_Sony 12
#define APAGA 12
#define ACENDE 13

//Variáveis
BitVoicerSerial bvSerial = BitVoicerSerial();
byte dataType = 0;
int valorComando = 0;
int ValLedB = 0;
int pinLedB = 2;
int ValLedS = 0;
int pinLedS = 3;  //TODO: Mudar, o IR usa o 3
int ValLedQ = 0;
int pinLedQ = 4;
int pinMotor = 9;
int pinIR = 11;
int pinLedErro = 6;
int pinLedLegal = 6;
int pinBuzzer = 10;

int RS = 22;
int En = 24;
int D4 = 33;
int D5 = 32;
int D6 = 31;
int D7 = 30;
//TODO: !!!!----Acho que mudei o led do IR pra 11. Testar----!!!!! // É.. foi pro 11


int ValLedErro = 0;
int TVdata = 0;
Servo motor;
IRsend irsend;
boolean ativarMotor = false;
int heySar = 0;
int estado = 0;
int quant = 0; 
boolean reverse = false;
unsigned long rctoggle = 0; //para os códigos RC5
LiquidCrystal lcd(RS, En, D4, D5, D6, D7);

void doMotor()
{
  float angle = motor.read();
  if(angle >= 90)
  {
    for (; angle > 0; angle--)  
    {                                  
      motor.write(angle);               
      delay(15);                   
    }
  }
  else if (angle < 90)
  {
    for(; angle < 180; angle++)
    {
      motor.write(angle);
      delay(15);
    }
  }
  ativarMotor = false;
} 

void sendPhilips(int data, int quant){
  int i;
  for (i = 0; i < 3*quant; i++){
    data = data ^ (rctoggle << 11);
    rctoggle = 1 - rctoggle;
    irsend.sendRC5(data, bits_RC5);
    delay(100);
  }
}

void sendSony(int data, int quant)
{
  int i;
  for (i = 0; i < 3*quant; i++)
  {
    irsend.sendSony(data, bits_Sony);
    delay(100);  
  }
}

void reconheceHeySAR(){
  for(int i=0; i<2; i++){
    digitalWrite(pinLedLegal, HIGH);
    tone(pinBuzzer, 2400, 50);
    delay(50);
    tone(pinBuzzer, 2550, 50);
    digitalWrite(pinLedLegal, LOW);
    delay(50);
    noTone(pinBuzzer);
  }
}

void comando_erro(){
    digitalWrite(pinLedErro, HIGH);
    tone(pinBuzzer, 2150, 500);
    delay(700);
    digitalWrite(pinLedErro, LOW);
}

//inicialização
void setup() {
  Serial.begin(9600);
  lcd.begin(16,2);
  pinMode(pinLedB, OUTPUT);
  pinMode(pinLedQ, OUTPUT);
  pinMode(pinLedS, OUTPUT);
  pinMode(pinIR, OUTPUT);       //Para o IR
  pinMode(pinLedErro, OUTPUT);
  pinMode(pinLedLegal, OUTPUT);
  pinMode(pinBuzzer, OUTPUT);
  motor.attach(pinMotor);
  motor.write(0);
  //lcd.print("Hello Motherfucker");

  randomSeed(analogRead(0)); //Apagar
}
void loop() {
  /*if(estado == 1)
    sendPhilips(TVdata, quant);     // Ficaria mandando o comando a todo momento no estado==1
  else
  */
  {
    digitalWrite(pinLedB, ValLedB);
    digitalWrite(pinLedQ, ValLedQ);
    digitalWrite(pinLedS, ValLedS);
    digitalWrite(13, HIGH);
    //digitalWrite(pinLedErro, ValLedErro);   // Substituído por comando_erro()
    if(ativarMotor)
      doMotor();

    //Testes//
    /*doMotor();
    reconheceHeySAR();
    digitalWrite(pinLedB, random(2));
    digitalWrite(pinLedQ, random(2));
    digitalWrite(pinLedS, random(2));
    for(int i=0; i<3; i++){
      irsend.sendNEC(0x0010, 16);
    }*/
    
    delay(2000);
  }
}  

//identificação do comando
void serialEvent()
{
  dataType = bvSerial.getData();
  ValLedErro = 0;
  if (dataType == BV_INT)
  {
    valorComando = bvSerial.intData;
    if(heySar == 0 && estado ==0){
      switch(valorComando)
        {
        case HEY_SAR:
          heySar = 1;
          reconheceHeySAR();
          break;
        default:
          comando_erro();
          break;
        }
    }else if(estado == 0 && heySar == 1){
        switch(valorComando)
        {
        case LedQuarto:
          ValLedQ = !ValLedQ;
          heySar = 0;
          break;
        case LedSala:
          ValLedS = !ValLedS;
          heySar = 0;
          break;
        case LedBanheiro: 
          ValLedB = !ValLedB;
          heySar = 0;
          break;
        case GATE:
          ativarMotor = true;
          heySar = 0;
          break;
        case ACENDE:
          ValLedB = 1;
          ValLedQ = 1;
          ValLedS = 1;
          heySar = 0;
          break;
        case APAGA:
          ValLedB = 0;
          ValLedQ = 0;
          ValLedS = 0;
          heySar = 0;
          break;
        case TV:
          lcd.clear();
          lcd.print("Comecou TV");
          estado = 1;
          break;
        default:
          comando_erro();
          heySar = 0;
          break;
        }
    }
    else{
        switch(valorComando)
        {
        case VOL_UP:
          lcd.clear();
          lcd.print("Volume UP");
          TVdata = 0x0010; 
          quant = 10;
          sendPhilips(TVdata, quant);
          break;
        case VOL_DOWN:
          TVdata = 0x0011;  
          quant = 10;
          sendPhilips(TVdata, quant);
          lcd.clear();
          lcd.print("Volume DOWN");
          break;
        case CH_UP: 
          TVdata = 0x0020;
          quant = 1;
          sendPhilips(TVdata, quant);
          lcd.clear();
          lcd.print("Channel Up");    
          break;
        case CH_DOWN:
          lcd.clear();
          lcd.print("Channel Down");
          TVdata = 0x0021;
          quant = 1;
          sendPhilips(TVdata, quant);
          break;
        case POWER:
          TVdata = 0x000c; //código Philips de POWER
          quant = 1;
          sendPhilips(TVdata, quant);
          lcd.clear();
          lcd.print("Power");
          break;
        case TV:        
          lcd.clear();
          lcd.print("Cabou TV");
          estado = 0;
          heySar = 0;
          break;
        default:
          comando_erro();
          break;
        }
      }
   }  
}
