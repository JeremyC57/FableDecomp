// Behaviour test for NScript::CQ_RansomVictimScript::DisappearFollowers (0x00DE4710): a copy of the thread logic over a mock
// game whose region flag toggles per frame; checks that followers are hidden (alpha 0, not pushable/hittable/damageable/
// targetable, no emote icon, hidden on the mini-map) while WitchwoodCavern is loaded and the victim is not rescued, restored
// when the region unloads, left alone once RescuedVictim is set, and that the thread stops on terminate.
#include <cstdio>
#include <vector>
#include <string>

struct Follower { float alpha; bool pushable, hit, damageable, targetable, emote, hiddenMap; };
struct MockGame {
    std::vector<int> region;           // per frame: 1 = WitchwoodCavern loaded
    size_t frame;
    std::vector<Follower> followers;
    int scripted, released, startFails;
    bool IsRegionLoaded(const char*) { return frame < region.size() ? region[frame] != 0 : false; }
    void NewScriptFrame() { ++frame; }
    long GetHero() { return 1; }
    void GetFollowingEntityList(long, std::vector<long>& out) { out.clear(); for (size_t i = 0; i < followers.size(); ++i) out.push_back((long)i); }
    bool StartScriptingEntity(long, int& seh, int) { if (startFails > 0) { --startFails; return false; } seh = 1; ++scripted; return true; }
    void EntitySetAlpha(long i, float a, bool) { followers[i].alpha = a; }
    void SetIsThingForcePushable(long i, bool v) { followers[i].pushable = v; }
    void EntitySetAsRespondToHit(long i, bool v) { followers[i].hit = v; }
    void EntitySetAsDamageable(long i, bool v) { followers[i].damageable = v; }
    void EntitySetTargetable(long i, bool v) { followers[i].targetable = v; }
    void EntitySetAsDisplayingEmoteIcon(long i, bool v) { followers[i].emote = v; }
    void EntitySetAsHiddenOnMiniMap(long i, bool v) { followers[i].hiddenMap = v; }
};
struct Script {
    MockGame* Game; bool RescuedVictim; int terminateAt, checks;
    bool IsActiveThreadTerminating() { return ++checks >= terminateAt; }
    void DisappearFollowers()
    {
        std::vector<long> followers;
        std::vector<int> seh_followers;
        for (;;) {
            if (IsActiveThreadTerminating()) return;
            if (Game->IsRegionLoaded("WitchwoodCavern") && !RescuedVictim) {
                if (IsActiveThreadTerminating()) return;
                Game->GetFollowingEntityList(Game->GetHero(), followers);
                long num = followers.size();
                seh_followers.resize(num);
                for (long i = 0; i < num; i++) {
                    if (IsActiveThreadTerminating()) return;
                    int& seh = seh_followers[i];
                    if (seh) { seh = 0; ++Game->released; }
                    while (!Game->StartScriptingEntity(followers[i], seh_followers[i], 4)) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
                    if (IsActiveThreadTerminating()) return;
                    Game->EntitySetAlpha(followers[i], 0.0f, true);
                    Game->SetIsThingForcePushable(followers[i], false);
                    Game->EntitySetAsRespondToHit(followers[i], false);
                    Game->EntitySetAsDamageable(followers[i], false);
                    Game->EntitySetTargetable(followers[i], false);
                    Game->EntitySetAsDisplayingEmoteIcon(followers[i], false);
                    Game->EntitySetAsHiddenOnMiniMap(followers[i], true);
                }
                if (IsActiveThreadTerminating()) return;
                while (Game->IsRegionLoaded("WitchwoodCavern") != false) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
                if (IsActiveThreadTerminating()) return;
                for (long i = 0; i < num; i++) {
                    if (IsActiveThreadTerminating()) return;
                    int& seh = seh_followers[i];
                    if (seh) { seh = 0; ++Game->released; }
                    Game->EntitySetAlpha(followers[i], 1.0f, true);
                    Game->SetIsThingForcePushable(followers[i], true);
                    Game->EntitySetAsRespondToHit(followers[i], true);
                    Game->EntitySetAsDamageable(followers[i], true);
                    Game->EntitySetTargetable(followers[i], true);
                    Game->EntitySetAsDisplayingEmoteIcon(followers[i], true);
                    Game->EntitySetAsHiddenOnMiniMap(followers[i], false);
                }
                if (IsActiveThreadTerminating()) return;
            }
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
        }
    }
};
static bool Hidden(const Follower& f) { return f.alpha == 0.0f && !f.pushable && !f.hit && !f.damageable && !f.targetable && !f.emote && f.hiddenMap; }
static bool Shown(const Follower& f) { return f.alpha == 1.0f && f.pushable && f.hit && f.damageable && f.targetable && f.emote && !f.hiddenMap; }
static MockGame Make(int fails) { MockGame g; g.frame = 0; g.scripted = 0; g.released = 0; g.startFails = fails; Follower f = { 0.5f, false, false, false, false, false, false }; g.followers.push_back(f); g.followers.push_back(f); return g; }
int main()
{
    // frames: not loaded, loaded (hide), loaded, loaded, unloaded (restore), then the thread is terminated at a later check
    MockGame g = Make(1); int r[] = { 0, 1, 1, 1, 0, 0, 0 }; g.region.assign(r, r + 7);
    Script s; s.Game = &g; s.RescuedVictim = false; s.terminateAt = 60; s.checks = 0;
    s.DisappearFollowers();
    if (g.scripted != 2 || g.released != 2 || !Shown(g.followers[0]) || !Shown(g.followers[1])) { std::printf("00de4710_TEST FAIL\n"); return 1; }
    // terminate while hidden: the followers stay hidden (the restore pass never runs)
    g = Make(0); g.region.assign(r, r + 7); s.Game = &g; s.checks = 0; s.terminateAt = 12;
    s.DisappearFollowers();
    if (g.scripted != 2 || !Hidden(g.followers[0]) || !Hidden(g.followers[1])) { std::printf("00de4710_TEST FAIL\n"); return 1; }
    // rescued victim: nothing happens even with the region loaded
    g = Make(0); g.region.assign(r, r + 7); s.Game = &g; s.RescuedVictim = true; s.checks = 0; s.terminateAt = 20;
    s.DisappearFollowers();
    if (g.scripted != 0 || g.followers[0].alpha != 0.5f) { std::printf("00de4710_TEST FAIL\n"); return 1; }
    std::printf("00de4710_TEST PASS\n");
    return 0;
}
