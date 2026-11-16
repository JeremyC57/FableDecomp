#ifndef FABLE_CSALOUTPUTNULL_H
#define FABLE_CSALOUTPUTNULL_H
#ifdef FABLE_RETAIL_CSALPLATFORM_CREATE_MIXER_PASS23
#include "engine/CSalOutputKS.h"
// Native older fork has an inline768-short frame instead of newer SDK m_pData.
// C5EEC0 clears count+604; C5EE30 clears frame+608/flag+60c. Allocation is610bytes.
// Additional native-only field names below are descriptive.
class CSalOutputNull:public CSalOutput {
public: CSalOutputNull();virtual ~CSalOutputNull();
 virtual bool Create(const GUID*,HWND,bool);virtual void Release();
 virtual bool CreateBuffer(unsigned int,unsigned int);virtual void ReleaseBuffer();
 virtual bool FrameIsReady();virtual void* FrameLock();virtual void FrameUnlock(void*);
 virtual unsigned int GetMinBufferSamples()const;
private:
 short m_frameBuffer[768];unsigned int m_readyCount,m_frameCount;bool m_frameReady;
};
FABLE_STATIC_ASSERT(sizeof(CSalOutputNull)==0x610);
#endif
#endif
