#include <IRremote.h>
#include <Servo.h>
#include <BitVoicer11.h>

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
#define bits_RC5  14

//Variáveis
BitVoicerSerial bvSerial = BitVoicerSerial();
byte dataType = 0;
int valorComando = 0;
int ValLedB = 0;
int pinLedB = 2;
int ValLedS = 0;
int pinLedS = 5;  //TODO: Mudar, o IR usa o 3
int ValLedQ = 0;
int pinLedQ = 4;
int pinMotor = 9;
int pinIR = 3;
int pinLedErro = 6;
int ValLedErro = 0;
int TVdata = 0;
Servo motor;
IRsend irsend;
boolean ativarMotor = false;
int estado = 0;
int quant = 0; 
boolean reverse = false;

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
  for (i = 0; i < quant; i++)
    irsend.sendRC5(data, bits_RC5);
}

//inicialização
void setup() {
  Serial.begin(9600);
  pinMode(pinLedB, OUTPUT);
  pinMode(pinLedQ, OUTPUT);
  pinMode(pinLedS, OUTPUT);
  pinMode(pinIR, OUTPUT);       //Para o IR
  pinMode(pinLedErro, OUTPUT);
  motor.attach(pinMotor);
}
void loop() {
  if(estado == 1)
    sendPhilips(TVdata, quant);
  else
  {
    digitalWrite(pinLedB, ValLedB);
    digitalWrite(pinLedQ, ValLedQ);
    digitalWrite(pinLedS, ValLedS);
    digitalWrite(pinLedErro, ValLedErro);
    if(ativarMotor)
      doMotor();
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
    if(estado == 0){
        switch(valorComando)
        {
        case LedQuarto:
          ValLedQ = !ValLedQ;
          break;
        case LedSala:
          ValLedS = !ValLedS;
          break;
        case LedBanheiro: 
          ValLedB = !ValLedB;
          break;
        case GATE:
          ativarMotor = true;
          break;
        case TV:
          //Serial.println("Comecou TV");
          estado = 1;
          break;
        default:
          ValLedErro = 1;
          break;
        }
    }
    else{
        switch(valorComando)
        {
        case VOL_UP:
          //Serial.println("Volume UP");
          TVdata = 0x7010; 
          quant = 10;
          break;
        case VOL_DOWN:
          TVdata = 0x7011;  
          quant = 10;
          //Serial.println("Volume DOWN");
          break;
        case CH_UP: 
          TVdata = 0x7020;
          quant = 1;
          //Serial.println("Channel Up");    
          break;
        case CH_DOWN:
          //Serial.println("Channel Down");
          TVdata = 0x7021;
          quant = 1;
          break;
        case POWER:
          TVdata = 0x700c; //código Philips de POWER
          quant = 1;
          //Serial.println("Power");
          break;
        case TV:        
          //Serial.println("Cabou TV");
          estado = 0;
          break;
        default:
          ValLedErro = 1;
          break;
        }
      }
   }  
}
