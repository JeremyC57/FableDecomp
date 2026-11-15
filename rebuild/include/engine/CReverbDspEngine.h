#ifndef FABLE_PASS22_CREVERBDSPENGINE_H
#define FABLE_PASS22_CREVERBDSPENGINE_H
#ifdef FABLE_RETAIL_PASS22_FLOATREVERB_CREATE
// Original GameCODA2.10 debug CodeView: complete data layout and used original API.
#include "engine/CReverbDspProcessor.h"
class CReverbDspEngine : public CReverbDspProcessor {
public:
 virtual ~CReverbDspEngine();
 virtual void Commit(void *,bool);
 virtual bool Process(void *,bool,void * *,unsigned int);
 virtual void Clear();
protected:
 virtual bool ProcessReverb(void *,bool,void * *,unsigned int) = 0;
 void * m_pvParams; // +0x4
 void * m_pvParamsNew; // +0x8
 void * m_pvParamsIncoming; // +0xc
 unsigned int m_uiSizeOfParams; // +0x10
 bool m_bIsFading; // +0x14
 int m_iFaderToken; // +0x18
 int m_iSamplesToFade; // +0x1c
 unsigned int m_uiNumComponents; // +0x20
private:
 bool m_bIncomingParameters; // +0x24
};
#if defined(_MSC_VER) && defined(_M_IX86)
typedef char CReverbDspEngineSizePass22[(sizeof(CReverbDspEngine)==40)?1:-1];
#endif
#endif // FABLE_RETAIL_PASS22_FLOATREVERB_CREATE
#endif
