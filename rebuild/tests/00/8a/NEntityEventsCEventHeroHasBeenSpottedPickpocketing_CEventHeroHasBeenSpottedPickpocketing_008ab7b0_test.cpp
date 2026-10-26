// behaviour test for 0x008AB7B0 NEntityEvents::CEventHeroHasBeenSpottedPickpocketing::CEventHeroHasBeenSpottedPickpocketing(CThing*, CThing*): WorldFrame starts at 0 and the two members
// point at the first and the second argument, in that order
#include <cstdio>
struct Ptr { void* Item; };
struct Event { long WorldFrame; Ptr First; Ptr Second; };
static void Construct(Event* e, void* a, void* b)            // the function under test
{
    e->WorldFrame = 0;
    e->First.Item = a;
    e->Second.Item = b;
}
int main()
{
    int fail = 0;
    int x = 1, y = 2;
    Event e; e.WorldFrame = 77; e.First.Item = 0; e.Second.Item = 0;
    Construct(&e, &x, &y);
    if (e.WorldFrame != 0) fail |= 1;
    if (e.First.Item != &x || e.Second.Item != &y) fail |= 2;
    Construct(&e, 0, &x);
    if (e.First.Item != 0 || e.Second.Item != &x) fail |= 4;
    if (sizeof(Ptr) != sizeof(void*)) fail |= 8;
    if (fail) { std::printf("008ab7b0_TEST FAIL %d\n", fail); return 1; }
    std::printf("008ab7b0_TEST PASS\n");
    return 0;
}