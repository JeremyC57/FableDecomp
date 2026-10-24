// Behaviour test for 0x007AFA10 __copy over CIntelligentPointer<CThingAICreature>: forward element assignment.
#include <cstdio>
struct CThing { int id; };
struct IP {
    CThing* PItem;
    CThing* GetPItem() const { return PItem; }
    void SetPItem(CThing* p) { PItem = p; }
    IP& operator=(const IP& o) { SetPItem(o.GetPItem()); return *this; }
};
static IP* Copy(IP* first, IP* last, IP* result)
{
    for (int n = (int)(last - first); n > 0; --n) { *result = *first; ++first; ++result; }
    return result;
}
int main()
{
    CThing t[4];
    IP a[4]; for (int i = 0; i < 4; ++i) a[i].PItem = &t[i];
    // overlapping left shift (erase of a[0]): a[0..2] = a[1..3]
    IP* end = Copy(a + 1, a + 4, a);
    if (end != a + 3 || a[0].PItem != &t[1] || a[1].PItem != &t[2] || a[2].PItem != &t[3]) { std::printf("007afa10_TEST FAIL\n"); return 1; }
    if (Copy(a, a, a + 2) != a + 2) { std::printf("007afa10_TEST FAIL\n"); return 1; }
    std::printf("007afa10_TEST PASS\n");
    return 0;
}