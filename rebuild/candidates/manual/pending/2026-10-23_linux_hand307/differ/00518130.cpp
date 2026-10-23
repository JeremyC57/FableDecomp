// Grade: DIFFER(526v526) -- after the coordinator's 007FCEF0 recipe (y guard + hoisted level*20 / level*12 offsets + integer-sum indexing) the Width-load SIB residue is gone; the only residue left is the spill-reload order at the level-loop exit (ebp iterator before edi this; retail edi first), same length
// CThingSearchTools::ForEachInRoughArea<CThingPhysical, CThingFilter_None, bool (__fastcall*)(CThingPhysical&)>
// (const C2DBoxI& area, CThingFilter_None filter, bool (__fastcall* const& func)(CThingPhysical&)) -- calls func on every
// physical thing linked into a mapwho cell overlapping the area (each thing once per search: the global search stamp),
// over every overlapping thing map and every mapwho level; true when any call returned true
// Retail: Fable.exe (TLC, VC7.1) @ 0x00518130, `ret 0xc`.
// Callees: CThingSearchTools::GetMapsInArea forwarder 0x0049C6F0 (fills the scratch map vector), GetCellExtents
//   0x0049C700 (the area clipped to the map, relative to its origin, by value), memmove 0x00BFEAE6 (the scratch
//   vector's clear).
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
    unsigned char _pad00[0x6c];
    unsigned long ThingTypeFlags;                          // +0x6c
};
struct CThingPhysical : CThing {};
struct CThingFilter_None { bool operator()(const CThing&) const { return true; } };
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
    template<class T, class F, class G> bool ForEachInRoughArea(const C2DBoxI& area, F filter, const G& func);
};
template<class T, class F, class G> bool CThingSearchTools::ForEachInRoughArea(const C2DBoxI& area, F filter, const G& func)
{
    bool result = false;
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
            if (box.X0 == box.X1 || box.Y0 == box.Y1)
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
                            if ((thing->ThingTypeFlags & 3) == 3 && filter(*thing) && func(*(T*)thing))
                                result = true;
                        }
                    }
                }
            }
            }
        }
    }
    maps->clear();
    return result;
}
template bool CThingSearchTools::ForEachInRoughArea<CThingPhysical, CThingFilter_None, bool (__fastcall*)(CThingPhysical&)>(
    const C2DBoxI& area, CThingFilter_None filter, bool (__fastcall* const& func)(CThingPhysical&));
