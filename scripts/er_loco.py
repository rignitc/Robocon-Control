#! /bin/python3
import rospy
#import vector3 message type
from geometry_msgs.msg import Quaternion
from geometry_msgs.msg import Vector3
import numpy as np
from std_msgs.msg import Float64
# import keyboard

RPM =0
x=0
y=0
w=0



def callback(rpm):
    RPM = rpm.data

def joy_callback(data):
    global x,y,w
    if -0.01<data.x<0.01:
        x=0
    else:
         
         x=data.x

    if -0.01<data.y<0.01:
         y=0
    else:

         y=data.y

    w=data.w
         


    # print(x,y,z,w)

def keyboard_node():
   
    global x,y,w
    joy=rospy.Subscriber('locomotion',Quaternion,joy_callback)

    #create publishers for each motor

    pub1=rospy.Publisher('keyboard_message1',Quaternion,queue_size=10)
    # pub2=rospy.Publisher('keyboard_message2',Vector3,queue_size=10)
    # pub3=rospy.Publisher('keyboard_message3',Vector3,queue_size=10)
    # pub4=rospy.Publisher('keyboard_message4',Vector3,queue_size=10)

    # pwm_publisher = rospy.Publisher('pwm_values',keyboard_pwm,queue_size=10)


    #initialize node
    rospy.init_node('keyboard_node',anonymous=True)



    #initialize speed variables
    mes1=Quaternion()
    # mes2=Vector3()
    # mes3=Vector3()
    # mes4=Vector3()


    #mobile base parameters
    l = 0.58/2
    w = 0.58/2
    r = 0.127/2

    #allocation matrix for the base (ref:Modern Robotic, mechanics ,planning and control pg.no. 519 http://hades.mech.northwestern.edu/images/7/7f/MR.pdf)
    allocation_matrix  = np.mat((1/r)*np.array([[-l-w ,-1,-1],[l+w,1,-1],[l+w,-1,-1],[-l-w,1,-1]]))


    pwms = np.mat(np.array([[0],[0],[0],[0]]))
    
    while not rospy.is_shutdown():
            
            twist = np.mat(np.array([float(w),float(x),float(y)])).T
            # print(x,y)
            wheel_speeds = allocation_matrix*twist
        #convert to pwm values


            pwms_target = speed_to_pwm(wheel_speeds)


            while np.linalg.norm((pwms_target-pwms))>=0.001 :
                pwms = pwms + 1*(pwms_target - pwms)
            # prev_pwms = pwms
                mes1.x = 1*pwms[0] 
                mes1.y = 1*pwms[1]
                mes1.z = 1*pwms[2]
                mes1.w = 1*pwms[3]
                

                pub1.publish(mes1)
                 
             
            # print(pwms)
            

             


        
             
 
        


def speed_to_pwm(speed):
    return speed*15


if __name__ == '__main__':
	try:
		keyboard_node()
	except rospy.ROSInterruptException:
		pass

				
		  

			
				
			
	


