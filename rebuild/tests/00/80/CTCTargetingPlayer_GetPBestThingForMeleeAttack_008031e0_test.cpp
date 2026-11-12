// Behaviour test for 0x008031E0 CTCTargetingPlayer::GetPBestThingForMeleeAttack: the same decision logic over a small model
// (thing flags, the targeted component's Z-targetable flag, the player's Z-targeting state and an enemy relation).
#include <cstdio>

struct Thing {
    bool dead;
    bool hasTargeted;
    unsigned long targetingFlags;
    bool smashable;
    int  faction;
    bool zTargeting;                 // the owning player's state (read through the parent thing)
};
static int g_enemyQueries = 0;
static bool AreThingsEnemies(const Thing& a, const Thing& b) { ++g_enemyQueries; return a.faction != b.faction; }

struct Targeting {
    Thing* parent;
    Thing* targeted;
    Thing* locked;
    Thing* GetPBestThingForMeleeAttack() const
    {
        Thing* p_thing = targeted;
        if (p_thing && !p_thing->dead) {
            if (parent->zTargeting) {
                if (locked != p_thing)
                    return 0;
            } else {
                if (p_thing->hasTargeted && (p_thing->targetingFlags & 4))
                    return p_thing;
                if (p_thing->smashable)
                    return p_thing;
                const Thing& self = *parent;
                if (!AreThingsEnemies(self, *p_thing))
                    return 0;
                if (p_thing == &self)
                    return p_thing;
            }
        }
        return p_thing;
    }
};

int main()
{
    Thing hero   = { false, false, 0, false, 1, false };
    Thing bandit = { false, true, 0, false, 2, false };
    Thing friendly = { false, true, 0, false, 1, false };
    Thing dummy  = { false, true, 4, false, 1, false };
    Thing barrel = { false, false, 0, true, 1, false };
    Thing corpse = { true, true, 0, false, 1, false };
    Targeting t = { &hero, 0, 0 };
    bool ok = true;
    ok = ok && t.GetPBestThingForMeleeAttack() == 0;                       // nothing targeted
    t.targeted = &corpse;   ok = ok && t.GetPBestThingForMeleeAttack() == &corpse;   // a dead target is returned as it is
    t.targeted = &bandit;   ok = ok && t.GetPBestThingForMeleeAttack() == &bandit;   // enemy
    t.targeted = &friendly; ok = ok && t.GetPBestThingForMeleeAttack() == 0;         // not an enemy, not targetable, not smashable
    g_enemyQueries = 0;
    t.targeted = &dummy;    ok = ok && t.GetPBestThingForMeleeAttack() == &dummy;    // Z-targetable: accepted before the enemy query
    t.targeted = &barrel;   ok = ok && t.GetPBestThingForMeleeAttack() == &barrel;   // smashable: likewise
    ok = ok && g_enemyQueries == 0;
    friendly.targetingFlags = 3;                                           // other flags do not make it Z-targetable
    t.targeted = &friendly; ok = ok && t.GetPBestThingForMeleeAttack() == 0;
    ok = ok && g_enemyQueries == 1;
    hero.zTargeting = true;                                                // while Z-targeting only the locked thing counts
    t.targeted = &bandit; t.locked = &friendly; ok = ok && t.GetPBestThingForMeleeAttack() == 0;
    t.locked = &bandit;     ok = ok && t.GetPBestThingForMeleeAttack() == &bandit;
    t.targeted = &friendly; t.locked = &friendly; ok = ok && t.GetPBestThingForMeleeAttack() == &friendly;
    t.targeted = &corpse; t.locked = 0; ok = ok && t.GetPBestThingForMeleeAttack() == &corpse;
    ok = ok && g_enemyQueries == 1;                                        // no enemy query on the Z-targeting path
    if (!ok) { std::printf("008031e0_TEST FAIL\n"); return 1; }
    std::printf("008031e0_TEST PASS\n");
    return 0;
}