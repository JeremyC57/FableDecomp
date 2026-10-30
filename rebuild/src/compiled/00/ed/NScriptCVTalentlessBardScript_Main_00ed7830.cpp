// NScript::CV_TalentlessBardScript::Main() -- the Talentless Bard quest main: bind the TB_BardSpawnPoint and TB_Bard entity scripts, post-add the scripted entities, spawn the WatchForCompletedQuests thread, wait until the bard is finished, then complete the active quest and deactivate it
// Leaf: ?Main@CV_TalentlessBardScript@NScript@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00ED7830, 497 bytes, thiscall, plain `ret`.
// Identity (proven): slot 2 (Main) of the quest-script vtable 0x012F3428 [dtor 0x00EDACF0, RegisterMain 0x00ED7790 (landed
//   NScript::CV_TalentlessBardScript::RegisterMain), Main 0x00ED7830, Init 0x00ED83C0, OnPersist 0x00ED85A0 (landed)], which the
//   landed ctor 0x00ED76E0 stores (site 0x00ED7700); the row is referenced only from that slot. Ego_r.pdb
//   ?Main@CV_TalentlessBardScript@NScript@@UAEXXZ (public virtual). The binding factories are CTB_BardSpawnPoint::Alloc 0x00ED8050
//   (landed) and CTB_Bard::Alloc 0x00ED7CD0 (new 0x2C + the out-of-line CTB_Bard ctor 0x00ED7D50), each paired with its
//   "TB_BardSpawnPoint" / "TB_Bard" name string; the spawned thread is ?WatchForCompletedQuests@CV_TalentlessBardScript@NScript@@
//   QAEXXZ 0x00ED7A40, paired with its "WatchForCompletedQuests" string. Catalog `sub_00ed7830`.
// Callees: operator new 0x00BFEA1A, CCharString(const char*, long) 0x0099EBF0 / copy 0x0099EC30 / ~ 0x0099EAE0,
//   CScriptBase::AddEntityScriptBinding 0x00CB8230 / PostAddScriptedEntities 0x00CB8930 / AddSpawnedFunction 0x00CB7E50 /
//   IsActiveThreadTerminating 0x00CB7940, CSpawnedFuncBase ctor 0x00CDD450; vtables CEntityScriptBinding<CV_TalentlessBardScript>
//   0x012F3478, CSpawnedFunc<CV_TalentlessBardScript> 0x012F343C; CGameScriptInterface slots 7 NewScriptFrame,
//   64 PostAddScriptedEntities, 281 DeactivateQuestLater, 288 SetQuestAsCompleted, 655 GetActiveQuestName
//   (ghidra_out/cgsi_slots_aligned.tsv).
// Literals: "TB_BardSpawnPoint" 0x012F3480, "TB_Bard" 0x012F3470, "WatchForCompletedQuests" 0x012F3458, "" 0x0122D70E (paired
//   with retail by relocation, rel.py).
// Layout (retail = Ego_r.pdb - 0x14, CScriptBase being 0x40 in retail): Game +0x40, PMasterData +0x44, bHurtBard +0x48,
//   bBard_finished +0x49 (Ego_r 0x5C / 0x5D).
// Model: the landed NScript::CQS_GuardianTrophyDealerInfoScript::Main 0x00E27920 (bindings, retail CSpawnedFuncBase through the
//   FABLE_RETAIL_SUSPENDABLE_PROCESS opt-in, SpawnFunction) plus the rotated wait of CV_RockTrollFirstEncounterScript::Main
//   0x00EC3D40. `cmp byte ptr [esi+49h],1` is `bBard_finished != true`. The unread `mov [esp+14h],ebx` at entry (the
//   conditional-temporary flags' home) comes out of VC7.1 by itself here. First grade.
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
    bool IsActiveThreadTerminating();                                                 // 0x00CB7940
    __forceinline void SpawnFunction(CSpawnedFuncBase* func, const char* regionName = "") { AddSpawnedFunction(func, CCharString(regionName)); }
};
namespace NScript {
class CV_TalentlessBardScript : public CScriptBase {
public:
    class CTB_Bard {
    public:
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CV_TalentlessBardScript& parent, CScriptDataBase* data, const CScriptThing& thing);   // 0x00ED7CD0
    };
    class CTB_BardSpawnPoint {
    public:
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CV_TalentlessBardScript& parent, CScriptDataBase* data, const CScriptThing& thing);   // 0x00ED8050
    };
    CGameScriptInterface* Game;                         // +0x40
    void* PMasterData;                                  // +0x44
    bool bHurtBard;                                     // +0x48
    bool bBard_finished;                                // +0x49
    virtual void Main();
    void WatchForCompletedQuests();                     // 0x00ED7A40
};

void CV_TalentlessBardScript::Main()
{
    AddEntityScriptBinding(new CEntityScriptBinding<CV_TalentlessBardScript>("TB_BardSpawnPoint", this, &CTB_BardSpawnPoint::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_TalentlessBardScript>("TB_Bard", this, &CTB_Bard::Alloc, 0));
    PostAddScriptedEntities();
    Game->PostAddScriptedEntities();
    SpawnFunction(new CSpawnedFunc<CV_TalentlessBardScript>("WatchForCompletedQuests", 0, this, &CV_TalentlessBardScript::WatchForCompletedQuests));
    while (bBard_finished != true) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->SetQuestAsCompleted(Game->GetActiveQuestName(), true, true, false);
    Game->DeactivateQuestLater(Game->GetActiveQuestName(), 0);
}
}   // namespace NScript