// Behaviour test for 0x00CFF9A0 NScript::CQ_AwakeningTheOracleScript::PlayCutscene: the body over a recording script
// interface, cut-scene runner and maps. Checks the call order of both branches: hero scripted and cast as HERO, the
// camera fixed, the cut-scene run with cs_chars / cs_args (inside a started movie sequence with entities paused when
// inMovieSequence), the camera released, cs_args then cs_chars cleared.
#include <cstdio>
#include <cstring>
static char g_log[256];
static void logs(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
class CCharString { public: const char* text; CCharString(const char* t) : text(t) {} };
class CScriptThing { public: int id; };
class CScriptGameResourceObjectScriptedThingBase { public: int who; CScriptGameResourceObjectScriptedThingBase() : who(0) {} };
class CScriptGameResourceObjectMovieBase { public: int id; CScriptGameResourceObjectMovieBase() : id(0) {} };
struct CastMap { CScriptGameResourceObjectScriptedThingBase hero; const char* key;
    CScriptGameResourceObjectScriptedThingBase& operator[](const CCharString& k) { key = k.text; logs("cast[]"); return hero; }
    void clear() { logs("cast.clear"); } };
struct ArgMap { void clear() { logs("args.clear"); } };
struct Game {
    CScriptThing GetHero() { logs("GetHero"); CScriptThing t; t.id = 7; return t; }
    void StartScriptingEntity(const CScriptThing& t, CScriptGameResourceObjectScriptedThingBase& o, int prio)
    { if (t.id == 7 && prio == 4) { o.who = 7; logs("Start"); } }
    void FixMovieSequenceCamera(bool on) { logs(on ? "Fix1" : "Fix0"); }
    void StartMovieSequence(const CCharString& s, CScriptGameResourceObjectMovieBase&) { logs(s.text[0] == 0 ? "Movie" : "Movie?"); }
    void PauseAllNonScriptedEntities(bool on) { logs(on ? "Pause1" : "Pause0"); }
};
static void RunCutsceneMacro(const CCharString& name, CastMap& cast, void* flags, ArgMap* args, bool a5, bool a6)
{ if (cast.hero.who == 7 && std::strcmp(cast.key, "HERO") == 0 && flags == 0 && args != 0 && !a5 && a6) logs(name.text); }
struct CQ_AwakeningTheOracleScript {
    Game* m_game; ArgMap cs_args; CastMap cs_chars;
    void PlayCutscene(CCharString name, bool inMovieSequence)
    {
        Game* Game = m_game;
        CScriptGameResourceObjectScriptedThingBase hero;
        Game->StartScriptingEntity(Game->GetHero(), hero, 4);
        cs_chars["HERO"] = hero;
        Game->FixMovieSequenceCamera(true);
        if (inMovieSequence) {
            CScriptGameResourceObjectMovieBase movie;
            Game->StartMovieSequence("", movie);
            struct Game* game = Game;
            game->PauseAllNonScriptedEntities(true);
            RunCutsceneMacro(name, cs_chars, 0, &cs_args, false, true);
            game->PauseAllNonScriptedEntities(false);
        } else
            RunCutsceneMacro(name, cs_chars, 0, &cs_args, false, true);
        Game->FixMovieSequenceCamera(false);
        cs_args.clear();
        cs_chars.clear();
    }
};
int main()
{
    int fails = 0;
    Game g; CQ_AwakeningTheOracleScript s; s.m_game = &g;
    g_log[0] = 0; s.PlayCutscene("CS1", false);
    if (std::strcmp(g_log, "GetHero;Start;cast[];Fix1;CS1;Fix0;args.clear;cast.clear;") != 0) { fails |= 1; std::printf("%s\n", g_log); }
    g_log[0] = 0; s.PlayCutscene("CS2", true);
    if (std::strcmp(g_log, "GetHero;Start;cast[];Fix1;Movie;Pause1;CS2;Pause0;Fix0;args.clear;cast.clear;") != 0) { fails |= 2; std::printf("%s\n", g_log); }
    if (fails) { std::printf("00cff9a0_TEST FAIL %d\n", fails); return 1; }
    std::printf("00cff9a0_TEST PASS\n");
    return 0;
}