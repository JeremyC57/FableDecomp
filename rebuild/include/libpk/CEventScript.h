#ifndef FABLE_LIBPK_CEVENTSCRIPT_H
#define FABLE_LIBPK_CEVENTSCRIPT_H
#if defined(FABLE_RETAIL_LIBPK_EVENT_SCRIPT_CTOR_PASS23)
#include "libpk/PKCTRRedblackTree.h"
#include "rebuild_abi.h"
#include <stddef.h>
// All names in this header are descriptive: libpk is absent from original PDBs/RTTI.
// Native C2CBD0/C2CC60 and vtables provide the hierarchy, full used prefix and slots.
class CEventScript;struct CEvent;struct CScriptOwner;
struct CEventTarget;struct CEventPosition;class CFlagOverrideBase;
class CScriptRoot {
public:
    virtual void Register()=0;
    virtual CEvent* CreateEvent(CEventTarget*,void*,void*)=0;
    virtual void* CreateInstance(void*,void*,const CEventPosition*,void*,void*)=0;
    virtual long GetEventCount()=0;
    virtual CEvent* GetEventByIndex(long)=0;
    virtual CFlagOverrideBase* GetFlagOverride()=0;
    virtual CEvent* CreateEventFromParameters(void*,void*,void*,void*,void*,void*)=0;
    virtual ~CScriptRoot(){}
    CScriptRoot(CScriptOwner* owner):m_pOwner(owner){}
    CScriptOwner* m_pOwner;
};
// Concrete registry belongs to the existing shared PKCTRList family. The
// original owner tail/registry implementation is not consumed by this constructor.
class CScriptRegistry : public PKCTRList<CEventScript> {
public:
    CScriptRegistry();
    virtual int IsEqual(const void*,const void*)const;
    virtual int IsLess(const void*,const void*)const;
    virtual int IsKeyEqual(const void*,const void*)const;
    virtual int IsKeyLess(const void*,const void*)const;
};
struct CScriptOwner {
    unsigned char UnrecoveredPrefix[16];
    CScriptRegistry m_registry;
};
class CEventContainer : public PKCTRList<CEvent> {
public:
    virtual int IsEqual(const void*,const void*)const;
    virtual int IsLess(const void*,const void*)const;
    virtual int IsKeyEqual(const void*,const void*)const;
    virtual int IsKeyLess(const void*,const void*)const;
    virtual void TransferAllFrom(PKCTRList<CEvent>*);
    CEventContainer():PKCTRList<CEvent>(&m_tree),m_tree(0){}
    virtual ~CEventContainer(){}
    PKCTRRedblackTree m_tree;
};
class CFlagOverrideBase {
public:
    virtual void LoadPrimaryFlags(void*,bool)=0;
    virtual void LoadSecondaryFlags(void*,bool)=0;
    virtual ~CFlagOverrideBase(){}
};
class CFlagOverride : public CFlagOverrideBase {
public:
    virtual void LoadPrimaryFlags(void*,bool);
    virtual void LoadSecondaryFlags(void*,bool);
    CFlagOverride();
    unsigned short m_maskB,m_valueB,m_maskA,m_valueA;
};
class CEventScript : public CScriptRoot {
public:
    virtual void Register();
    virtual CEvent* CreateEvent(CEventTarget*,void*,void*);
    virtual void* CreateInstance(void*,void*,const CEventPosition*,void*,void*);
    virtual long GetEventCount();
    virtual CEvent* GetEventByIndex(long);
    virtual CFlagOverrideBase* GetFlagOverride();
    virtual CEvent* CreateEventFromParameters(void*,void*,void*,void*,void*,void*);
    virtual ~CEventScript();
    CEventScript(CScriptOwner*);
    CEventContainer m_events;
    CFlagOverride m_flags;
};
FABLE_STATIC_ASSERT(sizeof(CScriptRoot)==8);
FABLE_STATIC_ASSERT(offsetof(CScriptOwner,m_registry)==16);
FABLE_STATIC_ASSERT(sizeof(CEventContainer)==48);
FABLE_STATIC_ASSERT(sizeof(CFlagOverride)==12);
FABLE_STATIC_ASSERT(offsetof(CEventScript,m_events)==8);
FABLE_STATIC_ASSERT(offsetof(CEventScript,m_flags)==56);
FABLE_STATIC_ASSERT(sizeof(CEventScript)==68);
#endif
#endif
