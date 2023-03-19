
#include <ESP8266WiFi.h>
#include <LiquidCrystal_I2C.h>
#include <DHTesp.h>

WiFiServer server(80);
LiquidCrystal_I2C lcd(0x27, 20, 4);
DHTesp dht;

int BUILDIN_LED = 2;
float h = dht.getHumidity();
float t = dht.getTemperature();
const char* ssid = "*";
const char* password = "43167618394590382086";

void setup(){  
  Serial.begin(115200);

  String thisBoard= ARDUINO_BOARD;
  Serial.println(thisBoard);

  dht.setup(14, DHTesp::DHT22);

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("ESP8266 WebServer");

  pinMode(BUILDIN_LED, OUTPUT);
}

void setupWiFi(){
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.println(".");
  }
  Serial.println("Server gestartet");
  Serial.println("IP-Adresse");
  Serial.println(WiFi.localIP().toString());
  server.begin();
}

void writeResponse(WiFiClient client){
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("");
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  client.println("<head>");
  client.println("<meta http-equiv='refresh' content='5; URL=http://"+WiFi.localIP().toString()+"'/>");
  client.println("</head>");
  client.println("<body>");

  if (isnan(h) || isnan(t)) {
    client.println("Fehler beim lesen der Sensorwerte!");
  } else {
    client.print("Temperatur: ");
    client.print(String(t,2));
    client.println(" &deg;C");
    client.println("<br/>");
    client.print("rel. Luftfeuchtigkeit: ");
    client.print(String(h,2));
    client.println(" %");
  }
  client.println("</body>");
  client.println("</html>");
}

void loop(){
  float h = dht.getHumidity();
  float t = dht.getTemperature();

  if (isnan(h) || isnan(t)){
    Serial.println("> Failed to read from DHT Sensor <");
    return;
  }
  lcd.setCursor(0, 2);
  lcd.print("Hum: ");
  lcd.print(h);
  lcd.print(" % ");
  lcd.setCursor(0, 3);
  lcd.print("Temp: ");
  lcd.print(t);
  lcd.print("C");
  
  digitalWrite(BUILDIN_LED, LOW);
  delay(1000);
  digitalWrite(BUILDIN_LED, HIGH);
  delay(1000);

  if(WiFi.status() != WL_CONNECTED){
      setupWiFi();
   }

   WiFiClient client = server.accept();
   if (!client){
    return;
   }

   while(!client.available()){
    delay(1);
   }

   Serial.println("Client verbunden");
   lcd.setCursor(0,1);
   lcd.print("Status OK");

   writeResponse(client);
   delay(1000);
}
