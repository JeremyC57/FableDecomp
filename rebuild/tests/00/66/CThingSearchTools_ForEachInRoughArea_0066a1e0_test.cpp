// Behaviour test for 0x0066A1E0 CThingSearchTools::ForEachInRoughArea<CThingPhysical, CFilter_Bind3<CThingFilter_SolidToThing,
// CThingFilter_PhysicsFlagsSolidToThing, CThingFilter_OverlappingCircle>, `anonymous namespace'::CThingFindHeightZBelowAt>: the row's
// defines, includes, functor declaration and explicit instantiation verbatim over the shared headers (+ the hand4c overlay opt-ins), run
// on zeroed object images (harness of the landed 0x00878430 test: maps B / A / C, layers, stamps). Physics components are images with a
// fake vtable (CanCollideWithThings at +0xE4, CanCollideWithThingsOneWay at +0xE8, called thiscall). Per thing: the CThingPhysical type
// mask (3), the source thing excluded, both physics components present, the source collides with things or one way, the tested one
// collides, its circle overlaps (strict, 2D), then the functor (a double recording its calls) -- its true result sets the return value.
// Three runs: (1) source collides one way only: the functor sees exactly the passing things in order, one returns true -> true;
// (2) the same with every functor answer false -> false; (3) a source that collides neither way -> no functor call, false; (4) a source
// that collides with things -> the same set; (5) a source with no physics component -> no functor call, false.
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
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_FOREACHINROUGHAREA_EGOD
#define FABLE_RETAIL_CTHINGMAP_PEEKCELL_INLINE
#define FABLE_RETAIL_CTCMAPWHO
#define FABLE_RETAIL_CTCMAPWHO_SEARCHID
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGPHYSICAL_STATTYPEFLAGS
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHING
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
#define FABLE_RETAIL_CTHINGFILTER_SOLIDTOTHING_BODY
#define FABLE_RETAIL_CTHINGFILTER_PHYSICSFLAGSSOLIDTOTHING_BODY
#include "engine/C2DExtentsI.h"
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CWorld.h"
class CTCMapwho;
#include "engine/CTCMapwho.h"
#include "engine/NThingMap_CThingMap.h"
#include "engine/CThingPhysical.h"
#include "engine/CThingSearchTools.h"
#include "engine/C2DBoxI.h"
#include "engine/CThingFilter_SolidToThing.h"
#include "engine/CThingFilter_PhysicsFlagsSolidToThing.h"
#include "engine/CThingFilter_OverlappingCircle.h"
#include "engine/CFilter_Bind3.h"
namespace {
class CThingFindHeightZBelowAt {
public:
    CThingFindHeightZBelowAt(const C3DVector& from, const C3DVector& to, float height);
    bool operator()(const CThing& thing) const;                       // 0x00668AB0
    float GetHeight() const;
private:
    const C3DVector& From;                                             // +0x00
    const C3DVector& To;                                               // +0x04
    mutable float Height;                                              // +0x08
};
}
template bool CThingSearchTools::ForEachInRoughArea<CThingPhysical,CFilter_Bind3<CThingFilter_SolidToThing,CThingFilter_PhysicsFlagsSolidToThing,CThingFilter_OverlappingCircle>,CThingFindHeightZBelowAt>(const C2DBoxI&,const CFilter_Bind3<CThingFilter_SolidToThing,CThingFilter_PhysicsFlagsSolidToThing,CThingFilter_OverlappingCircle>&,const CThingFindHeightZBelowAt&)const;

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
C3DVector CTCPhysicsBase::GetMapwhoPosition() const
{
    const float* f = (const float*)((const char*)this + 0x40);
    C3DVector pos; pos.X = f[0]; pos.Y = f[1]; pos.Z = f[2];
    return pos;
}
static int g_opCalls = 0, g_opLog[32], g_answerAll = 1;
static const void* g_things0 = 0;
namespace {
CThingFindHeightZBelowAt::CThingFindHeightZBelowAt(const C3DVector& from, const C3DVector& to, float height) : From(from), To(to), Height(height) {}
bool CThingFindHeightZBelowAt::operator()(const CThing& thing) const
{
    int i = (int)(((const unsigned char*)&thing - (const unsigned char*)g_things0) / 0x100);
    if (g_opCalls < 32) g_opLog[g_opCalls] = i;
    ++g_opCalls;
    Height += 1.0f;
    return g_answerAll && *(const long*)((const char*)&thing + 0x90) != 0;
}
}

// ---- harness ----
static int g_fail = 0;
static void check(int ok, const char* what) { if (!ok) { printf("FAIL %s\n", what); g_fail = 1; } }
struct SNode { void* mapwho; SNode* next; void* previous; void* cell; };       // NThingMap::CMapwhoNode
enum { NTHINGS = 19 };
static unsigned char things[NTHINGS][0x100], mapwhos[NTHINGS][0x60], physics[NTHINGS + 1][0x80];
static int g_collides[NTHINGS + 1], g_oneway[NTHINGS + 1];
static int PhysIndex(const void* p) { return (int)(((const unsigned char*)p - (const unsigned char*)physics) / 0x80); }
static bool __fastcall CanCollide(void* self, void*) { return g_collides[PhysIndex(self)] != 0; }
static bool __fastcall CanCollideOneWay(void* self, void*) { return g_oneway[PhysIndex(self)] != 0; }
static void* g_vtable[64];
static void SetThing(int i, float x, float y, float r, long answer)
{
    *(unsigned long*)(things[i] + 0x6c) = 3;                // CThing::ThingTypeFlags (CThingPhysical)
    *(void**)(things[i] + 0x60) = physics[i];               // CThing::PhysicsTC
    *(long*)(things[i] + 0x90) = answer;                    // the functor double's answer
    *(void**)physics[i] = g_vtable;
    float* p = (float*)(physics[i] + 0x40); p[0] = x; p[1] = y; p[2] = 0.0f;
    *(float*)(physics[i] + 0x34) = r;                       // CTCPhysicsBase::Radius
    g_collides[i] = 1;
    *(void**)(mapwhos[i] + 0x04) = things[i];               // CTCBase::ParentThing
}
static unsigned char toolsImage[0x18]; static void* scratch[4];
static bool Run(const C3DVector& centre, const unsigned char* source_filter_images)
{
    const CThingSearchTools& tools = *(const CThingSearchTools*)toolsImage;
    const CThingFilter_SolidToThing& solid = *(const CThingFilter_SolidToThing*)source_filter_images;
    const CThingFilter_PhysicsFlagsSolidToThing& flags = *(const CThingFilter_PhysicsFlagsSolidToThing*)(source_filter_images + 8);
    CThingFilter_OverlappingCircle circle(centre, 1.0f);
    CFilter_Bind3<CThingFilter_SolidToThing,CThingFilter_PhysicsFlagsSolidToThing,CThingFilter_OverlappingCircle> filter(solid, flags, circle);
    C3DVector from = centre, to = centre;
    CThingFindHeightZBelowAt op(from, to, 0.0f);
    C2DBoxI area; area.TLX = 0; area.TLY = 0; area.BRX = 3; area.BRY = 3;
    for (int i = 0; i < NTHINGS; ++i) *(unsigned long*)(mapwhos[i] + 0x4c) = 0;
    *(unsigned long*)(mapwhos[10] + 0x4c) = CThingSearchTools::SearchID + 1; *(unsigned long*)(mapwhos[12] + 0x4c) = CThingSearchTools::SearchID + 5;
    g_opCalls = 0; g_mapCalls = 0; g_clipCalls = 0;
    return tools.ForEachInRoughArea<CThingPhysical,CFilter_Bind3<CThingFilter_SolidToThing,CThingFilter_PhysicsFlagsSolidToThing,CThingFilter_OverlappingCircle>,CThingFindHeightZBelowAt>(area, filter, op);
}
int main()
{
    static long levelInfo[3][5];
    static SNode* cells0[16]; static SNode* cells1[4]; static SNode* cells2[16];
    static void* cellVectors[3][3];
    static SNode nodes[22];
    static long levelInfoC[3][5]; static SNode* cellsC0[24]; static SNode* cellsC1[6]; static void* cellVectorsC[3][3];
    g_things0 = things;
    for (int k = 0; k < 64; ++k) g_vtable[k] = 0;
    g_vtable[0xe4 / 4] = (void*)&CanCollide; g_vtable[0xe8 / 4] = (void*)&CanCollideOneWay;
    levelInfo[0][1] = 4; levelInfo[1][1] = 2; levelInfo[2][1] = 4;
    cellVectors[0][0] = cells0; cellVectors[1][0] = cells1; cellVectors[2][0] = cells2;
    *(void**)(g_mapA + 0x04) = levelInfo; *(void**)(g_mapA + 0x08) = levelInfo + 3; *(void**)(g_mapA + 0x0c) = levelInfo + 3;
    *(void**)(g_mapA + 0x14) = cellVectors; *(void**)(g_mapA + 0x18) = cellVectors + 3; *(void**)(g_mapA + 0x1c) = cellVectors + 3;
    levelInfoC[0][1] = 6; levelInfoC[1][1] = 3; levelInfoC[2][1] = 3;
    cellVectorsC[0][0] = cellsC0; cellVectorsC[1][0] = cellsC1; cellVectorsC[2][0] = cellsC1;
    *(void**)(g_mapC + 0x04) = levelInfoC; *(void**)(g_mapC + 0x08) = levelInfoC + 3; *(void**)(g_mapC + 0x0c) = levelInfoC + 3;
    *(void**)(g_mapC + 0x14) = cellVectorsC; *(void**)(g_mapC + 0x18) = cellVectorsC + 3; *(void**)(g_mapC + 0x1c) = cellVectorsC + 3;
    for (int i = 0; i < NTHINGS; ++i) SetThing(i, 0.5f, -0.5f, 1.0f, 0);
    SetThing(1, 1.0f, 1.5f, 1.0f, 0);       // layer 0 cell (1,0): passes (d2 3.25 < 4); listed again in cell (3,2)
    SetThing(3, 3.0f, 4.0f, 4.0f, 0);       // layer 0 cell (1,0): distance 5 == radius 4 + 1: out (strict)
    SetThing(11, 0.0f, -1.99f, 1.0f, 1);    // layer 0 cell (2,0): passes, the functor answers true
    *(void**)(things[15] + 0x60) = 0;       // layer 0 cell (2,1): no physics component
    g_collides[16] = 0;                     // layer 0 cell (2,1): its component does not collide with things
    *(unsigned long*)(things[17] + 0x6c) = 2;   // layer 0 cell (2,1): not a CThingPhysical (type bit 0 missing)
    *(unsigned long*)(things[18] + 0x6c) = 7;   // layer 0 cell (3,2): a superset of the mask: passes
    for (int i = 0; i < 15; ++i) nodes[i].mapwho = mapwhos[i];
    nodes[13].mapwho = mapwhos[1];
    nodes[18].mapwho = mapwhos[15]; nodes[19].mapwho = mapwhos[16]; nodes[20].mapwho = mapwhos[17]; nodes[21].mapwho = mapwhos[18];
    cells0[0 * 4 + 0] = &nodes[0];
    cells0[0 * 4 + 1] = &nodes[1]; nodes[1].next = &nodes[2]; nodes[2].next = &nodes[3];
    cells0[0 * 4 + 2] = &nodes[10]; nodes[10].next = &nodes[11]; nodes[11].next = &nodes[12];
    cells0[1 * 4 + 2] = &nodes[4]; nodes[4].next = &nodes[18]; nodes[18].next = &nodes[19]; nodes[19].next = &nodes[20];
    cells0[2 * 4 + 3] = &nodes[5]; nodes[5].next = &nodes[13]; nodes[13].next = &nodes[21];
    cells0[1 * 4 + 0] = &nodes[6];
    cells0[3 * 4 + 1] = &nodes[7];
    cells1[1 * 2 + 1] = &nodes[8];
    cells2[0 * 4 + 1] = &nodes[9];
    nodes[16].mapwho = mapwhos[13]; nodes[17].mapwho = mapwhos[14];
    cellsC1[0 * 3 + 1] = &nodes[16]; cellsC1[1 * 3 + 2] = &nodes[17];
    *(void**)(toolsImage + 0x0c) = scratch; *(void**)(toolsImage + 0x10) = scratch; *(void**)(toolsImage + 0x14) = scratch + 4;
    // source thing = thing 2 (in cell (1,0)): SolidToThing { SourceThing, PTCSolidToThing }, PhysicsFlags { SourceThing, SourceThingPhysicsTC }
    static unsigned char filters[16];
    *(void**)(filters + 0) = things[2]; *(void**)(filters + 8) = things[2]; *(void**)(filters + 12) = physics[NTHINGS];
    *(void**)physics[NTHINGS] = g_vtable; g_collides[NTHINGS] = 0; g_oneway[NTHINGS] = 1;
    C3DVector centre; centre.X = 0.0f; centre.Y = 0.0f; centre.Z = 0.0f;
    check(sizeof(std::vector<CThing*>) == 12 && sizeof(CThingSearchTools) == 0x18 && sizeof(CThingFindHeightZBelowAt) == 12, "layouts");
    bool r1 = Run(centre, filters);
    static const int expect[10] = { 1, 11, 4, 5, 18, 8, 13, 14 };
    int ok = g_opCalls == 8;
    for (int n = 0; ok && n < 8; ++n) if (g_opLog[n] != expect[n]) ok = 0;
    check(!g_bad && g_mapCalls == 1 && g_clipCalls == 3, "map list fetched once, each map clipped once");
    check(ok, "functor called for exactly the passing things, in layer / row / cell / node order, each once");
    check(r1, "true when a call returned true");
    check(*(void**)(toolsImage + 0x10) == scratch, "scratch map list cleared");
    check(CThingSearchTools::NoLookups == 5 && CThingSearchTools::SearchID == 1 && CThingSearchTools::NoThingsScanned == 0, "search statics");
    check(*(unsigned long*)(mapwhos[0] + 0x4c) == 0 && *(unsigned long*)(mapwhos[9] + 0x4c) == 0 && *(unsigned long*)(mapwhos[12] + 0x4c) == 5 && *(unsigned long*)(mapwhos[10] + 0x4c) == 1, "unvisited cells / later stamps left alone");
    g_answerAll = 0;
    bool r2 = Run(centre, filters);
    check(!r2 && g_opCalls == 8, "false when every call returned false");
    g_answerAll = 1; g_oneway[NTHINGS] = 0;
    bool r3 = Run(centre, filters);
    check(!r3 && g_opCalls == 0, "a source that collides neither way passes nothing");
    g_collides[NTHINGS] = 1;
    bool r4 = Run(centre, filters);
    check(r4 && g_opCalls == 8, "a source that collides with things passes the same set");
    *(void**)(filters + 12) = 0;
    bool r5 = Run(centre, filters);
    check(!r5 && g_opCalls == 0, "a source with no physics component passes nothing");
    if (!g_fail) printf("0066a1e0_TEST PASS\n");
    return g_fail;
}
