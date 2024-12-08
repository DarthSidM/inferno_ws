import serial
import serial.tools.list_ports

class SerialPortChecker():
    def __init__(self, baud_rate, timeout):
        self.baud_rate = baud_rate
        self.timeout = timeout

    def list_serial_ports(self):
        ports = serial.tools.list_ports.comports()
        return [port.device for port in ports if "USB" in port.description]

    def find_port(self, keyword):
        for port in self.list_serial_ports():
            with serial.Serial(port, self.baud_rate, timeout=self.timeout) as ser:
                print(f"Checking {port}...")
                ser.flushInput()  # Clear input buffer
                ser.flushOutput()  # Clear output buffer

                # Read data during the timeout period
                data = ser.readline().decode('utf-8').strip()
                if keyword.lower() in data.lower():
                    print(f"'{keyword}' detected on {port}!")
                    return port
        print("No port found.")
        exit()


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
    def setState(self, xData, yData):
        if xData > 0:
            self.state = [4, abs(xData)]
            status = f"Right\tPWM:{abs(xData)}"
        elif xData < 0:
            self.state = [3, abs(xData)]
            status = f"Left\tPWM:{abs(xData)}"
        elif yData > 0:
            self.state = [2, abs(yData)]
            status = f"Backward\tPWM:{abs(yData)}"
        elif yData < 0:
            self.state = [1, abs(yData)]
            status = f"Forward\tPWM:{abs(yData)}"
        else:
            self.state = [5, 0]
            status = "Stop"
        return "Drive State: "+status


class arm(control):
    def setState(self, xData, yData, pitchData, buttons):
        #Button is 0
        if xData > 0 and buttons[0] == 0:
            self.state = [4, 0, abs(xData)]
            status = f"Base\tDir: 0\tPWM: {abs(xData)}"
        elif xData < 0 and buttons[0] == 0:
            self.state = [4, 1, abs(xData)]
            status = f"Base\tDir: 1\tPWM: {abs(xData)}"

        elif yData > 0 and buttons[0] == 0:
            self.state = [1, 0, abs(yData)]
            status = f"Actuator1\tDir: 0\tPWM: {abs(yData)}"
        elif yData < 0 and buttons[0] == 0:
            self.state = [1, 1, abs(yData)]
            status = f"Actuator1\tDir: 1\tPWM: {abs(yData)}"


        #Button is 1
        elif xData > 0 and buttons[0] == 1:
            self.state = [5, 0, abs(xData)]
            status = f"Roll\tDir: 0\tPWM: {abs(xData)}"
        elif xData < 0 and buttons[0] == 1:
            self.state = [5, 1, abs(xData)]
            status = f"Roll\tDir: 1\tPWM: {abs(xData)}"
        elif yData > 0 and buttons[0] == 1:
            self.state = [2, 0, abs(yData)]
            status = f"Actuator2\tDir: 0\tPWM: {abs(yData)}"
        elif yData < 0 and buttons[0] == 1:
            self.state = [2, 1, abs(yData)]
            status = f"Actuator2\tDir: 1\tPWM: {abs(yData)}"
        

        #Pitch
        elif pitchData:
            if pitchData > 0:
                self.state = [3, 0, 127]
                status = f"Pitch\tDir: 0\tPWM: 127"
            else:
                self.state = [3, 1, 127]
                status = f"Pitch\tDir: 1\tPWM: 127"
        
        #Gripper
        elif buttons[1]:
            self.state = [6, 0, 255]
            status = f"Gripper\tDir: 0\tPWM: 255"
        elif buttons[3]:
            self.state = [6, 1, 255]
            status = f"Gripper\tDir: 1\tPWM: 255"
        
        else:
            self.state = [7, 0, 0]
            status = "Stop"
        
        return "Arm State: " + status
