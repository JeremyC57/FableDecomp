extern "C" int printf(const char*, ...);
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL line %d\n", __LINE__); } } while (0)
// Behaviour test for 0x00894B90 CGameScriptInterface::SetCreatureBrain: the brain def is looked up by name; when found and the
// script thing is an AI creature (thing type 1) the creature gets the new brain; the def reference is released afterwards.
static int cmp(const char* a, const char* b) { while (*a && *a == *b) { ++a; ++b; } return *a - *b; }
class CBrainDef { public: const char* name; long refs; };
class CDefinitionManager { public: CBrainDef* defs; int n;
    bool GetDef(const char* name, CBrainDef** out) { for (int i = 0; i < n; ++i) if (cmp(defs[i].name, name) == 0) { *out = &defs[i]; ++defs[i].refs; return true; } return false; } };
class CThing { public: long Type; CBrainDef* brain; };
static void SetCreatureBrain(CDefinitionManager& dm, CThing* t, const char* name)
{
    CBrainDef* brain = 0;
    if (dm.GetDef(name, &brain)) { if (t && t->Type == 1) t->brain = brain; }
    if (brain) --brain->refs;
}
int main()
{
    CBrainDef d[2] = { { "BRAIN_GUARD", 1 }, { "BRAIN_VILLAGER", 1 } };
    CDefinitionManager dm; dm.defs = d; dm.n = 2;
    CThing creature = { 1, 0 }, object = { 3, 0 };
    SetCreatureBrain(dm, &creature, "BRAIN_VILLAGER"); CHECK(creature.brain == &d[1] && d[1].refs == 1);
    SetCreatureBrain(dm, &object, "BRAIN_GUARD"); CHECK(object.brain == 0 && d[0].refs == 1);
    SetCreatureBrain(dm, &creature, "BRAIN_NONE"); CHECK(creature.brain == &d[1]);
    SetCreatureBrain(dm, 0, "BRAIN_GUARD"); CHECK(d[0].refs == 1);
    if (fails == 0) printf("00894b90_TEST PASS\n"); else printf("00894b90_TEST FAIL\n");
    return fails;
}