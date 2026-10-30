// CQ_SummoningTheShipScript::DoMission() -- set the first objective, wait for Hook Coast, prepare the lighthouse (summoner explosions spare the hero, no weapon-out crime, door and ships removed, village in limbo), play the pre-intro, start screen and fade in, wait for the lighthouse, play the intro, wait for the second wave or a failure, play the failed outro or wave two, wait for the summoners; let Briar Rose comment, then complete (outro + movie) or fail the quest, restore the village, crime and music, remove Briar Rose, teleport the hero to Lost Bay and deactivate the quest
// Leaf: DoMission
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DF36E0, 1899 bytes (int3 padded to 1904).
// Callees: CCharString 0x0099EBF0 / ~ 0x0099EAE0, CWideString 0x0099B6B0 / ~ 0x0099B510, IsActiveThreadTerminating
//   0x00CB7940, ~CScriptThing 0x004AA840, CScriptThing::IsAlive 0x004AB130, PlayCutscene(CCharString) 0x00DF3E50; Game slots
//   7, 11, 70, 72, 108, 133, 281, 288, 289, 296, 309, 364, 365, 366, 369, 374, 376, 440, 472, 574, 655, 696, 697.
// Layout (retail = Ego_r.pdb - 0x14): SummonersAlive +0x48, CurrentAttackWave +0x4c, LighthouseStarted +0x50,
//   SummonerAttacksStarted +0x51, MissionFailed +0x52, CommentaryTimer +0x54, ConversationIndex +0x58.
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
    const CCharString& operator=(const char* text);           // 0x0099EFE0
};
class CWideString { public:
    void* PStringData;
    CWideString(const unsigned short* text);            // 0x0099B6B0
    ~CWideString();                                     // 0x0099B510
};
class CScriptThing { public:                                   // 12 bytes {vptr, PImp.Data, PImp.Info}
    void* VTable;
    void* PImpData;
    void* PImpInfo;
    ~CScriptThing();                                    // 0x004AA840
    bool IsAlive() const;                               // 0x004AB130
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#include "engine/CScriptBase.h"
namespace NScript {
class CQ_SummoningTheShipScript : public CScriptBase {
public:
    CGameScriptInterface* Game;                         // +0x40
    void*                 PMasterData;                  // +0x44
    long                  SummonersAlive;               // +0x48
    long                  CurrentAttackWave;            // +0x4c
    bool                  LighthouseStarted;            // +0x50
    bool                  SummonerAttacksStarted;       // +0x51
    bool                  MissionFailed;                // +0x52
    long                  CommentaryTimer;              // +0x54 (CTimer)
    long                  ConversationIndex;            // +0x58
    void DoMission();
    void PlayCutscene(CCharString name);                // 0x00DF3E50
};
} // namespace NScript
using NScript::CQ_SummoningTheShipScript;

void NScript::CQ_SummoningTheShipScript::DoMission()
{
    Game->SetQuestCardObjective(Game->GetActiveQuestName(), "TEXT_QUEST_SUMMONING_SHIP_OBJECTIVE_01", "HookCoast", "HookCoast");
    while (!Game->IsLevelLoaded("HookCoast")) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->SetSummonerDeathExplosionAffectsHero(false);
    Game->SetWeaponOutCrimeEnabled(false);
    Game->RemoveThing(Game->GetThingWithScriptName("LightHouseDoor"), false, false);
    Game->RemoveThing(Game->GetThingWithScriptName("HookCoastShip1"), false, false);
    Game->RemoveThing(Game->GetThingWithScriptName("HookCoastShip2"), false, false);
    Game->SetVillageLimbo(Game->GetThingWithScriptName("HookCoastVillage"), true);
    PlayCutscene("CS_SUMSHIP_PREINTRO");
    Game->KickOffQuestStartScreen(Game->GetActiveQuestName(), true, false);
    Game->FadeScreenIn();
    while (!LighthouseStarted) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->OverrideMusic((EMusicSetType)0x17, false, false);
    Game->SetQuestCardObjective(Game->GetActiveQuestName(), "TEXT_QUEST_SUMMONING_SHIP_OBJECTIVE_02", "HookCoast", "HookCoast");
    PlayCutscene("CS_SUMSHIP_INTRO");
    SummonerAttacksStarted = true;
    Game->OverrideMusic((EMusicSetType)0x17, false, false);
    while (CurrentAttackWave <= 1 && !MissionFailed) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    if (MissionFailed) {
        if (IsActiveThreadTerminating())
            return;
        PlayCutscene("CS_SUMSHIP_OUTRO_FAILED");
        Game->SetQuestAsFailed(Game->GetActiveQuestName(), true, L"TEXT_QUEST_SUMMONING_THE_SHIP_FAILED", true);
    } else {
        if (IsActiveThreadTerminating())
            return;
        PlayCutscene("CS_SUMSHIP_WAVE2");
    }
    Game->SetSummonerDeathExplosionAffectsHero(true);
    while (SummonersAlive != 0 && !MissionFailed) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    CScriptThing rose = Game->GetThingWithScriptName("STS_BriarRose");
    if (rose.IsAlive()) {
        if (IsActiveThreadTerminating())
            return;
        long conversation = Game->AddNewConversation(rose, false, false);
        ConversationIndex = conversation;
        Game->AddPersonToConversation(conversation, Game->GetHero());
        Game->AddLineToConversation(conversation, "TEXT_QST_B02_BRIARROSE_FIREHEART_POWERED", false, rose, Game->GetHero());
    }
    Game->StopOverrideMusic(false);
    if (SummonersAlive == 0) {
        if (IsActiveThreadTerminating())
            return;
        Game->Pause(3.0f);
        PlayCutscene("CS_SUMSHIP_OUTRO");
        Game->PlayAVIMovie("Data\\Video\\Summoning_The_Ship.xmv");
        Game->SetQuestAsCompleted(Game->GetActiveQuestName(), true, false, false);
    } else {
        if (IsActiveThreadTerminating())
            return;
        PlayCutscene("CS_SUMSHIP_OUTRO_FAILED");
        Game->SetQuestAsFailed(Game->GetActiveQuestName(), true, L"TEXT_QUEST_SUMMONING_THE_SHIP_FAILED", true);
    }
    Game->SetVillageLimbo(Game->GetThingWithScriptName("HookCoastVillage"), false);
    Game->SetWeaponOutCrimeEnabled(true);
    Game->RemoveThing(Game->GetThingWithScriptName("STS_BriarRose"), false, true);
    Game->StopOverrideMusic(false);
    Game->EntityTeleportToThing(Game->GetHero(), Game->GetThingWithScriptName("LostBayHSP"), false);
    Game->DeactivateQuestLater(Game->GetActiveQuestName(), 0);
}