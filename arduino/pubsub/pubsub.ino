/** Throwing pwm subscriber **/
#include <ros.h>
#include <std_msgs/Int16.h>

ros::NodeHandle  nh;

int motor_pwm = 0;

int pwm_l = 2;
int pwm_r = 4;
int dir_l = 3;
int dir_r = 5;

void messageCb( const std_msgs::Int16& pwm){
  motor_pwm = pwm.data;   
}

ros::Subscriber<std_msgs::Int16> sub("pwm", messageCb );

void setup()
{
  pinMode(pwm_l, OUTPUT);
  pinMode(pwm_r, OUTPUT);
  pinMode(dir_l, OUTPUT);
  pinMode(dir_r, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  nh.initNode();
  nh.subscribe(sub);
}

void loop()
{
  digitalWrite(dir_l, LOW);
  digitalWrite(dir_r, LOW);
  analogWrite(pwm_l, motor_pwm);
  analogWrite(pwm_r, motor_pwm);
  if(motor_pwm < 45){
    digitalWrite(LED_BUILTIN, HIGH);
  }
  else {
    digitalWrite(LED_BUILTIN, LOW);
  }
  nh.spinOnce();
  delay(20);
}