
//Inculdes
#include <avr/sleep.h>
//arduino std
#include <SPI.h>
#include <Servo.h>
//arduino libs
#include <TimerThree.h>

//low power labs
//https://github.com/LowPowerLab

#include <SPIFlash.h>

#include <RFM69.h>
#include <RFM69registers.h>
#include <RFM69_ATC.h>
#include <RFM69_OTA.h>

//system config

#define PWM_PIN 10
#define VS_APIN 5
#define VP_APIN 3

#define PEROID 100000

//#define DEBUG_VALVE
//#define DEBUG_VERBOSE
#define SER_BAUD 115200

#define DFT_ANGLE 0

#define RF_TIMEOUT 50


//statics
Servo servo;
//number of poling loop hits with valve on target
byte onTargetHits = 0;
byte target = DFT_ANGLE;
byte angle;
byte bateryI,powerlv;
byte rfTimeout;

bool messageRES = false;

//warning:functionally not guaranteed
#ifdef DEBUG_VALVE
bool newData = true;
#endif

void setup() {

  servo.attach(PWM_PIN);
  servo.write(target);

  #ifdef DEBUG_VALVE
  Serial.begin(SER_BAUD);
  while(!Serial);
  Serial.println("Starting with DEBUG build");
  #endif

  angle = constrain(getCurrentAngle(), 0, 180);
  getBatteryChargeWD(bateryI);

  ini_rf(); 

  Timer3.initialize(PEROID);
  Timer3.attachInterrupt(timer3_IRS);

  set_sleep_mode(SLEEP_MODE_IDLE);
  sleep_enable();
}

void loop() 
{
  sleep_mode();

  if(radioPoleUpdate()){
    messageRES = true;
  }

  #ifdef DEBUG_VALVE
  if(newData){
    newData = false;

    #ifdef DEBUG_VERBOSE
    Serial.print("Angle: ");
    Serial.println(angle);
    Serial.print("Target: ");
    Serial.println(target);
    Serial.print("Battery: ");
    Serial.println(powerlv);
    #endif
  }
  #endif

  
}

void timer3_IRS()
{
  if(messageRES){
    messageRES = false;
    rfTimeout = 0;
  }else{
    if(rfTimeout > RF_TIMEOUT){
      target = DFT_ANGLE;
    }else{
      ++rfTimeout;
    }
  }
  
  powerlv = getBatteryCharge(bateryI);
  angle = constrain(getCurrentAngle(), 0, 180);
  updateValve();
  
  #ifdef DEBUG_VALVE
  newData = true;
  #endif
}

