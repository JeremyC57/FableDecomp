// 008664f0 CCombatAction_GenericStrikeResponseKnockdown::Initialise
// Leaf: ?Initialise@CCombatAction_GenericStrikeResponseKnockdown@@UAEXXZ
// Flags:
#define FABLE_RETAIL_C3DVECTOR_ISNONZERO_ORIGINAL_YXZ
#define FABLE_RETAIL_CANIMATIONENTRY_GETCOMPONENT_BODY
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_GET
#define FABLE_RETAIL_CVECTORMAP_ORIGINAL_FIND
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_HASTC_FORCEINLINE
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_DEREFERENCE
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE_FULL_MEMBERS
#define FABLE_RETAIL_CCREATUREACTIONBASE_ACCESSORS
#define FABLE_RETAIL_CCOMBATACTIONBASE_MELEE
#define FABLE_RETAIL_CCOMBATACTIONBASE_FINISHACTION
#define FABLE_RETAIL_CCOMBATACTIONBASE_SOUNDCTOR
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHING_STATEACCESS
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CMODEBASE_VIRTUALS
#define FABLE_RETAIL_CTCCREATUREMODEMANAGER
#define FABLE_RETAIL_CCOMBATACTIONBASE_INITIALISE
#define FABLE_RETAIL_CCOMBATACTIONGENERICSTRIKERESPONSEKNOCKDOWN
#define FABLE_RETAIL_CCOMBATACTIONGENERICSTRIKERESPONSEKNOCKDOWN_IMPLICIT_DTOR
#define FABLE_RETAIL_C3DVECTOR_PACKED
#define FABLE_RETAIL_C3DVECTOR_NORMALISE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCPHYSICSBASE_LASTPOSITION
#define FABLE_RETAIL_FIRE_RANGED_WEAPON
#define FABLE_RETAIL_CTCCREATUREMODEMANAGER_ADDMODEPOINTER
#define FABLE_RETAIL_CANIMFILTEROFGROUP_INLINE_CTOR
#define FABLE_RETAIL_CCHARSTRING_GETCRC_INLINE
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_PLAYER_REMOVEMODE
#define FABLE_RETAIL_CWORLDMAP
#define FABLE_RETAIL_CANIMATIONENTRY_GETCOMPONENT
#define FABLE_RETAIL_CANIMATIONENTRY
#define FABLE_RETAIL_ANIMCOMPONENT
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_CMODEBASE_CTOR
#define FABLE_RETAIL_CRIGHTHANDEDSET_CTOR_INLINE
#define FABLE_RETAIL_KNOCKDOWN_INITIALISE_PASS28
#include "rebuild_abi.h"
#include "fable_string.h"
#include "engine/ETCInterfaceType.h"
#include "engine/C3DVector.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CCountedPointer.h"
#include "stlport/fable_containers.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CCreatureActionBase.h"
#include "engine/CSoundPair.h"
#include "engine/CCombatActionBase.h"
#include "engine/ThingAlive.h"
#include "engine/CTCCreatureModeManager.h"
#include "engine/NCreatureMode.h"
#include "engine/CCombatAction_GenericStrikeResponseKnockdown.h"
#include "engine/CTCHitOnCollision.h"
#include "engine/CTCHeroRangedAttacks.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CPlayer.h"
#include "engine/CRightHandedSet.h"
#include "engine/CAnimationEntry.h"
#include "engine/CAnimComponentFollowingOnAnimFilter.h"
#include "engine/NCreatureMode_CModeKnockedDown.h"
#include "engine/GFCos.h"
// FableWin PUB32/GDATA at 04ABD0BA, Ego_d PUB32 and the retail load at 013B86EF.
namespace NGlobalConsole { extern bool GCheatModeEnableRemoveProjectileModeWhenHit; }
void CCombatAction_GenericStrikeResponseKnockdown::Initialise()
{
    FlipFacing = false;
    if (AnimName == "")
        PCreature->SetBodyReorienterActive(true);
    CTCHeroRangedAttacks* ptc_ranged_attacks;
    if (PCreature->GetTC(TCI_HERO_RANGED_ATTACKS,&ptc_ranged_attacks)) {
        ptc_ranged_attacks->CancelAimingWeapon();
        if (NGlobalConsole::GCheatModeEnableRemoveProjectileModeWhenHit)
            ptc_ranged_attacks->ResetProjectileWeaponMode();
        PCreature->GetPlayer().RemoveMode((EPlayerMode)0x22);
    }
    C3DVector new_facing_dir = PCreature->GetPhysicsTC()->GetFacingVector();
    if (AnimName != "") {
        SetCombatAnimation(AnimName);
    } else {
        C3DVector point_of_hit = PointOfHit;
        if (IS_THING_ALIVE(PThingThatHit)) {
            point_of_hit = PThingThatHit->GetPos();
            const CTCHitOnCollision* ptc_hit_on_coll;
            if (PThingThatHit->PeekTC(TCI_HIT_ON_COLLISION,&ptc_hit_on_coll) && ptc_hit_on_coll->IsThrowThingsOutLaterally()) {
                C3DVector velocity = PThingThatHit->GetPhysicsTC()->GetPosition() - PThingThatHit->GetPhysicsTC()->GetLastPosition();
                if (velocity.IsNonZero()) {
                    C3DVector diff = PThingThatHit->GetPos() - PCreature->GetPos();
                    float lambda_param = diff.Dot(velocity) / velocity.Dot(velocity);
                    point_of_hit = PThingThatHit->GetPos() + velocity * lambda_param;
                }
            }
        }
        C3DVector line_2d = point_of_hit - PCreature->GetPos();
        line_2d.Z=0.0f;
        if (line_2d.IsNonZero())
            line_2d.AccurateNormalise();
        C3DVector hit_dir_2d(DirectionOfHit);
        hit_dir_2d.Z=0.0f;
        if (hit_dir_2d.IsNonZero())
            hit_dir_2d.AccurateNormalise();
        C3DVector facing_dir_2d = PCreature->GetPhysicsTC()->GetFacingVector();
        facing_dir_2d.Z = 0.0f;
        if (facing_dir_2d.IsNonZero())
            facing_dir_2d.AccurateNormalise();
        new_facing_dir = facing_dir_2d;
        float cos_thet = line_2d.Dot(facing_dir_2d);
        if (cos_thet < GFCos(1.0f/3.0f)) {
            SetCombatAnimation("RESPONSE_GENERIC_HIT_BACK_KNOCKDOWN");
            if (PCreature->GetType() == 1)
                FlipFacing = true;
            if (IS_THING_ALIVE(PThingThatHit)) {
                C3DVector line_to_target = point_of_hit - PCreature->GetPos();
                line_to_target.Z = 0.0f;
                if (line_to_target.IsNonZero()) {
                    line_to_target.AccurateNormalise();
                    new_facing_dir = -line_to_target;
                } else
                    new_facing_dir = C3DVector(0.0f, -1.0f, 0.0f);
            }
        } else {
            if (IS_THING_ALIVE(PThingThatHit)) {
                C3DVector line_to_target = point_of_hit - PCreature->GetPos();
                line_to_target.Z = 0.0f;
                if (line_to_target.IsNonZero()) {
                    line_to_target.AccurateNormalise();
                    new_facing_dir = line_to_target;
                } else
                    new_facing_dir = C3DVector(0.0f, 1.0f, 0.0f);
            }
            C3DVector facing_vector = PCreature->GetPhysicsTC()->GetFacingVector();
            C3DVector kinda_up = facing_vector.Cross(hit_dir_2d);
            SetCombatAnimation(kinda_up.Z > 0.0f
                                             ? "RESPONSE_GENERIC_HIT_LEFT_KNOCKDOWN"
                                             : "RESPONSE_GENERIC_HIT_RIGHT_KNOCKDOWN");
        }
    }
    CCombatActionBase::Initialise();
    CTCCreatureModeManager* ptc_mode_manager;
    PCreature->GetTC(TCI_ENTITY_MODE_MANAGER,&ptc_mode_manager);
    if (ptc_mode_manager->GetCurrentMode().GetType() != 0x1d)
        PCreature->GetPhysicsTC()->ResetRHSet(CRightHandedSet(C3DVector(0,0,1),new_facing_dir));
    const CAnimComponentFollowingOnAnimFilter* pfilt_comp;
    PAnimEntry->GetComponent(&pfilt_comp);
    CCharString val(pfilt_comp->GetNextAnimFilter());
    ptc_mode_manager->AddMode(CCountedPointer<CTCCreatureModeManager::CModeBase>(new NCreatureMode::CModeKnockedDown(*PCreature,val)));
    ptc_mode_manager->RemoveMode(0x42);
    ptc_mode_manager->RemoveMode(0xd);
    ptc_mode_manager->RemoveMode(0x36);
}