// Retail opt-in (Codex pass20): SDK CodeView and native00c3d470; PROCESS branch only, default preserved.
#ifndef FABLE_CCODAOBJECT_H
#define FABLE_CCODAOBJECT_H
#include "engine/CCodaObjectBase.h"
// Layout: SDK release CCodaObject has the vptr plus two links, 12 bytes.
// Native CCodaSource m_ffCaps+0c, queue+54 and buffer+78 confirm it.
class CCodaObject:public CCodaObjectBase {
public:
 virtual ~CCodaObject();
#if !defined(FABLE_RETAIL_CCODACONTEXT_PROCESS) && !defined(FABLE_RETAIL_PASS22_CSALSW_CALCPARAMS) && !defined(FABLE_RETAIL_PASS22_CCODA3D_CREATE)
 virtual bool IsBuffer()const;
 virtual bool IsContext()const;
 virtual bool IsCreated()const;
 virtual bool IsEnvironment()const;
 virtual bool IsEAX()const;
 virtual bool IsListener()const;
 virtual bool IsSource()const;
 virtual bool IsReverb()const;
 virtual bool IsVoice()const;
#elif defined(FABLE_RETAIL_CCODACONTEXT_PROCESS)
 CCodaObject* GetNext()const{return m_pNext;}
#endif
#if defined(FABLE_RETAIL_CCODA3DSOURCE_COMMIT) && !defined(FABLE_RETAIL_CCODACONTEXT_PROCESS)
 // Retail opt-in (hand3c, row 0x00C41FB0 CCoda3DSource::Commit): SDK 2.10 CodeView ?GetNext@CCodaObject@@QBEPAV1@XZ (Dbg COMDAT:
 // return m_pNext); the same body as the PROCESS branch above.
 CCodaObject* GetNext()const{return m_pNext;}
#endif
private:CCodaObject* m_pPrev;CCodaObject* m_pNext;
};
#endif
