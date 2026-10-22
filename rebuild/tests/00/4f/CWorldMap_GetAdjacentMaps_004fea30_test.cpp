#include <stdio.h>
// CWorldMap::GetAdjacentMaps(long, vector<unsigned long>&) const -- collect the maps touching one map: grow its area by one unit on every side, clip it to the world, list the maps in it and drop the map itself
// Leaf: GetAdjacentMaps
// Retail: Fable.exe @ 0x004FEA30, 195 bytes, `ret 8`. PDB ?GetAdjacentMaps@CWorldMap@@QBEXJAAV?$vector@KV?$allocator@K@std@@@std@@@Z
// Callees: memmove 0x00BFEAE6 (the inlined vector erase copy-downs), CWorldMap virtual slot 13 GetDimensions() const
// (vtable 0x01244AEC -> 0x0051CDE0, returns &Area at +0x5C), C2DBoxI::ClipTo(const C2DBoxI&) 0x0049C810, CWorldMap virtual
// slot 20 GetAllMapsInArea(const C2DBoxI&, vector<unsigned long>&) const 0x004FF150.
// Retail CWorldMap offsets: Maps (CArray<CMapInfo>, 0x48-byte elements, Area at +0) +0x20.
// Shape: the vector is cleared with the inline erase(begin(), end()); its copy-down keeps a separate `result` (starting
// as dest) so the start pointer is the merged return value in eax; the area is a copy taken through a const reference to
// Maps[map].Area and grown X first (TLX, BRX, TLY, BRY); the self entry is removed with the inline single-element erase
// (copy-down of the tail, finish -= 1) inside a manual `it` loop. Local layout: engine/CWorldMap.h puts Maps at the PDB's
// +0x24, retail reads it at +0x20 (the header's reconciliation conflict), so the class is modelled here.
#include <string.h>

struct C2DBoxI {
    long TLX, TLY, BRX, BRY;
    void ClipTo(const C2DBoxI& box);                         // 0x0049C810
};

struct CULongVector {                                        // STLport vector<unsigned long> (12 bytes)
    unsigned long* _M_start;
    unsigned long* _M_finish;
    unsigned long* _M_end_of_storage;
    unsigned long* begin() { return _M_start; }
    unsigned long* end() { return _M_finish; }
    static unsigned long* CopyDown(unsigned long* src, unsigned long* srcEnd, unsigned long* dest)
    {
        unsigned long* result = dest;
        if (src != srcEnd)
        {
            unsigned long count = (char*)srcEnd - (char*)src;
            result = (unsigned long*)((char*)memmove(dest, src, count) + count);
        }
        return result;
    }
    unsigned long* erase(unsigned long* first, unsigned long* last)
    {
        _M_finish = CopyDown(last, _M_finish, first);
        return first;
    }
    unsigned long* erase(unsigned long* position)
    {
        CopyDown(position + 1, _M_finish, position);
        --_M_finish;
        return position;
    }
    void clear() { erase(begin(), end()); }
};

struct CMapInfo {                                            // 0x48 bytes
    C2DBoxI       Area;                                      // +0x00
    unsigned char _rest[0x38];                               // +0x10 .. +0x48
};
template <class T> struct CArray {
    T* Start;                                                // +0x00
    T* Finish;                                               // +0x04
    T* EndOfStorage;                                         // +0x08
    const T& operator[](unsigned long i) const { return Start[i]; }
};

struct CWorldMap {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12();
    virtual const C2DBoxI& GetDimensions() const;                                    // slot 13 (0x0051CDE0)
    virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19();
    virtual void GetAllMapsInArea(const C2DBoxI& area, CULongVector& maps) const;    // slot 20 (0x004FF150)
    unsigned char    _pad04[0x1c];
    CArray<CMapInfo> Maps;                                   // +0x20
    void GetAdjacentMaps(long map, CULongVector& maps) const;
};

void CWorldMap::GetAdjacentMaps(long map, CULongVector& maps) const
{
    maps.clear();
    const C2DBoxI& src = Maps[map].Area;
    C2DBoxI area = src;
    area.TLX -= 1;
    area.BRX += 1;
    area.TLY -= 1;
    area.BRY += 1;
    area.ClipTo(GetDimensions());
    GetAllMapsInArea(area, maps);
    for (unsigned long* it = maps.begin(); it != maps.end(); )
    {
        if (*it == map)
            maps.erase(it);
        else
            ++it;
    }
}

// ---- stubs ----
static int fails = 0;
static void check(bool ok, const char* what) { if (!ok) { printf("FAIL %s\n", what); ++fails; } }
static C2DBoxI g_world = { 0, 0, 100, 100 };
static C2DBoxI g_seenArea;
static unsigned long g_found[16]; static int g_foundCount = 0;
void C2DBoxI::ClipTo(const C2DBoxI& b)
{
    if (TLX < b.TLX) TLX = b.TLX;
    if (TLY < b.TLY) TLY = b.TLY;
    if (BRX > b.BRX) BRX = b.BRX;
    if (BRY > b.BRY) BRY = b.BRY;
}
void CWorldMap::v00() {} void CWorldMap::v01() {} void CWorldMap::v02() {} void CWorldMap::v03() {}
void CWorldMap::v04() {} void CWorldMap::v05() {} void CWorldMap::v06() {} void CWorldMap::v07() {}
void CWorldMap::v08() {} void CWorldMap::v09() {} void CWorldMap::v10() {} void CWorldMap::v11() {}
void CWorldMap::v12() {} void CWorldMap::v14() {} void CWorldMap::v15() {} void CWorldMap::v16() {}
void CWorldMap::v17() {} void CWorldMap::v18() {} void CWorldMap::v19() {}
const C2DBoxI& CWorldMap::GetDimensions() const { return g_world; }
void CWorldMap::GetAllMapsInArea(const C2DBoxI& area, CULongVector& maps) const
{
    g_seenArea = area;
    for (int i = 0; i < g_foundCount; ++i) *maps._M_finish++ = g_found[i];
}

static unsigned long g_store[32];
static int run(CWorldMap& wm, long map, const unsigned long* found, int n, int prefill, unsigned long* out)
{
    CULongVector v; v._M_start = g_store; v._M_finish = g_store + prefill; v._M_end_of_storage = g_store + 32;
    for (int i = 0; i < prefill; ++i) g_store[i] = 999;
    g_foundCount = n; for (int i = 0; i < n; ++i) g_found[i] = found[i];
    wm.GetAdjacentMaps(map, v);
    int count = (int)(v._M_finish - v._M_start);
    for (int i = 0; i < count; ++i) out[i] = v._M_start[i];
    return count;
}

int main()
{
    CMapInfo infos[4]; memset(infos, 0, sizeof infos);
    infos[0].Area.TLX = 0;  infos[0].Area.TLY = 0;  infos[0].Area.BRX = 10; infos[0].Area.BRY = 10;
    infos[2].Area.TLX = 20; infos[2].Area.TLY = 30; infos[2].Area.BRX = 40; infos[2].Area.BRY = 50;
    CWorldMap wm; wm.Maps.Start = infos; wm.Maps.Finish = infos + 4; wm.Maps.EndOfStorage = infos + 4;
    unsigned long out[32];

    unsigned long a[] = { 1, 2, 3 };                         // self (2) in the middle, prefilled vector cleared
    int n = run(wm, 2, a, 3, 5, out);
    check(n == 2 && out[0] == 1 && out[1] == 3, "middle self removed");
    check(g_seenArea.TLX == 19 && g_seenArea.TLY == 29 && g_seenArea.BRX == 41 && g_seenArea.BRY == 51, "grown area");

    n = run(wm, 0, a, 3, 0, out);                             // map 0: grown to -1..11, clipped at 0; no self in list
    check(g_seenArea.TLX == 0 && g_seenArea.TLY == 0 && g_seenArea.BRX == 11 && g_seenArea.BRY == 11, "clipped area");
    check(n == 3 && out[0] == 1 && out[1] == 2 && out[2] == 3, "no self kept all");

    unsigned long b[] = { 0, 0, 5, 0 };                       // self first, repeated, and last
    n = run(wm, 0, b, 4, 2, out);
    check(n == 1 && out[0] == 5, "repeated and trailing self removed");

    n = run(wm, 0, b, 0, 3, out);                             // nothing found
    check(n == 0, "empty result");

    unsigned long c[] = { 0 };                                // only self
    n = run(wm, 0, c, 1, 0, out);
    check(n == 0, "only self removed");

    if (fails) { puts("004fea30_TEST FAIL"); return 1; }
    puts("004fea30_TEST PASS");
    return 0;
}