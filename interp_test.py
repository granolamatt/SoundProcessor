import time
import numpy as np
import matplotlib.pyplot as plt
import dsp_example
import rrc_filter


D=64
N=1024

#plt.figure(0)

filt = rrc_filter.calcFilter(D)

midfilt = np.zeros(2*D, dtype=np.complex64)
for ii in range(0,2*D,2):
   midfilt[ii] = filt[ii]
for ii in range(1,2*D,2):
   midfilt[ii] = -filt[ii]
f = np.fft.fft(midfilt)

#plt.subplot(2,1,1)
#plt.plot(np.real(np.abs(f)))
#plt.subplot(2,1,2)
#plt.plot(np.real(filt))

qpsk = dsp_example.QPSKGen(N, D)
qpsk.setFilter(filt)
qpsk_in = np.zeros(D, dtype=np.complex64)

qpsk_in[0] = 1 + 1j
qpsk_in[31] = -1 - 1j

qpsk_out = qpsk.getSymbols(qpsk_in)

#plt.figure(1)
#plt.plot(np.real(qpsk_out))

first = qpsk_out[0:200]

#First get an interp object
interp = dsp_example.CubicInterpolator()

# for 1.5
#interp.setRate(1.5)
#interp.setMu(-0.75)

# for 1.0
interp.setRate(1.5)

#val = interp.doInterpIn(first)
#print "last is ", interp.getLast()
#second = qpsk_out[interp.getLast():interp.getLast()+200]
#val2 = interp.doInterpIn(second)
#val = np.concatenate((val, val2), axis=0)

N = 2000
n = np.array(range(N), dtype=np.complex64)
#sig = np.sign(-np.cos(2*np.pi*n/N) - 0.5)
sig = np.zeros(N, dtype=np.complex64)
sig[N/2 + 4] = 1
#sig = (-np.cos(2*np.pi*n/N) - 0.5)
#isig = np.empty_like(sig)
isig = interp.doInterpCenter(sig)
f = np.fft.ifft(isig)
for ii in range(1,N,2):
   f[ii] = -f[ii]

plt.subplot(2,1,1)
plt.plot(np.real(f))
plt.subplot(2,1,2)
plt.plot(np.real(isig))
plt.show()

print(sig)
print(isig)

