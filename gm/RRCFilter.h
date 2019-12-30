#ifndef _GM_RRCFILTER_H_
#define _GM_RRCFILTER_H_

#include <complex>
#include <math.h>
#include <pybind11/numpy.h>

namespace py = pybind11;

namespace gm {

class RRCFilter {
public:
	RRCFilter();
	~RRCFilter();
	py::array_t<std::complex<float>> calcFilt(int size);

private:

};

}


#endif //_GM_RRCFILTER_H_
