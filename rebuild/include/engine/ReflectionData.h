#ifndef FABLE_ENGINE_REFLECTIONDATA_H
#define FABLE_ENGINE_REFLECTIONDATA_H
// ReflectionData -- GameCODA 2.10 reverb: one early-reflection line's default tuning (CODAApiDbg.lib FloatReverbParameterTranslator.obj
// CodeView LF_STRUCTURE ReflectionData, SizeOf 20, five members as below). sc_DefaultReflectionData is the four-entry const table
// (CodeView LF_ARRAY of const ReflectionData, SizeOf 80; C symbol _sc_DefaultReflectionData, DIR32 in the CODAApi.lib release member
// ?UpdateReflectionsDelay@CFloatReverbParameterTranslator@@AAEXPAVCFloatReverbParameters@@PAVCHLRvbParams@@@Z +0x6f; retail
// 0x012BF0DC, the row reads fInputDelay[0] at 0x012BF0EC + 20 * i).
#include <stddef.h>
#include "rebuild_abi.h"
struct ReflectionData {
    float fOutputGain;          // +0x00
    float fFeedbackGain;        // +0x04
    float fMinFeedbackDelay;    // +0x08
    float fDelayOneSecond;      // +0x0c
    float fInputDelay[1];       // +0x10
};
FABLE_STATIC_ASSERT_AT(17, sizeof(ReflectionData) == 20);
FABLE_STATIC_ASSERT_AT(18, offsetof(ReflectionData, fDelayOneSecond) == 0x0c);
FABLE_STATIC_ASSERT_AT(19, offsetof(ReflectionData, fInputDelay) == 0x10);
extern "C" const ReflectionData sc_DefaultReflectionData[4];   // 0x012BF0DC
#endif
