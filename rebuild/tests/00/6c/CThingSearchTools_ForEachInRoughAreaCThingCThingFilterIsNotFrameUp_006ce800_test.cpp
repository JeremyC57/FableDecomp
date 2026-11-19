// Behaviour test for 0x006CE800 CThingSearchTools::ForEachInRoughArea<CThing, CThingFilter_IsNot, FrameUpdateThing>: the row's defines,
// includes and body verbatim over the shared headers, run on zeroed object images. Three maps: B (local area of width 0, NULL tables:
// any cell lookup faults), A (layers 0 and 1 scanned of three; layer 1 at shift 1, area moved one cell right by the ClipAndConvert
// double) and C (odd TLY / BRX: layer 1's TLY shift and BRX round-up decide two things). Node chains test the search-id stamp (<:
// equal and greater are skipped), the IsNot filter (the excluded thing never reaches the functor), the functor's result (false in
// run 1 -> false; true for one thing in run 2 -> true, with the scan carrying on past it), the visit order and the scratch clear.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
// ---- row body ----
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_CLIPAREA
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ORIGINAL_ROUGHAREA
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ROUGHAREA_EGOD
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_FOREACHINROUGHAREA
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_FOREACHINROUGHAREA_CONSTPROC
#define FABLE_RETAIL_CTHINGMAP_PEEKCELL_INLINE
#define FABLE_RETAIL_CTCMAPWHO
#define FABLE_RETAIL_CTCMAPWHO_SEARCHID
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_STATTYPEFLAGS
#define FABLE_RETAIL_C2DBOXI
#define FABLE_RETAIL_C2DBOXI_COPY
#define FABLE_RETAIL_C2DBOXI_GETWIDTH
#define FABLE_RETAIL_C2DBOXI_GETHEIGHT
#include "engine/C2DExtentsI.h"
#include "engine/CThing.h"
#include "engine/CWorld.h"
class CTCMapwho;
#include "engine/CTCMapwho.h"
#include "engine/NThingMap_CThingMap.h"
#include "engine/CThingSearchTools.h"
#include "engine/C2DBoxI.h"
#include "engine/CThingFilter_IsNot.h"
class CSteeringAgent;
namespace {
struct FrameUpdateThing {
    std::vector<const CSteeringAgent*>& Agents;
    FrameUpdateThing(std::vector<const CSteeringAgent*>& agents) : Agents(agents) {}
    bool operator()(const CThing& thing) const;            // 0x006CE730
};
}
template<class T,class Filter,class Proc>
bool CThingSearchTools::ForEachInRoughArea(const C2DBoxI& area,const Filter& filter_func,const Proc& proc) const
{
    bool result=false;
    unsigned long flags=T::StatGetTypeFlags();
    GetAllThingMapsInArea(area,const_cast<std::vector<NThingMap::CThingMap*>&>(AreaMaps));
    ++NoLookups;
    NoThingsScanned=0;
    ++SearchID;
    for(std::vector<NThingMap::CThingMap*>::const_iterator it=AreaMaps.begin();it!=AreaMaps.end();++it) {
        NThingMap::CThingMap& thing_map=**it;
        C2DBoxI local_area=ClipAndConvertWorldMapAreaToLocalArea(thing_map,area);
        for(long c1=0;c1<NThingMap::MaxLayers;++c1) {
            C2DBoxI clipped_area=GetMapwhoArea(thing_map,c1,local_area);
            if(clipped_area.TLX==clipped_area.BRX || clipped_area.TLY==clipped_area.BRY)
                continue;
            for(long cy=clipped_area.TLY;cy<clipped_area.BRY;++cy) {
                const NThingMap::CThingMapCell* pcell=&thing_map.PeekCell((unsigned char)c1,clipped_area.TLX,cy);
                for(long cx=clipped_area.TLX;cx<clipped_area.BRX;++cx,++pcell) {
                    for(const NThingMap::CMapwhoNode* ploop_node=pcell->PFirstNode;ploop_node;ploop_node=ploop_node->PNext) {
                        CTCMapwho* pmapwho=ploop_node->PMapwho;
                        if(pmapwho->GetSearchID()<SearchID) {
                            pmapwho->SetSearchID(SearchID);
                            CThing* loop_thing=&pmapwho->GetParentThing();
                            if((loop_thing->GetTypeFlags()&flags)==flags) {
                                T* tthing=static_cast<T*>(loop_thing);
                                if(filter_func(*tthing) && proc(*tthing))
                                    result=true;
                            }
                        }
                    }
                }
            }
        }
    }
    const_cast<std::vector<NThingMap::CThingMap*>&>(AreaMaps).clear();
    return result;
}
template bool CThingSearchTools::ForEachInRoughArea<CThing, CThingFilter_IsNot, FrameUpdateThing>(const C2DBoxI& area,
    const CThingFilter_IsNot& filter, const FrameUpdateThing& proc) const;

// ---- doubles ----
unsigned long CThingSearchTools::NoLookups = 4, CThingSearchTools::NoThingsScanned = 7, CThingSearchTools::SearchID = 0;
namespace NThingMap { long MaxLayers = 2; }
namespace NThingMap { extern const CLayerInfo LayerInfo[3] = { { 0, 0 }, { 0, 1 }, { 0, 0 } }; }   // Size, ScaleShift
static unsigned char g_mapA[0x40], g_mapB[0x40], g_mapC[0x40];
static int g_bad = 0, g_mapCalls = 0, g_clipCalls = 0;
void CThingSearchTools::GetAllThingMapsInArea(const C2DBoxI& area, std::vector<NThingMap::CThingMap*>& maps) const
{
    ++g_mapCalls;
    void*** v = (void***)&maps;                             // STLport vector: start, finish, end of storage
    if (area.TLX != 0 || area.TLY != 0 || area.BRX != 3 || area.BRY != 3 || v[0] != v[1]) g_bad = 1;
    v[0][0] = g_mapB; v[0][1] = g_mapA; v[0][2] = g_mapC; v[1] = v[0] + 3;
}
C2DBoxI CThingSearchTools::ClipAndConvertWorldMapAreaToLocalArea(const NThingMap::CThingMap& map, const C2DBoxI& area) const
{
    ++g_clipCalls;
    C2DBoxI local;
    if ((const void*)&map == (const void*)g_mapC) { local.TLX = 1; local.TLY = 1; local.BRX = 5; local.BRY = 3; }
    else if ((const void*)&map == (const void*)g_mapA) { local.TLX = area.TLX + 1; local.TLY = area.TLY; local.BRX = area.BRX + 1; local.BRY = area.BRY; }
    else { local.TLX = 4; local.TLY = 0; local.BRX = 4; local.BRY = 2; }   // width 0 on layer 0 and on layer 1
    return local;
}
static const CThing* g_excluded = 0;
static int g_filterCalls = 0;
bool CThingFilter_IsNot::operator()(const CThing& thing) const
{
    ++g_filterCalls;
    if (&Thing != g_excluded) g_bad = 1;
    return &thing != &Thing;
}
static const CThing* g_visited[32];
static int g_nvisited = 0;
static const CThing* g_trueFor = 0;
bool FrameUpdateThing::operator()(const CThing& thing) const
{
    if (g_nvisited < 32) g_visited[g_nvisited] = &thing;
    ++g_nvisited;
    return &thing == g_trueFor;
}

// ---- harness ----
static int g_fail = 0;
static void check(int ok, const char* what) { if (!ok) { printf("FAIL %s\n", what); g_fail = 1; } }
struct SNode { void* mapwho; SNode* next; void* previous; void* cell; };       // NThingMap::CMapwhoNode
enum { NTHINGS = 15 };
static unsigned char things[NTHINGS][0x100], mapwhos[NTHINGS][0x60];
static void SetThing(int i) { *(void**)(mapwhos[i] + 0x04) = things[i]; *(unsigned long*)(things[i] + 0x6c) = 0x1234u * i; }
static long levelInfo[3][5];                                // CMapwhoLevelInfo: Size, Extents.X, Extents.Y, ScaleShift, NoThings
static SNode* cells0[16]; static SNode* cells1[4]; static SNode* cells2[16];
static void* cellVectors[3][3]; static void* scratch[4]; static unsigned char toolsImage[0x18];
static SNode nodes[18];
static long levelInfoC[3][5]; static SNode* cellsC0[24]; static SNode* cellsC1[6]; static void* cellVectorsC[3][3];
static std::vector<const CSteeringAgent*> agents;
static bool Run(const CThing* trueFor)
{
    g_trueFor = trueFor; g_nvisited = 0; g_filterCalls = 0;
    const CThingSearchTools& tools = *(const CThingSearchTools*)toolsImage;
    CThingFilter_IsNot filter(*g_excluded);
    FrameUpdateThing proc(agents);
    C2DBoxI area; area.TLX = 0; area.TLY = 0; area.BRX = 3; area.BRY = 3;
    return tools.ForEachInRoughArea<CThing, CThingFilter_IsNot, FrameUpdateThing>(area, filter, proc);
}
int main()
{
    levelInfo[0][1] = 4; levelInfo[1][1] = 2; levelInfo[2][1] = 4;
    cellVectors[0][0] = cells0; cellVectors[1][0] = cells1; cellVectors[2][0] = cells2;
    *(void**)(g_mapA + 0x04) = levelInfo; *(void**)(g_mapA + 0x08) = levelInfo + 3; *(void**)(g_mapA + 0x0c) = levelInfo + 3;
    *(void**)(g_mapA + 0x14) = cellVectors; *(void**)(g_mapA + 0x18) = cellVectors + 3; *(void**)(g_mapA + 0x1c) = cellVectors + 3;
    levelInfoC[0][1] = 6; levelInfoC[1][1] = 3; levelInfoC[2][1] = 3;
    cellVectorsC[0][0] = cellsC0; cellVectorsC[1][0] = cellsC1; cellVectorsC[2][0] = cellsC1;
    *(void**)(g_mapC + 0x04) = levelInfoC; *(void**)(g_mapC + 0x08) = levelInfoC + 3; *(void**)(g_mapC + 0x0c) = levelInfoC + 3;
    *(void**)(g_mapC + 0x14) = cellVectorsC; *(void**)(g_mapC + 0x18) = cellVectorsC + 3; *(void**)(g_mapC + 0x1c) = cellVectorsC + 3;
    for (int i = 0; i < NTHINGS; ++i) { SetThing(i); nodes[i].mapwho = mapwhos[i]; }
    // 0 layer 0 cell (0,0): left of the area; 1 cell (1,0): visited (listed again in (3,2)); 2 cell (1,0): the excluded thing;
    // 3 cell (1,0): visited; 4 cell (2,1); 5 cell (3,2): last cell; 6 cell (0,1): past the right edge; 7 cell (1,3): bottom edge row;
    // 8 layer 1 cell (1,1); 9 layer 2: not scanned; 10 stamp equal: skipped; 11 stamp one less: visited; 12 stamp greater: skipped;
    // 13 map C layer 1 cell (1,0) (row TLY 1 >> 1); 14 map C layer 1 cell (2,1) (column = the rounded-up BRX 3).
    g_excluded = (const CThing*)things[2];
    *(unsigned long*)(mapwhos[10] + 0x4c) = 1; *(unsigned long*)(mapwhos[11] + 0x4c) = 0; *(unsigned long*)(mapwhos[12] + 0x4c) = 5;
    cells0[0 * 4 + 0] = &nodes[0];
    cells0[0 * 4 + 1] = &nodes[1]; nodes[1].next = &nodes[2]; nodes[2].next = &nodes[3];
    cells0[0 * 4 + 2] = &nodes[10]; nodes[10].next = &nodes[11]; nodes[11].next = &nodes[12];
    cells0[1 * 4 + 2] = &nodes[4];
    cells0[2 * 4 + 3] = &nodes[5]; nodes[5].next = &nodes[15]; nodes[15].mapwho = mapwhos[1];
    cells0[1 * 4 + 0] = &nodes[6];
    cells0[3 * 4 + 1] = &nodes[7];
    cells1[1 * 2 + 1] = &nodes[8];
    cells2[0 * 4 + 1] = &nodes[9];
    cellsC1[0 * 3 + 1] = &nodes[13]; cellsC1[1 * 3 + 2] = &nodes[14];
    *(void**)(toolsImage + 0x0c) = scratch; *(void**)(toolsImage + 0x10) = scratch; *(void**)(toolsImage + 0x14) = scratch + 4;
    check(sizeof(std::vector<CThing*>) == 12 && sizeof(CThingSearchTools) == 0x18, "layouts");

    // run 1: the functor never returns true
    bool r1 = Run(0);
    static const int expect[7] = { 1, 3, 11, 4, 5, 8, 13 };
    int order_ok = g_nvisited == 8;
    for (int i = 0; i < 7 && order_ok; ++i) if (g_visited[i] != (const CThing*)things[expect[i]]) order_ok = 0;
    if (order_ok && g_visited[7] != (const CThing*)things[14]) order_ok = 0;
    check(!r1, "no functor true -> false");
    check(!g_bad && g_mapCalls == 1 && g_clipCalls == 3, "map list fetched once, each map clipped once, filter bound to the excluded thing");
    check(order_ok, "functor called on each filtered thing once, in layer / row / cell / node order");
    check(g_filterCalls == 9, "filter called on every newly stamped thing");
    check(*(void**)(toolsImage + 0x10) == scratch, "scratch map list cleared");
    check(CThingSearchTools::NoLookups == 5 && CThingSearchTools::SearchID == 1 && CThingSearchTools::NoThingsScanned == 0, "search statics");
    static const int stamped[] = { 1, 2, 3, 4, 5, 8, 11, 13, 14 };
    for (int i = 0; i < 9; ++i) check(*(unsigned long*)(mapwhos[stamped[i]] + 0x4c) == 1, "scanned components stamped");
    static const int unvisited[] = { 0, 6, 7, 9 };
    for (int i = 0; i < 4; ++i) check(*(unsigned long*)(mapwhos[unvisited[i]] + 0x4c) == 0, "cells outside the area / layers not visited");
    check(*(unsigned long*)(mapwhos[12] + 0x4c) == 5 && *(unsigned long*)(mapwhos[10] + 0x4c) == 1, "equal / later stamps left alone");

    // run 2: the functor returns true for thing 3 only; the scan still visits everything after it
    *(unsigned long*)(mapwhos[10] + 0x4c) = 9;              // now above the next search id (2): skipped again
    g_mapCalls = 0; g_clipCalls = 0;
    bool r2 = Run((const CThing*)things[3]);
    check(r2, "one functor true -> true");
    check(g_nvisited == 8 && g_visited[7] == (const CThing*)things[14], "scan continues after a true result");
    check(CThingSearchTools::SearchID == 2 && CThingSearchTools::NoLookups == 6 && g_mapCalls == 1 && g_clipCalls == 3, "second search");

    // run 3: the functor returns true for the last thing only (result set late, not reset)
    bool r3 = Run((const CThing*)things[14]);
    check(r3, "last functor true -> true");
    bool r4 = Run((const CThing*)things[0]);                // thing 0 is outside the area: never true
    check(!r4, "result starts false each search");
    if (!g_fail) printf("006ce800_TEST PASS\n");
    return g_fail;
}