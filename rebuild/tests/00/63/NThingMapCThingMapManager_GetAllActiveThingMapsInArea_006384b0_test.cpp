// Behaviour test for 0x006384B0 (source body copied from src/006384b0.cpp; out-of-line callees stubbed below).
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#ifdef _MSC_VER
inline void* operator new(unsigned int, void* p) { return p; }
#else
#include <new>
#define __fastcall
#define __cdecl
#define __stdcall
#define __fastcall
#define __forceinline inline
#define __declspec(x)
#endif

class C2DBoxI {
public:
    long TLX, TLY, BRX, BRY;
    bool Overlapping(const C2DBoxI& other) const;      // 0x004FAB40
};

namespace std {

class __true_type { public: __true_type() {} };

template <class T> class allocator { public: allocator() {} };

template <class T, class Alloc = allocator<T> > class vector {
public:
    T* _M_start;                                         // +0x00
    T* _M_finish;                                        // +0x04
    T* _M_end_of_storage;                                // +0x08

    typedef const T* const_iterator;
    const_iterator begin() const { return _M_start; }
    const_iterator end() const { return _M_finish; }

    void resize(unsigned int new_size, const T& x);      // 0x00639730
    void resize(unsigned int new_size) { resize(new_size, T()); }

    void _M_insert_overflow(T* position, const T& x, const __true_type&, unsigned int fill_len, bool atend);   // 0x00639170

    void push_back(const T& x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) T(x);
            ++_M_finish;
        }
        else
            _M_insert_overflow(_M_finish, x, __true_type(), 1, true);
    }
};

// The real STLport _M_insert_overflow body (0x00639170 for T = CThingMap*), out of line in the TU.
#pragma auto_inline(off)
template <class T, class Alloc>
__declspec(noinline) void vector<T, Alloc>::_M_insert_overflow(T* pos, const T& x, const __true_type&, unsigned int n, bool atend)
{
    unsigned int oldSize = _M_finish - _M_start;
    unsigned int len = oldSize + (oldSize < n ? n : oldSize);
    T* ns = len ? (T*)malloc(len * sizeof(T)) : 0;
    T* nf = ns;
    T* p;
    for (p = _M_start; p != pos; ++p) { *nf++ = *p; }
    for (; n != 0; --n) { *nf++ = x; }
    if (!atend)
        for (p = pos; p != _M_finish; ++p) { *nf++ = *p; }
    if (_M_start)
        free(_M_start);
    _M_start = ns;
    _M_finish = nf;
    _M_end_of_storage = ns + len;
}
// Same-TU stand-in for vector::resize(n, x) 0x00639730 (truncates or appends copies; never keeps `x`).
template <class T, class Alloc>
__declspec(noinline) void vector<T, Alloc>::resize(unsigned int new_size, const T& x)
{
    if (new_size < (unsigned int)(_M_finish - _M_start))
        _M_finish = _M_start + new_size;
    else
        while ((unsigned int)(_M_finish - _M_start) < new_size)
            push_back(x);
}
#pragma auto_inline(on)

}

#pragma auto_inline(off)
// Same-TU stand-in for C2DBoxI::Overlapping 0x004FAB40 (read-only).
__declspec(noinline) bool C2DBoxI::Overlapping(const C2DBoxI& other) const
{
    return TLX <= other.BRX && other.TLX <= BRX && TLY <= other.BRY && other.TLY <= BRY;
}
#pragma auto_inline(on)

namespace NThingMap {

class CThingMap {
public:
    unsigned char _pad00[0x20];
    C2DBoxI       Area;                         // +0x20
};

class CThingMapManager {
public:
    class CThingMapInfo {
    public:
        CThingMap* PThingMap;                   // +0x00
        long       RefCount;                    // +0x04
    };
    void*           __vftable;                  // +0x00
    void*           PWorld;                     // +0x04
    unsigned char   ThingMaps[12];              // +0x08 (retail 12-byte container)
    std::vector<CThingMapInfo*> ActiveThingMaps;   // +0x14
    void GetAllActiveThingMapsInArea(const C2DBoxI& area, std::vector<CThingMap*>& maps) const;
};

void CThingMapManager::GetAllActiveThingMapsInArea(const C2DBoxI& area, std::vector<CThingMap*>& maps) const
{
    maps.resize(0);
    for (std::vector<CThingMapInfo*>::const_iterator it = ActiveThingMaps.begin(); it != ActiveThingMaps.end(); ++it) {
        CThingMapInfo* info = *it;
        if (info->PThingMap->Area.Overlapping(area))
            maps.push_back(info->PThingMap);
    }
}

}

// ---- test harness ----
using namespace NThingMap;
static CThingMap* MakeMap(long x0, long y0, long x1, long y1)
{
    CThingMap* m = (CThingMap*)malloc(sizeof(CThingMap));
    memset(m, 0, sizeof *m);
    m->Area.TLX = x0; m->Area.TLY = y0; m->Area.BRX = x1; m->Area.BRY = y1;
    return m;
}
int main()
{
    int ok = 1;
    CThingMapManager mgr;
    memset(&mgr, 0, sizeof mgr);
    CThingMap* a = MakeMap(0, 0, 10, 10);
    CThingMap* b = MakeMap(20, 20, 30, 30);
    CThingMap* c = MakeMap(5, 5, 25, 25);
    CThingMapManager::CThingMapInfo infos[3];
    infos[0].PThingMap = a; infos[1].PThingMap = b; infos[2].PThingMap = c;
    CThingMapManager::CThingMapInfo* ptrs[3] = { &infos[0], &infos[1], &infos[2] };
    mgr.ActiveThingMaps._M_start = ptrs; mgr.ActiveThingMaps._M_finish = ptrs + 3; mgr.ActiveThingMaps._M_end_of_storage = ptrs + 3;
    std::vector<CThingMap*> maps;
    maps._M_start = (CThingMap**)malloc(1 * sizeof(CThingMap*));
    maps._M_start[0] = b;                                   // stale entry: resize(0) drops it
    maps._M_finish = maps._M_start + 1; maps._M_end_of_storage = maps._M_start + 1;
    C2DBoxI box; box.TLX = 8; box.TLY = 8; box.BRX = 12; box.BRY = 12;   // overlaps a and c
    mgr.GetAllActiveThingMapsInArea(box, maps);
    if (maps._M_finish - maps._M_start != 2 || maps._M_start[0] != a || maps._M_start[1] != c) ok = 0;
    C2DBoxI none; none.TLX = 100; none.TLY = 100; none.BRX = 110; none.BRY = 110;
    mgr.GetAllActiveThingMapsInArea(none, maps);
    if (maps._M_finish != maps._M_start) ok = 0;
    C2DBoxI all; all.TLX = -5; all.TLY = -5; all.BRX = 50; all.BRY = 50;
    mgr.GetAllActiveThingMapsInArea(all, maps);
    if (maps._M_finish - maps._M_start != 3 || maps._M_start[0] != a || maps._M_start[1] != b || maps._M_start[2] != c) ok = 0;
    printf("006384b0_TEST %s\n", ok ? "PASS" : "FAIL");
    return ok ? 0 : 1;
}