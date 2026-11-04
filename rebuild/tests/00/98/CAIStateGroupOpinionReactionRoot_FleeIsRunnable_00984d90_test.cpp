// Behaviour test for CAIStateGroup_OpinionReactionRoot::FleeIsRunnable (0x00984D90): a model of the row -- when the thing reacted to
// is a creature (type 2) and the reacting creature is a village member whose village is not hostile over crime and whose creature
// type is not 1, a region whose def forbids both hero weapons and hero Will means "do not flee"; otherwise flee only within 12 units.
#include <stdio.h>
#include <math.h>
struct RegionDef { bool PreventHeroWeaponUse, PreventHeroWillUse; };
struct Village { bool IsEnemyBecauseOfCrime; };
struct Thing { int Type; float X, Y, Z; Village* village; int creatureType; RegionDef* regionDef; };
static bool FleeIsRunnable(const Thing& me, const Thing& other)
{
    if (other.Type == 2 && me.village) {
        Village* v = me.village;
        if (v && !v->IsEnemyBecauseOfCrime && me.creatureType != 1) {
            bool safe = me.regionDef && me.regionDef->PreventHeroWeaponUse && me.regionDef->PreventHeroWillUse;
            if (safe) return false;
        }
    }
    float dx = other.X - me.X, dy = other.Y - me.Y, dz = other.Z - me.Z;
    return (float)sqrt(dx * dx + dy * dy + dz * dz) < 12.0f;
}
int main()
{
    RegionDef safe = { true, true }, half = { true, false };
    Village calm = { false }, angry = { true };
    Thing other = { 2, 5, 0, 0, 0, 0, 0 };
    Thing a = { 2, 0, 0, 0, &calm, 0, &safe };     // safe region: no flee even though close
    Thing b = { 2, 0, 0, 0, &calm, 0, &half };     // not fully safe: close, flees
    Thing c = { 2, 0, 0, 0, &angry, 0, &safe };    // hostile village: flees when close
    Thing d = { 2, 20, 0, 0, 0, 0, 0 };            // no village, far: no flee
    Thing e = { 2, 0, 0, 0, &calm, 1, &safe };     // creature type 1 ignores the region
    int ok = !FleeIsRunnable(a, other) && FleeIsRunnable(b, other) && FleeIsRunnable(c, other) && !FleeIsRunnable(d, other)
          && FleeIsRunnable(e, other);
    printf(ok ? "00984d90_TEST PASS\n" : "00984d90_TEST FAIL\n");
    return ok ? 0 : 1;
}