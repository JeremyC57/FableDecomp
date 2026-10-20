// CWorldMap::GetAdjacentMaps(long, vector<unsigned long>&) const -- collect the maps touching one map: grow its area by one unit on every side, clip it to the world, list the maps in it and drop the map itself
// Leaf: GetAdjacentMaps
// Grade: DIFFER(ours v retail) -- 195v195, register allocation: the clear loads finish into eax and keeps dest in ecx (retail
// start in eax, finish in ecx, result straight from memmove), and the Maps[map] address goes through edx/ebx instead of eax;
// tried the early-out copy-down, the if form, STLport __copy_trivial (`n ? ... : result` folds the compare away) and a ternary
// Retail: Fable.exe @ 0x004FEA30, 195 bytes, `ret 8`. PDB ?GetAdjacentMaps@CWorldMap@@QBEXJAAV?$vector@KV?$allocator@K@std@@@std@@@Z
// Callees: memmove 0x00BFEAE6 (the inlined vector erase copy-downs), CWorldMap virtual slot 13 GetDimensions() const
// (vtable 0x01244AEC -> 0x0051CDE0, returns &Area at +0x5C), C2DBoxI::ClipTo(const C2DBoxI&) 0x0049C810, CWorldMap virtual
// slot 20 GetAllMapsInArea(const C2DBoxI&, vector<unsigned long>&) const 0x004FF150.
// Retail CWorldMap offsets: Maps (CArray<CMapInfo>, 0x48-byte elements, Area at +0) +0x20.
// Shape: the vector is cleared with the inline erase(begin(), end()) (copy-down `if (src != srcEnd) dest = memmove(..)+n;
// return dest;` gives retail's pointer compare and `je`); the area is a copy grown member by member; the self entry is removed with the inline
// single-element erase (copy-down of the tail, finish -= 1) inside a manual `it` loop.
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
        if (src != srcEnd)
        {
            unsigned long count = (char*)srcEnd - (char*)src;
            dest = (unsigned long*)((char*)memmove(dest, src, count) + count);
        }
        return dest;
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
    C2DBoxI area = Maps[map].Area;
    area.TLX -= 1;
    area.TLY -= 1;
    area.BRX += 1;
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
