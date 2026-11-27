// Retail opt-in (Codex pass20): SDK CodeView and native00c3d470; PROCESS branch only, default preserved.
#ifndef FABLE_CCODASOURCE_H
#define FABLE_CCODASOURCE_H
#include "engine/CODA_PlatformTypes.h"
#include "engine/CCodaObject.h"
#include "engine/CCodaBufferQueue.h"
// Layout: native+0c caps,+10 status,+50 voice,+54 queue,+78 buffer;
// SDK release object agrees, debug adds only the base diagnostic word.
class CCoda3DSource;
#ifdef FABLE_RETAIL_CCODASOURCE_EAX
struct _GUID;
#endif
#if defined(FABLE_RETAIL_PASS22_CSALSW_CALCPARAMS) || defined(FABLE_RETAIL_CCODACONTEXT_CREATE2D)
class CCoda2DSource;
#endif
class CCodaSource:public CCodaObject {
public:
#ifdef FABLE_RETAIL_CCODACONTEXT_CREATE3D
 CCodaSource();
#endif
#ifdef FABLE_RETAIL_CSALSW2D_CALCPARAMS_PASS29
 bool IsStereoFilter() const { return (m_ffStatus & 0x10000) != 0; }
#endif
 virtual ~CCodaSource();
#ifndef FABLE_RETAIL_CCODACONTEXT_PROCESS
 #ifndef FABLE_RETAIL_PASS22_CSALSW_CALCPARAMS
 #ifndef FABLE_RETAIL_PASS22_CCODA3D_CREATE
 virtual bool IsSource()const;
 #endif
 #if defined(FABLE_RETAIL_PASS22_CCODACONTEXT_CREATE) || defined(FABLE_RETAIL_PASS22_CCODA3D_CREATE)
 CCoda3DSource* Get3D()const;
 #endif
 #else
 public:
 const CODASOURCE& GetParams()const{return m_Params;}
 bool IsLooping()const{return (m_ffStatus & 0x100)!=0;}
 CCoda3DSource* Get3D()const;
 CCoda2DSource* Get2D()const{return Is3D()?0:(CCoda2DSource*)this;}
 #endif
#else
 void Process(CREFTIME);
 bool IsPlaying()const{return (m_ffStatus&0x217)!=0;}
 CCoda3DSource* Get3D()const;
#endif
#if defined(FABLE_RETAIL_CCODACONTEXT_CREATE2D) && (!defined(FABLE_RETAIL_PASS22_CSALSW_CALCPARAMS) || defined(FABLE_RETAIL_CCODACONTEXT_PROCESS))
 // Retail opt-in (Codex pass22): original const Get2D identity and native Is3D query.
 CCoda2DSource* Get2D()const;
#endif
 virtual bool Is3D()const;
 virtual bool IsCreated()const;
 virtual bool Create(unsigned int);
#ifdef FABLE_RETAIL_PASS22_CCODA3D_CREATE
 virtual void Release();
#endif
#ifdef FABLE_RETAIL_CCODASOURCE_EAX
 // Retail opt-in (w24C, rows 0x00C44090 / 0x00C44100; needs FABLE_RETAIL_PASS22_CCODA3D_CREATE for slots 0..4): the rest of the native
 // source vtable 0x012B5A88. Slot 5 is pure and returns the source's effective gain (2D 0x00C445B0 `fld [ecx+18h]`, 3D 0x00C412D0,
 // landed as GetEffectiveGain); slots 6 / 7 (0x00C43780 / 0x00C437A0) store the CODASOURCE pLPFilter / pEQFilter records
 // (+0x24 / +0x28); slot 8 is pure: the 2D / 3D source allocates the voice for a buffer (0x00C448A0 / 0x00C418C0) and returns the
 // error. Slot names are descriptive (no SDK symbol list on hand). EAXSet / EAXGet forward to CSalVoice::EAXSet / EAXGet (SDK
 // CodeView, voice slots 15 / 16), allocating the voice first when the source has none.
 virtual float GetEffectiveGain()const=0;
 virtual void SetLPFilter(const _CLPFILTER*);
 virtual void SetEQFilter(const _CEQFILTER*);
 virtual int AllocateVoice(CSalBuffer*,CSalVoice**)=0;
 void EAXSet(const _GUID*,int,void*,int);
 void EAXGet(const _GUID*,int,void*,int);
#endif
#ifdef FABLE_RETAIL_CSALSW3D_CALCPARAMS
 // Retail opt-in (hand4c w24, row 0x00C54C10): SDK 2.10 CodeView public ?GetCaps@CCodaSource@@QBEIXZ; release reads +0x0c.
 unsigned int GetCaps()const{return m_ffCaps;}
#endif
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
