// Behaviour test for 0x0084E590 NCameraModes::CBase::UpdateTrackMeleeOpponents: the row's defines, includes and body verbatim (the
// visible rough-area search included). Raw images at retail offsets: CBase CameraManager +0x40 -> CGameCameraManager (PObservedThing
// +0x100, World +0x74 -> CWorld PThingSearchTools +0x20); things (vptr, TypeFlags +0x6C, Dead bit of +0x91, PhysicsTC +0x60,
// PTCGraphicAppearance +0x64); CTCPhysicsBase Position +0x0C / OldPosition +0x18. Fake vtables give CTCPhysicsBase slot 70
// (GetFacingVector, +0x118) and CThingCreatureBase slot 81 (GetMaxRunningSpeed, +0x144). One thing map, one mapwho layer, one cell
// holding the candidate threats. Doubles record the C3DFrustrum calls (ctor, AddPoint, UpdateFrustrum; Focus written by
// UpdateFrustrum) and the two filters.
// Checks: a dead / missing / non-creature observed thing searches nothing; the search box is the creature's position (truncated)
// +-20; the filters get the creature and the observed thing; an empty search builds no frustum; otherwise the frustum starts at the
// camera position with up (0,0,1), takes every live threat's focal position (GetCameraFocalPos(1.0f)) at weight 1.0, skipping dead
// and filtered-out ones, and is updated once; then, unless the hero runs away from the threats' focus faster than half its max
// running speed (velocity . normalised(focus - hero focal) < -0.5 x speed at position + facing), the hero's focal position is added at
// weight 2.5 and the frustum updated again; look_dir is never written.
#include <stdio.h>
#include <string.h>
#include <math.h>
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHING_ORIGINAL_VTABLE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_GET_ORIGINAL_CHAIN
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C2DBOXI
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_STLP_VECTOR_OVERFLOW_OUTOFLINE
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_CLIPAREA
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ORIGINAL_ROUGHAREA
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ROUGHAREA_EGOD
#define FABLE_RETAIL_CTHINGMAP_PEEKCELL_INLINE
#define FABLE_RETAIL_CTCMAPWHO
#define FABLE_RETAIL_CTCMAPWHO_SEARCHID
#define FABLE_RETAIL_CTHINGCREATUREBASE_STATTYPEFLAGS
#define FABLE_RETAIL_C2DBOXI_COPY
#define FABLE_RETAIL_C2DBOXI_GETWIDTH
#define FABLE_RETAIL_C2DBOXI_GETHEIGHT
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCPHYSICSBASE_LASTPOSITION
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_CAMERAFOCALPOS
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_STATICCAMERAFOCALPOS
#define FABLE_RETAIL_NCAMERAMODES_CBASE_CYLINDER
#define FABLE_RETAIL_CGAMECAMERAMANAGER_GETOBSERVEDTHING
#define FABLE_RETAIL_CGAMECAMERAMANAGER_LOOKAROUND
#define FABLE_RETAIL_CGAMECAMERAMANAGER_LOOKAROUND_WORLD
#include "stlport/fable_containers.h"
#include "engine/C3DVector.h"
#include "engine/C2DExtentsI.h"
#include "engine/C2DBoxI.h"
// Include-order: engine/CTCMapwho.h names CTCMapwho in NThingMap::CMapwhoNode before its own class (as the landed 0x0087A7C0).
class CTCMapwho;
#include "engine/CTCMapwho.h"
#include "engine/NThingMap_CThingMap.h"
#include "engine/ThingAlive.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/NGraphicAppearance_CTCGraphicAppearance.h"
#include "engine/CWorld.h"
#include "engine/CThingSearchTools.h"
#include "engine/CFilter_Bind2.h"
#include "engine/NCameraModes_CBase.h"
#include "engine/CGameCameraManager.h"

// No shared headers for these three classes. CThingFilter_IsCreatureThreatTo (member Creature) and CThingFilter_IsAwareOfThing (member
//   PThingToBeAwareOf): Ego_d.pdb records (one-argument constructor, operator(), the private pointer; out-of-line operator() at the
//   retail addresses of their landed rows). C3DFrustrum: Ego_d.pdb `anonymous-namespace'::C3DFrustrum (Apex, Up, Points, FOV, Focus;
//   C3DFrustrum / IncorporatePoint / AddPoint / UpdateFrustrum / GetFOV / GetFocus / GetApex / GetUp) and Ego_r.pdb's out-of-line members
//   ?..@C3DFrustrum@?A0x49b2e1a3@@ (AddPoint QAEXABVC3DVector@@M@Z, IncorporatePoint the same, GetApex / GetFocus QBEABVC3DVector@@XZ);
//   with the STLport vector Focus sits at +0x28 (retail `fld [esp+7Ch]`, frustum at [esp+54h]). IncorporatePoint is inline here (retail
//   calls AddPoint 0x0084E390 then UpdateFrustrum 0x0084D0E0 at 0x0084E7A8 / 0x0084E7B1); Inferred body: add, then update.
class CThingFilter_IsCreatureThreatTo {
public:
    CThingFilter_IsCreatureThreatTo(const CThingCreatureBase& creature) : Creature(&creature) { }
    bool operator()(const CThingCreatureBase& thing) const;              // 0x006A7600
private:
    const CThingCreatureBase* Creature;
};
class CThingFilter_IsAwareOfThing {
public:
    CThingFilter_IsAwareOfThing(const CThing* thing) : PThingToBeAwareOf(thing) { }
    bool operator()(const CThing& thing) const;                          // 0x006A8630
private:
    const CThing* PThingToBeAwareOf;
};

namespace {
class C3DFrustrum {
public:
    C3DFrustrum(const C3DVector& apex, const C3DVector& up);            // 0x0084F4D0
    void AddPoint(const C3DVector& point, float weight);                 // 0x0084E390
    void UpdateFrustrum();                                               // 0x0084D0E0
    void IncorporatePoint(const C3DVector& point, float weight) { AddPoint(point, weight); UpdateFrustrum(); }
    const C3DVector& GetApex() const { return Apex; }
    const C3DVector& GetFocus() const { return Focus; }
private:
    C3DVector Apex;
    C3DVector Up;
    std::vector<std::pair<C3DVector, float> > Points;
    float FOV;
    C3DVector Focus;
};
}

namespace NCameraModes {
// Same bodies as the landed out-of-line rows 0x0084F1B0 GetObservedThing and 0x0084D050 GetWorld; inline here, as in the landed
//   UpdateCameraStretchAroundHero 0x0084D910 (retail expands both: `lea ecx,[eax+100h] ; call 0x00A01B50` and the +0x74 / +0x20 reads).
inline const CThingGameObject* CBase::GetObservedThing() const
{
    return CameraManager.GetObservedThing();
}
inline const CWorld& CBase::GetWorld() const
{
    return CameraManager.GetWorld();
}

void CBase::UpdateTrackMeleeOpponents(const C3DVector& new_cam_pos, C3DVector& look_dir) const
{
    const CThingGameObject* pthing = GetObservedThing();
    if (IS_THING_ALIVE(pthing) && (pthing->GetTypeFlags() & 8)) {
        const CThingCreatureBase* pcreature = static_cast<const CThingCreatureBase*>(pthing);
        const float COMBAT_SEARCH_RADIUS = 20.0f;
        C3DVector centre_pos(pthing->GetPos());
        std::vector<const CThingCreatureBase*> threat_things;
        GetWorld().PeekThingSearchTools().GetAllThingsInRoughArea<const CThingCreatureBase>(C2DBoxI(centre_pos.ToC2DCoordI(), (long)COMBAT_SEARCH_RADIUS),
            GFFilter_Bind2(CThingFilter_IsCreatureThreatTo(*pcreature), CThingFilter_IsAwareOfThing(pthing)), threat_things);
        if (!threat_things.empty()) {
            C3DFrustrum combatants(new_cam_pos, C3DVector(0.0f, 0.0f, 1.0f));
            for (std::vector<const CThingCreatureBase*>::iterator it = threat_things.begin(); it != threat_things.end(); ++it) {
                const CThingCreatureBase* pcret = *it;
                if (IS_THING_ALIVE(pcret))
                    combatants.AddPoint(pcret->GetTCGraphicAppearance()->GetStaticCameraFocalPos(), 1.0f);
            }
            combatants.UpdateFrustrum();
            // Inferred: default_look_pos before focus (FableWin declares focus first); see Shape (3).
            C3DVector default_look_pos = pcreature->GetTCGraphicAppearance()->GetStaticCameraFocalPos();
            C3DVector focus(combatants.GetFocus());
            C3DVector line_to_combat_focus = focus - default_look_pos;
            line_to_combat_focus.AccurateNormalise();
            C3DVector hero_velocity = pcreature->GetPhysicsTC()->GetPosition() - pcreature->GetPhysicsTC()->GetLastPosition();
            float running_away_amount = line_to_combat_focus.Dot(hero_velocity);
            const float FRACTION_OF_MAX_RUNNING_SPEED_TO_BE_CONSIDERED_RUNNING_AWAY = 0.5f;
            C3DVector to_pos = pcreature->GetPos() + pcreature->GetPhysicsTC()->GetFacingVector();
            // Inferred: FableWin tests `running_away_amount >= speed * FRACTION * -1.0` (a double -1.0: `fmul qword`, `test ah,5 ; jp`);
            //   retail multiplies once by the float -0.5f (0x01238798) and branches `test ah,41h ; je`, which this `!(<)` with a float
            //   -1.0f gives (the `>=` spelling grades `jp`, DIFFER 593v593).
            if (!(running_away_amount < pcreature->GetMaxRunningSpeed(to_pos) * FRACTION_OF_MAX_RUNNING_SPEED_TO_BE_CONSIDERED_RUNNING_AWAY * -1.0f)) {
                const float HERO_BIAS = 2.5f;
                combatants.IncorporatePoint(default_look_pos, HERO_BIAS);
                // Fidelity: desired_look_vector (Ego_d.pdb, FableWin 0x02883349) is never used; look_dir is never written (retail
                //   ends after UpdateFrustrum at 0x0084E7B1).
                C3DVector desired_look_vector = combatants.GetFocus() - combatants.GetApex();
            }
        }
    }
}
}

// ---- doubles: search plumbing ---------------------------------------------------------------------------------------------------
unsigned long CThingSearchTools::NoLookups = 0, CThingSearchTools::NoThingsScanned = 0, CThingSearchTools::SearchID = 0;
namespace NThingMap { long MaxLayers = 1; }
namespace NThingMap { extern const CLayerInfo LayerInfo[3] = { { 0, 0 }, { 0, 0 }, { 0, 0 } }; }   // Size, ScaleShift
static unsigned char g_map[0x40];
static int g_map_calls = 0; static C2DBoxI g_area;
void CThingSearchTools::GetAllThingMapsInArea(const C2DBoxI& area, std::vector<NThingMap::CThingMap*>& maps) const
{
    ++g_map_calls; g_area = area;
    void*** v = (void***)&maps;
    v[0][0] = g_map; v[1] = v[0] + 1;
}
C2DBoxI CThingSearchTools::ClipAndConvertWorldMapAreaToLocalArea(const NThingMap::CThingMap&, const C2DBoxI&) const
{
    C2DBoxI local; local.TLX = 0; local.TLY = 0; local.BRX = 1; local.BRY = 1; return local;
}
static const void* g_threat_creature = 0; static const void* g_aware_of = 0;
static const void* g_rejected = 0;
bool CThingFilter_IsCreatureThreatTo::operator()(const CThingCreatureBase& thing) const
{ g_threat_creature = *(const void* const*)this; return (const void*)&thing != g_rejected; }
bool CThingFilter_IsAwareOfThing::operator()(const CThing&) const { g_aware_of = *(const void* const*)this; return true; }

// ---- doubles: things, pointers, appearance, vectors -----------------------------------------------------------------------------
struct FakeThing { double align; unsigned char raw[0x200]; C3DVector pos; C3DVector focal; };
static FakeThing* fake_of(const void* p) { return (FakeThing*)((const char*)p - sizeof(double)); }
const C3DVector& CThing::GetPos() const { return fake_of(this)->pos; }
CBaseIntelligentPointer::CBaseIntelligentPointer() : PData(0) { }
CBaseIntelligentPointer::~CBaseIntelligentPointer() { }
CBaseObject* CBaseIntelligentPointer::GetPItem() const { return (CBaseObject*)PData; }
void CBaseIntelligentPointer::SetPItem(CBaseObject* item) { PData = (CBaseObjectPointer*)item; }
static float g_focal_arg = 0;
C3DVector NGraphicAppearance::CTCGraphicAppearance::GetCameraFocalPos(float f) const
{ g_focal_arg = f; return *(const C3DVector*)this; }                 // the fake appearance image holds the focal position
C3DVector& C3DVector::AccurateNormalise()
{ float m = (float)sqrt(X * X + Y * Y + Z * Z); if (m > 0) { X /= m; Y /= m; Z /= m; } return *this; }

// ---- doubles: the frustum ---------------------------------------------------------------------------------------------------------
static int g_ctor = 0, g_adds = 0, g_updates = 0;
static C3DVector g_apex, g_up, g_add_pos[16]; static float g_add_w[16]; static C3DVector g_focus_to_set;
namespace {
C3DFrustrum::C3DFrustrum(const C3DVector& apex, const C3DVector& up) { ++g_ctor; g_apex = apex; g_up = up; memset(this, 0, sizeof(*this)); }
void C3DFrustrum::AddPoint(const C3DVector& point, float weight) { if (g_adds < 16) { g_add_pos[g_adds] = point; g_add_w[g_adds] = weight; } ++g_adds; }
void C3DFrustrum::UpdateFrustrum() { ++g_updates; *(C3DVector*)((char*)this + 40) = g_focus_to_set; }   // Focus: retail +0x28 (STLport 12-byte Points)
}

// ---- harness ----------------------------------------------------------------------------------------------------------------------
static int g_fail = 0;
static void check(int ok, const char* what) { if (!ok) { printf("FAIL %s\n", what); g_fail = 1; } }
static C3DVector V(float x, float y, float z) { C3DVector v; v.X = x; v.Y = y; v.Z = z; return v; }
static int eq(const C3DVector& a, const C3DVector& b) { return a.X == b.X && a.Y == b.Y && a.Z == b.Z; }
static FakeThing things[8];
static unsigned char physics[0x100], appearances[8][16];
static void* creature_vtable[128]; static void* physics_vtable[128];
static C3DVector g_facing; static float g_max_speed = 0; static C3DVector g_speed_pos; static int g_speed_calls = 0;
static C3DVector* __fastcall FakeFacing(void*, void*, C3DVector* ret) { *ret = g_facing; return ret; }
static float __fastcall FakeMaxRun(void*, void*, const C3DVector* pos) { ++g_speed_calls; g_speed_pos = *pos; return g_max_speed; }
static unsigned char base_image[0x80], manager_image[0x200], world_image[0x40], tools_image[0x18];
struct SNode { void* mapwho; SNode* next; void* previous; void* cell; };
static unsigned char mapwhos[8][0x60]; static SNode nodes[8]; static SNode* cells[1];
static long level_info[1][5]; static void* cell_vectors[1][3]; static void* scratch[4];

static CThing* MakeThing(int i, float x, float y, float z, unsigned long type, bool dead, bool creature_vtbl)
{
    memset(&things[i], 0, sizeof(things[i]));
    unsigned char* raw = things[i].raw;
    things[i].pos = V(x, y, z);
    *(void**)raw = creature_vtbl ? (void*)creature_vtable : 0;
    *(unsigned long*)(raw + 0x6C) = type;
    raw[0x91] = (unsigned char)(dead ? 1 : 0);
    *(void**)(raw + 0x64) = appearances[i];
    return (CThing*)raw;
}
static void SetFocal(int i, float x, float y, float z) { *(C3DVector*)appearances[i] = V(x, y, z); }
static void Reset()
{
    g_map_calls = g_ctor = g_adds = g_updates = g_speed_calls = 0;
    g_threat_creature = g_aware_of = 0;
}
static void Observe(CThing* t) { *(void**)(manager_image + 0x104) = t; }
static void SetThreats(int n, const int* idx)
{
    for (int k = 0; k < n; ++k) {
        *(void**)(mapwhos[k] + 0x04) = things[idx[k]].raw;     // CTCMapwho parent thing
        *(unsigned long*)(mapwhos[k] + 0x4c) = 0;              // search stamp
        nodes[k].mapwho = mapwhos[k]; nodes[k].next = (k + 1 < n) ? &nodes[k + 1] : 0;
    }
    cells[0] = n ? &nodes[0] : 0;
}

int main()
{
    // the camera mode: CameraManager +0x40 -> manager (PObservedThing at +0x100: vptr + PData), World +0x74 -> world (tools +0x20)
    *(void**)(base_image + 0x40) = manager_image;
    *(void**)(manager_image + 0x74) = world_image;
    *(void**)(world_image + 0x20) = tools_image;
    *(void**)(tools_image + 0x0c) = scratch; *(void**)(tools_image + 0x10) = scratch; *(void**)(tools_image + 0x14) = scratch + 4;
    // one thing map: one layer, 1x1 cells
    level_info[0][1] = 1; cell_vectors[0][0] = cells;
    *(void**)(g_map + 0x04) = level_info; *(void**)(g_map + 0x08) = level_info + 1; *(void**)(g_map + 0x0c) = level_info + 1;
    *(void**)(g_map + 0x14) = cell_vectors; *(void**)(g_map + 0x18) = cell_vectors + 1; *(void**)(g_map + 0x1c) = cell_vectors + 1;
    creature_vtable[81] = (void*)&FakeMaxRun;
    physics_vtable[70] = (void*)&FakeFacing;
    *(void**)physics = physics_vtable;
    const NCameraModes::CBase& mode = *(const NCameraModes::CBase*)base_image;
    C3DVector cam = V(1, 2, 3), look = V(7, 8, 9);

    CThing* hero = MakeThing(0, 10.7f, -5.2f, 0, 0x0F, false, true);
    *(void**)((unsigned char*)hero + 0x60) = physics;
    SetFocal(0, 10, -5, 2);
    CThing* a = MakeThing(1, 14, -5, 0, 0x0F, false, true); SetFocal(1, 14, -5, 2);
    CThing* b = MakeThing(2, 10, 0, 0, 0x0F, false, true);  SetFocal(2, 10, 0, 2);
    CThing* d = MakeThing(3, 12, -5, 0, 0x0F, true, true);  SetFocal(3, 12, -5, 2);     // dead threat
    CThing* c = MakeThing(4, 9, -9, 0, 0x0F, false, true);  SetFocal(4, 9, -9, 2);      // filtered out
    CThing* notcreature = MakeThing(5, 0, 0, 0, 0x07, false, true);

    // 1. nothing observed / dead / not a creature: no search
    Reset(); Observe(0); mode.UpdateTrackMeleeOpponents(cam, look); check(g_map_calls == 0, "nothing observed: no search");
    ((unsigned char*)hero)[0x91] = 1;
    Reset(); Observe(hero); mode.UpdateTrackMeleeOpponents(cam, look); check(g_map_calls == 0, "dead observed thing: no search");
    ((unsigned char*)hero)[0x91] = 0;
    Reset(); Observe(notcreature); mode.UpdateTrackMeleeOpponents(cam, look); check(g_map_calls == 0, "non-creature: no search");

    // 2. no threats found: searched, no frustum
    Reset(); Observe(hero); SetThreats(0, 0); mode.UpdateTrackMeleeOpponents(cam, look);
    check(g_map_calls == 1, "searched once");
    check(g_area.TLX == 10 - 20 && g_area.TLY == -5 - 20 && g_area.BRX == 10 + 20 && g_area.BRY == -5 + 20, "search box: truncated position +-20");
    check(g_ctor == 0 && g_adds == 0 && g_updates == 0, "empty: no frustum");

    // 3. threats a, b, d (dead), c (filtered): frustum from the camera, live threats' focal positions at weight 1
    int idx[] = { 1, 2, 3, 4 };
    Reset(); g_rejected = c;                               // the threat filter rejects thing c
    SetThreats(4, idx);
    g_focus_to_set = V(12, -2.5f, 2);                      // after the first update
    // hero velocity: position - last position (physics +0x0C / +0x18)
    *(C3DVector*)(physics + 0x0C) = V(3, 1, 0); *(C3DVector*)(physics + 0x18) = V(1, 1, 0);   // velocity (2, 0, 0)
    g_facing = V(0, 1, 0); g_max_speed = 4.0f;
    mode.UpdateTrackMeleeOpponents(cam, look);
    check(g_threat_creature == hero && g_aware_of == hero, "filters bound to the creature and the observed thing");
    check(g_ctor == 1 && eq(g_apex, cam) && eq(g_up, V(0, 0, 1)), "frustrum(camera, up)");
    check(g_adds >= 2 && eq(g_add_pos[0], V(14, -5, 2)) && g_add_w[0] == 1.0f && eq(g_add_pos[1], V(10, 0, 2)) && g_add_w[1] == 1.0f, "live threats added at weight 1");
    check(g_focal_arg == 1.0f, "focal position at blend 1");
    check(g_speed_calls == 1 && eq(g_speed_pos, V(10.7f + 0.0f, -5.2f + 1.0f, 0.0f)), "max running speed at position + facing");
    // focus - hero focal = (2, 2.5, 0): velocity (2,0,0) . unit -> positive, not running away -> hero focal added at 2.5, updated again
    check(g_adds == 3 && eq(g_add_pos[2], V(10, -5, 2)) && g_add_w[2] == 2.5f, "dead / filtered threats skipped; hero focal added at bias 2.5");
    check(g_updates == 2, "updated after the threats and after the hero bias");
    check(eq(look, V(7, 8, 9)), "look_dir untouched");

    // 4. running away: velocity (-4, 0, 0) along a focus straight ahead in +X: -4 < -0.5 * 4 -> no hero bias
    Reset(); SetThreats(2, idx); g_focus_to_set = V(20, -5, 2);
    *(C3DVector*)(physics + 0x0C) = V(-3, 1, 0); *(C3DVector*)(physics + 0x18) = V(1, 1, 0);
    mode.UpdateTrackMeleeOpponents(cam, look);
    check(g_adds == 2 && g_updates == 1, "running away: no hero bias");
    // 5. exactly at the limit (-2 == -0.5 * 4): not below -> bias added
    Reset(); SetThreats(2, idx);
    *(C3DVector*)(physics + 0x0C) = V(-1, 1, 0);
    mode.UpdateTrackMeleeOpponents(cam, look);
    check(g_adds == 3 && g_updates == 2, "at the limit: bias added");
    // 6. only dead threats: frustum built and updated, nothing added before the bias
    int dead_only[] = { 3 };
    Reset(); SetThreats(1, dead_only); *(C3DVector*)(physics + 0x0C) = V(3, 1, 0);
    mode.UpdateTrackMeleeOpponents(cam, look);
    check(g_ctor == 1 && g_adds == 1 && eq(g_add_pos[0], V(10, -5, 2)) && g_updates == 2, "dead threats skipped");
    (void)b; (void)d;
    if (!g_fail) printf("0084e590_TEST PASS\n");
    return g_fail;
}