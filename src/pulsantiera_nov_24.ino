// Versione 0,9,1 Alberto Corti - 04/07/2024 - git new 18/11/2025

// nuova versione di sviluppo per modifiche github rev1.1 - main
#include <Arduino.h>
#include <Ethernet.h>
//#include <EthernetUDP.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
String cs="";
String dataIn="";
bool tasto;
bool prev_tasto;
float temp=0;
unsigned long t0;
byte stato = LOW;
LiquidCrystal_I2C lcd(0x27, 20, 4);
byte led1 = 2;  //relay1
byte led2 = 3;  //relay2
byte led3 = 5;  //relay3
byte led4 = 6;  //relaygit log4
byte led5 = 4;
byte led6 = 7;
byte pulsantePin = 14;  //pulsante1
byte pulsantePin2 = 15;
byte pulsantePin3 = 17; // invertiti i tasti per la nuova basetta
byte pulsantePin4 = 16;
byte ledStato = LOW;
byte ledStato2 = LOW;
byte ledStato3 = LOW;
byte ledStato4 = LOW;
byte pulsanteStato = LOW;
byte pulsanteStato2 = LOW;
byte pulsanteStato3 = LOW;
byte pulsanteStato4 = LOW;
byte attesaDebounce = 50;
byte attesaDebounce2 = 50;
byte attesaDebounce3 = 50;
byte attesaDebounce4 = 50;
unsigned long ultimoTempoDebounce = 0;
unsigned long ultimoTempoDebounce2 = 0;
unsigned long ultimoTempoDebounce3 = 0;
unsigned long ultimoTempoDebounce4 = 0;
byte ultimaLettura = LOW;
byte ultimaLettura2 = LOW;
byte ultimaLettura3 = LOW;
byte ultimaLettura4 = LOW;
uint8_t mac[] = { 0x00, 0x01, 0x2E, 0x2F, 0xF4, 0xEF };  // mac address
IPAddress ip(192, 168, 1, 150);                     // IP CASA
//const byte ip[] = { 192, 168, 2, 91 };
IPAddress gateway(192, 168, 1, 1);  // internet access CASA
IPAddress dns(8,8,8,8);
//const byte gateway[] = { 192, 168, 157, 171 };
IPAddress subnet (255, 255, 255, 0);  //subnet mask CASA
//const byte subnet[] = { 255, 255, 0, 0 };  //subnet mask
const byte remote_MAC_ADD[] = { 0x9C, 0xA2, 0xF4, 0xB9, 0xA8, 0xAF }; //PC EXGIULIA OK!
const byte remote_MAC_ADD2[] = { 0x74, 0xD4, 0x35, 0xD5, 0x5A, 0x41 };
const byte broadCastIp[] = { 192, 168, 1, 255 };
byte portaLocale = 7;
byte portaLocale2 = 10;
byte wolPort = 9;
byte wolPort2 = 12;
EthernetUDP Udp;
EthernetServer server(80);  //server port
String listener = "";
uint8_t SPENTO[8] = {
  0x0E,
  0x11,
  0x11,
  0x11,
  0x0A,
  0x0E,
  0x0E,
  0x04,
};
uint8_t ACCESO[8] = {
  0x0E,
  0x1F,
  0x1F,
  0x1F,
  0x0E,
  0x0E,
  0x0E,
  0x04,
};
byte pacman[8] = {
  B00000,
  B01110,
  B11101,
  B11111,
  B11000,
  B11111,
  B01110,
  B00000
};
byte ghost[8] = {
  B00000,
  B01110,
  B10101,
  B11111,
  B10001,
  B11111,
  B10101,
  B00000
};
//******************************* SETUP *****************************************************
void setup() {
  pinMode(8, INPUT_PULLUP); //_Ex imput pullup
  pinMode(9, INPUT_PULLUP);
  lcd.init();
  lcd.createChar(0, SPENTO);
  lcd.createChar(1, ACCESO);
  lcd.createChar(2, pacman);
  lcd.createChar(3, ghost);
  Wire.begin();
  digitalWrite(led6, HIGH);
  pulsantieraOn(2000);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(led4, OUTPUT);
  pinMode(led5, OUTPUT);
  pinMode(led6, OUTPUT);
  pinMode(pulsantePin, INPUT);
  pinMode(pulsantePin2, INPUT);
  pinMode(pulsantePin3, INPUT);
  pinMode(pulsantePin4, INPUT);
  Ethernet.begin(mac, ip, dns, gateway, subnet);
  server.begin();
  Udp.begin(portaLocale2);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(F("Server IP:"));
  lcd.setCursor(0, 1);
  lcd.print(Ethernet.localIP());
  delay(1000);
}
//******************************* END SETUP *****************************************************
char Time[] = "Ora:   :  :  ";
char Calendar[] = "Data:  -  -20   ----";
byte i, second, minute, hour, date, month, year;
char linebuf[80];
int charcount = 0;
boolean authentificated = false;
//******************************* LOOP **********************************************************
void loop() {
  tasto = !digitalRead(9);  // FUNZIONE TASTO MAGICO ************
  if (!prev_tasto && tasto) {
    t0 = millis();
  }
  if (prev_tasto && !tasto) {
    unsigned long t = millis() - t0;
    if (t > 2000) {
      stato = !stato;
      if (digitalRead(led5) == 0) {
        digitalWrite(led5, HIGH);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(F("TASTO MAGICO!"));
        lcd.setCursor(0, 1);
        lcd.print(F("ACCENSIONE TOTALE"));
        lcd.setCursor(0, 2);
        lcd.print(F("ACCENDO HD, NAS"));
        lcd.setCursor(0, 3);
        lcd.print(F("MIXER E CASSE"));
        digitalWrite(led1, stato);
        ledStato = 1;
        //inviaMagicPacket();
        delay(300);
        digitalWrite(led2, stato);
        ledStato2 = 1;
        //inviaMagicPacket2();
        delay(300);
        digitalWrite(led3, stato);
        ledStato3 = 1;
        delay(300);
        digitalWrite(led4, stato);
        ledStato4 = 1;
      } else if (digitalRead(led5) == 1) {
        digitalWrite(led5, LOW);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(F("TASTO MAGICO!"));
        lcd.setCursor(0, 1);
        lcd.print(F("SPEGNIMENTO TOTALE"));
        lcd.setCursor(0, 2);
        lcd.print(F("SPENGO HD, NAS"));
        lcd.setCursor(0, 3);
        lcd.print(F("MIXER E CASSE"));
        digitalWrite(led1, stato);
        ledStato = 0;
        delay(3000);
        digitalWrite(led2, stato);
        ledStato2 = 0;
        delay(2000);
        digitalWrite(led3, stato);
        ledStato3 = 0;
        delay(2000);
        digitalWrite(led4, stato);
        ledStato4 = 0;
      }
      delay(1000);
      lcd.clear();
    }
  }
  prev_tasto = tasto;
  if (!digitalRead(8)) {  // Se il pulsante1 (pin #8) è premuto
    i = 0;
    hour = edit(5, 0, hour);
    minute = edit(8, 0, minute);
    date = edit(5, 1, date);
    month = edit(8, 1, month);
    year = edit(13, 1, year);
    // Converti da decimal in BCD
    minute = ((minute / 10) << 4) + (minute % 10);
    hour = ((hour / 10) << 4) + (hour % 10);
    date = ((date / 10) << 4) + (date % 10);
    month = ((month / 10) << 4) + (month % 10);
    year = ((year / 10) << 4) + (year % 10);
    // Fine conversione
    // Scrivi data sul modulo DS3231 RTC
    Wire.beginTransmission(0x68);  // Inizia il protocollo I2C sul'indirizzo del DS3231
    Wire.write(0);                 // Invia registrazione indirizzo
    Wire.write(0);                 // Reimposta e avvia l'oscillatore
    Wire.write(minute);            // Scrivi i minuti
    Wire.write(hour);              // Scrivi l'ora
    Wire.write(1);                 // Scrivi giorno (non usato)
    Wire.write(date);              // Scrivi data
    Wire.write(month);             // Scrivi mese
    Wire.write(year);              // Scrivi anno
    Wire.endTransmission();        // Interrompere la trasmissione e rilasciare il bus I2C
    delay(200);                    // Attendi 200ms
  }
  Wire.beginTransmission(0x68);  // Inizia il protocollo I2C sul'indirizzo del DS3231
  Wire.write(0);                 // Invia registrazione indirizzo
  Wire.endTransmission(false);   // I2C restart
  Wire.requestFrom(0x68, 7);     // Richiedi 7 byte da DS3231 e rilascia il bus I2C alla fine della lettura
  second = Wire.read();          // Leggi i secondi dal registro 0
  minute = Wire.read();          // Leggi i minuti dal registro 1
  hour = Wire.read();            // Leggi ora dal registro 2
  Wire.read();                   // Leggi il giorno dal registro 3 (non utilizzato)
  date = Wire.read();            // Leggi la data dal registro 4
  month = Wire.read();           // Leggi il mese dal registro 5
  year = Wire.read();            // Leggi l'anno dal registro 6
  DS3231_display();              // Visualizza ora e calendario
  delay(50);
  // ******************** FARE LE FUNZIONI ******************************************************************


  byte lettura = digitalRead(pulsantePin);  // (int lettura = Variabile locale) leggo lo stato del pulsante
  byte lettura2 = digitalRead(pulsantePin2);
  byte lettura3 = digitalRead(pulsantePin3);
  byte lettura4 = digitalRead(pulsantePin4);
  letturaPulsanti(lettura, pulsantePin, ultimaLettura, ultimoTempoDebounce);
  letturaPulsanti(lettura2, pulsantePin2, ultimaLettura2, ultimoTempoDebounce2);
  letturaPulsanti(lettura3, pulsantePin3, ultimaLettura3, ultimoTempoDebounce3);
  letturaPulsanti(lettura4, pulsantePin4, ultimaLettura4, ultimoTempoDebounce4);
  // *********************************************************************************************************************
  if ((millis() - ultimoTempoDebounce) > attesaDebounce) {  // se il conteggio è superiore a 50 millisecondi
    if (lettura != pulsanteStato and lettura == HIGH) {     // se il pin7 è diverso dallo stato del pulsante e il pulsante1 è premuto
      ledStato = !ledStato;                                 // cambio lo stato del pin 13
      digitalWrite(led1, ledStato);                         // Imposto al pin13 lo stato di ledStato

      if (ledStato == 0) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(F("PC OFF"));
      } else if (ledStato == 1) {
        //inviaMagicPacket();
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(F("PC ON"));
      }
      delay(500);  // Invio magic packet, in questo caso al NAS
    }
    pulsanteStato = lettura;  // assegno lo stato del pulsante al pin7
  }
  if ((millis() - ultimoTempoDebounce2) > attesaDebounce2) {  // se il conteggio è superiore a 50 millisecondi
    if (lettura2 != pulsanteStato2 and lettura2 == HIGH) {    // se il pin8 è diverso dallo stato del pulsante e il pulsante2 è premuto
      ledStato2 = !ledStato2;                                 // cambio lo stato del pin 13
      digitalWrite(led2, ledStato2);                          // Imposto al pin13 lo stato di ledStato

      if (ledStato2 == 0) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(F("CASSE OFF"));
      } else if (ledStato2 == 1) {
        inviaMagicPacket2();
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(F("CASSE ON"));
      }
      delay(500);  // Invio magic packet, in questo caso al NAS
      lcd.clear();
    }
    pulsanteStato2 = lettura2;  // assegno lo stato del pulsante al pin7
  }

  if ((millis() - ultimoTempoDebounce3) > attesaDebounce3) {  // se il conteggio è superiore a 50 millisecondi
    if (lettura3 != pulsanteStato3 and lettura3 == HIGH) {    // se il pin7 è diverso dallo stato del pulsante e il pulsante3 è premuto
      ledStato3 = !ledStato3;                                 // cambio lo stato del pin 13
      digitalWrite(led3, ledStato3);
      if (ledStato3 == 0) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(F("YOTTAMASTER OFF"));

      } else if (ledStato3 == 1) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(F("YOTTAMASTER ON"));
      }
      delay(500);  // Imposto al pin13 lo stato di ledStato
    }
    pulsanteStato3 = lettura3;  // assegno lo stato del pulsante al pin7
  }
  if ((millis() - ultimoTempoDebounce4) > attesaDebounce4) {  // se il conteggio è superiore a 50 millisecondi
    if (lettura4 != pulsanteStato4 and lettura4 == HIGH) {    // se il pin7 è diverso dallo stato del pulsante4 e il pulsante1 è premuto
      ledStato4 = !ledStato4;                                 // cambio lo stato del pin 13
      digitalWrite(led4, ledStato4);
      // Imposto al pin13 lo stato di ledStato
      if (ledStato4 == 0) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(F("MIXER OFF"));
      } else if (ledStato4 == 1) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(F("MIXER ON"));
      }
      delay(500);
    }
    pulsanteStato4 = lettura4;  // assegno lo stato del pulsante al pin7
  }
  ultimaLettura = lettura;
  ultimaLettura2 = lettura2;
  ultimaLettura3 = lettura3;
  ultimaLettura4 = lettura4;
  delay(10);
  // Creo una connessione al client
  EthernetClient client = server.available();
  if (client) {
    Serial.println("new client");
    memset(linebuf, 0, sizeof(linebuf));
    uint8_t charcount = 0;
    authentificated = false;
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        if (listener.length() < 100) {
          listener += c;
        }
        linebuf[charcount] = c;
        if (charcount < sizeof(linebuf) - 1) charcount++;
        Serial.write(c);
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
          if (authentificated)
            SendOKpage(client);
          else
            SendAuthentificationpage(client);
          break;
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
          if (strstr(linebuf, "Authorization: Basic") > 0 && strstr(linebuf, "YWxiZTphbGJlMjQ2OA==") > 0)
            authentificated = true;
          memset(linebuf, 0, sizeof(linebuf));
          charcount = 0;
        } else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(1);
    client.stop();
    if (listener.indexOf("?button1on") > 0) {
      digitalWrite(led1, HIGH);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(F("Acceso"));
      lcd.setCursor(0, 1);
      lcd.print(F("PC da WEB"));
      ledStato = 1;
      //inviaMagicPacket();
      delay(1000);
    }
    if (listener.indexOf("?button1off") > 0) {
      digitalWrite(led1, LOW);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(F("Spento"));
      lcd.setCursor(0, 1);
      lcd.print(F("PC da WEB"));
      ledStato = 0;
      delay(500);
    }
    if (listener.indexOf("?button0on") > 0) {
      digitalWrite(led2, HIGH);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(F("Accese"));
      lcd.setCursor(0, 1);
      lcd.print(F("CASSE da WEB"));
      ledStato2 = 1;
      // inviaMagicPacket2();
      delay(500);
    }
    if (listener.indexOf("?button0off") > 0) {
      digitalWrite(led2, LOW);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(F("Spente"));
      lcd.setCursor(0, 1);
      lcd.print(F("CASSE da WEB"));
      ledStato2 = 0;
      delay(500);
    }
    if (listener.indexOf("?button3on") > 0) {
      digitalWrite(led3, HIGH);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(F("Acceso"));
      lcd.setCursor(0, 1);
      lcd.print(F("YOTTAMASTER da WEB"));
      ledStato3 = 1;
      delay(500);
    }
    if (listener.indexOf("?button3off") > 0) {
      digitalWrite(led3, LOW);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(F("Spento"));
      lcd.setCursor(0, 1);
      lcd.print(F("YOTTAMASTER da WEB"));
      ledStato3 = 0;
      delay(500);
    }
    if (listener.indexOf("?button4on") > 0) {
      digitalWrite(led4, HIGH);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(F("Acceso"));
      lcd.setCursor(0, 1);
      lcd.print(F("MIXER da WEB"));
      ledStato4 = 1;
      delay(500);
    }
    if (listener.indexOf("?button4off") > 0) {
      digitalWrite(led4, LOW);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(F("Spento"));
      lcd.setCursor(0, 1);
      lcd.print(F("MIXER da WEB"));
      ledStato4 = 0;
      delay(500);
    }
    if (listener.indexOf("?buttonallon") > 0) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(F("Accendo Tutto"));
      lcd.setCursor(0, 1);
      lcd.print(F("da remoto"));
      digitalWrite(led1, HIGH);
      ledStato = 1;
      //inviaMagicPacket();
      delay(300);
      digitalWrite(led2, HIGH);
      ledStato2 = 1;
      inviaMagicPacket2();
      delay(300);
      digitalWrite(led3, HIGH);
      ledStato3 = 1;
      delay(300);
      digitalWrite(led4, HIGH);
      ledStato4 = 1;
      delay(500);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(F("Accensione"));
      lcd.setCursor(0, 1);
      lcd.print(F("effettuata"));
      lcd.setCursor(0, 2);
      lcd.print(F("da remoto"));
      digitalWrite(led5, HIGH);
      stato = !stato;
      delay(500);
    }
    if (listener.indexOf("?buttonalloff") > 0) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(F("Spengo Tutto"));
      lcd.setCursor(0, 1);
      lcd.print(F("da remoto"));
      digitalWrite(led6, LOW);
      digitalWrite(led1, LOW);
      ledStato = 0;
      delay(2000);
      digitalWrite(led2, LOW);
      ledStato2 = 0;
      delay(2000);
      digitalWrite(led3, LOW);
      ledStato3 = 0;
      delay(2000);
      digitalWrite(led4, LOW);
      ledStato4 = 0;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(F("Spegnimento"));
      lcd.setCursor(0, 1);
      lcd.print(F("effettuato"));
      lcd.setCursor(0, 2);
      lcd.print(F("da remoto"));
      digitalWrite(led5, LOW);
      stato = !stato;
      delay(500);
    }
    if (listener.indexOf("?lcdoff") > 0) {
        digitalWrite(led6, LOW);
      lcd.noBacklight();
    }
    if (listener.indexOf("?lcdon") > 0) {
        digitalWrite(led6, HIGH);
      lcd.backlight();
    }
    listener = "";
  }
}

// ***************************************** Fine LOOP *****************************
//funzione per la creazione e l’invio ********************************************** FUNZIONI **************************************************
//del magic packet
void inviaMagicPacket() {
  byte magicPacket[102];
  byte Ciclo = 0, CicloMacAdd = 0, IndiceArray = 0;
  for (Ciclo = 0; Ciclo < 6; Ciclo++) {
    magicPacket[IndiceArray] = 0xFF;
    IndiceArray++;
  }
  for (Ciclo = 0; Ciclo < 16; Ciclo++) {
    for (CicloMacAdd = 0; CicloMacAdd < 6; CicloMacAdd++) {
      magicPacket[IndiceArray] = remote_MAC_ADD[CicloMacAdd];
      IndiceArray++;
    }
  }
  Udp.beginPacket(broadCastIp, wolPort);
  Udp.write(magicPacket, sizeof magicPacket);
  Udp.endPacket();
  lcd.setCursor(0, 3);
  lcd.print(F("mp inviato!"));
}
void inviaMagicPacket2() {
  byte magicPacket2[102];
  byte Ciclo = 0, CicloMacAdd = 0, IndiceArray = 0;
  for (Ciclo = 0; Ciclo < 6; Ciclo++) {
    magicPacket2[IndiceArray] = 0xFF;
    IndiceArray++;
  }
  for (Ciclo = 0; Ciclo < 16; Ciclo++) {
    for (CicloMacAdd = 0; CicloMacAdd < 6; CicloMacAdd++) {
      magicPacket2[IndiceArray] = remote_MAC_ADD2[CicloMacAdd];
      IndiceArray++;
    }
  }
  Udp.beginPacket(broadCastIp, wolPort2);
  Udp.write(magicPacket2, sizeof magicPacket2);
  Udp.endPacket();
  lcd.setCursor(0, 3);
  lcd.print(F("mp2 inviato!"));
}
void pulsantieraOn(int ritardo) {
  lcd.clear();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print(F("**** SEZIONE 31 ****"));

  lcd.setCursor(0, 1);
  lcd.print(F("SCRIVANIA --> ON"));
  lcd.setCursor(0, 2);
  lcd.print(F("TEMPERATURA:"));
  lcd.setCursor(12, 2);
  lcd.print(Temp() - 1.75);
  lcd.print((char)223);
  lcd.setCursor(0, 3);
  lcd.print(F("*PULSANTIERA WEB2.1*"));
  delay(ritardo);
}
void DS3231_display() {
  // Converto BCD in decimale
  second = (second >> 4) * 10 + (second & 0x0F);
  minute = (minute >> 4) * 10 + (minute & 0x0F);
  hour = (hour >> 4) * 10 + (hour & 0x0F);
  date = (date >> 4) * 10 + (date & 0x0F);
  month = (month >> 4) * 10 + (month & 0x0F);
  year = (year >> 4) * 10 + (year & 0x0F);
  // Fine conversione
  Time[12] = second % 10 + 48;
  Time[11] = second / 10 + 48;
  Time[9] = minute % 10 + 48;
  Time[8] = minute / 10 + 48;
  Time[6] = hour % 10 + 48;
  Time[5] = hour / 10 + 48;
  Calendar[14] = year % 10 + 48;
  Calendar[13] = year / 10 + 48;
  Calendar[9] = month % 10 + 48;
  Calendar[8] = month / 10 + 48;
  Calendar[6] = date % 10 + 48;
  Calendar[5] = date / 10 + 48;
  lcd.setCursor(0, 0);
  lcd.print(Time);  // Scrivi sul display l'ora
  lcd.setCursor(14, 0);
  lcd.write(byte(2));
  lcd.setCursor(16, 0);
  lcd.write(byte(3));
  lcd.write(byte(3));
  lcd.write(byte(3));
  lcd.write(byte(3));
  lcd.setCursor(0, 1);
  lcd.print(Calendar);  // Scrivi sul display la data
  lcd.setCursor(0, 2);
  lcd.print(F("Temperatura: "));
  lcd.print(Temp() - 1.75);
  lcd.print((char)223);
  lcd.setCursor(0, 3);
  lcd.print(F("KeyStation r1.0 "));
  if (ledStato == 0) {
    lcd.setCursor(16, 3);
    lcd.write(byte(0));
  } else {
    lcd.setCursor(16, 3);
    lcd.write(byte(1));
  }
  if (ledStato2 == 0) {
    lcd.setCursor(17, 3);
    lcd.write(byte(0));
  } else {
    lcd.setCursor(17, 3);
    lcd.write(byte(1));
  }
  if (ledStato3 == 0) {
    lcd.setCursor(18, 3);
    lcd.write(byte(0));
  } else {
    lcd.setCursor(18, 3);
    lcd.write(byte(1));
  }
  if (ledStato4 == 0) {
    lcd.setCursor(19, 3);
    lcd.write(byte(0));
  } else {
    lcd.setCursor(19, 3);
    lcd.write(byte(1));
  }
}
void blink_parameter() {
  byte j = 0;
  while (j < 10 && digitalRead(8) && digitalRead(9)) {
    j++;
    delay(25);
  }
}
byte edit(byte x, byte y, byte parameter) {
  char text[3];
  while (!digitalRead(8))
    ;
  while (true) {
    while (!digitalRead(9)) {  // Se il pulsante (pin #9) è premuto
      parameter++;
      if (i == 0 && parameter > 23)  // Se l'ora > 23 ==> ora = 0
        parameter = 0;
      if (i == 1 && parameter > 59)  // Se i minuti > 59 ==> minuti = 0
        parameter = 0;
      if (i == 2 && parameter > 31)  // Se la data > 31 ==> data = 1
        parameter = 1;
      if (i == 3 && parameter > 12)  // Se il mese > 12 ==> mese = 1
        parameter = 1;
      if (i == 4 && parameter > 99)  // Se l'anno > 99 ==> anno = 0
        parameter = 0;
      sprintf(text, "%02u", parameter);
      lcd.setCursor(x, y);
      lcd.print(text);
      delay(200);  // Attendi 200ms
    }
    lcd.setCursor(x, y);
    lcd.print("  ");  // Inserisci due spazi
    blink_parameter();
    sprintf(text, "%02u", parameter);
    lcd.setCursor(x, y);
    lcd.print(text);
    blink_parameter();
    if (!digitalRead(8)) {  // Se il pulsante sul (pin #8) è premuto
      i++;                  // Incrementa 'i' di un parametro
      return parameter;     // Restituisce il valore del parametro e esce
    }
  }
}
float Temp() {
  Wire.beginTransmission(0x68);
  Wire.write(0x11);
  Wire.endTransmission();
  Wire.requestFrom(0x68, 2);
  byte MSBtemp = Wire.read();
  byte LSBtemp = Wire.read();
  if (MSBtemp >> 7 == 1)
    MSBtemp = ~(~MSBtemp + 1 + 128) + 1;
  float Temperature = (float)(MSBtemp) + (float)((LSBtemp >> 6) * 0.25);
  return Temperature;
}
void letturaPulsanti(int lett, int pul, int ulett, unsigned long ultDb) {
  lett = digitalRead(pul);
  if (lett != ulett) {
    ultDb = millis();
  }
}
//*************************************** FUNZIONI PAGINA WEB NEW *********************************************************
void SendOKpage(EthernetClient &client) {
  // send a standard http response header
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println();
  client.println(F("<HTML>"));
  client.println(F("<HEAD>"));
  client.println(F("<meta name='apple-mobile-web-app-capable' content='yes' />"));
  client.println(F("<meta name='apple-mobile-web-app-status-bar-style' content='black-translucent' />"));
  client.println(F("<link rel='stylesheet' type='text/css' href='http://www.progettiarduino.com/uploads/8/1/0/8/81088074/style3.css' />"));
  client.println(F("<TITLE>KEYSTATION 1.1</TITLE>"));
  client.println(F("</HEAD>"));
  client.println(F("<BODY>"));
  client.println(F("<H1>KEYSTATION - Sezione31 - 2024</H1>"));
  client.println(F("<hr />"));
  client.println(F("<br />"));
  client.println("<H2>Pulsantiera Arduino con Ethernet Shield</H2>");
  client.println("<br />");
  client.println(F("<a href=\"/?button1on\"\">PC ON</a>"));
  client.println(F("<a href=\"/?button1off\"\">PC OFF</a><br />"));
  client.println(F("<br />"));
  client.println(F("<br />"));
  client.println(F("<a href=\"/?button0on\"\">CASSE ON</a>"));
  client.println(F("<a href=\"/?button0off\"\">CASSE OFF</a><br />"));
  client.println(F("<br />"));
  client.println(F("<br />"));
  client.println(F("<a href=\"/?button3on\"\">YOTTAMASTER ON</a>"));
  client.println(F("<a href=\"/?button3off\"\">YOTTAMASTER OFF</a><br />"));
  client.println(F("<br />"));
  client.println(F("<br />"));
  client.println(F("<a href=\"/?button4on\"\">MIXER ON</a>"));
  client.println(F("<a href=\"/?button4off\"\">MIXER OFF</a><br />"));
  client.println(F("<br />"));
  client.println(F("<br />"));
  client.println(F("<a href=\"/?buttonallon\"\">TUTTO ON</a>"));
  client.println(F("<a href=\"/?buttonalloff\"\">TUTTO OFF</a><br />"));
  client.println(F("<br />"));
  client.println(F("<br />"));
  client.println(F("<a href=\"/?lcdon\"\">LCD ON</a>"));
  client.println(F("<a href=\"/?lcdoff\"\">LCD OFF</a>"));
  client.println(F("<br />"));
  client.println(F("<br />"));
  client.println(F("<H2>Creato da Sezione31 - Alberto Corti</H2>"));
  client.println(F("<br />"));
  client.println(F("</BODY>"));
  client.println(F("</HTML>"));
}

void SendAuthentificationpage(EthernetClient &client) {
  client.println("HTTP/1.1 401 Authorization Required");
  client.println("WWW-Authenticate: Basic realm=\"Secure Area\"");
  client.println("Content-Type: text/html");
  client.println("Connnection: close");
  client.println();
  client.println("<!DOCTYPE HTML>");
  client.println("<HTML>  <HEAD>   <TITLE>Error</TITLE>");
  client.println(" </HEAD> <BODY><H1>401 Unauthorized.</H1></BODY> </HTML>");
}

/* void Receive_Serial_Data()
{
char cs = Serial.read();
  while (Serial.available() > 0) {
    
    if (cs == '\n') {
      break;
    } else {
      dataIn += cs;
    }
  }
  if (cs == '\n') {
    temp = dataIn.toFloat();
    cs = '0';
    dataIn = "";
    Serial.println((String) "Temperatura = " + temp);
  }
} */
