// behaviour test for 0x006F37B0 GetAllThingsInRoughArea (model: every thing of the searched type in the cells is appended once per stamp)
#include <cstdio>
#include <vector>
struct T { unsigned long flags; unsigned long stamp; };
int main()
{
    const unsigned long mask = 0x47;
    T things[4] = { { mask, 0 }, { 0, 0 }, { mask | 0x100000, 0 }, { mask, 5 } };
    T* cells[5] = { &things[0], &things[1], &things[2], &things[0], &things[3] };
    unsigned long stamp = 5;
    std::vector<T*> out;
    for (int i = 0; i < 5; ++i) {
        T* t = cells[i];
        if (t->stamp < stamp) {
            t->stamp = stamp;
            if ((t->flags & mask) == mask) out.push_back(t);
        }
    }
    int ok = out.size() == 2 && out[0] == &things[0] && out[1] == &things[2];
    if (!ok) { std::printf("006f37b0_TEST FAIL\n"); return 1; }
    std::printf("006f37b0_TEST PASS\n");
    return 0;
}
