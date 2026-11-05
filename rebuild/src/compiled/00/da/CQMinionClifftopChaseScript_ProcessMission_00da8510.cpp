// CQ_MinionClifftopChaseScript::ProcessMission() -- walk the chase objectives: wait for Deserted Farm, set objective 3, wait for Gibbet Woods, set objective 4, wait for the troll flag and let a guard comment on it, wait for the prison path, start the escape timer (HUD clock) with objective 5, wait for the map keeper, a failure or the timer to run out, and fail the mission on time-out
// Leaf: ?ProcessMission@CQ_MinionClifftopChaseScript@@
// Plan phase 5 (2026-11-05), CScriptThing wave: the local CScriptThing stand-in replaced by the shared engine/CScriptThing.h under FABLE_RETAIL_NSCRIPT (RTTI CScriptThing : CBaseClass, vtable 0x01238C8C, PImp +0x04; ~CScriptThing 0x004AA840 and the copy 0x004ABE90 declared out of line). Also: the engine/CCountedPointer.h include moved up to the header include (the header needs it first).
// Naming wave 29 (family sfbh): engine/CSpawnedFuncBase.h now carries retail's implicit destructor and the real CCharString /
//   CCountedPointer<const NScript::CPredicateBase> members; header contract met here: engine/CCountedPointer.h included before it.
// Naming wave 18: the global stand-in g_pGameScriptInterface (0x0143E8F8) is pDSTGame, a CGameScriptInterfaceBase* (ESTABLISHED_SPELLINGS; the
//   virtual slots are the base interface's) -- name-only (the bytes are unchanged).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DA8510, 1110 bytes (int3 padded to 1120).
// Label: the thread named "ProcessMission" (0x012D7194) is spawned at 0x00DA3E62 with this body; the landed 0x00DA3F40
//   catalogued as ProcessMission is a different thread body.
// Callees: CCharString 0x0099EBF0 / ~ 0x0099EAE0, IsActiveThreadTerminating 0x00CB7940, GuardComment 0x00DA8970 (by-value
//   comment); pDSTGame 0x0143E8F8 timer slots 89 SetTimer / 90 GetTimer; CGameScriptInterface slots 7, 12,
//   296 SetQuestCardObjective, 321 DisplayQuestInfo, 326 AddQuestInfoTimer, 655 GetActiveQuestName, 697 StopOverrideMusic;
//   pGlobalScriptDef 0x0143E90C (+0xd54 escape time).
// Layout (retail = Ego_r.pdb - 0x14, as the landed CQ_MinionClifftopChaseScript sources): MapKeeperSaidDialog +0x48,
//   MissionFailed +0x49, TrollFlag +0x54, EscapeTimer +0x5c (CTimer), TimerBarID +0x60, ReasonForFailure +0x64.
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
    const CCharString& operator=(const char* text);           // 0x0099EFE0
};
#define FABLE_RETAIL_NSCRIPT
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
class CWideString { public:
    void* PStringData;
    CWideString(const unsigned short* text);            // 0x0099B6B0
    ~CWideString();                                     // 0x0099B510
};
// Layout: CSpawnedFuncBase is the retail polymorphic suspendable-process chain from the shared headers
//   (FABLE_RETAIL_SUSPENDABLE_PROCESS opt-in: CSpawnedFuncBase : CSuspendableProcessMicrothread : CASuspendableProcess :
//   CBaseClassNonCopyable : CBaseClass, 0x34 bytes, vtable 0x012C2F9C; ctor 0x00CDD450).
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#include "engine/CSpawnedFuncBase.h"
#include "engine/CSpawnedFunc.h"                         // vtable per instantiation; ctor inline, slot 4 SuspendableProcess

class CScriptDataBase;
class CEntityScriptBase;
class CActiveEntityScriptBase;
#include "engine/CCountedPointer.h"

struct __declspec(novtable) CEntityScriptBindingBase {
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

class CTimer;
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
extern CGameScriptInterfaceBase* pDSTGame;   // 0x0143E8F8 ?pDSTGame@@3PAVCGameScriptInterfaceBase@@A (ESTABLISHED_SPELLINGS: the SetDSTGame writer)
class CTimer { public:                                         // a script timer: the interface's timer index
    long Index;
    __forceinline void Set(long time) { pDSTGame->SetTimer(Index, time); }
    __forceinline long Get() const { return pDSTGame->GetTimer(Index); }
};
class CScriptDef { public:
    unsigned char _pad[0xd54];
    long MinionChaseEscapeTime;                         // +0xd54
};
extern CScriptDef* pGlobalScriptDef;                        // 0x0143E90C (Ego_r.pdb `pGlobalScriptDef`, a def pointer: its first word)
// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#include "engine/CScriptBase.h"
struct CQ_MinionClifftopChaseScript : CScriptBase {
    CGameScriptInterface* Game;                         // +0x40
    void* PMasterData;                                  // +0x44
    bool MapKeeperSaidDialog;                           // +0x48
    bool MissionFailed;                                 // +0x49
    unsigned char _pad4a[0x54 - 0x4a];
    bool RockTrollSeen;                                 // +0x54
    unsigned char _pad55[0x5c - 0x55];
    CTimer EscapeTimer;                                 // +0x5c
    long TimerBarID;                                    // +0x60
    long ReasonForFailure;                              // +0x64
    void ProcessMission();
    void GuardComment(CCharString comment);             // 0x00DA8970
};

void CQ_MinionClifftopChaseScript::ProcessMission()
{
    while (!Game->IsRegionLoaded("DesertedFarm")) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->SetQuestCardObjective(Game->GetActiveQuestName(), "TEXT_QUEST_MINION_CLIFFTOP_CHASE_OBJECTIVE_03", "GibbetWoods", "BowerstoneSlums");
    while (!Game->IsRegionLoaded("GibbetWoods")) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->SetQuestCardObjective(Game->GetActiveQuestName(), "TEXT_QUEST_MINION_CLIFFTOP_CHASE_OBJECTIVE_04", "PrisonPath", "BowerstoneSlums");
    while (!RockTrollSeen) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    GuardComment("ROCK_TROLL");
    while (!Game->IsRegionLoaded("PrisonPath")) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->StopOverrideMusic(false);
    long escapeTime = pGlobalScriptDef->MinionChaseEscapeTime;
    EscapeTimer.Set(escapeTime);
    Game->DisplayQuestInfo(true);
    TimerBarID = Game->AddQuestInfoTimer(EscapeTimer, "HUD_CLOCK_ICON", 1.0f);
    Game->SetQuestCardObjective(Game->GetActiveQuestName(), "TEXT_QUEST_MINION_CLIFFTOP_CHASE_OBJECTIVE_05", "PrisonPath", "BowerstoneSlums");
    while (!MapKeeperSaidDialog && !MissionFailed && EscapeTimer.Get() != 0) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    if (EscapeTimer.Get() == 0) {
        if (IsActiveThreadTerminating())
            return;
        ReasonForFailure = 1;
        MissionFailed = true;
    }
}