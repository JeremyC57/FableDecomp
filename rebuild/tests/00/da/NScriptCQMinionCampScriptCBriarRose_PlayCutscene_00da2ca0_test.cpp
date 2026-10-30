// Behaviour test for NScript::CQ_MinionCampScript::CBriarRose::PlayCutscene 0x00DA2CA0
#include <cstdio>
#include <map>
#include <string>
static std::string g_log;
static void Log(const std::string& s) { g_log += s; g_log += ';'; }
struct CScriptedThing { std::string Who; };
class CGame { public:
    std::string GetHero() { return "hero"; }
    bool StartScriptingEntity(const std::string& who, CScriptedThing& out, int priority) { out.Who = who; Log("script:" + who); return priority == 4; }
    void StartMovieSequence(const char*) { Log("movie"); }
    void PauseAllNonScriptedEntities(bool p) { Log(p ? "pause" : "unpause"); }
    void FixMovieSequenceCamera(bool f) { Log(f ? "fix" : "unfix"); }
};
static void RunCutsceneMacro(const std::string& name, std::map<std::string, CScriptedThing>& cast)
{
    Log("run:" + name + ":" + cast["HERO"].Who + "," + cast["ROSE"].Who);
}
struct CBriarRose {
    CGame* Game;
    CScriptedThing seh_me;
    void PlayCutscene(std::string name)
    {
        CScriptedThing hero;
        Game->StartScriptingEntity(Game->GetHero(), hero, 4);
        std::map<std::string, CScriptedThing> cast;
        cast["HERO"] = hero;
        cast["ROSE"] = seh_me;
        Game->StartMovieSequence("");
        CGame* game = Game;
        game->PauseAllNonScriptedEntities(true);
        Game->FixMovieSequenceCamera(true);
        RunCutsceneMacro(name, cast);
        Game->FixMovieSequenceCamera(false);
        game->PauseAllNonScriptedEntities(false);
    }
};
int main()
{
    CGame g; CBriarRose r; r.Game = &g; r.seh_me.Who = "rose";
    r.PlayCutscene("CS_MINIONCAMP_ROSE");
    if (g_log != "script:hero;movie;pause;fix;run:CS_MINIONCAMP_ROSE:hero,rose;unfix;unpause;") { std::printf("00da2ca0_TEST FAIL\n"); return 1; }
    std::printf("00da2ca0_TEST PASS\n");
    return 0;
}