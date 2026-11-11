// Retail opt-in (Codex pass20): SDK CodeView and native00c3d470; PROCESS branch only, default preserved.
#ifndef FABLE_CCODAOBJECT_H
#define FABLE_CCODAOBJECT_H
#include "engine/CCodaObjectBase.h"
// Layout: SDK release CCodaObject has the vptr plus two links, 12 bytes.
// Native CCodaSource m_ffCaps+0c, queue+54 and buffer+78 confirm it.
class CCodaObject:public CCodaObjectBase {
public:
 virtual ~CCodaObject();
#ifndef FABLE_RETAIL_CCODACONTEXT_PROCESS
 virtual bool IsBuffer()const;
 virtual bool IsContext()const;
 virtual bool IsCreated()const;
 virtual bool IsEnvironment()const;
 virtual bool IsEAX()const;
 virtual bool IsListener()const;
 virtual bool IsSource()const;
 virtual bool IsReverb()const;
 virtual bool IsVoice()const;
#else
 CCodaObject* GetNext()const{return m_pNext;}
#endif
private:CCodaObject* m_pPrev;CCodaObject* m_pNext;
};
#endif
