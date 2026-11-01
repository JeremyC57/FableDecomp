// Behaviour test for NScript::CV_TalentlessBardScript::Init (0x00ED83C0): flags and counters cleared, the wander time
// taken from the global script def, the twelve quest name/tag pairs filled in retail order, the song-availability
// vector sized to twelve false entries (the fill loop checks the thread each step).
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <vector>
class CCharString { public:
    CCharString() : PStringData(0) {}
    ~CCharString() { free(PStringData); }
    CCharString& operator=(const char* t) { free(PStringData); PStringData = strdup(t); return *this; }
    char* PStringData;
};
class CScriptDef { public: float BardWanderResetTime; };
static CScriptDef g_scriptDef = { 42.5f };
static CScriptDef* pGlobalScriptDef = &g_scriptDef;
namespace NScript {
class CV_TalentlessBardScript {
public:
    struct QuestData { CCharString name; CCharString tag; };
    bool bHurtBard, bBard_finished, bRewardGiven, BardIntroComplete, AttackedByHero, BardFollowing;
    long lNumber_of_things_sung_about; float BardWanderTime;
    QuestData QuestInfo[12];
    std::vector<bool> SongThingAvailable;
    int terminateAt, checks;
    CV_TalentlessBardScript() : bHurtBard(true), bBard_finished(true), bRewardGiven(true), BardIntroComplete(true), AttackedByHero(true), BardFollowing(true), lNumber_of_things_sung_about(7), BardWanderTime(0), terminateAt(1000), checks(0) {}
    bool IsActiveThreadTerminating() { return ++checks > terminateAt; }
    void Init();
};
void CV_TalentlessBardScript::Init()
{
    AttackedByHero = false;
    BardFollowing = false;
    BardIntroComplete = false;
    bBard_finished = false;
    bRewardGiven = false;
    bHurtBard = false;
    lNumber_of_things_sung_about = 0;
    BardWanderTime = pGlobalScriptDef->BardWanderResetTime;
    QuestInfo[9].name = "Q_WaspBoss";                      QuestInfo[9].tag = "TEXT_QST_163_QUEST_WASP_BOSS";
    QuestInfo[5].name = "Q_OrchardFarmRaidGood";           QuestInfo[5].tag = "TEXT_QST_163_QUEST_ORCHARD_FARM_GOOD";
    QuestInfo[6].name = "Q_OrchardFarmRaidEvil";           QuestInfo[6].tag = "TEXT_QST_163_QUEST_ORCHARD_FARM_EVIL";
    QuestInfo[10].name = "Q_WhiteBalverineKnotholeGlade";  QuestInfo[10].tag = "TEXT_QST_163_QUEST_WHITE_BALV";
    QuestInfo[7].name = "Q_PrisonEscapeRescueMother";      QuestInfo[7].tag = "TEXT_QST_163_QUEST_PRISON";
    QuestInfo[1].name = "Q_BanditCamp";                    QuestInfo[1].tag = "TEXT_QST_163_QUEST_BANDIT_CAMP";
    QuestInfo[2].name = "Q_OpeningGraveyardSecretPassage"; QuestInfo[2].tag = "TEXT_QST_163_QUEST_GRAVEYARD";
    QuestInfo[8].name = "Q_TraderEscort";                  QuestInfo[8].tag = "TEXT_QST_163_QUEST_DARKWOOD_TRADERS";
    QuestInfo[11].name = "Q_WizardBattle";                 QuestInfo[11].tag = "TEXT_QST_163_QUEST_WIZARD_BATTLE";
    QuestInfo[0].name = "Q_Arena";                         QuestInfo[0].tag = "TEXT_QST_163_QUEST_ARENA";
    QuestInfo[3].name = "Q_EndGameBossBattle";             QuestInfo[3].tag = "TEXT_QST_163_QUEST_JACK";
    SongThingAvailable.resize(12, false);
    for (unsigned int i = 0; i < SongThingAvailable.size(); ++i) {
        if (IsActiveThreadTerminating())
            return;
        SongThingAvailable[i] = false;
    }
    if (IsActiveThreadTerminating())
        return;
}
}
int main()
{
    NScript::CV_TalentlessBardScript s;
    s.Init();
    if (s.bHurtBard || s.bBard_finished || s.bRewardGiven || s.BardIntroComplete || s.AttackedByHero || s.BardFollowing || s.lNumber_of_things_sung_about != 0) { printf("00ed83c0_TEST FAIL flags\n"); return 1; }
    if (s.BardWanderTime != 42.5f) { printf("00ed83c0_TEST FAIL wander\n"); return 1; }
    if (strcmp(s.QuestInfo[0].name.PStringData, "Q_Arena") || strcmp(s.QuestInfo[0].tag.PStringData, "TEXT_QST_163_QUEST_ARENA") ||
        strcmp(s.QuestInfo[3].name.PStringData, "Q_EndGameBossBattle") || strcmp(s.QuestInfo[11].tag.PStringData, "TEXT_QST_163_QUEST_WIZARD_BATTLE") ||
        s.QuestInfo[4].name.PStringData != 0) { printf("00ed83c0_TEST FAIL quests\n"); return 1; }
    if (s.SongThingAvailable.size() != 12) { printf("00ed83c0_TEST FAIL size\n"); return 1; }
    for (unsigned int i = 0; i < 12; ++i) if (s.SongThingAvailable[i]) { printf("00ed83c0_TEST FAIL song %u\n", i); return 1; }
    if (s.checks != 13) { printf("00ed83c0_TEST FAIL checks=%d\n", s.checks); return 1; }
    printf("00ed83c0_TEST PASS\n");
    return 0;
}