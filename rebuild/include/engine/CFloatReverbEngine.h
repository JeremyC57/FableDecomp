#ifndef FABLE_PASS22_CFLOATREVERBENGINE_H
#define FABLE_PASS22_CFLOATREVERBENGINE_H
#ifdef FABLE_RETAIL_PASS22_FLOATREVERB_CREATE
// Original GameCODA2.10 debug CodeView: complete data layout and used original API.
#include "engine/CFloatReverbConstants.h"
#include "engine/CFloatReverbDspEngine.h"
#include "engine/CFloatReverbParameterTranslator.h"
#include "engine/CFloatReverbParameters.h"
class CReverbConstants;
class CFloatReverbEngine {
public:
 bool Create(CReverbConstants *);
#ifdef FABLE_RETAIL_CFLOATREVERBENGINE_COMMIT
 // Retail opt-in (NF99P2, q11498): SDK 2.10 CodeView public ?Commit@CFloatReverbEngine@@QAEXPAVCHLRvbParams@@_N@Z (native 0x00C6AEF0;
 // CODAApi.lib FloatReverbEngine.obj member, 92 bytes, byte-identical with relocations masked).
 void Commit(class CHLRvbParams *,bool);
#endif
private:
 CFloatReverbParameters m_RvbParams; // +0x0
 CFloatReverbConstants m_RvbConsts; // +0x234
 CFloatReverbParameterTranslator m_Translator; // +0x260
 CFloatReverbDspEngine m_DspEngine; // +0x28c
 int m_iDecayTimeSamples; // +0x524
 int m_iSamplesSinceInputInvalid; // +0x528
};
#if defined(_MSC_VER) && defined(_M_IX86)
typedef char CFloatReverbEngineSizePass22[(sizeof(CFloatReverbEngine)==1324)?1:-1];
#endif
#endif // FABLE_RETAIL_PASS22_FLOATREVERB_CREATE
#endif
