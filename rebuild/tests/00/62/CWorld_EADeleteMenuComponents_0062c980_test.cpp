// behaviour test for 0x0062C980 CWorld::EADeleteMenuComponents: the hero's component is looked up and called only when the hero has interface 0x11
#include <cstdio>
static int gCalls = 0; static void* gThis = 0;
struct Comp { int tag; void Method() { ++gCalls; gThis = this; } };
struct Entry { long type; Comp* component; };
struct Thing {
    unsigned long Using[9];
    Entry* first; Entry* last;
    Entry* LowerBound(long key) { Entry* p = first; while (p != last && p->type < key) ++p; return p; }
    bool HasTC(long type) const { return (Using[type >> 5] & (1UL << (type & 31))) != 0; }
};
struct Player { Thing* hero; Thing* GetHeroThing() { return hero; } };
struct Event { long Type; signed char Player; };
struct World {
    Player players[2];
    Player* GetPlayer(long n) { return &players[n]; }
    void Action(const Event* event)                         // the function under test
    {
        Thing* hero = GetPlayer(event->Player)->GetHeroThing();
        if (hero->HasTC(0x11))
        {
            long key = 0x11;
            Entry* it = hero->LowerBound(key);
            if (it == hero->last || key < it->type) it = hero->last;
            it->component->Method();
        }
    }
};
int main()
{
    int fail = 0, i;
    Comp a, b; a.tag = 1; b.tag = 2;
    Entry e[3]; e[0].type = 3; e[0].component = &b; e[1].type = 0x11; e[1].component = &a; e[2].type = 0x60; e[2].component = &b;
    Thing h; for (i = 0; i < 9; ++i) h.Using[i] = 0; h.first = e; h.last = e + 3;
    Thing none = h;
    h.Using[0x11 >> 5] |= 1UL << (0x11 & 31);
    World w; w.players[0].hero = &none; w.players[1].hero = &h;
    Event ev; ev.Type = 0; ev.Player = 1;
    w.Action(&ev);
    if (gCalls != 1 || gThis != &a) fail |= 1;
    ev.Player = 0;
    w.Action(&ev);
    if (gCalls != 1) fail |= 2;
    if (fail) { std::printf("0062c980_TEST FAIL %d\n", fail); return 1; }
    std::printf("0062c980_TEST PASS\n");
    return 0;
}