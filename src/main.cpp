#include <Arduino.h>             // |---> incluir bilbiotecas necesarias
#include <SoftwareSerial.h>      // |
#include <DFRobotDFPlayerMini.h> // |

#define finalA 12 //pin del final azul
#define finalV 13 //pin del final verde
#define playerRX 10 //pin del RX del df player
#define playerTX 11 //pin del TX del df player

int EstadoV = 0; //Estado del final de carrera pin verde
int EstadoA = 0; //Estado del final de carrera pin azul
int UltEstadoV = LOW; //Ultimo estado del final verde
int UltEstadoA = LOW; //Ultimo estado del final azul
int numeroCanciones = 10;

SoftwareSerial mySoftwareSerial(playerRX, playerTX); //crear un serial de software para los pines del dfplayer(RX,TX)
DFRobotDFPlayerMini myDFPlayer; //genera un objeto que es el reproductor de sonido
void printDetail(uint8_t type, int value);

void setup() {
  mySoftwareSerial.begin(9600); //serial 9600 para mandar instrucciones al reproductor dfplayer
  Serial.begin(115200); //serial 115200 para las impresiones y monitoreo
  Serial.println("DFplayer demo");
  Serial.println("Iniciando reproductor... puede tardar 3~5 segundos");

  if (!myDFPlayer.begin(mySoftwareSerial)) {  //Use softwareSerial to communicate with mp3.

    Serial.println(F("Unable to begin:"));                 // |---> comprobar que el mp3 esta en linea (opcional)
    Serial.println(F("1.Please recheck the connection!")); // |
    Serial.println(F("2.Please insert the SD card!"));     // |

    while (true){delay(0);}
  }
  Serial.println("DFplayer en linea");

  myDFPlayer.setTimeOut(500); //colocar el tiempo de espera del reproductor
  myDFPlayer.volume(20); //volumen del reproductor (max 30)
  
  pinMode(finalA, INPUT); // |---> modo de los pines de entrada
  pinMode(finalV, INPUT); // |
}

void loop() {
  EstadoA = digitalRead(finalA); // |---> Leer estado de los pines
  EstadoV = digitalRead(finalV); // |

  int randomInt = 0;
  randomInt = random(1,numeroCanciones+1); //elige un numero aleatorio

  if(myDFPlayer.available()){
    printDetail(myDFPlayer.readType(),myDFPlayer.read());
  }

  //comprueba si ha habido una colision con cualquiera de los finales de carrera
  if(EstadoA != UltEstadoA && EstadoA == HIGH){
    myDFPlayer.play(randomInt);
    Serial.println("colision detectada");
    Serial.println(randomInt);
    delay(100);
  }

  if(EstadoV != UltEstadoV && EstadoV == HIGH){
    myDFPlayer.play(randomInt); //reproduce una cancion elegida por el random 
    Serial.println("colision detectada");
    Serial.println(randomInt);
    delay(100);
  }

    UltEstadoA = EstadoA;
    UltEstadoV = EstadoV;
}

void printDetail(uint8_t type, int value){
  switch (type) {
    case TimeOut:
      Serial.println(F("Time Out!"));
      break;
    case WrongStack:
      Serial.println(F("Stack Wrong!"));
      break;
    case DFPlayerCardInserted:
      Serial.println(F("Card Inserted!"));
      break;
    case DFPlayerCardRemoved:
      Serial.println(F("Card Removed!"));
      break;
    case DFPlayerCardOnline:
      Serial.println(F("Card Online!"));
      break;
    case DFPlayerUSBInserted:
      Serial.println("USB Inserted!");
      break;
    case DFPlayerUSBRemoved:
      Serial.println("USB Removed!");
      break;
    case DFPlayerPlayFinished:
      Serial.print(F("Number:"));
      Serial.print(value);
      Serial.println(F(" Play Finished!"));
      break;
    case DFPlayerError:
      Serial.print(F("DFPlayerError:"));
      switch (value) {
        case Busy:
          Serial.println(F("Card not found"));
          break;
        case Sleeping:
          Serial.println(F("Sleeping"));
          break;
        case SerialWrongStack:
          Serial.println(F("Get Wrong Stack"));
          break;
        case CheckSumNotMatch:
          Serial.println(F("Check Sum Not Match"));
          break;
        case FileIndexOut:
          Serial.println(F("File Index Out of Bound"));
          break;
        case FileMismatch:
          Serial.println(F("Cannot Find File"));
          break;
        case Advertise:
          Serial.println(F("In Advertise"));
          break;
        default:
          break;
      }
      break;
    default:
      break;
  }
} //genera una funcion que no hace nada pero es requerida su existencia