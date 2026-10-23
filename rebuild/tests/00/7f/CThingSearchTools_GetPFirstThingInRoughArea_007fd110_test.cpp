// Behaviour test: the source model over a 4x4 one-level fake mapwho grid (C++03; g++ and VC7.1).
#include <new>
#include <stdio.h>
#ifndef _MSC_VER
#define __forceinline inline
#endif
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
struct __true_type { __true_type() {} };
struct CThingFilter_HasScriptData {
    bool operator()(const CThing* thing) const;   // 0x006A7E30
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
        return &(*(const CCellVector*)((unsigned long)Cells + cellsOffset))._M_start[y * *(const long*)((unsigned long)offset + (unsigned long)LevelInfo + sizeof(long)) + x];
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
// CThingSearchTools: engine/CThingSearchTools.h names +0x0c "AreaMaps" (vector<NThingMap::CThingMap*>, 16 bytes in
// the PDB); retail's STLport vector is 12 bytes and only the scratch-vector role matters here, so a local model.
struct CThingSearchTools {
    void*      WorldMap;                                   // +0x00
    void*      PThingMapManager;                           // +0x04
    void*      ThingManager;                               // +0x08
    CMapVector AreaMaps;                                   // +0x0c
    void GetMapsInArea(const C2DBoxI& area, CMapVector* maps);              // 0x0049C6F0
    C2DBoxI GetCellExtents(CThingMap* map, const C2DBoxI& area);            // 0x0049C700
    template<class F, class T> bool GetPFirstThingInRoughArea(const C2DBoxI& area, const F& filter, T** out);
};
template<class F, class T> bool CThingSearchTools::GetPFirstThingInRoughArea(const C2DBoxI& area, const F& filter, T** out)
{
    GetMapsInArea(area, &AreaMaps);
    ++g_SearchStamps.Searches;
    g_SearchStamps.Found = 0;
    ++g_SearchStamps.Stamp;
    for (CThingMap** it = AreaMaps._M_start; it != AreaMaps._M_finish; ++it) {
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
                                if (IsThingOfType<T>(thing) && filter(thing)) {
                                *out = static_cast<T*>(thing);
                                return true;
                            }
                            }
                        }
                    }
                }
            }
        }
    }
    AreaMaps.clear();
    return false;
}
template bool CThingSearchTools::GetPFirstThingInRoughArea<CThingFilter_HasScriptData, CThing >(const C2DBoxI& area, const CThingFilter_HasScriptData& filter, CThing** out);
SMapwhoLevel g_MapwhoLevels[2];
long g_NumMapwhoLevels;
SSearchStamps g_SearchStamps;
static CThingMap* g_MapStore[4];
void CThingSearchTools::GetMapsInArea(const C2DBoxI&, CMapVector* maps)
{ maps->_M_finish = maps->_M_start + 1; }
C2DBoxI CThingSearchTools::GetCellExtents(CThingMap*, const C2DBoxI& area) { return area; }
struct TThing : CThing { unsigned char Extra[0x40]; };   // room for the filters' wider views
static TThing g_Things[6];
static bool g_Accept[6];
static int g_Dist[6];
static int ThingIndex(const CThing* thing) { return (int)(static_cast<const TThing*>(thing) - g_Things); }
bool CThingFilter_HasScriptData::operator()(const CThing* thing) const { return g_Accept[ThingIndex(thing)]; }
static CThingMapEntry g_Entries[6];
static CThingMapNode g_Nodes[8];
static CThingMapNode* g_Cells[16];
static CMapwhoLevelInfo g_Info[1];
static CCellVector g_CellVectors[1];
static int g_NodeCount;
static void Put(int thing, long x, long y)
{
    CThingMapNode* node = &g_Nodes[g_NodeCount++];
    node->Entry = &g_Entries[thing];
    node->Next = g_Cells[y * 4 + x];
    g_Cells[y * 4 + x] = node;
}
static int Fail(int line) { printf("FAIL line %d\n", line); return 1; }
#define CHECK(c) if (!(c)) return Fail(__LINE__)
static void Setup()
{
    g_NumMapwhoLevels = 1;
    g_MapwhoLevels[0].Shift = 0;
    g_Info[0].Width = 4;
    g_Info[0].Height = 4;
    g_CellVectors[0]._M_start = g_Cells;
    g_CellVectors[0]._M_finish = g_Cells + 16;
    static CThingMap map;
    map.LevelInfo = g_Info;
    map.Cells = g_CellVectors;
    g_MapStore[0] = &map;
    for (int i = 0; i < 6; ++i) {
        g_Things[i].ThingTypeFlags = CThing::TypeMask | 0x100000;
        g_Things[i].PhysicsTC = 0;
        g_Entries[i].Thing = &g_Things[i];
        g_Entries[i].SearchStamp = 0;
        g_Accept[i] = true;
        g_Dist[i] = 10 - i;
    }
    g_Things[5].ThingTypeFlags = 0;   // wrong type unless the mask is 0
    g_Accept[4] = false;              // rejected by the filter
    g_Dist[0] = 5; g_Dist[1] = 1; g_Dist[5] = 0;
    // area cells x 1..2, y 1..2; thing 0 is in two cells (visited once), 2 and 3 are outside the area
    Put(0, 1, 1); Put(0, 2, 1); Put(5, 2, 1); Put(4, 1, 2); Put(1, 2, 2); Put(2, 3, 3); Put(3, 0, 0);
}
int main()
{
    Setup();
    CThingSearchTools tools;
    CThingMap* storage[4];
    tools.AreaMaps._M_start = storage;
    tools.AreaMaps._M_finish = storage;
    tools.AreaMaps._M_end_of_storage = storage + 4;
    storage[0] = g_MapStore[0];
    C2DBoxI area(1, 1, 3, 3);
    CThingFilter_HasScriptData filter;
    CThing* first = 0;
    bool found = tools.GetPFirstThingInRoughArea<CThingFilter_HasScriptData, CThing >(area, filter, &first);
    CHECK(found && first == &g_Things[0]);
    CHECK(tools.AreaMaps._M_finish != tools.AreaMaps._M_start);   // a hit returns before the clear
    tools.AreaMaps._M_finish = storage;
    g_Accept[0] = false;
    found = tools.GetPFirstThingInRoughArea<CThingFilter_HasScriptData, CThing >(area, filter, &first);
    CHECK(found && first == ((CThing::TypeMask == 0) ? &g_Things[5] : &g_Things[1]));
    printf("007fd110_TEST PASS\n");
    return 0;
}