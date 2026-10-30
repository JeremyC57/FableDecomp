// Leaf: ??$FindThingsInRoughArea@
// Grade: DIFFER IMPROVED (14 -> 12 difflines, all but two are relocations): building the found pointer before the filter test (if (IsThingOfType<T>) { T* found = ...; if (filter(found)) push_back }) puts the thing spill at [esp+38h] before the filter call as retail. Left: the outer-loop exit reloads it ([esp+24h]) before this ([esp+38h]); retail reloads this first. Tried: AreaMaps._M_finish != it (16), maps->_M_finish (126), it declared before the loop (=). Leaf fixed to ??$FindThingsInRoughArea@ (the parked leaf matched no symbol).
#include <string.h>
#include <stdlib.h>
struct C2DBoxI {
    long X0, Y0, X1, Y1;
    C2DBoxI() {}
    C2DBoxI(long x0, long y0, long x1, long y1) : X0(x0), Y0(y0), X1(x1), Y1(y1) {}
};
// Retail CThing: ThingTypeFlags at +0x6c (THING_TYPE_* inheritance bits); TypeMask selects the searched class.
struct CThing {
    enum { TypeMask = 0 };
    unsigned char _pad00[0x60];
    struct CTCPhysicsBase* PhysicsTC;                      // +0x60
    unsigned char _pad64[0x6c - 0x64];
    unsigned long ThingTypeFlags;                          // +0x6c
};
// The type test: the mask is copied into a local, so the CThing (mask 0) test folds late and keeps retail's
// `xor dl,dl ; jne`, and a byte-sized mask compares through the low byte (`and edx,0Fh ; cmp dl,0Fh`).
template <class T> inline bool IsThingOfType(const CThing* thing)
{
    unsigned long mask = T::TypeMask;
    return (thing->ThingTypeFlags & mask) == mask;
}
inline void* operator new(unsigned int, void* where) { return where; }
struct __true_type { __true_type() {} };
struct CThingFilter_HasDefGlobalIndex {
    bool operator()(const CThing* thing) const;   // 0x006A70B0
};
// One mapwho cell list node: {entry, next}; the entry holds the thing (+4) and its search stamp (+0x4c).
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
// A thing map: per-level grid infos (20 bytes, width at +4) at +0x04, per-level cell vectors at +0x14.
struct CMapwhoLevelInfo { long SingleCell; long Width; long Height; long _pad[2]; };
struct CCellVector { CThingMapNode** _M_start; CThingMapNode** _M_finish; CThingMapNode** _M_end_of_storage; };
struct CThingMap {
    void*             _vfptr;
    CMapwhoLevelInfo* LevelInfo;                           // +0x04
    CMapwhoLevelInfo* LevelInfoEnd;
    CMapwhoLevelInfo* LevelInfoCap;
    void*             _pad10;
    CCellVector*      Cells;                               // +0x14
    // offset = level * sizeof(CMapwhoLevelInfo), cellsOffset = level * sizeof(CCellVector): the caller's user variables
    // (Width first by offset, the cell row first by the Cells pointer) are what give retail's SIB base/index order.
    CThingMapNode** GetCellPtr(long cellsOffset, long offset, long x, long y) const
    {
        return &(*(const CCellVector*)((unsigned long)Cells + cellsOffset))._M_start[y * *(const long*)((unsigned long)offset + (unsigned long)LevelInfo + 4) + x];
    }
};
struct SMapwhoLevel { long Shift; long _pad[3]; };
extern SMapwhoLevel g_MapwhoLevels[];                      // 0x0137DADC
extern long g_NumMapwhoLevels;                             // 0x013BA848
struct SSearchStamps { unsigned long Searches; unsigned long Found; unsigned long Stamp; };
extern SSearchStamps g_SearchStamps;                       // 0x013B89A8
// STLport vector<CThingMap*> (the search tools' scratch map list); clear() is erase(begin, end) over memmove.
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
// STLport vector<CThing*>; the overflow path is out of line (a same-TU stand-in below, so VC7.1 knows it
// only writes the finish pointer and homes the __true_type tag in a live argument slot as retail does).
struct CResultVector {
    CThing** _M_start;
    CThing** _M_finish;
    CThing** _M_end_of_storage;
    void _M_insert_overflow(CThing** position, CThing* const& x, const __true_type&, unsigned long fill_len, bool atend);   // 0x00436AC0
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
#pragma auto_inline(off)
void CResultVector::_M_insert_overflow(CThing** position, CThing* const& x, const __true_type&, unsigned long fill_len, bool atend)
{
    if (atend && fill_len == 1)
        _M_finish = position + (x != 0);
}
#pragma auto_inline(on)
// CThingSearchTools: engine/CThingSearchTools.h names +0x0c "AreaMaps" (vector<NThingMap::CThingMap*>, 16 bytes in
// the PDB); retail's STLport vector is 12 bytes and only the scratch-vector role matters here, so a local model.
struct CThingSearchTools {
    void*      WorldMap;                                   // +0x00
    void*      PThingMapManager;                           // +0x04
    void*      ThingManager;                               // +0x08
    CMapVector AreaMaps;                                   // +0x0c
    void GetMapsInArea(const C2DBoxI& area, CMapVector* maps);              // 0x0049C6F0
    C2DBoxI GetCellExtents(CThingMap* map, const C2DBoxI& area);            // 0x0049C700
    template<class T, class F> void FindThingsInRoughArea(const C2DBoxI& area, const F& filter, CResultVector& things);
};
template<class T, class F> void CThingSearchTools::FindThingsInRoughArea(const C2DBoxI& area, const F& filter, CResultVector& things)
{
    CMapVector* maps = &AreaMaps;
    GetMapsInArea(area, maps);
    ++g_SearchStamps.Searches;
    g_SearchStamps.Found = 0;
    ++g_SearchStamps.Stamp;
    for (CThingMap** it = maps->_M_start; it != AreaMaps._M_finish; ++it) {
        CThingMap* map = *it;
        C2DBoxI extents = GetCellExtents(map, area);
        for (long level = 0; level < g_NumMapwhoLevels; ++level) {
            long shift = g_MapwhoLevels[level].Shift;
            C2DBoxI box(extents.X0 >> shift, extents.Y0 >> shift, ((extents.X1 - 1) >> shift) + 1,
                        ((extents.Y1 - 1) >> shift) + 1);
            long width = box.X1 - box.X0;
            if (width == 0 || box.Y1 - box.Y0 == 0)
                continue;
            long y = box.Y0;
            if (y < box.Y1) {
                long offset = (unsigned char)level * (long)sizeof(CMapwhoLevelInfo);
                long cellsOffset = (unsigned char)level * (long)sizeof(CCellVector);
                for (; y < box.Y1; ++y) {
                    CThingMapNode** cell = map->GetCellPtr(cellsOffset, offset, box.X0, y);
                    for (long x = box.X0; x < box.X1; ++x, ++cell) {
                        for (CThingMapNode* node = *cell; node; node = node->Next) {
                            CThingMapEntry* entry = node->Entry;
                            if (entry->SearchStamp < g_SearchStamps.Stamp) {
                                entry->SearchStamp = g_SearchStamps.Stamp;
                                CThing* thing = entry->Thing;
                                if (IsThingOfType<T>(thing)) {
                                    T* found = static_cast<T*>(thing);
                                    if (filter(found))
                                        things.push_back(found);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    maps->clear();
}
template void CThingSearchTools::FindThingsInRoughArea<CThing, CThingFilter_HasDefGlobalIndex >(const C2DBoxI& area, const CThingFilter_HasDefGlobalIndex& filter, CResultVector& things);
