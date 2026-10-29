// Behaviour test for NScript::CQS_MeetSisterScript::CMeetSisterSister::Main 0x00E295E0: the scripting wait loop with its two
// termination exits, the Hero/Sister cast, the cut-scene inside a paused movie sequence and the quest wrap-up, in order.
#include <cstdio>
#include <cstring>
static char g_log[1024];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
struct Thing { int id; };
struct ScriptedObject { int thing; ScriptedObject() : thing(0) { } bool IsScripting() const { return thing != 0; } void Release() { thing = 0; Log("Release"); } };
struct CastEntry { const char* name; int thing; };
struct Game {
    int startFailures;
    Thing hero;
    Game() : startFailures(0) { hero.id = 7; }
    bool StartScriptingEntity(const Thing& t, ScriptedObject& o, int priority)
    {
        if (priority != 4) Log("BadPriority");
        if (t.id == hero.id) { o.thing = t.id; Log("StartHero"); return true; }
        Log("Start");
        if (startFailures > 0) { --startFailures; return false; }
        o.thing = t.id;
        return true;
    }
    void NewScriptFrame() { Log("Frame"); }
    Thing& GetHero() { return hero; }
    void StartMovieSequence(const char* name) { if (name[0] == 0) Log("Movie"); }
    void PauseAllNonScriptedEntities(bool pause) { Log(pause ? "Pause1" : "Pause0"); }
    void RunCutsceneMacro(const char* name, const CastEntry* cast, int count, bool a5, bool a6)
    {
        char b[128];
        std::sprintf(b, "Run(%s,%s=%d,%s=%d,%d,%d)", name, cast[0].name, cast[0].thing, cast[1].name, cast[1].thing, a5, a6);
        if (count == 2) Log(b);
    }
    void RemoveThing(const Thing& t, bool a2, bool a3) { char b[32]; std::sprintf(b, "Remove(%d,%d,%d)", t.id, a2, a3); Log(b); }
    const char* GetActiveQuestName() const { return "Q_ARENA"; }
    void SetQuestAsCompleted(const char* q, bool a, bool b, bool c) { if (!a && !b && !c) { Log("Complete"); Log(q); } }
    void DeactivateQuestLater(const char* q, unsigned int frames) { if (frames == 0) { Log("Deactivate"); Log(q); } }
    void FadeScreenIn() { Log("FadeIn"); }
};
struct SisterScript {
    Game* game;
    Thing me;
    int terminateAt;    // IsActiveThreadTerminating returns true on this call number (0 = never)
    int calls;
    bool IsActiveThreadTerminating() { ++calls; return calls == terminateAt; }
    void Main()
    {
        ScriptedObject sister;
        {
            ScriptedObject hero;
            if (sister.IsScripting())
                sister.Release();
            while (!game->StartScriptingEntity(me, sister, 4)) {
                game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
            }
            if (IsActiveThreadTerminating())
                return;
            game->StartScriptingEntity(game->GetHero(), hero, 4);
            CastEntry cast[2];
            cast[0].name = "Hero"; cast[0].thing = hero.thing;
            cast[1].name = "Sister"; cast[1].thing = sister.thing;
            game->StartMovieSequence("");
            Game* g = game;
            g->PauseAllNonScriptedEntities(true);
            game->RunCutsceneMacro("CS_ARENA_LEAVE_THERESA_02", cast, 2, false, true);
            g->PauseAllNonScriptedEntities(false);
        }
        game->RemoveThing(me, false, true);
        game->SetQuestAsCompleted(game->GetActiveQuestName(), false, false, false);
        game->DeactivateQuestLater(game->GetActiveQuestName(), 0);
        game->FadeScreenIn();
    }
};
static bool Run(int failures, int terminateAt, const char* expect)
{
    g_log[0] = 0;
    Game game; game.startFailures = failures;
    SisterScript s; s.game = &game; s.me.id = 3; s.terminateAt = terminateAt; s.calls = 0;
    s.Main();
    if (std::strcmp(g_log, expect) != 0) { std::printf("got %s\n", g_log); return false; }
    return true;
}
int main()
{
    if (!Run(2, 0, "Start;Frame;Start;Frame;Start;StartHero;Movie;Pause1;Run(CS_ARENA_LEAVE_THERESA_02,Hero=7,Sister=3,0,1);"
                   "Pause0;Remove(3,0,1);Complete;Q_ARENA;Deactivate;Q_ARENA;FadeIn;") ||
        !Run(5, 2, "Start;Frame;Start;Frame;") ||          // terminated inside the wait loop
        !Run(0, 1, "Start;")) {                            // terminated right after scripting started
        std::printf("00e295e0_TEST FAIL\n");
        return 1;
    }
    std::printf("00e295e0_TEST PASS\n");
    return 0;
}