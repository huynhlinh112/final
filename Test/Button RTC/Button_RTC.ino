/* Sketch for Tutorial 14
Author: Maximilian Hentsch
Sketch zu Arduino Tutorial 14 - Displaymenü
https://www.youtube.com/watch?v=DuAG98P9Seo
*/
#include <ESP8266WiFi.h>
#include <time.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


#define OLED_RESET LED_BUILTIN  //4
Adafruit_SSD1306 display(OLED_RESET);
int downButton = D4;
int selectButton = D3;
int CoutButton = D5;
int menu = -1;
int count = 0;
int i = 1;

const char* ssid = "Hoang Trang";
const char* password = "hoangtrang";

//SCL -- D1
//SDA -- D2

int ledPin = 13;

int timezone = 7 * 3600;
int dst = 0;

#if (SSD1306_LCDHEIGHT != 64)
//#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

void setup() {
   display.begin(SSD1306_SWITCHCAPVCC, 0x3C);

  // Clear the buffer.
  display.clearDisplay();
  display.display();

   pinMode(ledPin,OUTPUT);
  digitalWrite(ledPin,LOW);

  Serial.begin(115200);

  display.setTextSize(1);
  display.setTextColor(WHITE);
  
  display.setCursor(0,0);
  display.println("Wifi connecting to ");
  display.println( ssid );

  WiFi.begin(ssid,password);
 
  display.println("\nConnecting");

  display.display();

  while( WiFi.status() != WL_CONNECTED ){
      delay(500);
      display.print("."); 
      display.display();       
  }

  // Clear the buffer.
  display.clearDisplay();
  display.display();
  display.setCursor(0,0);
  
  display.println("Wifi Connected!");
  display.print("IP:");
  display.println(WiFi.localIP() );

  display.display();

  configTime(timezone, dst, "pool.ntp.org","time.nist.gov");
  display.println("\nWaiting for NTP...");

  while(!time(nullptr)){
     Serial.print("*");
     
     delay(1000);
  }
  display.println("\nTime response....OK"); 
  display.display();  
  delay(1000);

  Serial.begin(115200);

  display.setTextSize(1);
  display.setTextColor(WHITE);
  pinMode(downButton, INPUT_PULLUP);
  pinMode(selectButton, INPUT_PULLUP);
  pinMode(CoutButton, INPUT_PULLUP);
  display.clearDisplay();
  display.display();
}

void loop() {

  if (!digitalRead(downButton)){
    menu++;
    //updateMenu();
    Serial.println("BUTTON 2 Up: ");
    Serial.println(menu);
    delay(100);
    while (!digitalRead(downButton));
  }

  if (!digitalRead(selectButton)){
    Serial.println("BUTTON Select: ");
    Serial.println(count);
    count++;
    delay(100);
    while (!digitalRead(selectButton));
  }

    if (!digitalRead(CoutButton)){
    Serial.println("chon nhac:");
    Serial.println(i);
    i++;
    delay(100);
    while (!digitalRead(CoutButton));
  }
 
  if (count > 0)
  {
    if (count == 1) {
      updateMenu();
    }
    else if (count == 2) {
       executeAction( menu );
     }
     else if (count == 3) {
       updateMenu();
     }
      else if (count == 4) {
       executeAction( menu );
     }
      else if (count == 5) {
       updateMenu();
     }
      else if (count == 6) {
       executeAction( menu );
     }
      else if (count == 7) {
       updateMenu();
     }
      else if (count == 8) {
       executeAction( menu );
     }
     else
      count = 0;
  }
  else
  {
     RTC_I();
   }

}

void updateMenu() {
  switch (menu) {
    case 0:
      menu = 1;
      break;
    case 1:
      display.clearDisplay();
      display.setCursor(0,1);
      display.println("> Folder Music.");
      display.println("  Set Time.");
      display.println("  Volume.");
      display.println("  All case.");
      display.display(); 
      break;
    case 2:

      display.clearDisplay();
      display.setCursor(0,0);
      display.println("  Folder Music.");
      display.println("> Set Time.");
      display.println("  Volume.");
      display.println("  All case.");
      display.display(); 
      break;
    case 3:

      display.clearDisplay();
      display.setCursor(0,0);
      display.println("  Folder Music.");
      display.println("  Set Time. ");
      display.println("> Volume. ");
      display.println("  All case. ");
      display.display(); 
      break;
    case 4:
      display.clearDisplay();
      display.setCursor(0,0);
      display.println("  Folder Music.");
      display.println("  Set Time. ");
      display.println("  Volume. ");
      display.println("> All case. ");
      display.display(); 
      break;
    case 5:
        menu = 0;
      break;
  }
}

void executeAction(int menu) {
  switch (menu) {
    case 1:
      action1();
      break;
    case 2:
      action2();
      break;
    case 3:
      action3();
      break;
    case 4:
      action4();
      break;
  }
}

void action1() {
  display.clearDisplay();
  display.setCursor(0,0);
  updateMusic();
  display.display(); 
  delay(100);
}
void action2() {
  display.clearDisplay();
  display.setCursor(0,0);
  display.println("Zo day menu 2. ");
  display.display(); 
  delay(100);
}
void action3() {
  display.clearDisplay();
  display.setCursor(0,0);
  display.println("Zo day menu 3. ");
  display.display(); 
  delay(100);
}
void action4() {
   display.clearDisplay();
   display.setCursor(0,0);
  display.print("Zo day menu 4. ");
  display.display(); 
  delay(100);
}

//
void updateMusic() {
  switch (i) {
    case 0:
      i = 1;
      break;
    case 1:
      display.clearDisplay();
      display.setCursor(0,1);
      display.println("> Jazz.");
      display.println("  Pop. ");
      display.println("  Rock. ");
      display.println("  Classic. ");
      display.display(); 
      break;
    case 2:
      display.clearDisplay();
      display.setCursor(0,0);
      display.println("  Jazz.");
      display.println("> Pop. ");
      display.println("  Rock. ");
      display.println("  Classic. ");
      display.display(); 
      break;
    case 3:

      display.clearDisplay();
      display.setCursor(0,0);
      display.println("  Jazz.");
      display.println("  Pop. ");
      display.println("> Rock. ");
      display.println("  Classic. ");
      display.display(); 
      break;
    case 4:
      display.clearDisplay();
      display.setCursor(0,0);
      display.println("  Jazz.");
      display.println("  Pop. ");
      display.println("  Rock. ");
      display.println("> Classic. ");
      display.display(); 
      break;
    case 5:
        i = 1;
      break;
  }
}

void RTC_I (){
  
  time_t now = time(nullptr);
  struct tm* p_tm = localtime(&now);
  
  Serial.print(p_tm->tm_mday);
  Serial.print("/");
  Serial.print(p_tm->tm_mon + 1);
  Serial.print("/");
  Serial.print(p_tm->tm_year + 1900);
  
  Serial.print(" ");
  
  Serial.print(p_tm->tm_hour);
  Serial.print(":");
  Serial.print(p_tm->tm_min);
  Serial.print(":");
  Serial.println(p_tm->tm_sec);
  
  // Clear the buffer.
  display.clearDisplay();
 
  display.setTextSize(3);
  display.setTextColor(WHITE);
  
  display.setCursor(0,0);
  display.print(p_tm->tm_hour);
  display.print(":");
  if( p_tm->tm_min <10)
    display.print("0"); 
  display.print(p_tm->tm_min);
  
  display.setTextSize(2);
  display.setCursor(90,5);
  display.print(".");
  if( p_tm->tm_sec <10)
    display.print("0"); 
  display.print(p_tm->tm_sec);

  display.setTextSize(1);
  display.setCursor(24,25);
  display.print(p_tm->tm_mday);
  display.print("/");
  display.print(p_tm->tm_mon + 1);
  display.print("/");
  display.print(p_tm->tm_year + 1900);

  display.display();

  delay(1000); // update every 1 sec
  
  }
