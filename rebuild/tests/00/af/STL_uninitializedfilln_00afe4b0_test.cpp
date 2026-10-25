#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>

// Model of std::__uninitialized_fill_n over CArray<long>: n deep copies of one primitive-ID list.
struct LongArray {
    long* start; long* finish; long* eos;
    LongArray() : start(0), finish(0), eos(0) { }
    LongArray(const LongArray& x) : start(0), finish(0), eos(0)
    {
        unsigned int n = (unsigned int)(x.finish - x.start);
        start = n ? (long*)std::malloc(n * sizeof(long)) : 0;
        finish = start; eos = start + n;
        if (x.finish != x.start) { std::memmove(start, x.start, (x.finish - x.start) * sizeof(long)); finish = start + n; }
    }
};
static LongArray* FillN(LongArray* first, unsigned int n, const LongArray& x)
{
    LongArray* cur = first;
    for (; n > 0; --n, ++cur) new (cur) LongArray(x);
    return cur;
}

int main()
{
    long data[3] = { 4, 5, 6 };
    LongArray x; x.start = data; x.finish = data + 3; x.eos = data + 3;
    LongArray* raw = (LongArray*)std::malloc(sizeof(LongArray) * 2);
    LongArray* end = FillN(raw, 2, x);
    if (end != raw + 2 || raw[0].finish - raw[0].start != 3 || raw[1].start[2] != 6 || raw[0].start == data || raw[0].start == raw[1].start) { std::printf("00afe4b0_TEST FAIL\n"); return 1; }
    LongArray empty;
    LongArray* raw2 = (LongArray*)std::malloc(sizeof(LongArray));
    FillN(raw2, 1, empty);
    if (raw2[0].start != 0 || raw2[0].finish != 0 || raw2[0].eos != 0) { std::printf("00afe4b0_TEST FAIL\n"); return 1; }
    std::free(raw[0].start); std::free(raw[1].start); std::free(raw); std::free(raw2);
    std::printf("00afe4b0_TEST PASS\n");
    return 0;
}