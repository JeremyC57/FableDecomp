// Behaviour test for 0x00D27050 NScript::CQ_DragonBossFightScript::PlayCutscene: script the hero (priority 4) into the cast
// as "HERO", start an unnamed movie sequence, pause the non-scripted entities and fix the movie camera around the cutscene
// macro (run with the script's cs_flags map), then unfix the camera and unpause.
#include <stdio.h>
#include <string.h>
#include <map>
#include <string>

static char g_log[512];
static void Log(const char* s) { strcat(g_log, s); strcat(g_log, ";"); }
struct CHandle { int Id; CHandle() : Id(0) { } };
class CGame { public:
    int GetHero() { return 7; }
    void StartScriptingEntity(int thing, CHandle& h, int priority) { h.Id = thing; if (priority != 4) Log("PriorityBad"); Log("Script"); }
    void StartMovieSequence(const char* name, CHandle& m) { m.Id = 99; Log(name[0] ? "NamedMovie" : "Movie"); }
    void PauseAllNonScriptedEntities(bool b) { Log(b ? "Pause" : "Unpause"); }
    void FixMovieSequenceCamera(bool b) { Log(b ? "Fix" : "Unfix"); }
};
struct CDragonBossFight {
    CGame* Game;
    std::map<std::string, bool> cs_flags;
    void RunCutsceneMacro(const std::string& name, std::map<std::string, CHandle>& cast, std::map<std::string, bool>* flags,
                          int a, bool b, bool c)
    {
        Log(name.c_str());
        if (cast["HERO"].Id != 7 || flags != &cs_flags || a != 0 || b || !c) Log("ArgsBad");
        (*flags)[name] = true;
    }
    void PlayCutscene(std::string name)
    {
        CHandle hero;
        Game->StartScriptingEntity(Game->GetHero(), hero, 4);
        std::map<std::string, CHandle> cast;
        cast["HERO"] = hero;
        CHandle movie;
        Game->StartMovieSequence("", movie);
        CGame* game = Game;
        game->PauseAllNonScriptedEntities(true);
        Game->FixMovieSequenceCamera(true);
        RunCutsceneMacro(name, cast, &cs_flags, 0, false, true);
        Game->FixMovieSequenceCamera(false);
        game->PauseAllNonScriptedEntities(false);
    }
};

int main()
{
    CGame g; CDragonBossFight s; s.Game = &g;
    g_log[0] = 0;
    s.PlayCutscene("CS_DRAGON_INTRO");
    int ok = !strcmp(g_log, "Script;Movie;Pause;Fix;CS_DRAGON_INTRO;Unfix;Unpause;") && s.cs_flags["CS_DRAGON_INTRO"];
    if (!ok) { printf("00d27050_TEST FAIL\n"); return 1; }
    printf("00d27050_TEST PASS\n");
    return 0;
}