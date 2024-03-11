//LED_LISTENING pour signaler une réception de données
#define LED_LISTENING 10
//PIN_SPEAK_CONNECT : pour déelcncher la connexion tu TX via le CD4066 au RX de l'Arduino Mega
#define PIN_SPEAK_CONNECT 8
//PIN_SPEAK_INTER : bouton pour déclancher une émission
#define PIN_SPEAK_INTER 9
//PIN_ADDR : adressage : si relié à la masse : 10, sinon 9
#define PIN_ADDR 4
//SERIAL_DEST_ADDR : Adresse de l'aRduino Mega (destination)
#define SERIAL_DEST_ADDR 0x08
//Initialisation de mon adresse 
byte SERIAL_SRC_ADDR = 0;


#include <SoftwareSerial.h>
SoftwareSerial mySerial(2, 3); // RX, TX
byte button_current_state=0;
// Tableau de données qui seront échangées
byte data[6];



void setup() {
  pinMode(LED_LISTENING, OUTPUT);
  pinMode(PIN_SPEAK_CONNECT, OUTPUT);
  pinMode(PIN_SPEAK_INTER, INPUT );
  pinMode(PIN_ADDR, INPUT_PULLUP );

  digitalWrite(LED_LISTENING,LOW);
  digitalWrite(PIN_SPEAK_CONNECT,LOW);
  digitalWrite(LED_LISTENING,HIGH);
  
  //Lecture PIN_ADDR pour connaitre mon adresse
  if (digitalRead(PIN_ADDR)==HIGH) SERIAL_SRC_ADDR=0x09; else SERIAL_SRC_ADDR=0x0A;

  Serial.begin(9600);
  while (!Serial) { delay(10); }
  Serial.print("Serial OK on source 0x0");
  Serial.print(SERIAL_SRC_ADDR,HEX);
  Serial.print(" to destination 0x0");
  Serial.println(SERIAL_DEST_ADDR,HEX);

  digitalWrite(LED_LISTENING,LOW);
  delay(100);
  digitalWrite(LED_LISTENING,HIGH);

  mySerial.begin(9600);
  while (!mySerial) { delay(10); }
  Serial.println("mySerial OK");
  digitalWrite(LED_LISTENING,LOW);
  delay(100);
  button_current_state=0;
  Serial.println("Ready .... press button when you want");
  }


//Afficher proprement le tableau des données échangées
void affiche(String msg)
{
  Serial.println(msg);
  for (byte i=0;i<sizeof(data);i++) 
  {
    if (data[i]<16) Serial.print("0");
    Serial.print(data[i],HEX);
    Serial.print(" ");
  }
  Serial.println();
}

//Faire clignoter la led
void blink_led(byte c)
{
  byte n=c % 10;
  n++;
  for (byte i=0;i<n;i++)
  {
    digitalWrite(LED_LISTENING,HIGH);
    delay(20);
    digitalWrite(LED_LISTENING,LOW);
  }
}


//Renseigner le tableau de données
//1ère valeur = Adresse de l'adruino de destination
//2ième : celle de l'arduino source du message
//3,4,5,6 : random : remplace un numéro de badge
void MkData()
{
  for (byte i=0;i<6;i++)
  {
    if (i==0) data[i]=SERIAL_DEST_ADDR;
    if (i==1) data[i]=SERIAL_SRC_ADDR;
    if (i>1) data[i]=random(0, 255);
  }
}


void loop() 
{
  button_current_state=digitalRead(PIN_SPEAK_INTER);
  if (button_current_state==HIGH)
  {
    Serial.println("Btn pressed");
    //Création des données
    MkData();
    //connexion du TX au RX via le 4066
    digitalWrite(PIN_SPEAK_CONNECT,HIGH);
    delay(20); 
    //Envoi des données
    mySerial.write(data,sizeof(data));
    delay(100); 
    //fermeture connexion du TX au RX
    digitalWrite(PIN_SPEAK_CONNECT,LOW);
    //Affichage
    affiche("Sending ");
  }

  //Ecoute du port série 
  if (mySerial.available()>0)
  {
    //1er octet : l'adresse de destination, est-le ma mienne ?
    byte c0=mySerial.read();
    delay(10);
    if ((c0==SERIAL_SRC_ADDR) and (mySerial.available()>0))
      {
        data[0]=c0;
        //C'est pour moi, lecture de toutes les données(6 octets en tout)
        for (byte i=1;i<6;i++)
        {
            if (mySerial.available()>0) data[i]=mySerial.read();
            else data[i]=0;
            delay(10);
        }
        //Affichage
        affiche("Receiving ");
        //Faire clignoter la Led
        blink_led(data[2]);
      }
  }
}
