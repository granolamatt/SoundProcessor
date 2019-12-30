#ifndef _GM_QPSKGEN_H_
#define _GM_QPSKGEN_H_

#include <complex>
#include <fftw3.h>
#include <pybind11/numpy.h>

namespace gm {

class QPSKGen {

public:
	QPSKGen(int N_l, int D_l);
	~QPSKGen();
	void setFilter(pybind11::array_t<std::complex<float>> dataIn);
	pybind11::array_t<std::complex<float>> getSymbols(pybind11::array_t<std::complex<float>> dataIn);
private:
	int N;
	int D;
	fftwf_plan plan;
	fftwf_plan iplan;
	std::complex<float>* symbols;
	std::complex<float>* freqs;
	std::complex<float>* zeros;
	std::complex<float>* filter;
};

}

#endif //_GM_QPSKGEN_H_
