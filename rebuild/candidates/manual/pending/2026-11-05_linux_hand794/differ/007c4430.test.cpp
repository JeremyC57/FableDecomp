// Behaviour test for CTCHealLife::StartHealingEffect (0x007C4430): the level picks the looping sound; the target is the Z-targeted live
// thing (else the caster), falling back to the caster for an enemy; a live creature target gets one heal entry, effect slot and 0.0f
// accumulator; a self-cast adds every creature in range except the target, each with its slots, effects only for live non-enemies.
#include <cstdio>
#include <cstring>
struct Thing { int id; bool creature, dead, enemy, inRange; };
struct Heal {
    int sound; int target; int heal[16]; int nHeal; int effects[16]; float accum[16]; int nSlots; int events;
    void Start(int level, Thing* things, int n, int caster, int zTarget, bool zTargeting)
    {
        static const int sounds[4] = { 1, 2, 3, 4 };
        sound = level >= 0 && level < 4 ? sounds[level] : 0;
        nHeal = nSlots = events = 0;
        target = caster;
        if (zTarget >= 0 && !things[zTarget].dead && zTargeting) target = zTarget;
        if (things[target].enemy) target = caster;
        Thing& t = things[target];
        if (!t.creature || t.dead) return;
        heal[nHeal++] = target; effects[nSlots] = target; accum[nSlots++] = 0.0f; ++events;
        if (target != caster) return;
        for (int i = 0; i < n; ++i)
            if (things[i].creature && things[i].inRange && i != target) heal[nHeal++] = i;
        for (int k = 0; k < nHeal; ++k) {
            int c = heal[k];
            if (c == target) continue;
            effects[nSlots] = -1; accum[nSlots++] = 0.0f;
            if (!things[c].dead && !things[c].enemy) { effects[nSlots - 1] = c; ++events; }
        }
    }
};
int main()
{
    Thing t[5] = { { 0, true, false, false, true }, { 1, true, false, false, true }, { 2, true, false, true, true },
                   { 3, true, true, false, true }, { 4, true, false, false, false } };
    Heal h; h.Start(2, t, 5, 0, -1, false);
    bool ok = h.sound == 3 && h.target == 0 && h.nHeal == 4 && h.nSlots == 4 && h.events == 2 && h.effects[1] == 1 && h.effects[2] == -1
        && h.effects[3] == -1 && h.accum[3] == 0.0f;
    Heal g; g.Start(0, t, 5, 0, 1, true);
    ok = ok && g.sound == 1 && g.target == 1 && g.nHeal == 1 && g.events == 1;
    Heal e; e.Start(1, t, 5, 0, 2, true);
    ok = ok && e.target == 0 && e.nHeal == 4;
    if (!ok) { std::printf("007c4430_TEST FAIL\n"); return 1; }
    std::printf("007c4430_TEST PASS\n");
    return 0;
}
