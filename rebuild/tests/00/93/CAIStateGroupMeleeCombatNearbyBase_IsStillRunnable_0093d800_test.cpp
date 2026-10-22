// Behaviour test for CAIStateGroup_MeleeCombatNearbyBase::IsStillRunnable (0x0093D800): model copy, scripted stubs.
#include <cstdio>

struct CThing { unsigned char m_flags91; float x, y, z; bool engageable; bool hasStealth; int stealthFlagged; };
struct Creature {
    bool engaged;
    CThing* target;
    bool hasPerceive;
    CThing* perceived;
    bool canSee, direct;
    bool combatant;
    float range;
    CThing self;
};
static Creature g_c;
static CThing* g_enemy;
static bool g_runningAway;

static bool IsStillRunnable(bool checkCombatant)
{
    if (g_enemy && !(g_enemy->m_flags91 & 1)) {
        if (g_c.engaged && g_enemy->engageable) {
            CThing* target = g_c.target;
            if (g_enemy == target) {
                bool result = true;
                if (g_c.hasPerceive && g_c.perceived == g_enemy)
                    result = g_c.canSee || g_c.direct;
                if (result) {
                    if (checkCombatant)
                        result = g_c.combatant;
                    if (result) {
                        float dx = g_enemy->x - g_c.self.x, dy = g_enemy->y - g_c.self.y, dz = g_enemy->z - g_c.self.z;
                        float limit = g_c.range;
                        if (dx * dx + dy * dy + dz * dz > limit * limit)
                            result = false;
                        else if (g_runningAway) {
                            if (g_enemy->hasStealth)
                                ++g_enemy->stealthFlagged;
                            result = false;
                        }
                    }
                }
                return result;
            }
        }
    }
    return false;
}

int main()
{
    int ok = 1;
    CThing enemy = { 0, 3.0f, 4.0f, 0.0f, true, true, 0 };
    g_enemy = &enemy;
    g_c.engaged = true; g_c.target = &enemy; g_c.hasPerceive = true; g_c.perceived = &enemy;
    g_c.canSee = true; g_c.direct = false; g_c.combatant = true; g_c.range = 5.0f;
    g_c.self.x = 0; g_c.self.y = 0; g_c.self.z = 0;
    if (!IsStillRunnable(true)) ok = 0;            // exactly at range: still runnable
    g_c.range = 4.9f;
    if (IsStillRunnable(true)) ok = 0;             // out of range
    g_c.range = 6.0f;
    g_c.canSee = false;
    if (IsStillRunnable(false)) ok = 0;            // lost track of the perceived enemy
    g_c.direct = true;
    if (!IsStillRunnable(false)) ok = 0;
    g_c.perceived = 0; g_c.canSee = false; g_c.direct = false;
    if (!IsStillRunnable(false)) ok = 0;           // perceiving something else: no check
    g_c.combatant = false;
    if (IsStillRunnable(true) || !IsStillRunnable(false)) ok = 0;
    g_runningAway = true;
    if (IsStillRunnable(false) || enemy.stealthFlagged != 1) ok = 0;
    g_runningAway = false;
    g_c.target = 0;
    if (IsStillRunnable(false)) ok = 0;
    g_c.target = &enemy; enemy.engageable = false;
    if (IsStillRunnable(false)) ok = 0;
    enemy.engageable = true; g_c.engaged = false;
    if (IsStillRunnable(false)) ok = 0;
    g_c.engaged = true; enemy.m_flags91 = 1;
    if (IsStillRunnable(false)) ok = 0;
    if (!ok) { std::printf("0093d800_TEST FAIL\n"); return 1; }
    std::printf("0093d800_TEST PASS\n");
    return 0;
}