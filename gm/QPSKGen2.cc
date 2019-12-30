#include <iostream>
#include <complex>
#include <fftw3.h>
#include <pybind11/numpy.h>
#include "gm/QPSKGen.h"
#include "gm/RRCFilter.h"

namespace gm {

QPSKGen::QPSKGen(int N_l, int D_l) :
	N(N_l), D(D_l) {
		symbols = new std::complex<float>[2*D];
		filter = new std::complex<float>[2*D];
		freqs = new std::complex<float>[N];
		zeros = new std::complex<float>[N];
		plan = fftwf_plan_dft_1d(2*D, reinterpret_cast<fftwf_complex*>(symbols), 
			reinterpret_cast<fftwf_complex*>(symbols), FFTW_FORWARD, FFTW_ESTIMATE);
		iplan = fftwf_plan_dft_1d(N, reinterpret_cast<fftwf_complex*>(freqs), 
			reinterpret_cast<fftwf_complex*>(freqs), FFTW_BACKWARD, FFTW_ESTIMATE);

		RRCFilter rrc;
		py::array_t<std::complex<float>> filt_data = rrc.calcFilt(D);
		auto bufOut = filt_data.request();
		std::complex<float> *dataOut = (std::complex<float> *) bufOut.ptr;
		std::copy(dataOut, dataOut+2*D, symbols);
	    for (int cnt = 1; cnt < 2*D; cnt+=2) {
	    	symbols[cnt] = -symbols[cnt];
	    }
		fftwf_execute(plan);
		std::copy(symbols, symbols+2*D, filter);
		
}

QPSKGen::~QPSKGen() {
	fftwf_destroy_plan(plan);
	fftwf_destroy_plan(iplan);
	delete freqs;
	delete symbols;
	delete zeros;
	delete filter;
}

void QPSKGen::setFilter(pybind11::array_t<std::complex<float>> dataIn) {
	auto bufIn = dataIn.request();
    if (bufIn.size != 2*D) {
    	throw std::runtime_error("Size of filter must equal 2*D");
    }
    std::complex<float> *dIn = (std::complex<float> *) bufIn.ptr;
    std::copy(dIn, dIn+2*D, symbols);
    for (int cnt = 1; cnt < 2*D; cnt+=2) {
    	symbols[cnt] = -symbols[cnt];
    }
	fftwf_execute(plan);
	std::copy(symbols, symbols+2*D, filter);
}

pybind11::array_t<std::complex<float>> QPSKGen::getSymbols(pybind11::array_t<std::complex<float>> dataIn) {
    auto bufIn = dataIn.request();

    if (bufIn.size != D) {
    	throw std::runtime_error("Number of input symbols must equal D");
    }

    /* No pointer is passed, so NumPy will allocate the buffer */
    auto result = pybind11::array_t<std::complex<float>>(N);
    auto bufOut = result.request();
    
    std::complex<float> *dIn = (std::complex<float> *) bufIn.ptr;
    std::complex<float> *dataOut = (std::complex<float> *) bufOut.ptr;

    for (int cnt = 0; cnt < D; cnt++) {
    	symbols[2*cnt] = dIn[cnt];
    	symbols[2*cnt + 1] = 0;
    }

    fftwf_execute(plan);
    for (int cnt = 0; cnt < 2*D; cnt++) {
    	symbols[cnt] *= filter[cnt];
    }

    int offset = (N - 2*D) / 2;

    std::copy(zeros, zeros+N, freqs);
    std::copy(symbols, symbols+2*D, freqs+offset);

    fftwf_execute(iplan);
    for (int cnt = 1; cnt < N;cnt+=2) {
    	freqs[cnt] = -freqs[cnt];
    }
    std::copy(freqs, freqs+N, dataOut);

    return result;
}

}
