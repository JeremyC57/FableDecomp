// 00e73930 behaviour test: CDF_SexMad::Main (match.cpp body verbatim, pragmas dropped; for(;;) main loop with the
// terminate check at its top), host stand-ins. C++03, g++ -w; also VC7.1 /Od.
#include <stdio.h>
#include <string>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { printf("FAIL line %d: %s\n", __LINE__, #c); g_fail = 1; } } while (0)
static std::string g_log;
static void logs(const std::string& s) { g_log += s; g_log += ";"; }
static std::string num(long v) { char b[24]; sprintf(b, "%ld", v); return b; }
class CCharString { public:
    std::string s;
    CCharString(const char* text, long length = -1) : s(text) { (void)length; }
};
static int g_talks = 0;          // pending "talked to by hero" messages
static int g_taskFrames = 0;     // frames each Speak keeps the face busy
static int g_busy = 0;
static float g_faceHealth = 1.0f;
class CScriptThing { public:
    std::string Name;
    CScriptThing() {}
    CScriptThing(const char* n) : Name(n) {}
    bool MsgIsTalkedToBy(const CCharString& name) const { (void)name; if (g_talks > 0) { --g_talks; return true; } return false; }
};
struct CIsThingAlive {
    CScriptThing m_thing;
    CIsThingAlive(const CScriptThing& thing) : m_thing(thing) { }
};
enum ETextGroupSelectionMethod { TEXT_GROUP_SELECTION_RANDOM };
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_NONE };
static int g_startFails = 0;     // StartScriptingEntity fails this many times first
class CScriptGameResourceObjectScriptedThingBase { public:
    bool scripting;
    CScriptGameResourceObjectScriptedThingBase() : scripting(false) { }
    bool IsScripting() { return scripting; }
    void Release() { logs("release"); scripting = false; }
    CScriptThing GetScriptThing() const { return CScriptThing("face"); }
    void Speak(const CScriptThing& listener, const char* text, ETextGroupSelectionMethod method, bool a, bool b, bool c)
    { (void)method; (void)a; (void)b; (void)c; logs("speak-" + listener.Name + "-" + text); g_busy = g_taskFrames; }
    bool IsPerformingScriptTask() const { return g_busy > 0; }
};
class CScriptGameResourceObjectMovieBase { public:
    CScriptGameResourceObjectMovieBase() { }
    ~CScriptGameResourceObjectMovieBase() { logs("movie-end"); }
};
class CGameScriptInterface { public:
    CScriptThing Hero;
    long Frames, FrameLimit, SexCount;
    CGameScriptInterface() : Hero("hero"), Frames(0), FrameLimit(100), SexCount(0) {}
    void NewScriptFrame() const { CGameScriptInterface* g = const_cast<CGameScriptInterface*>(this); ++g->Frames; if (g_busy > 0) --g_busy; }
    bool StartScriptingEntity(const CScriptThing& thing, CScriptGameResourceObjectScriptedThingBase& object, EScriptAIPriority priority)
    { (void)thing; (void)priority; if (g_startFails > 0) { --g_startFails; return false; } object.scripting = true; logs("start"); return true; }
    CScriptThing& GetHero() { return Hero; }
    CScriptThing GetNearestWithDefName(const CScriptThing& thing, const CCharString& defName) const { logs("nearest-" + thing.Name + "-" + defName.s); return CScriptThing("exit"); }
    long GetNumberOfTimesHeroHasHadSex() const { return SexCount; }
    float GetHealth(const CScriptThing& thing) const { return thing.Name == "face" ? g_faceHealth : 0.0f; }
    void StartMovieSequence(const CCharString& name, CScriptGameResourceObjectMovieBase& movie) { (void)movie; logs("movie[" + name.s + "]"); }
    void PauseAllNonScriptedEntities(bool pause) { logs(pause ? "pause" : "unpause"); }
    void SetThingPersistent(const CScriptThing& thing, bool persistent) const { logs("persist-" + thing.Name + (persistent ? "-t" : "-f")); }
    void EntitySetAsDamageable(const CScriptThing& thing, bool damageable) const { logs("damageable-" + thing.Name + (damageable ? "-t" : "-f")); }
    void EntitySetAsKillable(const CScriptThing& thing, bool killable, bool b) const { (void)b; logs("killable-" + thing.Name + (killable ? "-t" : "-f")); }
    void EntitySetAsToAddToComboMultiplierWhenHit(const CScriptThing& thing, bool add) const { logs("combo-" + thing.Name + (add ? "-t" : "-f")); }
    void SetRegionExitAsActive(const CScriptThing& exit, bool active) const { logs("exit-" + exit.Name + (active ? "-t" : "-f")); }
};
static const float g_fZero = 0.0f;
struct CV_DemonDoorsScript {
    void FadeOutFaceAndOpenDoor(const CScriptThing& face, const CCharString& doorName) { logs("open-" + face.Name + "-" + doorName.s); }
};
class CEntityScriptBase { public:
    CGameScriptInterface* Game;
    CScriptThing          Me;
    long Checks, TerminateAtCheck;
    void RegisterFilter(CIsThingAlive& filter) { logs("filter-" + filter.m_thing.Name); }
    bool IsActiveThreadTerminating() const
    {
        CEntityScriptBase* s = const_cast<CEntityScriptBase*>(this);
        ++s->Checks;
        if (TerminateAtCheck >= 0 && Checks >= TerminateAtCheck) return true;
        return Game->Frames >= Game->FrameLimit;
    }
};
struct CDF_SexMad : CEntityScriptBase {
    CV_DemonDoorsScript* ParentClass;
    void Main();
};

void CDF_SexMad::Main()
{
    CScriptGameResourceObjectScriptedThingBase face;
    {
        CIsThingAlive filter(Me);
        RegisterFilter(filter);
    }
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    if (face.IsScripting())
        face.Release();
    while (!Game->StartScriptingEntity(Me, face, (EScriptAIPriority)4)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            {
                return;
            }
    }
    if (IsActiveThreadTerminating())
        {
            return;
        }
    Game->SetThingPersistent(Me, true);
    Game->EntitySetAsDamageable(Me, false);
    Game->EntitySetAsToAddToComboMultiplierWhenHit(Me, false);
    Game->EntitySetAsKillable(Me, false, false);
    CScriptThing exitPoint = Game->GetNearestWithDefName(Me, "REGION_EXIT_POINT");
    Game->SetRegionExitAsActive(exitPoint, false);
    for (;;) {
        if (IsActiveThreadTerminating())
            return;
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating())
                break;
            CScriptGameResourceObjectMovieBase movie;
            Game->StartMovieSequence("", movie);
            CGameScriptInterface* game = Game;
            game->PauseAllNonScriptedEntities(true);
            if (Game->GetNumberOfTimesHeroHasHadSex() == 0) {
                if (IsActiveThreadTerminating()) {
                    game->PauseAllNonScriptedEntities(false);
                    return;
                }
                if (Game->GetHealth(face.GetScriptThing()) > g_fZero) {
                    face.Speak(Game->GetHero(), "TEXT_QST_B19_SEX_DOOR_NONE", (ETextGroupSelectionMethod)0, false, true, false);
                    while (face.IsPerformingScriptTask()) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) {
                            game->PauseAllNonScriptedEntities(false);
                            return;
                        }
                    }
                    if (IsActiveThreadTerminating()) {
                        game->PauseAllNonScriptedEntities(false);
                        return;
                    }
                }
            } else if (Game->GetNumberOfTimesHeroHasHadSex() < 4) {
                if (IsActiveThreadTerminating()) {
                    game->PauseAllNonScriptedEntities(false);
                    return;
                }
                if (Game->GetHealth(face.GetScriptThing()) > g_fZero) {
                    face.Speak(Game->GetHero(), "TEXT_QST_B19_SEX_DOOR_LOW", (ETextGroupSelectionMethod)0, false, true, false);
                    while (face.IsPerformingScriptTask()) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) {
                            game->PauseAllNonScriptedEntities(false);
                            return;
                        }
                    }
                    if (IsActiveThreadTerminating()) {
                        game->PauseAllNonScriptedEntities(false);
                        return;
                    }
                }
            } else if (Game->GetNumberOfTimesHeroHasHadSex() < 7) {
                if (IsActiveThreadTerminating()) {
                    game->PauseAllNonScriptedEntities(false);
                    return;
                }
                if (Game->GetHealth(face.GetScriptThing()) > g_fZero) {
                    face.Speak(Game->GetHero(), "TEXT_QST_B19_SEX_DOOR_MEDIUM", (ETextGroupSelectionMethod)0, false, true, false);
                    while (face.IsPerformingScriptTask()) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) {
                            game->PauseAllNonScriptedEntities(false);
                            return;
                        }
                    }
                    if (IsActiveThreadTerminating()) {
                        game->PauseAllNonScriptedEntities(false);
                        return;
                    }
                }
            } else if (Game->GetNumberOfTimesHeroHasHadSex() < 10) {
                if (IsActiveThreadTerminating()) {
                    game->PauseAllNonScriptedEntities(false);
                    return;
                }
                if (Game->GetHealth(face.GetScriptThing()) > g_fZero) {
                    face.Speak(Game->GetHero(), "TEXT_QST_B19_SEX_DOOR_HIGH", (ETextGroupSelectionMethod)0, false, true, false);
                    while (face.IsPerformingScriptTask()) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) {
                            game->PauseAllNonScriptedEntities(false);
                            return;
                        }
                    }
                    if (IsActiveThreadTerminating()) {
                        game->PauseAllNonScriptedEntities(false);
                        return;
                    }
                }
            } else {
                if (IsActiveThreadTerminating()) {
                    game->PauseAllNonScriptedEntities(false);
                    return;
                }
                if (Game->GetHealth(face.GetScriptThing()) > g_fZero) {
                    face.Speak(Game->GetHero(), "TEXT_QST_B19_SEX_DOOR_DONE", (ETextGroupSelectionMethod)0, false, true, false);
                    while (face.IsPerformingScriptTask()) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) {
                            game->PauseAllNonScriptedEntities(false);
                            return;
                        }
                    }
                    if (IsActiveThreadTerminating()) {
                        game->PauseAllNonScriptedEntities(false);
                        return;
                    }
                }
                {
                    CCharString door("DD_SexMad");
                    ParentClass->FadeOutFaceAndOpenDoor(Me, door);
                }
                Game->SetRegionExitAsActive(exitPoint, true);
                game->PauseAllNonScriptedEntities(false);
                return;
            }
            game->PauseAllNonScriptedEntities(false);
        }
        Game->NewScriptFrame();
    }
}

static std::string Run(long sex, long frames, int talks, long terminateAt = -1)
{
    CGameScriptInterface g; g.SexCount = sex; g.FrameLimit = frames;
    CV_DemonDoorsScript p;
    CDF_SexMad s; s.Game = &g; s.Me = CScriptThing("door"); s.Checks = 0; s.TerminateAtCheck = terminateAt; s.ParentClass = &p;
    g_log = ""; g_talks = talks; g_busy = 0;
    s.Main();
    return g_log;
}

int main()
{
    const std::string setup = "filter-door;start;persist-door-t;damageable-door-f;combo-door-f;killable-door-f;nearest-door-REGION_EXIT_POINT;exit-exit-f;";
    g_taskFrames = 2; g_faceHealth = 1.0f; g_startFails = 1;
    // nobody talks: idle frames until the thread ends
    CHECK(Run(0, 5, 0) == setup);
    g_startFails = 0;
    // no sex yet: the NONE line, then idle
    CHECK(Run(0, 8, 1) == setup + "movie[];pause;speak-hero-TEXT_QST_B19_SEX_DOOR_NONE;unpause;movie-end;");
    CHECK(Run(2, 8, 1) == setup + "movie[];pause;speak-hero-TEXT_QST_B19_SEX_DOOR_LOW;unpause;movie-end;");
    CHECK(Run(5, 8, 1) == setup + "movie[];pause;speak-hero-TEXT_QST_B19_SEX_DOOR_MEDIUM;unpause;movie-end;");
    CHECK(Run(9, 8, 1) == setup + "movie[];pause;speak-hero-TEXT_QST_B19_SEX_DOOR_HIGH;unpause;movie-end;");
    // ten or more: the DONE line, the door opens, the exit activates and the script ends at once
    CHECK(Run(10, 50, 3) == setup + "movie[];pause;speak-hero-TEXT_QST_B19_SEX_DOOR_DONE;open-door-DD_SexMad;exit-exit-t;unpause;movie-end;");
    // talked to twice: two lines
    CHECK(Run(3, 20, 2) == setup + "movie[];pause;speak-hero-TEXT_QST_B19_SEX_DOOR_LOW;unpause;movie-end;"
                                   "movie[];pause;speak-hero-TEXT_QST_B19_SEX_DOOR_LOW;unpause;movie-end;");
    // a dead face says nothing
    g_faceHealth = 0.0f;
    CHECK(Run(0, 8, 1) == setup + "movie[];pause;unpause;movie-end;");
    g_faceHealth = 1.0f;
    // the thread ends while the line plays: unpause then out
    g_taskFrames = 50;
    CHECK(Run(0, 4, 1) == setup + "movie[];pause;speak-hero-TEXT_QST_B19_SEX_DOOR_NONE;unpause;movie-end;");
    // terminating at the in-arm check (check 4: first frame, start wait, loop top, arm) leaves before the movie
    g_taskFrames = 2;
    CHECK(Run(0, 50, 1, 4) == setup);
    // terminating right after the first frame: no start
    CHECK(Run(0, 50, 1, 1) == "filter-door;");
    if (!g_fail) printf("00e73930_TEST PASS\n");
    return g_fail;
}