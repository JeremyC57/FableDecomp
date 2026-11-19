// Behaviour test for 0x0087A7C0 CThingSearchTools::GetAllThingsInRoughArea<const CThingCreatureBase, std::vector<const CThing*>,
// CThingFilter_IsNot>: the row's defines, includes and instantiation verbatim over the shared headers, run on zeroed
// object images. Three maps: B (local area of width 0, NULL tables), A (layers 0 and 1 of three; layer 1 at shift 1; area moved one
// cell right) and C (odd TLY / BRX on layer 1). Node chains test the search-id stamp (<), the creature type mask (flags & 15 == 15:
// 0x0F and 0x1F pass, 0x07 and 0x0E fail), the IsNot filter (the excluded creature is stamped but not collected), the visit order,
// append-after-existing-elements, growth past the vector's capacity (out-of-line overflow) and the scratch clear.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
// ---- row body ----
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_STLP_VECTOR_OVERFLOW_OUTOFLINE
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_CLIPAREA
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ORIGINAL_ROUGHAREA
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ROUGHAREA_EGOD
#define FABLE_RETAIL_CTHINGMAP_PEEKCELL_INLINE
#define FABLE_RETAIL_CTCMAPWHO
#define FABLE_RETAIL_CTCMAPWHO_SEARCHID
#define FABLE_RETAIL_CTHINGCREATUREBASE_STATTYPEFLAGS
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_C2DBOXI
#define FABLE_RETAIL_C2DBOXI_COPY
#define FABLE_RETAIL_C2DBOXI_GETWIDTH
#define FABLE_RETAIL_C2DBOXI_GETHEIGHT
#include "engine/C2DExtentsI.h"
#include "engine/CThing.h"
class CTCMapwho;
#include "engine/CTCMapwho.h"
#include "engine/NThingMap_CThingMap.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingSearchTools.h"
#include "engine/C2DBoxI.h"
#include "engine/CThingFilter_IsNot.h"
template void CThingSearchTools::GetAllThingsInRoughArea<const CThingCreatureBase, std::vector<const CThing*>, CThingFilter_IsNot>(const C2DBoxI&, const CThingFilter_IsNot&, std::vector<const CThing*>&) const;

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
    else { local.TLX = 4; local.TLY = 0; local.BRX = 4; local.BRY = 2; }
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

// ---- harness ----
static int g_fail = 0;
static void check(int ok, const char* what) { if (!ok) { printf("FAIL %s\n", what); g_fail = 1; } }
struct SNode { void* mapwho; SNode* next; void* previous; void* cell; };       // NThingMap::CMapwhoNode
enum { NTHINGS = 17 };
static unsigned char things[NTHINGS][0x400], mapwhos[NTHINGS][0x60];
static void SetThing(int i, unsigned long flags) { *(void**)(mapwhos[i] + 0x04) = things[i]; *(unsigned long*)(things[i] + 0x6c) = flags; }
int main()
{
    static long levelInfo[3][5];
    static SNode* cells0[16]; static SNode* cells1[4]; static SNode* cells2[16];
    static void* cellVectors[3][3]; static void* scratch[4]; static unsigned char toolsImage[0x18];
    static SNode nodes[20];
    static long levelInfoC[3][5]; static SNode* cellsC0[24]; static SNode* cellsC1[6]; static void* cellVectorsC[3][3];
    levelInfo[0][1] = 4; levelInfo[1][1] = 2; levelInfo[2][1] = 4;
    cellVectors[0][0] = cells0; cellVectors[1][0] = cells1; cellVectors[2][0] = cells2;
    *(void**)(g_mapA + 0x04) = levelInfo; *(void**)(g_mapA + 0x08) = levelInfo + 3; *(void**)(g_mapA + 0x0c) = levelInfo + 3;
    *(void**)(g_mapA + 0x14) = cellVectors; *(void**)(g_mapA + 0x18) = cellVectors + 3; *(void**)(g_mapA + 0x1c) = cellVectors + 3;
    levelInfoC[0][1] = 6; levelInfoC[1][1] = 3; levelInfoC[2][1] = 3;
    cellVectorsC[0][0] = cellsC0; cellVectorsC[1][0] = cellsC1; cellVectorsC[2][0] = cellsC1;
    *(void**)(g_mapC + 0x04) = levelInfoC; *(void**)(g_mapC + 0x08) = levelInfoC + 3; *(void**)(g_mapC + 0x0c) = levelInfoC + 3;
    *(void**)(g_mapC + 0x14) = cellVectorsC; *(void**)(g_mapC + 0x18) = cellVectorsC + 3; *(void**)(g_mapC + 0x1c) = cellVectorsC + 3;
    for (int i = 0; i < NTHINGS; ++i) { SetThing(i, 0x0f); nodes[i].mapwho = mapwhos[i]; }
    SetThing(15, 0x07); SetThing(16, 0x1f); SetThing(3, 0x0e);
    // 0 cell (0,0) left of the area; 1 cell (1,0) found; 2 cell (1,0) the excluded creature; 3 cell (1,0) flags 0x0E: not a creature;
    // 15 cell (1,0) flags 0x07: not a creature; 16 cell (1,0) flags 0x1F: found; 4 cell (2,1); 5 cell (3,2) (+ thing 1 again);
    // 6 cell (0,1) past the right edge; 7 cell (1,3) bottom edge row; 8 layer 1 cell (1,1); 9 layer 2 (not scanned);
    // 10 / 11 / 12 cell (2,0): stamp equal (skipped) / one less (found) / greater (skipped); 13 / 14 map C layer 1 cells (1,0) / (2,1).
    g_excluded = (const CThing*)things[2];
    *(unsigned long*)(mapwhos[10] + 0x4c) = 1; *(unsigned long*)(mapwhos[11] + 0x4c) = 0; *(unsigned long*)(mapwhos[12] + 0x4c) = 5;
    cells0[0 * 4 + 0] = &nodes[0];
    cells0[0 * 4 + 1] = &nodes[1]; nodes[1].next = &nodes[2]; nodes[2].next = &nodes[3]; nodes[3].next = &nodes[15]; nodes[15].next = &nodes[16];
    cells0[0 * 4 + 2] = &nodes[10]; nodes[10].next = &nodes[11]; nodes[11].next = &nodes[12];
    cells0[1 * 4 + 2] = &nodes[4];
    cells0[2 * 4 + 3] = &nodes[5]; nodes[5].next = &nodes[17]; nodes[17].mapwho = mapwhos[1];
    cells0[1 * 4 + 0] = &nodes[6];
    cells0[3 * 4 + 1] = &nodes[7];
    cells1[1 * 2 + 1] = &nodes[8];
    cells2[0 * 4 + 1] = &nodes[9];
    cellsC1[0 * 3 + 1] = &nodes[13]; cellsC1[1 * 3 + 2] = &nodes[14];
    *(void**)(toolsImage + 0x0c) = scratch; *(void**)(toolsImage + 0x10) = scratch; *(void**)(toolsImage + 0x14) = scratch + 4;
    check(sizeof(std::vector<CThing*>) == 12 && sizeof(CThingSearchTools) == 0x18, "layouts");
    const CThingSearchTools& tools = *(const CThingSearchTools*)toolsImage;
    CThingFilter_IsNot filter(*g_excluded);
    std::vector<const CThing*> out;
    const CThing* sentinel = (const CThing*)things[0];
    out.push_back(sentinel);                                // existing element kept; capacity 1 -> the search must grow it
    C2DBoxI area; area.TLX = 0; area.TLY = 0; area.BRX = 3; area.BRY = 3;
    tools.GetAllThingsInRoughArea<const CThingCreatureBase, std::vector<const CThing*>, CThingFilter_IsNot>(area, filter, out);
    static const int expect[8] = { 0, 1, 16, 11, 4, 5, 8, 13 };
    int ok = out.size() == 9;
    for (int i = 0; i < 8 && ok; ++i) if (out[i] != (const CThing*)things[expect[i]]) ok = 0;
    if (ok && out[8] != (const CThing*)things[14]) ok = 0;
    check(ok, "creatures appended once each, in layer / row / cell / node order, after the existing element");
    check(!g_bad && g_mapCalls == 1 && g_clipCalls == 3, "map list fetched once, each map clipped once, filter bound to the excluded thing");
    check(g_filterCalls == 9, "filter called only on creatures (type mask before filter)");
    check(*(void**)(toolsImage + 0x10) == scratch, "scratch map list cleared");
    check(CThingSearchTools::NoLookups == 5 && CThingSearchTools::SearchID == 1 && CThingSearchTools::NoThingsScanned == 0, "search statics");
    static const int stamped[] = { 1, 2, 3, 4, 5, 8, 11, 13, 14, 15, 16 };
    for (int i = 0; i < 11; ++i) check(*(unsigned long*)(mapwhos[stamped[i]] + 0x4c) == 1, "scanned components stamped");
    static const int unvisited[] = { 0, 6, 7, 9 };
    for (int i = 0; i < 4; ++i) check(*(unsigned long*)(mapwhos[unvisited[i]] + 0x4c) == 0, "cells outside the area / layers not visited");
    check(*(unsigned long*)(mapwhos[12] + 0x4c) == 5 && *(unsigned long*)(mapwhos[10] + 0x4c) == 1, "equal / later stamps left alone");
    if (!g_fail) printf("0087a7c0_TEST PASS\n");
    return g_fail;
}