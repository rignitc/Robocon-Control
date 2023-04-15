# Robocon-Control

To run the files
1. Install joy ros package http://wiki.ros.org/joy
2. Git clone this repo into catkin_ws/src
3. Plug all arduinos and joystick
4. In terminal, run "roslaunch robocon_control main1.launch"


To run RR 
1. Git clone the repo
2. Edit bashrc with proper ip address of both client and server
3. Check if connected using rostopic list
(Check that the launch file in Raspberry Pi contains only rosserial node, rest are commented)
4. In both Raspberry Pi and Computer, run "source ./devel/setup.bash" and then
"roslaunch robocon_control robo_launch.launch"
5. Good to go