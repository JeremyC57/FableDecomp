#include <cstdio>
#include <cstring>
#include <vector>
// behaviour test: CQ_AmbushTradersScript::WatchForQuestSuccess -- after the three waits, the quest finishes once the convoy
// spy is (alive and) in the hero's following list while Bowerstone Posh is loaded; success is set once the speech is done
static int g_frames = 0, g_endAt = 1000, g_spyFollowsAt = 6, g_speechAt = 8;
struct CScriptThing {
    const char* name; bool alive;
    CScriptThing() : name(""), alive(true) {}
    CScriptThing(const char* n, bool a) : name(n), alive(a) {}
    bool IsAlive() const { return alive; }
    const char* GetName() const { return name; }
};
struct CQ_AmbushTradersScript;
struct CGameScriptInterface {
    CScriptThing hero;
    CQ_AmbushTradersScript* script;
    void NewScriptFrame() const;
    bool IsRegionLoaded(const char* r) const
    {
        if (!std::strcmp(r, "DesertedFarm")) return g_frames >= 1;
        if (!std::strcmp(r, "BowerstoneJail")) return g_frames >= 3;
        if (!std::strcmp(r, "BowerstonePosh")) return g_frames >= 4;
        return false;
    }
    CScriptThing& GetHero() { return hero; }
    CScriptThing GetThingWithScriptName(const char* n) const { return CScriptThing(n, true); }
    bool GetFollowingEntityList(const CScriptThing&, std::vector<CScriptThing>& out) const
    {
        out.push_back(CScriptThing("AT_ConvoySpy", false));          // a dead namesake is skipped
        out.push_back(CScriptThing("Henchman", true));
        if (g_frames >= g_spyFollowsAt) out.push_back(CScriptThing("AT_ConvoySpy", true));
        return true;
    }
};
struct CQ_AmbushTradersScript {
    CGameScriptInterface* Game;
    bool QuestFinished, QuestSucceeded, SpyFollowing, SpyFollowingHero, SpyContactMade, QuestSuccessSpeechComplete;
    bool IsActiveThreadTerminating() { return g_frames >= g_endAt; }
    void WatchForQuestSuccess();
};
void CGameScriptInterface::NewScriptFrame() const
{
    ++g_frames;
    if (g_frames == 2) script->SpyContactMade = true;
    if (g_frames == g_speechAt) script->QuestSuccessSpeechComplete = true;
}
void CQ_AmbushTradersScript::WatchForQuestSuccess()
{
    std::vector<CScriptThing> followers;
    while (!Game->IsRegionLoaded("DesertedFarm")) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
    if (IsActiveThreadTerminating()) return;
    while (!SpyContactMade) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
    if (IsActiveThreadTerminating()) return;
    while (!Game->IsRegionLoaded("BowerstoneJail")) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
    if (IsActiveThreadTerminating()) return;
    CScriptThing spy = Game->GetThingWithScriptName("AT_ConvoySpy");
    if (IsActiveThreadTerminating()) return;
    for (;;) {
        if (Game->IsRegionLoaded("BowerstonePosh") && !QuestFinished) {
            if (IsActiveThreadTerminating()) return;
            while (followers.size() != 0) followers.pop_back();
            Game->GetFollowingEntityList(Game->GetHero(), followers);
            int count = (int)followers.size();
            if (count > 0) {
                if (IsActiveThreadTerminating()) return;
                SpyFollowing = false;
                for (int i = 0; i < count; ++i) {
                    if (followers[i].IsAlive() && !std::strcmp(followers[i].GetName(), "AT_ConvoySpy")) {
                        if (IsActiveThreadTerminating()) return;
                        SpyFollowing = true;
                        break;
                    }
                }
            }
            if (SpyFollowingHero || SpyFollowing) {
                if (IsActiveThreadTerminating()) return;
                QuestFinished = true;
            }
        }
        if (QuestFinished && QuestSuccessSpeechComplete) break;
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
    }
    if (IsActiveThreadTerminating()) return;
    QuestSucceeded = true;
}
static void Reset(CQ_AmbushTradersScript& s) { g_frames = 0; s.QuestFinished = s.QuestSucceeded = s.SpyFollowing = s.SpyFollowingHero = s.SpyContactMade = s.QuestSuccessSpeechComplete = false; }
int main()
{
    CGameScriptInterface game; CQ_AmbushTradersScript s; s.Game = &game; game.script = &s;
    Reset(s);
    s.WatchForQuestSuccess();
    bool ok = s.QuestFinished && s.SpyFollowing && s.QuestSucceeded && g_frames == 8;
    Reset(s); g_spyFollowsAt = 1000; g_endAt = 20;                // the spy never follows: terminated without success
    s.WatchForQuestSuccess();
    ok = ok && !s.QuestFinished && !s.SpyFollowing && !s.QuestSucceeded && g_frames == 20;
    if (!ok) { std::printf("00f0a470_TEST FAIL\n"); return 1; }
    std::printf("00f0a470_TEST PASS\n");
    return 0;
}
