// Behaviour test for NScript::CQ_HeroSoulsMotherScript::PlayCutscene (0x00D848D0): the hero is put under scripted control,
// cast as HERO, the movie sequence starts with the pause on, the camera is fixed around the cut-scene run (with the quest's
// csargs and no flags map), then released and the pause lifted.
#include <cstdio>
#include <cstring>

static char gLog[512];
static void Log(const char* s) { std::strcat(gLog, s); std::strcat(gLog, ";"); }

struct Thing { int id; };
struct Handle { int thingId; Handle() : thingId(0) { } };
struct ArgMap { int marker; };

struct CastEntry { const char* key; Handle value; };
struct CastMap {
    CastEntry entries[4];
    int n;
    CastMap() : n(0) { }
    Handle& operator[](const char* key)
    {
        for (int i = 0; i < n; ++i)
            if (std::strcmp(entries[i].key, key) == 0)
                return entries[i].value;
        entries[n].key = key;
        return entries[n++].value;
    }
};

static const ArgMap* gSeenArgs;
static const void* gSeenFlags;
static int gSeenHero;
static const char* gSeenName;
static void RunCutsceneMacro(const char* name, CastMap& cast, const void* flags, const ArgMap* args, bool a5, bool a6)
{
    gSeenName = name; gSeenFlags = flags; gSeenArgs = args; gSeenHero = cast["HERO"].thingId;
    if (!a5 && a6)
        Log("Run");
}

struct Game {
    Thing hero;
    Thing& GetHero() { Log("GetHero"); return hero; }
    bool StartScriptingEntity(const Thing& t, Handle& h, int priority) { h.thingId = t.id; if (priority == 4) Log("Start4"); return true; }
    void StartMovieSequence(const char* name) { if (name[0] == 0) Log("Movie"); }
    void PauseAllNonScriptedEntities(bool p) { Log(p ? "Pause" : "Unpause"); }
    void FixMovieSequenceCamera(bool f) { Log(f ? "Fix" : "Unfix"); }
};

struct Mother {
    Game* game;
    ArgMap csargs;
    void PlayCutscene(const char* name)
    {
        Handle hero;
        game->StartScriptingEntity(game->GetHero(), hero, 4);
        CastMap cast;
        cast["HERO"] = hero;
        game->StartMovieSequence("");
        Game* g = game;
        g->PauseAllNonScriptedEntities(true);
        game->FixMovieSequenceCamera(true);
        RunCutsceneMacro(name, cast, 0, &csargs, false, true);
        game->FixMovieSequenceCamera(false);
        g->PauseAllNonScriptedEntities(false);
    }
};

int main()
{
    Game g; g.hero.id = 42;
    Mother m; m.game = &g; m.csargs.marker = 7;
    gLog[0] = 0;
    m.PlayCutscene("CS_HEROSOULS_SOUL2_MOTHER_START");
    if (std::strcmp(gLog, "GetHero;Start4;Movie;Pause;Fix;Run;Unfix;Unpause;") != 0 || gSeenHero != 42 || gSeenArgs != &m.csargs
        || gSeenFlags != 0 || std::strcmp(gSeenName, "CS_HEROSOULS_SOUL2_MOTHER_START") != 0) {
        std::printf("00d848d0_TEST FAIL\n");
        return 1;
    }
    std::printf("00d848d0_TEST PASS\n");
    return 0;
}