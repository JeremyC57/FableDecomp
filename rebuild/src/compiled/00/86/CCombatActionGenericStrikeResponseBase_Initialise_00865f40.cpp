// 00865f40 CCombatAction_GenericStrikeResponseBase::Initialise
// Flags: none
#define FABLE_RETAIL_C3DVECTOR_CROSS_XZ_GROUP
#define FABLE_RETAIL_C3DVECTOR_NORMALISE_LOCALS_XYZ
// Leaf: ?Initialise@CCombatAction_GenericStrikeResponseBase@@UAEXXZ
#define FABLE_RETAIL_GENERIC_STRIKE_RESPONSE_PASS29
#define FABLE_RETAIL_CCOMBATACTION_GENERICSTRIKERESPONSEBASE_TYPED
#define FABLE_RETAIL_C3DVECTOR_ANGLE_BODIES
#define FABLE_RETAIL_CTHING_ISDEAD_DIRECT
#define FABLE_RETAIL_CTHING_ORIGINAL_VTABLE
#define FABLE_RETAIL_CTHINGCREATUREBASE_LASTHANDEDNESS
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

#include "engine/CTCCreatureItemLevitationBase.h"
#include "engine/CCombatAction_GenericStrikeResponseBase.h"
namespace NCreatureMode { bool FABLE_FASTCALL IsFaceHitterOnHit(const CThing&); }
inline float FABLE_FASTCALL GFDotProduct(const C3DVector& a,const C3DVector& b) { return a.X*b.X+a.Y*b.Y+a.Z*b.Z; }
void CCombatAction_GenericStrikeResponseBase::Initialise()
{
    PCreature->IsPlayerCharacter();
    CTCHeroRangedAttacks* ptc_ranged_attacks;
    if (PCreature->GetTC(&ptc_ranged_attacks)) {
        ptc_ranged_attacks->CancelAimingWeapon();
        if (NGlobalConsole::GCheatModeEnableRemoveProjectileModeWhenHit)
            ptc_ranged_attacks->ResetProjectileWeaponMode();
        PCreature->GetPlayer().RemoveMode(PLAYER_MODE_USE_PROJECTILE_WEAPON_THIRD_PERSON);
    }
    C3DVector facing = PCreature->GetPhysicsTC()->GetFacingVector();
    C3DVector to_hit_point = PointOfHit - PCreature->GetPos();
    if (IS_THING_ALIVE(PThingThatHit))
        to_hit_point = PThingThatHit->GetPos() - PCreature->GetPos();
    to_hit_point.Normalise();
    C3DVector travel_direction(0,0,0);
    float dot = GFDotProduct(facing,to_hit_point);
    float min_angle = GFCos(100.0f/360.0f);
    if (dot < min_angle) {
        travel_direction = -to_hit_point;
        SetCombatAnimation("RESPONSE_GENERIC_HIT_BACK");
    } else {
        travel_direction = to_hit_point;
        bool anim_setup = false;
        if (IS_THING_ALIVE(PThingThatHit) && (PThingThatHit->GetTypeFlags() & 8)) {
            const CThingCreatureBase* pcreature = static_cast<const CThingCreatureBase*>(PThingThatHit.Get());
            if (pcreature->GetLastActionHandedness() == HANDED_RIGHT) {
                SetCombatAnimation("RESPONSE_GENERIC_HIT_LEFT");
                anim_setup = true;
            } else if (pcreature->GetLastActionHandedness() == HANDED_LEFT) {
                SetCombatAnimation("RESPONSE_GENERIC_HIT_RIGHT");
                anim_setup = true;
            }
        }
        if (!anim_setup) {
            C3DVector cross_result = facing ^ to_hit_point;
            if (cross_result.Z < 0.0f)
                SetCombatAnimation("RESPONSE_GENERIC_HIT_LEFT");
            else
                SetCombatAnimation("RESPONSE_GENERIC_HIT_RIGHT");
        }
    }
    CTCCreatureModeManager* ptc_cmm;
    if (PCreature->GetTC(&ptc_cmm)) {
        ptc_cmm->RemoveMode(0xd);
        if (NCreatureMode::IsFaceHitterOnHit(*PCreature))
            PCreature->GetPhysicsTC()->SetFacingAngleXY(travel_direction.GetAngleXY());
    }
    CTCCreatureItemLevitationBase* ptc_levitation;
    if (PCreature->GetTC(&ptc_levitation))
        ptc_levitation->DropItems();
    CCombatActionBase::Initialise();
    DustTravelDirection = travel_direction;
    FrameForDust = TotalTime / 2;
    if (ptc_cmm && ptc_cmm->IsModeActive(0x36))
        ptc_cmm->RemoveMode(0x36);
}