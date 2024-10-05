import serial

class control():
    def __init__(self, port, baud_rate):
        self.port = port
        self.baud_rate = baud_rate
        self.state = []

    def connect(self):
        self.serial_port = serial.Serial(self.port, self.baud_rate)
    
    def serialWrite(self):
        command = bytearray(self.state)
        self.serial_port.write(command)


class drive(control):
    def ctrl(self, xData, yData):
        if xData > 0:
            self.state = [4, abs(xData)]
            self.status = f"Right\tPWM:{abs(xData)}"
        elif xData < 0:
            self.state = [3, abs(xData)]
            self.status = f"Left\tPWM:{abs(xData)}"
        elif yData > 0:
            self.state = [2, abs(yData)]
            self.status = f"Backward\tPWM:{abs(yData)}"
        elif yData < 0:
            self.state = [1, abs(yData)]
            self.status = f"Forward\tPWM:{abs(yData)}"
        else:
            self.state = [5, 0]
            self.status = "Stop"
        return "Drive State: "+self.status


class arm(control):
    def ctrl(self, xData, yData, pitchData, buttons):
        #Button is 0
        if xData > 0 and buttons[0] == 0:
            self.state = [4, 0, abs(xData)]
            self.status = f"Base\tDir: 0\tPWM: {abs(xData)}"
        elif xData < 0 and buttons[0] == 0:
            self.state = [4, 1, abs(xData)]
            self.status = f"Base\tDir: 1\tPWM: {abs(xData)}"

        elif yData > 0 and buttons[0] == 0:
            self.state = [1, 0, abs(yData)]
            self.status = f"Actuator1\tDir: 0\tPWM: {abs(yData)}"
        elif yData < 0 and buttons[0] == 0:
            self.state = [1, 1, abs(yData)]
            self.status = f"Actuator1\tDir: 1\tPWM: {abs(yData)}"


        #Button is 1
        elif xData > 0 and buttons[0] == 1:
            self.state = [5, 0, abs(xData)]
            self.status = f"Roll\tDir: 0\tPWM: {abs(xData)}"
        elif xData < 0 and buttons[0] == 1:
            self.state = [5, 1, abs(xData)]
            self.status = f"Roll\tDir: 1\tPWM: {abs(xData)}"
        elif yData > 0 and buttons[0] == 1:
            self.state = [2, 0, abs(yData)]
            self.status = f"Actuator2\tDir: 0\tPWM: {abs(yData)}"
        elif yData < 0 and buttons[0] == 1:
            self.state = [2, 1, abs(yData)]
            self.status = f"Actuator2\tDir: 1\tPWM: {abs(yData)}"
        

        #Pitch
        elif pitchData:
            if pitchData > 0:
                self.state = [3, 0, 127]
                self.status = f"Pitch\tDir: 0\tPWM: 127"
            else:
                self.state = [3, 1, 127]
                self.status = f"Pitch\tDir: 1\tPWM: 127"
        
        #Gripper
        elif buttons[1]:
            self.state = [6, 0, 255]
            self.status = f"Gripper\tDir: 0\tPWM: 255"
        elif buttons[3]:
            self.state = [6, 1, 255]
            self.status = f"Gripper\tDir: 1\tPWM: 255"
        
        else:
            self.state = [7, 0, 0]
            self.status = "Stop"
        
        return "Arm State: " + self.status
