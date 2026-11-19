// Behaviour test for 0x0063FD40 CThingSearchTools::GetAllThingsInRoughArea<CThing, std::list<CThing*>, CFilter_Bind2<CThingFilter_OverlappingCircle,
// CWorldMap::CThingFilterBase> >: the row's defines, includes and explicit instantiation verbatim over the shared headers (+ the hand4c
// overlay opt-in FABLE_RETAIL_CTHINGFILTER_OVERLAPPINGCIRCLE_BODY), run on zeroed object images (harness of the landed twin 0x00878430's
// test). Three maps: B (width 0 on both layers, NULL tables: any cell lookup faults), A (two of three layers scanned, layer 1 at shift 1,
// local area moved one cell right), C (odd TLY / BRX: the GetMapwhoArea shift and round-up decide two finds). Node chains test the
// search-id stamp (<: equal / greater skipped), the inline circle filter (no physics component -> out; a thing exactly on the summed
// radius -> out (strict <); Z is ignored (2D distance)), the virtual world-map filter (asked only for things inside the circle; a
// rejection keeps the thing out). Doubles: GetAllThingMapsInArea, ClipAndConvertWorldMapAreaToLocalArea, CTCPhysicsBase::GetMapwhoPosition,
// the search / layer statics; the world-map filter is a CWorldMap::CThingFilterBase subclass.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
// ---- row body ----
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLDMAP
#define FABLE_RETAIL_CWORLDMAP_HEIGHT_BELOW
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_CLIPAREA
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ORIGINAL_ROUGHAREA
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ROUGHAREA_EGOD
#define FABLE_RETAIL_CTHINGMAP_PEEKCELL_INLINE
#define FABLE_RETAIL_CTCMAPWHO
#define FABLE_RETAIL_CTCMAPWHO_SEARCHID
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_STATTYPEFLAGS
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCPHYSICSBASE_CALCULATERADIUS
#define FABLE_RETAIL_CTCPHYSICSBASE_IMPULSE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_NORMALISE_SQUARE_HELPER
#define FABLE_RETAIL_C3DVECTOR_SQUARED2D_ONE_EXPRESSION_PASS23
#define FABLE_RETAIL_C2DBOXI_RADIUS_INITIALIZERS
#define FABLE_RETAIL_C2DBOXI
#define FABLE_RETAIL_C2DBOXI_COPY
#define FABLE_RETAIL_C2DBOXI_GETWIDTH
#define FABLE_RETAIL_C2DBOXI_GETHEIGHT
#define FABLE_RETAIL_CTHINGFILTER_OVERLAPPINGCIRCLE_BODY
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
#include "engine/CWorldMap.h"
#include "engine/CThingFilter_OverlappingCircle.h"
#include "engine/CFilter_Bind2.h"
template void CThingSearchTools::GetAllThingsInRoughArea<CThing,std::list<CThing*>,CFilter_Bind2<CThingFilter_OverlappingCircle,CWorldMap::CThingFilterBase> >(const C2DBoxI&,const CFilter_Bind2<CThingFilter_OverlappingCircle,CWorldMap::CThingFilterBase>&,std::list<CThing*>&)const;

// ---- doubles ----
unsigned long CThingSearchTools::NoLookups = 4, CThingSearchTools::NoThingsScanned = 7, CThingSearchTools::SearchID = 0;
namespace NThingMap { long MaxLayers = 2; }
namespace NThingMap { extern const CLayerInfo LayerInfo[3] = { { 0, 0 }, { 0, 1 }, { 0, 0 } }; }   // Size, ScaleShift
static unsigned char g_mapA[0x40], g_mapB[0x40], g_mapC[0x40];
static int g_bad = 0, g_mapCalls = 0, g_clipCalls = 0, g_posCalls = 0, g_baseCalls = 0;
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
C3DVector CTCPhysicsBase::GetMapwhoPosition() const
{
    ++g_posCalls;
    const float* f = (const float*)((const char*)this + 0x40);   // the double's own position words
    C3DVector pos; pos.X = f[0]; pos.Y = f[1]; pos.Z = f[2];
    return pos;
}
static const void* g_rejected = 0;
class CTestWorldFilter : public CWorldMap::CThingFilterBase {
public:
    virtual bool operator()(const CThing& thing) const { ++g_baseCalls; return (const void*)&thing != g_rejected; }
};

// ---- harness ----
static int g_fail = 0;
static void check(int ok, const char* what) { if (!ok) { printf("FAIL %s\n", what); g_fail = 1; } }
struct SNode { void* mapwho; SNode* next; void* previous; void* cell; };       // NThingMap::CMapwhoNode
enum { NTHINGS = 17 };
static unsigned char things[NTHINGS][0x100], mapwhos[NTHINGS][0x60], physics[NTHINGS][0x50];
static void SetThing(int i, float x, float y, float z, float r)
{
    *(void**)(things[i] + 0x60) = physics[i];               // CThing::PhysicsTC
    float* p = (float*)(physics[i] + 0x40); p[0] = x; p[1] = y; p[2] = z;
    *(float*)(physics[i] + 0x34) = r;                       // CTCPhysicsBase::Radius
    *(void**)(mapwhos[i] + 0x04) = things[i];               // CTCBase::ParentThing
}
int main()
{
    static long levelInfo[3][5];                            // CMapwhoLevelInfo: Size, Extents.X, Extents.Y, ScaleShift, NoThings
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
    for (int i = 0; i < NTHINGS; ++i) SetThing(i, 0.5f, -0.5f, 0.0f, 1.0f);   // inside the unit circle by default
    SetThing(1, 1.0f, 1.5f, 0.0f, 1.0f);    // layer 0 cell (1,0): found (d2 3.25 < 4); listed again in cell (3,2)
    *(void**)(things[2] + 0x60) = 0;        // layer 0 cell (1,0): no physics component: out
    SetThing(3, 3.0f, 4.0f, 0.0f, 4.0f);    // layer 0 cell (1,0): distance 5 == radius 4 + 1: out (strict <)
    SetThing(4, -1.9f, 0.0f, 0.0f, 1.0f);   // layer 0 cell (2,1): found (d2 3.61 < 4)
    SetThing(15, 0.0f, 0.0f, 500.0f, 0.5f); // layer 0 cell (2,1): far in Z only: found (2D test)
    SetThing(16, 0.0f, 0.0f, 0.0f, 1.0f);   // layer 0 cell (2,1): in the circle, rejected by the world-map filter
    SetThing(10, 0.0f, 2.1f, 0.0f, 1.0f);   // layer 0 cell (2,0): (stamped) would be out anyway
    SetThing(12, 3.0f, 0.0f, 0.0f, 1.0f);   // layer 0 cell (2,0): (stamp greater) d2 9 > 4
    SetThing(11, 0.0f, -1.99f, 0.0f, 1.0f); // layer 0 cell (2,0): found, just inside
    g_rejected = things[16];
    *(unsigned long*)(mapwhos[10] + 0x4c) = 1; *(unsigned long*)(mapwhos[11] + 0x4c) = 0; *(unsigned long*)(mapwhos[12] + 0x4c) = 5;
    for (int i = 0; i < 15; ++i) nodes[i].mapwho = mapwhos[i];
    nodes[13].mapwho = mapwhos[1];
    nodes[18].mapwho = mapwhos[15]; nodes[19].mapwho = mapwhos[16];
    cells0[0 * 4 + 0] = &nodes[0];
    cells0[0 * 4 + 1] = &nodes[1]; nodes[1].next = &nodes[2]; nodes[2].next = &nodes[3];
    cells0[0 * 4 + 2] = &nodes[10]; nodes[10].next = &nodes[11]; nodes[11].next = &nodes[12];
    cells0[1 * 4 + 2] = &nodes[4]; nodes[4].next = &nodes[18]; nodes[18].next = &nodes[19];
    cells0[2 * 4 + 3] = &nodes[5]; nodes[5].next = &nodes[13];
    cells0[1 * 4 + 0] = &nodes[6];
    cells0[3 * 4 + 1] = &nodes[7];
    cells1[1 * 2 + 1] = &nodes[8];
    cells2[0 * 4 + 1] = &nodes[9];
    nodes[16].mapwho = mapwhos[13]; nodes[17].mapwho = mapwhos[14];
    cellsC1[0 * 3 + 1] = &nodes[16]; cellsC1[1 * 3 + 2] = &nodes[17];
    *(void**)(toolsImage + 0x0c) = scratch; *(void**)(toolsImage + 0x10) = scratch; *(void**)(toolsImage + 0x14) = scratch + 4;
    const CThingSearchTools& tools = *(const CThingSearchTools*)toolsImage;
    std::list<CThing*> out;
    C3DVector centre; centre.X = 0.0f; centre.Y = 0.0f; centre.Z = 0.0f;
    CThingFilter_OverlappingCircle circle(centre, 1.0f);
    CTestWorldFilter world_filter;
    CFilter_Bind2<CThingFilter_OverlappingCircle,CWorldMap::CThingFilterBase> filter(circle, world_filter);
    C2DBoxI area; area.TLX = 0; area.TLY = 0; area.BRX = 3; area.BRY = 3;
    check(sizeof(std::vector<CThing*>) == 12 && sizeof(CThingSearchTools) == 0x18, "layouts");
    tools.GetAllThingsInRoughArea<CThing,std::list<CThing*>,CFilter_Bind2<CThingFilter_OverlappingCircle,CWorldMap::CThingFilterBase> >(area, filter, out);
    static const int expect[8] = { 1, 11, 4, 15, 5, 8, 13, 14 };
    int n = 0, order_ok = 1;
    for (std::list<CThing*>::iterator it = out.begin(); it != out.end(); ++it, ++n)
        if (n >= 8 || (const void*)*it != (const void*)things[expect[n]]) order_ok = 0;
    check(!g_bad && g_mapCalls == 1 && g_clipCalls == 3, "map list fetched once, each map clipped once");
    check(n == 8 && order_ok, "found things, in layer / row / cell / node order, each once");
    check(g_posCalls == 10, "position asked once per unstamped thing with physics");
    check(g_baseCalls == 9, "world-map filter asked only for things inside the circle");
    check(*(void**)(toolsImage + 0x10) == scratch, "scratch map list cleared");
    check(CThingSearchTools::NoLookups == 5 && CThingSearchTools::SearchID == 1 && CThingSearchTools::NoThingsScanned == 0, "search statics");
    static const int stamped[] = { 1, 2, 3, 4, 5, 8, 11, 13, 14, 15, 16 };
    for (int i = 0; i < 11; ++i) check(*(unsigned long*)(mapwhos[stamped[i]] + 0x4c) == 1, "scanned components stamped");
    static const int unvisited[] = { 0, 6, 7, 9 };
    for (int i = 0; i < 4; ++i) check(*(unsigned long*)(mapwhos[unvisited[i]] + 0x4c) == 0, "cells outside the area / layers not visited");
    check(*(unsigned long*)(mapwhos[12] + 0x4c) == 5, "a later stamp is left alone");
    if (!g_fail) printf("0063fd40_TEST PASS\n");
    return g_fail;
}