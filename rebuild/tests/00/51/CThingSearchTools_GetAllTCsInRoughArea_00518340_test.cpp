// Behaviour test for 0x00518340 CThingSearchTools::GetAllTCsInRoughArea<CTCCreatureGenerator, CThingFilter_IsInQuestOrNullQuest,
// std::vector<const CTCCreatureGenerator*> >: the row's defines, includes and explicit instantiation verbatim over the shared headers (+ the
// hand4c overlay opt-in FABLE_RETAIL_CTHINGSEARCHTOOLS_TCSINROUGHAREA_EGOD and engine/CThingFilter_IsInQuestOrNullQuest.h), run on zeroed
// object images (harness of the landed 0x00878430 test: maps B / A / C, layers, stamps). Per thing: a mapwho with no parent thing is skipped;
// the interface bit (UsingComponents word 1 bit 29, TCI 0x3D) gates the lookup; the component is found by binary search in the sorted
// component map (several entries before and after 0x3D); the quest filter (a double: thing word +0x90 == 0) is asked only for things that
// have the component, and a rejection keeps the component out; the result vector grows through STLport's _M_insert_overflow (capacity 0
// at start). Doubles: GetAllThingMapsInArea, ClipAndConvertWorldMapAreaToLocalArea, the filter operator(), the search / layer statics.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
// ---- row body ----
#define FABLE_RETAIL_CVECTORMAP_INLINE_LOWERBOUND
#define FABLE_RETAIL_STLP_VECTOR_OVERFLOW_OUTOFLINE
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_CLIPAREA
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ORIGINAL_ROUGHAREA
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ROUGHAREA_EGOD
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_TCSINROUGHAREA
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_TCSINROUGHAREA_EGOD
#define FABLE_RETAIL_CTHINGMAP_PEEKCELL_INLINE
#define FABLE_RETAIL_CTCMAPWHO
#define FABLE_RETAIL_CTCMAPWHO_SEARCHID
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCCREATUREGENERATOR
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C2DBOXI_RADIUS_INITIALIZERS
#define FABLE_RETAIL_C2DBOXI
#define FABLE_RETAIL_C2DBOXI_COPY
#define FABLE_RETAIL_C2DBOXI_GETWIDTH
#define FABLE_RETAIL_C2DBOXI_GETHEIGHT
#include "engine/C2DExtentsI.h"
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CWorld.h"
class CTCMapwho;
#include "engine/CTCMapwho.h"
#include "engine/NThingMap_CThingMap.h"
#include "engine/CThingSearchTools.h"
#include "engine/C2DBoxI.h"
#include "engine/CTCCreatureGenerator.h"
#include "engine/CThingFilter_IsInQuestOrNullQuest.h"
template void CThingSearchTools::GetAllTCsInRoughArea<CTCCreatureGenerator,CThingFilter_IsInQuestOrNullQuest,std::vector<const CTCCreatureGenerator*> >(const C2DBoxI&,const CThingFilter_IsInQuestOrNullQuest&,std::vector<const CTCCreatureGenerator*>&)const;

// ---- doubles ----
unsigned long CThingSearchTools::NoLookups = 4, CThingSearchTools::NoThingsScanned = 7, CThingSearchTools::SearchID = 0;
namespace NThingMap { long MaxLayers = 2; }
namespace NThingMap { extern const CLayerInfo LayerInfo[3] = { { 0, 0 }, { 0, 1 }, { 0, 0 } }; }   // Size, ScaleShift
static unsigned char g_mapA[0x40], g_mapB[0x40], g_mapC[0x40];
static int g_bad = 0, g_mapCalls = 0, g_clipCalls = 0, g_filterCalls = 0;
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
bool CThingFilter_IsInQuestOrNullQuest::operator()(const CThing& thing) const
{
    ++g_filterCalls;
    return *(const long*)((const char*)&thing + 0x90) == 0;
}

// ---- harness ----
static int g_fail = 0;
static void check(int ok, const char* what) { if (!ok) { printf("FAIL %s\n", what); g_fail = 1; } }
struct SNode { void* mapwho; SNode* next; void* previous; void* cell; };       // NThingMap::CMapwhoNode
enum { NTHINGS = 19 };
static unsigned char things[NTHINGS][0x100], mapwhos[NTHINGS][0x60];
static long entries[NTHINGS][5][2];                         // the component map: { interface type, component }
static unsigned char comps[NTHINGS][5][0x10];
static void SetThing(int i, int has, int quest)
{
    static const long types[5] = { 0x05, 0x21, 0x3d, 0x40, 0x52 };
    int n = (i % 3) + 3;                                    // 3..5 entries, 0x3D always the third
    for (int k = 0; k < n; ++k) { entries[i][k][0] = types[k]; entries[i][k][1] = (long)(size_t)comps[i][k]; }
    *(void**)(things[i] + 0x44) = entries[i]; *(void**)(things[i] + 0x48) = entries[i] + n; *(void**)(things[i] + 0x4c) = entries[i] + n;
    if (has) *(unsigned long*)(things[i] + 0x24) = 0x20000000u | 0x00000004u;
    else *(unsigned long*)(things[i] + 0x24) = 0xdfffffffu;   // every other interface bit, not 0x3D's
    *(long*)(things[i] + 0x90) = quest;
    *(void**)(mapwhos[i] + 0x04) = things[i];               // CTCBase::ParentThing
}
int main()
{
    static long levelInfo[3][5];                            // CMapwhoLevelInfo: Size, Extents.X, Extents.Y, ScaleShift, NoThings
    static SNode* cells0[16]; static SNode* cells1[4]; static SNode* cells2[16];
    static void* cellVectors[3][3]; static void* scratch[4]; static unsigned char toolsImage[0x18];
    static SNode nodes[22];
    static long levelInfoC[3][5]; static SNode* cellsC0[24]; static SNode* cellsC1[6]; static void* cellVectorsC[3][3];
    levelInfo[0][1] = 4; levelInfo[1][1] = 2; levelInfo[2][1] = 4;
    cellVectors[0][0] = cells0; cellVectors[1][0] = cells1; cellVectors[2][0] = cells2;
    *(void**)(g_mapA + 0x04) = levelInfo; *(void**)(g_mapA + 0x08) = levelInfo + 3; *(void**)(g_mapA + 0x0c) = levelInfo + 3;
    *(void**)(g_mapA + 0x14) = cellVectors; *(void**)(g_mapA + 0x18) = cellVectors + 3; *(void**)(g_mapA + 0x1c) = cellVectors + 3;
    levelInfoC[0][1] = 6; levelInfoC[1][1] = 3; levelInfoC[2][1] = 3;
    cellVectorsC[0][0] = cellsC0; cellVectorsC[1][0] = cellsC1; cellVectorsC[2][0] = cellsC1;
    *(void**)(g_mapC + 0x04) = levelInfoC; *(void**)(g_mapC + 0x08) = levelInfoC + 3; *(void**)(g_mapC + 0x0c) = levelInfoC + 3;
    *(void**)(g_mapC + 0x14) = cellVectorsC; *(void**)(g_mapC + 0x18) = cellVectorsC + 3; *(void**)(g_mapC + 0x1c) = cellVectorsC + 3;
    for (int i = 0; i < NTHINGS; ++i) SetThing(i, 1, 0);
    SetThing(2, 0, 0);          // layer 0 cell (1,0): no creature-generator component: out, filter not asked
    SetThing(3, 1, 7);          // layer 0 cell (1,0): component, quest filter rejects: out
    SetThing(15, 0, 7);         // layer 0 cell (2,1): neither
    *(void**)(mapwhos[17] + 0x04) = 0;   // layer 0 cell (2,1): a mapwho with no parent thing: skipped (stamped)
    *(unsigned long*)(mapwhos[10] + 0x4c) = 1; *(unsigned long*)(mapwhos[11] + 0x4c) = 0; *(unsigned long*)(mapwhos[12] + 0x4c) = 5;
    for (int i = 0; i < 15; ++i) nodes[i].mapwho = mapwhos[i];
    nodes[13].mapwho = mapwhos[1];
    nodes[18].mapwho = mapwhos[15]; nodes[19].mapwho = mapwhos[16]; nodes[20].mapwho = mapwhos[17]; nodes[21].mapwho = mapwhos[18];
    cells0[0 * 4 + 0] = &nodes[0];
    cells0[0 * 4 + 1] = &nodes[1]; nodes[1].next = &nodes[2]; nodes[2].next = &nodes[3];
    cells0[0 * 4 + 2] = &nodes[10]; nodes[10].next = &nodes[11]; nodes[11].next = &nodes[12];
    cells0[1 * 4 + 2] = &nodes[4]; nodes[4].next = &nodes[18]; nodes[18].next = &nodes[20]; nodes[20].next = &nodes[19];
    cells0[2 * 4 + 3] = &nodes[5]; nodes[5].next = &nodes[13]; nodes[13].next = &nodes[21];
    cells0[1 * 4 + 0] = &nodes[6];
    cells0[3 * 4 + 1] = &nodes[7];
    cells1[1 * 2 + 1] = &nodes[8];
    cells2[0 * 4 + 1] = &nodes[9];
    nodes[16].mapwho = mapwhos[13]; nodes[17].mapwho = mapwhos[14];
    cellsC1[0 * 3 + 1] = &nodes[16]; cellsC1[1 * 3 + 2] = &nodes[17];
    *(void**)(toolsImage + 0x0c) = scratch; *(void**)(toolsImage + 0x10) = scratch; *(void**)(toolsImage + 0x14) = scratch + 4;
    const CThingSearchTools& tools = *(const CThingSearchTools*)toolsImage;
    static unsigned char filterImage[8];
    const CThingFilter_IsInQuestOrNullQuest& filter = *(const CThingFilter_IsInQuestOrNullQuest*)filterImage;
    std::vector<const CTCCreatureGenerator*> out;
    C2DBoxI area; area.TLX = 0; area.TLY = 0; area.BRX = 3; area.BRY = 3;
    check(sizeof(std::vector<CThing*>) == 12 && sizeof(CThingSearchTools) == 0x18, "layouts");
    tools.GetAllTCsInRoughArea<CTCCreatureGenerator,CThingFilter_IsInQuestOrNullQuest,std::vector<const CTCCreatureGenerator*> >(area, filter, out);
    static const int expect[9] = { 1, 11, 4, 16, 5, 18, 8, 13, 14 };
    int ok = out.size() == 9;
    for (int n = 0; ok && n < 9; ++n)
        if ((const void*)out[n] != (const void*)comps[expect[n]][2]) ok = 0;
    check(!g_bad && g_mapCalls == 1 && g_clipCalls == 3, "map list fetched once, each map clipped once");
    check(ok, "the 0x3D components found, in layer / row / cell / node order, each thing once");
    check(g_filterCalls == 10, "quest filter asked only for things with the component");
    check(*(void**)(toolsImage + 0x10) == scratch, "scratch map list cleared");
    check(CThingSearchTools::NoLookups == 5 && CThingSearchTools::SearchID == 1 && CThingSearchTools::NoThingsScanned == 0, "search statics");
    static const int stamped[] = { 1, 2, 3, 4, 5, 8, 11, 13, 14, 15, 16, 17, 18 };
    for (int i = 0; i < 13; ++i) check(*(unsigned long*)(mapwhos[stamped[i]] + 0x4c) == 1, "scanned components stamped");
    static const int unvisited[] = { 0, 6, 7, 9 };
    for (int i = 0; i < 4; ++i) check(*(unsigned long*)(mapwhos[unvisited[i]] + 0x4c) == 0, "cells outside the area / layers not visited");
    check(*(unsigned long*)(mapwhos[12] + 0x4c) == 5, "a later stamp is left alone");
    if (!g_fail) printf("00518340_TEST PASS\n");
    return g_fail;
}