#!/usr/bin/env python

import rospy
import math
from std_msgs.msg import Int32
from geometry_msgs.msg import Vector3
from simple_pid import PID
from std_msgs.msg import Bool

set_point=0
current_value=0
pwm_voltage=0
accelx=0
accely=0
accelz=0
pid= PID(0,0,0,0)
pid.output_limits = (-255,255)

def imu_data_collector(data):
    global accelx,accely,accelz
    accelx=data.x
    accely=data.y
    accelz=data.z
    ori_calc()

def tar_set(data1):
    global set_point,pid
    set_point=data1.data
    pid = PID(2, 0.04, 0.4, setpoint=set_point)

def ori_calc():
    global accelx,accely,accelz,current_value
    current_value=-(math.atan2(-accelx,math.sqrt((accely**2)+(accelz**2))))*180/3.14      


def talker():
    global current_value,set_point,pwm_voltage,pid
    pub = rospy.Publisher('target_pitch', Int32, queue_size=10)
    rospy.init_node('processing', anonymous=True)
    rospy.Subscriber("imu_data", Vector3, imu_data_collector)
    rospy.Subscriber("target",Int32,tar_set)
    rate = rospy.Rate(5) # 10hz
    # set_point=float(input())
    # pid = PID(0.5, 0.01, 0.1, setpoint=set_point)
    while not rospy.is_shutdown():
        # hello_str = "hello world %s" % rospy.get_time()
        pwm_voltage=(pid(current_value))*5
        if(abs(current_value-set_point)>2):
                pub.publish(int(pwm_voltage))
        else:
            pub.publish(0)
        rospy.loginfo(f"Target {set_point} ,Current {current_value}, PWM {pwm_voltage}")
        rate.sleep()

if __name__ == '__main__':
    try:
        talker()
    except rospy.ROSInterruptException:
        pass
