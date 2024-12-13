import cv2

camera = cv2.VideoCapture("rtsp://admin:ABCdef123@192.168.1.64:554/cam/realmonitor?channel=1&subtype=0")


width=640
height=320

camera.set(cv2.CAP_PROP_FRAME_HEIGHT,height)
camera.set(cv2.CAP_PROP_FRAME_WIDTH,width)
camera.set(cv2.CAP_PROP_FPS,30)


while(True):
    ignore, frame = camera.read()

    cv2.imshow('my WebCam',frame)   #shows the frame
    if cv2.waitKey(1)==ord('q'):
        break
    
camera.release()

