// Behaviour test for CTCTargetingPlayer::GetPBestThingForMeleeAttack (0x008031E0): a model of the row -- a missing or dead target is
// returned as is; while Z-targeting only the locked target qualifies; otherwise a Z-targetable, smashable or enemy target qualifies.
#include <stdio.h>
struct Thing { bool dead; bool zTargetable; bool smashable; bool enemy; };
static Thing* Best(Thing* target, bool zTargeting, Thing* locked)
{
    if (!target || target->dead) return target;
    if (zTargeting) { if (locked != target) return 0; }
    else { if (target->zTargetable) return target; if (target->smashable) return target; if (!target->enemy) return 0; }
    return target;
}
int main()
{
    Thing dead = { true, false, false, false }, plain = { false, false, false, false }, foe = { false, false, false, true }, box = { false, false, true, false };
    int ok = Best(0, false, 0) == 0 && Best(&dead, false, 0) == &dead && Best(&plain, false, 0) == 0 && Best(&foe, false, 0) == &foe
          && Best(&box, false, 0) == &box && Best(&foe, true, &plain) == 0 && Best(&plain, true, &plain) == &plain;
    printf(ok ? "008031e0_TEST PASS\n" : "008031e0_TEST FAIL\n");
    return ok ? 0 : 1;
}
