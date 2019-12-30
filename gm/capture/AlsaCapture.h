#ifndef _GM_CAPTURE_ALSACAPTURE_H_
#define _GM_CAPTURE_ALSACAPTURE_H_


#include <alsa/asoundlib.h>
#include "gm/Thread.h"
#include "gm/shared.h"
#include "gm/tuner_mem.h"


namespace gm {
namespace capture {

class AlsaCapture : public Thread {

public:
	AlsaCapture();
	~AlsaCapture();
	void run() override;
private:
	void verify_struct();
};

}
}

#endif //_GM_CAPTURE_ALSACAPTURE_H_