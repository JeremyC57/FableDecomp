// Behaviour test for NScript::CExpression_FollowScript::Main (0x00EEA020): the thread predicate is set on the hero, one frame
// passes, the hero is taken under scripted control (retrying each frame), then a movie sequence opens, the screen fades out to
// black over 0.5s, the FollowThread thread is spawned, the camera reset and the screen faded back in; termination stops it early.
#include <cstdio>
#include <cstring>

static char gLog[512];
static void Log(const char* s) { std::strcat(gLog, s); std::strcat(gLog, ";"); }

struct Thing { int id; };
struct Handle { int thingId; Handle() : thingId(0) { } };
struct Colour { unsigned char B, G, R, A; Colour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; } };

struct Game {
    Thing hero;
    int startFailures;
    Thing& GetHero() { return hero; }
    void NewScriptFrame() { Log("Frame"); }
    bool StartScriptingEntity(const Thing& t, Handle& h, int priority)
    {
        if (startFailures > 0) { --startFailures; return false; }
        h.thingId = t.id;
        if (priority == 4) Log("Scripted");
        return true;
    }
    void StartMovieSequence(const char* name) { if (name[0] == 0) Log("Movie"); }
    void FadeScreenOut(float time, float hold, Colour c) { if (time == 0.5f && hold == 0.5f && c.R == 0 && c.G == 0 && c.B == 0 && c.A == 255) Log("FadeOut"); }
    void Pause(float t) { if (t == 1.0f) Log("Pause1"); }
    void CameraDefault() { Log("Camera"); }
    void FadeScreenIn() { Log("FadeIn"); }
};

struct Script {
    Game* game;
    int termBudget;
    int predicateThing;
    const char* spawned;
    bool IsActiveThreadTerminating() { if (termBudget <= 0) return true; --termBudget; return false; }
    void SetPredicate(const Thing& t) { predicateThing = t.id; Log("Predicate"); }
    void AddSpawnedFunction(const char* name, const char* region) { if (region[0] == 0) { spawned = name; Log("Spawn"); } }
    void Main()
    {
        SetPredicate(game->GetHero());
        game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        Handle hero;
        while (!game->StartScriptingEntity(game->GetHero(), hero, 4)) {
            game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        game->StartMovieSequence("");
        game->FadeScreenOut(0.5f, 0.5f, Colour(0, 0, 0, 255));
        game->Pause(1.0f);
        AddSpawnedFunction("FollowThread", "");
        game->CameraDefault();
        game->Pause(1.0f);
        game->FadeScreenIn();
    }
};

int main()
{
    Game g; g.hero.id = 9; g.startFailures = 2;
    Script s; s.game = &g; s.termBudget = 100; s.spawned = 0; s.predicateThing = 0;
    gLog[0] = 0;
    s.Main();
    if (std::strcmp(gLog, "Predicate;Frame;Frame;Frame;Scripted;Movie;FadeOut;Pause1;Spawn;Camera;Pause1;FadeIn;") != 0
        || s.predicateThing != 9 || s.spawned == 0 || std::strcmp(s.spawned, "FollowThread") != 0) {
        std::printf("00eea020_TEST FAIL\n"); return 1;
    }
    gLog[0] = 0; g.startFailures = 5; s.termBudget = 2; s.spawned = 0;
    s.Main();
    if (s.spawned != 0 || std::strstr(gLog, "Movie") != 0) { std::printf("00eea020_TEST FAIL\n"); return 1; }
    std::printf("00eea020_TEST PASS\n");
    return 0;
}