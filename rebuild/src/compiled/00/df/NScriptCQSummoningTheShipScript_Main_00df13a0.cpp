// NScript::CQ_SummoningTheShipScript::Main() -- the summoning-the-ship quest's main: bind six entity scripts ("STS_BriarRose", "SummonerAttacker", "SummonerMinion", "FireHeartHolder", "FireHeart", "M_ActivateLighthouse"), finish adding scripted entities, then spawn the DoMission thread
// Leaf: Main
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DF13A0, 768 bytes (vtable reference 0x012DE114).
// Callees: operator new 0x00BFEA1A, CCharString(const char*, long) 0x0099EBF0 / CCharString(const CCharString&)
//   0x0099EC30 / ~CCharString 0x0099EAE0, CScriptBase::AddEntityScriptBinding 0x00CB8230, CScriptBase::
//   PostAddScriptedEntities 0x00CB8930, CSpawnedFuncBase 0x00CDD450 (CSpawnedFunc vtable 0x012DE120),
//   CScriptBase::AddSpawnedFunction 0x00CB7E50; entity Allocs 0x00DF16B0 0x00DF21F0 0x00DF2960 0x00DF2C20 0x00DF2E90
//   0x00DF3460; DoMission 0x00DF36E0; the game script interface's PostAddScriptedEntities (slot 64, +0x100).
// Layout: CEntityScriptBinding<CQ_SummoningTheShipScript> (0x1C bytes, vtable 0x012DE194) as the landed
//   CQ_DragonBossFightScript::Main 0x00D255E0.
// Shape: each name string is a temporary guarded by its own bit of one flag word (ebx: 1 .. 0x40); with six
//   zero Flags stores the 0 is promoted into ebp.
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
};
void* __cdecl operator new(unsigned int size);          // 0x00BFEA1A
// Layout: CSpawnedFuncBase is the retail polymorphic suspendable-process chain from the shared headers
//   (FABLE_RETAIL_SUSPENDABLE_PROCESS opt-in: CSpawnedFuncBase : CSuspendableProcessMicrothread : CASuspendableProcess :
//   CBaseClassNonCopyable : CBaseClass, 0x34 bytes, vtable 0x012C2F9C; ctor 0x00CDD450).
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#include "engine/CSpawnedFuncBase.h"
#include "engine/CSpawnedFunc.h"                         // vtable per instantiation; ctor inline, slot 4 SuspendableProcess

class CScriptThing;
class CScriptDataBase;
class CEntityScriptBase;
class CActiveEntityScriptBase;
template <class T> class CCountedPointer { public: T* Data; void* Info; };

class __declspec(novtable) CEntityScriptBindingBase { public:
    virtual const CCharString& GetEntityScriptName() const = 0;
};
template <class T> struct CEntityScriptBinding : CEntityScriptBindingBase {
    typedef CCountedPointer<CEntityScriptBase> (__fastcall *TAllocFunc)(T& parent, CScriptDataBase* data, const CScriptThing& thing);
    CCharString     EntityScriptName;                   // +0x04
    T*              PParentScript;                      // +0x08
    void (T::*Func)();                                  // +0x0c
    TAllocFunc      ClassAllocFunc;                     // +0x10
    bool            ClassAllocValid;                    // +0x14
    unsigned long   Flags;                              // +0x18
    CEntityScriptBinding(const CCharString& name, T* parent, TAllocFunc alloc, unsigned long flags)
        : EntityScriptName(name), PParentScript(parent), ClassAllocFunc(alloc), ClassAllocValid(true), Flags(flags) {}
    virtual const CCharString& GetEntityScriptName() const { return EntityScriptName; }
};

// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

namespace NScript {
}   // namespace NScript (CScriptBase is global in retail)
class CScriptBase { public:                                    // retail 0x40 bytes
    unsigned char _body[0x40];
    void AddEntityScriptBinding(CEntityScriptBindingBase* binding);                   // 0x00CB8230
    void PostAddScriptedEntities();                                                   // 0x00CB8930
    void SpawnFunction(CSpawnedFuncBase* func, const char* regionName = "") { AddSpawnedFunction(func, CCharString(regionName)); }
public:
    void AddSpawnedFunction(CSpawnedFuncBase*, const CCharString&);   // 0x00CB7E50
};
namespace NScript {
class CQ_SummoningTheShipScript : public CScriptBase { public:
    struct CSTS_BriarRose {
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CQ_SummoningTheShipScript& parent, CScriptDataBase* data, const CScriptThing& thing);   // 0x00DF16B0
    };
    struct CSummonerAttacker {
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CQ_SummoningTheShipScript& parent, CScriptDataBase* data, const CScriptThing& thing);   // 0x00DF21F0
    };
    struct CSummonerMinion {
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CQ_SummoningTheShipScript& parent, CScriptDataBase* data, const CScriptThing& thing);   // 0x00DF2960
    };
    struct CFireHeartHolder {
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CQ_SummoningTheShipScript& parent, CScriptDataBase* data, const CScriptThing& thing);   // 0x00DF2C20
    };
    struct CFireHeart {
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CQ_SummoningTheShipScript& parent, CScriptDataBase* data, const CScriptThing& thing);   // 0x00DF2E90
    };
    struct CM_ActivateLighthouse {
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CQ_SummoningTheShipScript& parent, CScriptDataBase* data, const CScriptThing& thing);   // 0x00DF3460
    };
    CGameScriptInterface* GSI;                          // +0x40
    void*                 Data;                         // +0x44
    void Main();
    void DoMission();   // 0x00DF36E0
};
void CQ_SummoningTheShipScript::Main()
{
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_SummoningTheShipScript>("STS_BriarRose", this, &CSTS_BriarRose::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_SummoningTheShipScript>("SummonerAttacker", this, &CSummonerAttacker::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_SummoningTheShipScript>("SummonerMinion", this, &CSummonerMinion::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_SummoningTheShipScript>("FireHeartHolder", this, &CFireHeartHolder::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_SummoningTheShipScript>("FireHeart", this, &CFireHeart::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_SummoningTheShipScript>("M_ActivateLighthouse", this, &CM_ActivateLighthouse::Alloc, 0));
    PostAddScriptedEntities();
    GSI->PostAddScriptedEntities();
    SpawnFunction(new CSpawnedFunc<CQ_SummoningTheShipScript>("DoMission", 0, this, &CQ_SummoningTheShipScript::DoMission));
}
}