#include <cstdio>
#include <cstring>

// Behaviour test for 0x00CE5050 NScript::CHeroBoastsScript::BoastBreakSiegeNoAssistance: the three outcomes (quest failed,
// quest completed while unassisted, guards hired -> game info then failure) and thread termination at each wait.
class CCharString { public: const char* text; CCharString(const char* t) : text(t) { } };

struct FakeGame {
    int frames, failedAt, completedAt, clickedAt, terminateAt;   // frame numbers at which each event fires (-1: never)
    int boastFailed, boastCompleted, infoShown;
    char info[64];
    void NewScriptFrame() { ++frames; }
    bool MsgOnQuestFailedBeforeScreenShown(const CCharString&) { return failedAt >= 0 && frames >= failedAt; }
    bool MsgOnQuestCompletedBeforeScreenShown(const CCharString&) { return completedAt >= 0 && frames >= completedAt; }
    bool MsgIsGameInfoClickedPast() { return clickedAt >= 0 && frames >= clickedAt; }
    void DisplayGameInfo(const CCharString& s) { ++infoShown; std::strcpy(info, s.text); }
    void SetBoastAsFailed(long id, const CCharString&) { boastFailed = (int)id; }
    void SetBoastAsCompleted(long id, const CCharString&) { boastCompleted = (int)id; }
};
struct MasterData { bool BreakSiegeNoAssistance; int hireAt; };

struct CHeroBoastsScript {
    FakeGame* Game;
    MasterData* MasterScriptData;
    bool IsActiveThreadTerminating()
    {
        if (MasterScriptData->hireAt >= 0 && Game->frames >= MasterScriptData->hireAt) MasterScriptData->BreakSiegeNoAssistance = false;
        return Game->terminateAt >= 0 && Game->frames >= Game->terminateAt;
    }
    void BoastBreakSiegeNoAssistance(CCharString questName)
    {
        while (!IsActiveThreadTerminating()) {
            if (!MasterScriptData->BreakSiegeNoAssistance) {
                if (IsActiveThreadTerminating()) return;
                Game->DisplayGameInfo("TEXT_QST_BOAST_STATE_FAILED_HIRENOGUARDS");
                while (!Game->MsgIsGameInfoClickedPast()) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) return;
                }
                if (IsActiveThreadTerminating()) return;
                Game->SetBoastAsFailed(25, questName);
                return;
            }
            if (Game->MsgOnQuestFailedBeforeScreenShown(questName)) {
                if (IsActiveThreadTerminating()) return;
                Game->SetBoastAsFailed(25, questName);
                return;
            }
            if (Game->MsgOnQuestCompletedBeforeScreenShown(questName)) {
                if (IsActiveThreadTerminating()) return;
                Game->SetBoastAsCompleted(25, questName);
                return;
            }
            Game->NewScriptFrame();
        }
    }
};

static int Run(int failedAt, int completedAt, int clickedAt, int terminateAt, int hireAt, FakeGame& g)
{
    std::memset(&g, 0, sizeof(g));
    g.failedAt = failedAt; g.completedAt = completedAt; g.clickedAt = clickedAt; g.terminateAt = terminateAt;
    MasterData m; m.BreakSiegeNoAssistance = true; m.hireAt = hireAt;
    CHeroBoastsScript s; s.Game = &g; s.MasterScriptData = &m;
    s.BoastBreakSiegeNoAssistance(CCharString("Q_BreakSiege"));
    return 0;
}

int main()
{
    int bad = 0;
    FakeGame g;
    Run(-1, 4, -1, -1, -1, g);       // completed unassisted after 4 frames
    bad |= g.boastCompleted != 25 || g.boastFailed != 0 || g.frames != 4 || g.infoShown != 0;
    Run(3, -1, -1, -1, -1, g);       // quest failed
    bad |= g.boastFailed != 25 || g.boastCompleted != 0 || g.frames != 3;
    Run(-1, 9, 5, -1, 2, g);         // guards hired at frame 2: game info, clicked past at 5, then failure
    bad |= g.infoShown != 1 || std::strcmp(g.info, "TEXT_QST_BOAST_STATE_FAILED_HIRENOGUARDS") != 0;
    bad |= g.boastFailed != 25 || g.boastCompleted != 0 || g.frames != 5;
    Run(-1, -1, -1, 3, 1, g);        // terminated while waiting for the click: nothing reported
    bad |= g.infoShown != 1 || g.boastFailed != 0 || g.boastCompleted != 0 || g.frames != 3;
    Run(-1, -1, -1, 0, -1, g);       // terminating on entry
    bad |= g.frames != 0 || g.boastFailed != 0 || g.boastCompleted != 0;
    if (bad) { std::printf("00ce5050_TEST FAIL\n"); return 1; }
    std::printf("00ce5050_TEST PASS\n");
    return 0;
}