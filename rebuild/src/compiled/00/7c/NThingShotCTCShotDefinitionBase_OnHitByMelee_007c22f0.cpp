// 007c22f0 NThingShot::CTCShotDefinitionBase::OnHitByMelee
// Flags:
// The two original Init overloads share this translation unit. Their bodies are independently graded.
// Leaf: ?OnHitByMelee@CTCShotDefinitionBase@NThingShot@@UAEXAAVCThing@@PAV3@ABVC3DVector@@2_N3@Z
#define FABLE_RETAIL_SHOT_ONHIT_PASS29
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCSHOTDEFINITIONBASE_COLLISION
#define FABLE_RETAIL_CTCSHOTDEFINITIONBASE_MOVEMENT
#define FABLE_RETAIL_CTCSHOTDEFINITIONBASE_ONHITBYMELEE
#define FABLE_RETAIL_CTCSHOTDEFINITIONBASE_INIT
#define FABLE_RETAIL_CTCSHOTDEFINITIONBASE_SHOTACCESSORS
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE
#define FABLE_RETAIL_CCHARSTRING_FORCEINLINE_EQ
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY
#define FABLE_RETAIL_CINTELLIGENTPOINTER_CONVERT_MUTABLE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_DEREFERENCE
#define FABLE_RETAIL_C3DVECTOR_CROSS_MEMBER_X_GROUP
#define FABLE_RETAIL_C3DVECTOR_NONZERO_SQUARED_XYZ
#define FABLE_RETAIL_C3DVECTOR_ANGLE_BODIES
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
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
#include "engine/CTCWeaponTrail.h"
#include "engine/CTCSolidToThing.h"
#include "engine/CTCObjectAugmentations.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/NGraphicAppearance_CTCGraphicAppearance.h"
#include "engine/CWorld.h"
#include "engine/ThingAlive.h"
#include "engine/CTCTargetingBase.h"
#include "engine/CTCTargetingPlayer.h"
#include "engine/CPlayer.h"
#include "engine/NTargeting.h"
#include "engine/CMainGameComponent.h"
#include "engine/CAIGameCameraBase.h"
#include "engine/CGameCameraManager.h"
#include "engine/CCamera.h"
#include "engine/GFAngles.h"
#include "engine/CThingMultiArrowShotDef.h"
#include "engine/CDefinitionManager.h"
#include "engine/CDefClassBase_GetSubDef.h"
#include "engine/GFDebugPDynamicCast.h"
extern const float DELTA_NOTIONAL_ZERO;
#include "fable_assert.h"
#include <string.h>
// Inline-budget: native calls this complete owner out of line. Inline linkage coalesces its already landed copy.
inline void NThingShot::CTCShotDefinitionBase::Init(CDefPointer<const CThingShotDef> def, CThing* shooter, CThing* weapon, const C3DVector& target,
                                             float accuracy, float damageMultiplier, float buildupTime,
                                             const CDefPointer<const CMeleeCombatAbilityDef>& ability)
{
    PShotDef = def;
    PShotAbilityDef = ability;
    PShooter = CIntelligentPointer<CThing>(shooter);
    PWeapon = CIntelligentPointer<CThing>(weapon);
    ShotBuildupTime = buildupTime;
    DamageMultiplier = damageMultiplier;
    AccuracyPercentage = accuracy;
    DieOnFrame = -1;
    PassedTarget = false;
    KnockDown = false;
    Decapitate = false;
    if (!PShotAbilityDef.IsNull())
        KnockDown = true;
    memcpy(TargetPos, &target, sizeof(target));
    ParentThing->GetPhysicsTC()->ReturnToDefaultInteractionFlags();
    InitAttachedEffects();
    InitMovement();
    CTCObjectAugmentations* augmentations;
    if (IS_THING_ALIVE(weapon) && ParentThing->GetTC(&augmentations))
        augmentations->CopyAugmentationDataFromThing(*weapon);
    if (IS_THING_ALIVE(PShooter)) {
        CTCSolidToThing* shooterSolid;
        if (PShooter->GetTC(&shooterSolid))
            shooterSolid->AddThingNotSolidTo(ParentThing);
        CTCSolidToThing* solid;
        if (!ParentThing->GetTC(&solid)) {
            solid = 0;
            {
                CCharString name("CTCSolidToThing");
                ParentThing->AddTC(name, false, 0);
            }
            ParentThing->GetTC(&solid);
        }
        solid->AddThingNotSolidTo(PShooter.Get());
    }
    ParentThing->GetTCGraphicAppearance()->SetForceResendEveryRenderFrame(true);
    BestCollisionPos = ParentThing->GetPos();
    BestCollisionType = (ECollideType)0;
    CTCWeaponTrail* trail;
    if (ParentThing->GetTC(&trail))
        trail->UpdateWeaponTrails();
    StartFrame = GetWorld().GetFrame();
}
// Inline-budget: native calls this complete owner out of line. Inline linkage coalesces its already landed copy.
inline void NThingShot::CTCShotDefinitionBase::Init(CDefPointer<const CThingShotDef> def, CThing* shooter, CThing* weapon, const CThing* target,
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
void NThingShot::CTCShotDefinitionBase::OnHitByMelee(CThing& thing_hitting, CThing* pweapon,
    const C3DVector& point_of_hit, const C3DVector& hit_direction, bool knockdown, bool blockable)
{
    if (thing_hitting.HasTC(TCI_TRAP)) {
        BestCollisionPos = point_of_hit;
        BestCollisionNormal = hit_direction;
        BestCollisionType = COLLIDE_TYPE_THING_SOLID;
        PBestCollisionThing = CIntelligentPointer<CThing>(&thing_hitting);
        bool killed_me;
        OnCollisionProper(killed_me);
        return;
    }
    CDefPointer<const CThingShotDef> pdef = PShotDef;
    bool is_deflected = pdef->IsDeflectedOnHitByMelee;
    if (IsMultiArrowShotType()) {
        const CThingShotDef& pt_shot_definition = GetShotDef();
        CDefPointer<const CThingMultiArrowShotDef> pt_multiarrow_shot_definition;
        if (!pt_shot_definition.GetSubDef(&pt_multiarrow_shot_definition)) { FABLE_RETAIL_ASSERT(false); }
        is_deflected = pt_multiarrow_shot_definition->MultiArrowDeflectionByMelee;
    }
    if (is_deflected) {
        CThing& shooter = thing_hitting;
        CThing* ptarget = PShooter.Get();
        float damage_multiplier = GetShotDef().MeleeHitDeflectionDamageMultiplier;
        CTCShotDefinitionBase* ptc_shot_defn = GFDebugPDynamicCast(GetParentThing().AddTC("CTCShotDefinitionThrow", true, 0), (CTCShotDefinitionBase*)0);
        FABLE_RETAIL_ASSERT(ptc_shot_defn!=NULL);
        ptc_shot_defn->Init(pdef, &shooter, 0, ptarget, 100.0f, damage_multiplier, 0.0f, CDefPointer<const CMeleeCombatAbilityDef>());
    }
}