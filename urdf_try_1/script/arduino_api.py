#!/usr/bin/env python

import serial
import rospy
import time


class ArduinoApi:
    def __init__(self):
        self.ser = serial.Serial(rospy.get_param('/MegaPort'), rospy.get_param('/MegaBaudrate'))
        time.sleep(2)

    def read_info(self, mode, data_length):
        """
         Selecting which information do you want.
        :param mode:
                1. a: Get motor velocity ,motor degree and IMU info (pitch, row, yaw)

        :param data_length: To check message is correct.

        :return: From arduino mega information.
                 (motor velocity|motor degree|IMU info
        """

        while True:
            response = " "
            try:
                self.ser.write(str(mode + "\n").encode("utf-8"))
                time.sleep(0.01)

                while self.ser.in_waiting:
                    response = self.ser.readline().decode().rstrip()

                first_char = response[0]
                response = str(response).strip('a').split('|')
                self.clear_register()
                if first_char == mode and len(response) == data_length:
                    return response
            except:
                rospy.loginfo("Serial Reading Error!")
                continue


    def send_vel_command(self, desired_vel):
        """
        Send desired velocity to motors
        :param desired_vel: ("+0.10+0.00+0.00+0.00")
        """
        try:
            self.ser.write(str("d" + desired_vel + "\n").encode("utf-8"))
            time.sleep(0.01)
        except:
            rospy.loginfo("Serial Writing Error!")

    def clear_register(self):
        """
        Clear uart register,
        """
        while self.ser.in_waiting:
            self.ser.read()
