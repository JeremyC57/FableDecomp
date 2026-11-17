#ifndef FABLE_LIBPK_CPOOLEDRESOURCE_H
#define FABLE_LIBPK_CPOOLEDRESOURCE_H
#include "libpk/CPKVoiceParams.h"
struct CSoundEventManager;
struct CSoundPosition;
class CStreamSoundBank;
// Native012b3fdc has the same9-method sound-control prefix as existing event/group classes.
// DestructorC26550 is NONvirtual; slot0 C26740 returns the id address, not a deleting dtor.
class CPooledResource {
public:
 virtual long* GetIndexSlot();
 virtual void ResetLevel(CSoundEventManager*);
 virtual void SetUserGain(CSoundEventManager*,float);
 virtual void SetPlaybackParameter(CSoundEventManager*,float);
 virtual void ResetPlaybackState(CSoundEventManager*);
 virtual void Set3DPosition(CSoundEventManager*,const CSoundPosition&);
 virtual void SetPositionParameters(CSoundEventManager*,float,float);
 virtual void ApplyPan(CSoundEventManager*,float);
 virtual bool GetPositionFlag(CSoundEventManager*);
 long m_handle,m_id,m_padding;
 unsigned long m_size,m_offset,m_tail,m_key;
 CStreamSoundBank* m_pBank;
 CPKVoiceParams m_params;
 short m_state,m_flag;
 CPooledResource();
 ~CPooledResource();
 unsigned long GetFirstBlock();
 unsigned long GetBlockCount();
 void Activate(CSoundEventManager*);
 void Release(CSoundEventManager*);
#ifdef FABLE_RETAIL_CPOOLEDRESOURCE_DEACTIVATE
 // NF63 (round 7, rows 0x00C12460 / 0x00C12520): 0x00C26580 sets the state word to 2 and tail-calls the manager's event set
 // Remove (slot 8): the counterpart of the inline Activate (state 1, Add); Release 0x00C265B0 calls it when the state is 1. Descriptive
 // name of the landed ?Deactivate@CPooledResource@@ spelling (0x00C265B0's binding), with the manager type both callers pass.
 void Deactivate(CSoundEventManager*);                               // 0x00C26580
#endif
};
#ifdef FABLE_RETAIL_STREAMED_REQUEST_CALLEES_PASS23
#include "libpk/CSoundEventManager.h"
inline unsigned long CPooledResource::GetFirstBlock(){return (m_key-m_offset)/m_pBank->m_pStream->GetBlockSize();}
inline unsigned long CPooledResource::GetBlockCount(){return (m_offset+m_size+m_tail)/m_pBank->m_pStream->GetBlockSize();}
inline void CPooledResource::Activate(CSoundEventManager*manager){m_state=1;manager->m_events.Add(this);}
#endif
#ifdef FABLE_RETAIL_STREAMED_REQUEST_CTOR_PASS23
// NativeC266E0 zeroes these four words/two shorts and constructs the real parameter subobject.
inline CPooledResource::CPooledResource():m_handle(0),m_id(0),m_offset(0),m_tail(0),m_state(0),m_flag(0){}
#endif
#endif
