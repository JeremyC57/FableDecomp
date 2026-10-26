// behaviour test for 0x005798A3 vector<CTCInventoryExperience::ExperienceSpendInfo>::erase(first, last): the tail is assigned down front to back, the end moves back by
// last - first, first is returned; an empty range changes nothing
#include <cstdio>
struct Elem { int v[3]; };                           // stands in for the 12-byte CTCInventoryExperience::ExperienceSpendInfo (int: 32-bit under both compilers)
struct Vec {
    Elem* start; Elem* finish; Elem* end;
    Elem* Erase(Elem* first, Elem* last)                     // the function under test
    {
        Elem* r = first;
        for (int n = (int)(finish - last); n > 0; --n) { *r = *last; ++r; ++last; }
        finish = r;
        return first;
    }
};
int main()
{
    int fail = 0;
    if (sizeof(Elem) != 12u) fail |= 1;
    Elem a[6];
    for (int k = 0; k < 6; ++k) for (int i = 0; i < 3; ++i) a[k].v[i] = k * 100 + i;
    Vec v; v.start = a; v.finish = a + 6; v.end = a + 6;
    if (v.Erase(a + 1, a + 3) != a + 1) fail |= 2;
    if (v.finish != a + 4 || a[1].v[0] != 300 || a[3].v[0] != 500 || a[0].v[0] != 0) fail |= 4;
    if (v.Erase(a + 2, a + 2) != a + 2 || v.finish != a + 4 || a[2].v[0] != 400) fail |= 8;
    if (fail) { std::printf("005798a3_TEST FAIL %d\n", fail); return 1; }
    std::printf("005798a3_TEST PASS\n");
    return 0;
}