#define en 8
//#define rese 2
//#define slee 3
//#define stp 3
//#define dir 6
#define stp 2
#define dir 5
const int stepsPerRevolution=200;

void setup() {
  // put your setup code here, to run once:
  pinMode(en,OUTPUT);
//  pinMode(rese,OUTPUT);
//  pinMode(slee,OUTPUT);
  pinMode(dir,OUTPUT);
  pinMode(stp,OUTPUT);
  digitalWrite(en,LOW);
//  digitalWrite(rese,HIGH);
//  digitalWrite(slee,HIGH);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(dir, HIGH);
  for (int x = 0; x < 2*stepsPerRevolution; x++)
    {
      digitalWrite(stp, HIGH);
      delayMicroseconds(600);
      digitalWrite(stp, LOW);
      delayMicroseconds(600);
    }
    delay(1000);
  digitalWrite(dir, LOW);
    for (int x = 0; x < 2*stepsPerRevolution; x++)
    {
      digitalWrite(stp, HIGH);
      delayMicroseconds(600);
      digitalWrite(stp, LOW);
      delayMicroseconds(600);
    }
    delay(1000);
}
