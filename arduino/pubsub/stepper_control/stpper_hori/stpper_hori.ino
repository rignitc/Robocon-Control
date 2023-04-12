//#include "DRV8825.h"
#include <Arduino.h>
#include <ros.h>
#include <std_msgs/Int32.h>

#define HOR 1
#define VER 2

#define CLOCKWISE 1
#define ANTI_CLOCKWISE 2

#define DELAY 1000
#define STEPS 60

ros::NodeHandle nh;
//
//DRV8825 hstepper;
//DRV8825 vstepper;

const int hDIRECTION_PIN =5;
const int hSTEP_PIN = 2;

const int vDIRECTION_PIN =6;
const int vSTEP_PIN = 3;

void step_(int type, int dir){
  if(type == HOR){
    if(dir == CLOCKWISE){
      digitalWrite(hDIRECTION_PIN, HIGH);
    }
    else{
      digitalWrite(hDIRECTION_PIN, LOW);
    }
    for(int i=0;i < STEPS;i++){
      digitalWrite(hSTEP_PIN, HIGH);
      delayMicroseconds(DELAY);
      digitalWrite(hSTEP_PIN, LOW);
      delayMicroseconds(DELAY);
    }
  }
  else{
    if(dir == CLOCKWISE) {
      digitalWrite(vDIRECTION_PIN, HIGH);
    }
    else{
      digitalWrite(vDIRECTION_PIN, LOW);
    }
    for(int i=0;i < STEPS;i++){
      digitalWrite(vSTEP_PIN, HIGH);
      delayMicroseconds(DELAY);
      digitalWrite(vSTEP_PIN, LOW);
      delayMicroseconds(DELAY);
    }
  }
}

void VStepperCB(const std_msgs::Int32& vtrigger){
    if(vtrigger.data ==1){
        step_(VER, ANTI_CLOCKWISE);
    }
    else if(vtrigger.data ==-1){
        step_(VER, CLOCKWISE);
    }
}

void HStepperCB(const std_msgs::Int32& htrigger){
    if(htrigger.data ==1){
        step_(HOR, ANTI_CLOCKWISE);
    }
    else if(htrigger.data ==-1){
        step_(HOR, CLOCKWISE);
    }
}

ros::Subscriber<std_msgs::Int32> vert("screw_power", &VStepperCB);
ros::Subscriber<std_msgs::Int32> hori("belt_power", &HStepperCB);

void setup()
{
    pinMode(hDIRECTION_PIN, OUTPUT);
    pinMode(vDIRECTION_PIN, OUTPUT);

    pinMode(hSTEP_PIN, OUTPUT);
    pinMode(vSTEP_PIN, OUTPUT);
    pinMode(8,OUTPUT);    //to make enable zero. Its connected to 8 in sheild
    digitalWrite(8,LOW);
    nh.initNode();
    nh.subscribe(vert);
    nh.subscribe(hori);
}
void loop()
{
    nh.spinOnce();
    delay(10); 
}
