import rospy
from sensor_msgs.msg import Joy
import serial

ser = serial.Serial('/dev/ttyUSB0', '115200')

def drive(xData,yData):
    if xData > 0:
        driveState = [4, abs(xData)]
        driveStatus = f"Right\tPWM:{driveState[1]}"
    elif xData < 0:
        driveState = [3, abs(xData)]
        driveStatus = f"Left\tPWM:{driveState[1]}"
    elif yData > 0:
        driveState = [2, abs(yData)]
        driveStatus = f"Backward\tPWM:{driveState[1]}"
    elif yData < 0:
        driveState = [1, abs(yData)]
        driveStatus = f"Forward\tPWM:{driveState[1]}"
    else:
        driveState = [5, 0]
        driveStatus = "Stop"
        print("Drive State: "+driveStatus)
    command = bytearray(driveState)
    ser.write(command)

def joy_callback(data):
    # Assuming data.axes is a list containing the axis values
    # For example, data.axes[0] is the value of the first axis
    drive(int(data.axes[0]*255), -int(data.axes[1]*255))

def joystick_listener():
    rospy.init_node('joystick_listener', anonymous=True)

    # Replace "/joy" with the actual topic where joystick data is published
    rospy.Subscriber("/joy", Joy, joy_callback)

    # Spin to keep the script alive
    rospy.spin()


joystick_listener()
