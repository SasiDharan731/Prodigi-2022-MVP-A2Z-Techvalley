#include <SoftwareSerial.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <TinyGPS++.h>
#include <ESP8266WiFi.h>
 
const char* ssid = "";
const char* password = "";

char* text;
int value;
WiFiServer server(80);
 

#define rxPin 14
#define txPin 12
SoftwareSerial mygps(rxPin, txPin);

#define SCREEN_WIDTH 128     // OLED display width, in pixels
#define SCREEN_HEIGHT 64     // OLED display height, in pixels
#define OLED_RESET -1        //Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C  //See datasheet for Address
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

TinyGPSPlus gps;


void setup() {
  Serial.begin(115200);
  mygps.begin(9600);
  pinMode(0, OUTPUT);

  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    // Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ;  // Don't proceed, loop forever
  }
  display.clearDisplay();
  display.display();
  // Serial.println("Heyy");
  // delay(2000);

   Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.persistent( false );

  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
 
  // Start the server
  server.begin();
  Serial.println("Server started");
 
  // Print the IP address
  Serial.print("Use this URL to connect: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");
 
}


void loop() {
  boolean newData = false;
  for (unsigned long start = millis(); millis() - start < 999;) {
    while (mygps.available()) {
      if (gps.encode(mygps.read())) {
        newData = true;
      }
    }
  }

  //If newData is true
  if (newData == true) {
    newData = false;
    display.setTextColor(SSD1306_WHITE);

    if (gps.location.isValid() == 1) {
      //String gps_speed = String(gps.speed.kmph());
      // Serial.println(gps.location.lat(), 6);
      // display.setCursor(0, 0);
      // display.setTextSize(1);
      // display.print("Lat: ");
      // display.println(gps.location.lat(), 6);

      // display.setCursor(0, 10);
      // display.setTextSize(1);
      // display.print("Lng: ");
      // display.println(gps.location.lng(), 6);

      // display.setCursor(0, 20);
      // display.setTextSize(1);
      // display.print("Alt: ");
      // display.print(gps.altitude.kilometers());
      // display.println("Km");

      // display.setCursor(0, 30);
      // display.print(F("Tim: "));

      // display.setCursor(30, 30);
      // display.print(gps.time.hour());

      // display.setCursor(43, 30);
      // display.print(":");

      // display.setCursor(50, 30);
      // display.print(gps.time.minute());

      // display.setCursor(63, 30);
      // display.print(":");

      // display.setCursor(70, 30);
      // display.print(gps.time.second());



      // display.setCursor(0, 40);
      // display.print(F("Dat: "));


      // display.setCursor(30, 40);
      // display.print(gps.date.day());

      // display.setCursor(42, 40);
      // display.print("/");

      // display.setCursor(47, 40);
      // display.print(gps.date.month());

      // display.setCursor(54, 40);
      // display.print("/");

      // display.setCursor(62, 40);
      // display.print(gps.date.year());

      WiFiClient client = server.available();
  if (!client) {
    return;
  }
 
  // Wait until the client sends some data
  Serial.println("new client");
  while(!client.available()){
    delay(1);
  }
 
  // Read the first line of the request
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();
 
  // Match the request
 

  Serial.println(value);
  if (request.indexOf("/LED=ON") != -1)  {
  
    value = HIGH;

     
  
    
  }
  if (request.indexOf("/LED=OFF") != -1)  {
   
    value = LOW;

     
  }

  if (request.indexOf("/LED=LEFT") != -1)  {
   
    value = 3;

    
  }
 
  if (request.indexOf("/LED=RIGHT") != -1)  {
  
    value = 2;

  }

   if (request.indexOf("/LED=HAZARD") != -1)  {
  
    value = 4;

  }

// Set ledPin according to the request
//digitalWrite(ledPin, value);
 
  // Return the response
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println(""); //  do not forget this one
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
 
  client.print("Now moving: ");
 
  if(value == HIGH) {
    client.print("Front");

    text = "Go Front";
   
  } else if(value == LOW){
    client.print("Back");

       text = "Go Back";

    
  } else if(value == 3){
     client.print("Left");

       text = "Go Left";

  }else if(value == 2){
     client.print("Right");

       text = "Go Right";

     
  }else if(value == 4){
     client.print("Ristricted area");

       text = "Restricted area";

    for(int i = 0; i < 10; i++){
      if(i % 2 == 0){
        digitalWrite(0, HIGH);
        delay(400);
      }else{
        digitalWrite(0, LOW);
        delay(400);

      }
    }  
  }
  client.println("<br><br>");
  client.println("<a href=\"/LED=ON\"\"><button>Front </button></a><br />");
    client.println("<br><br>");
  client.println("<a href=\"/LED=OFF\"\"><button>Back </button></a><br />");  
    client.println("<br><br>");
  client.println("<a href=\"/LED=LEFT\"\"><button>Left </button></a><br />");
    client.println("<br><br>");
  client.println("<a href=\"/LED=RIGHT\"\"><button>Right </button></a><br />");
    client.println("<br><br>");  
   client.println("<a href=\"/LED=HAZARD\"\"><button>Hazard on </button></a><br />");
    client.println("<br><br>"); 
  client.println("</html>");
 
  delay(1);
  Serial.println("Client disonnected");
  Serial.println("");
  
  display.setCursor(0, 0);
  display.setTextSize(1);
  display.print("Exit directions");
  
  display.setCursor(0, 15);
  display.setTextSize(2);
  display.print(text);


  display.setTextSize(1);
  display.setCursor(0, 50);
  display.print("SAT:");
  display.setCursor(30, 50);
  display.print(gps.satellites.value());

  display.display();

  display.clearDisplay();
    }
  }
}
