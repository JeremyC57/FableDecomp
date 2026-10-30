// CQ_GuildTrainingWoodsMeleeScript::WatchForTermination() -- wait for the woods melee mission to end, fail or complete the active quest, point the quest card back at the Guild, have the hero say the "come back" line, flag the scorpion cutscene when the mission succeeded, then wait for the Guild complex and deactivate the quest
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D66880, length 789 bytes. Identity: the thread spawned as
//   "WatchForTermination" by the landed CQ_GuildTrainingWoodsMeleeScript::Main 0x00D66620 (manifest
//   "CQ_HobbeCaveScript::WatchForTermination" is propagated).
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, CWideString(const wchar_t*) 0x0099B6B0 /
//   ~CWideString 0x0099B510 (L"" 0x0122D70C), CScriptBase::IsActiveThreadTerminating 0x00CB7940; through the interface
//   (+0x40): NewScriptFrame (7), IsLevelLoaded (11), GetHero (70), DeactivateQuestLater (281), SetQuestAsCompleted
//   (288), SetQuestAsFailed (289), SetQuestCardObjective (296), AddNewConversation (364), AddLineToConversation (366),
//   GetActiveQuestName (655).
// Layout (retail = Ego_r.pdb - 0x14): MissionSucceeded +0x48, MissionFailed +0x49; PMasterData +0x44
//   (CQ_SunnyvaleMasterData::ScorpionsDestroyedCutscenePlayed +0x9d).
// Shape: the five frame waits are written out; each string literal goes straight to a `const CCharString&` parameter.
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
class CWideString { public:
    void* PStringData;
    CWideString(const unsigned short* text);            // 0x0099B6B0 (wchar_t is unsigned short under VC7.1)
    ~CWideString();                                     // 0x0099B510
};
class CScriptThing;
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
struct CQ_SunnyvaleMasterData {
    unsigned char _pad0[0x9d];
    bool ScorpionsDestroyedCutscenePlayed;              // +0x9d
};
class CScriptBase { public:                                    // retail 0x40 bytes
    unsigned char _body[0x40];
    bool IsActiveThreadTerminating();                   // 0x00CB7940
};
namespace NScript {
class CQ_GuildTrainingWoodsMeleeScript : public CScriptBase {
public:
    CGameScriptInterface* Game;                         // +0x40
    CQ_SunnyvaleMasterData* PMasterData;                // +0x44
    bool MissionSucceeded;                              // +0x48
    bool MissionFailed;                                 // +0x49
    bool MissionOver;                                   // +0x4a
    bool ScorpionsAlive;                                // +0x4b
    void WatchForTermination();
};
} // namespace NScript
using NScript::CQ_GuildTrainingWoodsMeleeScript;

void NScript::CQ_GuildTrainingWoodsMeleeScript::WatchForTermination()
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
        Game->SetQuestAsCompleted(Game->GetActiveQuestName(), true, false, false);
    }
    Game->SetQuestCardObjective("Q_GuildTraining", "TEXT_QUEST_GUILD_TRAINING_OBJECTIVE_02", "HeroGuildComplexInside", "");
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    long conversation = Game->AddNewConversation(Game->GetHero(), false, false);
    Game->AddLineToConversation(conversation, "TEXT_QST_028_GUILDSEAL_COME_BACK", false, Game->GetHero(), Game->GetHero());
    if (MissionSucceeded) {
        if (IsActiveThreadTerminating())
            return;
        PMasterData->ScorpionsDestroyedCutscenePlayed = true;
    }
    while (!Game->IsLevelLoaded("HeroGuildComplex")) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->DeactivateQuestLater("Q_GuildTrainingWoodsMelee", 0);
}