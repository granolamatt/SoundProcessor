#include <unistd.h>
#include <iostream>
#include "gm/Thread.h"
#include "gm/capture/AlsaCapture.h"


int main() {

	gm::capture::AlsaCapture alsa;

	alsa.start();

	while(true) {
		usleep(1000000);
	}

	return 0;
}



