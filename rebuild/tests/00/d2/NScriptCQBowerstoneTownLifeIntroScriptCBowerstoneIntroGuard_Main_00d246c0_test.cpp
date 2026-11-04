#include <cstdio>
#include <cstring>
static char g_log[4096];
static int g_frame = 0;
static int g_termAt = 1000;
static void Log(const char* s) { char b[32]; std::sprintf(b, "@%d;", g_frame); std::strcat(g_log, s); std::strcat(g_log, b); }
static void Reset(int termAt) { g_log[0] = 0; g_frame = 0; g_termAt = termAt; }
static int Fail(const char* addr, const char* what) { std::printf("%s_TEST FAIL %s [%s]\n", addr, what, g_log); return 1; }
#define CHECK(addr, cond) do { if (!(cond)) return Fail(addr, #cond); } while (0)
// behaviour test: CBowerstoneIntroGuard::Main scripts the guard, plays the intro cut-scene once (guarded by DoneGuardSpeech), releases the guard
static int g_guardStartAt = 0, g_heroStartAt = 0, g_alive = 0, g_scriptingFlag = 0;
class CScriptThing { public: int Id; };
struct CHandle {
    int Who;
    CHandle() : Who(0) { ++g_alive; }
    ~CHandle() { --g_alive; }
    bool IsScripting() const { return g_scriptingFlag != 0; }
    void Release() { Log("release"); }
};
struct CCast {
    int n; char keys[4][32]; int who[4];
    CCast() : n(0) { ++g_alive; }
    ~CCast() { --g_alive; }
    int& operator[](const char* k) { std::strcpy(keys[n], k); return who[n++]; }
};
struct CMovie { CMovie() { ++g_alive; } ~CMovie() { --g_alive; } };
class CGame { public:
    CScriptThing Hero;
    void NewScriptFrame() const { ++g_frame; }
    bool StartScriptingEntity(const CScriptThing& t, CHandle& h, int p)
    {
        char x[32]; std::sprintf(x, "start %d %d", t.Id, p); Log(x);
        if (g_frame >= (t.Id == 1 ? g_heroStartAt : g_guardStartAt)) { h.Who = t.Id; return true; }
        return false;
    }
    CScriptThing& GetHero() { return Hero; }
    void StartMovieSequence(const char* n, CMovie&) { char x[32]; std::sprintf(x, "movie[%s]", n); Log(x); }
    void FixMovieSequenceCamera(bool b) const { Log(b ? "fix1" : "fix0"); }
    void PauseAllNonScriptedEntities(bool b) { Log(b ? "pause1" : "pause0"); }
};
static void RunCutscene(const char* name, CCast& c)
{
    char x[128]; std::sprintf(x, "run %s %s=%d %s=%d", name, c.keys[0], c.who[0], c.keys[1], c.who[1]); Log(x);
}
static void AddLogbook(const char* t) { Log(t); }
struct CParent { bool DoneGuardSpeech; };
struct CBowerstoneIntroGuard {
    CGame* Game; CScriptThing Me; CParent* ParentClass;
    bool IsActiveThreadTerminating() const { return g_frame >= g_termAt; }
    void Main();
};
void CBowerstoneIntroGuard::Main()
{
    CHandle scripted;
    if (scripted.IsScripting())
        scripted.Release();
    while (!Game->StartScriptingEntity(Me, scripted, 4)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    if (!ParentClass->DoneGuardSpeech) {
        if (IsActiveThreadTerminating())
            return;
        CCast cast;
        CHandle hero;
        while (!Game->StartScriptingEntity(Game->GetHero(), hero, 4)) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        cast["HERO"] = hero.Who;
        cast["Guard"] = scripted.Who;
        {
            CMovie movie;
            Game->StartMovieSequence("", movie);
            CGame* game = Game;
            game->PauseAllNonScriptedEntities(true);
            Game->FixMovieSequenceCamera(true);
            RunCutscene("CS_BOWERSTONE_INTRO_TOWN", cast);
            Game->FixMovieSequenceCamera(false);
            AddLogbook("TEXT_QST_LOG_TOWNS_LAWANDORDER");
            AddLogbook("TEXT_QST_LOG_TOWNS_PUBS");
            ParentClass->DoneGuardSpeech = true;
            game->PauseAllNonScriptedEntities(false);
        }
    }
    if (scripted.IsScripting())
        scripted.Release();
}
int main()
{
    CGame game; game.Hero.Id = 1;
    CParent parent; parent.DoneGuardSpeech = false;
    CBowerstoneIntroGuard g; g.Game = &game; g.Me.Id = 7; g.ParentClass = &parent;
    Reset(100); g_guardStartAt = 1; g_heroStartAt = 2; g_scriptingFlag = 1;
    g.Main();
    CHECK("00d246c0", std::strcmp(g_log, "release@0;start 7 4@0;start 7 4@1;start 1 4@1;start 1 4@2;movie[]@2;pause1@2;fix1@2;"
        "run CS_BOWERSTONE_INTRO_TOWN HERO=1 Guard=7@2;fix0@2;TEXT_QST_LOG_TOWNS_LAWANDORDER@2;TEXT_QST_LOG_TOWNS_PUBS@2;pause0@2;release@2;") == 0);
    CHECK("00d246c0", parent.DoneGuardSpeech && g_alive == 0);
    Reset(100); g_guardStartAt = 0; g_scriptingFlag = 0;
    g.Main();
    CHECK("00d246c0", std::strcmp(g_log, "start 7 4@0;") == 0 && g_alive == 0);
    parent.DoneGuardSpeech = false;
    Reset(1); g_guardStartAt = 5;
    g.Main();
    CHECK("00d246c0", std::strcmp(g_log, "start 7 4@0;") == 0 && g_frame == 1 && !parent.DoneGuardSpeech && g_alive == 0);
    Reset(2); g_guardStartAt = 0; g_heroStartAt = 9;
    g.Main();
    CHECK("00d246c0", std::strcmp(g_log, "start 7 4@0;start 1 4@0;start 1 4@1;") == 0 && g_frame == 2 && !parent.DoneGuardSpeech && g_alive == 0);
    std::printf("00d246c0_TEST PASS\n");
    return 0;
}