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
#define LED6 3 // Atmega 328 pin 5
#define LED7 5 // Atmega 328 pin 11
#define LED8 6 // Atmega 328 pin 12
#define LED9 9 // Atmega 328 pin 15
#define LEDR 12 // Atmega 328 pin 18
#define LEDG 11 // Atmega 328 pin 17
#define LEDB 13 // Atmega pin 19

// Settings
// LDR: This will set min and max points of measurement based on the hardware.
#define LDR_min 100 // Set minimum(lowest) value for the LDR reading
#define LDR_max 1000 // Set the maximum (highest) value for the LDR reading

// LEDs: set minimum (visible or low intensity) and maximum intensity(range: 0-255)
#define LED_min 20 // The lowest ON state of the LED
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

void setup() {
  Serial.begin(9600);

  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);
  pinMode(LED5, OUTPUT);
  pinMode(LED6, OUTPUT);
  pinMode(LED7, OUTPUT);
  pinMode(LED8, OUTPUT);
  pinMode(LED9, OUTPUT);
  pinMode(LEDR, OUTPUT);
  pinMode(LEDG, OUTPUT);
  pinMode(LEDB, OUTPUT);
digitalWrite(LEDG, LOW); // Turn OFF RGB GREEN
digitalWrite(LEDR, HIGH); // Turn ON RGB RED
pingTimer = millis();
}
void loop(){
echoCheck(); // Send out the ping, calls "echoCheck" function every 24uS where you can check the ping status.


if(distance>0){ // If we measure something with the sonar sensor it means there is a movement and we could set the LEDs based on the LDR reading
movement_detected = true; // set the movement detection to true. Movement was detected
timer_off = millis();
off_state = false;
}
if( ((distance<=position0_min) || (distance > position0_max) ) ){ // If the distance is out of the range we need, restart the timer
  timer = millis();
  digitalWrite(LEDB, LOW);
}
else {
  digitalWrite(LEDB, HIGH);
}
if ( (millis()-timer) > green_timer){ // If enough time passed, turn the green LED
  green_light_process=true;
  initial_process=true;
  digitalWrite(LEDR, LOW);
  digitalWrite(LEDG, HIGH);
  Serial.println("Green");
}
if (millis()-timer_off>off_time){
  leds_off();
  off_state = true;
  initial_process = false;
}
else {
  off_state = false;
}
if (movement_detected == true && initial_process == false && off_state==false){ // if there was a movement after the device is ON, set the LEDs based on the LDR reading
 set_led_ldr(LDR_measure()); // set_led_ldr sets LEDs (2-8) on. LDR_measure() returns the intensity for the LEDs
}

while (green_light_process){ // While we are in green light do this
  echoCheck();
  if (distance<=0 || distance>position1_max){
    if (millis()-green_process_timer>1500){
      digitalWrite(LEDG, LOW);
      digitalWrite(LEDR, HIGH);
      green_light_process = false;
      timer_off = millis();
      timer = millis();
      Serial.println("Red");
    }
  }
  else if ( distance>position6_min && distance<=position6_max ) {
    green_process_timer = millis();
    set_position6();
  }
  else if(distance > position5_min && distance <=position5_max){
    green_process_timer = millis();
    set_position5();
  }
  else if(distance > position4_min && distance <=position4_max){
    green_process_timer = millis();
    set_position4();
  }
  else if(distance > position3_min && distance <=position3_max){
    green_process_timer = millis();
    set_position3();
  }
  else if(distance > position2_min && distance <=position2_max){
    green_process_timer = millis();
    set_position2();
  }
  else if(distance > position1_min && distance <=position1_max){
    green_process_timer = millis();
    set_position1();
  }

}
}
int LDR_measure(){ // Measure the LDR and return light intensity for the LEDs
  int measurement = analogRead(LDR);
  measurement = map(measurement,LDR_min, LDR_max,LED_min, LED_max);
  return measurement;
}
void set_led_ldr(int intensity){
  analogWrite(LED2, intensity);
  analogWrite(LED3, intensity);
  analogWrite(LED4, intensity);
  analogWrite(LED5, intensity);
  analogWrite(LED6, intensity);
  analogWrite(LED7, intensity);
  analogWrite(LED8, intensity);

}
void set_position1(){
// Position 1 LEDs and their intensity as defined on top of the sketch
  led1_next = 0;
  led2_next = 0;
  led3_next = 0;
  led4_next = 0;
  led5_next = LED_max;
  led6_next = 0;
  led7_next = 0;
  led8_next = 0;
  led9_next = 0;
  LED_dim();
}
void set_position2(){ //commented out by Luco
//  led1_next = LED_0;
//  led2_next = LED_0;
//  led3_next = LED_0;
//  led4_next = LED_med;
//  led5_next = LED_med_high;
//  led6_next = LED_med;
//  led7_next = LED_0;
//  led8_next = LED_0;
//  led9_next = 0;
//  LED_dim();
}
void set_position3(){
  led1_next = 0;
  led2_next = 0;
  led3_next = 0;
  led4_next = LED_min;
  led5_next = LED_med;
  led6_next = LED_min;
  led7_next = 0;
  led8_next = 0;
  led9_next = 0;
  LED_dim();
}
void set_position4(){
  led1_next = 0;
  led2_next = 0;
  led3_next = LED_min;
  led4_next = LED_med;
  led5_next = LED_med;
  led6_next = LED_med;
  led7_next = LED_min;
  led8_next = 0;
  led9_next = 0;
  LED_dim();
}
void set_position5(){
  led1_next = 0;
  led2_next = LED_min;
  led3_next = LED_min;
  led4_next = LED_min;
  led5_next = LED_min;
  led6_next = LED_min;
  led7_next = LED_min;
  led8_next = LED_min;
  led9_next = 0;
  LED_dim();
}
void set_position6(){
  led1_next = LED_min;
  led2_next = LED_min;
  led3_next = LED_min;
  led4_next = LED_min;
  led5_next = LED_min;
  led6_next = LED_min;
  led7_next = LED_min;
  led8_next = LED_min;
  led9_next = LED_min;
  LED_dim();
}
void leds_off(){
  analogWrite(LED1, 0);
  analogWrite(LED2, 0);
  analogWrite(LED3, 0);
  analogWrite(LED4, 0);
  analogWrite(LED5, 0);
  analogWrite(LED6, 0);
  analogWrite(LED7, 0);
  analogWrite(LED8, 0);
  analogWrite(LED9, 0);
}
void echoCheck() {
  delay(15);
  distance = sonar.ping_cm();
// Tester();

  if (distance>61){
    distance=0;
  }
// distance = (sonar.ping_result / US_ROUNDTRIP_CM); // Ping returned, uS result in ping_result, convert to cm with US_ROUNDTRIP_CM.
}
void LED_dim(){ //this section was developed with the external help
  if(led1_next>led1_prev && abs(led1_prev - led1_next)>2){
    led1_prev+=2;
    analogWrite(LED1, led1_prev);
    delay(led_delay);
  }
  if(led2_next>led2_prev && abs(led2_prev - led2_next)>2){
    led2_prev+=2;
    analogWrite(LED2, led2_prev);
    delay(led_delay);
  }
  if(led3_next>led3_prev && abs(led3_prev - led3_next)>2){
    led3_prev+=2;
    analogWrite(LED3, led3_prev);
    delay(led_delay);
  }
  if(led4_next>led4_prev && abs(led4_prev - led4_next)>2){
    led4_prev+=2;
    analogWrite(LED4, led4_prev);
    delay(led_delay);
  }
  if(led5_next>led5_prev && abs(led5_prev - led5_next)>2){
    led5_prev+=2;
    analogWrite(LED5, led5_prev);
    delay(led_delay);
  }
  if(led6_next>led6_prev && abs(led6_prev - led6_next)>2){
    led6_prev+=2;
    analogWrite(LED6, led6_prev);
    delay(led_delay);
  }
  if(led7_next>led7_prev && abs(led7_prev - led7_next)>2){
    led7_prev+=2;
    analogWrite(LED7, led7_prev);
    delay(led_delay);
  }
  if(led8_next>led8_prev && abs(led8_prev - led8_next)>2){
    led8_prev+=2;
    analogWrite(LED8, led8_prev);
    delay(led_delay);
  }
  if(led9_next>led9_prev && abs(led9_prev - led9_next)>2){
    led9_prev+=2;
    analogWrite(LED9, led9_prev);
    delay(led_delay);
  }
////////////////////
  if(led1_next<led1_prev && abs(led1_prev - led1_next)>2){
    led1_prev-=2;
    analogWrite(LED1, led1_prev);
    delay(led_delay);
  }
  if(led2_next<led2_prev && abs(led2_prev - led2_next)>2){
    led2_prev-=2;
    analogWrite(LED2, led2_prev);
    delay(led_delay);
  }
  if(led3_next<led3_prev && abs(led3_prev - led3_next)>2){
    led3_prev-=2;
    analogWrite(LED3, led3_prev);
    delay(led_delay);
  }
  if(led4_next<led4_prev && abs(led4_prev - led4_next)>2){
    led4_prev-=2;
    analogWrite(LED4, led4_prev);
    delay(led_delay);
  }
  if(led5_next<led5_prev && abs(led5_prev - led5_next)>2){
    led5_prev-=2;
    analogWrite(LED5, led5_prev);
    delay(led_delay);
  }
  if(led6_next<led6_prev && abs(led6_prev - led6_next)>2){
    analogWrite(LED6, led6_prev);
    led6_prev-=2;
    delay(led_delay);
  }
  if(led7_next<led7_prev && abs(led7_prev - led7_next)>2){
    led7_prev-=2;
    analogWrite(LED7, led7_prev);
    delay(led_delay);
  }
  if(led8_next<led8_prev && abs(led8_prev - led8_next)>2){
    led8_prev--;
    analogWrite(LED8, led8_prev);
    delay(led_delay);
  }
  if(led9_next<led9_prev && abs(led9_prev - led9_next)>2){
    led9_prev-=2;
    analogWrite(LED9, led9_prev);
    delay(led_delay);
  }
  if(led1_next==0 && led1_prev<=3 ){
    analogWrite(LED1, 0);
  }
  if(led2_next==0 && led2_prev<=3 ){
    analogWrite(LED2, 0);
  }
  if(led3_next==0 && led3_prev<=3 ){
    analogWrite(LED3, 0);
  }
  if(led4_next==0 && led4_prev<=3 ){
    analogWrite(LED4, 0);
  }
  if(led5_next==0 && led5_prev<=3 ){
    analogWrite(LED5, 0);
  }
  if(led6_next==0 && led6_prev<=3 ){
    analogWrite(LED6, 0);
  }
  if(led7_next==0 && led7_prev<=3 ){
    analogWrite(LED7, 0);
  }
  if(led8_next==0 && led8_prev<=3 ){
    analogWrite(LED8, 0);
  }
  if(led9_next==0 && led9_prev<=3 ){
    analogWrite(LED9, 0);
  }


}
void Tester (){
  char reading;
  if(Serial.available()>0){
    reading = Serial.read();
    if(reading == '0')
      distance = 0;
    else if (reading == '1')
      distance = 16;
    else if (reading == '4')
      distance = 40;
    else if (reading =='5')
      distance = 50;
    else if(reading == '2')
      distance = 25;
    Serial.println(distance);
  }
}
