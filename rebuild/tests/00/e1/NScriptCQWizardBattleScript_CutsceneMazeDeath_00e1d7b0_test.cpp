// Behaviour test for NScript::CQ_WizardBattleScript::CutsceneMazeDeath (0x00E1D7B0): the hero is put under scripted control,
// cast as Hero, the movie sequence starts with the pause on, CS_WIZARD_BATTLE_MAZE_DEATH runs with no flag/argument maps, the
// active quest is marked completed (true, false, false) and the pause is lifted.
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
    gSeenName = name; gSeenFlags = flags; gSeenArgs = args; gSeenHero = cast["Hero"].thingId;
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
    const char* GetActiveQuestName() const { return "Q_WizardBattle"; }
    void SetQuestAsCompleted(const char* name, bool a, bool b, bool c) { if (std::strcmp(name, "Q_WizardBattle") == 0 && a && !b && !c) Log("Completed"); }
};

struct Script {
    Game* game;
    ArgMap csargs;
    void CutsceneMazeDeath()
    {
        Handle hero;
        game->StartScriptingEntity(game->GetHero(), hero, 4);
        CastMap cast;
        cast["Hero"] = hero;
        game->StartMovieSequence("");
        Game* g = game;
        g->PauseAllNonScriptedEntities(true);
        RunCutsceneMacro("CS_WIZARD_BATTLE_MAZE_DEATH", cast, 0, 0, false, true);
        game->SetQuestAsCompleted(game->GetActiveQuestName(), true, false, false);
        g->PauseAllNonScriptedEntities(false);
    }
};

int main()
{
    Game g; g.hero.id = 42;
    Script m; m.game = &g; m.csargs.marker = 7;
    gLog[0] = 0;
    m.CutsceneMazeDeath();
    if (std::strcmp(gLog, "GetHero;Start4;Movie;Pause;Run;Completed;Unpause;") != 0 || gSeenHero != 42 || gSeenArgs != 0
        || gSeenFlags != 0 || std::strcmp(gSeenName, "CS_WIZARD_BATTLE_MAZE_DEATH") != 0) {
        std::printf("00e1d7b0_TEST FAIL\n");
        return 1;
    }
    std::printf("00e1d7b0_TEST PASS\n");
    return 0;
}