#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <Adafruit_ST7789.h>
#include <SPI.h>
#include <Adafruit_LIS3DH.h>
#include "pitches.h"

#define TFT_CS         13
#define TFT_RST        1
#define TFT_DC         12

//************* change ssid and password to your own settings ************

const char* ssid = "Get-2G-934E04";
const char* password = "zzy3cthj2n";

//************************************************************************

Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST); // for the tft screen
Adafruit_LIS3DH lis = Adafruit_LIS3DH(); // for the accelerometer sensor built into the esp32

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting...");
  }
  Serial.println("Connected!");

  tft.init(135, 240);
  tft.setSPISpeed(4000000);

  uint16_t time = millis();
  time = millis() - time;
  
  if (! lis.begin(0x18)) {
    while (1) yield();
  }

  startupscreen();
  startupJingle();
  delay(2000);
  
  welcomeText();
  delay(10000);
}

void loop() {
  if ((WiFi.status() == WL_CONNECTED)) {
    HTTPClient http;
    http.begin("https://www.boredapi.com/api/activity");
    int httpCode = http.GET();
    lis.read();
   
    if (lis.z < -10000) {
      if (httpCode > 0) {
        String payload = http.getString();
        tft.fillScreen(ST77XX_BLACK);
        tft.setCursor(0, 70);
        tft.setTextSize(2);
        tft.setTextColor(ST77XX_GREEN);
        parsingPayload(payload);
        dingNoice();
        delay(20000);
        tft.fillScreen(ST77XX_BLACK);
      } else {
        Serial.println("Error on HTTP request");
      }
    
    http.end(); //free up resources
    
    } else {
      tft.fillScreen(ST77XX_BLACK);
      standbyGrapic();
      standbyMusic();
      impatientFlipMessage();
      delay(1000);
    }
  }
  tft.setCursor(0, 0);
  tft.setTextWrap(true);
}

//*************** parsing section ********************

void parsingPayload(String payload){
  DynamicJsonDocument doc(2048);
  deserializeJson(doc,payload);
  const char* _activity = doc["activity"];
  tft.println(_activity);
}

//************ grapic Section ***************
void standbyGrapic() {
  
  int randomNumberV = 0;
  int randomNumberH = 0;
  int randomcursorx = 0;
  int randomCursory = 0;
  
  for (int i = 0; i < 7 ; i++) {
    int randomNumberV = random(135);
    int randomNumberH = random(240);
    int randomcursorX = random(120);
    int randomCursorY = random(230);
    
    tft.drawFastVLine(randomNumberV,0,240,ST77XX_GREEN);
    tft.drawFastHLine(0,randomNumberH,240,ST77XX_GREEN);
    
    tft.setCursor(randomcursorX, randomCursorY);
    tft.setTextColor(ST77XX_GREEN);
    tft.setTextSize(3);
    tft.println("?");
  }
}

//********* text section ************

void startupscreen() {
  tft.fillScreen(ST77XX_BLACK);
  tft.setCursor(0, 0);
  tft.setTextWrap(true);
  tft.setTextSize(5);
  tft.setTextColor(ST77XX_RED);
  tft.println("WHAT");
  tft.setTextColor(ST77XX_GREEN);
  tft.println("TO");
  tft.setTextColor(ST77XX_BLUE);
  tft.println("DO");
  tft.setTextColor(ST77XX_YELLOW);
  tft.println("NOW?");
  tft.setTextSize(1);
  tft.setTextColor(ST77XX_WHITE);
  tft.println("v.1.0");
}
void welcomeText() {
  tft.fillScreen(ST77XX_BLACK);
  tft.setCursor(15, 40);
  tft.setTextColor(ST77XX_RED);
  tft.setTextSize(2);
  tft.println("Flip me");
  tft.setCursor(15, 58);
  tft.println("over and");
  tft.setCursor(15, 76);
  tft.println("wait for");
  tft.setCursor(15, 94);
  tft.println("the ding!");
  tft.setCursor(15, 112);
  tft.println("");
  tft.setCursor(15, 130);
  tft.println("Then i'll");
  tft.setCursor(15, 148);
  tft.println("give you");
  tft.setCursor(15, 166);
  tft.println("something");
  tft.setCursor(15, 184);
  tft.println("fun to do");
}

void impatientFlipMessage() {
  tft.fillScreen(ST77XX_BLACK);
  tft.setCursor(5, 100);
  tft.setTextColor(ST77XX_RED);
  tft.setTextSize(3);
  tft.println("Flip me");
  tft.setCursor(50, 138);
  tft.println("<3");
}

//***************** music section *************************

void dingNoice() {
  int ding[] = {
    NOTE_B0, NOTE_G4,
  };
  int noteDurations[] = {
    8, 8
  };

  for (int thisNote = 0; thisNote < 2; thisNote++) {
    int noteDuration = 1000 / noteDurations[thisNote];
    tone(8, ding[thisNote], noteDuration);

    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
  }
}
void startupJingle() {
  int startupTune[] = {
    NOTE_A4, NOTE_D4, NOTE_F4, NOTE_G4, NOTE_A4, NOTE_A4, NOTE_D4,
  };
  int noteDurations[] = {
    4, 4, 8, 8, 2, 4, 1
  };

  for (int thisNote = 0; thisNote < 7; thisNote++) {
    int noteDuration = 1000 / noteDurations[thisNote];
    tone(8, startupTune[thisNote], noteDuration);

    int pauseBetweenNotes = noteDuration * 1.80;
    delay(pauseBetweenNotes);
  }
}
void standbyMusic() {
  int standbyTune[] = {
    NOTE_FS4, 0, NOTE_A4, NOTE_CS5, 0, NOTE_A4, 0, NOTE_FS4, NOTE_D4, NOTE_D4,
    NOTE_D4, 0, 0, 0, NOTE_CS4, NOTE_D4, NOTE_FS4, NOTE_A4, NOTE_CS5, 0, NOTE_A4,
    0, NOTE_F4, NOTE_E5, NOTE_DS5, NOTE_D5, 0, 0, NOTE_GS4, 0, NOTE_CS5, NOTE_FS4,
  };
  int noteDurations[] = {
    8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 4, 8,
    8, 8, 8, 8, 8, 8, 8, 8, 8, 4, 8, 8, 8, 4, 8, 8, 8, 8
  };

  for (int thisNote = 0; thisNote < 28; thisNote++) {
    int noteDuration = 1000 / noteDurations[thisNote];
    tone(8, standbyTune[thisNote], noteDuration);

    int pauseBetweenNotes = noteDuration * 2.40;
    delay(pauseBetweenNotes);
  }
}
