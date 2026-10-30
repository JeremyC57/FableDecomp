// NScript::CQS_GuardianTrophyDealerInfoScript::Main() -- the Guardian-trophy-dealer info quest main: bind the GTDI_Maze entity script, post-add the scripted entities, spawn the WaitForPieceOver thread, then set the quest card objective (TEXT_QUEST_GUARDIAN_TROPHY_DEALER_INFO_OBJECTIVE_01, HeroGuildComplexInside twice)
// Leaf: ?Main@CQS_GuardianTrophyDealerInfoScript@NScript@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E27920, 390 bytes, thiscall, plain `ret`.
// Identity (proven): slot 2 (Main) of the quest-script vtable 0x012E2018 [dtor 0x00E28CE0, RegisterMain 0x00E27880 (landed
//   NScript::CQS_GuardianTrophyDealerInfoScript::RegisterMain, which spawns slot 2 as "Main"), Main 0x00E27920, Init
//   0x00E27870, OnPersist 0x00E27E60]; the only reference to the row is that slot. Ego_r.pdb
//   ?Main@CQS_GuardianTrophyDealerInfoScript@NScript@@UAEXXZ (donor 0x00E73730, after RegisterMain 0x00E73600 as here). The
//   binding factory is the landed CGTDI_Maze::Alloc 0x00E27BB0, the spawned thread the landed WaitForPieceOver 0x00E27AC0
//   (Ego_r.pdb ?WaitForPieceOver@CQS_GuardianTrophyDealerInfoScript@NScript@@QAEXXZ, paired with its "WaitForPieceOver"
//   name string here). Catalog label: sub_00e27920.
// Callees: operator new 0x00BFEA1A, CCharString(const char*, long) 0x0099EBF0 / copy 0x0099EC30 / ~ 0x0099EAE0,
//   CScriptBase::AddEntityScriptBinding 0x00CB8230 / PostAddScriptedEntities 0x00CB8930 / AddSpawnedFunction 0x00CB7E50,
//   CSpawnedFuncBase ctor 0x00CDD450; vtables CEntityScriptBinding<T> 0x012E2090, CSpawnedFunc<T> 0x012E202C;
//   CGameScriptInterface slots 64 PostAddScriptedEntities, 296 SetQuestCardObjective, 655 GetActiveQuestName
//   (ghidra_out/cgsi_slots_aligned.tsv).
// Literals: "GTDI_Maze" 0x012E2098, "WaitForPieceOver" 0x012E207C, "" 0x0122D70E, "HeroGuildComplexInside" 0x01238FCC (twice),
//   "TEXT_QUEST_GUARDIAN_TROPHY_DEALER_INFO_OBJECTIVE_01" 0x012E2048.
// Layout: CScriptBase retail 0x40 bytes (polymorphic), Game +0x40; CEntityScriptBinding 0x1c bytes (Flags 0); CSpawnedFunc 0x3c.
// Model: the landed NScript::CV_IntroductionToTrophiesScript::Main 0x00E9C4B0 (binding / CSpawnedFunc / SpawnFunction).
// Shape: each new-expression's name string is a conditional temporary with its own ebx flag bit; the region name of
//   SpawnFunction is built after the new-expression; the SetQuestCardObjective strings are built right to left.
class CCharStringData;
class CCharString {
public:
    CCharStringData* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
};
void* __cdecl operator new(unsigned int bytes);         // 0x00BFEA1A (MSVCR71)
// Layout: the retail polymorphic suspendable-process chain from the shared headers (FABLE_RETAIL_SUSPENDABLE_PROCESS
//   opt-in: CSpawnedFuncBase : CSuspendableProcessMicrothread : CASuspendableProcess : CBaseClassNonCopyable : CBaseClass,
//   0x34 bytes, vtable 0x012C2F9C).
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#include "engine/CSpawnedFuncBase.h"
#include "engine/CSpawnedFunc.h"                         // vtable per instantiation; ctor inline, slot 4 SuspendableProcess
class CScriptThing;
class CScriptDataBase;
class CEntityScriptBase;
#include "engine/CCountedPointer.h"
class __declspec(novtable) CEntityScriptBindingBase {
public:
    virtual const CCharString& GetEntityScriptName() const = 0;
};
template <class T> class CEntityScriptBinding : public CEntityScriptBindingBase {   // 0x1c bytes
public:
    typedef CCountedPointer<CEntityScriptBase> (__fastcall *TAllocFunc)(T& parent, CScriptDataBase* data, const CScriptThing& thing);
    CCharString     EntityScriptName;                   // +0x04
    T*              PParentScript;                      // +0x08
    void (T::*Func)();                                  // +0x0c
    TAllocFunc      ClassAllocFunc;                     // +0x10
    bool            ClassAllocValid;                    // +0x14
    unsigned long   Flags;                              // +0x18
    CEntityScriptBinding(const CCharString& name, T* parent, TAllocFunc alloc, unsigned long flags)
        : EntityScriptName(name), PParentScript(parent), ClassAllocFunc(alloc), ClassAllocValid(true), Flags(flags) { }
    virtual const CCharString& GetEntityScriptName() const { return EntityScriptName; }
};
// CGameScriptInterfaceBase / CGameScriptInterface are the retail script interface from the shared headers
//   (FABLE_RETAIL_NSCRIPT opt-in: CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
class CScriptBase {                                     // retail 0x40 bytes
public:
    virtual ~CScriptBase();
    virtual void RegisterMain();                        // slot 1
    virtual void Main() = 0;                            // slot 2
    virtual void Init();                                // slot 3
    unsigned char _body[0x3c];                          // +0x04 .. +0x40
    void AddEntityScriptBinding(CEntityScriptBindingBase* binding);                   // 0x00CB8230
    void PostAddScriptedEntities();                                                   // 0x00CB8930
    void AddSpawnedFunction(CSpawnedFuncBase* func, const CCharString& regionName);   // 0x00CB7E50
    __forceinline void SpawnFunction(CSpawnedFuncBase* func, const char* regionName = "") { AddSpawnedFunction(func, CCharString(regionName)); }
};
namespace NScript {
class CQS_GuardianTrophyDealerInfoScript : public CScriptBase {
public:
    class CGTDI_Maze {
    public:
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CQS_GuardianTrophyDealerInfoScript& parent, CScriptDataBase* data, const CScriptThing& thing);   // 0x00E27BB0
    };
    CGameScriptInterface* Game;                         // +0x40
    virtual void Main();
    void WaitForPieceOver();                            // 0x00E27AC0
};

void CQS_GuardianTrophyDealerInfoScript::Main()
{
    AddEntityScriptBinding(new CEntityScriptBinding<CQS_GuardianTrophyDealerInfoScript>("GTDI_Maze", this, &CGTDI_Maze::Alloc, 0));
    PostAddScriptedEntities();
    Game->PostAddScriptedEntities();
    SpawnFunction(new CSpawnedFunc<CQS_GuardianTrophyDealerInfoScript>("WaitForPieceOver", 0, this, &CQS_GuardianTrophyDealerInfoScript::WaitForPieceOver));
    Game->SetQuestCardObjective(Game->GetActiveQuestName(), "TEXT_QUEST_GUARDIAN_TROPHY_DEALER_INFO_OBJECTIVE_01", "HeroGuildComplexInside", "HeroGuildComplexInside");
}
}   // namespace NScript