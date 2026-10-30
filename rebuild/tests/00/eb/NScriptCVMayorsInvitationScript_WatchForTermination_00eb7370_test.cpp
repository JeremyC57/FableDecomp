// Behaviour test for NScript::CV_MayorsInvitationScript::WatchForTermination (0x00EB7370): wait until the hero aborts, the
// mission fails or succeeds; on success complete the quest, make Lady Grey marryable and force the marriage, record it and
// (after a frame) activate LadyGreyWifeManager; otherwise deactivate V_ExposeMayor and clear Lady Grey's marker; then clear
// the wise villager's information and deactivate the quest.
#include <cstdio>
#include <cstring>

static char gLog[1024];
static void Log(const char* s) { std::strcat(gLog, s); std::strcat(gLog, ";"); }

struct Thing { const char* Name; };
struct Game {
    int frames, endAt; bool* flag;
    void NewScriptFrame() { ++frames; if (frames == endAt) *flag = true; }
    const char* GetActiveQuestName() { return "V_MayorsInvitation"; }
    void SetQuestAsCompleted(const char* q, bool, bool, bool) { Log("completed"); Log(q); }
    Thing GetThingWithScriptName(const char* n) { Thing t; t.Name = n; return t; }
    void ClearThingHasInformation(const Thing& t) { Log("noinfo"); Log(t.Name); }
    void MiniMapRemoveMarker(const Thing& t) { Log("nomarker"); Log(t.Name); }
    void EntitySetAsMarryable(const Thing& t, bool on) { if (on) { Log("marryable"); Log(t.Name); } }
    void EntityForceMarriageToHero(const Thing& t, bool) { Log("married"); Log(t.Name); }
    void ActivateQuest(const char* q) { Log("activate"); Log(q); }
    void DeactivateQuestLater(const char* q, unsigned long d) { if (d == 0) { Log("deactivate"); Log(q); } }
};
struct MasterData { bool HeroMarriedLadyGrey; };

struct CV_MayorsInvitationScript {
    Game* Game_; MasterData* MasterScriptData;
    bool MissionSucceeded, MissionFailed, HeroAbortedQuest;
    bool IsActiveThreadTerminating() { return false; }
    void WatchForTermination()
    {
        while (!HeroAbortedQuest && !MissionFailed && !MissionSucceeded) {
            Game_->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
        }
        if (IsActiveThreadTerminating()) return;
        if (!HeroAbortedQuest && !MissionFailed) {
            if (IsActiveThreadTerminating()) return;
            Game_->SetQuestAsCompleted(Game_->GetActiveQuestName(), false, false, false);
            Thing ladyGrey = Game_->GetThingWithScriptName("LadyGrey");
            Game_->ClearThingHasInformation(ladyGrey);
            Game_->MiniMapRemoveMarker(ladyGrey);
            Game_->EntitySetAsMarryable(ladyGrey, true);
            Game_->EntityForceMarriageToHero(ladyGrey, false);
            MasterScriptData->HeroMarriedLadyGrey = true;
            Game_->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
            Game_->ActivateQuest("LadyGreyWifeManager");
        } else {
            if (IsActiveThreadTerminating()) return;
            Game_->DeactivateQuestLater("V_ExposeMayor", 0);
            Thing ladyGrey = Game_->GetThingWithScriptName("LadyGrey");
            Game_->ClearThingHasInformation(ladyGrey);
            Game_->MiniMapRemoveMarker(ladyGrey);
        }
        Thing wiseVillager = Game_->GetThingWithScriptName("WiseVillager");
        Game_->ClearThingHasInformation(wiseVillager);
        Game_->DeactivateQuestLater(Game_->GetActiveQuestName(), 0);
    }
};

int main()
{
    MasterData m; m.HeroMarriedLadyGrey = false;
    CV_MayorsInvitationScript s; Game g;
    s.Game_ = &g; s.MasterScriptData = &m; s.MissionSucceeded = false; s.MissionFailed = false; s.HeroAbortedQuest = false;
    g.frames = 0; g.endAt = 3; g.flag = &s.MissionSucceeded; gLog[0] = 0;
    s.WatchForTermination();
    if (!m.HeroMarriedLadyGrey || g.frames != 4 || std::strcmp(gLog,
        "completed;V_MayorsInvitation;noinfo;LadyGrey;nomarker;LadyGrey;marryable;LadyGrey;married;LadyGrey;activate;LadyGreyWifeManager;noinfo;WiseVillager;deactivate;V_MayorsInvitation;") != 0) {
        std::printf("00eb7370_TEST FAIL\n"); return 1;
    }
    m.HeroMarriedLadyGrey = false; s.MissionSucceeded = false; s.HeroAbortedQuest = false; g.frames = 0; g.endAt = 2; g.flag = &s.HeroAbortedQuest; gLog[0] = 0;
    s.WatchForTermination();
    if (m.HeroMarriedLadyGrey || std::strcmp(gLog,
        "deactivate;V_ExposeMayor;noinfo;LadyGrey;nomarker;LadyGrey;noinfo;WiseVillager;deactivate;V_MayorsInvitation;") != 0) {
        std::printf("00eb7370_TEST FAIL\n"); return 1;
    }
    std::printf("00eb7370_TEST PASS\n");
    return 0;
}