import serial

class control():
    def __init__(self, port, baud_rate):
        self.port = port
        self.baud_rate = baud_rate
        self.driveState = [0, 0]
        self.armState = [0, 0, 0]

    def connect(self):
        self.serial_port = serial.Serial(self.port, self.baud_rate)

    def driveCtrl(self, xData, yData):
        if xData > 0:
            self.driveState = [4, abs(xData)]
            driveStatus = f"Right\tPWM:{self.driveState[1]}"
        elif xData < 0:
            self.driveState = [3, abs(xData)]
            driveStatus = f"Left\tPWM:{self.driveState[1]}"
        elif yData > 0:
            self.driveState = [2, abs(yData)]
            driveStatus = f"Backward\tPWM:{self.driveState[1]}"
        elif yData < 0:
            self.driveState = [1, abs(yData)]
            driveStatus = f"Forward\tPWM:{self.driveState[1]}"
        else:
            self.driveState = [5, 0]
            driveStatus = "Stop"
        return "Drive State: "+driveStatus

    def armCtrl(self, xData, yData, pitchData, buttons):
        #Button is 0
        if xData > 0 and buttons[0] == 0:
            self.armState = [4, 0, abs(xData)]
            armStatus = f"Base\tDir: 0\tPWM: {abs(xData)}"
        elif xData < 0 and buttons[0] == 0:
            self.armState = [4, 1, abs(xData)]
            armStatus = f"Base\tDir: 1\tPWM: {abs(xData)}"

        elif yData > 0 and buttons[0] == 0:
            self.armState = [1, 0, abs(yData)]
            armStatus = f"Actuator1\tDir: 0\tPWM: {abs(yData)}"
        elif yData < 0 and buttons[0] == 0:
            self.armState = [1, 1, abs(yData)]
            armStatus = f"Actuator1\tDir: 1\tPWM: {abs(yData)}"


        #Button is 1
        elif xData > 0 and buttons[0] == 1:
            self.armState = [5, 0, abs(xData)]
            armStatus = f"Roll\tDir: 0\tPWM: {abs(xData)}"
        elif xData < 0 and buttons[0] == 1:
            self.armState = [5, 1, abs(xData)]
            armStatus = f"Roll\tDir: 1\tPWM: {abs(xData)}"
        elif yData > 0 and buttons[0] == 1:
            self.armState = [2, 0, abs(yData)]
            armStatus = f"Actuator2\tDir: 0\tPWM: {abs(yData)}"
        elif yData < 0 and buttons[0] == 1:
            self.armState = [2, 1, abs(yData)]
            armStatus = f"Actuator2\tDir: 1\tPWM: {abs(yData)}"
        

        #Pitch
        elif pitchData:
            if pitchData > 0:
                self.armState = [3, 0, 127]
                armStatus = f"Pitch\tDir: 1\tPWM: 127"
            else:
                self.armState = [3, 1, 127]
                armStatus = f"Pitch\tDir: 1\tPWM: 127"
        
        #Gripper
        elif buttons[1]:
            self.armState = [6, 0, 255]
            armStatus = f"Gripper\tDir: 0\tPWM: 255"
        elif buttons[3]:
            self.armState = [6, 1, 255]
            armStatus = f"Gripper\tDir: 1\tPWM: 255"
        
        else:
            self.armState = [7, 0, 0]
            armStatus = "Stop"
        
        return "Arm State: " + armStatus

    def serialWrite(self):
        #command = bytearray(self.driveState)
        command = bytearray(self.armState)
        self.serial_port.write(command)
