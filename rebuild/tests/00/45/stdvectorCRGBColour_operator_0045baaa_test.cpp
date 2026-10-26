// Behaviour test for 0x0045BAAA std::vector<CRGBColour>::operator=: the three STLport assignment paths
#include <cstdio>
#include <cstdlib>
#include <new>
class CRGBColour { public:
    unsigned char B, G, R, A;
    CRGBColour() { }
    CRGBColour(const CRGBColour& o) : B(o.B), G(o.G), R(o.R), A(o.A) { }
};
struct ColourVector {
    CRGBColour* start; CRGBColour* finish; CRGBColour* eos;
    ColourVector() : start(0), finish(0), eos(0) { }
    unsigned int size() const { return (unsigned int)(finish - start); }
    unsigned int capacity() const { return (unsigned int)(eos - start); }
    CRGBColour* AllocateAndCopy(unsigned int n, const CRGBColour* f, const CRGBColour* l)
    {
        CRGBColour* r = n ? static_cast<CRGBColour*>(std::malloc(n * sizeof(CRGBColour))) : 0;
        CRGBColour* c = r;
        for (; f != l; ++f, ++c) new (c) CRGBColour(*f);
        return r;
    }
    void Clear() { if (start) std::free(start); }
    ColourVector& operator=(const ColourVector& x)
    {
        if (&x != this) {
            const unsigned int xlen = x.size();
            if (xlen > capacity()) {
                CRGBColour* tmp = AllocateAndCopy(xlen, x.start, x.finish);
                Clear();
                start = tmp;
                eos = start + xlen;
            } else if (size() >= xlen) {
                for (long n = x.finish - x.start, i = 0; i < n; ++i) start[i] = x.start[i];
            } else {
                unsigned int s = size();
                for (unsigned int i = 0; i < s; ++i) start[i] = x.start[i];
                CRGBColour* c = finish;
                for (const CRGBColour* f = x.start + s; f != x.finish; ++f, ++c) new (c) CRGBColour(*f);
            }
            finish = start + xlen;
        }
        return *this;
    }
};
static void Fill(ColourVector& v, unsigned int n, unsigned int cap, int seed)
{
    v.start = static_cast<CRGBColour*>(std::malloc(cap * sizeof(CRGBColour)));
    v.finish = v.start + n; v.eos = v.start + cap;
    for (unsigned int i = 0; i < n; ++i) { v.start[i].B = (unsigned char)(seed + i); v.start[i].G = 1; v.start[i].R = 2; v.start[i].A = 3; }
}
static bool Same(const ColourVector& a, const ColourVector& b)
{
    if (a.size() != b.size()) return false;
    for (unsigned int i = 0; i < a.size(); ++i)
        if (a.start[i].B != b.start[i].B || a.start[i].G != b.start[i].G || a.start[i].R != b.start[i].R || a.start[i].A != b.start[i].A) return false;
    return true;
}
int main()
{
    ColourVector src; Fill(src, 5, 5, 10);
    ColourVector grow; Fill(grow, 1, 2, 0);       // capacity too small: reallocate
    grow = src;
    ColourVector shrink; Fill(shrink, 7, 8, 50);  // larger: copy over, size drops
    shrink = src;
    ColourVector tail; Fill(tail, 2, 8, 90);      // smaller but enough capacity: copy + construct tail
    tail = src;
    if (!Same(grow, src) || !Same(shrink, src) || !Same(tail, src) || grow.capacity() != 5 || shrink.capacity() != 8) { std::printf("0045baaa_TEST FAIL\n"); return 1; }
    ColourVector* self = &src; *self = src;
    if (src.size() != 5) { std::printf("0045baaa_TEST FAIL\n"); return 1; }
    std::printf("0045baaa_TEST PASS\n");
    return 0;
}