#ifndef _GM_PROCESS_LOGPROCESSOR_H_
#define _GM_PROCESS_LOGPROCESSOR_H_

#include <complex>
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include "gm/process/LogProcessor.h"
#include "gm/process/FilterN.h"
#include "gm/Thread.h"
#include "gm/shared.h"
#include "gm/tuner_mem.h"
#include "gm/processor_mem.h"


namespace gm {
namespace process {

class LogProcessor : public Thread {

public:
	LogProcessor();
	~LogProcessor();
	void run() override;
private:
	FilterN *filters[NTUNERS];
};

}
}

#endif //_GM_PROCESS_LOGPROCESSOR_H_
