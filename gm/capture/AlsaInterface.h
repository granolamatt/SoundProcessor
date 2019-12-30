#ifndef _GM_CAPTURE_ALSAINTERFACE_H_
#define _GM_CAPTURE_ALSAINTERFACE_H_

#include <complex>
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include "gm/shared.h"
#include "gm/tuner_mem.h"

namespace py = pybind11;

namespace gm {
namespace capture {

class AlsaInterface {

public:
	AlsaInterface();
	~AlsaInterface();
	// py::object openTuner();
	// long getPosition(py::object tuner);
	RawTunerStruct* openTuner();
	py::array_t<short, py::array::c_style> getData(RawTunerStruct* tuner);

	//long getPosition(RawTunerStruct* tuner);
private:
	
};

}
}

#endif //_GM_CAPTURE_ALSAINTERFACE_H_