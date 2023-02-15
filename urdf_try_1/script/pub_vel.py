#!/usr/bin/env python
# -*- coding: UTF-8 -*-
import rospy
import tf
import time
import numpy as np
from sensor_msgs.msg import Imu
from std_msgs.msg import String



def imu_callback(imu_msgs):
    # Quaternion to Euler
    w = imu_msgs.orientation.w
    x = imu_msgs.orientation.x
    y = imu_msgs.orientation.y
    z = imu_msgs.orientation.z

    yaw = tf.transformations.euler_from_quaternion([w, x, y, z])[0]

    # Calculate velocity
    global last_time, last_a_x, last_a_y, v_x, v_y
    now_time = time.time()

    if last_time is not None:
        a_x = imu_msgs.linear_acceleration.x
        a_y = imu_msgs.linear_acceleration.y
        d_t = now_time - last_time

        v_x += d_t * (a_x)
        v_y += d_t * (a_y)

        rospy.loginfo(v_x)
        last_a_x = a_x
        last_a_y = a_y

    last_time = now_time

    vel_data = String()
    vel_data.data = "v_x:{}, v_y:{}".format(v_x, v_y)
    pub.publish(vel_data)

if __name__ == '__main__':
    rospy.init_node('pub_vel')

    last_time = None
    last_a_x = 0
    last_a_y = 0
    v_x = 0
    v_y = 0

    # Subscriber imu topic
    rospy.Subscriber('/imu', Imu, imu_callback)

    # Publisher imu velocity and euler angle
    pub = rospy.Publisher('/imu_vel_euler', String, queue_size=1)

    rospy.spin()