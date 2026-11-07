// Behaviour test for NScript::CV_BanditTollScript::CheckForLeaderDead (0x00E482F0): a copy of the logic over a mock game with a
// scripted liveness timeline; checks the wait for LeaderDead, the quest completion, the leader's on-kill line (only while the
// taunting bandit is still alive), the BanditsFlee / IsFactionSet flags, the three per-name wait loops (re-fetching the nearest
// bandit of that name until none is alive), the three sign text tags and the deferred deactivation; plus the dead-leader path.
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

struct Thing { int aliveFrames; bool IsAlive() const { return aliveFrames > 0; } };
struct MockGame {
    std::vector<std::string> log; int frame; int taunting, toll, blocking;   // how many frames each bandit stays alive
    void NewScriptFrame() { ++frame; }
    long GetHero() { return 1; }
    const char* GetActiveQuestName() { return "QuestBanditToll"; }
    void SetQuestAsCompleted(const char* q, bool, bool, bool) { log.push_back(std::string("complete:") + q); }
    Thing GetNearestWithScriptName(long, const char* name)
    {
        Thing t; int left = 0;
        if (!std::strcmp(name, "TauntingBandit")) left = taunting; else if (!std::strcmp(name, "TollBandit")) left = toll; else left = blocking;
        t.aliveFrames = left - frame;        // alive while the frame counter is below the bandit's death frame
        return t;
    }
    long AddNewConversation(const Thing&, bool, bool) { log.push_back("conversation"); return 42; }
    void AddPersonToConversation(long c, long) { if (c == 42) log.push_back("person"); }
    void AddLineToConversation(long c, const char* line, bool, const Thing&, long) { if (c == 42) log.push_back(std::string("line:") + line); }
    long GetThingWithScriptName(const char* name) { log.push_back(std::string("thing:") + name); return 5; }
    void SetReadableObjectTextTag(long, const char* tag) { log.push_back(std::string("tag:") + tag); }
    void DeactivateQuestLater(const char* q, long) { log.push_back(std::string("deactivate:") + q); }
};
struct Script {
    MockGame* Game; bool LeaderDead, BanditsFlee, IsFactionSet; int leaderDeadAtFrame; int terminateAt, checks;
    bool IsActiveThreadTerminating() { if (Game->frame >= leaderDeadAtFrame) LeaderDead = true; return ++checks == terminateAt; }
    void CheckForLeaderDead()
    {
        while (!LeaderDead) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
        if (IsActiveThreadTerminating()) return;
        Game->SetQuestAsCompleted(Game->GetActiveQuestName(), false, false, false);
        Thing Leader = Game->GetNearestWithScriptName(Game->GetHero(), "TauntingBandit");
        if (Leader.IsAlive()) {
            if (IsActiveThreadTerminating()) return;
            long conversation = Game->AddNewConversation(Leader, false, false);
            Game->AddPersonToConversation(conversation, Game->GetHero());
            Game->AddLineToConversation(conversation, "TEXT_QST_011_BANDIT_ON_KILL_LEADER_10", false, Leader, Game->GetHero());
            BanditsFlee = true;
            IsFactionSet = false;
            const char* names[3] = { "TauntingBandit", "TollBandit", "BlockingBandit" };
            for (int n = 0; n < 3; ++n) {
                Thing bandit = Game->GetNearestWithScriptName(Game->GetHero(), names[n]);
                while (bandit.IsAlive()) {
                    Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return;
                    while (Game->GetNearestWithScriptName(Game->GetHero(), names[n]).IsAlive()) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
                    if (IsActiveThreadTerminating()) return;
                    bandit = Game->GetNearestWithScriptName(Game->GetHero(), names[n]);
                }
                if (IsActiveThreadTerminating()) return;
            }
        }
        Game->SetReadableObjectTextTag(Game->GetThingWithScriptName("BT_Sign_Far"), "TEXT_QST_011_SIGN_FAR_GONE");
        Game->SetReadableObjectTextTag(Game->GetThingWithScriptName("BT_Sign_Close"), "TEXT_QST_011_SIGN_CLOSE_GONE");
        Game->SetReadableObjectTextTag(Game->GetThingWithScriptName("BT_Sign_Closer"), "TEXT_QST_011_SIGN_CLOSER_GONE");
        Game->DeactivateQuestLater(Game->GetActiveQuestName(), 0);
    }
};
static std::string Join(const std::vector<std::string>& v) { std::string s; for (size_t i = 0; i < v.size(); ++i) { if (i) s += " "; s += v[i]; } return s; }
int main()
{
    const char* tail = "thing:BT_Sign_Far tag:TEXT_QST_011_SIGN_FAR_GONE thing:BT_Sign_Close tag:TEXT_QST_011_SIGN_CLOSE_GONE thing:BT_Sign_Closer tag:TEXT_QST_011_SIGN_CLOSER_GONE deactivate:QuestBanditToll";
    MockGame g; g.frame = 0; g.taunting = 6; g.toll = 8; g.blocking = 4;
    Script s; s.Game = &g; s.LeaderDead = false; s.BanditsFlee = false; s.IsFactionSet = true; s.leaderDeadAtFrame = 3; s.terminateAt = 0; s.checks = 0;
    s.CheckForLeaderDead();
    if (Join(g.log) != std::string("complete:QuestBanditToll conversation person line:TEXT_QST_011_BANDIT_ON_KILL_LEADER_10 ") + tail || !s.BanditsFlee || s.IsFactionSet || g.frame != 8) { std::printf("00e482f0_TEST FAIL\n"); return 1; }
    // the leader is already dead when the flag is seen: no conversation, no waits
    MockGame h; h.frame = 0; h.taunting = 0; h.toll = 0; h.blocking = 0; s.Game = &h; s.LeaderDead = true; s.BanditsFlee = false; s.IsFactionSet = true; s.checks = 0;
    s.CheckForLeaderDead();
    if (Join(h.log) != std::string("complete:QuestBanditToll ") + tail || s.BanditsFlee || !s.IsFactionSet || h.frame != 0) { std::printf("00e482f0_TEST FAIL\n"); return 1; }
    // terminated inside the first wait loop: nothing is logged
    MockGame k; k.frame = 0; k.taunting = 6; k.toll = 8; k.blocking = 4; s.Game = &k; s.LeaderDead = false; s.leaderDeadAtFrame = 100; s.terminateAt = 2; s.checks = 0;
    s.CheckForLeaderDead();
    if (!k.log.empty()) { std::printf("00e482f0_TEST FAIL\n"); return 1; }
    std::printf("00e482f0_TEST PASS\n");
    return 0;
}
