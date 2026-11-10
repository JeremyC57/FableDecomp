#ifndef FABLE_CCODASOURCE_H
#define FABLE_CCODASOURCE_H
#include "engine/CODA_PlatformTypes.h"
#include "engine/CCodaObject.h"
#include "engine/CCodaBufferQueue.h"
// Layout: native+0c caps,+10 status,+50 voice,+54 queue,+78 buffer;
// SDK release object agrees, debug adds only the base diagnostic word.
class CCodaSource:public CCodaObject {
public:
 virtual ~CCodaSource();
 virtual bool IsSource()const;
 virtual bool Is3D()const;
 virtual bool IsCreated()const;
 virtual bool Create(unsigned int);
 bool IsBufferSet()const{return m_pBuffer!=0 || (m_Queue.IsCreated() && !m_Queue.IsEmpty());}
 // Native Play returns its voice-acquisition error or zero; SDK2.10 changed this return to void.
 int Play(bool bLooping);
protected:
 unsigned int m_ffCaps;
 unsigned int m_ffStatus;
 CODASOURCE m_Params;
 _CLPFILTER m_LPFilter;
 _CEQFILTER m_EQFilter;
 CSalVoice *m_pVoice;
private:
 CCodaBufferQueue m_Queue;
 CSalBuffer *m_pBuffer;
 _CLEVELENVELOPE m_LevelEnv;
 _CMULTIFUNCENVELOPE m_MultiFuncEnv;
 _CPITCHLFO m_PitchLFO;
 _CMULTIFUNCLFO m_MultiFuncLFO;
 void (FABLE_STDCALL *m_pfnQueueCallback)(void * ,const _CQUEUEDESC *,unsigned int );
 unsigned int m_QueueData;
 void (FABLE_STDCALL *m_pfnPlayCallback)(void * ,unsigned int ,unsigned int );
 unsigned int m_PlayCallbackData;
 void (FABLE_STDCALL *m_pfnBufferCallback)(void * ,void * ,const _CBUFFERDESC *,void * *);
 unsigned int m_cbMaxBufferSize;
 void (FABLE_STDCALL *m_pfnPreMixCallback)(void * ,const _CBUFFERDESC *,int );
 unsigned int m_cbPlayCursor;
 EAXSOURCE_tag *m_pEAXSource;
 CEAXBuffer *m_pEAXBuffer;
};
#endif
