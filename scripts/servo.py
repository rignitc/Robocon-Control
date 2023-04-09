#!/usr/bin/env python

import rospy
from std_msgs.msg import Int16
import time

rospy.init_node("servo_node")

class Main():

    def __init__(self):
        self.pub = rospy.Publisher("servo", Int16, queue_size=10)
        self.rate = rospy.Rate(10)

    def run(self):
        while not rospy.is_shutdown():
            # num = int(input("Enter angle: "))
            # rospy.loginfo(f"Angle: {num}")
            # self.pub.publish(num)
            # self.rate.sleep()
            num = input("Press to shoot")
            rospy.loginfo(f"Shooting")
            self.pub.publish(90)
            time.sleep(1)
            self.pub.publish(0)
            self.rate.sleep()


if __name__ == "__main__":
    main = Main()
    main.run()