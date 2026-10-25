// behaviour test for 0x00410460 std::vector<CDisplayModeDesc >::vector(const vector&): allocate exactly size() 16-byte elements
// (nothing for an empty source), copy-construct them in order, finish = end of storage
#include <cstdio>
#include <cstdlib>
struct Elem { int a, b, c, d; };
struct Vec { Elem* start; Elem* finish; Elem* eos; };
static int gAllocs = 0;
static Vec Copy(const Vec& x)
{
    Vec v; v.start = v.finish = v.eos = 0;
    unsigned int n = (unsigned int)(x.finish - x.start);
    v.start = n ? (++gAllocs, (Elem*)std::malloc(n * sizeof(Elem))) : 0;
    v.finish = v.start; v.eos = v.start + n;
    Elem* r = v.start;
    for (const Elem* p = x.start; p != x.finish; ++p, ++r) if (r) *r = *p;
    v.finish = r;
    return v;
}
int main()
{
    int fails = 0;
    Elem src[3] = { { 1, 2, 3, 4 }, { 5, 6, 7, 8 }, { 9, 10, 11, 12 } };
    Vec x; x.start = src; x.finish = src + 3; x.eos = src + 3;
    Vec v = Copy(x);
    if (gAllocs != 1 || v.finish - v.start != 3 || v.eos != v.finish) fails |= 1;
    for (int i = 0; i < 3; ++i) if (v.start[i].a != src[i].a || v.start[i].d != src[i].d) fails |= 2;
    if (v.start == src) fails |= 4;
    std::free(v.start);
    Vec e; e.start = e.finish = e.eos = 0;
    Vec w = Copy(e);
    if (gAllocs != 1 || w.start || w.finish || w.eos) fails |= 8;
    if (fails) { std::printf("00410460_TEST FAIL %d\n", fails); return 1; }
    std::printf("00410460_TEST PASS\n");
    return 0;
}