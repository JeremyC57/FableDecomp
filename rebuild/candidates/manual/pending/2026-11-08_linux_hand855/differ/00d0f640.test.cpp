// Behaviour model of NScript::CQ_BanditCampBossBattleScript::CheckForSecondAreaMassacre (0x00D0F640): each script frame (until the
// thread terminates or gate 3 is already open) wait for BanditCampResidential, then add up the things killed by the hero, by every
// follower and by every summoned creature; once the total reaches BAC_EnemiesToKillInSecondArea play CS_BANDITCAMP_ALARM_INNER
// (paused, faded), remove Gate3Guard, open gate 3, set every DefensiveGuardBandit on the hero, create the level-2 bandit leader at
// DefensiveGuardLeader and set it on the hero, then return.
#include <cstdio>
#include <string>
#include <vector>
struct Frame { bool Loaded; long HeroKills; std::vector<long> FollowerKills, SummonKills; };
struct Sim {
    std::vector<Frame> Frames; size_t Now; bool Gate3Open; float Threshold;
    std::vector<std::string> Log; int TargetsSet;
    bool Terminating() const { return Now >= Frames.size(); }
};
static void CheckForSecondAreaMassacre(Sim& g, int bandits)
{
    long killed = 0;
    for (;; ++g.Now) {
        if (g.Terminating()) return;
        while (!g.Frames[g.Now].Loaded) { ++g.Now; if (g.Terminating()) return; }
        if (g.Gate3Open) break;
        const Frame& f = g.Frames[g.Now];
        killed += f.HeroKills;
        for (size_t i = 0; i < f.FollowerKills.size(); ++i) killed += f.FollowerKills[i];
        for (size_t i = 0; i < f.SummonKills.size(); ++i) killed += f.SummonKills[i];
        if ((float)killed >= g.Threshold) {
            g.Log.push_back("CS_BANDITCAMP_ALARM_INNER");
            g.Log.push_back("remove Gate3Guard");
            g.Gate3Open = true;
            for (int i = 0; i < bandits; ++i) ++g.TargetsSet;
            g.Log.push_back("CREATURE_BANDIT_LEADER_LEVEL2");
            ++g.TargetsSet;
            return;
        }
    }
}
int main()
{
    Sim g; g.Now = 0; g.Gate3Open = false; g.Threshold = 5.0f; g.TargetsSet = 0;
    Frame wait = { false, 0, std::vector<long>(), std::vector<long>() };
    Frame a = { true, 2, std::vector<long>(1, 1), std::vector<long>() };
    Frame b = { true, 1, std::vector<long>(), std::vector<long>(2, 1) };
    g.Frames.push_back(wait); g.Frames.push_back(a); g.Frames.push_back(b); g.Frames.push_back(a);
    CheckForSecondAreaMassacre(g, 3);
    if (!g.Gate3Open || g.Now != 2 || g.Log.size() != 3 || g.TargetsSet != 4) { std::printf("00d0f640_TEST FAIL massacre\n"); return 1; }
    Sim h = g; h.Now = 0; h.Log.clear(); h.TargetsSet = 0;
    CheckForSecondAreaMassacre(h, 3);
    if (!h.Log.empty() || h.TargetsSet != 0) { std::printf("00d0f640_TEST FAIL gate open\n"); return 1; }
    Sim q = g; q.Gate3Open = false; q.Now = 0; q.Threshold = 100.0f; q.Log.clear();
    CheckForSecondAreaMassacre(q, 3);
    if (q.Gate3Open || !q.Log.empty()) { std::printf("00d0f640_TEST FAIL terminate\n"); return 1; }
    std::printf("00d0f640_TEST PASS\n");
    return 0;
}
