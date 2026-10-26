#include <cstdio>
#include <cstring>
#include <string>
#include <map>
// Behaviour test for CQ_SecretPassageScript::MotherTortureThread (0x00DF0750): waits for the torture chamber level,
// then for the hero to come within 3 units of the kick-off marker, casts hero and mother and runs the rescue cut-scene.
struct CScriptThing { int Id; float X; };
struct CHandle { int Thing; CHandle() : Thing(0) {} };
struct CGSI {
    int Frames; int LoadFrame; int Started; char Movie[32]; char Macro[64]; int CastHero; int CastMother;
    CScriptThing Hero;
    void NewScriptFrame() { ++Frames; Hero.X -= 1.0f; }
    bool IsLevelLoaded(const char* level) { return Frames >= LoadFrame && !std::strcmp(level, "PrisonTortureChamber"); }
    CScriptThing GetThingWithScriptName(const char* name)
    {
        CScriptThing t; t.X = 0.0f; t.Id = !std::strcmp(name, "MotherTorture") ? 3 : !std::strcmp(name, "CUTSCENE_KICKOFF_MARKER") ? 2 : 0;
        return t;
    }
    CScriptThing& GetHero() { return Hero; }
    bool StartScriptingEntity(const CScriptThing& t, CHandle& h, int priority) { h.Thing = t.Id; Started += priority; return true; }
    void StartMovieSequence(const char* name, CHandle&) { std::strcpy(Movie, name); }
};
static CGSI* g_GSI;
static bool IsDistanceBetweenThingsUnder(const CScriptThing& a, const CScriptThing& b, float d)
{
    float dx = a.X - b.X; if (dx < 0) dx = -dx; return dx < d;
}
static void RunCutsceneMacro(const char* name, std::map<std::string, CHandle>& cast, void*, void*, bool, bool)
{
    std::strcpy(g_GSI->Macro, name); g_GSI->CastHero = cast["Hero"].Thing; g_GSI->CastMother = cast["Mother"].Thing;
}
struct CQ_SecretPassageScript {
    CGSI* GSI;
    bool IsActiveThreadTerminating() { return GSI->Frames > 40; }
    void MotherTortureThread()
    {
        while (!GSI->IsLevelLoaded("PrisonTortureChamber")) { GSI->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
        if (IsActiveThreadTerminating()) return;
        CScriptThing marker = GSI->GetThingWithScriptName("CUTSCENE_KICKOFF_MARKER");
        while (!IsDistanceBetweenThingsUnder(GSI->GetHero(), marker, 3.0f)) { GSI->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
        if (IsActiveThreadTerminating()) return;
        CScriptThing mother = GSI->GetThingWithScriptName("MotherTorture");
        CHandle hero, motherHandle;
        GSI->StartScriptingEntity(GSI->GetHero(), hero, 4);
        GSI->StartScriptingEntity(mother, motherHandle, 4);
        std::map<std::string, CHandle> cast;
        cast["Hero"] = hero;
        cast["Mother"] = motherHandle;
        CHandle movie;
        GSI->StartMovieSequence("", movie);
        RunCutsceneMacro("CS_MOTHER_RESCUE_TORTURE", cast, 0, 0, false, true);
    }
};
int main()
{
    CGSI gsi; std::memset(&gsi, 0, sizeof gsi); gsi.LoadFrame = 4; gsi.Hero.Id = 1; gsi.Hero.X = 10.0f; gsi.Movie[0] = 'x';
    g_GSI = &gsi;
    CQ_SecretPassageScript s; s.GSI = &gsi;
    s.MotherTortureThread();
    bool ok = gsi.Frames == 8 && gsi.Started == 8 && gsi.Movie[0] == 0 && !std::strcmp(gsi.Macro, "CS_MOTHER_RESCUE_TORTURE")
              && gsi.CastHero == 1 && gsi.CastMother == 3;
    if (!ok) { std::printf("00df0750_TEST FAIL %d\n", gsi.Frames); return 1; }
    std::printf("00df0750_TEST PASS\n");
    return 0;
}