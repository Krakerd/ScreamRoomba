#include <Arduino.h>
#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>

#define finalA 12 //pin del final azul
#define finalV 13 //pin del final verde
#define playerRX 10 //pin del RX del df player
#define playerTX 11 //pin del TX del df player

int EstadoV = 0; //Estado del final de carrera pin verde
int EstadoA = 0; //Estado del final de carrera pin azul
int UltEstadoV = LOW; //Ultimo estado del final verde
int UltEstadoA = LOW; //Ultimo estado del final azul
int numeroCanciones = 10;

SoftwareSerial mySoftwareSerial(playerRX, playerTX); //crear un serial de software para los pines del df player(RX,TX)
DFRobotDFPlayerMini myDFPlayer; //genera un objeto que es el reproductor de sonido
void printDetail(uint8_t type, int value); //genera una funcion que no hace nada pero es requerida su existencia

void setup() {
  mySoftwareSerial.begin(9600); //serial 9600 para mandar instrucciones al reproductor dfplayer
  Serial.begin(115200); //serial 115200 para las impresiones y monitoreo
  Serial.println("DFplayer demo");
  Serial.println("Iniciando reproductor... puede tardar 3~5 segundos");

  if (!myDFPlayer.begin(mySoftwareSerial)) {  //Use softwareSerial to communicate with mp3.

    Serial.println(F("Unable to begin:"));                 // |---> comprobar que el mp3 esta en linea (opcional)
    Serial.println(F("1.Please recheck the connection!")); // |
    Serial.println(F("2.Please insert the SD card!"));     // |

    while (true);
  }
  Serial.println("DFplayer en linea");

  myDFPlayer.setTimeOut(500); //colocar el tiempo de espera del reproductor
  myDFPlayer.volume(20); //volumen del reproductor (max 30)
  myDFPlayer.EQ(DFPLAYER_EQ_NORMAL); //equalizador
  myDFPlayer.outputDevice(DFPLAYER_DEVICE_SD); //fuente de los archivos
  
  pinMode(finalA, INPUT); // |---> modo de los pines de entrada
  pinMode(finalV, INPUT); // |
}

void loop() {
  EstadoA = digitalRead(finalA); // |---> Leer estado de los pines
  EstadoV = digitalRead(finalV); // |
  int randomInt = 0;
  
  //comparar con el estado anterior, si es distinto ()
  if(EstadoA != UltEstadoA || EstadoV != UltEstadoV){
    if(EstadoA == HIGH || EstadoV == HIGH){
      randomInt = random(1,numeroCanciones+1);
      myDFPlayer.play(randomInt);
      Serial.println("colision detectada");
      delay(1000);
    }
    }

    UltEstadoA = EstadoA;
    UltEstadoV = EstadoV;
}