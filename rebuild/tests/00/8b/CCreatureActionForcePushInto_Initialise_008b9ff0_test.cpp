// behaviour test for 0x008B9FF0 CCreatureAction_ForcePushInto::Initialise(): set the "FORCE_PUSH_INTO" combat animation
// (the name dies first), run the combat Initialise, then give the creature's mode manager a new charge-force-push mode
// (creature, OnlyHitEnemies) through a counted pointer; no mode manager, no mode
#include <cstdio>
#include <cstring>
static int gStrings = 0, gOrder = 0;
struct Str { const char* s; Str(const char* t) : s(t) { ++gStrings; } ~Str() { --gStrings; } };
struct Thing;
struct Mode { virtual ~Mode() {} };
struct ChargeForcePush : Mode { Thing* thing; bool onlyEnemies; ChargeForcePush(Thing& t, bool e) : thing(&t), onlyEnemies(e) {} };
struct Counted { Mode* Data; explicit Counted(Mode* m) : Data(m) {} };
struct Modes { Mode* added; int count; bool AddMode(Counted c) { delete added; added = c.Data; ++count; return true; } ~Modes() { delete added; } };
struct Thing { Modes* modes; bool GetTC(Modes** out) { if (!modes) return false; *out = modes; return true; } };
struct Action {
    Thing* PCreature; bool OnlyHitEnemies; char anim[24]; int animStrings, initAt, liveStringsAtInit;
    void SetCombatAnimation(const Str& s) { std::strcpy(anim, s.s); animStrings = gStrings; }
    void BaseInitialise() { initAt = ++gOrder; liveStringsAtInit = gStrings; }
    void Initialise()
    {
        {
            Str animName("FORCE_PUSH_INTO");
            SetCombatAnimation(animName);
        }
        BaseInitialise();
        Modes* modes;
        if (PCreature->GetTC(&modes))
            modes->AddMode(Counted(new ChargeForcePush(*PCreature, OnlyHitEnemies)));
    }
};
int main()
{
    int fails = 0;
    Modes m; m.added = 0; m.count = 0;
    Thing hero = { &m };
    Action a; a.PCreature = &hero; a.OnlyHitEnemies = true;
    a.Initialise();
    if (std::strcmp(a.anim, "FORCE_PUSH_INTO") != 0 || a.animStrings != 1 || a.liveStringsAtInit != 0 || a.initAt != 1) fails |= 1;
    ChargeForcePush* mode = (ChargeForcePush*)m.added;
    if (m.count != 1 || !mode || mode->thing != &hero || !mode->onlyEnemies) fails |= 2;
    Thing ghost = { 0 }; a.PCreature = &ghost; a.Initialise();
    if (m.count != 1 || a.initAt != 2) fails |= 4;
    if (fails) { std::printf("008b9ff0_TEST FAIL %d\n", fails); return 1; }
    std::printf("008b9ff0_TEST PASS\n");
    return 0;
}