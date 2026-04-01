#include <Arduino.h>
#include "config.h"
#include "wifi_manager.h"
#include "ui.h"
#include "touch.h"
#include "buzzer.h"
#include "melodies.h"
#include "ha_config.h"
#include "mqtt_manager.h"
#include "ha_interface.h"
#include <WiFi.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include <FluxGarage_RoboEyes.h>

// Create display
Adafruit_SH1106G display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Create RoboEyes instance (TEMPLATE VERSION)
RoboEyes<Adafruit_SH1106G> roboEyes(display);

// Mood tracking
int currentMood = 0;
bool lastTouchState = LOW;

// WiFi Connected State Tracking
bool wifiConnectedState = !WiFi.isConnected();
bool wifiFirstBoot = true;

// First Boot Flag (track if this is the first boot)
bool firstBoot = true;

//******************************************************************************************************
// Pause RoboEyes and show a full-screen message
//******************************************************************************************************
void showMessagePauseEyes(const char* line1, const char* line2 = nullptr, uint16_t duration_ms = 3000) {
    // Stop RoboEyes updates temporarily
    display.clearDisplay();
    display.setCursor(0, 0);
    display.setTextSize(1);
    display.setTextColor(SH110X_WHITE);

    display.println(line1);
    if (line2) display.println(line2);

    display.display();

    delay(duration_ms);  // leave message visible

    // Resume RoboEyes animation
    display.clearDisplay();
    roboEyes.begin(SCREEN_WIDTH, SCREEN_HEIGHT, ROBO_FPS);
}

//******************************************************************************************************
// ANIMATIONS ENGINE
//******************************************************************************************************
enum AnimationType { ANIM_NONE, ANIM_WIFI_CONNECT, ANIM_WIFI_DISCONNECT, ANIM_BOOT, ANIM_SLEEP, ANIM_HAPPY };

AnimationType currentAnim = ANIM_NONE;
int animStep = 0;
unsigned long animStart = 0;
bool animStarted = false;

void playAnimation(AnimationType anim) {
  if (animStarted) return;
    currentAnim = anim;
    animStep = 0;
    animStart = millis();
}

void updateAnimation() {
    if(currentAnim == ANIM_NONE) return;

    unsigned long t = millis() - animStart;

    switch(currentAnim) {

        case ANIM_WIFI_CONNECT:
            if(animStep == 0 && t >= 0){ animStarted = true; roboEyes.setMood(HAPPY); roboEyes.open(); roboEyes.setPosition(DEFAULT); roboEyes.setCuriosity(OFF); roboEyes.setSweat(OFF); animStep = 1; }
            else if(animStep == 1 && t >= 900){ roboEyes.anim_laugh(); animStep = 2; }
            else if(animStep == 2 && t >= 3000){ roboEyes.setMood(DEFAULT); animStep = 3; }
            else if(animStep == 3 && t >= 5000){ roboEyes.setCuriosity(ON); currentAnim = ANIM_NONE; animStarted = false; }
        break;

        case ANIM_WIFI_DISCONNECT:
            if(animStep == 0 && t >= 0){ animStarted = true; roboEyes.setAutoblinker(OFF); roboEyes.close(); roboEyes.setPosition(S); roboEyes.setCuriosity(OFF); roboEyes.setSweat(ON); animStep = 1; }
            else if(animStep == 1 && t >= 1000){ roboEyes.setAutoblinker(AUTOBLINK); roboEyes.open(); roboEyes.setMood(TIRED); animStep = 2; }
            else if(animStep == 2 && t >= 4000){ currentAnim = ANIM_NONE; animStarted = false; }
        break;

        case ANIM_BOOT:
            if(animStep == 0 && t >= 0){ animStarted = true; roboEyes.setIdleMode(OFF); roboEyes.setAutoblinker(OFF); roboEyes.setPosition(DEFAULT); roboEyes.close(); animStep = 1; }
            else if(animStep == 1 && t >= 1500){ roboEyes.open(); roboEyes.setMood(HAPPY); animStep = 2; }
            else if(animStep == 2 && t >= 4000){ roboEyes.blink(0,1); animStep = 3; }
            else if(animStep == 3 && t >= 5500){ roboEyes.setAutoblinker(AUTOBLINK); roboEyes.setIdleMode(IDLE); roboEyes.anim_confused(); roboEyes.setMood(DEFAULT); animStep = 4; }
            else if(animStep == 4 && t >= 6500){ currentAnim = ANIM_NONE; animStarted = false; }
        break;

        case ANIM_SLEEP:
            animStarted = true; roboEyes.setMood(TIRED); roboEyes.close(); currentAnim = ANIM_NONE; animStarted = false;
        break;

        case ANIM_HAPPY:
            animStarted = true; roboEyes.setMood(HAPPY); roboEyes.anim_laugh(); currentAnim = ANIM_NONE; animStarted = false;
        break;
    }
}

//******************************************************************************************************//******************************************************************************************************
// VOID SETUP //******************************************************************************************************//****************************************************************************************
//******************************************************************************************************//******************************************************************************************************
void setup() {
  Serial.begin(9600);
  delay(1000);
  // Serial.println("Serial alive!");
  
//******************************************************************************************************
// INITIALIZE DISPLAY
//******************************************************************************************************
  // Initialize I2C explicitly for ESP32
  Wire.begin(SDA_PIN, SCL_PIN);
  Wire.setClock(400000);

  if (!display.begin(I2C_ADDRESS, true)) {
    Serial.println("Display not found");
    while (true);
  }

  display.clearDisplay();
  display.display();

//******************************************************************************************************
// INITIALIZE ROBOEYES
//******************************************************************************************************
  // Start RoboEyes
  roboEyes.begin(SCREEN_WIDTH, SCREEN_HEIGHT, ROBO_FPS);  // screen-width, screen-height, max framerate - 60-100fps are good for smooth animations

  // Automatic behaviours
  roboEyes.setAutoblinker(AUTOBLINK, AUTOBLINK_INTERVAL, AUTOBLINK_VARIATION);
  roboEyes.setIdleMode(IDLE, IDLE_INTERVAL, IDLE_VARIATION);
  roboEyes.setPosition(DEFAULT); // eye position should be middle center

  //   ### Define Eye Shapes, all values in pixels
  roboEyes.setWidth(E_WIDTH_L, E_WIDTH_R);
  roboEyes.setHeight(E_HEIGHT_L, E_HEIGHT_R);
  roboEyes.setBorderradius(E_BORDERRADIUS_L, E_BORDERRADIUS_R);
  roboEyes.setSpacebetween(E_SPACEBETWEEN);
  roboEyes.setCyclops(E_CYCLOPS);

//******************************************************************************************************
// INITIALIZE TOUCH AND BUZZER INPUTS
//******************************************************************************************************
  pinMode(TOUCH_PIN, INPUT);
  buzzerInit(BUZZER_PIN);

//******************************************************************************************************
// INITIALIZE WIFI
//******************************************************************************************************
  uiInit(&display);  // ✅ pass pointer
  #if DISPLAY_ENABLED
    setDisplay(&display);
  #else
      setDisplay(nullptr);
  #endif // optional - displays on the screen wifi connection

  //******************************************************************************************************
  // INITIALIZE FIRST BOOT ANIMATIONS AND SONGS
  //******************************************************************************************************
  buzzerPlayRTTTL(BOOT_MELODY);
  playAnimation(ANIM_BOOT);
}

//******************************************************************************************************//******************************************************************************************************
// VOID LOOP //******************************************************************************************************//*****************************************************************************************
//******************************************************************************************************//******************************************************************************************************
void loop() {

//******************************************************************************************************
// INITIALIZE FIRST BOOT ANIMATIONS AND SONGS (WE TURN OFF FIRST BOOT FLAG IN ANIMATIONS ANIM_BOOT)
//******************************************************************************************************
  if (firstBoot) {
    
    updateAnimation(); // run animation engine
    roboEyes.update();
    buzzerUpdate();

    if (!buzzerIsPlaying() && animStarted == false) {
      startWifiManager(); // start wifi manager

      while(!isWiFiReady()) {        
        delay(50);
      } // delay, just breathing room

      unsigned long startTime = millis(); // Wait for XX seconds for WiFi to connect before starting MQTT client 
      while (WiFi.status() != WL_CONNECTED) {
        if (millis() - startTime >= 10000) {  // 10 seconds timeout
        break;
        }
        delay(1000);
      }

      mqttSetup(&roboEyes); // start MQTT client
      while (!mqttConnect()) { // wait until connected
          delay(1000);
      }
      haInit(&roboEyes); // send discovery AFTER MQTT is connected

      firstBoot = false; 
    }  // turn off first boot flag when song and animation ends 
    return;  
  }

//******************************************************************************************************
// SCREEN, BUZZER, CUSTOM ANIMATIONS, TOUCH EVENT UPDATES AND HANDLING
//******************************************************************************************************
  // wifiLoop(); // will check the WiFi status every 5 sec and try to reconnect
  // display.display();   // push buffer

  // display.clearDisplay();
  if (!messageActive) {
      roboEyes.update();
  }
  updateAnimation(); // run animation engine
  buzzerUpdate();
  mqttLoop(); // MQTT loop 
  TouchEvent event = touchUpdate();

//******************************************************************************************************
// handle wifi events
//******************************************************************************************************
bool currentWifi = WiFi.isConnected();

if (currentWifi != wifiConnectedState || wifiFirstBoot) {

    wifiFirstBoot = false;
    wifiConnectedState = currentWifi;

    if (WiFi.isConnected()) {
        buzzerPlayRTTTL(WIFI_CONNECT_MELODY);
        playAnimation(ANIM_WIFI_CONNECT);
    }
    else {
        buzzerPlayRTTTL(WIFI_DISCONNECT_MELODY);
        playAnimation(ANIM_WIFI_DISCONNECT);
    }
}

//******************************************************************************************************
// handle touch events
//******************************************************************************************************
switch (event) {

    case TOUCH_SINGLE_TAP:
        Serial.println("Single Tap");
        roboEyes.open();
        buzzerPlayRTTTL(TOUCH_MELODY);
        roboEyes.setMood(HAPPY);

        haPublishTouch("single");
        break;

    case TOUCH_DOUBLE_TAP:
        Serial.println("Double Tap");
        roboEyes.open();
        buzzerPlayRTTTL(MUTE_MELODY);
        roboEyes.setMood(ANGRY);

        haPublishTouch("double");
        break;

    case TOUCH_LONG_PRESS:
        Serial.println("Long Press");
        roboEyes.open();
        buzzerPlayRTTTL(CONFIRM_MELODY);
        roboEyes.setMood(TIRED);

        haPublishTouch("long");
        break;

    default:
        break;
  }
}