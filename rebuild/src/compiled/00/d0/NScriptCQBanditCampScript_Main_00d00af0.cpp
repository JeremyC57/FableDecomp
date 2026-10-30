// CQ_BanditCampScript::Main() -- the bandit camp quest's main: bind the two gate-guard entity scripts, finish adding scripted entities, on first run set the quest card and activate the Oakvale bandit raid, spawn the four watcher threads, wait for gate 1 to open (then autosave), and take the camp pass and the hostage key from the hero if he carries them
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D00AF0, length 1234 bytes.
// Callees: operator new 0x00BFEA1A, CCharString(const char*, long) 0x0099EBF0 / CCharString(const CCharString&) 0x0099EC30
//   / ~CCharString 0x0099EAE0, CScriptBase::IsActiveThreadTerminating 0x00CB7940, AddEntityScriptBinding 0x00CB8230,
//   PostAddScriptedEntities 0x00CB8930, CSpawnedFuncBase::CSpawnedFuncBase 0x00CDD450 (CSpawnedFunc vtable 0x012C83E8),
//   AddSpawnedFunction 0x00CB7E50; binding allocators 0x00D014C0 (Gate1GuardOuter), 0x00D02A70 (Gate1GuardInner) (binding
//   vtable 0x012C850C); threads WatchForTermination 0x00D01290, WatchForCostume 0x00D00FD0 (landed), CheckAnyBanditsKilled
//   0x00D032F0 (landed), WatchForEndOfScript 0x00D013A0; the game script interface (+0x40) slots 7, 64, 70, 125, 184, 276,
//   296, 703.
// Layout (retail = Ego_r.pdb - 0x14, as the landed CQ_BanditCampScript::Init 0x00D006D0): Gate1Open +0x49,
//   ActivatedBanditRaid +0x4d.
// Shape: the landed CQ_GuildTrainingWoodsMeleeScript::Main 0x00D66620 model (one flag word guarding the name temporaries);
//   generated script code (`if (IsActiveThreadTerminating()) return;` after every statement).
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (struct->class; already NScript). Same bytes.
class C3DVector { public: float X, Y, Z; };
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
};
void* __cdecl operator new(unsigned int size);          // 0x00BFEA1A
class CScriptThing { public:                                   // 12 bytes, vtable 0x01238C8C
    virtual void t000(); virtual void t001(); virtual void t002(); virtual void t003(); virtual void t004(); virtual void t005();
    virtual const C3DVector& GetPos() const;            // slot 6 (+0x18) 0x004AA980
    void* PImpData;
    void* PImpInfo;
    virtual ~CScriptThing();                                    // 0x004AA840
};
// Layout: CSpawnedFuncBase is the retail polymorphic suspendable-process chain from the shared headers
//   (FABLE_RETAIL_SUSPENDABLE_PROCESS opt-in: CSpawnedFuncBase : CSuspendableProcessMicrothread : CASuspendableProcess :
//   CBaseClassNonCopyable : CBaseClass, 0x34 bytes, vtable 0x012C2F9C; ctor 0x00CDD450).
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#include "engine/CSpawnedFuncBase.h"
#include "engine/CSpawnedFunc.h"                         // vtable per instantiation; ctor inline, slot 4 SuspendableProcess
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
class CQ_BanditCampScript : public CScriptBase {
public:
    struct CGate1GuardOuter { static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CQ_BanditCampScript& parent, CScriptDataBase* data, const CScriptThing& thing); };   // 0x00D014C0
    struct CGate1GuardInner { static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CQ_BanditCampScript& parent, CScriptDataBase* data, const CScriptThing& thing); };   // 0x00D02A70
    CGameScriptInterface* GSI;                          // +0x40
    void*                 Data;                         // +0x44
    bool AttackedOuterGateGuards;                       // +0x48
    bool Gate1Open;                                     // +0x49
    bool BoastBegun;                                    // +0x4a
    bool MissionSucceeded;                              // +0x4b
    bool MissionFailed;                                 // +0x4c
    bool ActivatedBanditRaid;                           // +0x4d
    void Main();
    void WatchForTermination();                         // 0x00D01290
    void WatchForCostume();                             // 0x00D00FD0
    void CheckAnyBanditsKilled();                       // 0x00D032F0
    void WatchForEndOfScript();                         // 0x00D013A0
};
void CQ_BanditCampScript::Main()
{
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_BanditCampScript>("Gate1GuardOuter", this, &CGate1GuardOuter::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_BanditCampScript>("Gate1GuardInner", this, &CGate1GuardInner::Alloc, 0));
    PostAddScriptedEntities();
    GSI->PostAddScriptedEntities();
    if (!ActivatedBanditRaid) {
        if (IsActiveThreadTerminating())
            return;
        GSI->SetQuestCardObjective("Q_BanditCamp", "TEXT_QUEST_BANDIT_CAMP_OBJECTIVE_01", "", "");
        GSI->ActivateQuest("Q_OakValeBanditRaid");
        ActivatedBanditRaid = true;
    }
    AddSpawnedFunction(new CSpawnedFunc<CQ_BanditCampScript>("WatchForTermination", 0, this, &CQ_BanditCampScript::WatchForTermination), CCharString(""));
    AddSpawnedFunction(new CSpawnedFunc<CQ_BanditCampScript>("WatchForCostume", 0, this, &CQ_BanditCampScript::WatchForCostume), CCharString(""));
    AddSpawnedFunction(new CSpawnedFunc<CQ_BanditCampScript>("CheckAnyBanditsKilled", 0, this, &CQ_BanditCampScript::CheckAnyBanditsKilled), CCharString(""));
    AddSpawnedFunction(new CSpawnedFunc<CQ_BanditCampScript>("WatchForEndOfScript", 0, this, &CQ_BanditCampScript::WatchForEndOfScript), CCharString(""));
    if (!Gate1Open) {
        if (IsActiveThreadTerminating())
            return;
        while (!Gate1Open) {
            GSI->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        GSI->AutoSaveCheckPoint();
    }
    if (GSI->IsObjectInThingsPossession("OBJECT_RESIDENTIAL_BANDIT_CAMP_PASS", GSI->GetHero())) {
        if (IsActiveThreadTerminating())
            return;
        GSI->TakeObjectFromHero("OBJECT_RESIDENTIAL_BANDIT_CAMP_PASS");
    }
    if (GSI->IsObjectInThingsPossession("OBJECT_BANDIT_CAMP_HOSTAGE_KEY", GSI->GetHero())) {
        if (IsActiveThreadTerminating())
            return;
        GSI->TakeObjectFromHero("OBJECT_BANDIT_CAMP_HOSTAGE_KEY");
    }
}
}