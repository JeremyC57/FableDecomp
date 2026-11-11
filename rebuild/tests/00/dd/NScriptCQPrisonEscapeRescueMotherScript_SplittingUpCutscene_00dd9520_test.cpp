// Behaviour test for 0x00DD9520 NScript::CQ_PrisonEscapeRescueMotherScript::SplittingUpCutscene(): the row's body copied over
// test doubles (game interface, script thing, handles, cut-scene macro, thread-termination counter); checks the call order of
// the early-exit paths and of the full cut-scene path, the teleport arguments (marker position / angle, the null-thing angle 0)
// and the quest-completion tail.
#include <cstdio>
#include <cstring>
#include <map>
#include <string>
namespace prison_test {
static int g_log[256]; static int g_n = 0;
static void Log(int v) { if (g_n < 256) g_log[g_n++] = v; }
struct CCharString {
    std::string s;
    CCharString(const char* t) : s(t) { }
    bool operator<(const CCharString& o) const { return s < o.s; }
};
struct C3DVector { float X, Y, Z; };
static C3DVector g_nullCoord = { 0.0f, 0.0f, 0.0f };
struct CThingImp { C3DVector pos; float angle; bool alive; };
static CThingImp g_marker = { { 1.0f, 2.0f, 3.0f }, 0.75f, true };
static CThingImp g_mother = { { 0.0f, 0.0f, 0.0f }, 0.0f, true };
struct CScriptThing {
    CThingImp* Data;
    CScriptThing() : Data(0) { }
    const C3DVector& GetPos() const { if (!Data) return g_nullCoord; return Data->pos; }
    float GetAngleXY() const { if (!Data) return 0.0f; return (Data->angle); }
    bool IsAlive() const { if (!Data) return false; return Data->alive; }
};
struct CScriptGameResourceObjectScriptedThingBase {
    int id;
    CScriptGameResourceObjectScriptedThingBase() : id(0) { }
    void Release() { Log(60); }
};
struct CScriptGameResourceObjectMovieBase { };
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_HIGHEST = 4 };
struct CScriptDef { long PrisonEscapeExperienceFromMother; };
static CScriptDef g_def = { 250 };
static CScriptDef* pGlobalScriptDef = &g_def;
static bool g_regionLoaded = true; static bool g_nullMarker = false;
static float g_teleAngle = -1.0f; static C3DVector g_telePos;
struct CGame {
    CScriptThing hero;
    void NewScriptFrame() { Log(1); }
    bool IsRegionLoaded(const CCharString& s) { Log(s.s == "Graveyard" ? 2 : 99); return g_regionLoaded; }
    CScriptThing GetThingWithScriptName(const CCharString& s) {
        CScriptThing t;
        if (s.s == "OUTRO_SPLITTING_UP") { Log(3); if (!g_nullMarker) t.Data = &g_marker; }
        else if (s.s == "MotherTorture") { Log(4); t.Data = &g_mother; }
        else Log(99);
        return t;
    }
    const CScriptThing& GetHero() { return hero; }
    void EntityTeleportToPosition(const CScriptThing&, const C3DVector& p, float a, bool b1, bool b2) {
        Log(b1 && !b2 ? 5 : 99); g_telePos = p; g_teleAngle = a;
    }
    void PlayAVIMovie(const CCharString& s) { Log(s.s == "Data\\Video\\8_prison_2_comp.xmv" ? 6 : 99); }
    bool StartScriptingEntity(const CScriptThing&, CScriptGameResourceObjectScriptedThingBase& h, EScriptAIPriority p) { Log(p == 4 ? 7 : 99); h.id = 42; return true; }
    void RemoveThing(const CScriptThing& t, bool a, bool b) { Log(t.Data == &g_mother && !a && b ? 8 : 99); }
    void StartMovieSequence(const CCharString& s, CScriptGameResourceObjectMovieBase&) { Log(s.s.empty() ? 9 : 99); }
    void ResetPlayerCreatureCombatMultiplier() { Log(10); }
    void GiveHeroExperience(long e) { Log(e == 250 ? 11 : 99); }
    CCharString GetActiveQuestName() { return CCharString("Q_PrisonEscape"); }
    void SetQuestAsCompleted(const CCharString& s, bool a, bool b, bool c) { Log(s.s == "Q_PrisonEscape" && a && !b && !c ? 12 : 99); }
    void SetGuardsIgnoreCrimes(bool b) { Log(!b ? 13 : 99); }
    void SetTeleportingAsActive(bool b) { Log(b ? 14 : 99); }
    void ResetWhetherToFailQuestOnDeathToDefault() { Log(15); }
    void ResetDeathRecoveryMarkerNameToDefault() { Log(16); }
    void DeactivateQuestLater(const CCharString& s, long d) { Log(d == 0 && s.s == "Q_PrisonEscape" ? 17 : (d == 0 && s.s == "Q_PrisonEscapeFromCell" ? 18 : 99)); }
};
static void RunCutsceneMacro(const CCharString& s, std::map<CCharString, CScriptGameResourceObjectScriptedThingBase>& cast, void*, void*, bool a, bool b)
{
    Log(s.s == "CS_MOTHER_SPLITTING_UP" && cast.size() == 1 && cast.begin()->second.id == 42 && !a && b ? 20 : 99);
}
static int g_doneAfter = 1000, g_doneCalls = 0;
static int g_followAfter = 0;
struct CQ_PrisonEscapeRescueMotherScript {
    CGame* Game; bool MotherFollowing;
    bool IsActiveThreadTerminating() { Log(30); if (g_followAfter > 0 && --g_followAfter == 0) MotherFollowing = true; return ++g_doneCalls >= g_doneAfter; }
    void SplittingUpCutscene();
};
void CQ_PrisonEscapeRescueMotherScript::SplittingUpCutscene()
{
    while (!MotherFollowing) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    while (!Game->IsRegionLoaded("Graveyard")) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    CScriptThing outro = Game->GetThingWithScriptName("OUTRO_SPLITTING_UP");
    Game->EntityTeleportToPosition(Game->GetHero(), outro.GetPos(), outro.GetAngleXY(), true, false);
    Game->PlayAVIMovie("Data\\Video\\8_prison_2_comp.xmv");
    CScriptGameResourceObjectScriptedThingBase hero;
    Game->StartScriptingEntity(Game->GetHero(), hero, SCRIPT_AI_PRIORITY_HIGHEST);
    CScriptGameResourceObjectScriptedThingBase motherHandle;
    CScriptThing mother = Game->GetThingWithScriptName("MotherTorture");
    if (mother.IsAlive()) {
        if (IsActiveThreadTerminating())
            return;
        Game->RemoveThing(mother, false, true);
    }
    CScriptGameResourceObjectMovieBase movie;
    Game->StartMovieSequence("", movie);
    std::map<CCharString, CScriptGameResourceObjectScriptedThingBase> cast;
    cast["Hero"] = hero;
    RunCutsceneMacro("CS_MOTHER_SPLITTING_UP", cast, 0, 0, false, true);
    hero.Release();
    Game->ResetPlayerCreatureCombatMultiplier();
    Game->GiveHeroExperience(pGlobalScriptDef->PrisonEscapeExperienceFromMother);
    Game->SetQuestAsCompleted(Game->GetActiveQuestName(), true, false, false);
    Game->SetGuardsIgnoreCrimes(false);
    Game->SetTeleportingAsActive(true);
    Game->ResetWhetherToFailQuestOnDeathToDefault();
    Game->ResetDeathRecoveryMarkerNameToDefault();
    Game->DeactivateQuestLater(Game->GetActiveQuestName(), 0);
    Game->DeactivateQuestLater("Q_PrisonEscapeFromCell", 0);
}
static bool Same(const int* want, int n)
{
    if (g_n != n) return false;
    for (int i = 0; i < n; ++i) if (g_log[i] != want[i]) return false;
    return true;
}
static void Reset(int doneAfter, int followAfter, bool following)
{
    g_n = 0; g_doneCalls = 0; g_doneAfter = doneAfter; g_followAfter = followAfter; g_teleAngle = -1.0f;
    (void)following;
}
} // namespace prison_test
using namespace prison_test;
int main()
{
    bool ok = true;
    CGame game;
    CQ_PrisonEscapeRescueMotherScript s;
    s.Game = &game;
    // 1. Mother not yet following: wait a frame, she follows on the first poll, then termination at the next test.
    Reset(2, 1, false); s.MotherFollowing = false;
    s.SplittingUpCutscene();
    { const int w[] = { 1, 30, 30 }; ok = ok && Same(w, 3); }
    // 2. Full path, region loaded at once, marker present, mother alive.
    Reset(1000, 0, true); s.MotherFollowing = true; g_regionLoaded = true; g_nullMarker = false;
    s.SplittingUpCutscene();
    { const int w[] = { 30, 2, 30, 3, 5, 6, 7, 4, 30, 8, 9, 20, 60, 10, 11, 12, 13, 14, 15, 16, 17, 18 };
      ok = ok && Same(w, sizeof(w) / sizeof(w[0])); }
    ok = ok && g_teleAngle == 0.75f && g_telePos.X == 1.0f && g_telePos.Y == 2.0f && g_telePos.Z == 3.0f;
    // 3. Null marker: the teleport gets the null coordinate and angle 0; termination while removing the mother.
    Reset(3, 0, true); s.MotherFollowing = true; g_nullMarker = true;
    s.SplittingUpCutscene();
    { const int w[] = { 30, 2, 30, 3, 5, 6, 7, 4, 30 }; ok = ok && Same(w, sizeof(w) / sizeof(w[0])); }
    ok = ok && g_teleAngle == 0.0f && g_telePos.X == 0.0f;
    // 4. Region not loaded on the first poll: a frame, then termination inside the region wait.
    Reset(2, 0, true); s.MotherFollowing = true; g_regionLoaded = false;
    s.SplittingUpCutscene();
    { const int w[] = { 30, 2, 1, 30 }; ok = ok && Same(w, 4); }
    std::printf(ok ? "00dd9520_TEST PASS\n" : "00dd9520_TEST FAIL\n");
    return ok ? 0 : 1;
}