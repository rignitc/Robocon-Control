//#define ROSSERIAL_ARDUINO_TCP
#include <ros.h>
#include <geometry_msgs/Quaternion.h>
#include <std_msgs/Int32.h>
#include <std_msgs/Bool.h>

// #define yaw_pwm 6
// #define yaw_dir 6
#define pitch_pwm 12
#define pitch_dir 13
#define motor_pwm_l_pin 1
#define motor_pwm_r_pin 2
#define motor_dir_r_pin 3
#define motor_dir_l_pin 4







ros::NodeHandle nh;

int PWM1;
int PWM2;
int PWM3;
int PWM4;
int pwm_index = 0;
int pwm_array[] = {0, 160, 200, 255};
int motor_pwm = pwm_array[pwm_index];

//float dir;

//Callback START ---------------



void callback1(const geometry_msgs::Quaternion &msg){
 
PWM1 = -1*msg.x;
PWM2 = 1*msg.y;
PWM3 = 1*msg.z;
PWM4 = -1*msg.w;
Serial.println(PWM1);

//receives both +ve and negative values acc to motion needed )

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
    if (msg.data<0)
  {
    digitalWrite(pitch_dir,HIGH);
    analogWrite(pitch_pwm,-msg.data);
  }
  else{
    digitalWrite(pitch_dir,LOW);
    analogWrite(pitch_pwm,msg.data);
  }

}

void callback_speed(const std_msgs::Bool &msg){
  if(msg.data){
    pwm_index = (pwm_index + 1) % 4;
    motor_pwm = pwm_array[pwm_index];
  }
}

//Subscriber
ros::Subscriber<geometry_msgs::Quaternion> sub1("keyboard_message1", &callback1);
ros::Subscriber<std_msgs::Int32> sub_pitch("target_pitch", &callback_pitch);
// ros::Subscriber<geometry_msgs::Int32> sub_yaw("target_yaw", &callback_yaw);
ros::Subscriber<std_msgs::Bool> sub_speed("speed", &callback_speed);



void setup() {

 //NODE 
 nh.initNode();
 nh.subscribe(sub1);
 nh.subscribe(sub_pitch);
 nh.subscribe(sub_speed);

//  nh.subscribe(sub_yaw);

 

Serial.begin(57600);
 Serial.println("node is initialised");
 
 
 
// pinMode(5,INPUT_PULLUP); //Encoder pin
// pinMode(4,INPUT); // Encoder Pin

 
 pinMode(8,OUTPUT);
 pinMode(9,OUTPUT);

 pinMode(6,OUTPUT);
 pinMode(7,OUTPUT);
 
 pinMode(4,OUTPUT);
 pinMode(5,OUTPUT);

 pinMode(2,OUTPUT);
 pinMode(3,OUTPUT);

//  pinMode(yaw_dir,OUTPUT);
//  pinMode(yaw_pwm,OUTPUT);
 pinMode(pitch_dir,OUTPUT);
 pinMode(pitch_pwm,OUTPUT);

 pinMode(motor_pwm_l_pin,OUTPUT);
 pinMode(motor_pwm_r_pin,OUTPUT);
 pinMode(motor_dir_l_pin,OUTPUT);
 pinMode(motor_dir_r_pin,OUTPUT);

 digitalWrite(motor_dir_l_pin,LOW);
 digitalWrite(motor_dir_r_pin,LOW);




 
 
//attachInterrupt(digitalPinToInterrupt(5), right_wheel_pulse,CHANGE);
//attachInterrupt(digitalPinToInterrupt(4),right_wheel_pulse_b,CHANGE);
 
 
 
  // put your setup code here, to run once:

}

void loop() {

  //if forward pwm value is +ve (let),then -ve pwm makes opp rotation
 analogWrite(8,abs(PWM1));
 digitalWrite(9,PWM1<0);
//
  analogWrite(6,abs(PWM2));
 digitalWrite(7,PWM2<0);
//
  analogWrite(4,abs(PWM3));
 digitalWrite(5,PWM3<0);

  analogWrite(2,abs(PWM4));
 digitalWrite(3,PWM4<0);

  analogWrite(motor_pwm_l_pin,motor_pwm);
  analogWrite(motor_pwm_r_pin,motor_pwm);

  analogWrite(2,abs(PWM4));
 
// 
 nh.spinOnce();
 delay(1);
 
 
 
  // put your main code here, to run repeatedly:

}
