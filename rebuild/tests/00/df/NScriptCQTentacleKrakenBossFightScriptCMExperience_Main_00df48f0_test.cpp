// Behaviour test for CM_Experience::Main 0x00DF48F0: a mock script runtime drives the same control flow (predicate on Me,
// wait for MotherDodgingKraken, wait while the Kraken lives, orb at Me + (0,0,5) with the def's experience amount).
#include <cstdio>
#include <cstring>
class C3DVector { public:
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector operator+(const C3DVector& v) const { return C3DVector(X + v.X, Y + v.Y, Z + v.Z); }
};
struct Thing { bool alive; C3DVector pos; Thing() : alive(true), pos(0, 0, 0) { } };
class CScriptThing { public:
    Thing* PImp;
    explicit CScriptThing(Thing* t) : PImp(t) { }
    bool IsAlive() const { return PImp && PImp->alive; }
    const C3DVector& GetPos() const { return PImp->pos; }
};
struct MasterData { bool MotherDodgingKraken; };
struct ScriptDef { long PrisonExperienceForKillingKraken; };
static ScriptDef g_def = { 250 };
static ScriptDef* pGlobalScriptDef = &g_def;
struct Game {
    int frames, dodgeAt, krakenDiesAt, terminateAt;
    MasterData* data; Thing* kraken;
    bool orbMade; C3DVector orbPos; long orbAmount; const CScriptThing* predicateThing;
    Game() : frames(0), dodgeAt(3), krakenDiesAt(7), terminateAt(1000), data(0), kraken(0), orbMade(false), orbPos(0, 0, 0), orbAmount(0), predicateThing(0) { }
    void NewScriptFrame()
    {
        ++frames;
        if (frames == dodgeAt) data->MotherDodgingKraken = true;
        if (frames == krakenDiesAt) kraken->alive = false;
    }
    CScriptThing GetThingWithScriptName(const char* name) { return CScriptThing(std::strcmp(name, "TheKraken") == 0 ? kraken : 0); }
    void CreateExperienceOrb(const C3DVector& p, long amount) { orbMade = true; orbPos = p; orbAmount = amount; }
};
struct CM_Experience {
    Game* G; CScriptThing Me; MasterData* PMasterData;
    CM_Experience(Game* g, Thing* me, MasterData* d) : G(g), Me(me), PMasterData(d) { }
    void SetPredicate(const CScriptThing& t) { G->predicateThing = &t; }
    bool IsActiveThreadTerminating() const { return G->frames >= G->terminateAt; }
    MasterData* GetMasterData() { return PMasterData; }
    void Main()
    {
        SetPredicate(Me);
        G->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
        while (!GetMasterData()->MotherDodgingKraken) {
            G->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
        }
        if (IsActiveThreadTerminating()) return;
        CScriptThing kraken = G->GetThingWithScriptName("TheKraken");
        while (kraken.IsAlive()) {
            G->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
        }
        if (IsActiveThreadTerminating()) return;
        G->CreateExperienceOrb(Me.GetPos() + C3DVector(0.0f, 0.0f, 5.0f), pGlobalScriptDef->PrisonExperienceForKillingKraken);
    }
};
int main()
{
    Thing me, kraken; me.pos = C3DVector(1, 2, 3);
    MasterData d = { false };
    Game g; g.data = &d; g.kraken = &kraken;
    CM_Experience s(&g, &me, &d);
    s.Main();
    if (!g.orbMade || g.frames != 7 || g.orbAmount != 250 || g.orbPos.X != 1 || g.orbPos.Y != 2 || g.orbPos.Z != 8) {
        std::printf("00df48f0_TEST FAIL\n"); return 1;
    }
    Thing k2; MasterData d2 = { false }; Game g2; g2.data = &d2; g2.kraken = &k2; g2.terminateAt = 5;
    CM_Experience s2(&g2, &me, &d2);
    s2.Main();
    if (g2.orbMade || g2.frames != 5) { std::printf("00df48f0_TEST FAIL\n"); return 1; }
    std::printf("00df48f0_TEST PASS\n");
    return 0;
}