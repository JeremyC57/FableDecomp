// Behaviour test: CCreatureAction_NymphGetHit::Initialise (0x008CA220) -- reset the physics interpolation, disable
// physics, remember the "DUMMY_CENTRE" dummy position (the creature position when missing), flag the nymph as hit
// (when the creature has the nymph component), then run the base Initialise.
#include <cstdio>
#include <cstring>
static char g_log[256];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
struct V3 { float X, Y, Z; };
class CTCNymph { public: bool BeenHit; };
struct CCreature { V3 Pos; bool DummyFound; V3 Dummy; CTCNymph* Nymph; bool PhysicsOn; };
struct CGetHit {
    CCreature* PCreature;
    V3 InitDummyPos;
    void Initialise()
    {
        Log("resetinterp");
        PCreature->PhysicsOn = false; Log("physicsoff");
        if (PCreature->DummyFound) InitDummyPos = PCreature->Dummy;
        else InitDummyPos = PCreature->Pos;
        if (PCreature->Nymph) PCreature->Nymph->BeenHit = true;
        Log("base");
    }
};
int main()
{
    CTCNymph nymph = { false };
    CCreature c; std::memset(&c, 0, sizeof(c));
    c.Pos.X = 7; c.Dummy.X = 3; c.DummyFound = true; c.Nymph = &nymph; c.PhysicsOn = true;
    CGetHit a; a.PCreature = &c; a.Initialise();
    bool ok = std::strcmp(g_log, "resetinterp;physicsoff;base;") == 0 && a.InitDummyPos.X == 3 && nymph.BeenHit && !c.PhysicsOn;
    c.DummyFound = false; c.Nymph = 0; a.Initialise();
    ok = ok && a.InitDummyPos.X == 7;
    if (!ok) { std::printf("008ca220_TEST FAIL\n"); return 1; }
    std::printf("008ca220_TEST PASS\n");
    return 0;
}