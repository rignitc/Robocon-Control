#!/usr/bin/env python
# license removed for brevity


#keep analog mode on in controller



import rospy
from std_msgs.msg import String
from std_msgs.msg import Int32
from std_msgs.msg import Bool

from sensor_msgs.msg import Joy

from geometry_msgs.msg import Quaternion

loco = Quaternion()
yaw= Int32()
pitch= Int32()
yaw_lock=0  #0 locked 1 unlocked
pitch_lock=0
screw_power= Int32()
belt_power= Int32()
flick= Bool()
speed= Bool()
locoSpeed = Bool()

def map_range(x, in_min, in_max, out_min, out_max):
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min

def changer(value): 
    if(value==0):
        value=1
        print('unlocked')
    elif(value==1):
        value=0
        print('locked')
    else:
        rospy.loginfo("error in changer")
        rospy.signal_shutdown
    return value



def loco_finder(data):
    global loco
    loco.z=0
    loco.x=-data.axes[0]
    loco.y=data.axes[1]
    if(data.buttons[0]==1):
        loco.w=1
    elif(data.buttons[1]==1):
        loco.w=-1
    else:
        loco.w=0
    if(data.buttons[10]==1):
        locoSpeed.data=True
    else:
        locoSpeed.data=False



def yp_finder(data):
    global yaw,pitch,lock,yaw_lock,pitch_lock
    yaw_decimal=-data.axes[3]
    pitch_decimal=data.axes[4]
    if(yaw_lock==1):
        yaw.data=int(map_range(yaw_decimal,-1,1,-30,30))
    if(pitch_lock==1):
        pitch.data=int(map_range(pitch_decimal,-1,1,-255,255))
 
def pick_control(data):
    global screw_power,belt_power,flick,speed
    screw_power.data=int(data.axes[6])
    belt_power.data=-int(data.axes[5])
    if(data.buttons[6]==1):
        flick.data=True
    else:
        flick.data=False
    if(data.buttons[4]==1):
        speed.data=True
    else:
        speed.data=False
    
    



def joy_callback(data):
    global yaw_lock,pitch_lock
    loco_finder(data)
    # rospy.loginfo(data.axes)
    if(data.buttons[5]==1):
        yaw_lock=changer(yaw_lock)
    if(data.buttons[7]==1):
        pitch_lock=changer(pitch_lock)
    yp_finder(data)
    pick_control(data)


def talker():
    rospy.Subscriber('joy', Joy, joy_callback)
    pub_loco = rospy.Publisher('locomotion', Quaternion, queue_size=10)
    pub_yaw = rospy.Publisher('target_yaw', Int32, queue_size=10)
    pub_pitch = rospy.Publisher('target_pitch', Int32, queue_size=10)
    pub_screw = rospy.Publisher('screw_power', Int32, queue_size=10)        #these screw and belt can be combined into a single custom message
    pub_belt = rospy.Publisher('belt_power', Int32, queue_size=10)
    pub_flick = rospy.Publisher('flick', Bool, queue_size=10)
    pub_speed = rospy.Publisher('speed', Bool, queue_size=10)
    pub_locoSpeed = rospy.Publisher('locoSpeed', Bool, queue_size=10)


    

    rospy.init_node('talker', anonymous=True)
    rate = rospy.Rate(10) # 10hz
    while not rospy.is_shutdown():
        # hello_str = "hello world %s" % rospy.get_time()
        # rospy.loginfo(hello_str)
        pub_loco.publish(loco)
        pub_yaw.publish(yaw)
        pub_pitch.publish(pitch)
        pub_screw.publish(screw_power)    
        pub_belt.publish(belt_power)    
        pub_flick.publish(flick)   
        pub_speed.publish(speed)
        pub_locoSpeed.publish(locoSpeed)    


        rate.sleep()

if __name__ == '__main__':
    try:
        talker()
    except rospy.ROSInterruptException:
        pass
