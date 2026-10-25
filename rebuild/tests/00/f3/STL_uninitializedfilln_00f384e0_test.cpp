#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>
// Behaviour test for 00f384e0: __uninitialized_fill_n of CResourcePage records -- n copies of x, each with its own copy of
// x's vector<long>; n == 0 constructs nothing and returns first; the return value is first + n.
struct LongVec {
    long* start; long* finish; long* eos;
    LongVec() : start(0), finish(0), eos(0) { }
    LongVec(const LongVec& x) : start(0), finish(0), eos(0)
    {
        unsigned int n = (unsigned int)(x.finish - x.start);
        start = n != 0 ? (long*)std::malloc(n * sizeof(long)) : 0;
        finish = start;
        eos = start + n;
        if (x.finish != x.start) {
            std::memmove(start, x.start, (x.finish - x.start) * sizeof(long));
            finish = start + (x.finish - x.start);
        }
    }
};
struct Page { long scratchPool; LongVec list; unsigned char* nextFree; long freeSpace; bool autoRestore; };
static Page* uninit_fill_n(Page* first, unsigned int n, const Page& x)
{
    Page* cur = first;
    for (; n > 0; --n, ++cur)
        new (cur) Page(x);
    return cur;
}
int main()
{
    static long a0[2] = { 4, 5 };
    Page x;
    x.scratchPool = 3; x.list.start = a0; x.list.finish = a0 + 2; x.list.eos = a0 + 2;
    x.nextFree = (unsigned char*)0; x.freeSpace = 64; x.autoRestore = true;
    void* raw = std::malloc(sizeof(Page) * 3);
    Page* dst = (Page*)raw;
    Page* end = uninit_fill_n(dst, 3, x);
    int ok = end == dst + 3;
    for (int i = 0; i < 3; ++i) {
        ok = ok && dst[i].scratchPool == 3 && dst[i].freeSpace == 64 && dst[i].autoRestore;
        ok = ok && dst[i].list.finish - dst[i].list.start == 2 && dst[i].list.start != a0 && dst[i].list.start[1] == 5;
    }
    ok = ok && dst[0].list.start != dst[1].list.start;
    ok = ok && uninit_fill_n(dst, 0, x) == dst;
    for (int j = 0; j < 3; ++j) std::free(dst[j].list.start);
    std::free(raw);
    if (!ok) { std::printf("00f384e0_TEST FAIL\n"); return 1; }
    std::printf("00f384e0_TEST PASS\n");
    return 0;
}