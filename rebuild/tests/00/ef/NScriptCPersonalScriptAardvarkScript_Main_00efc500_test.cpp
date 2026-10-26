// Behaviour test for NScript::CPersonalScript_AardvarkScript::Main (0x00EFC500): the demon is looked up as "DEMON" and
// taken under scripted control (retrying a frame at a time); a live demon speaks "Smack my bitch up" to the hero and the
// script waits for it; the world is paused around the movie and unpaused on every exit after it started; every exit
// destroys the handle and the demon exactly once and the movie at most once; Release runs only on the normal path.
#include <cstdio>
#include <string>

static std::string g_log;
class CScriptThing { public: int id; };
struct CScriptThingLocal {
    CScriptThing thing;
    int destroyed;
    const CScriptThing& Thing() const { return thing; }
    void Destroy() { ++destroyed; g_log += "~demon "; }
    void DestroyInline() { ++destroyed; g_log += "~demon "; }
};
struct CHandle {
    int destroyed, released, speakFrames;
    float health;
    CHandle() : destroyed(0), released(0), speakFrames(0), health(0) { }
    void Release() { ++released; g_log += "release "; }
    void Speak(const CScriptThing& listener, const char* text, bool, bool, bool c, bool)
    {
        g_log += "speak(" + std::string(text) + (listener.id == 7 && c ? ",hero) " : ",?) ");
        speakFrames = 2;
    }
    bool IsPerformingScriptTask() const { return speakFrames > 0; }
    CScriptThing GetScriptThing() const { CScriptThing t; t.id = 99; return t; }
    void Destroy() { ++destroyed; g_log += "~handle "; }
    void DestroyInline() { ++destroyed; g_log += "~handle "; }
};
struct CMovie {
    void DestroyInline() { g_log += "~movie "; }
    void DestroyOutOfLine() { g_log += "~movie "; }
};
class CGameScriptInterface { public:
    int startFailures;
    CScriptThing hero;
    CHandle* lastHandle;
    float health;
    void NewScriptFrame() const { g_log += "frame "; }
    bool StartScriptingEntity(const CScriptThing& thing, CHandle& handle, int priority)
    {
        lastHandle = &handle;
        handle.health = health;
        if (thing.id != 5 || priority != 4)
            return false;
        if (startFailures > 0) { --startFailures; return false; }
        g_log += "start ";
        return true;
    }
    CScriptThing& GetHero() { return hero; }
    CScriptThingLocal GetThingWithScriptName(const std::string& name) const
    {
        CScriptThingLocal r; r.thing.id = name == "DEMON" ? 5 : 0; r.destroyed = 0; return r;
    }
    float GetHealth(const CScriptThing& thing) const { return thing.id == 99 ? lastHandle->health : -1.0f; }
    void StartMovieSequence(const std::string&, CMovie&) { g_log += "movie "; }
    void Pause(float seconds) { char b[32]; sprintf(b, "pause%g ", seconds); g_log += b; }
    void PauseAllNonScriptedEntities(bool pause) { g_log += pause ? "freeze " : "thaw "; }
};
class CScriptBase { public:
    CGameScriptInterface* GSI;
    int calls, stopAt;
    CHandle** speaker;
    bool IsActiveThreadTerminating()
    {
        if (*speaker && (*speaker)->speakFrames > 0) --(*speaker)->speakFrames;
        return ++calls >= stopAt;
    }
};
class CScriptGameResourceObjectScriptedThingBase : public CHandle { public: };
class CScriptGameResourceObjectMovieBase : public CMovie { public: };

namespace NScript {
class CPersonalScript_AardvarkScript : public CScriptBase { public:
    void Main();
};
void CPersonalScript_AardvarkScript::Main()
{
    CScriptThingLocal demon = GSI->GetThingWithScriptName("DEMON");
    CScriptGameResourceObjectScriptedThingBase handle;
    *speaker = &handle;
    while (!GSI->StartScriptingEntity(demon.Thing(), handle, 4)) {
        GSI->NewScriptFrame();
        if (IsActiveThreadTerminating()) {
            handle.DestroyInline();
            demon.DestroyInline();
            return;
        }
    }
    if (IsActiveThreadTerminating()) {
        handle.Destroy();
        demon.Destroy();
        return;
    }
    {
        CScriptGameResourceObjectMovieBase movie;
        GSI->StartMovieSequence("", movie);
        CGameScriptInterface* game = GSI;
        game->PauseAllNonScriptedEntities(true);
        GSI->Pause(1.0f);
        if (GSI->GetHealth(handle.GetScriptThing()) > 0.0f) {
            handle.Speak(GSI->GetHero(), "Smack my bitch up", false, false, true, false);
            while (handle.IsPerformingScriptTask()) {
                GSI->NewScriptFrame();
                if (IsActiveThreadTerminating()) {
                    game->PauseAllNonScriptedEntities(false);
                    movie.DestroyInline();
                    goto done;
                }
            }
            if (IsActiveThreadTerminating()) {
                game->PauseAllNonScriptedEntities(false);
                movie.DestroyOutOfLine();
                goto done;
            }
        }
        GSI->Pause(2.0f);
        game->PauseAllNonScriptedEntities(false);
        movie.DestroyInline();
        handle.Release();
    }
done:
    handle.Destroy();
    demon.Destroy();
}
}

static std::string Run(int failures, float health, int stopAt)
{
    g_log = "";
    CGameScriptInterface game;
    game.startFailures = failures;
    game.hero.id = 7;
    game.lastHandle = 0;
    game.health = health;
    CHandle* speaker = 0;
    NScript::CPersonalScript_AardvarkScript script;
    script.GSI = &game;
    script.calls = 0;
    script.stopAt = stopAt;
    script.speaker = &speaker;
    script.Main();
    return g_log;
}

int main()
{
    int ok = 1;
    // Live demon, never terminated: speaks, waits two frames, then the 2-second pause and the clean-up.
    ok &= Run(0, 10.0f, 100) ==
        "start movie freeze pause1 speak(Smack my bitch up,hero) frame frame pause2 thaw ~movie release ~handle ~demon ";
    // Dead demon: no speech.
    ok &= Run(0, 0.0f, 100) == "start movie freeze pause1 pause2 thaw ~movie release ~handle ~demon ";
    // Start fails once, then the thread stops inside the wait loop.
    ok &= Run(3, 1.0f, 1) == "frame ~handle ~demon ";
    // Started, but the thread stops before the movie.
    ok &= Run(0, 1.0f, 1) == "start ~handle ~demon ";
    // Stops while the demon is speaking: thaw and end the movie, no Release.
    ok &= Run(0, 1.0f, 2) == "start movie freeze pause1 speak(Smack my bitch up,hero) frame thaw ~movie ~handle ~demon ";
    // Stops right after the speech ended.
    ok &= Run(0, 1.0f, 4) ==
        "start movie freeze pause1 speak(Smack my bitch up,hero) frame frame thaw ~movie ~handle ~demon ";
    printf("00efc500_TEST %s\n", ok ? "PASS" : "FAIL");
    return ok ? 0 : 1;
}