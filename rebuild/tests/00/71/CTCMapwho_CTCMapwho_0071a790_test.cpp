// Behaviour test for CTCMapwho::~CTCMapwho (0x0071A790): a thing still in the mapwho is removed from the thing-map
// manager; one that is not is left alone.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
struct Manager { int removed; void RemoveThing(int*) { ++removed; } };
static Manager g_manager;
struct Mapwho { bool InMapwho; void RemoveFromMapwho() { if (InMapwho) g_manager.RemoveThing(0); } ~Mapwho() { RemoveFromMapwho(); } };
int main()
{
    { Mapwho a; a.InMapwho = true; }
    { Mapwho b; b.InMapwho = false; }
    bool ok = g_manager.removed == 1;
    printf(ok ? "0071a790_TEST PASS\n" : "0071a790_TEST FAIL\n");
    return ok ? 0 : 1;
}