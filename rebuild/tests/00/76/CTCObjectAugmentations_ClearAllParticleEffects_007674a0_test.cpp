// Behaviour test for CTCObjectAugmentations::ClearAllParticleEffects (0x007674A0): kill the live trail and reset it, kill
// every live running effect and attack particle (dead or null ones are left alone), then empty both particle tables.
#include <cstdio>

struct Thing { bool dead; int kills; };
struct Ptr { Thing* item; };
static bool Alive(const Ptr& p) { if (p.item != 0) { if (!p.item->dead) return true; } return false; }
struct Aug {
    Ptr trail;
    Ptr running[3]; int runningCount;
    Ptr attack[2][2]; int attackInner[2]; int attackCount;
    int particleCount;
};
static void ClearAllParticleEffects(Aug& a)
{
    if (Alive(a.trail)) { a.trail.item->kills++; a.trail.item = 0; }
    for (long i = 0; i < a.runningCount; ++i)
        if (Alive(a.running[i])) a.running[i].item->kills++;
    for (int o = 0; o < a.attackCount; ++o)
        for (long j = 0; j < a.attackInner[o]; ++j)
            if (Alive(a.attack[o][j])) a.attack[o][j].item->kills++;
    a.particleCount = 0;
    a.attackCount = 0;
}
int main()
{
    Thing t = { false, 0 }, r0 = { false, 0 }, r1 = { true, 0 }, p0 = { false, 0 }, p1 = { false, 0 };
    Aug a;
    a.trail.item = &t;
    a.running[0].item = &r0; a.running[1].item = &r1; a.running[2].item = 0; a.runningCount = 3;
    a.attack[0][0].item = &p0; a.attack[0][1].item = 0; a.attack[1][0].item = &p1; a.attack[1][1].item = &r1;
    a.attackInner[0] = 2; a.attackInner[1] = 2; a.attackCount = 2; a.particleCount = 4;
    ClearAllParticleEffects(a);
    if (t.kills != 1 || a.trail.item != 0 || r0.kills != 1 || r1.kills != 0 || p0.kills != 1 || p1.kills != 1 ||
        a.attackCount != 0 || a.particleCount != 0) { std::printf("007674a0_TEST FAIL\n"); return 1; }
    ClearAllParticleEffects(a);
    if (t.kills != 1 || r0.kills != 2) { std::printf("007674a0_TEST FAIL\n"); return 1; }
    std::printf("007674a0_TEST PASS\n");
    return 0;
}