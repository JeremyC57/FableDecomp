// Behaviour test for 0x0085FA00 NCreatureMode::IsToEvadeAllHits(const CThing& thing).
#include <stdio.h>
struct CAction { bool Evade; bool IsToEvadeAllHits() const { return Evade; } };
class CThing { public:
    unsigned long Flags; CAction* Action; bool PhysicalEvade, KnockedDown, Underground, HasModes, Intangible, BriarIntangible;
};
static bool IsKnockedDown(const CThing& t) { return t.KnockedDown; }
static bool IsUnderground(const CThing& t) { return t.Underground; }
static bool IsToEvadeAllHits(const CThing& thing)
{
    if (thing.Flags & 8) return thing.Action->IsToEvadeAllHits();
    if ((thing.Flags & 2) && thing.PhysicalEvade) return true;
    if (IsKnockedDown(thing)) return true;
    if (IsUnderground(thing)) return true;
    if (thing.HasModes) return thing.Intangible || thing.BriarIntangible;
    return false;
}
int main()
{
    CAction act = { true };
    CThing t = { 0, &act, false, false, false, false, false, false };
    int ok = !IsToEvadeAllHits(t);
    t.Flags = 8; ok = ok && IsToEvadeAllHits(t); act.Evade = false; ok = ok && !IsToEvadeAllHits(t);
    t.Flags = 2; t.PhysicalEvade = true; ok = ok && IsToEvadeAllHits(t);
    t.PhysicalEvade = false; t.KnockedDown = true; ok = ok && IsToEvadeAllHits(t);
    t.KnockedDown = false; t.Underground = true; ok = ok && IsToEvadeAllHits(t);
    t.Underground = false; t.HasModes = true; ok = ok && !IsToEvadeAllHits(t);
    t.BriarIntangible = true; ok = ok && IsToEvadeAllHits(t);
    t.BriarIntangible = false; t.Intangible = true; ok = ok && IsToEvadeAllHits(t);
    t.Flags = 0; t.HasModes = false; ok = ok && !IsToEvadeAllHits(t);
    printf(ok ? "0085fa00_TEST PASS\n" : "0085fa00_TEST FAIL\n");
    return ok ? 0 : 1;
}