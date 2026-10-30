// NScript::CQR_EscortTraderScript::Main() -- bind the M_BanditTrigger / AttackingBandit / TraderToEscort entity scripts, set the quest card objective, then spawn WatchForTermination, DoMission and WatchForRegionLoads
// Leaf: Main
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E1E110, length 853 bytes (padded to 864), plain ret.
// Callees: operator new 0x00BFEA1A; CCharString(const char*, long) 0x0099EBF0 / (const CCharString&) 0x0099EC30 /
// ~CCharString 0x0099EAE0; CScriptBase::AddEntityScriptBinding 0x00CB8230, PostAddScriptedEntities 0x00CB8930,
// AddSpawnedFunction 0x00CB7E50, IsActiveThreadTerminating 0x00CB7940; CSpawnedFuncBase 0x00CDD450 (CSpawnedFunc vtable
// 0x012E12F4); CEntityScriptBinding vtable 0x012E1504; entity Alloc functions 0x00E1E880 / 0x00E1ED50 / 0x00E1E7A0
// (manifest "CTriangleList" labels are propagated); spawned WatchForTermination 0x00E1E530, DoMission 0x00E1F0C0,
// WatchForRegionLoads 0x00E1E470; CGameScriptInterface slots 7, 64, 296 SetQuestCardObjective, 655 GetActiveQuestName.
// Layout: as the landed CV_KnotholeGladeGatesScript::Main 0x00E9FCD0 (binding 0x1c bytes with Flags 0 here; spawned
// func 0x3c); Game at +0x40.
// Shape: every new-expression's name string is a conditional temporary with its own ebx flag bit.
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
};
// Layout: CSpawnedFuncBase is the retail polymorphic suspendable-process chain from the shared headers
//   (FABLE_RETAIL_SUSPENDABLE_PROCESS opt-in: CSpawnedFuncBase : CSuspendableProcessMicrothread : CASuspendableProcess :
//   CBaseClassNonCopyable : CBaseClass, 0x34 bytes, vtable 0x012C2F9C; ctor 0x00CDD450).
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#include "engine/CSpawnedFuncBase.h"
#include "engine/CSpawnedFunc.h"                         // vtable per instantiation; ctor inline, slot 4 SuspendableProcess
class CScriptThing;
class CScriptDataBase;
class CEntityScriptBase;
#include "engine/CCountedPointer.h"
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
class CQR_EscortTraderScript : public CScriptBase { public:
    struct CM_BanditTrigger {
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CQR_EscortTraderScript& parent, CScriptDataBase* data, const CScriptThing& thing);   // 0x00E1E880
    };
    struct CAttackingBandit {
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CQR_EscortTraderScript& parent, CScriptDataBase* data, const CScriptThing& thing);   // 0x00E1ED50
    };
    struct CTraderToEscort {
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CQR_EscortTraderScript& parent, CScriptDataBase* data, const CScriptThing& thing);   // 0x00E1E7A0
    };
    CGameScriptInterface* Game;                         // +0x40
    void*                 Data;                         // +0x44
    void WatchForTermination();                         // 0x00E1E530
    void DoMission();                                   // 0x00E1F0C0
    void WatchForRegionLoads();                         // 0x00E1E470
    void Main();
};

void CQR_EscortTraderScript::Main()
{
    AddEntityScriptBinding(new CEntityScriptBinding<CQR_EscortTraderScript>("M_BanditTrigger", this, &CM_BanditTrigger::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CQR_EscortTraderScript>("AttackingBandit", this, &CAttackingBandit::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CQR_EscortTraderScript>("TraderToEscort", this, &CTraderToEscort::Alloc, 0));
    PostAddScriptedEntities();
    Game->PostAddScriptedEntities();
    Game->SetQuestCardObjective(Game->GetActiveQuestName(), "TEXT_QUEST_ORCHARD_FARM_ESCORT_OBJECTIVE_01", "LookoutPoint", "HeroGuildComplexInside");
    AddSpawnedFunction(new CSpawnedFunc<CQR_EscortTraderScript>("WatchForTermination", 0, this, &CQR_EscortTraderScript::WatchForTermination), CCharString(""));
    AddSpawnedFunction(new CSpawnedFunc<CQR_EscortTraderScript>("DoMission", 0, this, &CQR_EscortTraderScript::DoMission), CCharString(""));
    AddSpawnedFunction(new CSpawnedFunc<CQR_EscortTraderScript>("WatchForRegionLoads", 0, this, &CQR_EscortTraderScript::WatchForRegionLoads), CCharString(""));
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating()) return;
}
}