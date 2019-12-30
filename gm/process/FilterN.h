#ifndef _GM_PROCESS_FILTERN_H_
#define _GM_PROCESS_FILTERN_H_

#include <complex>

namespace gm {
namespace process {

class FilterN {
public:
	FilterN(int myN);
	~FilterN();
	float doAve(float val);
private:
    int N;
    std::complex<float> perc;
    std::complex<float> operc;
    double phase;
    std::complex<float> ave;
};

}
}

#endif //_GM_PROCESS_FILTERN_H_
