#include <ESP8266WiFi.h>
#include <LiquidCrystal_I2C.h>

const char* ssid = "*";
const char* password = "43167618394590382086";

WiFiServer server(80);
LiquidCrystal_I2C lcd(0x27, 20, 4);

void setup(){
  Serial.begin(115200);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 1);
  lcd.println("SSID");
  lcd.println(ssid);

  //Initialisieren der Wifi Verbindung.
  WiFi.begin(ssid, password); 

  int index = 0;
  //Warten bis die Verbindung aufgebaut wurde.
  while (WiFi.status() != WL_CONNECTED) { 
    delay(500); //eine kleine Pause von 500ms
    index = index +1; //incrementieren des Indexes
    //setzen des Cursors auf die Spalte=<index> und Zeile=18
    Serial.println(index,18);
    //schreiben eines Striches an die gesetzte Stelle
    Serial.println("-");
  }

  Serial.println("Server gestartet");
  //starten des Servers
  server.begin(); // Starten des Servers.
  //setzen des Cursors auf die Zeile=10; Spalte=0  
  Serial.println("IP-Adresse");
  //schreiben des Textes auf das Serial
  Serial.println(WiFi.localIP().toString());
}

void loop(){
   if(WiFi.status() != WL_CONNECTED){
      setup();
   }
}
