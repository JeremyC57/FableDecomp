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
