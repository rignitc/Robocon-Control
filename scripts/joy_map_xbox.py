#!/usr/bin/env python
# license removed for brevity


#keep analog mode on in controller



import rospy
from std_msgs.msg import String
from std_msgs.msg import Int32
from std_msgs.msg import Bool

from sensor_msgs.msg import Joy
import time
from geometry_msgs.msg import Quaternion

loco = Quaternion()
yaw= Int32()
pitch= Int32()
yaw_lock=0  #0 locked 1 unlocked
auto_lock=Bool(True)
obj=Bool(False)
screw_power= Int32()
belt_power= Int32()
flick= Bool()
speed= Bool()
locoSpeed = Bool()
prev=time.time()
data=None
i=0

def map_range(x, in_min, in_max, out_min, out_max):
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min

# def changer(value): 
#     if(value==0):
#         value=1
#         print('unlocked')
#     elif(value==1):
#         value=0
#         print('locked')
#     else:
#         rospy.loginfo("error in changer")
#         rospy.signal_shutdown
#     return value



def loco_finder(data):
    global loco
    loco.z=0
    loco.x=(-data.axes[0])/0.8
    loco.y=(data.axes[1])/0.8
    if(data.buttons[3]==1):
        if(loco.y>=0):
            loco.w=-1
        else:
            loco.w=1
        # loco.w=-1
        
    elif(data.buttons[1]==1):
        if(loco.y>=0):
            loco.w=1
        else:
            loco.w=-1
        # loco.w=1

    else:
        loco.w=0
    if(data.buttons[2]==1):
        locoSpeed.data=True
    else:
        locoSpeed.data=False



def yp_finder(data):
    global yaw,pitch
    # yaw_decimal=-data.axes[3]
    pitch_decimal=-data.axes[4]
    # yaw.data=int(map_range(yaw_decimal,-1,1,-30,30))
    pitch.data=int(map_range(pitch_decimal,-1,1,-255,255))

def pick_control(data):
    global screw_power,belt_power,flick,speed,i,obj,prev
    screw_power.data=int(data.axes[6])
    belt_power.data=-int(data.axes[7])
    curr_t=time.time()
    # print(time.time()-prev)
    if(data.axes[2]==-1 and curr_t-prev>2):
        # print(curr_t-prev)
        flick.data=True
        prev=time.time()
        # print('t')
    else:
        flick.data=False
        # print('f')

    if(data.buttons[4]==1):
        i=(i+1)%4
        speed.data=True
        print(f"speed {i}")
    else:
        speed.data=False
    # print('helo')
    if(data.buttons[5]==1):
        obj.data=True
        # print('hi')
    else:
        obj.data=False
    
    



def joy_callback(data_ros):
    global auto_lock,data
    # if(data.axes[5]==-1):
        # auto_lock.data=(not auto_lock.data)
    # print('helo')
    data=data_ros
    # print(type(data))
    # rospy.loginfo(data.axes)
    # if(data.buttons[5]==1):
    #     yaw_lock=changer(yaw_lock)


def talker():
    global obj,data
    rospy.Subscriber('joy', Joy, joy_callback)
    pub_loco = rospy.Publisher('locomotion', Quaternion, queue_size=10)
    pub_yaw = rospy.Publisher('target_yaw', Int32, queue_size=10)
    pub_pitch = rospy.Publisher('target_pitch', Int32, queue_size=10)
    pub_screw = rospy.Publisher('screw_power', Int32, queue_size=10)        #these screw and belt can be combined into a single custom message
    pub_belt = rospy.Publisher('belt_power', Int32, queue_size=10)
    pub_flick = rospy.Publisher('flick', Bool, queue_size=10)
    pub_speed = rospy.Publisher('speed', Bool, queue_size=10)
    pub_locoSpeed = rospy.Publisher('locoSpeed', Bool, queue_size=10)
    pub_auto = rospy.Publisher('auto_locker', Bool, queue_size=10)
    pub_changer = rospy.Publisher('obj_changer', Bool, queue_size=10)


    

    rospy.init_node('talker', anonymous=True)
    rate = rospy.Rate(10) # 10hz
    while not rospy.is_shutdown():
        # hello_str = "hello world %s" % rospy.get_time()
        # rospy.loginfo(hello_str)
        pub_auto.publish(auto_lock)
        if (data):
            loco_finder(data)
            yp_finder(data)
            pick_control(data)
        if auto_lock.data:
            pub_loco.publish(loco)
            pub_yaw.publish(yaw)
            pub_pitch.publish(pitch)
            pub_screw.publish(screw_power)    
            pub_belt.publish(belt_power)    
            pub_flick.publish(flick)   
            pub_speed.publish(speed)
            pub_locoSpeed.publish(locoSpeed)
        else:
            pub_changer.publish(obj)    


        rate.sleep()

if __name__ == '__main__':
    try:
        talker()
    except rospy.ROSInterruptException:
        pass