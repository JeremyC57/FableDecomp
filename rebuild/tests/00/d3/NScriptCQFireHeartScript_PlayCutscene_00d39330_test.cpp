// Behaviour test for CQ_FireHeartScript::PlayCutscene (0x00D39330): cast HERO, GUEST (the prophet chosen as guest), PROPHET1-5,
// ROSE, SCYTHE and GM, run the named cutscene with the quest's flags and arguments between fix/unfix, then clear the arguments.
#include <cstdio>
#include <cstring>

static char g_log[256];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ","); }

struct Handle { int id; };
struct CastMap {
    const char* keys[16]; Handle values[16]; int n;
    CastMap() : n(0) {}
    Handle& operator[](const char* key) { keys[n] = key; values[n].id = 0; return values[n++]; }
};
struct FireHeart {
    Handle seh_Prophet[5], seh_BriarRose, seh_Scythe, seh_GuildMaster; long Guest; int csargs; CastMap last;
    void Run(const char* name, CastMap& cast) { Log(name); last = cast; }
    void PlayCutscene(const char* name)
    {
        Handle hero; hero.id = 99; Log("script");
        CastMap cast;
        cast["HERO"] = hero; cast["GUEST"] = seh_Prophet[Guest];
        cast["PROPHET1"] = seh_Prophet[0]; cast["PROPHET2"] = seh_Prophet[1]; cast["PROPHET3"] = seh_Prophet[2];
        cast["PROPHET4"] = seh_Prophet[3]; cast["PROPHET5"] = seh_Prophet[4];
        cast["ROSE"] = seh_BriarRose; cast["SCYTHE"] = seh_Scythe; cast["GM"] = seh_GuildMaster;
        Log("movie"); Log("pause"); Log("fix");
        Run(name, cast);
        Log("unfix"); csargs = 0; Log("unpause");
    }
};

int main()
{
    FireHeart f; for (int i = 0; i < 5; ++i) f.seh_Prophet[i].id = 10 + i;
    f.seh_BriarRose.id = 20; f.seh_Scythe.id = 21; f.seh_GuildMaster.id = 22; f.Guest = 3; f.csargs = 5;
    g_log[0] = 0; f.PlayCutscene("CS_FIREHEART_INTRO");
    bool ok = std::strcmp(g_log, "script,movie,pause,fix,CS_FIREHEART_INTRO,unfix,unpause,") == 0 && f.csargs == 0;
    ok = ok && f.last.n == 10 && std::strcmp(f.last.keys[1], "GUEST") == 0 && f.last.values[1].id == 13;
    ok = ok && std::strcmp(f.last.keys[9], "GM") == 0 && f.last.values[9].id == 22 && f.last.values[0].id == 99;
    if (!ok) { std::printf("%s 00d39330_TEST FAIL\n", g_log); return 1; }
    std::printf("00d39330_TEST PASS\n");
    return 0;
}