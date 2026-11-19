// Behaviour test for 0x00727220 CApplyAdditionalImpulseIfInCombatAndTooClose::operator()(CThing*) const: the row's defines, includes,
// class and body verbatim over the shared headers (+ the hand4d overlay opt-in NCREATUREMODE_PUSHABLEBYSELFAVOIDANCE), run on zeroed
// thing / physics-component images: things carry the TCI 0x4D presence bit (word +0x28 bit 13) and a sorted component map (+0x44
// begin / +0x48 end, the real out-of-line LowerBound), PhysicsTC +0x60; physics components carry ParentThing +0x04, Radius +0x34 and a
// fake vtable (IsPushableByHero +0x14C, CanCollideWithThings +0xE4, GetFacingVector +0x118 by hidden pointer, ApplyControlledImpulse
// +0x144). Covers: both solid-to-thing tests (and the thing each is asked about), the repulsion console flag, IsPerformingAnyAction,
// IsPushableByHero, self-avoidance, the strict vertical overlap, both collision flags, the strict squared XY band (radius + 0.1
// tolerance), the push away scaled by the penetration of the band (Z dropped), and the same-spot branch (facing * 0.1, rotated by
// plus_minus * (random(45) * plus_minus) / 360 turns, plus_minus -1 on even frames).
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
// ---- row body ----
// CApplyAdditionalImpulseIfInCombatAndTooClose::operator()(CThing* nearby_creature) const -- the for_each functor of CTCPhysicsNavigator::UpdateAdditionalImpulse: when the owner and the nearby creature are solid to each other, creature-action repulsion is on and the owner creature is busy, pushable by the hero and by self-avoidance, and the nearby creature overlaps the owner vertically and lies within the combined radius plus the tolerance band in XY (both colliding with things): push the owner away from it (scaled down by how far into the band it is), or, when they share a spot, along its facing vector turned by a random angle of up to 45 degrees whose sign alternates with the world frame
// Grade: RELOCATION_MATCH (hand4d 2026-11-19, first compiling grade). The parked local model (CThingCreatureBase* parameter, non-const,
//   "GetVelocity" / "AddAdditionalImpulse" slot names, hand-expanded IsToApplyAdditionalImpulse) is replaced by the PDB class with
//   Ego_d.pdb's locals and scopes (solid_to_thing, tc_solid_to_thing; between, vertical_distance_between, min_radius; between_sq_mag,
//   radius_sq_mag, tolerance_band; scale, between_mag; dist; push_vector, world, push_angle, plus_minus, push_rotation_degrees) in
//   FableWin.pdb's statement order (0x0252CC60) over the shared headers, as the landed sibling 0x00726160. Retail literals are floats
//   (0.0001f, 0.0f, 0.2f, 0.1f, 1/360 dword constants) where FableWin has doubles. The slot names follow the sibling: +0x118 is
//   GetFacingVector, +0x144 ApplyControlledImpulse (the FableWin revision calls the same pair at +0x128 / +0x154).
// Overlay: engine/NCreatureMode.h FABLE_RETAIL_NCREATUREMODE_PUSHABLEBYSELFAVOIDANCE (new, declaration only).
// Leaf: ??RCApplyAdditionalImpulseIfInCombatAndTooClose@@QBEXPAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00727220, 732 bytes, thiscall `ret 4`. Name: Ego_r.pdb / FableWin.pdb
//   ??RCApplyAdditionalImpulseIfInCombatAndTooClose@@QBEXPAVCThing@@@Z (Ego_r 0x0073E1E0, FableWin 0x0252CC60, tc_physics_navigator.obj).
// Class: Ego_d.pdb record 0x390B5 (sizeof 24): Position (C3DVector) +0x00, Radius +0x0C, Owner (CTCPhysicsBase&) +0x10, OwnerCreature
//   (const CThingCreatureBase*) +0x14; ctor (const C3DVector&, float, CTCPhysicsBase&); operator() const; static __fastcall
//   GetToleranceBand / IsToApplyAdditionalImpulse(const CThing&); private GetPushRotationExtentDegrees() const. All inline in retail.
//   GetToleranceBand returns its function-local static `tolerance_band` (FableWin public ?tolerance_band@?1??GetToleranceBand@...@4MA;
//   retail 0x01381198, read at 0x00727347). IsToApplyAdditionalImpulse (FableWin 0x0252BFC0, Ego_d locals owner / owner_creature):
//   NGlobalConsole::AllowCreatureActionRepulsion (FableWin data 0x04A40A29; retail 0x0137574A, read at 0x007272B6), then
//   IsPerformingAnyAction, the physics TC's IsPushableByHero and NCreatureMode::IsPhysicsPushableBySelfAvoidance.
// Callees: CVectorMap LowerBound 0x0040F020 (inline GetTC<CTCSolidToThing>, twice), CTCSolidToThing::IsSolidToThing 0x00770DE0,
//   CThingCreatureBase::IsPerformingAnyAction 0x006622A0, NCreatureMode::IsPhysicsPushableBySelfAvoidance 0x0085FE60, CThing::GetPos
//   0x004C73D0, CWorld::GetFrame 0x0049D870, CWorld::GetWorldSeed 0x0049D810, GFFloatRandom 0x004978A0, C3DVector::RotateXY 0x00A14040;
//   CTCPhysicsBase virtuals IsPushableByHero (+0x14C), CanCollideWithThings (+0xE4, twice), GetFacingVector (+0x118),
//   ApplyControlledImpulse (+0x144).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCSOLIDTOTHING
#define FABLE_RETAIL_CTCSOLIDTOTHING_ISSOLIDTOTHING
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_NORMALISE_SQUARE_HELPER
#define FABLE_RETAIL_C3DVECTOR_MAGNITUDE_SQRTF
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_WORLD
#define FABLE_RETAIL_CMAINGAMECOMPONENT_GETWORLD
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_ACTIONQUERY
#define FABLE_RETAIL_NCREATUREMODE_PUSHABLEBYSELFAVOIDANCE
#include "fable_compiler.h"
#include <math.h>
#pragma intrinsic(fabs, sqrt)
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CTCSolidToThing.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CWorld.h"
#include "engine/CMainGameComponent.h"
#include "engine/GFFloatRandom.h"
#include "engine/GFDebugPDynamicCast.h"
#include "engine/NCreatureMode.h"

namespace NGlobalConsole { extern bool AllowCreatureActionRepulsion; }   // 0x0137574A

class CApplyAdditionalImpulseIfInCombatAndTooClose {
public:
    CApplyAdditionalImpulseIfInCombatAndTooClose(const C3DVector& position, float radius, CTCPhysicsBase& owner);
    void operator()(CThing* nearby_creature) const;
    static float FABLE_FASTCALL GetToleranceBand()
    {
        static float tolerance_band = 0.1f;
        return tolerance_band;
    }
    static bool FABLE_FASTCALL IsToApplyAdditionalImpulse(const CThing& owner)
    {
        if (NGlobalConsole::AllowCreatureActionRepulsion) {
            const CThingCreatureBase* owner_creature = GFDebugPDynamicCast(&owner, (const CThingCreatureBase*)0);
            if (owner_creature->IsPerformingAnyAction() && owner_creature->GetPhysicsTC()->IsPushableByHero()
                && NCreatureMode::IsPhysicsPushableBySelfAvoidance(*owner_creature))
                return true;
        }
        return false;
    }
private:
    float GetPushRotationExtentDegrees() const { return 45.0f; }
    C3DVector Position;                                      // +0x00
    float Radius;                                            // +0x0c
    CTCPhysicsBase& Owner;                                   // +0x10
    const CThingCreatureBase* OwnerCreature;                 // +0x14
};

void CApplyAdditionalImpulseIfInCombatAndTooClose::operator()(CThing* nearby_creature) const
{
    bool solid_to_thing = true;
    CTCSolidToThing* tc_solid_to_thing;
    if (Owner.GetParentThing().GetTC(&tc_solid_to_thing))
        solid_to_thing = tc_solid_to_thing->IsSolidToThing(nearby_creature);
    if (solid_to_thing && nearby_creature->GetTC(&tc_solid_to_thing))
        solid_to_thing = tc_solid_to_thing->IsSolidToThing(&Owner.GetParentThing());
    if (solid_to_thing && IsToApplyAdditionalImpulse(Owner.GetParentThing())) {
        C3DVector between = nearby_creature->GetPos() - Position;
        float vertical_distance_between = fabs(between.Z);
        between.Z = 0.0f;
        float min_radius = nearby_creature->GetPhysicsTC()->GetRadius() + Radius;
        if (vertical_distance_between < min_radius) {
            float tolerance_band = GetToleranceBand();
            float between_sq_mag = between.GetSquaredMagnitude();
            float radius_sq_mag = GFSqr(min_radius + tolerance_band);
            if (Owner.CanCollideWithThings() && nearby_creature->GetPhysicsTC()->CanCollideWithThings() && between_sq_mag < radius_sq_mag) {
                float scale = 1.0f;
                float between_mag = GFRoot(between_sq_mag);
                if (between_mag > 0.0001f) {
                    float dist = between_mag - min_radius;
                    if (dist > 0.0f)
                        scale = (tolerance_band - dist) / tolerance_band;
                    between /= between_mag;
                    between *= -scale * 0.2f;
                    Owner.ApplyControlledImpulse(between);
                } else {
                    C3DVector push_vector = Owner.GetFacingVector();
                    push_vector *= 0.1f;
                    const CWorld& world = CMainGameComponent::Get()->GetWorld();
                    float plus_minus = (CWorld::GetFrame() % 2 == 0) ? -1.0f : 1.0f;
                    float push_angle = GFFloatRandom(GetPushRotationExtentDegrees(), world.GetWorldSeed()) * plus_minus;
                    float push_rotation_degrees = push_angle * (1.0f / 360.0f);
                    push_vector.RotateXY(plus_minus * push_rotation_degrees);
                    Owner.ApplyControlledImpulse(push_vector);
                }
            }
        }
    }
}

// ---- doubles ----
bool NGlobalConsole::AllowCreatureActionRepulsion = true;
static unsigned char g_mgc[0x40];
CMainGameComponent* CMainGameComponent::PMainGameComponent = (CMainGameComponent*)g_mgc;
enum { NT = 3 };
static unsigned char things[NT][0x200], physics[NT][0x80], solids[NT][0x20];
static int g_busy[NT], g_pushableByHero[NT], g_selfAvoid[NT], g_collides[NT];
static const void* g_notSolidTo[NT];
static C3DVector g_pos[NT], g_facing[NT];
static int g_impulses = 0; static C3DVector g_impulse; static const void* g_impulseOn = 0;
static const void* g_solidAskedAbout[8]; static int g_solidCalls = 0;
static int TI(const void* p) { return (int)(((const unsigned char*)p - (const unsigned char*)things) / 0x200); }
static int PI(const void* p) { return (int)(((const unsigned char*)p - (const unsigned char*)physics) / 0x80); }
static int SI(const void* p) { return (int)(((const unsigned char*)p - (const unsigned char*)solids) / 0x20); }
bool CTCSolidToThing::IsSolidToThing(const CThing* thing) const
{
    if (g_solidCalls < 8) g_solidAskedAbout[g_solidCalls] = thing;
    ++g_solidCalls;
    return (const void*)thing != g_notSolidTo[SI(this)];
}
bool CThingCreatureBase::IsPerformingAnyAction() const { return g_busy[TI(this)] != 0; }
bool __fastcall NCreatureMode::IsPhysicsPushableBySelfAvoidance(const CThing& thing) { return g_selfAvoid[TI(&thing)] != 0; }
const C3DVector& CThing::GetPos() const { return g_pos[TI(this)]; }
static long g_frame = 0;
long __fastcall CWorld::GetFrame() { return g_frame; }
static unsigned long g_seed = 0;
static unsigned char g_world[0x400];
unsigned long* CWorld::GetWorldSeed() const { return ((const void*)this == g_world) ? &g_seed : 0; }
static float g_unit = 0.0f, g_range = 0.0f; static unsigned long* g_seedArg = 0;
float __fastcall GFFloatRandom(float range, unsigned long* seed) { g_range = range; g_seedArg = seed; return range * g_unit; }
static float g_turns = 99.0f; static int g_rotations = 0;
void C3DVector::RotateXY(float angle) { g_turns = angle; ++g_rotations; }
static bool __fastcall PushableByHero(void* self, void*) { return g_pushableByHero[PI(self)] != 0; }
static bool __fastcall CanCollide(void* self, void*) { return g_collides[PI(self)] != 0; }
static C3DVector* __fastcall Facing(void* self, void*, C3DVector* out) { *out = g_facing[PI(self)]; return out; }
static void __fastcall Apply(void* self, void*, const C3DVector* v) { ++g_impulses; g_impulse = *v; g_impulseOn = self; }
static void* g_physVt[128];
CApplyAdditionalImpulseIfInCombatAndTooClose::CApplyAdditionalImpulseIfInCombatAndTooClose(const C3DVector& position, float radius, CTCPhysicsBase& owner)
    : Position(position), Radius(radius), Owner(owner), OwnerCreature(0) {}

// ---- harness ----
static int g_fail = 0;
static void check(int ok, const char* what) { if (!ok) { printf("FAIL %s\n", what); g_fail = 1; } }
static bool Near(float a, float b) { return fabs(a - b) < 1e-5f; }
struct SPair { long type; void* comp; };
static SPair g_maps[NT][3];
static void SetSolid(int i, bool has)
{
    unsigned long* word = (unsigned long*)(things[i] + 0x28);
    if (has) *word |= 0x2000; else *word &= ~0x2000UL;
}
static void Reset()
{
    for (int i = 0; i < NT; ++i) {
        memset(things[i], 0, sizeof(things[i])); memset(physics[i], 0, sizeof(physics[i]));
        *(void**)(things[i] + 0x60) = physics[i];
        *(void**)physics[i] = g_physVt; *(void**)(physics[i] + 0x04) = things[i];
        g_maps[i][0].type = 0x10; g_maps[i][0].comp = 0;
        g_maps[i][1].type = 0x4d; g_maps[i][1].comp = solids[i];
        g_maps[i][2].type = 0x60; g_maps[i][2].comp = 0;
        *(void**)(things[i] + 0x44) = g_maps[i]; *(void**)(things[i] + 0x48) = g_maps[i] + 3;
        g_busy[i] = 1; g_pushableByHero[i] = 1; g_selfAvoid[i] = 1; g_collides[i] = 1; g_notSolidTo[i] = 0;
        g_facing[i].X = 1.0f; g_facing[i].Y = 0.0f; g_facing[i].Z = 0.5f;
    }
    *(float*)(physics[0] + 0x34) = 1.0f; *(float*)(physics[1] + 0x34) = 0.5f;
    g_pos[1].X = 1.5f; g_pos[1].Y = 0.0f; g_pos[1].Z = 0.2f;
    NGlobalConsole::AllowCreatureActionRepulsion = true;
    g_impulses = 0; g_impulseOn = 0; g_solidCalls = 0; g_rotations = 0; g_turns = 99.0f; g_range = 0.0f; g_seedArg = 0;
    g_unit = 0.0f; g_frame = 0;
}
static void Run()
{
    C3DVector origin; origin.X = 0.0f; origin.Y = 0.0f; origin.Z = 0.0f;
    const CApplyAdditionalImpulseIfInCombatAndTooClose f(origin, 1.0f, *(CTCPhysicsBase*)physics[0]);
    f((CThing*)things[1]);
}
int main()
{
    *(void**)(g_mgc + 0x24) = g_world;
    g_physVt[0x14c / 4] = (void*)&PushableByHero; g_physVt[0xe4 / 4] = (void*)&CanCollide;
    g_physVt[0x118 / 4] = (void*)&Facing; g_physVt[0x144 / 4] = (void*)&Apply;
    check(sizeof(CApplyAdditionalImpulseIfInCombatAndTooClose) == 24 && CApplyAdditionalImpulseIfInCombatAndTooClose::GetToleranceBand() == 0.1f, "layout / band");
    // 1. edge of the radius (between 1.5 = owner 1.0 + nearby 0.5): pushed away, unscaled, Z dropped
    Reset(); Run();
    check(g_impulses == 1 && g_impulseOn == physics[0] && Near(g_impulse.X, -0.2f) && Near(g_impulse.Y, 0.0f) && g_impulse.Z == 0.0f, "edge push");
    // 2. half way into the 0.1 band, diagonal: scale 0.5
    Reset(); g_pos[1].X = 0.93f; g_pos[1].Y = 1.24f; g_pos[1].Z = 0.3f; Run();
    check(g_impulses == 1 && Near(g_impulse.X, -0.06f) && Near(g_impulse.Y, -0.08f) && g_impulse.Z == 0.0f, "band push scaled");
    // 3. outside the band (1.7^2 > 1.6^2): nothing
    Reset(); g_pos[1].X = 1.7f; Run();
    check(g_impulses == 0, "outside band");
    Reset(); g_pos[1].X = 1.55f; Run();
    check(g_impulses == 1, "inside band");
    // 4. vertical overlap is strict
    Reset(); g_pos[1].Z = 1.5f; Run();
    check(g_impulses == 0, "vertical == radius");
    Reset(); g_pos[1].Z = -1.4f; Run();
    check(g_impulses == 1, "vertical below radius (negative)");
    // 5. same spot: facing * 0.1 rotated; even frame -> plus_minus -1, twice -> +1/16 turn for a half-range random
    Reset(); g_pos[1].X = 0.0f; g_pos[1].Z = 0.1f; g_unit = 0.5f; g_frame = 4; Run();
    check(g_impulses == 1 && Near(g_impulse.X, 0.1f) && g_impulse.Y == 0.0f && Near(g_impulse.Z, 0.05f), "same spot facing");
    check(g_rotations == 1 && g_range == 45.0f && g_seedArg == &g_seed && Near(g_turns, 22.5f / 360.0f), "same spot rotation even");
    Reset(); g_pos[1].X = 0.0f; g_unit = 0.5f; g_frame = -3; Run();
    check(g_rotations == 1 && Near(g_turns, 22.5f / 360.0f), "same spot rotation odd");
    // 6. solid-to-thing filters
    Reset(); SetSolid(0, true); g_notSolidTo[0] = things[1]; Run();
    check(g_impulses == 0 && g_solidCalls == 1 && g_solidAskedAbout[0] == things[1], "owner not solid to nearby");
    Reset(); SetSolid(0, true); g_notSolidTo[0] = things[2]; Run();
    check(g_impulses == 1 && g_solidCalls == 1, "owner solid to nearby");
    Reset(); SetSolid(1, true); g_notSolidTo[1] = things[0]; Run();
    check(g_impulses == 0 && g_solidCalls == 1 && g_solidAskedAbout[0] == things[0], "nearby not solid to owner");
    Reset(); SetSolid(0, true); SetSolid(1, true); g_notSolidTo[1] = things[2]; Run();
    check(g_impulses == 1 && g_solidCalls == 2 && g_solidAskedAbout[1] == things[0], "both solid");
    Reset(); SetSolid(0, true); SetSolid(1, true); g_notSolidTo[0] = things[1]; Run();
    check(g_impulses == 0 && g_solidCalls == 1, "owner refusal skips the nearby test");
    // 7. owner gates
    Reset(); NGlobalConsole::AllowCreatureActionRepulsion = false; Run(); check(g_impulses == 0, "repulsion off");
    Reset(); g_busy[0] = 0; Run(); check(g_impulses == 0, "owner idle");
    Reset(); g_busy[1] = 0; Run(); check(g_impulses == 1, "nearby idle does not matter");
    Reset(); g_pushableByHero[0] = 0; Run(); check(g_impulses == 0, "owner not pushable by hero");
    Reset(); g_selfAvoid[0] = 0; Run(); check(g_impulses == 0, "owner not pushable by self avoidance");
    // 8. collision flags
    Reset(); g_collides[0] = 0; Run(); check(g_impulses == 0, "owner does not collide");
    Reset(); g_collides[1] = 0; Run(); check(g_impulses == 0, "nearby does not collide");
    if (!g_fail) printf("00727220_TEST PASS\n");
    return g_fail;
}