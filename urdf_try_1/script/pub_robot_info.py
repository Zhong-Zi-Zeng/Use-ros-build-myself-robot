#!/usr/bin/env python

import time
import rospy
from std_msgs.msg import String
from arduino_api import ArduinoApi

def publish_robot_info():
    while not rospy.is_shutdown():
        # Get real robot state
        response = arduino.read_info('a', 3)

        # Publish messages
        # pub_motor_vel.publish(response[0])
        # pub_motor_degree.publish(response[1])
        # pub_imu_info.publish(response[2])

if __name__ == "__main__":
    rospy.init_node('RobotStatePublish', anonymous=True)
    rospy.loginfo("Start Robot State Publisher")

    arduino = ArduinoApi()

    # Set publisher
    pub_motor_vel = rospy.Publisher("motor_vel", String, queue_size=5)
    pub_motor_degree = rospy.Publisher("motor_degree", String, queue_size=5)
    pub_imu_info = rospy.Publisher("imu_info", String, queue_size=5)

    # Publish real robot info
    publish_robot_info()

    rospy.spin()
