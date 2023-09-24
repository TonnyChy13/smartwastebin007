#include <WiFi.h>
#include <DHT.h>
#include <WiFiClient.h>
#include <HTTPClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <HCSR04.h>
#define buzzer 14
#define green_led 27

// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);

//DHT11 for reading temperature and humidity value
#define DHTPIN 26
String temp;
String hum;
String sendval1, sendval2, sendval3, postData;
#define DHTTYPE DHT11     // DHT 11
DHT dht(DHTPIN, DHTTYPE);
HCSR04 hc(13, 12);   // GPIO 13, GPIO 12
// Your WiFi credentials.
// Set password to "" for open networks.
const char* ssid = "Tonny";
const char* password = "11223344";
//// Set your Static IP address
IPAddress local_IP(192, 168, 0, 184);
//// Set your Gateway IP address
IPAddress gateway(192, 168, 0, 1);
//
IPAddress subnet(255, 255, 0, 0);
IPAddress primaryDNS(8, 8, 8, 8);   //optional
IPAddress secondaryDNS(8, 8, 4, 4); //optional

WebServer server(80);

void handleRoot() {
  server.send(200, "text/plain", "hello from esp8266!");
}

void handleNotFound() {

  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}
int dis;
int level;
void setup()
{
  // Debug console
  Serial.begin(115200);
  pinMode(buzzer, OUTPUT);
  pinMode(green_led, OUTPUT);
  lcd.begin();                // Turn on the blacklight and print a message.
  lcd.backlight();
  WiFi.begin(ssid, password);
  Serial.println("");
  lcd.setCursor(0,0);
  lcd.print("Connecting WIFI");
  lcd.setCursor(0,1);
  int count = 0;
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    lcd.print(".");
    count++;
    if(count>15)
    {
      count = 0;
      lcd.setCursor(0,1);
      lcd.print("                ");
      lcd.setCursor(0,1);
    }
  }
  
  Serial.println("");
  Serial.print("Connected to ");
  lcd.setCursor(0,0);
  lcd.print("  Connected to  ");
  lcd.setCursor(0,1);
  lcd.print(ssid);
  delay(1000);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("   IP Address   ");
  lcd.setCursor(0,1);
  lcd.print(WiFi.localIP());
  delay(2000);
  lcd.clear();
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  dht.begin();
  server.on("/", handleRoot); // http://localIPAddress/
  server.on("/dht-temp", []() // http://localIPAddress/dht-temp
  {
    int t = dht.readTemperature(); 
    temp = String(t);
    Serial.println();
    Serial.print("Temperature:");
    Serial.println(temp);
    server.send(200, "text/plain", temp);
  });
  server.on("/dht-hum", []()  // http://localIPAddress/dht-hum
  {
    int h = dht.readHumidity();
    hum = String(h);
    Serial.print("Humidity:");
    Serial.println(hum);
    server.send(200, "text/plain", hum);
  });

  server.on("/level", []()  // http://localIPAddress/level
  {
    server.send(200, "text/plain", String(level_sensor()));
  });
  server.onNotFound(handleNotFound);
  server.begin();
  Serial.println("HTTP server started");
  lcd.setCursor(0, 0);
  lcd.print("    Smart Bin  ");
  lcd.setCursor(0, 1);
  lcd.print("Level: ");
}
unsigned long interval = millis();
void loop()
{
  server.handleClient();
  level_sensor();
  if(millis() - interval > 10000) // send data to server at every 10 secs interval
  {
    sendToServer();
    interval = millis(); 
  }
}

int level_sensor()
{
  dis = hc.dist();

  if (dis > 70)
    dis = 70;
  level = map(dis, 5, 70, 100, 0);
  if (level > 100)
    level = 100;
  if ( level > 90)
    alarm();
  else
  {
    digitalWrite(green_led, 1);
  }
  lcd.setCursor(6, 1);
  lcd.print(level);
  lcd.print("%  ");
//  Serial.print("Distance:");
//  Serial.println(dis);
  delay(100);
  return level;
}
void alarm()
{
  digitalWrite(buzzer, HIGH);
  digitalWrite(green_led, LOW);
  delay(300);
  digitalWrite(buzzer, LOW);
  delay(300);
}

void sendToServer()
 {
  // wait for WiFi connection
  if ((WiFi.status() == WL_CONNECTED)) {

    WiFiClient client;

    HTTPClient http;    // http object of clas HTTPClient


    // Convert integer variables to string
    sendval1 = String(dht.readTemperature());
    sendval2 = String(dht.readHumidity());
    sendval3 = String(level_sensor());


    postData = "sendval1=" + sendval1 + "&sendval2=" + sendval2 + "&sendval3=" + sendval3;
    // Update Host URL here:-

    http.begin(client, "https://192.168.0.103/esp-32_smartbin/smartwastebin007/database_write_bin2.php");              // Connect to host where MySQL databse is hosted
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");            //Specify content-type header
    int httpCode = http.POST(postData);   // Send POST request to php file and store server response code in variable named httpCode
    Serial.println("Values are, " + postData);

    // if connection eatablished then do this
    if (httpCode == 200) {
      Serial.println("Values uploaded successfully."); 
      Serial.println(httpCode);
      String webpage = http.getString();    // Get html webpage output and store it in a string
      Serial.println(webpage + "\n");
    }

    // if failed to connect then return and restart

    else {
      Serial.println(httpCode);
      Serial.println("Failed to upload values. \n");
      http.end();
      return;
    }

  }
 }
