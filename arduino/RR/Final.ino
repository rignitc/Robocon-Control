// #define ROSSERIAL_ARDUINO_TCP
#include <ros.h>
#include <geometry_msgs/Quaternion.h>
#include <std_msgs/Int32.h>
#include <std_msgs/Int16.h>
#include <std_msgs/Bool.h>
#include "CytronMotorDriver.h"
#include "DRV8825.h"

// #define yaw_pwm 6
// #define yaw_dir 6
#define pitch_pwm 7
#define pitch_dir 37
#define motor_pwm_l_pin 6
// #define motor_pwm_r_pin 11
// #define motor_dir_r_pin 22
#define motor_dir_l_pin 36
#define wheel1_pwm 13
#define wheel2_pwm 3
#define wheel3_pwm 4
#define wheel4_pwm 5
#define wheel1_dir 32
#define wheel2_dir 33
#define wheel3_dir 34
#define wheel4_dir 35
#define Enable 12
#define hDIRECTION_PIN 10
#define hSTEP_PIN 11
#define vDIRECTION_PIN 8
#define vSTEP_PIN 9

ros::NodeHandle nh;
CytronMD actuator(PWM_DIR, pitch_pwm, pitch_dir);
CytronMD wheel1(PWM_DIR, wheel1_pwm, wheel1_dir);
CytronMD wheel2(PWM_DIR, wheel2_pwm, wheel2_dir);
CytronMD wheel3(PWM_DIR, wheel3_pwm, wheel3_dir);
CytronMD wheel4(PWM_DIR, wheel4_pwm, wheel4_dir);
DRV8825 hstepper;
DRV8825 vstepper;


// CytronMD motor_l(PWM_DIR,motor_pwm_l,motor)

int PWM1;
int PWM2;
int PWM3;
int PWM4;
int pwm_index = 0;
int pwm_array[] = {0, 160, 200, 255};
float multiplier_array[] = {0.25, 1};
int multiplier_index = 0;

int motor_pwm = pwm_array[pwm_index];
int act_pwm;
float multiplier = 1;
int prev_speed_time = 0;
int prev_locoSpeed_time = 0;

void VStepperCB(const std_msgs::Int32 &vtrigger)
{
  if (vtrigger.data == 1)
  {
    vstepper.setDirection(DRV8825_COUNTERCLOCK_WISE);
    for (int i = 0; i < 100; i++)
    {
      vstepper.step();
      delay(1);
    }
  }
  else if (vtrigger.data == -1)
  {
    vstepper.setDirection(DRV8825_CLOCK_WISE);
    for (int i = 0; i < 100; i++)
    {
      vstepper.step();
      delay(1);
    }
  }
}

void HStepperCB(const std_msgs::Int32 &htrigger)
{
  if (htrigger.data == 1)
  {
    hstepper.setDirection(DRV8825_COUNTERCLOCK_WISE);
    for (int i = 0; i < 100; i++)
    {
      hstepper.step();
      delay(2);
    }
  }
  else if (htrigger.data == -1)
  {
    hstepper.setDirection(DRV8825_CLOCK_WISE);
    for (int i = 0; i < 100; i++)
    {
      hstepper.step();
      delay(2);
    }
  }
}
void callback1(const geometry_msgs::Quaternion &msg)
{

  PWM1 = -1 * msg.x;
  PWM2 = 1 * msg.y;
  PWM3 = 1 * msg.z;
  PWM4 = -1 * msg.w;
  // Serial.println(PWM1);

  // receives both +ve and negative values acc to motion needed )
}

// void callback_yaw(const std_msgs::Int32 &msg)
// {
//   if (msg.data<0)
//   {
//     digitalWrite(yaw_dir,HIGH);
//     analogWrite(yaw_pwm,msg.data);
//   }
//   else{
//     digitalWrite(yaw_dir,LOW);
//     analogWrite(yaw_pwm,msg.data);
//   }

// }

void callback_pitch(const std_msgs::Int32 &msg)
{
  act_pwm = msg.data;
}

void callback_speed(const std_msgs::Bool &msg)
{
  if (((millis() - prev_speed_time) > 1000) && (msg.data))
  {
    prev_speed_time = millis();
    pwm_index = (pwm_index + 1) % 4;
    motor_pwm = pwm_array[pwm_index];
  }
}

void locoSpeed(const std_msgs::Bool &msg)
{
  if (((millis() - prev_locoSpeed_time) > 1000) && (msg.data))
  {
    prev_locoSpeed_time = millis();
    multiplier_index = (multiplier_index + 1) % 2;
    multiplier = multiplier_array[multiplier_index];
  }
}

// Subscriber
ros::Subscriber<geometry_msgs::Quaternion> sub1("keyboard_message1", &callback1);
ros::Subscriber<std_msgs::Int32> sub_pitch("target_pitch", &callback_pitch);
// ros::Subscriber<geometry_msgs::Int32> sub_yaw("target_yaw", &callback_yaw);
ros::Subscriber<std_msgs::Bool> sub_speed("speed", &callback_speed);
ros::Subscriber<std_msgs::Bool> sub_locoSpeed("locoSpeed", &locoSpeed);
ros::Subscriber<std_msgs::Int32> vert("screw_power", &VStepperCB);
ros::Subscriber<std_msgs::Int32> hori("belt_power", &HStepperCB);

void setup()
{
  hstepper.begin(hDIRECTION_PIN, hSTEP_PIN);
  vstepper.begin(vDIRECTION_PIN, vSTEP_PIN);
  pinMode(Enable, OUTPUT); // to make enable zero. Its connected to 8 in sheild
  digitalWrite(Enable, LOW);
  // NODE
  nh.initNode();
  nh.subscribe(sub1);
  nh.subscribe(sub_pitch);
  nh.subscribe(sub_speed);
  nh.subscribe(sub_locoSpeed);
  nh.subscribe(vert);
  nh.subscribe(hori);
//  nh.subscribe(sub_yaw);

Serial.begin(57600);
Serial.println("node is initialised");

// pinMode(5,INPUT_PULLUP); //Encoder pin
// pinMode(4,INPUT); // Encoder Pin

pinMode(wheel1_dir, OUTPUT);
pinMode(wheel1_pwm, OUTPUT);

pinMode(wheel2_dir, OUTPUT);
pinMode(wheel2_pwm, OUTPUT);

pinMode(wheel3_dir, OUTPUT);
pinMode(wheel3_pwm, OUTPUT);

pinMode(wheel4_dir, OUTPUT);
pinMode(wheel4_pwm, OUTPUT);

//  pinMode(yaw_dir,OUTPUT);
//  pinMode(yaw_pwm,OUTPUT);
// pinMode(pitch_dir,OUTPUT);
// pinMode(pitch_pwm,OUTPUT);

pinMode(motor_pwm_l_pin, OUTPUT);
pinMode(motor_dir_l_pin, OUTPUT);

// flick_servo.attach(flick_servo_pin);

digitalWrite(motor_dir_l_pin, HIGH);
// digitalWrite(motor_dir_r_pin,LOW);

// put your setup code here, to run once:
}

void loop()
{
  wheel1.setSpeed(PWM1 * multiplier);
  wheel2.setSpeed(PWM2 * multiplier);
  wheel3.setSpeed(PWM3 * multiplier);
  wheel4.setSpeed(PWM4 * multiplier);

  analogWrite(motor_pwm_l_pin, motor_pwm);
  //  analogWrite(motor_pwm_r_pin,motor_pwm);

  analogWrite(2, abs(PWM4));
  actuator.setSpeed(act_pwm);

  //
  nh.spinOnce();
  delay(1);

  // put your main code here, to run repeatedly:
}
