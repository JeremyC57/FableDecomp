// 007a8700 CTCBattleCharge::RushForwards
// Leaf: ?RushForwards@CTCBattleCharge@@AAEXXZ
// Flags: none
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY_ORIGINAL_CHAIN
#define FABLE_RETAIL_STLP_PASS22_FLARE_ABI
#define FABLE_RETAIL_C3DVECTOR_NORMALISE_LOCALS_XYZ
#define FABLE_RETAIL_CTCTARGETINGPLAYER_UPDATE_LIST
#define FABLE_RETAIL_C3DVECTOR_PACKED
#define FABLE_RETAIL_C3DVECTOR_ORIGINAL_COMPONENT_CTOR
#define FABLE_RETAIL_C3DVECTOR_ORIGINAL_COPY_FORMAL
#define FABLE_RETAIL_BATTLE_CHARGE_RUSH_PASS29
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CTCBATTLECHARGE_STARTUP_PASS24
#define FABLE_RETAIL_CTCBATTLECHARGE_CREATEHIT_PASS27
#define FABLE_RETAIL_CSPECIALABILITIESBATTLECHARGEDEF_STARTUP_PASS24
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

#include "engine/CTCBattleCharge.h"
#include "engine/_quarantine/CSpecialAbilitiesBattleChargeDef.h"
#include "engine/C2DCoordI.h"
#include "engine/CWorldMap.h"
#include "engine/CWorld.h"
#include "engine/GFSqr.h"
#include "stlport/fable_algorithm.h"
#include "engine/GFCos.h"
const float MAX_ALLOWED_Z_CHANGE_FOR_RUSH_MOVE = 25.0f;

void CTCBattleCharge::SetupFacingDirection(C3DVector& direction)
{
    direction.Normalise();
    if(direction.Dot(C3DVector(0,0,1))>GFCos(60.f/360.f))direction=GetParentThing().GetPhysicsTC()->GetFacingVector();
}
void CTCBattleCharge::RushForwards()
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
    float distance_travelled_sq=DisplacementVector.GetSquaredMagnitude();
    float sqrdashdistance=GFSqr(DashDistance);
    if(distance_from_start_point_sq<sqrdashdistance && distance_travelled_sq<sqrdashdistance) {
        if(StartedMoving) {
            if(CollidedThing!=0 && (CollidedThing->GetTypeFlags()&8)) {
                if(std::find(PeopleHit.begin(),PeopleHit.end(),CollidedThing)==PeopleHit.end()){
                    CreateHitEffect();
                    PeopleHit.push_back(CollidedThing);
                    if(PeopleHit.size()>=PDef->NumPeopleHit[SpellLevel]) {
                        FinishMove(parent_creature,true);
                        return;
                    }
                }
                C3DVector target_pos=parent_creature.GetPos()+movement_vector;
                float height_z=parent_creature.GetHeightZBelowAt(target_pos,true,true);
                if(height_z<target_pos.Z)target_pos.Z=height_z;
                parent_physics.SetForcedToMovePos(target_pos,true);
                CollidedThing=(CThing*)0;
                DisplacementVector+=target_pos-parent_creature.GetPos();
                return;
            }
            C3DVector lastpos=parent_physics.GetLastPosition();
            C3DVector expected_position=lastpos+movement_vector;
            expected_position.Z=parent_creature.GetHeightZBelowAt(expected_position,false,true);
            C3DVector thingpos=parent_creature.GetPos();
            C3DVector difference=expected_position-thingpos;
            if(FakeCollideWithSomething || difference.GetSquaredMagnitude2D()>GFSqr(PDef->RunSpeed*0.3f) || !GetWorld().PeekWorldMap().IsPosPassable(C2DCoordI((long)expected_position.X,(long)expected_position.Y))) {
                CreateHitEffect();FinishMove(parent_creature,true);return;
            }
        }
        C3DVector target_pos=parent_creature.GetPos()+movement_vector;
        float height_z=parent_creature.GetHeightZBelowAt(target_pos,true,true);
        if(height_z<target_pos.Z) {
            float diff=target_pos.Z-height_z;
            if(diff<MAX_ALLOWED_Z_CHANGE_FOR_RUSH_MOVE)target_pos.Z=height_z;
        }
        parent_physics.SetForcedToMovePos(target_pos,true);
        DisplacementVector+=target_pos-parent_creature.GetPos();
        C3DVector pos=parent_creature.GetPos();
        C3DVector dir=target_pos-pos;
        target_pos+=dir*100.f;
        StartedMoving=true;
    } else {
        bool still_hitting_some_creature=(CollidedThing!=0 && (CollidedThing->GetTypeFlags()&8));
        FinishMove(parent_creature,still_hitting_some_creature);
    }
}