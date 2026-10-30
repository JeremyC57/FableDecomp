// CQ_GuildTrainingWoodsMeleeScript::DoMission() -- give the hero the first objective, wait until GuildWoods is loaded, spawn the WatchForLeaving and TeleportOutHero threads, take a frame and run EndMission unless the mission failed
// Leaf: DoMission
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D66CA0, 487 bytes (496 with padding), plain `ret`.
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0 ("first objective" 0x012D19FC,
//   "GuildWoods" 0x012CD7DC, "WatchForLeaving" 0x012D19EC, "TeleportOutHero" 0x012D19DC, "" 0x0122D70E),
//   CScriptBase::IsActiveThreadTerminating 0x00CB7940, operator new 0x00BFEA1A, CSpawnedFuncBase(const CCharString&,
//   unsigned long) 0x00CDD450 (CSpawnedFunc<CQ_GuildTrainingWoodsMeleeScript> vtable 0x012D1D30, functions
//   WatchForLeaving 0x00D66E90 and TeleportOutHero 0x00D66F50 -- manifest labels for both are propagated),
//   CScriptBase::AddSpawnedFunction 0x00CB7E50, EndMission 0x00D66EE0 (manifest "CQ_CinemaTestScript::EndMission" is
//   propagated); CGameScriptInterface (+0x40) slots 7 NewScriptFrame, 11 IsLevelLoaded (+0x2c), 268
//   GiveHeroNewQuestObjective (+0x430).
// Shape: as the landed CV_ChapelOfEvilScript::DoMission 0x00E6A7F0 (inline `const char*` SpawnFunction, `__forceinline`
//   CSpawnedFunc ctor); each thread name temporary is guarded by a construction flag (built only when new succeeds).
// Layout (retail = Ego_r.pdb - 0x14): MissionFailed +0x49.
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
#include "engine/CSpawnedFunc.h"                         // vtable per instantiation; ctor inline, slot 4 SuspendableProcess
// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#include "engine/CScriptBase.h"
namespace NScript {
class CQ_GuildTrainingWoodsMeleeScript : public CScriptBase {
public:
    CGameScriptInterface* Game;                          // +0x40
    void*                 Data;                          // +0x44
    bool                  MissionSucceeded;              // +0x48
    bool                  MissionFailed;                 // +0x49
    void WatchForLeaving();                              // 0x00D66E90
    void TeleportOutHero();                              // 0x00D66F50
    void EndMission();                                   // 0x00D66EE0
    void DoMission();
};
} // namespace NScript
using NScript::CQ_GuildTrainingWoodsMeleeScript;

void NScript::CQ_GuildTrainingWoodsMeleeScript::DoMission()
{
    Game->GiveHeroNewQuestObjective("first objective", 1);
    while (!Game->IsLevelLoaded("GuildWoods")) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    AddSpawnedFunction(new CSpawnedFunc<CQ_GuildTrainingWoodsMeleeScript>("WatchForLeaving", 0, this, &NScript::CQ_GuildTrainingWoodsMeleeScript::WatchForLeaving), CCharString(""));
    AddSpawnedFunction(new CSpawnedFunc<CQ_GuildTrainingWoodsMeleeScript>("TeleportOutHero", 0, this, &NScript::CQ_GuildTrainingWoodsMeleeScript::TeleportOutHero), CCharString(""));
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    if (!MissionFailed)
        EndMission();
}