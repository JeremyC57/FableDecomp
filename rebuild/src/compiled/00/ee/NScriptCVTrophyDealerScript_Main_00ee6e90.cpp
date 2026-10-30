// NScript::CV_TrophyDealerScript::Main() -- the Trophy Dealer quest main: switch off Witchwood2's creature generators, bind the TrophyDealerInCave and DemonDoorFace entity scripts, activate V_SingingStones without loading resources, spawn HilightGuildTeleporter, wait for Witchwood1 to load, then set the quest card objective (FIND_TROPHY_DEALER_OBJECTIVE_02, Witchwood2) and kick off the V_TrophyDealer quest start screen
// Leaf: ?Main@CV_TrophyDealerScript@NScript@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EE6E90, 767 bytes, thiscall, plain `ret`.
// Identity (proven): slot 2 (Main) of the quest-script vtable 0x012F51F4 [dtor 0x00EE80E0, RegisterMain 0x00EE6DF0 (landed as
//   NScript::CV_TrophyDealerScript::RegisterMain), Main 0x00EE6E90, Init 0x00EE6D60 (landed ...::Init), OnPersist 0x00CBD4E0];
//   Ego_r.pdb 0x00F5FA70 ?Main@CV_TrophyDealerScript@NScript@@UAEXXZ. The two binding factories are the class's landed Alloc
//   statics (CTrophyDealerInCave 0x00EE7340, CDemonDoorFace 0x00EE7430; Ego_r donor order 0x00F5F6D0 / 0x00F5F7D0) and the
//   spawned thread is the PDB member HilightGuildTeleporter 0x00EE71A0 (landed; spawn string "HilightGuildTeleporter").
//   Catalog label: sub_00ee6e90 (gap scan).
// Callees: operator new 0x00BFEA1A, CCharString(const char*, long) 0x0099EBF0 / copy 0x0099EC30 / ~ 0x0099EAE0,
//   CScriptBase::IsActiveThreadTerminating 0x00CB7940 / AddSpawnedFunction 0x00CB7E50 / AddEntityScriptBinding 0x00CB8230 /
//   PostAddScriptedEntities 0x00CB8930, CSpawnedFuncBase ctor 0x00CDD450; vtables CEntityScriptBinding<T> 0x012F52A4,
//   CSpawnedFunc<T> 0x012F5234; CGameScriptInterface slots 7 NewScriptFrame, 12 IsRegionLoaded, 64 PostAddScriptedEntities,
//   278 ActivateQuestWithoutLoadingResources, 296 SetQuestCardObjective, 309 KickOffQuestStartScreen,
//   558 SetCreatureGeneratorsEnabled, 655 GetActiveQuestName (ghidra_out/cgsi_slots_aligned.tsv).
// Literals: "Witchwood2" 0x012DCEF8, "TrophyDealerInCave" 0x012F52AC, "DemonDoorFace" 0x012F5294, "V_SingingStones"
//   0x012F2DDC, "HilightGuildTeleporter" 0x012F527C, "" 0x0122D70E, "Witchwood1" 0x012CC268,
//   "TEXT_QUEST_FIND_TROPHY_DEALER_OBJECTIVE_02" 0x012F5250, "V_TrophyDealer" 0x012C5200.
// Layout (retail = Ego_r.pdb - 0x14): Game +0x40, PMasterData +0x44; Main reads no member of its own.
// Model: the landed NScript::CV_RockTrollFirstEncounterScript::Main 0x00EC3D40 (CEntityScriptBinding / CSpawnedFunc /
//   SpawnFunction, polymorphic CScriptBase, CGameScriptInterfaceBase keeping its padding virtuals).
class CCharStringData;
class CCharString {
public:
    CCharStringData* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
};
void* __cdecl operator new(unsigned int bytes);         // 0x00BFEA1A (MSVCR71)
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
    bool IsActiveThreadTerminating();                                                 // 0x00CB7940
    void AddSpawnedFunction(CSpawnedFuncBase* func, const CCharString& regionName);   // 0x00CB7E50
    void AddEntityScriptBinding(CEntityScriptBindingBase* binding);                   // 0x00CB8230
    void PostAddScriptedEntities();                                                   // 0x00CB8930
    __forceinline void SpawnFunction(CSpawnedFuncBase* func, const char* regionName = "") { AddSpawnedFunction(func, CCharString(regionName)); }
};
namespace NScript {
class CV_TrophyDealerScript : public CScriptBase {
public:
    class CTrophyDealerInCave {
    public:
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CV_TrophyDealerScript& parent, CScriptDataBase* data, const CScriptThing& thing);   // 0x00EE7340
    };
    class CDemonDoorFace {
    public:
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CV_TrophyDealerScript& parent, CScriptDataBase* data, const CScriptThing& thing);   // 0x00EE7430
    };
    CGameScriptInterface* Game;                         // +0x40
    void* PMasterData;                                  // +0x44
    virtual void Main();
    void HilightGuildTeleporter();                      // 0x00EE71A0
};

void CV_TrophyDealerScript::Main()
{
    Game->SetCreatureGeneratorsEnabled("Witchwood2", false);
    AddEntityScriptBinding(new CEntityScriptBinding<CV_TrophyDealerScript>("TrophyDealerInCave", this, &CTrophyDealerInCave::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_TrophyDealerScript>("DemonDoorFace", this, &CDemonDoorFace::Alloc, 0));
    PostAddScriptedEntities();
    Game->PostAddScriptedEntities();
    Game->ActivateQuestWithoutLoadingResources("V_SingingStones");
    SpawnFunction(new CSpawnedFunc<CV_TrophyDealerScript>("HilightGuildTeleporter", 0, this, &CV_TrophyDealerScript::HilightGuildTeleporter));
    while (!Game->IsRegionLoaded("Witchwood1")) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->SetQuestCardObjective(Game->GetActiveQuestName(), "TEXT_QUEST_FIND_TROPHY_DEALER_OBJECTIVE_02", "Witchwood2", "");
    Game->KickOffQuestStartScreen("V_TrophyDealer", true, false);
}
}   // namespace NScript