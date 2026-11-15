#ifndef FABLE_PASS22_CFLOATLONGREVERB_H
#define FABLE_PASS22_CFLOATLONGREVERB_H
#ifdef FABLE_RETAIL_PASS22_FLOATREVERB_CREATE
// Original GameCODA2.10 debug CodeView: complete data layout and used original API.
#include "engine/CFloatFilter.h"
#include "engine/CFloatLongReverbModule.h"
#include "engine/CReverbDspProcessor.h"
class CReverbConstants;
class CFloatLongReverb : public CReverbDspProcessor {
private:
 CFloatFilter m_filter[3]; // +0x4
 CFloatLongReverbModule m_longReverbModule[2]; // +0x40
 CReverbConstants *m_pRvbConstants; // +0x90
 int m_iMyFaderTokenMin; // +0x94
 int m_iMyFaderTokenMax; // +0x98
public:
 virtual ~CFloatLongReverb();
};
#if defined(_MSC_VER) && defined(_M_IX86)
typedef char CFloatLongReverbSizePass22[(sizeof(CFloatLongReverb)==156)?1:-1];
#endif
#endif // FABLE_RETAIL_PASS22_FLOATREVERB_CREATE
#endif
