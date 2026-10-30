// NScript::CQ_BanditCampScript::WatchForTermination() -- wait until the bandit camp mission has succeeded or failed; then complete or fail the active quest, and deactivate it
// Leaf: ?WatchForTermination@CQ_BanditCampScript@NScript@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D01290, length 265 bytes.
// Name: the row is referenced once, from 0x00D00CE5 inside the landed CQ_BanditCampScript::Main 0x00D00AF0, which spawns
//   it as the "WatchForTermination" thread (CSpawnedFunc member pointer); Ego_r.pdb has
//   ?WatchForTermination@CQ_BanditCampScript@NScript@@QAEXXZ. (Catalog label CScriptBase_WaitForEntityAndExecuteOnePair
//   is invented.)
// Callees: CScriptBase::IsActiveThreadTerminating 0x00CB7940, CWideString(const unsigned short*) 0x0099B6B0 /
//   ~CWideString 0x0099B510, ~CCharString 0x0099EAE0, and through the script interface (Game, +0x40): NewScriptFrame
//   (slot 7), DeactivateQuestLater (slot 281), SetQuestAsCompleted (slot 288), SetQuestAsFailed (slot 289),
//   GetActiveQuestName (slot 655, a CCharString by value).
// Layout (retail = Ego_r.pdb - 0x14, as the landed CQ_BanditCampScript::Main / ctor 0x00D006A0): CLevelScriptBase Game
//   +0x40 / MasterScriptData +0x44; MissionSucceeded +0x4b, MissionFailed +0x4c.
// Shape: the landed CQ_WizardBattleScript::WatchForTermination 0x00E1AD60 model -- the `&&` wait rotates into an entry
//   test of both flags and a bottom test of the first; each arm re-tests the thread; the empty wide message (L"",
//   0x0122D70C) is an implicit conversion temporary built before the other arguments.
// Model: local CScriptBase / CLevelScriptBase / CGameScriptInterfaceBase models (the engine headers are flattened, no
//   vtables; the landed quest ctors use the same model); quest members are the PDB names at the retail offsets.
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
class CQ_BanditCampScript : public CLevelScriptBase<NScript::CQ_SunnyvaleMasterData> {
public:
    bool AttackedOuterGateGuards;                               // +0x48
    bool Gate1Open;                                             // +0x49
    bool BoastBegun;                                            // +0x4a
    bool MissionSucceeded;                                      // +0x4b
    bool MissionFailed;                                         // +0x4c
    bool ActivatedBanditRaid;                                   // +0x4d
    void WatchForTermination();
};
} // namespace NScript

void NScript::CQ_BanditCampScript::WatchForTermination()
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