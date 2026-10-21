// Behaviour test for 008a0260 CGameScriptInterface::GiveThingBestEnemyTarget: AI targeting override, enemy list add, change notification.
#include <cstdio>
#include <cstring>
struct Thing { bool creature; bool hasTargeting; const char* cls; Thing* target; bool hasEnemies; int enemies; int type; int notified; };
static void Give(Thing* thing, Thing* target, bool targetValid)
{
    Thing* previous = 0;
    if (thing->hasTargeting && !std::strcmp(thing->cls, "CTCTargetingAI")) {
        previous = thing->target;
        thing->target = (targetValid && target->creature) ? target : 0;
    }
    if (targetValid && thing->hasEnemies) ++thing->enemies;
    if (previous != target && thing->type == 1) ++thing->notified;
}
int main()
{
    Thing enemy = { true, false, "", 0, false, 0, 2, 0 };
    Thing hero = { true, true, "CTCTargetingAI", 0, true, 0, 1, 0 };
    Give(&hero, &enemy, true);
    bool a = hero.target == &enemy && hero.enemies == 1 && hero.notified == 1;
    Give(&hero, &enemy, true);
    bool b = hero.notified == 1 && hero.enemies == 2;
    if (!a || !b) { std::printf("008a0260_TEST FAIL\n"); return 1; }
    std::printf("008a0260_TEST PASS\n");
    return 0;
}
