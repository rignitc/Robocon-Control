#include <Arduino.h>
#include <ros.h>
#include <std_msgs/Int32.h>
#include <Stepper.h>

#define st1_pi1 4
#define st1_pi2 5
#define st1_pi3 6
#define st1_pi4 7
#define st2_pi1 11
#define st2_pi2 12
#define st2_pi3 13
#define st2_pi4 14


ros::NodeHandle nh;

int stepsPerRev = 200;
int rpm = 330;
int dir=0;
int dir2=0;




Stepper hori(stepsPerRev,st1_pi1,st1_pi2,st1_pi3,st1_pi4);
Stepper vert(stepsPerRev,st2_pi1,st2_pi2,st2_pi3,st2_pi4);



void VStepperCB(const std_msgs::Int32& vtrigger){
    dir2=vtrigger.data;
   }

void HStepperCB(const std_msgs::Int32& htrigger){
    dir=htrigger.data;
}

ros::Subscriber<std_msgs::Int32> verti("screw_power", &VStepperCB);
ros::Subscriber<std_msgs::Int32> horiz("belt_power", &HStepperCB);

void setup()
{

    hori.setSpeed(rpm);
    nh.initNode();
   nh.subscribe(verti);
    nh.subscribe(horiz);
    pinMode(st1_pi1,OUTPUT);
    pinMode(st1_pi2,OUTPUT);
    pinMode(st1_pi3,OUTPUT);
    pinMode(st1_pi4,OUTPUT);
    pinMode(st2_pi1,OUTPUT);
    pinMode(st2_pi2,OUTPUT);
    pinMode(st2_pi3,OUTPUT);
    pinMode(st2_pi4,OUTPUT);

}
void loop()
{     
    nh.spinOnce();
    if (dir!=0)
    {
    hori.step(dir);  
    }
    else
    {
      digitalWrite(st1_pi1,LOW);
      digitalWrite(st1_pi2,LOW);
      digitalWrite(st1_pi3,LOW);
      digitalWrite(st1_pi4,LOW);
      
    }
    if (dir2!=0)
    {
    vert.step((dir2));  
    }
    else
    {
      digitalWrite(st2_pi1,LOW);
      digitalWrite(st2_pi2,LOW);
      digitalWrite(st2_pi3,LOW);
      digitalWrite(st2_pi4,LOW);
    }
    
    delayMicroseconds(100); 
}
