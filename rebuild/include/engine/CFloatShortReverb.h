#ifndef FABLE_PASS22_CFLOATSHORTREVERB_H
#define FABLE_PASS22_CFLOATSHORTREVERB_H
#ifdef FABLE_RETAIL_PASS22_FLOATREVERB_CREATE
// Original GameCODA2.10 debug CodeView: complete data layout and used original API.
#include "engine/CFloatShortReverbModule.h"
#include "engine/CReverbDspProcessor.h"
class CFloatShortReverb : public CReverbDspProcessor {
private:
 CFloatShortReverbModule m_shortReverbModule[4]; // +0x4
 unsigned int m_uiNumOutputs; // +0xa4
 int m_iMyFaderTokenMin; // +0xa8
 int m_iMyFaderTokenMax; // +0xac
public:
 virtual ~CFloatShortReverb();
};
#if defined(_MSC_VER) && defined(_M_IX86)
typedef char CFloatShortReverbSizePass22[(sizeof(CFloatShortReverb)==176)?1:-1];
#endif
#endif // FABLE_RETAIL_PASS22_FLOATREVERB_CREATE
#endif
