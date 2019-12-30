#include <pybind11/pybind11.h>
#include <iostream>
#include "gm/CubicInterpolator.h"
#include "gm/RRCFilter.h"
#include "gm/QPSKGen.h"
#include "gm/capture/AlsaCapture.h"
#include "gm/capture/AlsaInterface.h"
#include "gm/process/LogProcessor.h"
#include "gm/Thread.h"
#include "gm/PythonModel.h"
#include "gm/tuner_mem.h"

namespace py = pybind11;

namespace gm{
    void print(std::string mystring) {
        py::gil_scoped_acquire acquire;
        py::print(mystring);
//        py::gil_scoped_release release;
    }
}

PYBIND11_MODULE(dsp_example, m) {

    m.doc() = "dsp example plugin";

    py::class_<gm::CubicInterpolator>(m, "CubicInterpolator")
        .def(py::init<>())
        .def("getRate", &gm::CubicInterpolator::getRate)
        .def("setRate", &gm::CubicInterpolator::setRate)
        .def("getMu", &gm::CubicInterpolator::getMu)
        .def("getLast", &gm::CubicInterpolator::getLast)
        .def("setMu", &gm::CubicInterpolator::setMu)
        .def("setIdxOffset", &gm::CubicInterpolator::setIdxOffset)
        .def("getOutputSize", &gm::CubicInterpolator::getOutputSize)
        .def("doInterpOut", &gm::CubicInterpolator::doInterpOut)
        .def("doInterpIn", &gm::CubicInterpolator::doInterpIn)
        .def("doInterpCenter", &gm::CubicInterpolator::doInterpCenter);

    py::class_<gm::RRCFilter>(m, "RRCFilter")
        .def(py::init<>())
        .def("calcFilter", &gm::RRCFilter::calcFilt);

    py::class_<gm::QPSKGen>(m, "QPSKGen")
        .def(py::init<int,int>())
        .def("getSymbols", &gm::QPSKGen::getSymbols)
        .def("setFilter", &gm::QPSKGen::setFilter);

    py::class_<gm::Runnable> gmrunnable(m, "Runnable");
    gmrunnable
        .def("setRunning", &gm::Runnable::setRunning);

    py::class_<gm::Thread> gmthread(m, "Thread", gmrunnable);
    gmthread
        .def("start", &gm::Thread::start)
        .def("join", &gm::Thread::join);
  
    py::class_<gm::capture::AlsaCapture>(m, "AlsaCapture", gmthread)
        .def(py::init<>());

    py::class_<gm::process::LogProcessor>(m, "LogProcessor", gmthread)
        .def(py::init<>());

    py::class_<RawTunerStruct>(m, "RawTunerStruct")
        .def_readwrite("current_sample", &RawTunerStruct::current_sample)
        .def_readwrite("sample_rate", &RawTunerStruct::sample_rate);


    py::class_<gm::capture::AlsaInterface>(m, "AlsaInterface")
        .def(py::init<>())
        .def("openTuner", &gm::capture::AlsaInterface::openTuner, py::return_value_policy::reference_internal)
        .def("getData", &gm::capture::AlsaInterface::getData);

}

