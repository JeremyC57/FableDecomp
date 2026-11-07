// Behaviour test for NScript::CQ_BanditCampBossBattleScript::CheckForFirstAreaMassacre (0x00D0EE70): a copy of the watcher over a
// mock game; checks the Gate1 wait, the per-frame kill tally over the hero, the hero's followers and the summoned creatures
// (each list cleared after counting), the BanditCampMain reload wait, and the massacre response once the tally reaches the def's
// BAC_EnemiesToKillInFirstArea (alarm cut-scene, Gate2Guard1 removed, Gate2Open, every DefensiveGuardBandit and a new level-2
// leader targeted at the hero), plus the terminate exit.
#include <cstdio>
#include <string>
#include <vector>

struct MockGame {
    std::vector<std::string> log; int frame; int gateOpenAt; int reloadFrames; int heroKillsPerFrame, followerKills, summonedKills;
    bool IsOpenDoor(long) { return frame >= gateOpenAt; }
    long GetThingWithScriptName(const char* n) { log.push_back(std::string("thing:") + n); return 3; }
    bool IsLevelLoaded(const char*) { return reloadFrames-- > 0; }
    void NewScriptFrame() { ++frame; }
    long GetHero() { return 1; }
    bool MsgGetThingsKilled(long who, std::vector<unsigned long>& kills) { int n = who == 1 ? heroKillsPerFrame : who == 2 ? followerKills : summonedKills; for (int i = 0; i < n; ++i) kills.push_back(i); return n > 0; }
    void GetFollowingEntityList(long, std::vector<long>& v) { v.clear(); v.push_back(2); }
    void GetHeroSummonedCreaturesList(std::vector<long>& v) { v.clear(); v.push_back(4); v.push_back(4); }
    void StartMovieSequence(const char*, int) { log.push_back("movie"); }
    void PauseAllNonScriptedEntities(bool on) { log.push_back(on ? "pause" : "unpause"); }
    void StartScriptingEntity(long, int&, int) { log.push_back("script_hero"); }
    void FadeScreenOut(float, float, int) { log.push_back("fade"); }
    void Pause(float) { log.push_back("wait"); }
    void RemoveThing(long, bool, bool) { log.push_back("remove"); }
    void GetAllThingsWithScriptName(const char* n, std::vector<long>& v) { log.push_back(std::string("all:") + n); v.clear(); v.push_back(7); v.push_back(8); }
    void GiveThingBestEnemyTarget(long t, long) { log.push_back(std::string("target:") + (char)('0' + t)); }
    long CreateCreature(const char* def, long, const char*, bool) { log.push_back(std::string("create:") + def); return 9; }
    long GetPos(long) { return 0; }
};
struct Pause { MockGame* g; Pause(MockGame& game) : g(&game) { g->PauseAllNonScriptedEntities(true); } ~Pause() { g->PauseAllNonScriptedEntities(false); } };
struct Def { float BAC_EnemiesToKillInFirstArea; };
struct Script {
    MockGame* Game; Def* def; bool Gate2Open; int terminateAt, checks;
    bool IsActiveThreadTerminating() { return ++checks == terminateAt; }
    void RunCutsceneMacro(const char* n) { Game->log.push_back(std::string("cs:") + n); }
    void CheckForFirstAreaMassacre()
    {
        while (!Game->IsOpenDoor(Game->GetThingWithScriptName("Gate1"))) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
        if (IsActiveThreadTerminating()) return;
        long killed = 0;
        for (; !IsActiveThreadTerminating(); Game->NewScriptFrame()) {
            if (Game->IsLevelLoaded("BanditCampMain")) {
                if (IsActiveThreadTerminating()) return;
                while (Game->IsLevelLoaded("BanditCampMain")) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
                if (IsActiveThreadTerminating()) return;
            }
            std::vector<unsigned long> kills;
            if (Game->MsgGetThingsKilled(Game->GetHero(), kills)) { if (IsActiveThreadTerminating()) return; killed += kills.size(); kills.clear(); }
            std::vector<long> things;
            Game->GetFollowingEntityList(Game->GetHero(), things);
            for (unsigned long i = 0; i < things.size(); i++) { if (IsActiveThreadTerminating()) return; if (Game->MsgGetThingsKilled(things[i], kills)) { if (IsActiveThreadTerminating()) return; killed += kills.size(); kills.clear(); } }
            if (IsActiveThreadTerminating()) return;
            Game->GetHeroSummonedCreaturesList(things);
            for (unsigned long i = 0; i < things.size(); i++) { if (IsActiveThreadTerminating()) return; if (Game->MsgGetThingsKilled(things[i], kills)) { if (IsActiveThreadTerminating()) return; killed += kills.size(); kills.clear(); } }
            if (IsActiveThreadTerminating()) return;
            if ((float)killed >= def->BAC_EnemiesToKillInFirstArea) {
                if (IsActiveThreadTerminating()) return;
                {
                    int movie = 0; Game->StartMovieSequence("", movie);
                    Pause PauseMyEntitiesPlease(*Game);
                    int seh_Hero = 0; Game->StartScriptingEntity(Game->GetHero(), seh_Hero, 4);
                    Game->FadeScreenOut(0.5f, 0.5f, 0); Game->Pause(1.0f);
                    RunCutsceneMacro("CS_BANDITCAMP_ALARM_OUTER");
                    Game->RemoveThing(Game->GetThingWithScriptName("Gate2Guard1"), false, true);
                }
                Gate2Open = true;
                std::vector<long> bandits; Game->GetAllThingsWithScriptName("DefensiveGuardBandit", bandits);
                for (unsigned long i = 0; i < bandits.size(); i++) { if (IsActiveThreadTerminating()) return; Game->GiveThingBestEnemyTarget(bandits[i], Game->GetHero()); }
                if (IsActiveThreadTerminating()) return;
                long leader = Game->CreateCreature("CREATURE_BANDIT_LEADER_LEVEL2", Game->GetPos(Game->GetThingWithScriptName("DefensiveGuardLeader")), "", false);
                Game->GiveThingBestEnemyTarget(leader, Game->GetHero());
                return;
            }
        }
    }
};
static std::string Join(const std::vector<std::string>& v) { std::string s; for (size_t i = 0; i < v.size(); ++i) { if (i) s += " "; s += v[i]; } return s; }
int main()
{
    MockGame g; g.frame = 0; g.gateOpenAt = 2; g.reloadFrames = 1; g.heroKillsPerFrame = 2; g.followerKills = 1; g.summonedKills = 1;   // 5 kills per frame
    Def d; d.BAC_EnemiesToKillInFirstArea = 12.0f;
    Script s; s.Game = &g; s.def = &d; s.Gate2Open = false; s.terminateAt = 0; s.checks = 0;
    s.CheckForFirstAreaMassacre();
    const char* expected = "thing:Gate1 thing:Gate1 thing:Gate1 movie pause script_hero fade wait cs:CS_BANDITCAMP_ALARM_OUTER thing:Gate2Guard1 remove unpause all:DefensiveGuardBandit target:7 target:8 thing:DefensiveGuardLeader create:CREATURE_BANDIT_LEADER_LEVEL2 target:9";
    // gate opens at frame 2; the one-frame reload flag is consumed by the first test (the inner wait sees it clear); frames 2, 3, 4 tally 5 kills each -> 15 >= 12 on the third tally
    if (Join(g.log) != expected || !s.Gate2Open || g.frame != 4) { std::printf("00d0ee70_TEST FAIL\n"); return 1; }
    MockGame h = g; h.log.clear(); h.frame = 0; h.gateOpenAt = 0; h.reloadFrames = 0; s.Game = &h; s.Gate2Open = false; s.checks = 0; s.terminateAt = 4;
    s.CheckForFirstAreaMassacre();
    if (Join(h.log) != "thing:Gate1" || s.Gate2Open) { std::printf("00d0ee70_TEST FAIL\n"); return 1; }
    std::printf("00d0ee70_TEST PASS\n");
    return 0;
}
