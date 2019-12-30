/*
A Minimal Capture Program
This program opens an audio interface for capture, configures it for
stereo, 16 bit, 44.1kHz, interleaved conventional read/write
access. Then its reads a chunk of random data from it, and exits. It
isn't meant to be a real program.
From on Paul David's tutorial : http://equalarea.com/paul/alsa-audio.html
Fixes rate and buffer problems
sudo apt-get install libasound2-dev
gcc -o alsa-record-example -lasound alsa-record-example.c && ./alsa-record-example hw:0
*/
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <alsa/asoundlib.h>
#include "gm/shared.h"
#include "gm/tuner_mem.h"
#include "gm/capture/AlsaCapture.h"

namespace gm {
namespace capture {

AlsaCapture::AlsaCapture() {
    setRun([&]{this->run();});
}

AlsaCapture::~AlsaCapture() {

}

void AlsaCapture::verify_struct() {
	RawTunerStruct vs;

	printf("version[STRINGSIZE]; %ld \n", -((long)&vs - (long)&vs.version[0]));
	printf("description[STRINGSIZE]; %ld \n", -((long)&vs - (long)&vs.description[0]));
	printf("lock[MAXLOCKSIZE]; %ld \n", -((long)&vs - (long)&vs.lock[0]));
	printf("buffer_size; %ld \n", -((long)&vs - (long)&vs.buffer_size));
	printf("frequency; %ld \n", -((long)&vs - (long)&vs.frequency));
   printf("sample_rate; %ld \n", -((long)&vs - (long)&vs.sample_rate));
	printf("current_sample; %ld \n", -((long)&vs - (long)&vs.current_sample));
	printf("sample_holder; %ld \n", -((long)&vs - (long)&vs.sample_holder));
		
}

void AlsaCapture::run() {
    //gm::print(std::string("Starting the thread"));
    printf("Starting the run\n");
    setRunning(true);
    int i;
    int err;
    char *buffer;
    ComplexShort* buffer_type;
    int buffer_frames = 128;
    unsigned int rate = 44100;
    snd_pcm_t *capture_handle;
    snd_pcm_hw_params_t *hw_params;
    snd_pcm_format_t format = SND_PCM_FORMAT_S16_LE;
    RawTunerStruct* tuner_data;
    int cnt;
    long position;
    ComplexShort* mem_base; // Calling it complex short though real to eliminate right channel
    
     char hwname[16384];

     // if (argc > 1) {
     //    strncpy(hwname, argv[1], 16384);
     // } else {
        sprintf(hwname,"hw:0");
     // }


    if ((err = snd_pcm_open (&capture_handle, hwname, SND_PCM_STREAM_CAPTURE, 0)) < 0) {
        fprintf (stderr, "cannot open audio device %s (%s)\n",
                 hwname,
                 snd_strerror (err));
        exit (1);
    }

    fprintf(stdout, "audio interface opened\n");
    if ((err = snd_pcm_hw_params_malloc (&hw_params)) < 0) {
        fprintf (stderr, "cannot allocate hardware parameter structure (%s)\n",
                 snd_strerror (err));
        exit (1);
    }

    fprintf(stdout, "hw_params allocated\n");
    if ((err = snd_pcm_hw_params_any (capture_handle, hw_params)) < 0) {
        fprintf (stderr, "cannot initialize hardware parameter structure (%s)\n",
                 snd_strerror (err));
        exit (1);
    }

    fprintf(stdout, "hw_params initialized\n");
    if ((err = snd_pcm_hw_params_set_access (capture_handle, hw_params, SND_PCM_ACCESS_RW_INTERLEAVED)) < 0) {
//    if ((err = snd_pcm_hw_params_set_access (capture_handle, hw_params, SND_PCM_ACCESS_RW_NONINTERLEAVED)) < 0) {
        fprintf (stderr, "cannot set access type (%s)\n",
                 snd_strerror (err));
        exit (1);
    }

    fprintf(stdout, "hw_params access setted\n");
    if ((err = snd_pcm_hw_params_set_format (capture_handle, hw_params, format)) < 0) {
        fprintf (stderr, "cannot set sample format (%s)\n",
                 snd_strerror (err));
        exit (1);
    }

    fprintf(stdout, "hw_params format setted\n");
    if ((err = snd_pcm_hw_params_set_rate_near (capture_handle, hw_params, &rate, 0)) < 0) {
        fprintf (stderr, "cannot set sample rate (%s)\n",
                 snd_strerror (err));
        exit (1);
    }
    fprintf(stdout, "hw_params rate setted\n");

    if ((err = snd_pcm_hw_params_set_channels (capture_handle, hw_params, 2)) < 0) {
        fprintf (stderr, "cannot set channel count (%s)\n",
                 snd_strerror (err));
        exit (1);
    }

    fprintf(stdout, "hw_params channels setted\n");
    if ((err = snd_pcm_hw_params (capture_handle, hw_params)) < 0) {
        fprintf (stderr, "cannot set parameters (%s)\n",
                 snd_strerror (err));
        exit (1);
    }

    fprintf(stdout, "hw_params setted\n");
    snd_pcm_hw_params_free (hw_params);

    fprintf(stdout, "hw_params freed\n");
    if ((err = snd_pcm_prepare (capture_handle)) < 0) {
        fprintf (stderr, "cannot prepare audio interface for use (%s)\n",
                 snd_strerror (err));
        exit (1);
    }

    fprintf(stdout, "audio interface prepared\n");
    buffer = (char*)malloc(2 * buffer_frames * snd_pcm_format_width(format) / 8);
    buffer_type = (ComplexShort*)buffer;

    fprintf(stdout, "buffer allocated width is %d \n", snd_pcm_format_width(format));

    //gm::print(std::string("Mapping memory to tuner_output"));
    tuner_data = (RawTunerStruct*)map_shared_memory("tuner_output", sizeof(RawTunerStruct));
    printf("Created struct %lu \n", sizeof(RawTunerStruct));
    
	 tuner_data->buffer_size = BUFFERSIZE;  // in samples
	 tuner_data->frequency = 0;
    tuner_data->sample_rate = 44100;
	 tuner_data->current_sample = 0;
	 for (cnt = 0; cnt < BUFFERSIZE; cnt++) {
		tuner_data->sample_holder.samples[cnt].x = (short)3;
		tuner_data->sample_holder.samples[cnt].y = (short)4;
	 }
	 mem_base = tuner_data->sample_holder.samples;	 
	 position = 0;
	 
    //printf("verify struct\n");
	//verify_struct();

    while (isRunning()) {
        if ((err = snd_pcm_readi (capture_handle, buffer, buffer_frames)) != buffer_frames) {
            fprintf (stderr, "read from audio interface failed (%s)\n",
                      snd_strerror (err));
            exit (1);
        }
     //    for (i = 0; i < buffer_frames; i++) {
     //        // Make the left channel imaginary and set it to zero
     //        buffer_type[i].y = (buffer_type[i].x < 0) ? -1 : 0;
	    // }
	    memcpy(&mem_base[(int)(position % BUFFERSIZE)], buffer, buffer_frames*sizeof(int));
	    position += (long)buffer_frames;
        tuner_data->current_sample = position;
        //fprintf(stdout, "read %ld done size %d frames %d\n", position, BUFFERSIZE, buffer_frames);
    }

    free(buffer);
    setRunning(false);

    std::cout << "audio interface closed" << std::endl;
//    gm::print(std::string("buffer freed"));
    snd_pcm_close (capture_handle);
//    gm::print(std::string("audio interface closed"));
    unmap_shared_memory((char *)tuner_data, sizeof(RawTunerStruct));
}

}
}
