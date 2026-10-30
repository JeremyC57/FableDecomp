// NScript::CQ_BanditCampBossBattleScript::WatchForTermination() -- wait until the bandit camp boss battle has succeeded or failed; on success complete the active quest, set the prize tavern table, pause a second and fade the screen in; on failure fail the quest; then deactivate it
// Leaf: ?WatchForTermination@CQ_BanditCampBossBattleScript@NScript@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D04260, length 305 bytes.
// Name: the row is referenced once, at 0x00D04060 inside Main 0x00D038E0 (slot 2 of the quest vtable 0x012C88E0 that the
//   landed RegisterMain 0x00D03820 / Allocs use), which spawns it as the "WatchForTermination" thread (string 0x012C84B0);
//   Ego_r.pdb has ?WatchForTermination@CQ_BanditCampBossBattleScript@NScript@@QAEXXZ. (Catalog label
//   CScriptBase_WaitForEntityAndExecuteTwoPair is invented.)
// Callees: CScriptBase::IsActiveThreadTerminating 0x00CB7940, CWideString(const unsigned short*) 0x0099B6B0 /
//   ~CWideString 0x0099B510, ~CCharString 0x0099EAE0, and through the script interface (Game, +0x40): NewScriptFrame
//   (slot 7), DeactivateQuestLater (slot 281), SetQuestAsCompleted (slot 288), SetQuestAsFailed (slot 289),
//   FadeScreenIn (slot 374), Pause (slot 376), GetActiveQuestName (slot 655, by value), SetPrizeTavernTable (slot 866:
//   the retail vtable 0x01260F0C entry is 0x0088FB20, landed as CGameScriptInterface::SetPrizeTavernTable).
// Layout (retail = Ego_r.pdb - 0x14): MissionSucceeded +0x5d, MissionFailed +0x5e (Ego_r +0x71 / +0x72).
// Shape: the CQ_BanditCampScript::WatchForTermination 0x00D01290 form (same chunk) with three more interface calls in
//   the succeeded arm; Pause(1.0f) pushes the float immediate 0x3F800000.
// Model: local CScriptBase / CLevelScriptBase / CGameScriptInterfaceBase models (the engine headers are flattened, no
//   vtables); quest members are the PDB names at the retail offsets.
class CCharString { public:                                     // 4 bytes: the shared string block pointer
    void* PStringData;
    ~CCharString();                                             // 0x0099EAE0
};
class CWideString { public:                                     // 4 bytes
    void* PStringData;
    CWideString(const unsigned short* text);                    // 0x0099B6B0
    ~CWideString();                                             // 0x0099B510
};
// CGameScriptInterfaceBase is the retail abstract script interface from the shared headers (FABLE_RETAIL_NSCRIPT
//   opt-in: CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x0126009C, 917 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterfaceBase.h"
class CScriptDataBase;
class CPersistContext;
// CScriptBase / CLevelScriptBase<T>: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40; CLevelScriptBase<T> Game +0x40, MasterScriptData +0x44).
#include "engine/CLevelScriptBase.h"
namespace NScript { class CQ_SunnyvaleMasterData; }

namespace NScript {
class CQ_BanditCampBossBattleScript : public CLevelScriptBase<NScript::CQ_SunnyvaleMasterData> {
public:
    bool AttackedOuterGateGuards;                               // +0x48
    bool AttackedGate2OuterGuard;                               // +0x49
    bool AttackedGate3OuterGuard;                               // +0x4a
    bool Gate1Open;                                             // +0x4b
    bool Gate2Open;                                             // +0x4c
    bool Gate3Open;                                             // +0x4d
    bool HostagesRescued;                                       // +0x4e
    bool HostageKilled;                                         // +0x4f
    bool ItsAllOver;                                            // +0x50
    bool TheresaFrescoShown;                                    // +0x51
    bool BanditKingFightStarted;                                // +0x52
    bool BanditKingFightEnded;                                  // +0x53
    bool BanditKingCrowdSpawn;                                  // +0x54
    int  KingHealth;                                            // +0x58
    bool SpokenToSecondGuard;                                   // +0x5c
    bool MissionSucceeded;                                      // +0x5d
    bool MissionFailed;                                         // +0x5e
    void WatchForTermination();
};
} // namespace NScript

void NScript::CQ_BanditCampBossBattleScript::WatchForTermination()
{
    while (!MissionFailed && !MissionSucceeded) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    if (MissionSucceeded) {
        if (IsActiveThreadTerminating())
            return;
        Game->SetQuestAsCompleted(Game->GetActiveQuestName(), false, false, false);
        Game->SetPrizeTavernTable(true);
        Game->Pause(1.0f);
        Game->FadeScreenIn();
    } else {
        if (IsActiveThreadTerminating())
            return;
        Game->SetQuestAsFailed(Game->GetActiveQuestName(), false, L"", true);
    }
    Game->DeactivateQuestLater(Game->GetActiveQuestName(), 0);
}