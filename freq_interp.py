import numpy as np

class freqInterp:

    def freqInterp(dataIn):
        N = 2 * userN
        taps = userN + 1
        beta = 0.35 / 2.0
        over = 2.0
        dt = 1.0 / over
        gain = 1.0 - beta + 4.0 * beta / np.pi
        t = 0

        filt = np.zeros(N, dtype=np.complex64)

        for ii in range(((taps+1)/2 - 1),taps):
            tmp_tap = 0
            if (ii == ((taps + 1)/2 - 1)):
                tmp_tap = gain
            else:
                tmp_t = np.abs(t - 1.0 / 4.0 / beta)
                if (tmp_t < 1.0e-9):
                    sinarg = (1.0 + 2.0 / np.pi) * np.sin(np.pi / beta / 4.0)
                    cosarg = (1.0 - 2.0 / np.pi) * np.cos(np.pi / beta / 4.0)
                    tmp_tap = beta / np.sqrt(2.0 * (sinarg + cosarg))
                else:
                    sinarg = np.sin(np.pi * t * (1.0 - beta))
                    cosarg = 4.0 * beta * t * np.cos(np.pi * t * (1 + beta))
                    den = np.pi * t * (1.0 - 16.0 * (beta * t) * (beta * t))
                    tmp_tap = (sinarg + cosarg) / den
            indTop = ii
            indBot = taps - ii - 1
            filt[indTop] = tmp_tap
            if (ii > (taps + 1) / 2 - 1):
                filt[indBot] = tmp_tap
            t = t + dt
        sumFilt = 0
        for ii in range(N):
            sumFilt += filt[ii]
        for ii in range(N):
            filt[ii] /= sumFilt
    #    for ii in range(1,N,2):
    #        filt[ii] = -filt[ii]
        return filt
