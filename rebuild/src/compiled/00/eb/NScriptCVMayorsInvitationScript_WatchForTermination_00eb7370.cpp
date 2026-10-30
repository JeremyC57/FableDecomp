// NScript::CV_MayorsInvitationScript::WatchForTermination() -- wait until the hero aborts, fails or succeeds; on success complete the quest, make Lady Grey marryable and marry her to the hero (clearing her information and minimap marker), record the marriage and activate LadyGreyWifeManager; otherwise deactivate V_ExposeMayor and clear Lady Grey's information and marker; finally clear the WiseVillager's information and deactivate the quest
// Leaf: ?WatchForTermination@CV_MayorsInvitationScript@NScript@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EB7370, 625 bytes, thiscall, plain `ret`.
// Identity (proven): spawned as "WatchForTermination" (0x012C84B0) by the landed CV_MayorsInvitationScript::Main 0x00EB6D70 (which
//   already declares this row as WatchForTermination 0x00EB7370): the CSpawnedFunc Func store at 0x00EB6FA6 writes this row right
//   after that literal (CSpawnedFunc<CV_MayorsInvitationScript> vtable 0x012F0548); the row has no other reference. Ego_r.pdb
//   ?WatchForTermination@CV_MayorsInvitationScript@NScript@@QAEXXZ. Catalog label MayorsInvitation_OnEnter (invented).
// Callees: CScriptBase::IsActiveThreadTerminating 0x00CB7940, CCharString(const char*, long) 0x0099EBF0 / ~ 0x0099EAE0,
//   ~CScriptThing 0x004AA840; Game (+0x40) slots 7 NewScriptFrame, 72 GetThingWithScriptName, 206 EntitySetAsMarryable, 208
//   EntityForceMarriageToHero, 276 ActivateQuest, 281 DeactivateQuestLater, 288 SetQuestAsCompleted, 352 MiniMapRemoveMarker,
//   361 ClearThingHasInformation, 655 GetActiveQuestName.
// Literals: "LadyGrey" 0x012C6C24 (x2), "LadyGreyWifeManager" 0x012F05C0, "V_ExposeMayor" 0x012F05B0, "WiseVillager" 0x012E9FE4.
// Layout: retail NScript headers (CLevelScriptBase 0x48); CV_MayorsInvitationScript members at Ego_r.pdb - 0x14: MissionSucceeded
//   +0x48, MissionFailed +0x49, MissionOver +0x4a, HeroAbortedQuest +0x4b. CQ_SunnyvaleMasterData::HeroMarriedLadyGrey +0xd7
//   (Ego_r offset).
// Shape: the landed CQ_WizardBattleScript::WatchForTermination 0x00E1AD60 form (`&&` wait, terminate test in each arm); each arm
//   has its own LadyGrey CScriptThing local (VC7.1 merges their out-of-line destructor calls into one tail).
class CCharString {
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
#include "engine/CCountedPointer.h"
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
#include "engine/CLevelScriptBase.h"
#include "engine/CGameScriptInterfaceBase.h"

namespace NScript {
class CQ_SunnyvaleMasterData {
public:
    unsigned char _before[0xd7];
    bool HeroMarriedLadyGrey;                           // +0xd7
};
class CV_MayorsInvitationScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    bool MissionSucceeded;                              // +0x48
    bool MissionFailed;                                 // +0x49
    bool MissionOver;                                   // +0x4a
    bool HeroAbortedQuest;                              // +0x4b
    void WatchForTermination();
};

void CV_MayorsInvitationScript::WatchForTermination()
{
    while (!HeroAbortedQuest && !MissionFailed && !MissionSucceeded) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    if (!HeroAbortedQuest && !MissionFailed) {
        if (IsActiveThreadTerminating())
            return;
        Game->SetQuestAsCompleted(Game->GetActiveQuestName(), false, false, false);
        CScriptThing ladyGrey = Game->GetThingWithScriptName("LadyGrey");
        Game->ClearThingHasInformation(ladyGrey);
        Game->MiniMapRemoveMarker(ladyGrey);
        Game->EntitySetAsMarryable(ladyGrey, true);
        Game->EntityForceMarriageToHero(ladyGrey, false);
        MasterScriptData->HeroMarriedLadyGrey = true;
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        Game->ActivateQuest("LadyGreyWifeManager");
    } else {
        if (IsActiveThreadTerminating())
            return;
        Game->DeactivateQuestLater("V_ExposeMayor", 0);
        CScriptThing ladyGrey = Game->GetThingWithScriptName("LadyGrey");
        Game->ClearThingHasInformation(ladyGrey);
        Game->MiniMapRemoveMarker(ladyGrey);
    }
    CScriptThing wiseVillager = Game->GetThingWithScriptName("WiseVillager");
    Game->ClearThingHasInformation(wiseVillager);
    Game->DeactivateQuestLater(Game->GetActiveQuestName(), 0);
}
}   // namespace NScript