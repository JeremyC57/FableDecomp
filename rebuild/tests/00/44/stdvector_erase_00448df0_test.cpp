// behaviour test for 0x00448df0 vector<CCountedPointer<const CInputTypeBase>>::erase(first, last): assign the tail
// down over the gap, destroy the now-unused tail elements, shrink finish, return first
#include <cstdio>
static int gReleases = 0;
struct CP { int id; ~CP() { ++gReleases; } };
struct Vec { CP* start; CP* finish;
    CP* erase(CP* first, CP* last)
    {
        CP* i = first;
        for (CP* s = last; s != finish; ++s, ++i) *i = *s;
        for (CP* p = i; p != finish; ++p) p->~CP();
        finish = i;
        return first;
    } };
int main()
{
    int fails = 0;
    static unsigned char raw[6 * sizeof(CP)];
    CP* a = (CP*)raw;
    for (int k = 0; k < 6; ++k) a[k].id = k;
    Vec v; v.start = a; v.finish = a + 6;
    CP* r = v.erase(a + 1, a + 3);
    if (r != a + 1) fails |= 1;
    if (v.finish != a + 4) fails |= 2;
    if (a[0].id != 0 || a[1].id != 3 || a[2].id != 4 || a[3].id != 5) fails |= 4;
    if (gReleases != 2) fails |= 8;
    if (v.erase(a + 4, a + 4) != a + 4 || v.finish != a + 4 || gReleases != 2) fails |= 16;
    if (fails) { std::printf("00448df0_TEST FAIL %d\n", fails); return 1; }
    std::printf("00448df0_TEST PASS\n");
    return 0;
}