#include <iostream>
#include <complex>
#include <math.h>
#include "gm/RRCFilter.h"

namespace gm {

RRCFilter::RRCFilter() {

}

RRCFilter::~RRCFilter() {
}

py::array_t<std::complex<float>> RRCFilter::calcFilt(int userN) {
	int N = 2 * userN;
	int taps = userN + 1;
	const double beta = 0.35 / 2.0;
	double over = 2.0;
	double dt = 1.0 / over;
	double gain = 1.0 - beta + 4.0 * beta / M_PI;
	double t = 0;
	double sinarg;
	double cosarg;
	double tmp_tap;
	int ii, cnt;
//	fftw_plan p;

	double* filt = new double[N];

	for (cnt = 0; cnt < N; cnt++) {
		filt[cnt] = 0;
	}

	for (ii = ((taps + 1) / 2) - 1; ii < taps; ii++) {
		if (ii == (taps + 1) / 2 - 1) {
			tmp_tap = gain;
		} else {
			double tmp_t = fabs(t - 1.0 / 4.0 / beta);
			if (tmp_t < 1.0e-9) {
				sinarg = (1.0 + 2.0 / M_PI) * sin(M_PI / beta / 4.0);
				cosarg = (1.0 - 2.0 / M_PI) * cos(M_PI / beta / 4.0);
				tmp_tap = beta / sqrt(2.0 * (sinarg + cosarg));
			} else {
				sinarg = sin(M_PI * t * (1.0 - beta));
				cosarg = 4.0 * beta * t * cos(M_PI * t * (1 + beta));
				double den = M_PI * t * (1.0 - 16.0 * (beta * t) * (beta * t));
				tmp_tap = (sinarg + cosarg) / den;
			}
		}
		int indTop = ii;
		int indBot = taps - ii - 1;
		filt[indTop] = tmp_tap;
		if (ii > (taps + 1) / 2 - 1) {
			filt[indBot] = tmp_tap;
		}
		t = t + dt;
	}
	double sumFilt = 0;
	for (ii = 0; ii < N; ii++) {
		sumFilt += filt[ii];
	}
	for (ii = 0; ii < N; ii++) {
		filt[ii] /= sumFilt;
	}

	auto result = py::array_t<std::complex<float>>(N);
	auto buf2 = result.request();
	std::complex<float> *calcFilt = (std::complex<float> *) buf2.ptr;

	for (cnt = 0; cnt < N; cnt++) {
		calcFilt[cnt] = std::complex<float>(filt[cnt], 0);
	}

	// for (cnt = 1; cnt < N; cnt += 2) {
	// 	calcFilt[cnt] = -calcFilt[cnt];
	// }
	// p = fftw_plan_dft_1d(N, calcFilt, calcFilt, FFTW_FORWARD, FFTW_ESTIMATE);
	// fftw_execute(p);
	// fftw_destroy_plan(p);
	// multEXPJ(calcFilt, N, 0.25);

	delete filt;

	return result;
}

}

