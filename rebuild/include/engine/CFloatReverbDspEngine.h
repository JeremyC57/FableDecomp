#ifndef FABLE_PASS22_CFLOATREVERBDSPENGINE_H
#define FABLE_PASS22_CFLOATREVERBDSPENGINE_H
#ifdef FABLE_RETAIL_PASS22_FLOATREVERB_CREATE
// Original GameCODA2.10 debug CodeView: complete data layout and used original API.
#include "engine/CFloatCyclicBuffer.h"
#include "engine/CFloatDiffuser.h"
#include "engine/CFloatEcho.h"
#include "engine/CFloatFilter.h"
#include "engine/CFloatLongReverb.h"
#include "engine/CFloatPitch.h"
#include "engine/CFloatReflection.h"
#include "engine/CFloatReverbConstants.h"
#include "engine/CFloatShortReverb.h"
#include "engine/CReverbDspEngine.h"
class CFloatReverbDspEngine : public CReverbDspEngine {
public:
 virtual ~CFloatReverbDspEngine();
 bool Create(CFloatReverbConstants *);
 virtual void Clear();
protected:
 virtual bool ProcessReverb(void *,bool,void * *,unsigned int);
private:
 CFloatReverbConstants m_ReverbConstants; // +0x28
 CFloatCyclicBuffer m_reflectionsInputBuffer; // +0x54
 CFloatCyclicBuffer m_reverbBuffer; // +0x60
 CFloatFilter m_inputFilterLowPass; // +0x6c
 CFloatFilter m_inputFilterHighPass; // +0x80
 CFloatShortReverb m_ShortReverb; // +0x94
 CFloatLongReverb m_LongReverb; // +0x144
 CFloatPitch m_Pitch; // +0x1e0
 CFloatReflection m_reflection[4]; // +0x1f8
 CFloatEcho m_echo; // +0x248
 CFloatDiffuser m_diffuser; // +0x25c
 void * m_pvData; // +0x280
 void * m_pvStaticData; // +0x284
 float * m_pfLongRvbXfeed; // +0x288
 bool m_bOldInputValid; // +0x28c
 int m_iInputFilterFaderToken; // +0x290
 bool m_bCreated; // +0x294
};
#if defined(_MSC_VER) && defined(_M_IX86)
typedef char CFloatReverbDspEngineSizePass22[(sizeof(CFloatReverbDspEngine)==664)?1:-1];
#endif
#endif // FABLE_RETAIL_PASS22_FLOATREVERB_CREATE
#endif
