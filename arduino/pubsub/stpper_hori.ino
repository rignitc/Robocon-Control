#include "DRV8825.h"
#include <Arduino.h>
#include <ros.h>
#include <std_msgs/Int32.h>

ros::NodeHandle nh;

DRV8825 hstepper;
DRV8825 vstepper;

const int hDIRECTION_PIN =5;
const int hSTEP_PIN = 2;

const int vDIRECTION_PIN =6;
const int vSTEP_PIN = 3;


void VStepperCB(const std_msgs::Int32& vtrigger){
    if(vtrigger.data ==1){
        vstepper.setDirection(DRV8825_COUNTERCLOCK_WISE);
        for(int i=0;i<30;i++)
            {
            vstepper.step();
            delay(5);
            }

    }
    else if(vtrigger.data ==-1){
        vstepper.setDirection(DRV8825_CLOCK_WISE);
        for(int i=0;i<30;i++)
            {
            vstepper.step();
            delay(5);
            }

    }
}

void HStepperCB(const std_msgs::Int32& htrigger){
    if(htrigger.data ==1){
            hstepper.setDirection(DRV8825_COUNTERCLOCK_WISE);
            for(int i=0;i<100;i++)
            {
            hstepper.step();
            delay(5);
            }

    }
    else if(htrigger.data ==-1){
            hstepper.setDirection(DRV8825_CLOCK_WISE);
            for(int i=0;i<100;i++)
            {
            hstepper.step();
            delay(5);
            }
    }
}

ros::Subscriber<std_msgs::Int32> vert("screw_power", &VStepperCB);
ros::Subscriber<std_msgs::Int32> hori("belt_power", &HStepperCB);

void setup()
{
    hstepper.begin(hDIRECTION_PIN, hSTEP_PIN);
    vstepper.begin(vDIRECTION_PIN, vSTEP_PIN);
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
