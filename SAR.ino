#include <BitVoicer11.h>

//Variáveis
BitVoicerSerial bvSerial = BitVoicerSerial();
byte dataType = 0;
int valorComando = 0;
int ValLedB = 0;
int pinLedB = 12;
int ValLedS = 0;
int pinLedS = 10;
int ValLedQ = 0;
int pinLedQ = 11;

int estado = 1;

//inicialização
void setup() {
  Serial.begin(9600);
  pinMode(pinLed, OUTPUT);
}

void loop() {
  digitalWrite(13, ValLed);
}  

//identificação do comando
void serialEvent()
{
  dataType = bvSerial.getData();
  if (dataType == BV_INT)
  {
    valorComando = bvSerial.intData;
    switch(valorComando)
    case 1: 
      ValLed = !ValLed;
   }  
}
