#!/usr/bin/env python
import rospy
from sensor_msgs.msg import Joy
from serial_package import ctrl
from roverPilot.msg import sensorMsg
import threading

baud_rate = 115200
port = input("Enter port: ")

sciObj = ctrl.science(port, baud_rate)
sciObj.connect()

def listener():
    def callback(data):
        status = sciObj.setState(data.buttons[0], data.buttons[1])
        sciObj.serialWrite()
        rospy.loginfo("%s", status)

    # Subscriber for joystick messages
    rospy.Subscriber('/joy', Joy, callback)
    rospy.spin()  # Keep the listener running

def talker():
    pub = rospy.Publisher('sensor_data', sensorMsg, queue_size=10)
    rate = rospy.Rate(1)  # 1 Hz

    while not rospy.is_shutdown():
        dictData = sciObj.readState()

        sensorData = sensorMsg()
        sensorData.UV_Voltage = dictData['UV Voltage']
        sensorData.CO_MQ7 = dictData['CO (MQ-7)']
        sensorData.pH = dictData['pH']
        sensorData.Ozone_MQ131 = dictData['Ozone (MQ-131)']
        sensorData.NH4_MQ135 = dictData['NH4 (MQ-135)']
        sensorData.Methane_MQ4 = dictData['Methane (MQ-4)']
        sensorData.Soil_Temperature = dictData['Soil Temp']
        sensorData.Soil_Moisture = dictData['Soil Moisture']

        pub.publish(sensorData)
        rate.sleep()

if __name__ == '__main__':
    try:
        # Initialize the ROS node (only once)
        rospy.init_node('talker', anonymous=True)

        # Create and start publisher thread
        publisher_thread = threading.Thread(target=talker)
        publisher_thread.daemon = True
        publisher_thread.start()

        # Run the listener (subscriber) in the main thread
        listener()

        rospy.loginfo("Exiting main program.")

    except rospy.ROSInterruptException:
        pass
