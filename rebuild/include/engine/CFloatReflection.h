#ifndef FABLE_PASS22_CFLOATREFLECTION_H
#define FABLE_PASS22_CFLOATREFLECTION_H
#ifdef FABLE_RETAIL_PASS22_FLOATREVERB_CREATE
// Original GameCODA2.10 debug CodeView: complete data layout and used original API.
#include "engine/CFloatCyclicBuffer.h"
#include "engine/CReverbDspProcessor.h"
class CFloatReflection : public CReverbDspProcessor {
private:
 CFloatCyclicBuffer m_Buffer; // +0x4
 int m_iMyFaderToken; // +0x10
public:
 virtual ~CFloatReflection();
};
#if defined(_MSC_VER) && defined(_M_IX86)
typedef char CFloatReflectionSizePass22[(sizeof(CFloatReflection)==20)?1:-1];
#endif
#endif // FABLE_RETAIL_PASS22_FLOATREVERB_CREATE
#endif
