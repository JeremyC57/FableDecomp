#ifndef FABLE_ENGINE_CODA_BUFFER_DESC_H
#define FABLE_ENGINE_CODA_BUFFER_DESC_H
#ifdef FABLE_RETAIL_CODA_OPENAL_BUFFER_PASS26
#include "rebuild_abi.h"
struct _CBUFFERDESC {
    unsigned int ffFormat;
    unsigned int nChannels;
    unsigned int nSamplesPerSec;
    unsigned int cbSize;
    unsigned int cbLoopStart;
    unsigned int cbLoopEnd;
    void* pvData;
};
FABLE_STATIC_ASSERT_AT(14, sizeof(_CBUFFERDESC) == 0x1C);
#endif
#endif
