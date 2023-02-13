#!/usr/bin/env python

import rospy
import numpy as np
import time
from sensor_msgs.msg import JointState
from std_msgs.msg import String
from std_msgs.msg import Header


def pub_wheel_joint_state(motor_degree):
    motor_degree = motor_degree.data.strip('b')
    motor_degree = list(map(float, motor_degree.split()))

    joint_state_info = JointState()
    joint_state_info.header = Header()
    joint_state_info.header.stamp = rospy.Time.now()
    joint_state_info.name = ['lf_wheel_join', 'lb_wheel_join', "rf_wheel_join", "rb_wheel_join"]
    joint_state_info.position = [motor_degree[0] * np.pi / 180.,
                                 motor_degree[1] * np.pi / 180.,
                                 motor_degree[2] * np.pi / 180.,
                                 motor_degree[3] * np.pi / 180.]

    joint_state_info.velocity = []
    joint_state_info.effort = []

    pub.publish(joint_state_info)

if __name__ == '__main__':
    rospy.init_node('JointPublisher', anonymous=True)
    rospy.loginfo("Start Robot Joint Publisher")

    # Set subscriber
    pub = rospy.Publisher('joint_states', JointState, queue_size=10)
    sub_motor_degree = rospy.Subscriber("/motor_degree", String, pub_wheel_joint_state)

    rospy.spin()
