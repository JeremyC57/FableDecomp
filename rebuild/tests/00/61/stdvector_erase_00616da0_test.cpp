// behaviour test for 0x00616DA0 std::vector<CTCInventoryMap::CUIRegionInfo >::erase(first, last) over 0x28-byte records: the tail moves down onto
// first, the end shrinks by last - first, the result is first, and an empty range changes nothing
#include <cstdio>
struct Elem { int Words[10]; };
struct Vec {
    Elem* Start; Elem* Finish; Elem* EndOfStorage;
    Elem* Erase(Elem* first, Elem* last)                   // the function under test
    {
        Elem* r = first;
        for (int n = (int)(Finish - last); n > 0; --n) { *r = *last; ++r; ++last; }
        Finish = r;
        return first;
    }
};
int main()
{
    int fail = 0;
    if (sizeof(Elem) != 0x28u) fail |= 16;
    Elem a[6];
    for (int i = 0; i < 6; ++i) for (int w = 0; w < 10; ++w) a[i].Words[w] = i * 100 + w;
    Vec v; v.Start = a; v.Finish = a + 6; v.EndOfStorage = a + 6;
    if (v.Erase(a + 1, a + 3) != a + 1 || v.Finish != a + 4) fail |= 1;
    int want[4] = { 0, 300, 400, 500 };
    for (int i = 0; i < 4; ++i) for (int w = 0; w < 10; ++w) if (a[i].Words[w] != want[i] + w) fail |= 2;
    if (v.Erase(a + 2, a + 2) != a + 2 || v.Finish != a + 4) fail |= 4;
    if (v.Erase(a + 2, v.Finish) != a + 2 || v.Finish != a + 2) fail |= 8;
    if (fail) { std::printf("00616da0_TEST FAIL %d\n", fail); return 1; }
    std::printf("00616da0_TEST PASS\n");
    return 0;
}