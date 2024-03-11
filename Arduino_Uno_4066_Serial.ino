//Mon adresse
#define SERIAL_SRC_ADDR 0x08
//Nombre de périhériques avec lesquelles dialoguer (nombre de Arduino Nano)
#define NB_PERIPHIQUES 2

//Adresse des périphérique
byte ADDRS_PERIPH[NB_PERIPHIQUES]={0x0A,0x09};
//Statut des boutons
byte buttons_current_state[NB_PERIPHIQUES]={0,0};
//Pin des boutons pour déclancher une émission
byte PINS_SPEAK_INTER[NB_PERIPHIQUES]={9,6};
//Pin des boutons de commutation des TX/RX sur le 4066
byte PINS_SPEAK_CONNECT[NB_PERIPHIQUES]={3,2};
//Pin des LED qui signalent une communications
byte LEDS_LISTENING[NB_PERIPHIQUES]={10,7};
// Tableau de données
byte data[6];



void setup() 
{
  for (byte i=0;i<NB_PERIPHIQUES;i++)
  {
    pinMode(LEDS_LISTENING[i], OUTPUT);
    pinMode(PINS_SPEAK_CONNECT[i], OUTPUT);
    pinMode(PINS_SPEAK_INTER[i], INPUT );
    digitalWrite(LEDS_LISTENING[i],HIGH);
    digitalWrite(PINS_SPEAK_CONNECT[i],LOW);
    buttons_current_state[i]=0;
  } 

  Serial.begin(9600);
  while (!Serial) { delay(10); }
  Serial.println();
  Serial.print("Serial OK on source 0x0");
  Serial.println(SERIAL_SRC_ADDR,HEX);
  delay(100);
  Serial1.begin(9600);
  while (!Serial1) { delay(10); }
  Serial1.println("Serial 1 OK");
  for (byte i=0;i<NB_PERIPHIQUES;i++)
  {
    digitalWrite(LEDS_LISTENING[i],LOW);
  } 
  delay(100);
  
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

byte find_nano_index_by_addr(byte addr)
{
  byte f=0;
  byte i=0;
  while ((f==0) and (i<NB_PERIPHIQUES)) 
  { 
    if (addr==ADDRS_PERIPH[i]) { f=1;} 
    else i++;
  }
  if (f==1) return i;
  else return NB_PERIPHIQUES+1;
}

byte find_nano_index_by_btn()
{
  byte f=0;
  byte i=0;
  while ((f==0) and (i<NB_PERIPHIQUES)) 
  { 
    buttons_current_state[i]=digitalRead(PINS_SPEAK_INTER[i]);
    if (buttons_current_state[i]==HIGH)
    {
      f=1;
    } 
    else i++;
  }
  if (f==1) return i;
  else return NB_PERIPHIQUES+1;
}


void blink_led(byte addr, byte c)
{
  byte j=find_nano_index_by_addr(addr);
  if (j>=NB_PERIPHIQUES) return;
  Serial.print(" ");
  Serial.print(LEDS_LISTENING[j]);

  byte n=c % 10;
  n++;
  for (byte i=0;i<n;i++)
  {
    digitalWrite(LEDS_LISTENING[j],HIGH);
    delay(20);
    digitalWrite(LEDS_LISTENING[j],LOW);
  }
}

//Renseigner le tableau de données
//1ère valeur = Adresse de l'adruino de destination
//2ième : celle de l'arduino source du message
//3,4,5,6 : random : remplace un numéro de badge
void MkData(byte addr)
{
  for (byte i=0;i<6;i++)
  {
    if (i==0) data[i]=addr;
    if (i==1) data[i]=SERIAL_SRC_ADDR;
    if (i>1) data[i]=random(0, 255);
  }
}

void loop() 
{
  byte ind=find_nano_index_by_btn();
  if (ind<NB_PERIPHIQUES)
  {
    Serial.println("Btn pressed ");
    MkData(ADDRS_PERIPH[ind]);
    digitalWrite(PINS_SPEAK_CONNECT[ind],HIGH);
    delay(20);
    Serial1.write(data,sizeof(data));
    delay(100);
    digitalWrite(PINS_SPEAK_CONNECT[ind],LOW);
    affiche("Sending ");
  }
  
  if (Serial1.available()>0)
  {

        byte buffer[6]={0,0,0,0,0};
        Serial1.readBytes(buffer,sizeof(buffer));
        if (buffer[0]==SERIAL_SRC_ADDR)
        {
          memcpy(data,buffer,sizeof(buffer));
          affiche("Receiving ");
          blink_led(data[1],data[2]);
        }
  }

 }
