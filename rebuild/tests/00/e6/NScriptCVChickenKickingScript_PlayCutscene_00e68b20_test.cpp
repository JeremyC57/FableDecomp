// Behaviour test for NScript::CV_ChickenKickingScript::PlayCutscene 0x00E68B20
#include <cstdio>
#include <map>
#include <string>
static std::string g_log;
static void Log(const std::string& s) { g_log += s; g_log += ';'; }
struct CScriptedThing { std::string Who; };
class CGame { public:
    std::string GetHero() { return "hero"; }
    bool StartScriptingEntity(const std::string& who, CScriptedThing& out, int) { out.Who = who; Log("script:" + who); return true; }
    void FixMovieSequenceCamera(bool f) { Log(f ? "fix" : "unfix"); }
};
static void RunCutsceneMacro(const std::string& name, std::map<std::string, CScriptedThing>& cast, std::map<std::string, std::string>* args)
{
    Log("run:" + name + ":" + cast["HERO"].Who + "," + cast["CHICKEN"].Who + "," + cast["ORGANISER"].Who + ":" + (*args)["PRIZE"]);
}
struct CV_ChickenKickingScript {
    CGame* Game;
    std::map<std::string, std::string> csargs;
    CScriptedThing seh_Chicken;
    CScriptedThing seh_ChickenMaster;
    void PlayCutscene(std::string name)
    {
        CScriptedThing hero;
        Game->StartScriptingEntity(Game->GetHero(), hero, 4);
        std::map<std::string, CScriptedThing> cast;
        cast["HERO"] = hero;
        cast["CHICKEN"] = seh_Chicken;
        cast["ORGANISER"] = seh_ChickenMaster;
        Game->FixMovieSequenceCamera(true);
        RunCutsceneMacro(name, cast, &csargs);
        Game->FixMovieSequenceCamera(false);
        csargs.clear();
    }
};
int main()
{
    CGame g; CV_ChickenKickingScript s; s.Game = &g;
    s.seh_Chicken.Who = "chicken"; s.seh_ChickenMaster.Who = "master"; s.csargs["PRIZE"] = "100";
    s.PlayCutscene("CS_CHICKEN_KICK");
    if (g_log != "script:hero;fix;run:CS_CHICKEN_KICK:hero,chicken,master:100;unfix;" || !s.csargs.empty()) { std::printf("00e68b20_TEST FAIL\n"); return 1; }
    std::printf("00e68b20_TEST PASS\n");
    return 0;
}