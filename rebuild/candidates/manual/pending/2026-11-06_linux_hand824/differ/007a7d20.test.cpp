// Behaviour test for CTCBattleCharge::CreateHitEffect (0x007A7D20), the damage rule: every creature in range except the charger,
// the incapacitated and those already hit takes the level's damage, scaled by (1 - distance / radius) unless it is the collided
// thing, where the radius is the dragon radius (DragonExtraGumphRadiusMultiplier * RadiusOfEffect + DragonExtraGumphRadius) for
// CREATURE_DRAGON_01 and RadiusOfEffect otherwise; game objects must be enemies of the attacker (the z-targetted live target, which
// itself is always hit, else the charger). A model.
#include <cstdio>
#include <cmath>
struct Victim { float X, Y, Z; bool Self, Incapacitated, AlreadyHit, Collided, Dragon, GameObject, Enemy, IsTarget; };
float Damage(const Victim& v, float base, float radius, float mult, float extra, bool zTarget, bool targetEnemy, bool* hit)
{
    *hit = false;
    if (v.Self || v.Incapacitated || v.AlreadyHit) return 0.0f;
    float damage = base;
    if (!v.Collided) {
        float dist = (float)std::sqrt(v.Z * v.Z + v.Y * v.Y + v.X * v.X);
        float r = v.Dragon ? mult * radius + extra : radius;
        damage *= 1.0f - dist / r;
    }
    if (v.GameObject) {
        if (zTarget) { if (!v.IsTarget && !targetEnemy) return 0.0f; }
        else if (!v.Enemy) return 0.0f;
    }
    *hit = true;
    return damage;
}
int main()
{
    bool hit;
    Victim a = { 3.0f, 4.0f, 0.0f, false, false, false, false, false, true, true, false };
    bool ok = Damage(a, 10.0f, 10.0f, 2.0f, 0.0f, false, false, &hit) == 5.0f && hit;
    Victim d = { 3.0f, 4.0f, 0.0f, false, false, false, false, true, true, true, false };
    ok = ok && Damage(d, 10.0f, 5.0f, 2.0f, 0.0f, false, false, &hit) == 5.0f && hit;
    Victim c = { 3.0f, 4.0f, 0.0f, false, false, false, true, false, true, true, false };
    ok = ok && Damage(c, 10.0f, 10.0f, 2.0f, 0.0f, false, false, &hit) == 10.0f && hit;
    Victim f = { 1.0f, 0.0f, 0.0f, false, false, false, false, false, true, false, false };
    ok = ok && Damage(f, 10.0f, 10.0f, 2.0f, 0.0f, false, false, &hit) == 0.0f && !hit;
    Victim t = { 1.0f, 0.0f, 0.0f, false, false, false, true, false, true, false, true };
    ok = ok && Damage(t, 10.0f, 10.0f, 2.0f, 0.0f, true, false, &hit) == 10.0f && hit;
    Victim s = { 0.0f, 0.0f, 0.0f, true, false, false, false, false, false, false, false };
    Damage(s, 10.0f, 10.0f, 2.0f, 0.0f, false, false, &hit);
    ok = ok && !hit;
    Victim i = { 0.0f, 0.0f, 0.0f, false, true, false, false, false, false, false, false };
    Damage(i, 10.0f, 10.0f, 2.0f, 0.0f, false, false, &hit);
    ok = ok && !hit;
    std::printf(ok ? "007a7d20_TEST PASS\n" : "007a7d20_TEST FAIL\n");
    return ok ? 0 : 1;
}
