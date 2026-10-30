// CV_MayorsInvitationScript::MonitorThunderHeroDuel() -- (single instance) point the quest card at the Bowerstone Jail duel, make sure the Hero Duel quest is running, then wait for the duel's outcome and record whether the hero beat Thunder
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EB70F0, 0x276 bytes, plain `ret`. PDB ?MonitorThunderHeroDuel@CV_MayorsInvitationScript@NScript@@QAEXXZ.
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, IsActiveThreadTerminating 0x00CB7940;
//   CGameScriptInterface slots 7 NewScriptFrame, 276 ActivateQuest, 280 DeactivateQuest, 284 IsQuestActive,
//   286 IsQuestCompleted, 296 SetQuestCardObjective, 655 GetActiveQuestName.
// Shape: `!IsQuestActive(..) && !IsQuestCompleted(..)` keeps conditional-destruction bits (1, 3) for its two temporaries
//   (VC7.1 reuses the ebx = 1 it loaded for the MonitorThunderHeroDuelRunning store); the outcome wait is
//   `while (!won && !lost)` with an early-out block for an already-finished duel.
// Layout (retail = Ego_r.pdb - 0x14): Game +0x40, the shared master data +0x44 (its +0x61 / +0x62 / +0x63 are unnamed
//   hero-duel outcome flags), HeroHasDefeatedThunder +0x4e, HeroHasLostToThunder +0x4f, MonitorThunderHeroDuelRunning +0x5c.
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
class CScriptBase { public:                                     // retail 0x40 bytes
    unsigned char _body[0x40];
    bool IsActiveThreadTerminating();                    // 0x00CB7940
};
struct CMasterScriptData {
    unsigned char _pad00[0x61];
    bool HeroWonThunderDuel;                             // +0x61 (unnamed)
    bool HeroLostThunderDuel;                            // +0x62 (unnamed)
    bool ThunderDuelResolved;                            // +0x63 (unnamed)
};
namespace NScript {
class CV_MayorsInvitationScript : public CScriptBase {
public:
    CGameScriptInterface* Game;                          // +0x40
    CMasterScriptData* PMasterData;                      // +0x44
    bool MissionSucceeded;                               // +0x48
    bool MissionFailed;                                  // +0x49
    bool MissionOver;                                    // +0x4a
    bool HeroAbortedQuest;                               // +0x4b
    bool HeroLearnedSecret;                              // +0x4c
    bool HeroReturnedFromParanoidQuest;                  // +0x4d
    bool HeroHasDefeatedThunder;                         // +0x4e
    bool HeroHasLostToThunder;                           // +0x4f
    bool QuestCardGiven;                                 // +0x50
    long SecretsLearned;                                 // +0x54
    long WhichQuest;                                     // +0x58
    long MonitorThunderHeroDuelRunning;                  // +0x5c
    void MonitorThunderHeroDuel();
};
} // namespace NScript
using NScript::CV_MayorsInvitationScript;

void NScript::CV_MayorsInvitationScript::MonitorThunderHeroDuel()
{
    if (MonitorThunderHeroDuelRunning) {
        if (IsActiveThreadTerminating())
            return;
        return;
    }
    if (IsActiveThreadTerminating())
        return;
    MonitorThunderHeroDuelRunning = 1;
    if (!PMasterData->ThunderDuelResolved) {
        if (IsActiveThreadTerminating())
            return;
        Game->SetQuestCardObjective(Game->GetActiveQuestName(), "TEXT_QUEST_MAYORS_INVITATION_OBJECTIVE_07", "BowerstoneJail", "");
    }
    if (!Game->IsQuestActive("V_HeroDuel") && !Game->IsQuestCompleted("V_HeroDuel"))
        Game->ActivateQuest("V_HeroDuel");
    while (!PMasterData->HeroWonThunderDuel && !PMasterData->HeroLostThunderDuel) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        if (PMasterData->ThunderDuelResolved) {
            if (IsActiveThreadTerminating())
                return;
            HeroHasDefeatedThunder = true;
            if (Game->IsQuestActive("V_HeroDuel")) {
                if (IsActiveThreadTerminating())
                    return;
                Game->DeactivateQuest("V_HeroDuel", 0);
            }
            return;
        }
    }
    if (IsActiveThreadTerminating())
        return;
    if (PMasterData->HeroWonThunderDuel) {
        if (IsActiveThreadTerminating())
            return;
        HeroHasDefeatedThunder = true;
    } else {
        if (IsActiveThreadTerminating())
            return;
        HeroHasLostToThunder = true;
    }
}