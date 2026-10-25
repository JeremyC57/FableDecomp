// Behaviour test: std::__uninitialized_fill_n over CMinimapData -- n copies of the prototype record, each with its own vector storage
#include <cstdio>
#include <cstdlib>
#include <new>

struct CSurface {
    void* PD3DSurface;
    unsigned int AllocationSource;
    void* PAllocatedMemory;
    CSurface() : PD3DSurface(0), AllocationSource(0), PAllocatedMemory(0) { }
    CSurface(const CSurface& o) : PD3DSurface(o.PD3DSurface), AllocationSource(o.AllocationSource), PAllocatedMemory(o.AllocationSource == 2 ? o.PAllocatedMemory : 0) { }
    virtual ~CSurface() { }
};
struct CTexture {
    void* PD3DTexture;
    unsigned int ByteLength;
    CTexture() : PD3DTexture(0), ByteLength(0) { }
    CTexture(const CTexture& o) : PD3DTexture(o.PD3DTexture), ByteLength(o.ByteLength) { }
};
struct C2DExtentsI {
    int X, Y;
    C2DExtentsI() : X(0), Y(0) { }
    C2DExtentsI(const C2DExtentsI& o) : X(o.X), Y(o.Y) { }
};
template <class T> struct CVec {                        // STLport vector with a deep copy constructor
    T* start;
    T* finish;
    T* eos;
    CVec() : start(0), finish(0), eos(0) { }
    CVec(const CVec& x) : start(0), finish(0), eos(0)
    {
        unsigned int n = (unsigned int)(x.finish - x.start);
        start = n ? (T*)std::malloc(n * sizeof(T)) : 0;
        finish = start;
        eos = start + n;
        for (const T* p = x.start; p != x.finish; ++p, ++finish)
            *finish = *p;
    }
    ~CVec() { if (start) std::free(start); }
private:
    CVec& operator=(const CVec&);
};
struct CMinimapData {
    C2DExtentsI        MinimapExtents;
    CVec<int>          MinimapThemeList;
    CSurface           MinimapSurface;
    CTexture           MinimapTexture;
    unsigned int       MaxMinimapThemeColours;
    CVec<unsigned int> ColourMinimapTheme;
};
static void Init(CMinimapData& d, int tag)
{
    d.MinimapExtents.X = tag; d.MinimapExtents.Y = tag + 1;
    d.MinimapThemeList.start = (int*)std::malloc(2 * sizeof(int));
    d.MinimapThemeList.start[0] = tag; d.MinimapThemeList.start[1] = tag * 2;
    d.MinimapThemeList.finish = d.MinimapThemeList.eos = d.MinimapThemeList.start + 2;
    d.MinimapSurface.AllocationSource = 2; d.MinimapSurface.PAllocatedMemory = &d;
    d.MinimapTexture.ByteLength = 100 + tag;
    d.MaxMinimapThemeColours = 3;
    d.ColourMinimapTheme.start = (unsigned int*)std::malloc(3 * sizeof(unsigned int));
    for (int i = 0; i < 3; ++i)
        d.ColourMinimapTheme.start[i] = 0xff000000u + tag + i;
    d.ColourMinimapTheme.finish = d.ColourMinimapTheme.eos = d.ColourMinimapTheme.start + 3;
}
static bool Copied(const CMinimapData& c, const CMinimapData& o, int tag)
{
    return c.MinimapExtents.X == tag && c.MinimapExtents.Y == tag + 1 && c.MinimapThemeList.start != o.MinimapThemeList.start
        && c.MinimapThemeList.finish - c.MinimapThemeList.start == 2 && c.MinimapThemeList.start[1] == tag * 2
        && c.MinimapSurface.PAllocatedMemory == &o && c.MinimapTexture.ByteLength == (unsigned int)(100 + tag)
        && c.MaxMinimapThemeColours == 3 && c.ColourMinimapTheme.start != o.ColourMinimapTheme.start
        && c.ColourMinimapTheme.finish - c.ColourMinimapTheme.start == 3 && c.ColourMinimapTheme.start[2] == 0xff000000u + tag + 2;
}

static CMinimapData* __uninitialized_fill_n(CMinimapData* first, unsigned int n, const CMinimapData& x)
{
    CMinimapData* cur = first;
    for (; n > 0; --n, ++cur)
        new (cur) CMinimapData(x);
    return cur;
}

int main()
{
    CMinimapData proto;
    Init(proto, 3);
    void* raw = std::malloc(3 * sizeof(CMinimapData));
    CMinimapData* dst = (CMinimapData*)raw;
    CMinimapData* end = __uninitialized_fill_n(dst, 3, proto);
    if (end != dst + 3 || !Copied(dst[0], proto, 3) || !Copied(dst[2], proto, 3) || dst[0].ColourMinimapTheme.start == dst[1].ColourMinimapTheme.start)
        { std::printf("0082cc40_TEST FAIL\n"); return 1; }
    if (__uninitialized_fill_n(dst, 0, proto) != dst)
        { std::printf("0082cc40_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 3; ++i)
        dst[i].~CMinimapData();
    std::free(raw);
    std::printf("0082cc40_TEST PASS\n");
    return 0;
}