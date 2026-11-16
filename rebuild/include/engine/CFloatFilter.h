#ifndef FABLE_PASS22_CFLOATFILTER_H
#define FABLE_PASS22_CFLOATFILTER_H
#ifdef FABLE_RETAIL_PASS22_FLOATREVERB_CREATE
// Original GameCODA2.10 debug CodeView: complete data layout and used original API.
#include "engine/CReverbDspProcessor.h"
class CFloatFilter : public CReverbDspProcessor {
public:
 virtual ~CFloatFilter();
#ifdef FABLE_RETAIL_CFLOATFILTER_APPLY_PASS23
 CFloatFilter();
 void Create();
 void Clear();
 float* GetFilterHistory(){return m_fHistoryData;}
 void Process(const class CFloatFilterParams*,const float*,float*,bool,unsigned int);
 void Process(class CFloatFilterParams*,const class CFloatFilterParams*,const float*,float*,bool,unsigned int,unsigned int);
private:
 void BeginFading();
 void ApplyFilter(const float*,const float*,float*,const float*,float*,bool,unsigned int);
#endif
private:
 float m_fHistoryData[4]; // +0x4
};
#if defined(_MSC_VER) && defined(_M_IX86)
typedef char CFloatFilterSizePass22[(sizeof(CFloatFilter)==20)?1:-1];
#endif
#endif // FABLE_RETAIL_PASS22_FLOATREVERB_CREATE
#endif
