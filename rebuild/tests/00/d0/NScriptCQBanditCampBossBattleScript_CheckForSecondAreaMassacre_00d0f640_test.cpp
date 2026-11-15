// Behaviour test for NScript::CQ_BanditCampBossBattleScript::CheckForSecondAreaMassacre (0x00D0F640): a state model of the
// spawned thread. Each frame (stop on thread termination, checked before every step that can follow a frame or a kill count):
// wait while BanditCampResidential is not loaded; stop (after one more termination check) once Gate3Open is set elsewhere; add the
// kills of the hero, then of every follower, then of every summoned creature (one list reused, a termination check per entry and per
// non-empty kill list); at the inner-area quota (float compare) play the alarm cut scene (movie, pause, script the hero, fade, pause,
// CS_BANDITCAMP_ALARM_INNER, remove the gate-3 guard), open gate 3, set every defensive bandit on the hero (termination check each),
// create the level-2 leader at DefensiveGuardLeader and set him on the hero, and end the thread.
#include <cstdio>
#include <cstring>

static char g_log[1024];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ","); }

struct Script {
    int Frame, TerminateAt, LoadedAt;
    bool Gate3Open; int Gate3OpenAt;
    long HeroKills[8], FollowerKills[8], SummonKills[8];   // per frame
    int Followers, Summons, Bandits;
    float Quota;
    int Checks;
    bool T() { ++Checks; return Frame >= TerminateAt; }
    void NSF() { ++Frame; if (Frame == Gate3OpenAt) Gate3Open = true; }
    bool Loaded() const { return Frame >= LoadedAt; }
    void Run()
    {
        long killed = 0;
        for (; !false; NSF()) {
            if (T()) return;
            if (!Loaded()) {
                if (T()) return;
                while (!Loaded()) {
                    NSF();
                    if (T()) return;
                }
                if (T()) return;
            }
            if (Gate3Open) break;
            if (HeroKills[Frame & 7]) {
                if (T()) return;
                killed += HeroKills[Frame & 7];
            }
            for (int i = 0; i < Followers; i++) {
                if (T()) return;
                if (FollowerKills[Frame & 7]) {
                    if (T()) return;
                    killed += FollowerKills[Frame & 7];
                }
            }
            if (T()) return;
            for (int i = 0; i < Summons; i++) {
                if (T()) return;
                if (SummonKills[Frame & 7]) {
                    if (T()) return;
                    killed += SummonKills[Frame & 7];
                }
            }
            if (T()) return;
            if ((float)killed >= Quota) {
                if (T()) return;
                Log("movie"); Log("pause_entities"); Log("script_hero"); Log("fade"); Log("pause");
                Log("CS_BANDITCAMP_ALARM_INNER"); Log("remove_Gate3Guard");
                Gate3Open = true;
                unsigned long i = 0;
                while (i < (unsigned long)Bandits) {
                    if (T()) return;
                    Log("target_bandit");
                    i++;
                }
                if (T()) return;
                Log("create_leader"); Log("target_leader");
                return;
            }
        }
        if (T()) return;
    }
};

static Script Make(int terminateAt)
{
    Script s; std::memset(&s, 0, sizeof s);
    s.TerminateAt = terminateAt; s.Gate3OpenAt = -1; s.Quota = 10.0f; g_log[0] = 0;
    return s;
}

int main()
{
    bool ok = true;
    // hero 2 kills a frame, two followers 1 each, one summon 1: 5 a frame -> quota 10 reached on the second frame
    Script a = Make(100);
    for (int k = 0; k < 8; k++) { a.HeroKills[k] = 2; a.FollowerKills[k] = 1; a.SummonKills[k] = 1; }
    a.Followers = 2; a.Summons = 1; a.Bandits = 3; a.Run();
    ok = ok && std::strcmp(g_log, "movie,pause_entities,script_hero,fade,pause,CS_BANDITCAMP_ALARM_INNER,remove_Gate3Guard,"
                                  "target_bandit,target_bandit,target_bandit,create_leader,target_leader,") == 0
            && a.Gate3Open && a.Frame == 1;
    // level not loaded until frame 3, quota 1: waits, then fires on frame 3
    Script b = Make(100); b.LoadedAt = 3; b.Quota = 1.0f; b.HeroKills[3] = 1; b.Bandits = 0; b.Run();
    ok = ok && std::strcmp(g_log, "movie,pause_entities,script_hero,fade,pause,CS_BANDITCAMP_ALARM_INNER,remove_Gate3Guard,"
                                  "create_leader,target_leader,") == 0 && b.Frame == 3;
    // gate 3 opened elsewhere on frame 2 before the quota: the thread stops without the cut scene
    Script c = Make(100); c.Gate3OpenAt = 2; c.HeroKills[0] = 1; c.Run();
    ok = ok && g_log[0] == 0 && c.Frame == 2;
    // terminated on frame 4 while counting below the quota
    Script d = Make(4); d.Followers = 3; d.FollowerKills[1] = 1; d.Run();
    ok = ok && g_log[0] == 0 && d.Frame == 4;
    // quota 0 reached on the first frame with nothing killed: five bandits targeted, then the leader
    Script e = Make(100); e.Quota = 0.0f; e.Bandits = 5; e.Run();
    ok = ok && std::strcmp(g_log, "movie,pause_entities,script_hero,fade,pause,CS_BANDITCAMP_ALARM_INNER,remove_Gate3Guard,"
                                  "target_bandit,target_bandit,target_bandit,target_bandit,target_bandit,create_leader,target_leader,") == 0
            && e.Frame == 0;
    if (!ok) { std::printf("%s 00d0f640_TEST FAIL\n", g_log); return 1; }
    std::printf("00d0f640_TEST PASS\n");
    return 0;
}