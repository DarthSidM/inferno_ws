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
        print(xData)
        print(yData)
        print(pitchData)

        if buttons[0] == 0:
            if xData > 0:
                self.armState = [4, 0, abs(xData)]
            elif xData < 0:
                self.armState = [4, 1, abs(xData)]

            if yData > 0:
                self.armState = [1, 0, abs(yData)]
            elif yData < 0:
                self.armState = [1, 1, abs(yData)]

        elif buttons[0] == 1:
            if xData > 0:
                self.armState = [5, 0, abs(xData)]
            elif xData < 0:
                self.armState = [5, 1, abs(xData)]

            if yData > 0:
                self.armState = [2, 0, abs(yData)]
            elif yData < 0:
                self.armState = [2, 1, abs(yData)] 
        
        else:
            self.armState = [7, 0, 0]
        '''elif pitchData:
            if pitchData > 0:
                self.armState = [3, 0, 127]
            else:
                self.armState = [3, 1, 127]

        elif buttons[1]:
            self.armState = [6, 0, 127]
        
        elif buttons[3]:
            self.armState = [6, 1, 127]
        '''
        
        print(self.armState)

    def serialWrite(self):
        #command = bytearray(self.driveState)
        command = bytearray(self.armState)
#        for i in self.armState:
#            command += chr(i)
        self.serial_port.write(command)
