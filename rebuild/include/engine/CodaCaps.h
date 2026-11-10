#ifndef FABLE_CODA_CAPS_H
#define FABLE_CODA_CAPS_H
#include "engine/CODA_PlatformTypes.h"
struct _CSWMIXERCAPS {
 unsigned int nSamplesPerSec;
 unsigned int nMinFrameSamples;
 unsigned int nFrameStepSamples;
 unsigned int nFrameSamples;
 unsigned int ffOutputFormat;
};
struct _CVOICECAPS {
 int nVoices;
 int nFreeVoices;
 unsigned int ffCaps;
 unsigned int ffFormats;
 unsigned int nMinSamplesPerSec;
 unsigned int nMaxSamplesPerSec;
};
struct _CCAPS {
 _CVOICECAPS HW2DVoice;
 _CVOICECAPS HW3DVoice;
 _CVOICECAPS SW2DVoice;
 _CVOICECAPS SW3DVoice;
 _CSWMIXERCAPS SWMixer;
 unsigned int ffSystem;
 unsigned int ffOutputConfig;
 int nSpeakers;
 unsigned int ffSpeakers;
 int nHWAllVoices;
 int nHWFreeVoices;
 int nSWAllVoices;
 int nSWFreeVoices;
 int nHWReverbs;
 CREFTIME RefTimeTicksPerSec;
};
extern _CCAPS g_SalCaps;
#endif
