import sys
sys.path.append('/home/matt/workarea/SoundProcessor/build')
import time
import dsp_example
#import ctypes
#libc = ctypes.CDLL(None)

alsa_capture = dsp_example.AlsaCapture()
alsa_capture.start()

time.sleep(2)
#log_proc = dsp_example.LogProcessor()
#log_proc.start()

print "hello"
ai = dsp_example.AlsaInterface()
mem = ai.openTuner()
data = ai.getData(mem)

# numz = 0;
# for cnt in range(1048576):
# 	if data[cnt] == 0:
# 		numz = numz + 1

# print "Zeros ", numz

while True:	
	pos = (mem.current_sample - 1000) % data.size
	print "data is ", data[pos] / 65536.0, " size ", data.size, " pos ", pos
	print "test ", mem.current_sample, " rate ", mem.sample_rate
	time.sleep(1)
