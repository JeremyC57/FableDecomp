// Grade: DIFFER(573v583) -- register assignment: retail keeps this in edi, the map vector in ebp and the map iterator in ebx (ours: ebx/edi/ebp), and its x87-free frame is 0x50 with the inner count and both cell bounds spilled to the dead argument slots; the body (map query, stamps, per-level shifted box, cell lists, the mask-0 type test that leaves xor dl,dl/jne, the filter, the STLport push_back, the scratch clear) is modelled
// CThingSearchTools::FindThingsInRoughArea<CThing, CThingFilter_HasPhysicsMesh>(const C2DBoxI& area, const
// CThingFilter_HasPhysicsMesh& filter, vector<CThing*>& things) -- appends every thing linked into a mapwho cell
// overlapping the area that passes the filter (each thing once per search: the global search stamp), over every
// overlapping thing map and every mapwho level. The catalog's GetPBestThingInRoughArea<HasTrackName...> label is wrong:
// the body has no comparator, calls CThingFilter_HasPhysicsMesh::operator() 0x006A6F50 and pushes into a vector.
// Retail: Fable.exe (TLC, VC7.1) @ 0x0049C490, `ret 0xc`.
// Callees: CThingSearchTools::GetMapsInArea forwarder 0x0049C6F0 (fills the scratch map vector), GetCellExtents
//   0x0049C700 (the area clipped to the map, relative to its origin, by value), memmove 0x00BFEAE6 (the scratch
//   vector's clear), CThingFilter_HasPhysicsMesh::operator() 0x006A6F50, vector<CThing*>::_M_insert_overflow 0x0049C370.
// Globals: the search stamps 0x013B89A8 / +4 / +8, the mapwho level count 0x013BA848, the per-level shift table
//   0x0137DADC (16 bytes per level).
// Layout: CThingSearchTools scratch vector<CThingMap*> +0x0c; CThingMap level infos +0x04 (20 bytes, width +4), cells
//   +0x14 (vector per level, 4-byte list heads); cell node {entry, next}; entry thing +4, search stamp +0x4c;
//   CThing ThingTypeFlags +0x6c (low two bits = physical).
#include <string.h>
struct C2DBoxI {
    long X0, Y0, X1, Y1;
    C2DBoxI() {}
    C2DBoxI(long x0, long y0, long x1, long y1) : X0(x0), Y0(y0), X1(x1), Y1(y1) {}
};
struct CThing {
    enum { TypeMask = 0 };
    unsigned char _pad00[0x6c];
    unsigned long ThingTypeFlags;                          // +0x6c
};
struct CThingFilter_HasPhysicsMesh {
    bool operator()(CThing* thing) const;                  // 0x006A6F50
};
inline void* operator new(unsigned int, void* where) { return where; }
struct __true_type { __true_type() {} };
struct CThingVector {                                      // STLport vector<CThing*>
    CThing** _M_start;
    CThing** _M_finish;
    CThing** _M_end_of_storage;
    void _M_insert_overflow(CThing** position, CThing* const& x, const __true_type&, unsigned long fill_len, bool atend);   // 0x0049C370
    void push_back(CThing* const& x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) CThing*(x);
            ++_M_finish;
        }
        else
            _M_insert_overflow(_M_finish, x, __true_type(), 1, true);
    }
};
struct CThingMapEntry {
    unsigned char _pad00[4];
    CThing*       Thing;                                   // +0x04
    unsigned char _pad08[0x4c - 0x08];
    unsigned long SearchStamp;                             // +0x4c
};
struct CThingMapNode {
    CThingMapEntry* Entry;
    CThingMapNode*  Next;
};
struct CMapwhoLevelInfo { long SingleCell; long Width; long Height; long _pad[2]; };
struct CCellVector { CThingMapNode** _M_start; CThingMapNode** _M_finish; CThingMapNode** _M_end_of_storage; };
struct CThingMap {
    void*             _vfptr;
    CMapwhoLevelInfo* LevelInfo;                           // +0x04
    CMapwhoLevelInfo* LevelInfoEnd;
    CMapwhoLevelInfo* LevelInfoCap;
    void*             _pad10;
    CCellVector*      Cells;                               // +0x14
    CThingMapNode** GetCellPtr(unsigned char level, long x, long y) const
    {
        return &Cells[level]._M_start[y * LevelInfo[level].Width + x];
    }
};
struct SMapwhoLevel { long Shift; long _pad[3]; };
extern SMapwhoLevel g_MapwhoLevels[];                      // 0x0137DADC
extern long g_NumMapwhoLevels;                             // 0x013BA848
struct SSearchStamps { unsigned long Searches; unsigned long Found; unsigned long Stamp; };
extern SSearchStamps g_SearchStamps;                       // 0x013B89A8
struct CMapVector {
    CThingMap** _M_start;
    CThingMap** _M_finish;
    CThingMap** _M_end_of_storage;
    static CThingMap** CopyDown(CThingMap** src, CThingMap** srcEnd, CThingMap** dest)
    {
        if (src == srcEnd)
            return dest;
        unsigned long count = (const char*)srcEnd - (const char*)src;
        return (CThingMap**)((char*)memmove(dest, src, count) + count);
    }
    void erase(CThingMap** first, CThingMap** last) { _M_finish = CopyDown(last, _M_finish, first); }
    void clear() { erase(_M_start, _M_finish); }
};
struct CThingSearchTools {
    void*      _pad00;
    void*      Manager;                                    // +0x04
    void*      _pad08;
    CMapVector TempMaps;                                   // +0x0c
    void GetMapsInArea(const C2DBoxI& area, CMapVector* maps);              // 0x0049C6F0
    C2DBoxI GetCellExtents(CThingMap* map, const C2DBoxI& area);            // 0x0049C700
    template<class T, class F> void FindThingsInRoughArea(const C2DBoxI& area, const F& filter, CThingVector& things);
};
template<class T, class F> void CThingSearchTools::FindThingsInRoughArea(const C2DBoxI& area, const F& filter, CThingVector& things)
{
    CMapVector* maps = &TempMaps;
    GetMapsInArea(area, maps);
    ++g_SearchStamps.Searches;
    g_SearchStamps.Found = 0;
    ++g_SearchStamps.Stamp;
    for (CThingMap** it = maps->_M_start; it != TempMaps._M_finish; ++it) {
        CThingMap* map = *it;
        C2DBoxI extents = GetCellExtents(map, area);
        for (long level = 0; level < g_NumMapwhoLevels; ++level) {
            long shift = g_MapwhoLevels[level].Shift;
            C2DBoxI box(extents.X0 >> shift, extents.Y0 >> shift, ((extents.X1 - 1) >> shift) + 1,
                        ((extents.Y1 - 1) >> shift) + 1);
            long width = box.X1 - box.X0;
            if (width == 0 || box.Y1 - box.Y0 == 0)
                continue;
            for (long y = box.Y0; y < box.Y1; ++y) {
                CThingMapNode** cell = map->GetCellPtr((unsigned char)level, box.X0, y);
                for (long x = box.X0; x < box.X1; ++x, ++cell) {
                    for (CThingMapNode* node = *cell; node; node = node->Next) {
                        CThingMapEntry* entry = node->Entry;
                        if (entry->SearchStamp < g_SearchStamps.Stamp) {
                            entry->SearchStamp = g_SearchStamps.Stamp;
                            CThing* thing = entry->Thing;
                            if ((unsigned char)(thing->ThingTypeFlags & T::TypeMask) == T::TypeMask && filter(thing))
                                things.push_back(thing);
                        }
                    }
                }
            }
        }
    }
    maps->clear();
}
template void CThingSearchTools::FindThingsInRoughArea<CThing, CThingFilter_HasPhysicsMesh>(
    const C2DBoxI& area, const CThingFilter_HasPhysicsMesh& filter, CThingVector& things);
