// behaviour test for CAIStateGroup_TrollBase::HasEnemyBeenKnockedDown (0x0094D9C0): logic copy over stubs
#include <cstdio>
struct Thing { unsigned char flags91; };
struct Event { long frame; const Thing* attacker; bool explosion; bool knockedDown; };
struct Events {
    Event* list; int count;
    bool FindKnockedDownBy(Event** out, long after, const Thing* by)
    { for (int i = 0; i < count; i++) if (list[i].frame > after && list[i].knockedDown && list[i].attacker == by) { *out = &list[i]; return true; } return false; }
    bool FindExplosion(Event** out, long after)
    { for (int i = 0; i < count; i++) if (list[i].frame > after && list[i].explosion) { *out = &list[i]; return true; } return false; }
};
struct Troll {
    Thing* enemy; Events* enemyEvents; Thing* self; long knockDownFrame;
    bool HasEnemyBeenKnockedDown()
    {
        if (enemy && !(enemy->flags91 & 1)) {
            { Event* hit; if (enemyEvents->FindKnockedDownBy(&hit, knockDownFrame, self)) { knockDownFrame = hit->frame; return true; } }
            { Event* hit; if (enemyEvents->FindExplosion(&hit, knockDownFrame)) { knockDownFrame = hit->frame; return true; } }
        }
        return false;
    }
};
int main()
{
    Thing me = { 0 }, other = { 0 }, foe = { 0 };
    Event evs[3] = { { 10, &other, false, true }, { 20, &me, false, true }, { 30, 0, true, false } };
    Events e = { evs, 3 };
    Troll t = { &foe, &e, &me, 0 };
    bool ok = t.HasEnemyBeenKnockedDown() && t.knockDownFrame == 20;
    if (!t.HasEnemyBeenKnockedDown() || t.knockDownFrame != 30) ok = false;      // explosion after our knock-down
    if (t.HasEnemyBeenKnockedDown()) ok = false;                                // nothing newer
    foe.flags91 = 1; t.knockDownFrame = 0;
    if (t.HasEnemyBeenKnockedDown()) ok = false;                                // dead enemy
    if (!ok) { std::printf("0094d9c0_TEST FAIL\n"); return 1; }
    std::printf("0094d9c0_TEST PASS\n");
    return 0;
}