import rospy
from sensor_msgs.msg import Joy
import serial

ser = serial.Serial('/dev/ttyACM0', '115200')

def arm(xData,yData,pitchData,buttons):
    #Button is 0
    if xData > 0 and buttons[0] == 0:
        armState = [4, 0, abs(xData)]
        armStatus = f"Base\tDir: 0\tPWM: {abs(xData)}"
    elif xData < 0 and buttons[0] == 0:
        armState = [4, 1, abs(xData)]
        armStatus = f"Base\tDir: 1\tPWM: {abs(xData)}"

    elif yData > 0 and buttons[0] == 0:
        armState = [1, 0, abs(yData)]
        armStatus = f"Actuator1\tDir: 0\tPWM: {abs(yData)}"
    elif yData < 0 and buttons[0] == 0:
        armState = [1, 1, abs(yData)]
        armStatus = f"Actuator1\tDir: 1\tPWM: {abs(yData)}"


    #Button is 1
    elif xData > 0 and buttons[0] == 1:
        armState = [5, 0, abs(xData)]
        armStatus = f"Roll\tDir: 0\tPWM: {abs(xData)}"
    elif xData < 0 and buttons[0] == 1:
        armState = [5, 1, abs(xData)]
        armStatus = f"Roll\tDir: 1\tPWM: {abs(xData)}"
    elif yData > 0 and buttons[0] == 1:
        armState = [2, 0, abs(yData)]
        armStatus = f"Actuator2\tDir: 0\tPWM: {abs(yData)}"
    elif yData < 0 and buttons[0] == 1:
        armState = [2, 1, abs(yData)]
        armStatus = f"Actuator2\tDir: 1\tPWM: {abs(yData)}"
    

    #Pitch
    elif pitchData:
        if pitchData > 0:
            armState = [3, 0, 127]
            armStatus = f"Pitch\tDir: 0\tPWM: 127"
        else:
            armState = [3, 1, 127]
            armStatus = f"Pitch\tDir: 1\tPWM: 127"
    
    #Gripper
    elif buttons[1]:
        armState = [6, 0, 255]
        armStatus = f"Gripper\tDir: 0\tPWM: 255"
    elif buttons[3]:
        armState = [6, 1, 255]
        armStatus = f"Gripper\tDir: 1\tPWM: 255"
    
    else:
        armState = [7, 0, 0]
        armStatus = "Stop"
    
    print("Arm State: " + armStatus)
    command = bytearray(armState)
    ser.write(command)

def joy_callback(data):
    # Assuming data.axes is a list containing the axis values
    # For example, data.axes[0] is the value of the first axis
    arm(int(data.axes[0]*255), int(data.axes[1]*255), int(data.axes[5]*255), data.buttons)

def joystick_listener():
    rospy.init_node('joystick_listener', anonymous=True)

    # Replace "/joy" with the actual topic where joystick data is published
    rospy.Subscriber("/joy", Joy, joy_callback)

    # Spin to keep the script alive
    rospy.spin()


joystick_listener()
