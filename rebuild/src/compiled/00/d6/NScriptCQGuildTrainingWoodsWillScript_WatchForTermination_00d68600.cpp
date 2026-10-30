// NScript::CQ_GuildTrainingWoodsWillScript::WatchForTermination() -- wait until the woods Will-training mission has failed or succeeded; then fail or complete the active quest, and deactivate Q_GuildTrainingWoodsWill
// Leaf: ?WatchForTermination@CQ_GuildTrainingWoodsWillScript@NScript@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D68600, length 267 bytes.
// Name: the row is referenced once, at 0x00D67A86 inside Main 0x00D67890 (slot 2 of the quest vtable 0x012D1E08 that the
//   landed ctor 0x00D677A0 stores), which spawns it as the "WatchForTermination" thread (string 0x012C84B0); Ego_r.pdb has
//   ?WatchForTermination@CQ_GuildTrainingWoodsWillScript@NScript@@QAEXXZ. (Catalog label
//   Quest_GuildTrainingWoods_Will_Init is invented.)
// Callees: CScriptBase::IsActiveThreadTerminating 0x00CB7940, CWideString(const unsigned short*) 0x0099B6B0 /
//   ~CWideString 0x0099B510, CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, and through the script
//   interface (Game, +0x40): NewScriptFrame (slot 7), DeactivateQuestLater (slot 281), SetQuestAsCompleted (slot 288),
//   SetQuestAsFailed (slot 289), GetActiveQuestName (slot 655, a CCharString by value).
// Layout (retail = Ego_r.pdb - 0x14): MissionSucceeded +0x48, MissionFailed +0x49, MissionOver +0x4a.
// Shape: the CQ_CinemaTestScript::WatchForTermination 0x00F01A40 form (same chunk); the deactivation names the quest
//   ("Q_GuildTrainingWoodsWill", 0x012C5AC4) through an implicit CCharString temporary.
// Model: local CScriptBase / CLevelScriptBase / CGameScriptInterfaceBase models (the engine headers are flattened, no
//   vtables); quest members are the PDB names at the retail offsets.
class CCharString { public:                                     // 4 bytes: the shared string block pointer
    void* PStringData;
    CCharString(const char* text, long length = -1);            // 0x0099EBF0
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
class CQ_GuildTrainingWoodsWillScript : public CLevelScriptBase<NScript::CQ_SunnyvaleMasterData> {
public:
    bool MissionSucceeded;                                      // +0x48
    bool MissionFailed;                                         // +0x49
    bool MissionOver;                                           // +0x4a
    bool WhisperAnimate;                                        // +0x4b
    bool BanditsAlive;                                          // +0x4c
    void WatchForTermination();
};
} // namespace NScript

void NScript::CQ_GuildTrainingWoodsWillScript::WatchForTermination()
{
    while (!MissionFailed && !MissionSucceeded) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    if (MissionFailed) {
        if (IsActiveThreadTerminating())
            return;
        Game->SetQuestAsFailed(Game->GetActiveQuestName(), true, L"", true);
    } else {
        if (IsActiveThreadTerminating())
            return;
        Game->SetQuestAsCompleted(Game->GetActiveQuestName(), false, true, false);
    }
    Game->DeactivateQuestLater("Q_GuildTrainingWoodsWill", 0);
}