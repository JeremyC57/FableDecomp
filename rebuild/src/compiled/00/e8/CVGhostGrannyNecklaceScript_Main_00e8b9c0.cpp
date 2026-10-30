// CV_GhostGrannyNecklaceScript::Main() -- the Ghost Granny's Necklace quest main: bind the necklace / daughter-in-law / son entity scripts, spawn CreateBandit and GhostGrannyHints, wait for the necklace to be returned, complete the quest, then wait for the hero to leave OrchardFarm and deactivate it
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E8B9C0, 0x35d bytes, plain `ret`.
// Callees: CCharString 0x0099EBF0 / 0x0099EC30 / ~ 0x0099EAE0, IsActiveThreadTerminating 0x00CB7940, operator new 0x00BFEA1A,
//   CSpawnedFuncBase 0x00CDD450 (CSpawnedFunc<CV_GhostGrannyNecklaceScript> vtable 0x012EBA50), AddSpawnedFunction 0x00CB7E50,
//   AddEntityScriptBinding 0x00CB8230 (binding vtable 0x012EBAF8), CScriptBase::PostAddScriptedEntities 0x00CB8930;
//   allocators 0x00E8BD30 / 0x00E8BE10 / 0x00E8CA60; threads CreateBandit 0x00E8CB40, GhostGrannyHints 0x00E8D2D0;
//   CGameScriptInterface slots 7, 12, 64, 281 DeactivateQuestLater, 288 SetQuestAsCompleted, 655 GetActiveQuestName.
// Layout (retail = Ego_r.pdb - 0x14): Game +0x40, NecklaceReturned +0x4a.
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

class CGameScriptInterface;
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#include "engine/CScriptBase.h"
struct CV_GhostGrannyNecklaceScript : CScriptBase {
    struct CGhostGrannyNecklace {
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CV_GhostGrannyNecklaceScript& parent, CScriptDataBase* data, const CScriptThing& thing);   // 0x00E8BD30
    };
    struct CGhostGrannyDaughterInLaw {
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CV_GhostGrannyNecklaceScript& parent, CScriptDataBase* data, const CScriptThing& thing);   // 0x00E8BE10
    };
    struct CGhostGrannySon {
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CV_GhostGrannyNecklaceScript& parent, CScriptDataBase* data, const CScriptThing& thing);   // 0x00E8CA60
    };
    CGameScriptInterface* Game;                          // +0x40
    void* PMasterData;                                   // +0x44
    bool WifeNeededForCutscene;                          // +0x48
    bool HeroKnowsAboutNecklace;                         // +0x49
    bool NecklaceReturned;                               // +0x4a
    void Main();
    void CreateBandit();                                 // 0x00E8CB40
    void GhostGrannyHints();                             // 0x00E8D2D0
};

void CV_GhostGrannyNecklaceScript::Main()
{
    AddEntityScriptBinding(new CEntityScriptBinding<CV_GhostGrannyNecklaceScript>("GhostGrannyNecklace", this, &CGhostGrannyNecklace::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_GhostGrannyNecklaceScript>("GhostGrannyDaughterInLaw", this, &CGhostGrannyDaughterInLaw::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_GhostGrannyNecklaceScript>("GhostGrannySon", this, &CGhostGrannySon::Alloc, 0));
    PostAddScriptedEntities();
    Game->PostAddScriptedEntities();
    AddSpawnedFunction(new CSpawnedFunc<CV_GhostGrannyNecklaceScript>("CreateBandit", 0, this, &CV_GhostGrannyNecklaceScript::CreateBandit), CCharString(""));
    AddSpawnedFunction(new CSpawnedFunc<CV_GhostGrannyNecklaceScript>("GhostGrannyHints", 0, this, &CV_GhostGrannyNecklaceScript::GhostGrannyHints), CCharString(""));
    while (!NecklaceReturned) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->SetQuestAsCompleted(Game->GetActiveQuestName(), false, true, false);
    while (!!Game->IsRegionLoaded("OrchardFarm")) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->DeactivateQuestLater(Game->GetActiveQuestName(), 0);
}