// Behaviour test for 0x006BC160 CRegion::RemoveMap(long): erase(remove(begin, end, map)) -- the kept elements close up, finish drops by exactly one slot.
#include <cstdio>
#include <cstring>
struct LongVec {
    long* start; long* finish; long* eos;
    void RemoveMap(long map)
    {
        long* first = start;
        long* last = finish;
        while (first != last && *first != map) ++first;          // find
        long* result = first;
        if (first != last)
            for (long* p = first + 1; p != last; ++p)           // remove_copy
                if (*p != map) *result++ = *p;
        if (result + 1 != finish)                                // single-element erase
            std::memmove(result, result + 1, (char*)finish - (char*)(result + 1));
        --finish;
    }
};
static int fail() { std::printf("006bc160_TEST FAIL\n"); return 1; }
int main()
{
    long a[6] = { 3, 7, 9, 7, 11, 13 };
    LongVec v; v.start = a; v.finish = a + 6; v.eos = a + 6;
    v.RemoveMap(9);                                    // unique id: plain removal
    if (v.finish != a + 5 || a[0] != 3 || a[1] != 7 || a[2] != 7 || a[3] != 11 || a[4] != 13) return fail();
    v.RemoveMap(13);                                   // last element: no shift
    if (v.finish != a + 4 || a[3] != 11) return fail();
    v.RemoveMap(7);                                    // duplicate id: remove packs {3,11}, then one slot is erased
    if (v.finish != a + 3 || a[0] != 3 || a[1] != 11 || a[2] != 11) return fail();
    std::printf("006bc160_TEST PASS\n");
    return 0;
}