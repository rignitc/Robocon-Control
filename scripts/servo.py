#!/usr/bin/env python

import rospy
from std_msgs.msg import Int16

rospy.init_node("servo_node")

class Main():

    def __init__(self):
        self.pub = rospy.Publisher("servo", Int16, queue_size=10)
        self.rate = rospy.Rate(10)

    def run(self):
        while not rospy.is_shutdown():
            num = int(input("Enter the angle: "))
            rospy.loginfo(f"Angle: {num}")
            self.pub.publish(num)
            self.rate.sleep()

