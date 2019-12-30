#ifndef PROCESSOR_MEM_H_
#define PROCESSOR_MEM_H_

#include "gm/tuner_mem.h"

#ifdef __cplusplus
extern "C" {
#endif

//#include "tuner_mem.h"

#define NTUNERS 60
#define TUNEBUFF 65536

typedef struct
{
  float    		  samples[TUNEBUFF][NTUNERS];
} BinHolder;

typedef struct
{
    char               version[STRINGSIZE];
    char               description[STRINGSIZE];
    char               lock[MAXLOCKSIZE];
    long               tuner_position;
    int				   buffer_size;
    int                num_tuners;
    BinHolder          bins;
} LogStruct;

#ifdef __cplusplus
}
#endif

#endif /* PROCESSOR_MEM_H_ */
