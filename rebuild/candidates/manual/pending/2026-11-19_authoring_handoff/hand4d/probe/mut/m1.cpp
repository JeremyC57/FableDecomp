// Behaviour test for 0x0085B2A0 CThingSearchTools::GetAllThingsInRoughArea<CThingGameObject, std::list<CThingGameObject*>,
// CFilter_Bind4<CThingFilter_IsNot, CThingFilter_CombatIsStrikeToStrike, CThingFilter_CombatOverlappingSphere,
// CThingFilter_CombatCanCollideOrInteractWithThing> >: the row's defines, includes, filter classes and explicit instantiation verbatim over
// the shared headers (+ the hand4d overlay opt-in CFILTER_BIND4_FORCEINLINE), run on zeroed object images. One map, one layer: its local
// area (from the ClipAndConvert double) is the world area moved one cell right, so a thing left of it must not be visited. Things carry
// type flags +0x6C (game object = 7), the TCI 0x7F presence bit (word +0x2C bit 31) and a sorted component map (+0x44 begin / +0x48 end)
// with a lower-keyed entry before the hit-locations one. Covers: the search-id stamp (equal skipped), the game-object type test before any
// filter, the four filters in order with short-circuit (call counts per filter), the inline sphere test (no hit-locations TC; strict <
// on the squared distance against (hit radius + filter radius)^2, X, Y and Z all counted; GetBoundingSphere asked with `true`), and the
// results appended in cell / node order. Doubles: GetAllThingMapsInArea, ClipAndConvertWorldMapAreaToLocalArea, the three out-of-line
// filter operators, CTCHitLocations::GetBoundingSphere, the search / layer statics.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
// ---- row body ----
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_CFILTER_BIND4_FORCEINLINE
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_CLIPAREA
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ORIGINAL_ROUGHAREA
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ROUGHAREA_EGOD
#define FABLE_RETAIL_CTHINGMAP_PEEKCELL_INLINE
#define FABLE_RETAIL_CTCMAPWHO
#define FABLE_RETAIL_CTCMAPWHO_SEARCHID
#define FABLE_RETAIL_CTHINGGAMEOBJECT_STATTYPEFLAGS
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_NORMALISE_SQUARE_HELPER
#define FABLE_RETAIL_HITLOCATIONS_BOUND
#define FABLE_RETAIL_C2DBOXI
#define FABLE_RETAIL_C2DBOXI_COPY
#define FABLE_RETAIL_C2DBOXI_GETWIDTH
#define FABLE_RETAIL_C2DBOXI_GETHEIGHT
#include "engine/C2DExtentsI.h"
#include "engine/C3DVector.h"
#include "engine/CThing.h"
class CTCMapwho;
#include "engine/CTCMapwho.h"
#include "engine/NThingMap_CThingMap.h"
#include "engine/CThingGameObject.h"
#include "engine/CThingSearchTools.h"
#include "engine/C2DBoxI.h"
#include "engine/CTCHitLocations.h"
#include "engine/CThingFilter_IsNot.h"
#include "engine/CFilter_Bind4.h"

namespace {
class CThingFilter_CombatIsStrikeToStrike : public std::unary_function<CThing,bool> {
public:
    CThingFilter_CombatIsStrikeToStrike(const CThing& thing_striking) : ThingStriking(thing_striking) {}
    bool operator()(const CThing& test_thing) const;                    // 0x008565E0
private:
    const CThing& ThingStriking;                                        // +0x00
};
class CThingFilter_CombatCanCollideOrInteractWithThing : public std::unary_function<CThing,bool> {
public:
    CThingFilter_CombatCanCollideOrInteractWithThing(const CTCPhysicsBase& tc_physics, bool exclude_dead_creatures, bool exclude_creatures)
        : TCPhysics(tc_physics), ExcludeCreatures(exclude_creatures), ExcludeDeadCreatures(exclude_dead_creatures) {}
    bool operator()(const CThing& test_thing) const;                    // 0x00857250
private:
    const CTCPhysicsBase& TCPhysics;                                    // +0x00
    bool ExcludeCreatures;                                              // +0x04
    bool ExcludeDeadCreatures;                                          // +0x05
};
}

class CThingFilter_CombatOverlappingSphere : public std::unary_function<CThing,bool> {
public:
    CThingFilter_CombatOverlappingSphere(const C3DVector& pos, float radius) : SpherePos(pos), Radius(radius) {}
    FABLE_FORCEINLINE bool operator()(const CThing& test_thing) const
    {
        const CTCHitLocations* ptc_hit_locations;
        if (test_thing.PeekTC(&ptc_hit_locations)) {
            C3DVector centre_pos;
            float radius;
            ptc_hit_locations->GetBoundingSphere(centre_pos, radius, true);
            if ((centre_pos - SpherePos).GetSquaredMagnitude() <= GFSqr(radius + Radius))
                return true;
        }
        return false;
    }
private:
    C3DVector SpherePos;                                                // +0x00
    float Radius;                                                       // +0x0c
};

template void CThingSearchTools::GetAllThingsInRoughArea<CThingGameObject, std::list<CThingGameObject*>, CFilter_Bind4<CThingFilter_IsNot, CThingFilter_CombatIsStrikeToStrike, CThingFilter_CombatOverlappingSphere, CThingFilter_CombatCanCollideOrInteractWithThing> >(const C2DBoxI&, const CFilter_Bind4<CThingFilter_IsNot, CThingFilter_CombatIsStrikeToStrike, CThingFilter_CombatOverlappingSphere, CThingFilter_CombatCanCollideOrInteractWithThing>&, std::list<CThingGameObject*>&) const;

// ---- doubles ----
unsigned long CThingSearchTools::NoLookups = 4, CThingSearchTools::NoThingsScanned = 7, CThingSearchTools::SearchID = 0;
namespace NThingMap { long MaxLayers = 1; }
namespace NThingMap { extern const CLayerInfo LayerInfo[3] = { { 0, 0 }, { 0, 1 }, { 0, 0 } }; }   // Size, ScaleShift
static unsigned char g_mapA[0x40];
static int g_bad = 0, g_mapCalls = 0, g_clipCalls = 0;
void CThingSearchTools::GetAllThingMapsInArea(const C2DBoxI& area, std::vector<NThingMap::CThingMap*>& maps) const
{
    ++g_mapCalls;
    void*** v = (void***)&maps;                             // STLport vector: start, finish, end of storage
    if (area.TLX != 0 || area.TLY != 0 || area.BRX != 3 || area.BRY != 3 || v[0] != v[1]) g_bad = 1;
    v[0][0] = g_mapA; v[1] = v[0] + 1;
}
C2DBoxI CThingSearchTools::ClipAndConvertWorldMapAreaToLocalArea(const NThingMap::CThingMap& map, const C2DBoxI& area) const
{
    ++g_clipCalls;
    if ((const void*)&map != (const void*)g_mapA) g_bad = 1;
    C2DBoxI local;
    local.TLX = area.TLX + 1; local.TLY = area.TLY; local.BRX = area.BRX + 1; local.BRY = area.BRY;
    return local;
}
enum { NT = 12 };
static unsigned char things[NT][0x100], mapwhos[NT][0x60], hits[NT][0x40];
static int g_strikeOk[NT], g_collideOk[NT];
static float g_sphere[NT][4];
static int g_isNotCalls = 0, g_strikeCalls = 0, g_sphereCalls = 0, g_collideCalls = 0, g_badArg = 0;
static const void* g_striker = 0; static const void* g_tcPhysics = 0;
static int TI(const void* p) { return (int)(((const unsigned char*)p - (const unsigned char*)things) / 0x100); }
bool CThingFilter_IsNot::operator()(const CThing& thing) const { ++g_isNotCalls; return &thing != &Thing; }
namespace {
bool CThingFilter_CombatIsStrikeToStrike::operator()(const CThing& test_thing) const
{
    ++g_strikeCalls;
    if ((const void*)&ThingStriking != g_striker) g_badArg = 1;
    return g_strikeOk[TI(&test_thing)] != 0;
}
bool CThingFilter_CombatCanCollideOrInteractWithThing::operator()(const CThing& test_thing) const
{
    ++g_collideCalls;
    if ((const void*)&TCPhysics != g_tcPhysics || !ExcludeCreatures || ExcludeDeadCreatures) g_badArg = 1;
    return g_collideOk[TI(&test_thing)] != 0;
}
}
void CTCHitLocations::GetBoundingSphere(C3DVector& centre, float& radius, bool flag) const
{
    ++g_sphereCalls;
    int i = (int)(((const unsigned char*)this - (const unsigned char*)hits) / 0x40);
    if (!flag) g_badArg = 1;
    centre.X = g_sphere[i][0]; centre.Y = g_sphere[i][1]; centre.Z = g_sphere[i][2]; radius = g_sphere[i][3];
}

// ---- harness ----
static int g_fail = 0;
static void check(int ok, const char* what) { if (!ok) { printf("FAIL %s\n", what); g_fail = 1; } }
struct SNode { void* mapwho; SNode* next; void* previous; void* cell; };       // NThingMap::CMapwhoNode
struct SPair { long type; void* tc; };
static SPair comps[NT][2];
static void SetThing(int i, unsigned long flags, int strike, int hit, float x, float y, float z, float r, int collide)
{
    *(unsigned long*)(things[i] + 0x6c) = flags;
    g_strikeOk[i] = strike; g_collideOk[i] = collide;
    comps[i][0].type = 0x10; comps[i][0].tc = 0;
    comps[i][1].type = 0x7f; comps[i][1].tc = hits[i];
    *(void**)(things[i] + 0x44) = comps[i]; *(void**)(things[i] + 0x48) = comps[i] + 2; *(void**)(things[i] + 0x4c) = comps[i] + 2;
    if (hit) *(unsigned long*)(things[i] + 0x2c) = 0x80000000UL;
    g_sphere[i][0] = x; g_sphere[i][1] = y; g_sphere[i][2] = z; g_sphere[i][3] = r;
    *(void**)(mapwhos[i] + 0x04) = things[i];               // CTCBase::ParentThing
}
int main()
{
    static long levelInfo[3][5];                            // CMapwhoLevelInfo: Size, Extents.X, Extents.Y, ScaleShift, NoThings
    static SNode* cells0[16];
    static void* cellVectors[3][3]; static void* scratch[4]; static unsigned char toolsImage[0x18];
    static unsigned char physicsImage[0x10];
    static SNode nodes[NT + 2];
    levelInfo[0][1] = 4;
    cellVectors[0][0] = cells0;
    *(void**)(g_mapA + 0x04) = levelInfo; *(void**)(g_mapA + 0x08) = levelInfo + 3; *(void**)(g_mapA + 0x0c) = levelInfo + 3;
    *(void**)(g_mapA + 0x14) = cellVectors; *(void**)(g_mapA + 0x18) = cellVectors + 3; *(void**)(g_mapA + 0x1c) = cellVectors + 3;
    //       i  flags strike hit   x     y     z     r    collide
    SetThing(0,  7,   1,     1,   0.0f, 0.0f, 0.0f, 1.0f, 1);   // the striker itself: IsNot rejects it
    SetThing(1,  7,   1,     1,   0.0f, 0.0f, 0.0f, 1.0f, 1);   // found
    SetThing(2,  3,   1,     1,   0.0f, 0.0f, 0.0f, 1.0f, 1);   // physical, not a game object: no filter runs
    SetThing(3,  7,   0,     1,   0.0f, 0.0f, 0.0f, 1.0f, 1);   // not strike-to-strike: no sphere test
    SetThing(4,  7,   1,     0,   0.0f, 0.0f, 0.0f, 1.0f, 1);   // no hit-locations TC
    SetThing(5,  7,   1,     1,   3.0f, 4.0f, 0.0f, 1.5f, 1);   // distance 5 == 1.5 + 3.5: not overlapping
    SetThing(6,  7,   1,     1,   3.0f, 4.0f, 0.0f, 2.0f, 1);   // distance 5 < 2 + 3.5: found (needs both radii)
    SetThing(7,  7,   1,     1,   0.0f, 0.0f, 0.0f, 1.0f, 0);   // cannot collide or interact
    SetThing(8,  7,   1,     1,   0.0f, 0.0f, 5.0f, 2.0f, 1);   // Z 5 < 5.5: found
    SetThing(9,  7,   1,     1,   0.0f, 0.0f, 6.0f, 2.0f, 1);   // Z 6 > 5.5: out (Z counted)
    SetThing(10, 7,   1,     1,   0.0f, 0.0f, 0.0f, 1.0f, 1);   // already stamped with this search's id
    SetThing(11, 7,   1,     1,   0.0f, 0.0f, 0.0f, 1.0f, 1);   // left of the local area: never visited
    *(unsigned long*)(mapwhos[10] + 0x4c) = 1;
    for (int i = 0; i < NT; ++i) nodes[i].mapwho = mapwhos[i];
    // cell (1,0): 0 1 2 3 4 10   cell (3,2): 5 6 7 8 9   cell (0,0): 11
    cells0[0 * 4 + 1] = &nodes[0]; nodes[0].next = &nodes[1]; nodes[1].next = &nodes[2]; nodes[2].next = &nodes[3];
    nodes[3].next = &nodes[4]; nodes[4].next = &nodes[10];
    cells0[2 * 4 + 3] = &nodes[5]; nodes[5].next = &nodes[6]; nodes[6].next = &nodes[7]; nodes[7].next = &nodes[8]; nodes[8].next = &nodes[9];
    cells0[0 * 4 + 0] = &nodes[11];
    *(void**)(toolsImage + 0x0c) = scratch; *(void**)(toolsImage + 0x10) = scratch; *(void**)(toolsImage + 0x14) = scratch + 4;
    const CThingSearchTools& tools = *(const CThingSearchTools*)toolsImage;
    g_striker = things[0]; g_tcPhysics = physicsImage;
    C3DVector centre; centre.X = 0.0f; centre.Y = 0.0f; centre.Z = 0.0f;
    CThingFilter_IsNot is_not(*(const CThing*)things[0]);
    CThingFilter_CombatIsStrikeToStrike strike(*(const CThing*)things[0]);
    CThingFilter_CombatOverlappingSphere sphere(centre, 3.5f);
    CThingFilter_CombatCanCollideOrInteractWithThing collide(*(const CTCPhysicsBase*)physicsImage, false, true);
    typedef CFilter_Bind4<CThingFilter_IsNot, CThingFilter_CombatIsStrikeToStrike, CThingFilter_CombatOverlappingSphere, CThingFilter_CombatCanCollideOrInteractWithThing> TFilter;
    TFilter filter(is_not, strike, sphere, collide);
    C2DBoxI area; area.TLX = 0; area.TLY = 0; area.BRX = 3; area.BRY = 3;
    std::list<CThingGameObject*> out;
    out.push_back((CThingGameObject*)things[11]);         // appended after an existing entry
    tools.GetAllThingsInRoughArea<CThingGameObject, std::list<CThingGameObject*>, TFilter>(area, filter, out);
    static const int expect[4] = { 11, 1, 6, 8 };
    int n = 0, order_ok = 1;
    for (std::list<CThingGameObject*>::iterator it = out.begin(); it != out.end(); ++it, ++n)
        if (n >= 4 || (void*)*it != (void*)things[expect[n]]) order_ok = 0;
    check(!g_bad && g_mapCalls == 1 && g_clipCalls == 1, "map list fetched once, the map clipped once");
    check(n == 4 && order_ok, "found things appended in cell / node order");
    check(!g_badArg, "filter members and the GetBoundingSphere flag");
    check(g_isNotCalls == 9, "IsNot runs on every new game object only");
    check(g_strikeCalls == 8, "strike-to-strike runs after IsNot passes");
    check(g_sphereCalls == 6, "bounding sphere asked only with a hit-locations TC after the strike test");
    check(g_collideCalls == 4, "collide / interact test runs last, only on overlapping spheres");
    check(*(void**)(toolsImage + 0x10) == scratch, "scratch map list cleared");
    check(CThingSearchTools::NoLookups == 5 && CThingSearchTools::SearchID == 1 && CThingSearchTools::NoThingsScanned == 0, "search statics");
    for (int i = 0; i < 11; ++i) check(*(unsigned long*)(mapwhos[i] + 0x4c) == 1, "scanned components stamped");
    check(*(unsigned long*)(mapwhos[11] + 0x4c) == 0, "cell left of the local area not visited");
    if (!g_fail) printf("0085b2a0_TEST PASS\n");
    return g_fail;
}
