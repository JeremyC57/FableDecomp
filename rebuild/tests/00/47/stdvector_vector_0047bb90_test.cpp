// behaviour test for 0x0047bb90 std::vector<CHeroLogBook::CHeroLogBookEntry >::vector(const vector&): allocate exactly size() 0xC-byte
// elements (nothing for an empty source), copy-construct them in order, finish = end of storage
#include <cstdio>
#include <cstdlib>
struct Elem { int w[3]; };
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
    Elem src[3];
    for (int i = 0; i < 3; ++i) for (int k = 0; k < 3; ++k) src[i].w[k] = i * 100 + k;
    Vec x; x.start = src; x.finish = src + 3; x.eos = src + 3;
    Vec v = Copy(x);
    if (gAllocs != 1 || v.finish - v.start != 3 || v.eos != v.finish) fails |= 1;
    for (int i = 0; i < 3; ++i) for (int k = 0; k < 3; ++k) if (v.start[i].w[k] != i * 100 + k) fails |= 2;
    if (v.start == src) fails |= 4;
    std::free(v.start);
    Vec e; e.start = e.finish = e.eos = 0;
    Vec w = Copy(e);
    if (gAllocs != 1 || w.start || w.finish || w.eos) fails |= 8;
    if (fails) { std::printf("0047bb90_TEST FAIL %d\n", fails); return 1; }
    std::printf("0047bb90_TEST PASS\n");
    return 0;
}