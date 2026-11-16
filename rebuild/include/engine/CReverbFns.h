#ifndef FABLE_ENGINE_CREVERBFNS_H
#define FABLE_ENGINE_CREVERBFNS_H
#include "rebuild_abi.h"
class CFloatCyclicBuffer;
// Original SDK release names/debug CodeView:9-slot virtual shape; overloaded ring wrappers are nonvirtual in original PUBLICs.
class CReverbFns {
public:
 enum eReverbType {eReverb_Float=0,eReverb_SSE=1,eReverb_SSE2=2,eReverb_Short=3,eReverb_Int=4,eReverb_MMX=5};
 CReverbFns(){}
#if defined(FABLE_RETAIL_PASS23_HOST_TEST_ABI) && !defined(_MSC_VER)
 static void* operator new(size_t);
#else
 static void* FABLE_CDECL operator new(unsigned int);
#endif
 static void FABLE_CDECL operator delete(void*);
 virtual eReverbType Type();
 virtual void ScaleSamples(float*,const float,const unsigned int);
 virtual void ScaleSamples(float*,const float*,const float,const unsigned int);
 virtual void ScaleSamples(float*,float,float,unsigned int);
 virtual void ScaleSamples(float*,const float*,float,float,unsigned int);
 virtual void AddSamples(float*,const float*,const unsigned int);
 virtual void ScaleAddSamples(float*,const float*,float,const unsigned int);
 virtual void ScaleAddSamples(float*,const float*,float,const float,const unsigned int);
 virtual void ApplyFilterOrder2(float*,const float*,const float*,const float*,float*,const unsigned int);
 void CopySamples(CFloatCyclicBuffer*,const unsigned int,const float*,const unsigned int);
 void CopySamples(float*,const CFloatCyclicBuffer*,const unsigned int,const unsigned int);
 void ScaleSamples(CFloatCyclicBuffer*,unsigned int,const float*,float,unsigned int);
 void ScaleSamples(float*,const CFloatCyclicBuffer*,unsigned int,float,float,unsigned int);
 void ScaleSamples(float*,const CFloatCyclicBuffer*,unsigned int,float,unsigned int);
 void AddSamples(CFloatCyclicBuffer*,const unsigned int,const float*,const unsigned int);
 void AddSamples(float*,const CFloatCyclicBuffer*,const unsigned int,const unsigned int);
 void ScaleAddSamples(float*,const CFloatCyclicBuffer*,const unsigned int,const float,const float,const unsigned int);
 void ScaleAddSamples(float*,const CFloatCyclicBuffer*,const unsigned int,const float,const unsigned int);
};
#endif
