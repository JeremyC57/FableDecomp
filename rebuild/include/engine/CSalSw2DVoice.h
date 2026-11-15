#ifndef FABLE_CSALSW2DVOICE_H
#define FABLE_CSALSW2DVOICE_H
#include "engine/CSalSwVoice.h"
class CCoda2DSource;
// Original public method and base chain. Unused derived tail is not projected here.
class CSalSw2DVoice:public CSalSwVoice {
public: void CalcParams(const CCoda2DSource*,unsigned int);
};
#endif
