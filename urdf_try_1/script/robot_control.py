#!/usr/bin/env python

import rospy
import numpy as np
from arduino_api import ArduinoApi
from geometry_msgs.msg import Twist


def publish_desired_vel(twist_msg):
    linear_vel_x = twist_msg.linear.x
    linear_vel_y = twist_msg.linear.y
    angular_vel_z = twist_msg.angular.z

    # Sport Model
    T = np.array([[1, -1, -(Rx + Ry)],
                  [1, 1, -(Rx + Ry)],
                  [1, -1, (Rx + Ry)],
                  [1, 1, (Rx + Ry)]])

    X = np.array([[linear_vel_x, linear_vel_y, angular_vel_z]]).T
    W = np.dot(T, X) / R
    W = W.reshape(-1, )

    if (angular_vel_z > 0 and linear_vel_x > 0):
        if (W[2] > 7):
            W[2:] = W[2:] - 5
    # Send to arduino api
    desired_vel = [round(w, 2) for w in W]
    desired_vel = list(map(str, desired_vel))

    arduino.send_vel_command(",".join(list(map(str,desired_vel))))

if __name__ == "__main__":
    rospy.init_node("RobotControl", anonymous=True)
    rospy.loginfo("Start Robot Controller")
    arduino = ArduinoApi()

    Rx = rospy.get_param("/Rx")  # Center distance of side wheel
    Ry = rospy.get_param("/Ry")  # The distance of robot center point to wheel
    R = rospy.get_param("/R")  # Wheel radius

    # Velocity subscriber
    sub_vel = rospy.Subscriber(rospy.get_param("/VelTopic"), Twist, publish_desired_vel)

    rospy.spin()
