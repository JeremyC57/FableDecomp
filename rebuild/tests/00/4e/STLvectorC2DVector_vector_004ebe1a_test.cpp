#include <cstdio>
#include <cstdlib>
#include <new>
// Behaviour copy of the STLport vector copy constructor: allocate exactly x.size() elements (null when empty), copy-construct
// x's range, finish = end of storage = start + n.
struct SElem { float X; float Y; };
struct SVec {
    SElem* start; SElem* finish; SElem* eos;
    SVec() : start(0), finish(0), eos(0) { }
    SVec(const SVec& x) : start(0), finish(0), eos(0)
    {
        unsigned n = unsigned(x.finish - x.start);
        start = n != 0 ? (SElem*)std::malloc(n * sizeof(SElem)) : 0;
        finish = start; eos = start + n;
        SElem* cur = start;
        for (const SElem* f = x.start; f != x.finish; ++f, ++cur) new (cur) SElem(*f);
        finish = cur;
    }
};
int main()
{
    bool ok = true;
    SElem src[4];
    for (int i = 0; i < 4; ++i) { SElem& e = src[i]; e.X = 1.5f * i; e.Y = -2.0f * i; }
    SVec a; a.start = src; a.finish = src + 3; a.eos = src + 4;
    SVec c(a);
    if (c.start == 0 || c.start == src || c.finish != c.start + 3 || c.eos != c.start + 3) ok = false;
    for (int i = 0; ok && i < 3; ++i) if (c.start[i].X != 1.5f * i || c.start[i].Y != -2.0f * i) ok = false;
    std::free(c.start);
    SVec e; SVec d(e);
    if (d.start != 0 || d.finish != 0 || d.eos != 0) ok = false;
    if (!ok) { std::printf("004ebe1a_TEST FAIL\n"); return 1; }
    std::printf("004ebe1a_TEST PASS\n");
    return 0;
}