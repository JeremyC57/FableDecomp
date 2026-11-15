#ifndef FABLE_PASS22_CFLOATSHORTREVERBMODULE_H
#define FABLE_PASS22_CFLOATSHORTREVERBMODULE_H
#ifdef FABLE_RETAIL_PASS22_FLOATREVERB_CREATE
// Original GameCODA2.10 debug CodeView: complete data layout and used original API.
#include "engine/CFloatCyclicBuffer.h"
#include "engine/CFloatFilter.h"
#include "engine/CReverbDspProcessor.h"
class CFloatShortReverbModule : public CReverbDspProcessor {
private:
 CFloatFilter m_Filter; // +0x4
 CFloatCyclicBuffer m_Buffer; // +0x18
 int m_iMyFaderToken; // +0x24
public:
 virtual ~CFloatShortReverbModule();
};
#if defined(_MSC_VER) && defined(_M_IX86)
typedef char CFloatShortReverbModuleSizePass22[(sizeof(CFloatShortReverbModule)==40)?1:-1];
#endif
#endif // FABLE_RETAIL_PASS22_FLOATREVERB_CREATE
#endif
