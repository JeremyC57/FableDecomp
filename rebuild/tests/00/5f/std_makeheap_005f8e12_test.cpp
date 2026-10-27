// behaviour test for 0x005F8E12: make_heap leaves the largest key at the front and a valid max-heap
#include <cstdio>
#include <algorithm>
struct P { long k; long v; };
static bool Less(const P& a, const P& b) { return a.k < b.k; }
int main()
{
    P v[6] = { { 4, 0 }, { 1, 1 }, { 9, 2 }, { 3, 3 }, { 7, 4 }, { 2, 5 } };
    std::make_heap(v, v + 6, Less);
    int ok = v[0].k == 9;
    for (int i = 1; i < 6; ++i) ok = ok && !(v[(i - 1) / 2].k < v[i].k);
    if (!ok) { std::printf("005f8e12_TEST FAIL\n"); return 1; }
    std::printf("005f8e12_TEST PASS\n");
    return 0;
}