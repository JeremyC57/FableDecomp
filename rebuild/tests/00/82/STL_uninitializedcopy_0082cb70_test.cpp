// Behaviour test: std::__uninitialized_copy over CMinimapData -- each record is copy-constructed member by member (deep vector copies, shared surface/texture)
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

static CMinimapData* __uninitialized_copy(const CMinimapData* first, const CMinimapData* last, CMinimapData* result)
{
    CMinimapData* cur = result;
    for (; first != last; ++first, ++cur)
        new (cur) CMinimapData(*first);
    return cur;
}

int main()
{
    CMinimapData src[2];
    Init(src[0], 1);
    Init(src[1], 5);
    void* raw = std::malloc(3 * sizeof(CMinimapData));
    CMinimapData* dst = (CMinimapData*)raw;
    CMinimapData* end = __uninitialized_copy(src, src + 2, dst);
    if (end != dst + 2 || !Copied(dst[0], src[0], 1) || !Copied(dst[1], src[1], 5))
        { std::printf("0082cb70_TEST FAIL\n"); return 1; }
    if (__uninitialized_copy(src, src, dst + 2) != dst + 2)   // empty range
        { std::printf("0082cb70_TEST FAIL\n"); return 1; }
    dst[0].~CMinimapData();
    dst[1].~CMinimapData();
    std::free(raw);
    std::printf("0082cb70_TEST PASS\n");
    return 0;
}