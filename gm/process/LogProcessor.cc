#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <complex>
#include "gm/shared.h"
#include "gm/tuner_mem.h"
#include "gm/processor_mem.h"
#include "gm/process/LogProcessor.h"
#include "gm/PythonModel.h"

namespace gm {
namespace process {

LogProcessor::LogProcessor() {
    for (int cnt = 0; cnt < NTUNERS; cnt++) {
        filters[cnt] = NULL;
    }
}

LogProcessor::~LogProcessor() {

}

void LogProcessor::run() {

	setRunning(true);

	RawTunerStruct *tuner_struct;
	long memSize = get_shared_memory_size("tuner_output");
    if (memSize != sizeof(RawTunerStruct))
    {
        printf("Tuner Struct is not correct, cannot map memory\n");
        exit(3);
    }
    printf("Opening %s for input and %s for output\n", "tuner_output", "log_output");
    tuner_struct = (RawTunerStruct *)map_shared_memory("tuner_output", memSize);
	long now = tuner_struct->current_sample;
    long next = now + tuner_struct->sample_rate;
    LogStruct* log_data = (LogStruct*)map_shared_memory("log_output", sizeof(LogStruct));
    memset(log_data, 0, sizeof(LogStruct));
    log_data->buffer_size = TUNEBUFF;
    log_data->num_tuners = NTUNERS;

    for (int cnt = 0; cnt < NTUNERS; cnt++) {
        double fm = pow(2, cnt / (12.0 * 8)) * 55.0;
        int numSamples = (int) (tuner_struct->sample_rate / fm);
        filters[cnt] = new FilterN(numSamples * 8);
    }

    while (isRunning()) {
        long current = tuner_struct->current_sample;
        if (current == now) {
            usleep(100);
        } else {
            if ((current - now) > 45000) {
                // should throw an exception here
                printf("Falling behind %d now %ld current %ld diff %ld!!\n", log_data->num_tuners, now, current, (current - now));
                //now = current;
            }
        	while(now < current) {
                int dma_pos = now % tuner_struct->buffer_size;
                int proc_pos = now % log_data->buffer_size;
                std::complex<short>* sample = (std::complex<short>*) &tuner_struct->sample_holder.samples[dma_pos];
                for (int cnt = 0; cnt < log_data->num_tuners; cnt++) {
                     float ave = filters[cnt]->doAve(sample->real());
                     log_data->bins.samples[proc_pos][cnt] = ave;
                }
                now++;
            }
        	log_data->tuner_position = current;
        }
    }

    setRunning(false);
    for (int cnt = 0; cnt < NTUNERS; cnt++) {
        if (filters[cnt])
            delete(filters[cnt]);
        filters[cnt] = NULL;
    }
    unmap_shared_memory((char *)log_data, sizeof(LogStruct));
}


}
}
