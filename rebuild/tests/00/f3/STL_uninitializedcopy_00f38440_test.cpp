#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>
// Behaviour test for 00f38440: __uninitialized_copy of CResourcePage records into raw storage -- each record is copied
// memberwise, its vector<long> gets its own exact-size buffer holding the same longs (empty source -> null buffer), and the
// return value is result + (last - first).
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
static Page* uninit_copy(Page* first, Page* last, Page* result)
{
    Page* cur = result;
    for (; first != last; ++first, ++cur)
        new (cur) Page(*first);
    return cur;
}
int main()
{
    static long a0[3] = { 7, 8, 9 };
    Page src[2];
    src[0].scratchPool = 1; src[0].list.start = a0; src[0].list.finish = a0 + 3; src[0].list.eos = a0 + 3;
    src[0].nextFree = (unsigned char*)0; src[0].freeSpace = 100; src[0].autoRestore = true;
    src[1].scratchPool = 2; src[1].freeSpace = 200; src[1].autoRestore = false; src[1].nextFree = (unsigned char*)0;
    void* raw = std::malloc(sizeof(Page) * 2);
    Page* dst = (Page*)raw;
    Page* end = uninit_copy(src, src + 2, dst);
    int ok = end == dst + 2;
    ok = ok && dst[0].scratchPool == 1 && dst[0].freeSpace == 100 && dst[0].autoRestore;
    ok = ok && dst[0].list.start != a0 && dst[0].list.finish - dst[0].list.start == 3 && dst[0].list.eos == dst[0].list.finish;
    ok = ok && dst[0].list.start[0] == 7 && dst[0].list.start[2] == 9;
    ok = ok && dst[1].scratchPool == 2 && dst[1].list.start == 0 && dst[1].list.finish == 0 && !dst[1].autoRestore;
    ok = ok && uninit_copy(src, src, dst) == dst;
    std::free(dst[0].list.start);
    std::free(raw);
    if (!ok) { std::printf("00f38440_TEST FAIL\n"); return 1; }
    std::printf("00f38440_TEST PASS\n");
    return 0;
}