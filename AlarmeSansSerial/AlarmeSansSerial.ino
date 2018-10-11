#include <SPI.h>
#include <RFID.h>

//Broches
const char ledV=2;
const char ledJ=3;
const char ledR=4;
const char buzz=5;

const char but =A2;
const char det =A1;



//RFID
RFID ModuleRFID(10,9);
int UID[5];
int UID1[]={41,166,42,59,158};
int UID2[]={129,40,173,33,37};


//ETAT: 0=bluetooth ; 1=alarme
boolean STATE=1;





//INITIALISATION
void setup(){
  //liaison
  //Serial.begin(9600);
  //RFID
  SPI.begin();
  ModuleRFID.init();
  //Pins
  pinMode(ledJ, OUTPUT);
  pinMode(ledR, OUTPUT);
  pinMode(ledV, OUTPUT);
  pinMode(buzz, OUTPUT);
  
  //Fin d'initialisation
  //Serial.println("Initialisation terminee");
}







void loop() {
  start:
  if(STATE){
    int a=RFIDread();
    if(a==1){
      digitalWrite(ledR, LOW);
      digitalWrite(ledV, HIGH);
      digitalWrite(buzz, LOW);
      delay(1);
      STATE=0;
      goto start;
    }
    else if(a==2){
      digitalWrite(ledJ, HIGH);
    }
    else if(a==0){
      if(analogRead(det)>512){
        //Serial.println("Détection");
        digitalWrite(ledJ, HIGH);
        alarme(10);
        //Serial.println("Alarme OFF");
      }
    }
  }
  else{
    if(analogRead(but)>512){
      digitalWrite(ledV, LOW);
      digitalWrite(ledJ, LOW);
      delay(1);
      STATE=1;
      goto start;
    }
  }
}










int alarme(int temps){
  //Serial.print("Alarme ON pour ");
  //Serial.print(temps);
  //Serial.println(" secondes");
  unsigned long t=millis();
  digitalWrite(buzz, 1);
  boolean i=0;
  while((millis()-t)<temps*1000 && RFIDread()!=1){
    if(i){
      digitalWrite(ledR, 1);
      delay(100);
      i=0;
    }
    else{
      digitalWrite(ledR, 0);
      delay(100);
      i=1;
    }
  }
  digitalWrite(buzz, 0);
  digitalWrite(ledR, 0);
  delay(1);
  return int((millis()-t)/1000);
}

















int RFIDread(){
  int a=0;
  if (ModuleRFID.isCard()) {
    if (ModuleRFID.readCardSerial()) {
      a=2;
      //Serial.print("L'UID est: ");
      for(int i=0;i<=4;i++){
        UID[i]=ModuleRFID.serNum[i];
        //Serial.print(UID[i],DEC);
        //Serial.print(".");
        }
      //Serial.println("");
      if (UID[0] == UID1[0]
       && UID[1] == UID1[1]
       && UID[2] == UID1[2]
       && UID[3] == UID1[3]
       && UID[4] == UID1[4]){a=1;}
      else if(UID[0] == UID2[0]
           && UID[1] == UID2[1]
           && UID[2] == UID2[2]
           && UID[3] == UID2[3]
           && UID[4] == UID2[4]){a=1;}
      }
    ModuleRFID.halt();
    }
  delay(2);
  return a;
}

