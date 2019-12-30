#include <unistd.h>
#include <iostream>
#include "gm/Thread.h"
#include "gm/process/LogProcessor.h"


int main() {

	gm::process::LogProcessor processor;

	processor.start();

	while(true) {
		usleep(1000000);
	}

	return 0;
}



