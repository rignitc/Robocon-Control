#include <Arduino.h>
#include <ros.h>
#include <std_msgs/Int32.h>
#define Enable 8

ros::NodeHandle nh;

const int hDIRECTION_PIN = 5;
const int hSTEP_PIN = 2;
const int vDIRECTION_PIN = 6;
const int vSTEP_PIN = 3;
const int vlim_top = 12;
const int vlim_bot = 11;
const int stepsPerRevolution = 200;

int step_1 = 0;
int step_2 = 0;

void VStepperCB(const std_msgs::Int32 &vtrigger)
{
  if (vtrigger.data != 0)
  {
    step_1 = 1;
    if (vtrigger.data == 1)
    {
      if (digitalRead(vlim_top))
      {
      digitalWrite(vDIRECTION_PIN, HIGH);
//      digitalWrite(vDIRECTION_PIN2, HIGH);
      }
      else
      {
          step_1=0;
      }
    }
    else if (vtrigger.data == -1)
    {
      if (digitalRead(vlim_bot))
      {
      digitalWrite(vDIRECTION_PIN, LOW);
//      digitalWrite(vDIRECTION_PIN2, LOW);
      }
      else
      {
//        digitalWrite(vDIRECTION_PIN, HIGH);
          step_1=0;
//        digitalWrite(vDIRECTION_PIN2, HIGH);
      }
    }
  }
  else
  {
    step_1 = 0;
  }
}

void HStepperCB(const std_msgs::Int32 &htrigger)
{
  if (htrigger.data != 0)
  {
    step_2 = 1;
    if (htrigger.data == 1)
    {
      digitalWrite(hDIRECTION_PIN, HIGH);
    }
    else
    {
      digitalWrite(hDIRECTION_PIN, LOW);
    }
  }
  else
  {
    step_2 = 0;
  }
}

ros::Subscriber<std_msgs::Int32> vert("screw_power", &VStepperCB);
ros::Subscriber<std_msgs::Int32> hori("belt_power", &HStepperCB);

void setup()
{
  pinMode(Enable, OUTPUT); // to make enable zero. Its connected to 8 in sheild
  digitalWrite(Enable, HIGH);
  pinMode(hDIRECTION_PIN, OUTPUT);
//  pinMode(vDIRECTION_PIN2, OUTPUT);
  pinMode(vDIRECTION_PIN, OUTPUT);
  pinMode(hSTEP_PIN, OUTPUT);
//  pinMode(vSTEP_PIN2, OUTPUT);
  pinMode(vSTEP_PIN, OUTPUT);
  pinMode(vlim_top, INPUT_PULLUP);
  pinMode(vlim_bot, INPUT_PULLUP);
  nh.initNode();
  nh.subscribe(vert);
  nh.subscribe(hori);
}
void loop()
{
  nh.spinOnce();
  if (step_2 == 1)
  {
     digitalWrite(Enable, LOW);
    for (int x = 0; x < stepsPerRevolution; x++)
    {
      digitalWrite(hSTEP_PIN, HIGH);
      delayMicroseconds(600);
      digitalWrite(hSTEP_PIN, LOW);
      delayMicroseconds(600);
    }
  }
  if (step_1 == 1)
  {
    digitalWrite(Enable, LOW);
    for (int x = 0; x < stepsPerRevolution; x++)
    {
      digitalWrite(vSTEP_PIN, HIGH);
//      digitalWrite(vSTEP_PIN2, HIGH);
      delayMicroseconds(600);
      digitalWrite(vSTEP_PIN, LOW);
//      digitalWrite(vSTEP_PIN2, LOW);
      delayMicroseconds(600);
    }
  }
  else if (step_2 == 0)
  {
    digitalWrite(Enable, HIGH);
  }
}
