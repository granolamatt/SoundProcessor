#include <iostream>
#include <complex>
#include <math.h>
#include "gm/CubicInterpolator.h"

namespace py = pybind11;

namespace gm {

CubicInterpolator::CubicInterpolator() :
    rate(1), last(0), mu(0), idx_offset(0) {
    //std::cout << "Constructor was called" << std::endl;
}

CubicInterpolator::~CubicInterpolator() {
    //std::cout << "Destructor was called" << std::endl;
}

/*
*   does the interpolation to get outputSize points. Input buffer size must be
*   greater or equal to (int) floor(rate * outputSize + mu) + 3
*/
int CubicInterpolator::doInterp(int outputSize, std::complex<float>* dataIn, std::complex<float>* dataOut) {

    last = (int) floor(rate * outputSize + mu);

    for (int cnt = 0; cnt < outputSize; cnt++) {
        const int ii = (int) floor(rate * cnt + mu);
        const double d_mu_calc = (rate * cnt + mu) - ii;
        const int pos = ii;
        const float mu_l = (float) (d_mu_calc);
        const float mu2_l = mu_l * mu_l;

        const float yi0 = dataIn[pos].imag();
        const float yi1 = dataIn[pos + 1].imag();
        const float yi2 = dataIn[pos + 2].imag();
        const float yi3 = dataIn[pos + 3].imag();
        const float yr0 = dataIn[pos].real();
        const float yr1 = dataIn[pos + 1].real();
        const float yr2 = dataIn[pos + 2].real();
        const float yr3 = dataIn[pos + 3].real();

        const float ai0 = yi3 - yi2 - yi0 + yi1;
        const float ai1 = yi0 - yi1 - ai0;
        const float ai2 = yi2 - yi0;
        const float ai3 = yi1;

        const float ar0 = yr3 - yr2 - yr0 + yr1;
        const float ar1 = yr0 - yr1 - ar0;
        const float ar2 = yr2 - yr0;
        const float ar3 = yr1;

        const float aReal = ar0 * mu_l * mu2_l + ar1 * mu2_l + ar2 * mu_l + ar3;
        const float aImag = ai0 * mu_l * mu2_l + ai1 * mu2_l + ai2 * mu_l + ai3;

        dataOut[cnt] = std::complex<float>(aReal, aImag);
    }


    mu = rate * outputSize + mu - last;

    return last;
}

/*
*   does the interpolation to get outputSize points. Input buffer size must be
*   greater or equal to (int) floor(rate * outputSize + mu) + 3
*/
int CubicInterpolator::doInterpOut(py::array_t<std::complex<float>> dIn, py::array_t<std::complex<float>> dOut) {
    auto buf1 = dIn.request(), buf2 = dOut.request();

    if (buf1.ndim != 1 || buf2.ndim != 1)
        throw std::runtime_error("Number of dimensions must be one");


    int outputSize = buf2.size;

    std::complex<float> *dataIn = (std::complex<float> *) buf1.ptr,
                         *dataOut = (std::complex<float> *) buf2.ptr;

    int mylast = (int) floor(getRate() * outputSize + getMu());

    if (buf1.size < mylast)
        throw std::runtime_error("Not enough input points");

    return doInterp(outputSize, dataIn, dataOut);
}

/*
*   does the interpolation to get outputSize points. Input buffer size must be
*   greater or equal to (int) floor(rate * outputSize + mu) + 3
*/
py::array_t<std::complex<float>> CubicInterpolator::doInterpIn(py::array_t<std::complex<float>> dIn) {
    auto buf1 = dIn.request();

    int outputSize = (int)floor((buf1.size - 4.0 - getMu()) / getRate());

    /* No pointer is passed, so NumPy will allocate the buffer */
    auto result = py::array_t<std::complex<float>>(outputSize);
    auto buf2 = result.request();

    std::complex<float> *dataIn = (std::complex<float> *) buf1.ptr,
                         *dataOut = (std::complex<float> *) buf2.ptr;

    doInterp(outputSize, dataIn, dataOut);

    return result;
}

/*
*   does the interpolation to get outputSize points. Input buffer size must be
*   greater or equal to (int) floor(rate * outputSize + mu) + 3
*/
py::array_t<std::complex<float>> CubicInterpolator::doInterpCenter(py::array_t<std::complex<float>> dIn) {
    auto buf1 = dIn.request();


    int retLength = buf1.size;


    // trueStart is actually the point we want to focus on
    // this can be used for pictures too and center on a spot
    double trueStart = (double)retLength / 2.0;
    int idxoff = 0;
    double mu_off = -((trueStart - idxoff) * (rate - 1.0)) + (idxoff - 1);
    int startOff = 0;
    if (rate < 1) {
        while (mu_off > 2.0) {
            idxoff--;
            mu_off = -((trueStart - idxoff) * (rate - 1.0)) + (idxoff - 1);
        }
    } else {
        while (mu_off < -1.0) {
            idxoff++;
            mu_off = -((trueStart - idxoff) * (rate - 1.0)) + (idxoff - 1);
        }
    }
    mu = mu_off;

    int outputSize = (int)floor((buf1.size - 4.0 - getMu()) / getRate());
    int copyLength = outputSize;
    if (rate < 1) {
        startOff = -idxoff;
        idxoff = 0;
        copyLength = retLength;
    }


    std::cout << "N/2+1 " << trueStart << " offset " << idxoff << " mu " << mu << " copyLength " << copyLength << std::endl;

    /* No pointer is passed, so NumPy will allocate the buffer */
    auto result = py::array_t<std::complex<float>>(retLength);
    auto buf2 = result.request();

    // for now double copy
    std::complex<float>* dataHold = new std::complex<float>[outputSize];
    std::complex<float>* dataIn = (std::complex<float> *) buf1.ptr;
    std::complex<float>* dataOut = (std::complex<float> *) buf2.ptr;

    doInterp(outputSize, dataIn, dataHold);
    for (int cnt = 0; cnt < idxoff; cnt++) {
        dataOut[cnt] = 0;
    }
    std::copy(&dataHold[startOff], &dataHold[startOff] + copyLength, &dataOut[idxoff]);
    for (int cnt = copyLength; cnt < retLength - idxoff; cnt++) {
        dataOut[cnt + idxoff] = 0;
    }

    return result;
}

}

