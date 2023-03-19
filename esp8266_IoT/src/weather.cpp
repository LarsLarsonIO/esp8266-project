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

 
  WiFi.begin(ssid, password); 

  int index = 0;
  while (WiFi.status() != WL_CONNECTED) { 
    delay(500); 
    index = index +1; 
    Serial.println(index,18);
    Serial.println("-");
  }

  Serial.println("Server gestartet");
  server.begin();
  Serial.println("IP-Adresse");
  Serial.println(WiFi.localIP().toString());
}

void loop(){
   if(WiFi.status() != WL_CONNECTED){
      setup();
   }
   WiFiClient client = server.available();
   if (!client){
    return;
   }
   Serial.println("Client verbunden");
   while(!client.available()){
    delay(1);
   }
   writeResponse(client);
}

void writeResponse(WiFiClient client){
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("");
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  client.println("<body>");
  client.println("ESP8266");
  client.println("</body>");
  client.println("</html>");
}
