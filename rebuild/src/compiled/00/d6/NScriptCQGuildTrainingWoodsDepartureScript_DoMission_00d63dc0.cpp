// CQ_GuildTrainingWoodsDepartureScript::DoMission() -- give the hero the first objective, wait until GuildWoods is loaded, spawn the WatchForLeaving and TeleportOutHero threads, then (unless the mission failed) wait for the mission to end and complete the active quest
// Leaf: DoMission
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D63DC0, 569 bytes.
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0 ("first objective" 0x012D19FC,
//   "GuildWoods" 0x012CD7DC, "WatchForLeaving" 0x012D19EC, "TeleportOutHero" 0x012D19DC, "" 0x0122D70E),
//   CScriptBase::IsActiveThreadTerminating 0x00CB7940, operator new 0x00BFEA1A, CSpawnedFuncBase(const CCharString&,
//   unsigned long) 0x00CDD450 (CSpawnedFunc<CQ_GuildTrainingWoodsDepartureScript> vtable 0x012D1610, functions
//   WatchForLeaving 0x00D64000 and TeleportOutHero 0x00D64080 -- manifest labels for both are propagated),
//   CScriptBase::AddSpawnedFunction 0x00CB7E50; CGameScriptInterface (+0x40) slots 7 NewScriptFrame, 11 IsLevelLoaded,
//   268 GiveHeroNewQuestObjective, 288 SetQuestAsCompleted, 655 GetActiveQuestName.
// Shape: the landed CQ_GuildTrainingWoodsMeleeScript::DoMission 0x00D66CA0 plus the mission-over wait.
// Layout (retail = Ego_r.pdb - 0x14): MissionSucceeded +0x48, MissionFailed +0x49, MissionOver +0x4a.
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
class CCharString { public:                                     // 4 bytes: the shared string block pointer
    void* PStringData;
    CCharString(const char* text, long length = -1);     // 0x0099EBF0
    ~CCharString();                                      // 0x0099EAE0
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
// Layout: CSpawnedFuncBase is the retail polymorphic suspendable-process chain from the shared headers
//   (FABLE_RETAIL_SUSPENDABLE_PROCESS opt-in: CSpawnedFuncBase : CSuspendableProcessMicrothread : CASuspendableProcess :
//   CBaseClassNonCopyable : CBaseClass, 0x34 bytes, vtable 0x012C2F9C; ctor 0x00CDD450).
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#include "engine/CSpawnedFuncBase.h"
template <class T> struct CSpawnedFunc : CSpawnedFuncBase {
    void (T::*Func)();                                   // +0x34
    T* PClass;                                           // +0x38
    __forceinline CSpawnedFunc(const CCharString& name, unsigned long id, T* pclass, void (T::*func)())
        : CSpawnedFuncBase(name, id), Func(func), PClass(pclass) {}
    virtual void SuspendableProcess() { (PClass->*Func)(); }
};
class CScriptBase { public:                                     // retail 0x40 bytes
    unsigned char _body[0x40];
    bool IsActiveThreadTerminating();                    // 0x00CB7940
    void AddSpawnedFunction(CSpawnedFuncBase* func, const CCharString& regionName);   // 0x00CB7E50
    __forceinline void SpawnFunction(CSpawnedFuncBase* func, const char* regionName = "") { AddSpawnedFunction(func, CCharString(regionName)); }
};
namespace NScript {
class CQ_GuildTrainingWoodsDepartureScript : public CScriptBase {
public:
    CGameScriptInterface* Game;                          // +0x40
    void*                 Data;                          // +0x44
    bool                  MissionSucceeded;              // +0x48
    bool                  MissionFailed;                 // +0x49
    bool                  MissionOver;                   // +0x4a
    long                  DepartureMissionPoint;         // +0x4c
    void WatchForLeaving();                              // 0x00D64000
    void TeleportOutHero();                              // 0x00D64080
    void DoMission();
};
} // namespace NScript
using NScript::CQ_GuildTrainingWoodsDepartureScript;

void NScript::CQ_GuildTrainingWoodsDepartureScript::DoMission()
{
    Game->GiveHeroNewQuestObjective("first objective", 1);
    while (!Game->IsLevelLoaded("GuildWoods")) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    SpawnFunction(new CSpawnedFunc<CQ_GuildTrainingWoodsDepartureScript>("WatchForLeaving", 0, this, &NScript::CQ_GuildTrainingWoodsDepartureScript::WatchForLeaving));
    SpawnFunction(new CSpawnedFunc<CQ_GuildTrainingWoodsDepartureScript>("TeleportOutHero", 0, this, &NScript::CQ_GuildTrainingWoodsDepartureScript::TeleportOutHero));
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    if (MissionFailed)
        return;
    while (!MissionOver) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->SetQuestAsCompleted(Game->GetActiveQuestName(), true, true, false);
    MissionSucceeded = true;
}