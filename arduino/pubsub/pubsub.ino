/** Throwing pwm subscriber **/
#include <ros.h>
#include <std_msgs/Int16.h>
#include <std_msgs/Bool.h>
#include <Servo.h>

Servo myservo;

ros::NodeHandle  nh;



int pwm_l = 2;
int pwm_r = 4;
int dir_l = 3;
int dir_r = 5;

int pwm_index = 0;
int pwm_array[] = {0, 160, 200, 255};
int motor_pwm = pwm_array[pwm_index];


//void messageCb( const std_msgs::Int16& pwm){
//  motor_pwm = pwm.data;   
//}

void pwmCb(const std_msgs::Bool& toggle) {
  if(toggle.data){
    pwm_index = (pwm_index + 1) % 4;
    motor_pwm = pwm_array[pwm_index];
  }
}

void servoCb( const std_msgs::Int16& theta) {
  myservo.write(theta.data);
}

ros::Subscriber<std_msgs::Int16> sub("pwm", messageCb );
ros::Subscriber<std_msgs::Int16> servo_sub("servo", servoCb);
void setup()
{
  myservo.attach(9);
  pinMode(pwm_l, OUTPUT);
  pinMode(pwm_r, OUTPUT);
  pinMode(dir_l, OUTPUT);
  pinMode(dir_r, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  nh.initNode();
  nh.subscribe(sub);
  nh.subscribe(servo_sub);
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
