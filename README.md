# autonomous-underwater-robot
This repository contains Robot-C code for the dry land prototype of an autonomous underwater robot .

Robot-C is a C-based programming language used primarily for robotics. The entire functionality of the prototype is defined using this language. 

The dry land proptotype of this robot connects cables autonomously from one to point to another. It uses a combination of ultrasonic and IR sensors to scan its vicinity to detect the beacon upon which the target object is placed. After detecting the beacon, the robot travels towards it, course correcting on the way as necessary. Upon arriving at a suitable distance from the beacon, the mechanincal arm of the robot descends downwards to connect the cable. The prototype uses multiple of LED's to depict the distance from the target object. 
The robot avoids obstaces by using IR sensors and bumper switches.

The dry land prototype was created in response to a RFP from Ocean Networks Canada (ONC) to help them design autonomous robots that connect cables from one point to another. This allowed them to reduce reliance on human divers.
