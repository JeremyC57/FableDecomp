#ifndef FABLE_PASS22_CFLOATECHO_H
#define FABLE_PASS22_CFLOATECHO_H
#ifdef FABLE_RETAIL_PASS22_FLOATREVERB_CREATE
// Original GameCODA2.10 debug CodeView: complete data layout and used original API.
#include "engine/CFloatCyclicBuffer.h"
#include "engine/CReverbDspProcessor.h"
class CFloatEcho : public CReverbDspProcessor {
public:
 virtual ~CFloatEcho();
private:
 CFloatCyclicBuffer m_buffer; // +0x4
 int m_iMyFaderToken; // +0x10
};
#if defined(_MSC_VER) && defined(_M_IX86)
typedef char CFloatEchoSizePass22[(sizeof(CFloatEcho)==20)?1:-1];
#endif
#endif // FABLE_RETAIL_PASS22_FLOATREVERB_CREATE
#endif
