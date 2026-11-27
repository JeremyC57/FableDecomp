// 00867200 CCombatAction_GenericResponseToExplosion::Initialise
// Flags: none
#define FABLE_RETAIL_CANIMFILTERBASE_SIZE_PASS27
#define FABLE_RETAIL_CANIMATIONENTRY_GETCOMPONENT
#define FABLE_RETAIL_CANIMATIONENTRY_GETCOMPONENT_BODY
#define FABLE_RETAIL_EXPL_RESPONSE_PASS29
#define FABLE_RETAIL_CCOMBATACTIONGENERICRESPONSETOEXPLOSION
#define FABLE_RETAIL_CMODEBASE_ORIGINAL_GETTYPE_PASS26
#define FABLE_RETAIL_CCHARSTRING_GETCRC_INLINE
#define FABLE_RETAIL_CANIMFILTEROFGROUP_INLINE_CTOR
#define FABLE_RETAIL_CTCCREATUREMODEMANAGER_ADDMODEPOINTER
#define FABLE_RETAIL_CRIGHTHANDEDSET_CTOR_INLINE
#define FABLE_RETAIL_CCLASS_GETTYPE
#define FABLE_RETAIL_ANIMCOMPONENT
// Leaf: ?Initialise@CCombatAction_GenericResponseToExplosion@@UAEXXZ
#define FABLE_RETAIL_C3DVECTOR_ANGLE_BODIES
#define FABLE_RETAIL_CTHING_ISDEAD_DIRECT
#define FABLE_RETAIL_CTHING_ORIGINAL_VTABLE
#define FABLE_RETAIL_C3DVECTOR_ISNONZERO_ORIGINAL_YXZ
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
#define FABLE_RETAIL_CTHING_GETTC_DONOR_NAMES_PASS23
#define FABLE_RETAIL_CTHING_STATEACCESS
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CMODEBASE_VIRTUALS
#define FABLE_RETAIL_CTCCREATUREMODEMANAGER
#define FABLE_RETAIL_CCOMBATACTIONBASE_INITIALISE
#define FABLE_RETAIL_C3DVECTOR_PACKED
#define FABLE_RETAIL_C3DVECTOR_NORMALISE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCPHYSICSBASE_LASTPOSITION
#define FABLE_RETAIL_FIRE_RANGED_WEAPON
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_PLAYER_REMOVEMODE
#define FABLE_RETAIL_CWORLDMAP
#define FABLE_RETAIL_CANIMATIONENTRY
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_CMODEBASE_CTOR
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
#include "engine/ECombatAnimationHandedness.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CCreatureActionBase.h"
#include "engine/CSoundPair.h"
#include "engine/CCombatActionBase.h"
#include "engine/ThingAlive.h"
#include "engine/CTCCreatureModeManager.h"
#include "engine/NCreatureMode.h"
#include "engine/CTCHeroRangedAttacks.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/EPlayerMode.h"
#include "engine/CPlayer.h"
#include "engine/CRightHandedSet.h"
#include "engine/CAnimationEntry.h"
#include "engine/GFCos.h"
// FableWin PUB32/GDATA at 04ABD0BA, Ego_d PUB32 and the retail load at 013B86EF.
namespace NGlobalConsole { extern bool GCheatModeEnableRemoveProjectileModeWhenHit; }

#include "engine/CAnimComponentFollowingOnAnimFilter.h"
#include "engine/NCreatureMode_CModeExplosiveForcePushed.h"
#include "engine/CCombatAction_GenericResponseToExplosion.h"
#include <math.h>
float FABLE_FASTCALL GFGetSignedAngleDifference(float,float);
void CCombatAction_GenericResponseToExplosion::Initialise()
{
    PCreature->SetBodyReorienterActive(true);
    CTCHeroRangedAttacks* ptc_ranged_attacks;
    if (PCreature->GetTC(&ptc_ranged_attacks)) {
        ptc_ranged_attacks->CancelAimingWeapon();
        if (NGlobalConsole::GCheatModeEnableRemoveProjectileModeWhenHit)
            ptc_ranged_attacks->ResetProjectileWeaponMode();
        PCreature->GetPlayer().RemoveMode(PLAYER_MODE_USE_PROJECTILE_WEAPON_THIRD_PERSON);
    }
    HitDir.Z=0;
    if (HitDir.IsNonZero()) HitDir.AccurateNormalise();
    else {
        HitDir=PCreature->GetPhysicsTC()->GetFacingVector();
        HitDir.Z=0;
        if (HitDir.IsNonZero()) HitDir.AccurateNormalise();
        else HitDir=C3DVector(1,0,0);
    }
    C3DVector toExplosion=HitDir * -1.0f;
    float hitAng=GFGetSignedAngleDifference(PCreature->GetPhysicsTC()->GetFacingAngleXY(),toExplosion.GetAngleXY());
    C3DVector newFacingDir;
    if (fabs(hitAng)>0.25f) {
        SetCombatAnimation("RESPONSE_GENERIC_HIT_BACK_KNOCKDOWN");
        if (PCreature->GetType()==1) FlipFacing=true;
        newFacingDir=HitDir;
    } else {
        if (hitAng<0) SetCombatAnimation("RESPONSE_GENERIC_HIT_LEFT_KNOCKDOWN");
        else SetCombatAnimation("RESPONSE_GENERIC_HIT_RIGHT_KNOCKDOWN");
        newFacingDir=toExplosion;
    }
    CCombatActionBase::Initialise();
    CTCCreatureModeManager* ptc_mode_manager;
    PCreature->GetTC(&ptc_mode_manager);
    if (ptc_mode_manager->GetCurrentMode().GetType()!=NCreatureMode::MODE_STUNNED)
        PCreature->GetPhysicsTC()->ResetRHSet(CRightHandedSet(C3DVector(0,0,1),newFacingDir));
    CAnimComponentFollowingOnAnimFilter* pfilt_comp;
    PAnimEntry->GetComponent(&pfilt_comp);
    CCharString val(pfilt_comp->GetNextAnimFilter());
    ptc_mode_manager->AddMode(new NCreatureMode::CModeExplosiveForcePushed(*PCreature,*PExplosion,val));
    ptc_mode_manager->RemoveMode(NCreatureMode::MODE_SLEEP_BED);
}