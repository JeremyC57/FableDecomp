#ifndef FABLE_CSALSW3DVOICE_H
#define FABLE_CSALSW3DVOICE_H
#include "engine/CSalSwVoice.h"
class CCoda3DSource;
// Original public method and base chain. Unused derived tail is not projected here.
class CSalSw3DVoice:public CSalSwVoice {
public: void CalcParams(const CCoda3DSource*,unsigned int);
};
#endif
