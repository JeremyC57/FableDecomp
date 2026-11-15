#ifndef FABLE_PASS22_CFLOATPITCH_H
#define FABLE_PASS22_CFLOATPITCH_H
#ifdef FABLE_RETAIL_PASS22_FLOATREVERB_CREATE
// Original GameCODA2.10 debug CodeView: complete data layout and used original API.
#include "engine/CFloatCyclicBuffer.h"
#include "engine/CReverbDspProcessor.h"
class CFloatPitch : public CReverbDspProcessor {
public:
 virtual ~CFloatPitch();
private:
 CFloatCyclicBuffer m_Buffer; // +0x4
 float m_fControlPoint; // +0x10
 int m_iMyFaderToken; // +0x14
};
#if defined(_MSC_VER) && defined(_M_IX86)
typedef char CFloatPitchSizePass22[(sizeof(CFloatPitch)==24)?1:-1];
#endif
#endif // FABLE_RETAIL_PASS22_FLOATREVERB_CREATE
#endif
