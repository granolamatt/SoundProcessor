#include <math.h>
#include "gm/process/FilterN.h"

namespace gm{
namespace process {

	FilterN::FilterN(int myN) :
	    N(myN) {
        perc = std::complex<float>(1.0 / (N * 1.44269375), 0);
        operc = std::complex<float>(1.0 - 1.0 / (N * 1.44269375), 0);
	}

	FilterN::~FilterN() {
	}

    float FilterN::doAve(float val) {
        // 8 is the number corresponding to tests before
        phase -= -2.0 * 8.0 * M_PI / (double) N;
        while (phase < 0) {
            phase += 2.0 * M_PI;
        }
        std::complex<float> exp = std::complex<float>(cosf(phase), sinf(phase));
        ave = ave*operc + (val*perc*exp);
        return abs(ave) / (float)N;
    }

}
}