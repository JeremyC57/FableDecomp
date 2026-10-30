// NScript::CV_ExposeMayorScript::Main() -- the expose-the-mayor quest's main: bind five entity scripts ("EM_SuspiciousVillager", "EM_Sheriff", "EM_FlashLightPoint", "EM_Rhodri", "HH_CellarDoor"), finish adding scripted entities, activate the "V_ExposeMayorRhodri" quest, spawn DoMission and PreventConflictWithTCE, then declare the gossip categories
// Leaf: Main
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E75BA0, 848 bytes. Catalog label "CQ_SummoningTheShipScript::Main" is propagated:
//   the row's only reference is the vtable at 0x012E96D4, its strings are the EM_ (Expose Mayor) entities, and it
//   ends in the landed DeclareGossipCategories 0x00E76040 (the mayor gossip categories; landed under the
//   propagated CGameflowScript label).
// Callees: operator new 0x00BFEA1A, CCharString(const char*, long) 0x0099EBF0 / CCharString(const CCharString&)
//   0x0099EC30 / ~CCharString 0x0099EAE0, CScriptBase::AddEntityScriptBinding 0x00CB8230, CScriptBase::
//   PostAddScriptedEntities 0x00CB8930, CSpawnedFuncBase 0x00CDD450 (CSpawnedFunc vtable 0x012E96E0),
//   CScriptBase::AddSpawnedFunction 0x00CB7E50; entity Allocs 0x00E76360 0x00E76470 0x00E76730 0x00E765D0 0x00E76800;
//   DoMission 0x00E7A410, PreventConflictWithTCE 0x00E75EF0, DeclareGossipCategories 0x00E76040; game script
//   interface PostAddScriptedEntities (slot 64, +0x100) and ActivateQuest (slot 276, +0x450).
// Layout: CEntityScriptBinding<CV_ExposeMayorScript> (0x1C bytes, vtable 0x012E97DC) as the landed
//   CQ_DragonBossFightScript::Main 0x00D255E0; the first two bindings carry Flags 1.
// Shape: each name string is a temporary guarded by its own bit of one flag word (ebx: 1 .. 0x40).
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
// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#include "engine/CScriptBase.h"
namespace NScript {
class CV_ExposeMayorScript : public CScriptBase { public:
    struct CEM_SuspiciousVillager {
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CV_ExposeMayorScript& parent, CScriptDataBase* data, const CScriptThing& thing);   // 0x00E76360
    };
    struct CEM_Sheriff {
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CV_ExposeMayorScript& parent, CScriptDataBase* data, const CScriptThing& thing);   // 0x00E76470
    };
    struct CEM_FlashLightPoint {
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CV_ExposeMayorScript& parent, CScriptDataBase* data, const CScriptThing& thing);   // 0x00E76730
    };
    struct CEM_Rhodri {
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CV_ExposeMayorScript& parent, CScriptDataBase* data, const CScriptThing& thing);   // 0x00E765D0
    };
    struct CHH_CellarDoor {
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CV_ExposeMayorScript& parent, CScriptDataBase* data, const CScriptThing& thing);   // 0x00E76800
    };
    CGameScriptInterface* GSI;                          // +0x40
    void*                 Data;                         // +0x44
    void Main();
    void DoMission();   // 0x00E7A410
    void PreventConflictWithTCE();   // 0x00E75EF0
    void DeclareGossipCategories();   // 0x00E76040
};
void CV_ExposeMayorScript::Main()
{
    AddEntityScriptBinding(new CEntityScriptBinding<CV_ExposeMayorScript>("EM_SuspiciousVillager", this, &CEM_SuspiciousVillager::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_ExposeMayorScript>("EM_Sheriff", this, &CEM_Sheriff::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_ExposeMayorScript>("EM_FlashLightPoint", this, &CEM_FlashLightPoint::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_ExposeMayorScript>("EM_Rhodri", this, &CEM_Rhodri::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_ExposeMayorScript>("HH_CellarDoor", this, &CHH_CellarDoor::Alloc, 0));
    PostAddScriptedEntities();
    GSI->PostAddScriptedEntities();
    GSI->ActivateQuest("V_ExposeMayorRhodri");
    AddSpawnedFunction(new CSpawnedFunc<CV_ExposeMayorScript>("DoMission", 0, this, &CV_ExposeMayorScript::DoMission), CCharString(""));
    AddSpawnedFunction(new CSpawnedFunc<CV_ExposeMayorScript>("PreventConflictWithTCE", 0, this, &CV_ExposeMayorScript::PreventConflictWithTCE), CCharString(""));
    DeclareGossipCategories();
}
}