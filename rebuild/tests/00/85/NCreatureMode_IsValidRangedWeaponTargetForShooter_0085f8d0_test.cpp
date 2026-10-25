// Behaviour test for NCreatureMode::IsValidRangedWeaponTargetForShooter (0x0085F8D0): a locked-on player needs a live
// targeted thing; otherwise the normalised direction to the target must be within `angle` degrees of the facing
// (dot >= cos(angle)); a coincident target keeps the zero offset (dot 0).
#include <cstdio>
#include <cmath>

struct Vec { float X, Y, Z; };
struct Thing {
    bool hasTargeting;
    Thing* targeted;
    bool dead;
    bool lockedOn;
    Vec position;
    Vec facing;
};

static bool IsValid(const Thing& shooter, const Thing& target, float angle)
{
    const Thing* targeted = 0;
    if (shooter.hasTargeting)
        targeted = shooter.targeted;
    if (shooter.lockedOn) {
        if (!targeted || targeted->dead)
            return false;
    }
    Vec o = { target.position.X - shooter.position.X, target.position.Y - shooter.position.Y,
              target.position.Z - shooter.position.Z };
    float lenSq = o.X * o.X + o.Y * o.Y + o.Z * o.Z;
    if (lenSq > 0.0001f * 0.0001f) {
        float len = std::sqrt(lenSq);
        o.X /= len; o.Y /= len; o.Z /= len;
    }
    const Vec& f = shooter.facing;
    if (f.X * o.X + f.Y * o.Y + f.Z * o.Z < std::cos(angle / 360.0f * 6.2831854820251465))
        return false;
    return true;
}

int main()
{
    int fails = 0;
    Thing other = { false, 0, false, false, { 0, 0, 0 }, { 1, 0, 0 } };
    Thing shooter = { true, &other, false, false, { 0, 0, 0 }, { 1, 0, 0 } };
    Thing ahead = { false, 0, false, false, { 10, 1, 0 }, { 1, 0, 0 } };     // ~5.7 degrees off the facing
    Thing side = { false, 0, false, false, { 1, 2, 0 }, { 1, 0, 0 } };      // ~63 degrees off
    Thing behind = { false, 0, false, false, { -5, 0, 0 }, { 1, 0, 0 } };
    Thing same = { false, 0, false, false, { 0, 0, 0 }, { 1, 0, 0 } };
    if (!IsValid(shooter, ahead, 45.0f)) ++fails;
    if (IsValid(shooter, side, 45.0f)) ++fails;
    if (!IsValid(shooter, side, 70.0f)) ++fails;
    if (IsValid(shooter, behind, 170.0f)) ++fails;
    if (!IsValid(shooter, same, 90.0f)) ++fails;          // dot 0 >= cos(90) ~ -4e-8
    if (IsValid(shooter, same, 45.0f)) ++fails;
    shooter.lockedOn = true;
    if (!IsValid(shooter, ahead, 45.0f)) ++fails;        // locked on, targeted thing alive
    other.dead = true;
    if (IsValid(shooter, ahead, 45.0f)) ++fails;         // locked on, targeted thing dead
    other.dead = false;
    shooter.hasTargeting = false;
    if (IsValid(shooter, ahead, 45.0f)) ++fails;         // locked on, no targeting component
    shooter.lockedOn = false;
    if (!IsValid(shooter, ahead, 45.0f)) ++fails;        // not locked on: no targeted thing needed
    if (fails) { std::printf("0085f8d0_TEST FAIL\n"); return 1; }
    std::printf("0085f8d0_TEST PASS\n");
    return 0;
}