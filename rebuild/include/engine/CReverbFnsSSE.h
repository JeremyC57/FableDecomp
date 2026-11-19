#ifndef FABLE_ENGINE_CREVERBFNSSSE_H
#define FABLE_ENGINE_CREVERBFNSSSE_H
// GameCODA 2.10 CReverbFnsSSE (CODAApiDbg.lib ReverbFns_SSE.obj CodeView LF_CLASS 0x11C9, size 4): the SSE overrides of the
// CReverbFns virtuals and the four-filter SSE second-order filter. Member signatures as the CodeView records them.
#include "engine/CReverbFns.h"
class CFloatFilterParams;
class CReverbFnsSSE : public CReverbFns {
public:
 virtual eReverbType Type();
 virtual void ScaleSamples(float*,const float*,float,float,unsigned int);
 virtual void ScaleSamples(float*,float,float,unsigned int);
 virtual void ScaleSamples(float*,const float*,const float,const unsigned int);
 virtual void ScaleSamples(float*,const float,const unsigned int);
 virtual void AddSamples(float*,const float*,const unsigned int);
 virtual void ScaleAddSamples(float*,const float*,float,const float,const unsigned int);
 virtual void ScaleAddSamples(float*,const float*,float,const unsigned int);
 void ApplyFilterOrder2_quad(float* pfDest,const float* pfSrc,float** apHistory,CFloatFilterParams** aFilterParams,unsigned int uiSamples);
};
#if defined(_MSC_VER) && defined(_M_IX86)
typedef char CReverbFnsSSESize[(sizeof(CReverbFnsSSE)==4)?1:-1];
#endif
#endif
