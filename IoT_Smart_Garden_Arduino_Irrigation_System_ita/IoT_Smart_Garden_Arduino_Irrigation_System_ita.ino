/* 
  Sketch generato da Arduino IoT Cloud per il progetto:
  "IoT_Smart_Garden_Arduino_Irrigation_System_3CI_AS_2425"

  Variabili Cloud auto-generate:
  - int pl1, pl2, pl3, pl4: valori letti dai sensori di umidità
  - bool bypass: attiva/disattiva irrigazione manuale da dashboard

  Le variabili READ/WRITE attivano funzioni automatiche quando modificate da cloud.
*/

/* Autori: Prof. Antonio Caiafa - Amedeo De Giglio
   Classe: 3C Informatica
*/

#include "thingProperties.h"          // Includi file generato da Arduino Cloud con variabili e funzioni cloud
#include "ArduinoGraphics.h"          // Libreria per funzioni grafiche (testo, animazioni)
#include "Arduino_LED_Matrix.h"       // Libreria per gestire la matrice LED integrata (Arduino Uno R4)

#define thesholdmin 277               // Soglia minima di umidità (non usata nel codice attuale)
#define thesholdmax 650               // Soglia massima: oltre questo valore il terreno è considerato secco

// Definizione dei pin analogici connessi ai sensori di umidità per 4 piante
#define plant1 A0
#define plant2 A1
#define plant3 A2
#define plant4 A3

// Definizione dei pin digitali collegati ai relè/pompe
#define p1 7
#define p2 6
#define p3 5
#define p4 4

// Pin digitale collegato al pulsante fisico per irrigazione manuale
#define SW 2

ArduinoLEDMatrix matrix;              // Crea oggetto per controllare la matrice LED

void setup() {
  Serial.begin(9600);                 // Avvia comunicazione seriale a 9600 bps
  delay(1500);                        // Attendi 1.5 secondi per permettere apertura monitor seriale

  initProperties();                   // Inizializza le variabili e le funzioni legate al cloud

  ArduinoCloud.begin(ArduinoIoTPreferredConnection, false); // Avvia connessione al cloud usando WiFi
  delay(3000);                        // Attesa per completare connessione

  matrix.begin();                     // Inizializza la matrice LED integrata

  // Imposta i pin delle pompe come OUTPUT
  pinMode(p1, OUTPUT);
  pinMode(p2, OUTPUT);
  pinMode(p3, OUTPUT);
  pinMode(p4, OUTPUT);

  // Spegne tutte le pompe all'avvio (HIGH perché relè attivi LOW)
  digitalWrite(p1, HIGH);
  digitalWrite(p2, HIGH);
  digitalWrite(p3, HIGH);
  digitalWrite(p4, HIGH);

  pinMode(SW, INPUT);                 // Imposta pin del pulsante manuale come INPUT

  setDebugMessageLevel(4);           // Livello massimo per messaggi di debug
  ArduinoCloud.printDebugInfo();     // Stampa le info di debug nel monitor seriale
}

void loop() {
  int flag = 0;                       // Variabile che indica quale messaggio mostrare sul display

  ArduinoCloud.update();             // Aggiorna la connessione al cloud per sincronizzazione

  // Mostra animazione di avvio sulla matrice LED
  matrix.loadSequence(LEDMATRIX_ANIMATION_STARTUP);  
  matrix.play(true);                 
  delay(2500);                        // Attendi che l'animazione finisca

  Manual_Bypass();                   // Controlla se il pulsante fisico è stato premuto
  flag = 1;                           // Prima fase della visualizzazione

  ArduinoCloud.update();             // Nuovo aggiornamento cloud
  Print_Led_panel(flag);             // Mostra primo messaggio sul display

  Manual_Bypass();                   // Controllo pulsante manuale

  // Controllo e irrigazione automatica per pianta 1
  pl1 = analogRead(plant1);                              // Leggi valore sensore umidità pianta 1
  Serial.println("Humidity Plant 1: " + String(pl1));    // Mostra valore nel monitor seriale
  if (pl1 > thesholdmax) {                               // Se il suolo è troppo secco
    digitalWrite(p1, LOW);                               // Accendi la pompa
    while (analogRead(plant1) > thesholdmax) {           // Continua a irrigare finché il suolo resta secco
      delay(2000);                                        // Pausa di 2 secondi
    }
    digitalWrite(p1, HIGH);                              // Spegni la pompa quando il suolo è abbastanza umido
  }

  Manual_Bypass();                   // Controllo pulsante manuale
  flag++;                            // Passa al secondo messaggio
  ArduinoCloud.update();             // Aggiorna connessione cloud
  Print_Led_panel(flag);             // Mostra secondo messaggio

  Manual_Bypass();                   // Controllo manuale

  // Pianta 2
  pl2 = analogRead(plant2);
  Serial.println("Humidity Plant 2: " + String(pl2));
  if (pl2 > thesholdmax) {
    digitalWrite(p2, LOW);
    while (analogRead(plant2) > thesholdmax) {
      delay(2000);
    }
    digitalWrite(p2, HIGH);
  }

  Manual_Bypass();
  flag++;
  ArduinoCloud.update();
  Print_Led_panel(flag);

  Manual_Bypass();

  // Pianta 3
  pl3 = analogRead(plant3);
  Serial.println("Humidity Plant 3: " + String(pl3));
  if (pl3 > thesholdmax) {
    digitalWrite(p3, LOW);
    while (analogRead(plant3) > thesholdmax) {
      delay(2000);
    }
    digitalWrite(p3, HIGH);
  }

  Manual_Bypass();
  flag++;
  ArduinoCloud.update();
  Print_Led_panel(flag);

  Manual_Bypass();

  // Pianta 4
  pl4 = analogRead(plant4);
  Serial.println("Humidity Plant 4: " + String(pl4));
  if (pl4 > thesholdmax) {
    digitalWrite(p4, LOW);
    while (analogRead(plant4) > thesholdmax) {
      delay(2000);
    }
    digitalWrite(p4, HIGH);
  }

  Manual_Bypass();
  flag++;
  ArduinoCloud.update();
  Print_Led_panel(flag);

  Manual_Bypass();  // Ultimo controllo del pulsante manuale
}

// Funzione che viene chiamata automaticamente quando cambia lo stato della variabile "bypass" nel cloud
void onBypassChange() {
  Serial.println("Attivazione switch virtuale");
  Serial.println(bypass);              // Stampa stato attuale

  // Attiva tutte le pompe per 3 secondi
  digitalWrite(p1, LOW);
  digitalWrite(p2, LOW);
  digitalWrite(p3, LOW);
  digitalWrite(p4, LOW);
  delay(3000);

  Serial.println("Disattivazione switch virtuale");
  Serial.println(bypass);              // Stampa stato attuale

  // Spegne tutte le pompe
  digitalWrite(p1, HIGH);
  digitalWrite(p2, HIGH);
  digitalWrite(p3, HIGH);
  digitalWrite(p4, HIGH);
}

// Funzione per gestione dell'irrigazione tramite pulsante fisico
void Manual_Bypass(){
  if (digitalRead(SW) == 1) {          // Se il pulsante è premuto
    Serial.println("Attivazione switch fisico");

    // Accende tutte le pompe
    digitalWrite(p1, LOW);
    digitalWrite(p2, LOW);
    digitalWrite(p3, LOW);
    digitalWrite(p4, LOW);

    // Mantiene le pompe attive finché il pulsante resta premuto
    while (digitalRead(SW) == 1) {
      delay(2000);
    }

    // Spegne tutte le pompe dopo il rilascio del pulsante
    digitalWrite(p1, HIGH);
    digitalWrite(p2, HIGH);
    digitalWrite(p3, HIGH);
    digitalWrite(p4, HIGH);
  }
}

// Funzione che mostra messaggi scorrevoli sulla matrice LED
void Print_Led_panel(int flag){
  matrix.play(true);                        // Avvia la sequenza sulla matrice
  matrix.beginDraw();                       // Inizia il disegno
  matrix.textScrollSpeed(75);             // Imposta velocità di scorrimento
  matrix.textFont(Font_5x7);               // Font da 5x7 pixel
  matrix.beginText(0, 1, 0xFFFFFF);        // Testo in bianco, posizione iniziale

  // Mostra messaggi diversi a seconda del valore di flag
  if(flag == 1){
    matrix.println(" IISS 'Augusto Righi' ");
  }
  else if(flag == 2){
    matrix.println(" Taranto Proff. Antonio Caiafa ");
  }
  else if(flag == 3){
    matrix.println("- Amedeo De Giglio ");
  }
  else if(flag == 4){
    matrix.println(" Hidro Project AS 24/25 ");
  }
  else if(flag == 5){
    matrix.println(" 3C Computer Science");
  }

  matrix.endText(SCROLL_LEFT);             // Scorri testo da destra a sinistra
  matrix.endDraw();                        // Termina il disegno
}