#ifndef FABLE_PASS22_CFLOATLONGREVERBPARAMS_H
#define FABLE_PASS22_CFLOATLONGREVERBPARAMS_H
#ifdef FABLE_RETAIL_PASS22_FLOATREVERB_CREATE
// Original GameCODA2.10 debug CodeView: complete data layout and used original API.
#include "engine/CFloatFilterParams.h"
#include "engine/CFloatLongReverbModuleParams.h"
struct CFloatLongReverbParams {
public:
 unsigned int m_uiInputFilterDelay[3]; // +0x0
 CFloatFilterParams m_aInputFilter[3]; // +0xc
 CFloatLongReverbModuleParams m_aLongRvb[2]; // +0x60
};
#if defined(_MSC_VER) && defined(_M_IX86)
typedef char CFloatLongReverbParamsSizePass22[(sizeof(CFloatLongReverbParams)==216)?1:-1];
#endif
#endif // FABLE_RETAIL_PASS22_FLOATREVERB_CREATE
#endif
