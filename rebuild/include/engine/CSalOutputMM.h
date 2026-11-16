#ifndef FABLE_CSALOUTPUTMM_H
#define FABLE_CSALOUTPUTMM_H
#ifdef FABLE_RETAIL_CSALPLATFORM_CREATE_MIXER_PASS23
#include "engine/CSalOutputKS.h"
struct wavehdr_tag;struct HWAVEOUT__;
// SDK member names; native C5D750 constructs the28-byte mutex at+1c.
class CSalOutputMM:public CSalOutput {
public: CSalOutputMM();virtual ~CSalOutputMM();
 virtual bool Create(const GUID*,HWND,bool);virtual void Release();
 virtual bool CreateBuffer(unsigned int,unsigned int);virtual void ReleaseBuffer();
 virtual bool FrameIsReady();virtual void* FrameLock();virtual void FrameUnlock(void*);
 virtual unsigned int GetMinBufferSamples()const;
private:
 wavehdr_tag* m_pWaveHeaderTable;HWAVEOUT__* m_hWaveOut;char* m_pBuffer;
 unsigned int m_nFrames,m_nFrameCount,m_nFrameIndex;CSalMutex m_mutexFrameCount;
};
FABLE_STATIC_ASSERT(sizeof(CSalOutputMM)==0x38);
FABLE_STATIC_ASSERT(sizeof(CSalOutputKS)==0x5c);
#endif
#endif
