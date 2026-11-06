// Behaviour test for CQ_FireHeartScript::Main (0x00D35090), closing part: on success give the Fire Heart back with the
// confiscated items; then the demon-door exit cutscene depends on the Jack boss result, and the prisoner reward on whether
// all prisoners were killed (checked first) or all freed.
#include <cstdio>
#include <cstring>

static char g_log[512];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ","); }

struct FireHeart {
    bool MissionSucceeded, KilledAllPrisoners, FreedAllPrisoners; long JackBossBattleResult;
    void PlayCutscene(const char* name) { Log(name); }
    void Finish()
    {
        if (MissionSucceeded) { Log("give"); Log("return"); }
        Log("region");
        if (JackBossBattleResult == 1) PlayCutscene("HAVESWORD"); else PlayCutscene("NOSWORD");
        if (KilledAllPrisoners) Log("killreward");
        else if (FreedAllPrisoners) Log("freereward");
        Log("complete");
    }
};

static bool Run(bool ok, bool kill, bool freed, long jack, const char* want)
{
    FireHeart f; f.MissionSucceeded = ok; f.KilledAllPrisoners = kill; f.FreedAllPrisoners = freed; f.JackBossBattleResult = jack;
    g_log[0] = 0; f.Finish();
    return std::strcmp(g_log, want) == 0;
}

int main()
{
    bool ok = Run(true, false, true, 1, "give,return,region,HAVESWORD,freereward,complete,");
    ok = ok && Run(false, true, true, 0, "region,NOSWORD,killreward,complete,");
    ok = ok && Run(true, false, false, 2, "give,return,region,NOSWORD,complete,");
    if (!ok) { std::printf("%s 00d35090_TEST FAIL\n", g_log); return 1; }
    std::printf("00d35090_TEST PASS\n");
    return 0;
}