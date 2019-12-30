#ifndef _GM_FRACTIONAL_INTERP_H_
#define _GM_FRACTIONAL_INTERP_H_

#include <complex>
#include <math.h>
#include <pybind11/numpy.h>

namespace py = pybind11;

namespace gm {

class CubicInterpolator {
public:
	CubicInterpolator();
	~CubicInterpolator();
	/*
	*	return the current mu, percentage between the two points
	*/
	double getMu() {
		return mu;
	}
	/*
	*	sets the current mu, percentage between the two points
	*/
	void setMu(double d_mu) {
		mu = d_mu;
	}
	/*
	*	return the number of input points used for last run
	*	can also be calculated (int) floor(rate * outputSize + mu);
	*/
	int getLast() {
		return last;
	}
	/*
	*	return the number of input points required with set rate
	* 	and number of outputs requested (int) floor(rate * outputSize + mu);
	*	Input buffer must be this size + four
	*/
	int getOutputSize(int outputSize) {
		return (int) floor(rate * outputSize + mu);
	}
	/*
	*	return the number of output points required with set rate
	* 	and number of input requested (int)floor((inputSize - 4.0 - mu)/rate);
	*/
	int getInputSize(int inputSize) {
		return (int)floor((inputSize - 3.0 - mu) / rate);
	}
	/*
	*	sets the current rate, 0.5 is one output per two inputs
	*/
	void setRate(double d_rate) {
		rate = d_rate;
	}
	/*
	*	gets the current rate, 0.5 is one output per two inputs
	*/
	double getRate() {
		return rate;
	}

	void setIdxOffset(int off) {
		idx_offset = off;
	}
	/*
	*	does the interpolation to get outputSize points. Input buffer size must be
	*   greater or equal to (int) floor(rate * outputSize + mu) + 4
	*/
	int doInterp(int outputSize, std::complex<float>* dataIn, std::complex<float>* dataOut);
	/*
	*	does the interpolation to get outputSize points. Input buffer size must be
	*   greater or equal to (int) floor(rate * outputSize + mu) + 4
	*/
	int doInterpOut(py::array_t<std::complex<float>> dataIn, py::array_t<std::complex<float>> dataOut);
	/*
	*	does the interpolation on number Input points. Output buffer size is
	*    stored in last and is (int)floor((inputSize - 4.0 - mu)/rate);
	*/
	py::array_t<std::complex<float>> doInterpIn(py::array_t<std::complex<float>> dataIn);
	/*
	*	does the interpolation on number Input points. Output buffer size is
	*    stored in last and is (int)floor((inputSize - 4.0 - mu)/rate);
	*/
	py::array_t<std::complex<float>> doInterpCenter(py::array_t<std::complex<float>> dataIn);

private:
	double rate;   // rate to interpolate.  0.5 is 1 out per two in
	double mu;     // Fraction between to bits on input, this is adjusted as it goes
	int last;      // Number inputs we used last run, shift input buffer by this amount
	int idx_offset;
};

}

#endif //_GM_FRACTIONAL_INTERP_H_