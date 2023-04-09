//#define ROSSERIAL_ARDUINO_TCP
#include <ros.h>
#include <geometry_msgs/Quaternion.h>




ros::NodeHandle nh;

int PWM1;
int PWM2;
int PWM3;
int PWM4;
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

//Subscriber
ros::Subscriber<geometry_msgs::Quaternion> sub1("keyboard_message1", &callback1);


void setup() {

 //NODE 
 nh.initNode();
 nh.subscribe(sub1);
 

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
// 
 nh.spinOnce();
 delay(1);
 
 
 
  // put your main code here, to run repeatedly:

}
