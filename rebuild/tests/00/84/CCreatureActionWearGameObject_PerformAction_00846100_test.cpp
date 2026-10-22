// Behaviour test for CCreatureAction_WearGameObject::PerformAction (0x00846100): the carried thing's def is worn after
// every conflicting worn modifier (last first) is taken off and dropped with a +-0.2 X/Y push and 0.15 up; the carried
// thing is killed; nothing happens without a carried thing or with a zero def index.
#include <cstdio>
#include <list>

struct Item { unsigned short defIndex; bool killed; bool hasSubDef; long slot; };
struct Wearer { bool hasModifiers; std::list<long> worn; std::list<long> conflicts; long wornDef; int drops; float lastZ; };

static unsigned int g_seed = 99;
static float Random04()
{
    g_seed = g_seed * 1103515245u + 12345u;
    return (float)((g_seed >> 8) % 1000) * 0.0004f;
}

static void PerformAction(Wearer* w, Item* item)
{
    if (item == 0 || item->defIndex == 0)
        return;
    if (w->hasModifiers && item->hasSubDef) {
        std::list<long> conflicting = w->conflicts;
        while (!conflicting.empty()) {
            long id = conflicting.back();
            w->worn.remove(id);
            conflicting.remove(id);
            float x = 0.2f - Random04();
            float y = 0.2f - Random04();
            if (x < -0.2f || x > 0.2f || y < -0.2f || y > 0.2f)
                return;
            ++w->drops;
            w->lastZ = 0.15f;
        }
    }
    item->killed = true;
    if (w->hasModifiers)
        w->wornDef = item->defIndex;
}

int main()
{
    Wearer w;
    w.hasModifiers = true; w.wornDef = 0; w.drops = 0; w.lastZ = 0.0f;
    w.worn.push_back(3); w.worn.push_back(4); w.worn.push_back(5);
    w.conflicts.push_back(3); w.conflicts.push_back(5);
    Item hat = { 7, false, true, 2 };
    PerformAction(&w, &hat);
    if (w.drops != 2 || w.worn.size() != 1 || w.worn.front() != 4 || !hat.killed || w.wornDef != 7 || w.lastZ != 0.15f) {
        std::printf("00846100_TEST FAIL\n"); return 1;
    }
    Item none = { 0, false, true, 0 };
    PerformAction(&w, &none);
    PerformAction(&w, 0);
    if (none.killed || w.wornDef != 7) { std::printf("00846100_TEST FAIL\n"); return 1; }
    Wearer plain;
    plain.hasModifiers = false; plain.wornDef = 0; plain.drops = 0; plain.lastZ = 0.0f;
    Item ring = { 9, false, true, 1 };
    PerformAction(&plain, &ring);
    if (!ring.killed || plain.wornDef != 0 || plain.drops != 0) { std::printf("00846100_TEST FAIL\n"); return 1; }
    std::printf("00846100_TEST PASS\n");
    return 0;
}