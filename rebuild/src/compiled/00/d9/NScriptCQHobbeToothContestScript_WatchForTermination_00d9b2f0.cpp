// NScript::CQ_HobbeToothContestScript::WatchForTermination() -- wait until the hobbe tooth contest has succeeded or failed; then complete the active quest or fail it with TEXT_QST_035_QUEST_FAILED, deactivate it and hide the quest info
// Leaf: ?WatchForTermination@CQ_HobbeToothContestScript@NScript@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D9B2F0, length 278 bytes.
// Name: the row is referenced once, at 0x00D9A5D5 inside Main 0x00D9A190 (slot 2 of the quest vtable 0x012D65CC that the
//   landed ctor 0x00D99E40 stores), which spawns it as the "WatchForTermination" thread (string 0x012C84B0); Ego_r.pdb has
//   ?WatchForTermination@CQ_HobbeToothContestScript@NScript@@QAEXXZ. (Catalog label Script_Quest_Failed_Notify is invented.)
// Callees: CScriptBase::IsActiveThreadTerminating 0x00CB7940, CWideString(const unsigned short*) 0x0099B6B0 /
//   ~CWideString 0x0099B510, ~CCharString 0x0099EAE0, and through the script interface (Game, +0x40): NewScriptFrame
//   (slot 7), DeactivateQuestLater (slot 281), SetQuestAsCompleted (slot 288), SetQuestAsFailed (slot 289),
//   DisplayQuestInfo (slot 321), GetActiveQuestName (slot 655, a CCharString by value).
// Layout (retail = Ego_r.pdb - 0x14): MissionSucceeded +0x5b, MissionFailed +0x5c (Ego_r +0x6f / +0x70).
// Shape: the CQ_BanditCampScript::WatchForTermination 0x00D01290 form (same chunk) with a real failure text
//   (L"TEXT_QST_035_QUEST_FAILED", 0x012D67F0) and a trailing DisplayQuestInfo(false).
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
class CQ_HobbeToothContestScript : public CLevelScriptBase<NScript::CQ_SunnyvaleMasterData> {
public:
    long NumberOfHobbesInTotal;                                 // +0x48
    long NumberOfHobbeTeethCollectedByHero;                     // +0x4c
    long NumberOfHobbeTeethCollectedByWhisper;                  // +0x50
    long NumberOfHobbeTeethNeeded;                              // +0x54
    bool CS_IntroDone;                                          // +0x58
    bool CS_OutroBegin;                                         // +0x59
    bool DrinkingHobbesNotHit;                                  // +0x5a
    bool MissionSucceeded;                                      // +0x5b
    bool MissionFailed;                                         // +0x5c
    bool WhisperOrFarmerHit;                                    // +0x5d
    void WatchForTermination();
};
} // namespace NScript

void NScript::CQ_HobbeToothContestScript::WatchForTermination()
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
        Game->SetQuestAsCompleted(Game->GetActiveQuestName(), true, true, false);
    } else {
        if (IsActiveThreadTerminating())
            return;
        Game->SetQuestAsFailed(Game->GetActiveQuestName(), true, L"TEXT_QST_035_QUEST_FAILED", true);
    }
    Game->DeactivateQuestLater(Game->GetActiveQuestName(), 0);
    Game->DisplayQuestInfo(false);
}