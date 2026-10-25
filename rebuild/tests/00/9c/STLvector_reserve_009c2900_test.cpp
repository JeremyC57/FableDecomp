// Behaviour test for 0x009C2900 vector<CDisplayModeDesc>::reserve: below the requested capacity, a non-empty vector copies
// its display modes into a fresh n-element block and frees the old one, an empty one just allocates; size is kept,
// capacity becomes n; a request at or below the capacity changes nothing.
#include <cstdio>
#include <cstdlib>
#include <new>

class C2DExtentsI { public: long X, Y; };
class CDisplayModeDesc {
public:
    C2DExtentsI Dimensions;
    long BitsPerPixel;
    long RefreshRate;
    CDisplayModeDesc(long x, long y, long bpp, long hz) { Dimensions.X = x; Dimensions.Y = y; BitsPerPixel = bpp; RefreshRate = hz; }
    CDisplayModeDesc(const CDisplayModeDesc& o) : Dimensions(o.Dimensions), BitsPerPixel(o.BitsPerPixel), RefreshRate(o.RefreshRate) { }
};

static int g_mallocs = 0;
static int g_frees = 0;

class ModeVec {
public:
    CDisplayModeDesc* start;
    CDisplayModeDesc* finish;
    CDisplayModeDesc* eos;
    unsigned int size() const { return (unsigned int)(finish - start); }
    unsigned int capacity() const { return (unsigned int)(eos - start); }
    static CDisplayModeDesc* allocate(unsigned int n) { if (!n) return 0; ++g_mallocs; return (CDisplayModeDesc*)std::malloc(n * sizeof(CDisplayModeDesc)); }
    void reserve(unsigned int n)
    {
        if (capacity() < n) {
            const unsigned int old_size = size();
            CDisplayModeDesc* tmp;
            if (start) {
                tmp = allocate(n);
                CDisplayModeDesc* cur = tmp;
                for (CDisplayModeDesc* p = start; p != finish; ++p, ++cur)
                    new (cur) CDisplayModeDesc(*p);
                ++g_frees;
                std::free(start);
            } else {
                tmp = allocate(n);
            }
            start = tmp; finish = tmp + old_size; eos = tmp + n;
        }
    }
};

int main()
{
    ModeVec v; v.start = v.finish = v.eos = 0;
    int ok = sizeof(CDisplayModeDesc) == 16;
    v.reserve(2);                                             // empty: plain allocation
    if (g_mallocs != 1 || g_frees != 0 || v.size() != 0 || v.capacity() != 2) ok = 0;
    new (v.finish) CDisplayModeDesc(640, 480, 32, 60); ++v.finish;
    new (v.finish) CDisplayModeDesc(1024, 768, 16, 75); ++v.finish;
    v.reserve(2);                                             // already enough: untouched
    if (g_mallocs != 1 || v.capacity() != 2) ok = 0;
    v.reserve(5);                                             // grow: copy then free the old block
    if (g_mallocs != 2 || g_frees != 1 || v.size() != 2 || v.capacity() != 5) ok = 0;
    if (v.start[0].Dimensions.X != 640 || v.start[0].RefreshRate != 60) ok = 0;
    if (v.start[1].Dimensions.Y != 768 || v.start[1].BitsPerPixel != 16) ok = 0;
    std::free(v.start);
    std::printf(ok ? "009c2900_TEST PASS\n" : "009c2900_TEST FAIL\n");
    return ok ? 0 : 1;
}
