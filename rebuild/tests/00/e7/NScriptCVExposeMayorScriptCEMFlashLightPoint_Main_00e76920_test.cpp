// Behaviour test for CEM_FlashLightPoint::Main (0x00E76920): waits for stage 2, spawns CheckLampSlot, then per
// attempt waits for the hero to be near, the lamp to go off and on, spawns HeroFlashesLamp(5), waits for a result or
// a reset, clears ResetLamp and pauses; stops when the hero flashed correctly or the thread terminates.
#include <cstdio>
#include <cstring>

static int g_frame, g_terminateAt;
static int g_spawnCheck, g_spawnFlash, g_lastPar, g_pauses;
static bool g_lampLit[64];
static bool g_near[64];
class CScriptThing { public: int id; };
struct Script {
    long StageNumber; bool HeroFlashedLightsCorrectly; bool ResetLamp; float MaxFlashDistFromStables;
};
static Script g_parent;
static void Tick()
{
    ++g_frame;
    if (g_frame == 3) g_parent.StageNumber = 2;
    if (g_spawnFlash == 1 && g_frame == 20) g_parent.ResetLamp = true;
    if (g_spawnFlash == 2 && g_frame == 30) g_parent.HeroFlashedLightsCorrectly = true;
}
struct Game {
    void NewScriptFrame() const { Tick(); }
    CScriptThing& GetHero() { static CScriptThing hero = { 1 }; return hero; }
    bool IsHeroHandLampLit() const { return g_lampLit[g_frame]; }
    void Pause(float s) const { if (s == 0.5f) ++g_pauses; }
};
static bool IsDistanceBetweenThingsUnder(const CScriptThing&, const CScriptThing&, float d) { return d == 18.0f && g_near[g_frame]; }
struct Point {
    Game* G; CScriptThing Me; Script* ParentClass;
    bool IsActiveThreadTerminating() { return g_frame >= g_terminateAt; }
    void SpawnCheck(const char* name) { if (std::strcmp(name, "ParentClass.CheckLampSlot") == 0) ++g_spawnCheck; }
    void SpawnFlash(const char* name, long par) { if (std::strcmp(name, "ParentClass.HeroFlashesLamp") == 0) { ++g_spawnFlash; g_lastPar = par; } }
    void Main()
    {
        while (ParentClass->StageNumber != 2) { G->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
        if (IsActiveThreadTerminating()) return;
        SpawnCheck("ParentClass.CheckLampSlot");
        while (!ParentClass->HeroFlashedLightsCorrectly) {
            G->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
            while (!IsDistanceBetweenThingsUnder(G->GetHero(), Me, ParentClass->MaxFlashDistFromStables)) { G->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
            if (IsActiveThreadTerminating()) return;
            while (G->IsHeroHandLampLit()) { G->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
            if (IsActiveThreadTerminating()) return;
            while (!G->IsHeroHandLampLit()) { G->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
            if (IsActiveThreadTerminating()) return;
            SpawnFlash("ParentClass.HeroFlashesLamp", 5);
            while (!ParentClass->HeroFlashedLightsCorrectly && !ParentClass->ResetLamp) { G->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
            if (IsActiveThreadTerminating()) return;
            ParentClass->ResetLamp = false;
            G->Pause(0.5f);
        }
        if (IsActiveThreadTerminating()) return;
    }
};
int main()
{
    int i;
    for (i = 0; i < 64; ++i) { g_near[i] = i >= 6; g_lampLit[i] = (i >= 6 && i < 9) || (i >= 12) ; }
    g_terminateAt = 1000;
    g_parent.StageNumber = 0; g_parent.HeroFlashedLightsCorrectly = false; g_parent.ResetLamp = false; g_parent.MaxFlashDistFromStables = 18.0f;
    Game game; Point p; p.G = &game; p.Me.id = 2; p.ParentClass = &g_parent;
    p.Main();
    // stage reached at frame 3; first attempt: near at 6, lamp lit 6..8 -> off at 9, on again at 12 -> spawn 1;
    // reset at 20 -> second attempt: lamp lit -> wait until off... lamp stays lit, so terminate guard decides.
    bool ok = g_spawnCheck == 1 && g_spawnFlash >= 1 && g_lastPar == 5 && g_pauses >= 1;
    // second scenario: terminate during the first wait
    g_frame = 0; g_terminateAt = 2; g_spawnCheck = 0; g_spawnFlash = 0; g_pauses = 0;
    g_parent.StageNumber = 0; g_parent.HeroFlashedLightsCorrectly = false; g_parent.ResetLamp = false;
    p.Main();
    ok = ok && g_spawnCheck == 0 && g_spawnFlash == 0 && g_frame == 2;
    if (!ok) { std::printf("00e76920_TEST FAIL\n"); return 1; }
    std::printf("00e76920_TEST PASS\n");
    return 0;
}