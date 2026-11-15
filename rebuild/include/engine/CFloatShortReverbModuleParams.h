#ifndef FABLE_PASS22_CFLOATSHORTREVERBMODULEPARAMS_H
#define FABLE_PASS22_CFLOATSHORTREVERBMODULEPARAMS_H
#ifdef FABLE_RETAIL_PASS22_FLOATREVERB_CREATE
// Original GameCODA2.10 debug CodeView: complete data layout and used original API.
#include "engine/CFloatFilterParams.h"
struct CFloatShortReverbModuleParams {
public:
 unsigned int m_auiInputDelay[2]; // +0x0
 float m_afInputGain[2]; // +0x8
 CFloatFilterParams m_fFilter; // +0x10
 float m_fOutputGain; // +0x2c
};
#if defined(_MSC_VER) && defined(_M_IX86)
typedef char CFloatShortReverbModuleParamsSizePass22[(sizeof(CFloatShortReverbModuleParams)==48)?1:-1];
#endif
#endif // FABLE_RETAIL_PASS22_FLOATREVERB_CREATE
#endif
