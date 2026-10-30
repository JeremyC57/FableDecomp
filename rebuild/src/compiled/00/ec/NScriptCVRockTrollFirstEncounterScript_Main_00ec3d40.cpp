// NScript::CV_RockTrollFirstEncounterScript::Main() -- the Rock Troll first-encounter quest main: bind the RTFE_Sparrow, RTFE_RockTroll and M_RTFERockTrollTrigger entity scripts, wait for Witchwood1 to load, spawn WatchForRegionExit, activate V_RockTrollFirstEncounter_Activate once, wait until the mission is over, drop the troll's health bar and the quest info, wait for the next region-loaded message, re-enable Witchwood1's creature generators and deactivate both quests
// Leaf: ?Main@CV_RockTrollFirstEncounterScript@NScript@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00EC3D40, 994 bytes (+ int3 padding), thiscall, plain `ret`.
// Identity (proven): slot 2 (Main) of the quest-script vtable 0x012F15C4, bound as ??_7CV_RockTrollFirstEncounterScript@NScript@@6B@
//   (its RTTI-less vtable; slot 1 0x00EC3CA0, slot 3 0x00EC3C50); Ego_r.pdb ?Main@CV_RockTrollFirstEncounterScript@NScript@@UAEXXZ.
//   The three entity-script factories it registers are the class's own Alloc statics (Ego_r donor order
//   CM_RTFERockTrollTrigger / CRTFE_RockTroll / CRTFE_Sparrow = retail 0x00EC41F0 / 0x00EC4490 / 0x00EC45C0), and the
//   thread it spawns is ?WatchForRegionExit@CV_RockTrollFirstEncounterScript@NScript@@QAEXXZ (0x00EC4130, spawn vtable
//   0x012F15D8 = ??_7?$CSpawnedFunc@VCV_RockTrollFirstEncounterScript@NScript@@@@6B@).
// Label: catalog "sub_00ec3d40" (gap scan).
// Callees: operator new 0x00BFEA1A, CCharString() 0x0099E4B0 / (const char*, long) 0x0099EBF0 / copy 0x0099EC30 /
//   ~ 0x0099EAE0, CScriptBase::IsActiveThreadTerminating 0x00CB7940 / AddSpawnedFunction 0x00CB7E50 /
//   AddEntityScriptBinding 0x00CB8230 / PostAddScriptedEntities 0x00CB8930, CSpawnedFuncBase ctor 0x00CDD450;
//   CGameScriptInterface slots 7 NewScriptFrame, 12 IsRegionLoaded, 22 MsgOnRegionLoaded, 64 PostAddScriptedEntities,
//   276 ActivateQuest, 281 DeactivateQuestLater, 321 DisplayQuestInfo, 338 RemoveQuestInfoElement,
//   558 SetCreatureGeneratorsEnabled, 655 GetActiveQuestName (ghidra_out/cgsi_slots_aligned.tsv).
// Literals: "RTFE_Sparrow" 0x012F165C, "RTFE_RockTroll" 0x012F1644, "M_RTFERockTrollTrigger" 0x012F162C, "Witchwood1"
//   0x012CC268, "WatchForRegionExit" 0x012F1618, "" 0x0122D70E, "V_RockTrollFirstEncounter_Activate" 0x012F15F4.
// Layout (retail = Ego_r.pdb - 0x14, the CScriptBase containers being STLport-sized): Game +0x40, PMasterData +0x44,
//   MissionSucceeded +0x48 .. RockTrollTriggered +0x4E, RockTrollHealthBarID +0x50, PlayedExhumeCutScene +0x54,
//   AddedItemsToRockTroll +0x55 (0x58 bytes).
// Model: the landed NScript::CV_SwordInTheStoneScript::Main 0x00ED4D20 (CEntityScriptBinding / CSpawnedFunc / SpawnFunction).
class CCharStringData;
class CCharString {
public:
    CCharStringData* PStringData;
    CCharString();                                      // 0x0099E4B0
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
template <class T> class CCountedPointer {
public:
    T* Data;
    void* Info;
};
class __declspec(novtable) CEntityScriptBindingBase {
public:
    virtual const CCharString& GetEntityScriptName() const = 0;
};
template <class T> class CEntityScriptBinding : public CEntityScriptBindingBase {
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
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
class CScriptBase {                                     // retail 0x40 bytes
public:
    virtual ~CScriptBase();
    virtual void Init();                                // slot 1
    virtual void Main() = 0;                            // slot 2
    unsigned char _body[0x3c];                          // +0x04 .. +0x40
    bool IsActiveThreadTerminating();                                                 // 0x00CB7940
    void AddSpawnedFunction(CSpawnedFuncBase* func, const CCharString& regionName);   // 0x00CB7E50
    void AddEntityScriptBinding(CEntityScriptBindingBase* binding);                   // 0x00CB8230
    void PostAddScriptedEntities();                                                   // 0x00CB8930
    __forceinline void SpawnFunction(CSpawnedFuncBase* func, const char* regionName = "") { AddSpawnedFunction(func, CCharString(regionName)); }
};
namespace NScript {
class CV_RockTrollFirstEncounterScript : public CScriptBase {
public:
    class CM_RTFERockTrollTrigger {
    public:
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CV_RockTrollFirstEncounterScript& parent, CScriptDataBase* data, const CScriptThing& thing);   // 0x00EC41F0
    };
    class CRTFE_RockTroll {
    public:
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CV_RockTrollFirstEncounterScript& parent, CScriptDataBase* data, const CScriptThing& thing);   // 0x00EC4490
    };
    class CRTFE_Sparrow {
    public:
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CV_RockTrollFirstEncounterScript& parent, CScriptDataBase* data, const CScriptThing& thing);   // 0x00EC45C0
    };
    CGameScriptInterface* Game;                         // +0x40
    void* PMasterData;                                  // +0x44
    bool MissionSucceeded;                              // +0x48
    bool MissionFailed;                                 // +0x49
    bool MissionOver;                                   // +0x4a
    bool TrollAlive;                                    // +0x4b
    bool TrollAwake;                                    // +0x4c
    bool Activated;                                     // +0x4d
    bool RockTrollTriggered;                            // +0x4e
    long RockTrollHealthBarID;                          // +0x50
    bool PlayedExhumeCutScene;                          // +0x54
    bool AddedItemsToRockTroll;                         // +0x55
    virtual void Main();
    void WatchForRegionExit();                          // 0x00EC4130
};

void CV_RockTrollFirstEncounterScript::Main()
{
    AddEntityScriptBinding(new CEntityScriptBinding<CV_RockTrollFirstEncounterScript>("RTFE_Sparrow", this, &CRTFE_Sparrow::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_RockTrollFirstEncounterScript>("RTFE_RockTroll", this, &CRTFE_RockTroll::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_RockTrollFirstEncounterScript>("M_RTFERockTrollTrigger", this, &CM_RTFERockTrollTrigger::Alloc, 0));
    PostAddScriptedEntities();
    Game->PostAddScriptedEntities();
    while (!Game->IsRegionLoaded("Witchwood1")) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    SpawnFunction(new CSpawnedFunc<CV_RockTrollFirstEncounterScript>("WatchForRegionExit", 0, this, &CV_RockTrollFirstEncounterScript::WatchForRegionExit));
    if (!Activated) {
        if (IsActiveThreadTerminating())
            return;
        Game->ActivateQuest("V_RockTrollFirstEncounter_Activate");
        Activated = true;
    }
    while (!MissionOver) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->RemoveQuestInfoElement(RockTrollHealthBarID);
    Game->DisplayQuestInfo(false);
    CCharString region;
    while (!Game->MsgOnRegionLoaded(region)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->SetCreatureGeneratorsEnabled("Witchwood1", true);
    Game->DeactivateQuestLater("V_RockTrollFirstEncounter_Activate", 0);
    Game->DeactivateQuestLater(Game->GetActiveQuestName(), 0);
}
}   // namespace NScript