#include <NewPing.h>
#define TRIGGER_PIN 4 // Atmega 328 pin 6
#define ECHO_PIN 4 // Atmega 328 pin 6
#define MAX_DISTANCE 200

// SED LED pins
#define LDR A0
#define LED1 9 // Atmega 328 pin 15
#define LED2 6 // Atmega 328 pin 12
#define LED3 5 // Atmega 328 pin 11
#define LED4 3 // Atmega 328 pin 5
#define LED5 10 // Atmega 328 pin 16
// #define LED6 3 // Atmega 328 pin 5
// #define LED7 5 // Atmega 328 pin 11
// #define LED8 6 // Atmega 328 pin 12
// #define LED9 9 // Atmega 328 pin 15

#define LEDR 12 // Atmega 328 pin 18
#define LEDG 11 // Atmega 328 pin 17
#define LEDB 13 // Atmega pin 19

// Settings
// LDR: This will set min and max points of measurement based on the hardware.
#define LDR_min 100 // Set minimum(lowest) value for the LDR reading
#define LDR_max 1000 // Set the maximum (highest) value for the LDR reading

// LEDs: set minimum (visible or low intensity) and maximum intensity(range: 0-255)
#define LED_off 0   // off
#define LED_min 20  // The lowest ON state of the LED
#define LED_max 255 // Set the maximum intensity for the LEDs (max - 255)
#define LED_med 120 // Set medium intensity
#define LED_med_high 200 // Set medium high intensity

// Set position distances
#define position0_min 15 // set the min position of the intial state (before the green led turns ON)
#define position0_max 30 // max
#define position1_min 50
#define position1_max 60
#define position2_min 40
#define position2_max 50
#define position3_min 30
#define position3_max 40
#define position4_min 20
#define position4_max 30
#define position5_min 10
#define position5_max 20
#define position6_min 0
#define position6_max 10
#define green_timer 3000 // set the time to turn the green LED in mS default = 3000ms = 3 seconds
#define off_time 10000 // Set the timer for rutning the leds off
#define led_delay 0
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

boolean movement_detected = false; // This value tells if any movement is detected, if detected it will go to the next operation
boolean green_light_process = false; // This value will tells that we are in the process when the RGB is green
boolean initial_process = false; // For the initial process when red light and LDR are used
boolean off_state = false; // shows off state of the leds
signed int distance = 0;
long unsigned int timer = 0, timer_off=0;
unsigned int pingSpeed = 50;
unsigned long pingTimer;
long green_process_timer=0;

// LED States to control the dimming
byte led1_prev, led1_next, 
led2_prev, led2_next, 
led3_prev, led3_next, 
led4_prev, led4_next, 
led5_prev, led5_next, 
led6_prev, led6_next,
led7_prev, led7_next, 
led8_prev, led8_next, 
led9_prev, led9_next;

int LEDS[] = {LED1,LED2,LED3,LED4,LED5}; //,LED6,LED7,LED8,LED9};
int LEDS_VALS[] = {0,0,0,0,0}; //,0,0,0,0};

int targets[] = {0,0,0,0,0}; //,0,0,0,0};

int keyboardTarget[] = {0,0,0,50,50,50,0,0,0};
int paperTarget[] = {0,0,0,50,50,50,0,0,0};
int detailTarget[] = {0,0,0,50,50,50,0,0,0};
int socialTarget[] = {0,0,0,50,50,50,0,0,0}; 

int state   = 0; // off = 0 idle = 10 booting = 100 on = 1000 customization = 5000
int medium  = 0; // off = 0 keyboard = 1 paper = 2 detail = 3 social = 4
int angle   = 0; // TODO -- get these values

int time_customize_up   = 0;
int time_customize_on   = 0;
int time_customize_down = 0;
int time_boot   = 0;
int time_wake   = 0;
int time_sleep  = 0;

void setup() {
  Serial.begin(9600);
  // while (!Serial) {

  // }

  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);
  pinMode(LED5, OUTPUT);
  // pinMode(LED6, OUTPUT);
  // pinMode(LED7, OUTPUT);
  // pinMode(LED8, OUTPUT);
  // pinMode(LED9, OUTPUT);
  pinMode(LEDR, OUTPUT);
  pinMode(LEDG, OUTPUT);
  pinMode(LEDB, OUTPUT);
  
  digitalWrite(LEDG, LOW); // Turn OFF RGB GREEN
  digitalWrite(LEDR, HIGH); // Turn ON RGB RED

  pingTimer = millis();

  if (state == 0){ setLEDS(LED_min); }
  //setting medium to 1
  medium = 1;
}

void loop(){
  updateLights();
  //displayLights();

  //stateDriver();
  minLights();

  displayLights();

  // analogWrite(LEDS[0], LED_min);  
  //showLED(1);
  // analogWrite(LED1, LED_min);
  // delay(500);
  // analogWrite(LED1, LED_off);
  // delay(500);
}

void stateDriver(){
  switch(state){
    case 0: bootUp(medium); break; // this obviously needs to be activitatied with motions
  }
}

void bootUp(int m){ //medium we are booting to
  setMediums(m);
  state == 100; //booting state
}

void setMediums(int m){ //more mediumds needed
  switch(m){
    case 1:
      memcpy(targets, keyboardTarget, sizeof(targets));
      break;
  }
}

void updateLights(){
  minLights();
}
void displayLights(){
  showLEDS();
}

void maxLights(){ setLEDS(LED_max); }
void medLights(){ setLEDS(LED_med); }
void medHighLights(){ setLEDS(LED_med_high); }
void minLights(){ setLEDS(LED_min); }
void offLights(){ setLEDS(LED_off); }

void showLEDS(){
  for(int i = 0; i < 5; i++){
    showLED(i);
  }
}
void showLED(int i){
  analogWrite(LEDS[i], LEDS_VALS[i]);
}
void setLEDS(int v){
  for(int i = 0; i < 5; i++){
   setLED(i,v);
  }
}
void setLED(int i, int v){
  LEDS_VALS[i] = v;
}
