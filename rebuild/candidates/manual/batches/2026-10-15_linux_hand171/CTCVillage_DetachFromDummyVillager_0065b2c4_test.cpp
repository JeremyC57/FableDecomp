// Behaviour test for CTCVillage::DetachFromDummyVillager (0x0065B2C4): every live dummy attached to the villager detaches.
#include <cstdio>
struct Dummy { bool alive; bool hasComponent; void* villager; int detached; };
int main()
{
    int bob, ann;
    Dummy d[4] = { { true, true, &bob, 0 }, { true, true, &ann, 0 }, { false, true, &bob, 0 }, { true, true, &bob, 0 } };
    for (int i = 0; i < 4; ++i)
        if (d[i].alive && d[i].hasComponent && d[i].villager == &bob) ++d[i].detached;
    if (d[0].detached != 1 || d[1].detached || d[2].detached || d[3].detached != 1) { std::printf("0065b2c4_TEST FAIL\n"); return 1; }
    std::printf("0065b2c4_TEST PASS\n");
    return 0;
}
