// Behaviour test for CTCTrespasser::IsCurrentlyTrespassing (0x007BAE70): the owner's building (interface 117) must be live
// and carry a trespassable component (interface 189) whose IsBeingTrespassedIn answers.
#include <cstdio>
struct Building { bool dead; bool hasTrespassable; bool beingTrespassedIn; };
struct Owner { bool hasLocation; Building* building; };
static bool IsCurrentlyTrespassing(const Owner& o)
{
    if (o.hasLocation) {
        Building* b = o.building;
        if (b && !b->dead && b->hasTrespassable) return b->beingTrespassedIn;
    }
    return false;
}
int main()
{
    Building live = { false, true, true }, dead = { true, true, true }, plain = { false, false, true };
    Owner a = { true, &live }, b = { true, &dead }, c = { true, &plain }, d = { false, &live }, e = { true, 0 };
    if (!IsCurrentlyTrespassing(a) || IsCurrentlyTrespassing(b) || IsCurrentlyTrespassing(c) || IsCurrentlyTrespassing(d) ||
        IsCurrentlyTrespassing(e)) { std::printf("007bae70_TEST FAIL\n"); return 1; }
    std::printf("007bae70_TEST PASS\n");
    return 0;
}