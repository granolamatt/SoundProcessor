#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include "gm/shared.h"
#include "gm/tuner_mem.h"
#include "gm/capture/AlsaInterface.h"
#include "gm/PythonModel.h"

namespace py = pybind11;

namespace gm {
namespace capture {

AlsaInterface::AlsaInterface() {

}

AlsaInterface::~AlsaInterface() {

}

RawTunerStruct* AlsaInterface::openTuner() {
	RawTunerStruct* tuner_data = (RawTunerStruct*)map_shared_memory("tuner_output", sizeof(RawTunerStruct));
	return tuner_data;
}

py::array_t<short, py::array::c_style> AlsaInterface::getData(RawTunerStruct* tuner) {

	short* data = (short*)&tuner->sample_holder;
//	auto result = py::array_t<std::complex<float>>(BUFFERSIZE);

	return py::array(2*tuner->buffer_size, data);
//	return result;
}

// long AlsaInterface::getPosition(py::object tuner) {
// 	RawTunerStruct* tuner_data = tuner.cast<RawTunerStruct *>();
// 	return tuner_data->current_sample;
// }

// long AlsaInterface::getPosition(RawTunerStruct* tuner) {
// 	return tuner->current_sample;
// }


}
}
