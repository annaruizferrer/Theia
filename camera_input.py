import cv2
import time
from pythonosc import osc_message_builder
from pythonosc import udp_client

cap = cv2.VideoCapture(0)
original_width = cap.get(cv2.cv2.CAP_PROP_FRAME_WIDTH);
original_height = cap.get(cv2.cv2.CAP_PROP_FRAME_HEIGHT);

scale = 0.2

width = int(scale*original_width)
height = int(scale*original_height)

print ('Input size: ', width*height)

# start oscP5, listening for incoming messages at port 12000
client = udp_client.SimpleUDPClient("127.0.0.1", 6448)

while(True):
    # Capture frame-by-frame
    ret, frame = cap.read()

    # Rgb to gray
    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    
    # Down-scale image
    input = cv2.resize(
    	gray, (width, height), interpolation = cv2.INTER_CUBIC)
    
    # Display the resulting frame
    cv2.imshow('frame',input)

    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

    client.send_message(
        "/wek/inputs", input.reshape(int(width*height)).tolist())
    time.sleep(1)

# When everything done, release the capture
cap.release()
cv2.destroyAllWindows()