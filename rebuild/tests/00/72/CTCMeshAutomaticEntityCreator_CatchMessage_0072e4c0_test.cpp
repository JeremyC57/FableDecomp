#include <cstdio>
// Behaviour model of CTCMeshAutomaticEntityCreator::CatchMessage (0x0072E4C0): every live owned thing carrying the
// tracked interface is killed, then the list is pruned and the children recreated.
struct Thing { bool dead; bool hasTC; int kills; };
int main()
{
    Thing a = { false, true, 0 }, b = { true, true, 0 }, c = { false, false, 0 };
    Thing* list[4] = { &a, &b, &c, 0 };
    int pruned = 0, created = 0;
    bool hasOwner = true;
    if (hasOwner) {
        for (int i = 0; i < 4; ++i) {
            Thing* t = list[i];
            if (t && !t->dead && t->hasTC) t->kills++;
        }
        pruned++;
    }
    created++;
    if (a.kills != 1 || b.kills || c.kills || pruned != 1 || created != 1) { std::printf("0072e4c0_TEST FAIL\n"); return 1; }
    std::printf("0072e4c0_TEST PASS\n");
    return 0;
}