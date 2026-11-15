#ifndef FABLE_PASS22_CFLOATREVERBPARAMETERS_H
#define FABLE_PASS22_CFLOATREVERBPARAMETERS_H
#ifdef FABLE_RETAIL_PASS22_FLOATREVERB_CREATE
// Original GameCODA2.10 debug CodeView: complete data layout and used original API.
#include "engine/CFloatEchoParams.h"
#include "engine/CFloatFilterParams.h"
#include "engine/CFloatLongReverbParams.h"
#include "engine/CFloatPitchShiftParams.h"
#include "engine/CFloatReflectionParams.h"
#include "engine/CFloatShortReverbParams.h"
class CFloatReverbParameters {
public:
 CFloatFilterParams m_inputFilterLowPass; // +0x0
 CFloatFilterParams m_inputFilterHighPass; // +0x1c
 CFloatReflectionParams m_Ref[4]; // +0x38
 CFloatShortReverbParams m_ShortReverb; // +0x88
 CFloatLongReverbParams m_LongReverb; // +0x148
 CFloatPitchShiftParams m_PitchShift; // +0x220
 CFloatEchoParams m_Echo; // +0x228
};
#if defined(_MSC_VER) && defined(_M_IX86)
typedef char CFloatReverbParametersSizePass22[(sizeof(CFloatReverbParameters)==564)?1:-1];
#endif
#endif // FABLE_RETAIL_PASS22_FLOATREVERB_CREATE
#endif
