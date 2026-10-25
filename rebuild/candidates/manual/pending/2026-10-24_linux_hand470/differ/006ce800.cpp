// CThingSearchTools::ForEachInRoughArea(const C2DBoxI& area, const CThingFilter_IsNot& filter, FrameUpdateThing& func) -- runs the frame-update functor on every thing linked into a mapwho cell overlapping the area (each thing once per search) other than the excluded thing; true when any call returned true
// Grade: DIFFER(535v535) -- scheduling only: at the level-loop exit retail reloads esi (this) before ebp (the map iterator), ours reloads ebp first (every other byte matches)
// Leaf: ??$ForEachInRoughArea@
// Retail: Fable.exe (TLC, VC7.1) @ 0x006CE800, 535 bytes (incl. padding), `__thiscall`, `ret 0xc`; one caller 0x006CC983.
// Callees: GetMapsInArea 0x0049C6F0, GetCellExtents 0x0049C700, CThingFilter_IsNot::operator() 0x006A76A0,
//   (anonymous)::FrameUpdateThing::operator() 0x006CE730 (catalog label "CTCInterface_InsertPropertyToVector" is
//   propagated), memmove 0x00BFEAE6 (the scratch clear()).
// Shape: the landed ForEachInRoughArea 0x00518130 template over plain CThing (mask 0 through the IsThingOfType mask local,
//   which leaves retail's folded `xor cl,cl ; jne`), with the filter and the functor taken by reference and both
//   operators out of line.
#include <string.h>
struct C2DBoxI {
    long X0, Y0, X1, Y1;
    C2DBoxI() {}
    C2DBoxI(long x0, long y0, long x1, long y1) : X0(x0), Y0(y0), X1(x1), Y1(y1) {}
};
struct CThing {
    unsigned char _pad00[0x6c];
    unsigned long ThingTypeFlags;                          // +0x6c
    enum { ThingTypeMask = 0 };
};
template <class T> __forceinline bool IsThingOfType(const CThing* thing)
{
    unsigned long mask = T::ThingTypeMask;
    return (thing->ThingTypeFlags & mask) == mask;
}
class CThingFilter_IsNot {
public:
    CThing* Thing;                                         // +0x00
    bool operator()(const CThing& thing) const;            // 0x006A76A0
};
namespace {
struct FrameUpdateThing {
    bool operator()(CThing* thing);                        // 0x006CE730
};
}
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
    template<class T, class F, class G> bool ForEachInRoughArea(const C2DBoxI& area, const F& filter, G& func);
};
template<class T, class F, class G> bool CThingSearchTools::ForEachInRoughArea(const C2DBoxI& area, const F& filter, G& func)
{
    bool result = false;
    GetMapsInArea(area, &TempMaps);
    ++g_SearchStamps.Searches;
    g_SearchStamps.Found = 0;
    ++g_SearchStamps.Stamp;
    for (CThingMap** it = TempMaps._M_start; it != TempMaps._M_finish; ++it) {
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
                            if (IsThingOfType<T>(thing) && filter(*(T*)thing) && func((T*)thing))
                                result = true;
                        }
                    }
                }
            }
            }
        }
    }
    TempMaps.clear();
    return result;
}
template bool CThingSearchTools::ForEachInRoughArea<CThing, CThingFilter_IsNot, FrameUpdateThing>(const C2DBoxI& area,
    const CThingFilter_IsNot& filter, FrameUpdateThing& func);
