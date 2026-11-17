// CTCForcePushed::FrameUpdate() -- keep the parent creature in the being-force-pushed action; while it is still within the push distance of its start point, slide it along the push direction each frame (the physics' forced move position, facing back along the push) unless, once moving, the physics' last position plus the per-frame push is more than a fifth of a frame's movement (2D) away from where the creature is (it was blocked): then finish the push early with the distance left (and a HIT_EXPLOSION particle effect when the parent is a physical thing); past the push distance finish it normally
// Leaf: ?FrameUpdate@CTCForcePushed@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) 0x00798940, 615 bytes, thiscall `ret`. Grade: RELOCATION_MATCH (e05 egod lane).
// Names: Ego_r.pdb ?FrameUpdate@CTCForcePushed@@UAEXXZ; Ego_d tc_force_pushed.obj locals verbatim (pcreature, currrent_pos [sic],
//   distance_from_start_point_sq; parent_physics; expected_position; number_of_metres_left_in_movement; parent_physical_thing, effect,
//   pspecial_effects; thing) in Ego_d's scopes. FITTED: pspecial_effects is declared before effect (retail zeroes the handle first,
//   0x00798A44 / 0x00798A48; Ego_d's debug frame offsets do not give declaration order -- checked against e04's matched 0x0057AEB4).
// Callees: CCreatureAction_BeingForcePushed(CThingCreatureBase&) 0x008BD0B0 and the implicit destructor's ~CCombatActionBase
//   0x0062E900, CThingCreatureBase::SetCurrentAction 0x006644F0, CThing::GetPos 0x004C73D0, CDefClassBase::GetSubDef<CSpecialEffectsDef>
//   0x00669580 (the shared body of engine/CDefClassBase_GetSubDef.h is visible; VC7.1 keeps it a call by itself -- no inline control), CSpecialEffectsDef::GetEffectFromString 0x00669D30,
//   CTCSpecialAbilities::CreateParticleEffect 0x0072AB70, CTCPhysicsBase slots 34 (+0x88) SetForcedToMovePos / 66 (+0x108)
//   SetFacingAngleXY, CTCForcePushed::FinishBeingForcePushed 0x00798770, fmod 0x00BFEB28 (C3DVector::GetAngleXY's wrap).
// GFSqr (FableWin.pdb ?GFSqr@@YIMM@Z, existing opt-in FABLE_RETAIL_C3DVECTOR_NORMALISE_SQUARE_HELPER) for the two squared bounds closed
//   the last residue, the operand order of the expected-position X sum (`fld [esi+2Ch] ; fadd [edi+18h]`, 0x007989E2): an x87 order
//   keyed on c2 record count (a dead-local ruler needed +2 there; GFSqr's inline parameter supplies them).
// Fidelity: `thing` (Ego_d innermost block, CThing*) holds CreateParticleEffect's result and is not read again (retail discards eax, 0x00798AB4).
// Headers: new engine/CTCForcePushed.h (Ego_d class record); opt-ins FABLE_RETAIL_CCREATUREACTION_BEINGFORCEPUSHED_COMBAT (the class over
//   CCombatActionBase) and FABLE_RETAIL_GFDEBUGDYNAMICCAST (CThing.h: the checked cast for any row; Ego_d / FableWin instantiate
//   GFDebugDynamicCast<CThingCreatureBase,CThingPhysical> -- the match does not depend on it); existing FABLE_RETAIL_CTCPHYSICSBASE_LASTPOSITION,
//   FABLE_RETAIL_C3DVECTOR_ANGLE_BODIES, FABLE_RETAIL_C3DVECTOR_SQUARED2D_ONE_EXPRESSION_PASS23.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_GFDEBUGDYNAMICCAST
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_NORMALISE_SQUARE_HELPER
#define FABLE_RETAIL_C3DVECTOR_ANGLE_BODIES
#define FABLE_RETAIL_C3DVECTOR_SQUARED2D_ONE_EXPRESSION_PASS23
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCPHYSICSBASE_LASTPOSITION
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTION_BEINGFORCEPUSHED_COMBAT
#define FABLE_RETAIL_CSPECIALABILITIESFORCEPUSHDEF
#define FABLE_RETAIL_CSPECIALEFFECTSDEF_TYPENAME
#define FABLE_RETAIL_CTCSPECIALABILITIES_FULL
#include "fable_string.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CTCForcePushed.h"
#include "engine/CCreatureAction_BeingForcePushed.h"
#include "engine/CDefClassBase.h"
#include "engine/CSpecialAbilitiesForcePushDef.h"
#include "engine/CSpecialEffectsDef.h"
#include "engine/CDefClassBase_GetSubDef.h"
#include "engine/CTCSpecialAbilities.h"

void CTCForcePushed::FrameUpdate()
{
    CThingCreatureBase* pcreature = static_cast<CThingCreatureBase*>(&GetParentThing());
    pcreature->SetCurrentAction(CCreatureAction_BeingForcePushed(*pcreature));
    C3DVector currrent_pos = pcreature->GetPos();
    float distance_from_start_point_sq = (currrent_pos - PositionForcePushStarted).GetSquaredMagnitude();
    if (distance_from_start_point_sq < GFSqr(DistanceBeingPushed)) {
        CTCPhysicsBase* parent_physics = pcreature->GetPhysicsTC();
        if (StartedMoving) {
            C3DVector expected_position = parent_physics->GetLastPosition() + DirectionBeingPushed;
            if ((expected_position - currrent_pos).GetSquaredMagnitude2D() > GFSqr(DistanceMovingPerFrame * 0.2f)) {
                float number_of_metres_left_in_movement = 0.0f;
                if (pcreature->GetTypeFlags() & 2) {
                    number_of_metres_left_in_movement = DistanceBeingPushed - (float)sqrt(distance_from_start_point_sq);
                    CThingPhysical& parent_physical_thing = *GFDebugDynamicCast(pcreature, (CThingPhysical*)0);
                    CDefPointer<const CSpecialEffectsDef> pspecial_effects;
                    long effect = 0;
                    if (PDef->GetSubDef(&pspecial_effects) && pspecial_effects->GetEffectFromString("HIT_EXPLOSION", effect)) {
                        CThing* thing = CTCSpecialAbilities::CreateParticleEffect(effect, parent_physical_thing.GetPos());
                    }
                }
                FinishBeingForcePushed(true, number_of_metres_left_in_movement);
                return;
            }
        }
        parent_physics->SetForcedToMovePos(currrent_pos + DirectionBeingPushed, true);
        parent_physics->SetFacingAngleXY((-DirectionBeingPushed).GetAngleXY());
        StartedMoving = true;
    } else
        FinishBeingForcePushed(false, 0.0f);
}
