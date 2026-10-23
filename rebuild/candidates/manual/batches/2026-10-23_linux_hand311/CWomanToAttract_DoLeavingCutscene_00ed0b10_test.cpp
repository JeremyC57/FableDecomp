// Behaviour test for CWomanToAttract::DoLeavingCutscene (0x00ED0B10): cast HERO and WOMAN, pass $ITEM, run the leave
// cut-scene between fix and unfix of the movie camera.
#include <cstdio>
#include <cstring>

static char g_log[256];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ","); }

struct Handle { int id; };
struct CastMap {
    const char* keys[4]; Handle values[4]; int n;
    CastMap() : n(0) {}
    Handle& operator[](const char* key) { keys[n] = key; values[n].id = 0; return values[n++]; }
};
struct ArgMap {
    const char* key; const char* value;
    ArgMap() : key(0), value(0) {}
    const char*& operator[](const char* k) { key = k; return value; }
};
struct Game {
    void StartScriptingEntity(Handle& h) { h.id = 3; Log("script"); }
    void FixMovieSequenceCamera(bool fix) { Log(fix ? "fix" : "unfix"); }
};
static const char* g_ingredient = "OBJECT_INGREDIENT";
struct Woman {
    Game* game; Handle me; CastMap lastCast; ArgMap lastArgs;
    void Run(const char* name, CastMap& cast, ArgMap* args) { Log(name); lastCast = cast; lastArgs = *args; }
    void DoLeavingCutscene()
    {
        Handle hero;
        game->StartScriptingEntity(hero);
        CastMap cast;
        cast["HERO"] = hero;
        cast["WOMAN"] = me;
        ArgMap args;
        args["$ITEM"] = g_ingredient;
        game->FixMovieSequenceCamera(true);
        Run("CS_SICK_LAUGHING_WOMAN_LEAVES", cast, &args);
        game->FixMovieSequenceCamera(false);
    }
};

int main()
{
    Game game; Woman w; w.game = &game; w.me.id = 9;
    g_log[0] = 0; w.DoLeavingCutscene();
    int ok = std::strcmp(g_log, "script,fix,CS_SICK_LAUGHING_WOMAN_LEAVES,unfix,") == 0;
    ok = ok && w.lastCast.n == 2 && w.lastCast.values[0].id == 3 && std::strcmp(w.lastCast.keys[1], "WOMAN") == 0 && w.lastCast.values[1].id == 9;
    ok = ok && std::strcmp(w.lastArgs.key, "$ITEM") == 0 && w.lastArgs.value == g_ingredient;
    if (!ok) { std::printf("%s 00ed0b10_TEST FAIL\n", g_log); return 1; }
    std::printf("00ed0b10_TEST PASS\n");
    return 0;
}
