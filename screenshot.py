import mss
import timeit
import numpy as np
import cv2
import socket

IP = "10.100.102.100"
PORT = 1234

def numToStr(color:int):
	to_ret = str(color)
	while len(to_ret) != 3:
		to_ret = '0'+to_ret
	return to_ret

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((IP,PORT))
with mss.mss() as mss_instance:
	i = 0
	mon = 0
	for monitor in mss_instance.monitors:
		screenshot = np.array(mss_instance.grab(monitor))
		
		cv2.imshow(str(mon),screenshot)
		mon += 1
	cv2.waitKey(0)
	cv2.destroyAllWindows()
	choice = int(input("Enter screen of your choice: "))

	monitor = mss_instance.monitors[choice]

	start_time = timeit.default_timer()
	screenshot = np.array(mss_instance.grab(monitor))
	pix_am = screenshot.shape[0]*screenshot.shape[1]

	print("Starting loop")
	try:
		while 1:
			screenshot = np.array(mss_instance.grab(monitor))
			b = np.sum(screenshot[:,:,0])//pix_am
			g = np.sum(screenshot[:,:,1])//pix_am
			r = np.sum(screenshot[:,:,2])//pix_am
			to_send = numToStr(r)+numToStr(g)+numToStr(b)+';'
			s.sendall(to_send.encode())
			i+=1
	except:
		pass

	print('fps: ', i/(timeit.default_timer() - start_time))

input()