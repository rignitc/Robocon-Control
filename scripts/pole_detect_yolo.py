import torch
import matplotlib.pyplot as plt
import cv2
import sys
import numpy
import pandas
import rospy
from std_msgs.msg import Float32
from simple_pid import PID
from geometry_msgs.msg import Quaternion

loco = Quaternion()
loco.x=0
loco.y=0
loco.z=0

model = torch.hub.load('ultralytics/yolov5', 'custom',path='/home/mkris/Downloads/best.pt')
model.conf = 0.1  # NMS confidence threshold
model.iou = 0.45  # NMS IoU threshold
model.agnostic = False  # NMS class-agnostic
model.multi_label = False  # NMS multiple labels per box
model.classes = None    # (optional list) filter by class, i.e. = [0, 15, 16] for COCO persons, cats and dogs
model.max_det = 1  # maximum number of detections per image
model.amp = False  # Automatic Mixed Precision (AMP) inference

s = 0
if len(sys.argv) > 1:
    s = sys.argv[1]
alive = True
win_name = "Camera Filters"
cv2.namedWindow(win_name, cv2.WINDOW_NORMAL)
result = None

source = cv2.VideoCapture(s)
set_point= source.get(cv2.CAP_PROP_FRAME_WIDTH)/2
pid = PID(0.005, 0.0001, 0.0001, setpoint=set_point) #midpoint value lenght of frame /2
pid.output_limits=(-1,1)

def talker():
    global set_point,loco
    pub = rospy.Publisher('locomotion', Quaternion, queue_size=10)
    rospy.init_node('CV', anonymous=True)
    rate = rospy.Rate(10) # 10hz
    while not rospy.is_shutdown():
        has_frame, frame = source.read()
        if not has_frame:
            break
        results = model(frame)
        # print(results.pandas().xywh[0].xcenter)
        cv2.imshow(win_name, results.render()[0])
        key = cv2.waitKey(1)
        if key == ord("Q") or key == ord("q") or key == 27:
            rospy.signal_shutdown()
        XCEN_LIST= results.pandas().xywh[0].xcenter.tolist() 
        if bool(XCEN_LIST):
            loco.w=-pid(XCEN_LIST[0])
            rospy.loginfo(f"Target {set_point} ,Current {XCEN_LIST[0]}, PWM {loco.w}")
            # print(type(XCEN_LIST[0]))
            pub.publish(loco)
        rate.sleep()
    source.release()
    cv2.destroyWindow(win_name)



if __name__ == '__main__':
    try:
        talker()
    except rospy.ROSInterruptException:
        pass