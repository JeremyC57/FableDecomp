#ifndef FABLE_PASS22_CREVERBDSPPROCESSOR_H
#define FABLE_PASS22_CREVERBDSPPROCESSOR_H
#ifdef FABLE_RETAIL_PASS22_FLOATREVERB_CREATE
// Original GameCODA2.10 debug CodeView: complete data layout and used original API.
#ifdef FABLE_RETAIL_CFLOATFILTER_APPLY_PASS23
#include "engine/CReverbFns.h"
#endif
class CReverbDspProcessor {
public:
#ifdef FABLE_RETAIL_CFLOATFILTER_APPLY_PASS23
 CReverbDspProcessor();
#endif
 virtual ~CReverbDspProcessor();
#ifdef FABLE_RETAIL_CFLOATFILTER_APPLY_PASS23
protected:
 static CReverbFns* m_pDSP;
 static int m_iDSP_ResourceCount;
private:
 bool CreateProcessor();
 void DestroyProcessor();
#endif
};
#if defined(_MSC_VER) && defined(_M_IX86)
typedef char CReverbDspProcessorSizePass22[(sizeof(CReverbDspProcessor)==4)?1:-1];
#endif
#endif // FABLE_RETAIL_PASS22_FLOATREVERB_CREATE
#endif
