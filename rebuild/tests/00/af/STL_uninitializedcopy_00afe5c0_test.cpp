#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>

// Model of std::__uninitialized_copy over CArray<long>: deep copies of a run of primitive-ID lists.
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
static LongArray* Copy(const LongArray* first, const LongArray* last, LongArray* result)
{
    LongArray* cur = result;
    for (; first != last; ++first, ++cur) new (cur) LongArray(*first);
    return cur;
}

int main()
{
    long data[3] = { 4, 5, 6 };
    LongArray x; x.start = data; x.finish = data + 3; x.eos = data + 3;
    LongArray src[2]; src[0] = x; src[1] = x;
    LongArray* raw = (LongArray*)std::malloc(sizeof(LongArray) * 2);
    LongArray* end = Copy(src, src + 2, raw);
    if (end != raw + 2 || raw[0].finish - raw[0].start != 3 || raw[1].start[2] != 6 || raw[0].start == data || raw[0].start == raw[1].start) { std::printf("00afe5c0_TEST FAIL\n"); return 1; }
    LongArray empty;
    LongArray* raw2 = (LongArray*)std::malloc(sizeof(LongArray));
    if (Copy(&empty, &empty, raw2) != raw2) { std::printf("00afe5c0_TEST FAIL\n"); return 1; }
    Copy(&empty, &empty + 1, raw2);
    if (raw2[0].start != 0 || raw2[0].finish != 0 || raw2[0].eos != 0) { std::printf("00afe5c0_TEST FAIL\n"); return 1; }
    std::free(raw[0].start); std::free(raw[1].start); std::free(raw); std::free(raw2);
    std::printf("00afe5c0_TEST PASS\n");
    return 0;
}