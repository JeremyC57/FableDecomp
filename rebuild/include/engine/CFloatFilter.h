#ifndef FABLE_PASS22_CFLOATFILTER_H
#define FABLE_PASS22_CFLOATFILTER_H
#ifdef FABLE_RETAIL_PASS22_FLOATREVERB_CREATE
// Original GameCODA2.10 debug CodeView: complete data layout and used original API.
#include "engine/CReverbDspProcessor.h"
class CFloatFilter : public CReverbDspProcessor {
public:
 virtual ~CFloatFilter();
private:
 float m_fHistoryData[4]; // +0x4
};
#if defined(_MSC_VER) && defined(_M_IX86)
typedef char CFloatFilterSizePass22[(sizeof(CFloatFilter)==20)?1:-1];
#endif
#endif // FABLE_RETAIL_PASS22_FLOATREVERB_CREATE
#endif
