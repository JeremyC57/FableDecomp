// 007aac50 CTCAssassinRush::RushForwards
// Leaf: ?RushForwards@CTCAssassinRush@@AAEXXZ
// Flags: none
#define FABLE_RETAIL_C3DVECTOR_NORMALISE_LOCALS_XYZ
#define FABLE_RETAIL_CBULLETTIMEMANAGER
#define FABLE_RETAIL_CENVIRONMENT_SPELLEFFECTINTERPOLATOR
#define FABLE_RETAIL_CENVIRONMENTSPELLEFFECTINTERPOLATOR_BULLETTIME_PASS23
#define FABLE_RETAIL_CWORLD_ENVIRONMENT
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTHINGAICREATURE_DRAINED
#define FABLE_RETAIL_CTCSOUNDPLAYER
#define FABLE_RETAIL_CBULLETTIMEMANAGER_SETSLOWDOWN
#define FABLE_RETAIL_CTHINGCREATUREBASE_FINISHACTION
#define FABLE_RETAIL_CTCTARGETINGPLAYER_UPDATE_LIST
#define FABLE_RETAIL_C3DVECTOR_PACKED
#define FABLE_RETAIL_C3DVECTOR_ORIGINAL_COMPONENT_CTOR
#define FABLE_RETAIL_C3DVECTOR_ORIGINAL_COPY_FORMAL
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CTHINGCREATUREBASE_HEIGHTZBELOW
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_GFDEBUGDYNAMICCAST
#define FABLE_RETAIL_C2DCOORDI
#define FABLE_RETAIL_STLP_FIND_RANDOM_DISPATCH
#define FABLE_RETAIL_CANIMFILTERBASE_SIZE_PASS27
#define FABLE_RETAIL_CCOMBATACTIONGENERICRESPONSETOEXPLOSION
#define FABLE_RETAIL_CMODEBASE_ORIGINAL_GETTYPE_PASS26
#define FABLE_RETAIL_CCHARSTRING_GETCRC_INLINE
#define FABLE_RETAIL_CANIMFILTEROFGROUP_INLINE_CTOR
#define FABLE_RETAIL_CTCCREATUREMODEMANAGER_ADDMODEPOINTER
#define FABLE_RETAIL_CRIGHTHANDEDSET_CTOR_INLINE
#define FABLE_RETAIL_CCLASS_GETTYPE
#define FABLE_RETAIL_ANIMCOMPONENT
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

#include <math.h>

#include "engine/C2DCoordI.h"
#include "engine/CWorldMap.h"
#include "engine/CWorld.h"
#include "engine/GFSqr.h"
#include "stlport/fable_algorithm.h"
#include "engine/CSpecialAbilitiesAssassinRushDef.h"
#include "engine/CTCAssassinRush.h"
#include "engine/CThingAICreature.h"
#include "engine/CBulletTimeManager.h"
#include "engine/CEnvironment.h"
#include "engine/CEnvironmentSpellEffectInterpolator.h"
#include "engine/CTCSoundPlayer.h"
#include "engine/CMainGameComponent.h"
inline void CTCAssassinRush::FinishMove(CThingCreatureBase& parent_creature)
{
    parent_creature.FinishCurrentAction();
    if(FinalAction.Get())parent_creature.SetCurrentAction(*FinalAction);
    StartTrailDelay=true;
    CleanUpEffects();
}
#include "engine/GFCos.h"
const float MAX_ALLOWED_Z_CHANGE_FOR_RUSH_MOVE = 25.0f;

void CTCAssassinRush::SetupFacingDirection(C3DVector& direction)
{
    direction.Normalise();
    if(direction.Dot(C3DVector(0,0,1))>GFCos(60.f/360.f))direction=GetParentThing().GetPhysicsTC()->GetFacingVector();
}
void CTCAssassinRush::RushForwards()
{
    CThingCreatureBase& parent_creature=*GFDebugDynamicCast(&GetParentThing(),(CThingCreatureBase*)0);
    CTCPhysicsBase& parent_physics=*GetParentThing().GetPhysicsTC();
    C3DVector direction_to_move;
    if(DirectionOverride && !IsZTargetting) direction_to_move=DirectionOverrideVector;
    else if(IS_THING_ALIVE(PTarget)) direction_to_move=PTarget->GetPos()-parent_creature.GetPos();
    else direction_to_move=parent_creature.GetPhysicsTC()->GetFacingVector();
    SetupFacingDirection(direction_to_move);
    parent_physics.SetFacingAnglesFromRHSet(CRightHandedSet(C3DVector(0,0,1),direction_to_move));
    C3DVector movement_vector=direction_to_move*PDef->RunSpeed;
    float distance_from_start_point_sq=(parent_creature.GetPos()-PositionStartedFrom).GetSquaredMagnitude();
    bool dest_reached=false;
    if(dest_reached || distance_from_start_point_sq<GFSqr(DashDistance)) {
        if(StartedMoving){
            if(dest_reached)CollidedThing=PTarget;
            if(IS_THING_ALIVE(CollidedThing)&&UseCollision&&(CollidedThing->GetTypeFlags()&8)){
                if(CollidedThing->GetType()==1){
                    CThingAICreature* ai_creature=GFDebugPDynamicCast(CollidedThing.Get(),(CThingAICreature*)0);
                    ai_creature->ForceAIReset();
                }
                BehindTargetPos=GetValidTeleportPos(&parent_physics);
                parent_physics.SetPosition(BehindTargetPos);
                parent_physics.SetFacingAnglesFromRHSet(CRightHandedSet(C3DVector(0,0,1),CollidedThing->GetPos()-BehindTargetPos));
                if(parent_creature.GetType()==2){
                    SlowTimeLength=(long)(CMainGameComponent::GetConstantFPS()*PDef->SlowTimeLength);
                    GetWorld().GetBulletTimeManager().SetBulletTimeSlowdownMultiplier(PDef->SlowTimeMultiplier,this,0.5f);
                }
                FinishMove(parent_creature);
                return;
            }
            C3DVector lastpos=parent_physics.GetLastPosition();
            C3DVector expected_position=lastpos+movement_vector;
            expected_position.Z=parent_creature.GetHeightZBelowAt(expected_position,false,true);
            C3DVector thingpos=parent_creature.GetPos();
            C3DVector difference=expected_position-thingpos;
            if(FakeCollideWithSomething || (expected_position-parent_creature.GetPos()).GetSquaredMagnitude2D()>GFSqr(PDef->RunSpeed*0.3f) || !GetWorld().PeekWorldMap().IsPosPassable(C2DCoordI((long)expected_position.X,(long)expected_position.Y))){
                CTCSoundPlayer* ptc_sound_player;
                if(parent_creature.GetTC(&ptc_sound_player))ptc_sound_player->PlayAttachedCriteria("HERO_ABILITY_ASSASSIN_RUSH_COLLISION",true,false,false,-1);
                FinishMove(parent_creature);
                return;
            }
        }
        C3DVector target_pos=parent_creature.GetPos()+movement_vector;
        float height_z=parent_creature.GetHeightZBelowAt(target_pos,true,true);
        if(height_z<target_pos.Z){
            float diff=target_pos.Z-height_z;
            if(diff<MAX_ALLOWED_Z_CHANGE_FOR_RUSH_MOVE)target_pos.Z=height_z;
        }
        if(!UseCollision && height_z>target_pos.Z)target_pos.Z=height_z;
        parent_physics.SetForcedToMovePos(target_pos,UseCollision);
        C3DVector pos=parent_creature.GetPos();
        C3DVector dir=target_pos-pos;
        target_pos+=dir*100.f;
        CEnvironmentSpellEffectInterpolator& interp=GetWorld().GetEnvironment().GetSpellEffectInterpolator();
        interp.RadialBlurSetCenterWorldPos(RadialBlurFadeKey,C3DVector(pos.X,pos.Y,pos.Z+PDef->DistFromFloor));
        StartedMoving=true;
    } else FinishMove(parent_creature);
}