#include <cstdio>
#include <cmath>
// Behaviour test for CCombatSequence_WillForcePush::IsValid @ 0x009098E0.
struct V { float X, Y, Z; };
struct Thing { V pos; int team; };
static bool AreThingsEnemies(const Thing* a, const Thing* b) { return a->team != b->team; }
static bool IsValid(bool hasWillPower, bool active, const Thing* hero, const Thing* owner)
{
    if (hasWillPower) {
        if (active) {
            if (!AreThingsEnemies(hero, owner)) {
                float dx = hero->pos.X - owner->pos.X, dy = hero->pos.Y - owner->pos.Y, dz = hero->pos.Z - owner->pos.Z;
                if ((float)std::sqrt(dx * dx + (dy * dy + dz * dz)) < 8.0f) return false;
            }
            return true;
        }
        return false;
    }
    return true;
}
static int Fail() { std::printf("009098e0_TEST FAIL\n"); return 1; }
int main()
{
    Thing hero = {{0, 0, 0}, 1}, near = {{3, 4, 0}, 1}, far = {{6, 8, 0}, 1}, foe = {{1, 0, 0}, 2};
    if (!IsValid(false, false, &hero, &near)) return Fail();
    if (IsValid(true, false, &hero, &far)) return Fail();
    if (IsValid(true, true, &hero, &near)) return Fail();
    if (!IsValid(true, true, &hero, &far)) return Fail();
    if (!IsValid(true, true, &hero, &foe)) return Fail();
    std::printf("009098e0_TEST PASS\n");
    return 0;
}
