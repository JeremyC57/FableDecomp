// NScript::CQ_OpeningGraveyardSecretPassageScript::WatchForTermination() -- wait until the graveyard secret-passage mission has succeeded or failed; then complete or fail the active quest, and deactivate it
// Leaf: ?WatchForTermination@CQ_OpeningGraveyardSecretPassageScript@NScript@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00DC3BD0, length 265 bytes.
// Name: the row is referenced once, at 0x00DC2DCC inside Main 0x00DC27C0 (slot 2 of the quest vtable 0x012DA11C that the
//   landed ctor 0x00DC2570 stores), which spawns it as the "WatchForTermination" thread (string 0x012C84B0, CSpawnedFunc
//   member pointer at +0x34); Ego_r.pdb has ?WatchForTermination@CQ_OpeningGraveyardSecretPassageScript@NScript@@QAEXXZ.
//   (Catalog label Script_Assignment_Failed is invented.)
// Callees: CScriptBase::IsActiveThreadTerminating 0x00CB7940, CWideString(const unsigned short*) 0x0099B6B0 /
//   ~CWideString 0x0099B510, ~CCharString 0x0099EAE0, and through the script interface (Game, +0x40): NewScriptFrame
//   (slot 7), DeactivateQuestLater (slot 281), SetQuestAsCompleted (slot 288), SetQuestAsFailed (slot 289),
//   GetActiveQuestName (slot 655, a CCharString by value).
// Layout (retail = Ego_r.pdb - 0x14, as the landed ctor 0x00DC2570): MissionSucceeded +0x55, MissionFailed +0x56.
// Shape: twin of CQ_BanditCampScript::WatchForTermination 0x00D01290 (same chunk), after the landed
//   CQ_WizardBattleScript::WatchForTermination 0x00E1AD60 model: the `&&` wait rotates into an entry test of both flags
//   and a bottom test of the first; each arm re-tests the thread; the empty wide message (L"", 0x0122D70C) is an implicit
//   conversion temporary built before the other arguments.
// Model: local CScriptBase / CLevelScriptBase / CGameScriptInterfaceBase models (the engine headers are flattened, no
//   vtables); quest members are the PDB names at the retail offsets (the landed ctor's layout).
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
class CQ_OpeningGraveyardSecretPassageScript : public CLevelScriptBase<NScript::CQ_SunnyvaleMasterData> {
public:
    bool finished_quest;                                        // +0x48
    bool DoorChange;                                            // +0x49
    bool ZombieCamPlayed;                                       // +0x4a
    bool skeleton_visited;                                      // +0x4b
    bool doors_open;                                            // +0x4c
    bool GravekeeperIntroduced;                                 // +0x4d
    bool GravekeeperCutsceneStart;                              // +0x4e
    bool UndeadCutscenePlayed;                                  // +0x4f
    bool UndeadCutsceneStarted;                                 // +0x50
    bool SkeletonStuff[4];                                      // +0x51
    bool MissionSucceeded;                                      // +0x55
    bool MissionFailed;                                         // +0x56
    bool DoorClosed;                                            // +0x57
    void WatchForTermination();
};
} // namespace NScript

void NScript::CQ_OpeningGraveyardSecretPassageScript::WatchForTermination()
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
        Game->SetQuestAsCompleted(Game->GetActiveQuestName(), true, false, false);
    } else {
        if (IsActiveThreadTerminating())
            return;
        Game->SetQuestAsFailed(Game->GetActiveQuestName(), true, L"", true);
    }
    Game->DeactivateQuestLater(Game->GetActiveQuestName(), 0);
}