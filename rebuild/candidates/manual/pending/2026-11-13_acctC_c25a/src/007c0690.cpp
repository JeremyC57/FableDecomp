// NThingShot::CTCShotDefinitionBase::Init(CDefPointer<const CThingShotDef>, CThing* shooter, CThing* weapon, const CThing* target,
//   float accuracy, float damageMultiplier, float buildupTime, const CDefPointer<const CMeleeCombatAbilityDef>& ability)
// -- the target-THING overload: remember the target thing, work out the position to shoot at (the shooter's targeted position when
// first-person targeting or locked on, else the target thing's targeting position when it is vulnerable, else along the camera view
// to the player's maximum targeting distance in third-person aiming mode, else 20 units along the shot's facing, pitched onto the
// shooter's horizontal facing) and forward everything to the target-position overload.
// Retail: Fable.exe (TLC, VC7.1) @ 0x007C0690, 1088 bytes, thiscall `ret 0x20`.
// Callees: CBaseIntelligentPointer ctor 0x00A01B10 / SetPItem 0x00A01B90 / GetPItem 0x00A01B50 / dtor 0x00A01C10 (PTargetThing),
//   CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound const 0x004365B0 (PeekTC<CTCTargetingBase> / <CTCTargetingPlayer>, interface 8),
//   CTCTargetingBase::GetTargetedPos (vslot 37, +0x94) / GetPTargetLockedThing (vslot 36, +0x90), CThing::GetPlayer 0x004C7A30,
//   CPlayer::IsFirstPersonTargeting 0x004874D0, IS_THING_ALIVE<CThing*> 0x0040DCE0 (called at the locked-thing site),
//   NTargeting::IsThingVulnerableToMeleeStrikesFromThing 0x007F59F0, NTargeting::GetTargetingPositionOnThing 0x007EA2F0,
//   CTCTargetingPlayer::GetMaxTargetingDistance 0x00802390, CTCBase::GetWorld 0x00686A70, CAIGameCameraBase::GetCameraManager (vslot 9,
//   +0x24; result unused), CThing::GetPos 0x004C73D0, CTCPhysicsBase::GetFacingVector (vslot 70, +0x118), C3DVector::AccurateNormalise
//   0x00A14440 / Rotate 0x00A13C60, _CIacos 0x00BFEBA2 (GFArcCos), the target-position Init 0x007BF790.
// Shape: the shot's facing is a named local copied into `direction` (retail copies the call's temporary from its frame slot and reads the
//   sign's Z through it); GetTargetingPositionOnThing's result is a named vector assigned to target_pos, its thing a named reference taken
//   after the vulnerability test; the shooter-facing test is GetSquaredMagnitude() after Z = 0 (SQUAREDMAGNITUDE_ZYX: Y then X loaded);
//   the clamp and `float angle = GFArcCos(cos_angle)` are ResolveFacingDirection's (0x006AB820) idiom; the physics TC is reached through
//   GetParentThing() (FableWin inline accessor) into a named pointer kept across the facing call (edi).
// Leaf: ?Init@CTCShotDefinitionBase@NThingShot@@QAEXV?$CDefPointer@$$CBVCThingShotDef@@@@PAVCThing@@1PBV4@MMMABV?$CDefPointer@$$CBVCMeleeCombatAbilityDef@@@@@Z
#define FABLE_RETAIL_CTCSHOTDEFINITIONBASE_COLLISION
#define FABLE_RETAIL_CTCSHOTDEFINITIONBASE_MOVEMENT
#define FABLE_RETAIL_CTCSHOTDEFINITIONBASE_ONHITBYMELEE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_FORCEINLINE_EQ
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY
#define FABLE_RETAIL_CINTELLIGENTPOINTER_CONVERT_MUTABLE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_DEREFERENCE
#define FABLE_RETAIL_C3DVECTOR_CROSS_MEMBER_X_GROUP
#define FABLE_RETAIL_C3DVECTOR_NONZERO_SQUARED_XYZ
#define FABLE_RETAIL_C3DVECTOR_ANGLE_BODIES
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_ORIGINAL_VTABLE
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHING_CUTSCENEPAUSE
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHING_GETTCDRIVER
#define FABLE_RETAIL_CTHING_MOREACCESS
#define FABLE_RETAIL_CTHING_STATEACCESS
#define FABLE_RETAIL_CTHING_LOADSINGLE
#define FABLE_RETAIL_CTHING_VIRTUALS
#define FABLE_RETAIL_CTHINGPHYSICAL_VIRTUALS
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCPHYSICSBASE_SETASMOVING
#define FABLE_RETAIL_CTCPHYSICSBASE_LASTPOSITION
#define FABLE_RETAIL_CTCENTITYEVENTS
#define FABLE_RETAIL_CTCENTITYEVENTS_ADDEVENT
#define FABLE_RETAIL_CTCDPARTICLEEMITTER_FULL
#define FABLE_RETAIL_CTCHEROEXPERIENCE
#define FABLE_RETAIL_CHITPARAMETERS_CTOR
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLDMAP
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_CRIGHTHANDEDSET_CTOR_INLINE
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_FOCAL
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_C3DVECTOR_SQUAREDMAGNITUDE_ZYX
#define FABLE_RETAIL_STLP_FIND_RANDOM_DISPATCH
#define FABLE_RETAIL_STLP_FIND_NOINLINE_BODY
#define FABLE_RETAIL_CINTELLIGENTPOINTER_EQUAL_MIXED
#define FABLE_RETAIL_CTCWEAPONTRAIL_HASPOINTS
#define FABLE_RETAIL_CTCSHOTDEFINITIONBASE_INIT
#define FABLE_RETAIL_CTCSHOTDEFINITIONBASE_INIT_POSITION
#define FABLE_RETAIL_CTCSOLIDTOTHING
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_SETFORCERESEND
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_MOVEMENT
#define FABLE_RETAIL_CTCWEAPONTRAIL_UPDATEWEAPONTRAILS
#define FABLE_RETAIL_CTCOBJECTAUGMENTATIONS_COPYFROMTHING
#define FABLE_RETAIL_CTCTARGETINGPLAYER
#define FABLE_RETAIL_CTCTARGETINGPLAYER_MAXDISTANCE
#define FABLE_RETAIL_NTARGETING_MELEE
#define FABLE_RETAIL_CMAINGAMECOMPONENT_WORLD
#define FABLE_RETAIL_CAIGAMECAMERABASE
#define FABLE_RETAIL_CGAMECAMERAMANAGER
#define FABLE_RETAIL_GFANGLES_SIGNED_ROUNDING
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_PLAYER_ISPCPROJECTILEAIMING
#define FABLE_RETAIL_C3DVECTOR_MAGNITUDE2D
#define FABLE_RETAIL_C3DVECTOR_MAGNITUDE2D_ONE_EXPRESSION
#include "engine/CThing.h"
#include "engine/NThingShot_CTCShotDefinitionBase.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CTCTargetingBase.h"
#include "engine/CTCTargetingPlayer.h"
#include "engine/CPlayer.h"
#include "engine/NTargeting.h"
#include "engine/CMainGameComponent.h"
#include "engine/CWorld.h"
#include "engine/CAIGameCameraBase.h"
#include "engine/CGameCameraManager.h"
#include "engine/CCamera.h"
#include "engine/GFAngles.h"
#include "engine/ThingAlive.h"

extern const float DELTA_NOTIONAL_ZERO;          // 0x0129BA3C ?DELTA_NOTIONAL_ZERO@@3MB (1e-4f)

void NThingShot::CTCShotDefinitionBase::Init(CDefPointer<const CThingShotDef> def, CThing* shooter, CThing* weapon, const CThing* target,
                                             float accuracy, float damageMultiplier, float buildupTime,
                                             const CDefPointer<const CMeleeCombatAbilityDef>& ability)
{
    PTargetThing = CIntelligentPointer<const CThing>(target);
    C3DVector target_pos;
    bool have_target_pos = false;
    if (IS_THING_ALIVE(shooter)) {
        const CTCTargetingBase* targeting;
        if (shooter->PeekTC(&targeting) && targeting->GetTargetedPos(target_pos, 1.0f)) {
            if (shooter->GetPlayer().IsFirstPersonTargeting())
                have_target_pos = true;
            else if (shooter->GetPlayer().IsZTargeting() && IS_THING_ALIVE(targeting->GetPTargetLockedThing()))
                have_target_pos = true;
        }
        if (!have_target_pos) {
            if (IS_THING_ALIVE(PTargetThing) && NTargeting::IsThingVulnerableToMeleeStrikesFromThing(*PTargetThing, *shooter)) {
                const CThing& target_thing = *PTargetThing;
                C3DVector pos = NTargeting::GetTargetingPositionOnThing(CMainGameComponent::Get()->PeekWorld(), target_thing, 1.0f);
                target_pos = pos;
                have_target_pos = true;
            } else if (shooter->GetType() == 2 && shooter->GetPlayer().IsPCProjectileWeaponThirdPersonAimingMode()) {
                float distance = 0.0f;
                const CTCTargetingPlayer* targeting_player;
                if (shooter->PeekTC(&targeting_player))
                    distance = targeting_player->GetMaxTargetingDistance();
                CGameCameraManager* manager;
                GetWorld().GetGameCamera()->GetCameraManager(&manager);
                target_pos = ParentThing->GetPos() + manager->GetCamera().PeekViewDirection() * distance;
                have_target_pos = true;
            }
        }
    }
    if (!have_target_pos) {
        CTCPhysicsBase* physics = GetParentThing().GetPhysicsTC();
        C3DVector facing_vector = physics->GetFacingVector();
        C3DVector direction = facing_vector;
        if (IS_THING_ALIVE(shooter)) {
            C3DVector facing = shooter->GetPhysicsTC()->GetFacingVector();
            facing.Z = 0.0f;
            if (facing.GetSquaredMagnitude() > DELTA_NOTIONAL_ZERO * DELTA_NOTIONAL_ZERO) {
                facing.AccurateNormalise();
                C3DVector axis(facing.Y, -facing.X, 0.0f);
                float sign = (facing_vector.Z > 0.0f) ? 1.0f : -1.0f;
                float cos_angle = direction.GetMagnitude2D();
                if (cos_angle < -1.0f)
                    cos_angle = -1.0f;
                else if (cos_angle > 1.0f)
                    cos_angle = 1.0f;
                float angle = GFArcCos(cos_angle);
                facing.Rotate(axis, angle * sign);
                direction = facing;
            }
        }
        target_pos = physics->GetPosition() + direction * 20.0f;
    }
    Init(def, shooter, weapon, target_pos, accuracy, damageMultiplier, buildupTime, ability);
}
