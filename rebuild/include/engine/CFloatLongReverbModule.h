#ifndef FABLE_PASS22_CFLOATLONGREVERBMODULE_H
#define FABLE_PASS22_CFLOATLONGREVERBMODULE_H
#ifdef FABLE_RETAIL_PASS22_FLOATREVERB_CREATE
// Original GameCODA2.10 debug CodeView: complete data layout and used original API.
#include "engine/CFloatCyclicBuffer.h"
#include "engine/CFloatFilter.h"
#include "engine/CReverbDspProcessor.h"
class CFloatLongReverbModule : public CReverbDspProcessor {
private:
 CFloatCyclicBuffer m_Buffer; // +0x4
 CFloatFilter m_Filter; // +0x10
 int m_iMyFaderToken; // +0x24
public:
 virtual ~CFloatLongReverbModule();
};
#if defined(_MSC_VER) && defined(_M_IX86)
typedef char CFloatLongReverbModuleSizePass22[(sizeof(CFloatLongReverbModule)==40)?1:-1];
#endif
#endif // FABLE_RETAIL_PASS22_FLOATREVERB_CREATE
#endif
