#!/usr/bin/env python
import time

import rospy
import numpy as np
import tf
from std_msgs.msg import String
from nav_msgs.msg import Odometry

def imu_info():
    imu = rospy.wait_for_message('/imu_info', String).data
    imu = list(map(float, imu.split()))

    return imu

def vel_info():
    vel = rospy.wait_for_message('/motor_vel', String).data
    vel = list(map(float, vel.split()))

    return vel

def pub_robot_state():
    rate = rospy.Rate(60)
    x = 0.0
    y = 0.0

    current_time = rospy.Time.now()
    last_time = current_time
    imu_init_value = imu_info()[2]
    last_imu_value = 0

    odom_pub = rospy.Publisher("odom", Odometry, queue_size=10)

    while not rospy.is_shutdown():
        current_time = rospy.Time.now()

        imu = imu_info()
        vel = vel_info()

        # Sport model
        T = np.array([[1, 1, 1, 1],
                      [-1, 1, -1, 1],
                      [-1 / (Rx + Ry), -1 / (Rx + Ry), 1 / (Rx + Ry), 1 / (Rx + Ry)]])

        W = np.array(vel).reshape((-1, 1))
        V = (R / 4) * np.dot(T, W)
        V = V.reshape((-1))

        dt = (current_time - last_time).to_sec()
        th = (imu[2] - imu_init_value) * np.pi / 180
        delta_x = (V[0] * np.cos(th) - V[1] * np.sin(th)) * dt
        delta_y = (V[0] * np.sin(th) + V[1] * np.cos(th)) * dt

        x += delta_x
        y += delta_y

        # Publisher Odom
        quat = tf.transformations.quaternion_from_euler(0, 0, th)
        br = tf.TransformBroadcaster()
        br.sendTransform((x, y, 0.0),
                         quat,
                         current_time,
                         "base_link",
                         "odom")

        # Publisher Nav odom
        odom = Odometry()
        odom.header.stamp = current_time
        odom.header.frame_id = 'odom'

        odom.pose.pose.position.x = x
        odom.pose.pose.position.y = y
        odom.pose.pose.position.z = 0.0
        odom.pose.pose.orientation.x = quat[0]
        odom.pose.pose.orientation.y = quat[1]
        odom.pose.pose.orientation.z = quat[2]
        odom.pose.pose.orientation.w = quat[3]

        odom.child_frame_id = "base_link"
        odom.twist.twist.linear.x = V[0]
        odom.twist.twist.linear.y = V[1]
        odom.twist.twist.angular.z = (imu[2] - last_imu_value) * np.pi / (dt * 180)

        odom_pub.publish(odom)

        last_time = current_time
        last_imu_value = imu[2]
        rate.sleep()


if __name__ == '__main__':
    rospy.init_node('TFPublisher', anonymous=True)
    rospy.loginfo("Start Robot TF Publisher")

    Rx = rospy.get_param("/Rx")  # Center distance of side wheel
    Ry = rospy.get_param("/Ry")  # The distance of robot center point to wheel
    R = rospy.get_param("/R")  # Wheel radius

    # Set subscriber
    pub_robot_state()
