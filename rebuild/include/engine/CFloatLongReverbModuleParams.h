#ifndef FABLE_PASS22_CFLOATLONGREVERBMODULEPARAMS_H
#define FABLE_PASS22_CFLOATLONGREVERBMODULEPARAMS_H
#ifdef FABLE_RETAIL_PASS22_FLOATREVERB_CREATE
// Original GameCODA2.10 debug CodeView: complete data layout and used original API.
#include "engine/CFloatFilterParams.h"
struct CFloatLongReverbModuleParams {
public:
 unsigned int m_auiInputDelay[4]; // +0x0
 float m_fInputGain; // +0x10
 float m_fCrossfeedGain; // +0x14
 CFloatFilterParams m_fFeedbackFilter; // +0x18
 unsigned int m_uiFeedbackDelay; // +0x34
 float m_fOutputGain; // +0x38
};
#if defined(_MSC_VER) && defined(_M_IX86)
typedef char CFloatLongReverbModuleParamsSizePass22[(sizeof(CFloatLongReverbModuleParams)==60)?1:-1];
#endif
#endif // FABLE_RETAIL_PASS22_FLOATREVERB_CREATE
#endif
