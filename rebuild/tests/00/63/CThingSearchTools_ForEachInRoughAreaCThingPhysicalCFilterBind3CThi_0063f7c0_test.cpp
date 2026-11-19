// Behaviour test for 0x0063F7C0 CThingSearchTools::ForEachInRoughArea<CThingPhysical, CFilter_Bind3<CThingFilter_IsNot,
// CThingFilter_HasNavigationInformation, CThingFilter_OverlappingBoundingBox>, CThingFindNavigationLayerAt>: the row's defines,
// includes, filter / finder classes and explicit instantiation verbatim over the shared headers, run on zeroed object images (the
// map / layer / cell / node harness of hand4c's 0x0066A1E0 test). Things and physics components carry fake vtables (thiscall slots:
// thing +0xDC CanBlockNavigatingCreature; physics +0xA8 HasNavigationLayers, +0x174 GetPhysicsMeshInfo). Per thing: CThingPhysical type
// mask (3), each thing once per search (stamps), not the ignored thing (IsNot double), can block navigating creatures, a physics mesh
// info and the filter point inside the mesh bounding box (GetPhysicsMeshObject / IsPointContainedInBoundingBox doubles), then the finder:
// with navigation layers it asks GetNavigationLevelAt and keeps the largest level above the running one. Runs: (1) the passing set in
// cell order, the maximum level; (2) all levels <= 0: the layer stays 0, still true; (3) no point inside: no finder call, false.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
// ---- row body ----
// CThingSearchTools::ForEachInRoughArea<CThingPhysical, CFilter_Bind3<CThingFilter_IsNot, CThingFilter_HasNavigationInformation, CThingFilter_OverlappingBoundingBox>, CThingFindNavigationLayerAt>(const C2DBoxI& area, const filter&, const finder&) const -- runs the navigation-layer finder over every physical thing in the mapwho cells under the area that is not the ignored thing, has navigation information and whose physics mesh's bounding box contains the filter's point (each thing once per search); true when any thing was visited
// Grade: RELOCATION_MATCH 785v785 (hand4d 2026-11-19, first grade of the port): the old FOREACHINROUGHAREA_BODY copy left the
//   post-node-loop reload order (TLX first in retail); the egod body FABLE_RETAIL_CTHINGSEARCHTOOLS_FOREACHINROUGHAREA_EGOD (hand4c,
//   landed with 0x0066A1E0: Ego_d locals, FableWin order, edge-compare empty test) over ROUGHAREA_EGOD closes it. The filter / finder
//   locals carry Ego_d.pdb names (test_thing; tc_physics; physics, mesh; physics, object, new_layer). True length 785 (ret 0Ch at
//   0x0063FACE; the catalog row says 769).
// Leaf: ??$ForEachInRoughArea@VCThingPhysical@@V?$CFilter_Bind3@VCThingFilter_IsNot@@VCThingFilter_HasNavigationInformation@@VCThingFilter_OverlappingBoundingBox@@@@VCThingFindNavigationLayerAt@@@CThingSearchTools@@QBE_NABVC2DBoxI@@ABV?$CFilter_Bind3@VCThingFilter_IsNot@@VCThingFilter_HasNavigationInformation@@VCThingFilter_OverlappingBoundingBox@@@@ABVCThingFindNavigationLayerAt@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0063F7C0, 785 bytes, thiscall `ret 0Ch`. Ego_r.pdb / FableWin.pdb spell it as the Leaf above
//   (const; the finder by const reference). Its one caller is the landed CWorldMap::GetNavigationLayerAt 0x0063E590 (the Bind3
//   branch, `call 0x0063F7C0` at 0x0063E6CC); the Bind2 twin is 0x0063F4B0.
// Callees: CThingSearchTools::GetAllThingMapsInArea 0x0049C6F0, ClipAndConvertWorldMapAreaToLocalArea 0x0049C700,
//   CThingFilter_IsNot::operator() 0x006A76A0 (out of line), CTCPhysicsBase::HasNavigationLayers (slot 42, +0xA8) /
//   GetPhysicsMeshInfo (slot 93, +0x174), CThingPhysical::CanBlockNavigatingCreature (slot 55, +0xDC),
//   CTCPhysicsBase::GetPhysicsMeshObject 0x006B0850 (by value), CPhysicsMeshObject::IsPointContainedInBoundingBox 0x00A12890 /
//   GetNavigationLevelAt 0x00A11D10 / ~CPhysicsMeshObject 0x00A127D0, memmove 0x00BFEAE6 (the AreaMaps clear).
// Layout: shared headers (CThingSearchTools AreaMaps +0x0C; NThingMap::CThingMap; CTCMapwho SearchID +0x4C; CThing PhysicsTC +0x60,
//   ThingTypeFlags +0x6C; CFilter_Bind3 const references F1/F2/F3; CThingFilter_IsNot). The two filters and the finder have no shared
//   header: Ego_r.pdb classes CThingFilter_HasNavigationInformation (empty), CThingFilter_OverlappingBoundingBox {C3DVector Pos;
//   float Radius} and CThingFindNavigationLayerAt {C3DVector Pos; long Layer} (struct_layouts_egor.tsv), operator() QBE in Ego_r
//   (??RCThingFilter_HasNavigationInformation@@QBE_NABVCThingPhysical@@@Z, ??RCThingFindNavigationLayerAt@@QBE_NABVCThing@@@Z: the
//   const finder keeps its running Layer in a mutable member), all inline here.
// Shape: the shared egod body FABLE_RETAIL_CTHINGSEARCHTOOLS_FOREACHINROUGHAREA_EGOD (+ FOREACHINROUGHAREA, _CONSTPROC, ROUGHAREA_EGOD),
//   instantiated explicitly. The three functor classes have no shared header (declared here as the landed caller 0x0063E590 does).
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
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
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_ORIGINAL_VTABLE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CPHYSICSMESHOBJECT_NAVIGATION
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
#include "engine/CPhysicsMeshObject.h"
#include "engine/CThingPhysical.h"
class CTCMapwho;
#include "engine/CTCMapwho.h"
#include "engine/NThingMap_CThingMap.h"
#include "engine/CThingSearchTools.h"
#include "engine/C2DBoxI.h"
#include "engine/CThingFilter_IsNot.h"
#include "engine/CFilter_Bind3.h"

class CThingFilter_HasNavigationInformation {
public:
    bool operator()(const CThingPhysical& test_thing) const
    {
        const CTCPhysicsBase* tc_physics = test_thing.GetPhysicsTC();
        if (tc_physics && tc_physics->HasNavigationLayers())
            return test_thing.CanBlockNavigatingCreature();
        return test_thing.CanBlockNavigatingCreature();
    }
};

class CThingFilter_OverlappingBoundingBox {
public:
    CThingFilter_OverlappingBoundingBox(const C3DVector& pos, float radius) : Pos(pos), Radius(radius) {}
    bool operator()(const CThingPhysical& test_thing) const
    {
        const CTCPhysicsBase* physics = test_thing.GetPhysicsTC();
        if (physics->GetPhysicsMeshInfo()) {
            CPhysicsMeshObject mesh = physics->GetPhysicsMeshObject();
            return mesh.IsPointContainedInBoundingBox(Pos);
        }
        return false;
    }
private:
    C3DVector Pos;                                           // +0x00
    float     Radius;                                        // +0x0c
};

class CThingFindNavigationLayerAt {
public:
    CThingFindNavigationLayerAt(const C3DVector& pos) : Pos(pos), Layer(0) {}
    bool operator()(const CThing& test_thing) const
    {
        const CTCPhysicsBase* physics = test_thing.GetPhysicsTC();
        if (physics && physics->HasNavigationLayers()) {
            CPhysicsMeshObject object = physics->GetPhysicsMeshObject();
            long new_layer = object.GetNavigationLevelAt(Pos);
            if (new_layer > Layer)
                Layer = new_layer;
        }
        return true;
    }
    float GetLayer() const { return (float)Layer; }
private:
    C3DVector    Pos;                                        // +0x00
    mutable long Layer;                                      // +0x0c
};

template bool CThingSearchTools::ForEachInRoughArea<CThingPhysical,
    CFilter_Bind3<CThingFilter_IsNot, CThingFilter_HasNavigationInformation, CThingFilter_OverlappingBoundingBox>, CThingFindNavigationLayerAt>(
    const C2DBoxI& area, const CFilter_Bind3<CThingFilter_IsNot, CThingFilter_HasNavigationInformation, CThingFilter_OverlappingBoundingBox>& filter,
    const CThingFindNavigationLayerAt& proc) const;

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
enum { NTHINGS = 19 };
static unsigned char things[NTHINGS][0x100], mapwhos[NTHINGS][0x60], physics[NTHINGS][0x80];
static int g_canBlock[NTHINGS], g_hasNav[NTHINGS], g_meshInfo[NTHINGS], g_inside[NTHINGS];
static long g_level[NTHINGS];
static int g_levelCalls = 0, g_levelLog[32], g_insideCalls = 0, g_isNotCalls = 0;
static const C3DVector* g_point = 0;
static int ThingIndex(const void* p) { return (int)(((const unsigned char*)p - (const unsigned char*)things) / 0x100); }
static int PhysIndex(const void* p) { return (int)(((const unsigned char*)p - (const unsigned char*)physics) / 0x80); }
bool CThingFilter_IsNot::operator()(const CThing& thing) const { ++g_isNotCalls; return &thing != &Thing; }
CBaseClass::CBaseClass() {}
CBaseClass::~CBaseClass() {}
CPhysicsMeshObject::~CPhysicsMeshObject() {}
CPhysicsMeshObject CTCPhysicsBase::GetPhysicsMeshObject() const
{
    CPhysicsMeshObject object;
    int i = PhysIndex(this);
    memcpy((char*)&object + 4, &i, 4);                      // the Mesh pointer bytes carry the physics index
    return object;
}
static int MeshIndex(const CPhysicsMeshObject* o) { int i; memcpy(&i, (const char*)o + 4, 4); return i; }
bool CPhysicsMeshObject::IsPointContainedInBoundingBox(const C3DVector& pos) const
{
    ++g_insideCalls;
    if (&pos == g_point || pos.X != 1.5f || pos.Y != 2.5f || pos.Z != 3.5f) g_bad = 1;   // the filter's own copy of the point
    return g_inside[MeshIndex(this)] != 0;
}
long CPhysicsMeshObject::GetNavigationLevelAt(const C3DVector& pos) const
{
    int i = MeshIndex(this);
    if (pos.X != 1.5f || pos.Y != 2.5f || pos.Z != 3.5f) g_bad = 1;
    if (g_levelCalls < 32) g_levelLog[g_levelCalls] = i;
    ++g_levelCalls;
    return g_level[i];
}
static bool __fastcall CanBlock(void* self, void*) { return g_canBlock[ThingIndex(self)] != 0; }
static bool __fastcall HasNav(void* self, void*) { return g_hasNav[PhysIndex(self)] != 0; }
static const void* __fastcall MeshInfo(void* self, void*) { return g_meshInfo[PhysIndex(self)] ? (const void*)things : (const void*)0; }
static void* g_thingVt[64];
static void* g_physVt[128];

// ---- harness ----
static int g_fail = 0;
static void check(int ok, const char* what) { if (!ok) { printf("FAIL %s\n", what); g_fail = 1; } }
struct SNode { void* mapwho; SNode* next; void* previous; void* cell; };       // NThingMap::CMapwhoNode
static void SetThing(int i)
{
    *(void**)things[i] = g_thingVt;
    *(unsigned long*)(things[i] + 0x6c) = 3;                // CThing::ThingTypeFlags (CThingPhysical)
    *(void**)(things[i] + 0x60) = physics[i];               // CThing::PhysicsTC
    *(void**)physics[i] = g_physVt;
    g_canBlock[i] = 1; g_hasNav[i] = 1; g_meshInfo[i] = 1; g_inside[i] = 1; g_level[i] = 100 + i;
    *(void**)(mapwhos[i] + 0x04) = things[i];               // CTCBase::ParentThing
}
static unsigned char toolsImage[0x18]; static void* scratch[4];
static bool Run(float* layer)
{
    const CThingSearchTools& tools = *(const CThingSearchTools*)toolsImage;
    C3DVector point; point.X = 1.5f; point.Y = 2.5f; point.Z = 3.5f; g_point = &point;
    CThingFilter_IsNot is_not(*(const CThing*)things[2]);
    CThingFilter_HasNavigationInformation has_nav;
    CThingFilter_OverlappingBoundingBox overlapping(point, 0.1f);
    CFilter_Bind3<CThingFilter_IsNot, CThingFilter_HasNavigationInformation, CThingFilter_OverlappingBoundingBox> filter(is_not, has_nav, overlapping);
    CThingFindNavigationLayerAt finder(point);
    C2DBoxI area; area.TLX = 0; area.TLY = 0; area.BRX = 3; area.BRY = 3;
    for (int i = 0; i < NTHINGS; ++i) *(unsigned long*)(mapwhos[i] + 0x4c) = 0;
    *(unsigned long*)(mapwhos[10] + 0x4c) = CThingSearchTools::SearchID + 1; *(unsigned long*)(mapwhos[12] + 0x4c) = CThingSearchTools::SearchID + 5;
    g_levelCalls = 0; g_mapCalls = 0; g_clipCalls = 0; g_insideCalls = 0; g_isNotCalls = 0;
    bool r = tools.ForEachInRoughArea<CThingPhysical, CFilter_Bind3<CThingFilter_IsNot, CThingFilter_HasNavigationInformation, CThingFilter_OverlappingBoundingBox>, CThingFindNavigationLayerAt>(area, filter, finder);
    *layer = finder.GetLayer();
    return r;
}
int main()
{
    static long levelInfo[3][5];
    static SNode* cells0[16]; static SNode* cells1[4]; static SNode* cells2[16];
    static void* cellVectors[3][3];
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
    for (int k = 0; k < 64; ++k) g_thingVt[k] = 0;
    for (int k2 = 0; k2 < 128; ++k2) g_physVt[k2] = 0;
    g_thingVt[0xdc / 4] = (void*)&CanBlock; g_physVt[0xa8 / 4] = (void*)&HasNav; g_physVt[0x174 / 4] = (void*)&MeshInfo;
    for (int i = 0; i < NTHINGS; ++i) SetThing(i);
    // thing 2 is the ignored thing (cell (1,0)); thing 1 is listed twice (cells (1,0) and (3,2))
    g_canBlock[3] = 0;                      // layer 0 cell (1,0): has navigation layers but cannot block navigating creatures
    g_level[11] = 500; g_hasNav[11] = 0;    // layer 0 cell (2,0): passes, but no navigation layers: its level is never asked
    g_canBlock[15] = 0; g_hasNav[15] = 0;   // layer 0 cell (2,1): no navigation layers and cannot block navigating creatures
    g_meshInfo[16] = 0;                     // layer 0 cell (2,1): no physics mesh info
    *(unsigned long*)(things[17] + 0x6c) = 2;   // layer 0 cell (2,1): not a CThingPhysical (type bit 0 missing)
    *(unsigned long*)(things[18] + 0x6c) = 7;   // layer 0 cell (3,2): a superset of the mask: passes
    g_level[5] = 150;                       // the largest level among the asked things (others 100 + index <= 118)
    *(void**)(toolsImage + 0x0c) = scratch; *(void**)(toolsImage + 0x10) = scratch; *(void**)(toolsImage + 0x14) = scratch + 4;
    check(sizeof(CThingFindNavigationLayerAt) == 16 && sizeof(CThingFilter_OverlappingBoundingBox) == 16 && sizeof(CThingSearchTools) == 0x18, "layouts");
    float layer = -1.0f;
    bool r1 = Run(&layer);
    static const int expect[7] = { 1, 4, 5, 18, 8, 13, 14 };
    int ok = g_levelCalls == 7;
    for (int n = 0; ok && n < 7; ++n) if (g_levelLog[n] != expect[n]) ok = 0;
    check(!g_bad && g_mapCalls == 1 && g_clipCalls == 3, "map list fetched once, each map clipped once, point passed through");
    check(ok, "levels asked for exactly the passing things with navigation layers, in layer / row / cell / node order, each once");
    check(r1 && layer == 150.0f, "true, the largest level kept");
    check(g_insideCalls == 8 && g_isNotCalls == 12, "bounding box asked only for things past IsNot / HasNavigationInformation / mesh info");
    check(*(void**)(toolsImage + 0x10) == scratch, "scratch map list cleared");
    check(CThingSearchTools::NoLookups == 5 && CThingSearchTools::SearchID == 1 && CThingSearchTools::NoThingsScanned == 0, "search statics");
    check(*(unsigned long*)(mapwhos[12] + 0x4c) == 5 && *(unsigned long*)(mapwhos[10] + 0x4c) == 1, "later stamps left alone");
    for (int i = 0; i < NTHINGS; ++i) g_level[i] = -3 + (i % 3);
    bool r2 = Run(&layer);
    check(r2 && layer == 0.0f && g_levelCalls == 7, "levels <= 0 leave the layer at 0, still true");
    g_level[18] = 1; g_level[13] = 0;
    bool r2b = Run(&layer);
    check(r2b && layer == 1.0f, "a level of 1 raises the layer");
    for (int i2 = 0; i2 < NTHINGS; ++i2) g_inside[i2] = 0;
    bool r3 = Run(&layer);
    check(!r3 && g_levelCalls == 0 && layer == 0.0f && g_insideCalls > 0, "no point inside: no finder call, false");
    if (!g_fail) printf("0063f7c0_TEST PASS\n");
    return g_fail;
}