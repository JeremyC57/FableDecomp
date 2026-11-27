#define FABLE_RETAIL_CTCPHYSICSBASE_ORIGINAL_CTOR_PASS23
#define FABLE_RETAIL_C3DVECTOR_ISNONZERO_ORIGINAL_YXZ
#define FABLE_RETAIL_IS_THING_DEAD_DECL_ONLY
// Leaf: ?AIAction_MoveToThingProximity@CAIStateGroupBase@@IAE?AW4EAIActionReturn@@W4EAIActionRepeat@@V?$CIntelligentPointer@$$CBVCThing@@@@MAA_NW4EMovementType@@_N444M@Z
#define FABLE_RETAIL_CTHING_ISDEAD_DIRECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING_GETTC_DONOR_NAMES_PASS23
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_DEREFERENCE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CINTELLIGENTPOINTER_GET_ORIGINAL_CHAIN
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE_AIACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEALACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_MOVETOPOSPROXIMITY
#define FABLE_RETAIL_CAISTATEGROUPBASE_PEEK
#define FABLE_RETAIL_CAISTATEGROUPBASE_PEEK_PROTECTED
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_PACKED
#define FABLE_RETAIL_C3DVECTOR_MAGNITUDE2D
#define FABLE_RETAIL_MOVETOTHINGPROXIMITY_PASS29
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DVECTOR_FROM3D
#define FABLE_RETAIL_C2DVECTOR_GFGET2DDISTANCESQUARED
#define FABLE_RETAIL_C3DVECTOR_SQUAREDMAGNITUDE_XYZ
#define FABLE_RETAIL_CTCCREATURENAVIGATION
#define FABLE_RETAIL_CTCCREATURENAVIGATION_NAVIGABLE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE_FULL_MEMBERS
#define FABLE_RETAIL_CTHINGCREATUREBASE_ANIMATION
#define FABLE_RETAIL_CTHINGCREATUREBASE_ACTIONQUERY
#define FABLE_RETAIL_CAIBRAIN_SUSPENDABLE_BASE
#define FABLE_RETAIL_CAIBRAIN_SETOUTPUT
#define FABLE_RETAIL_CBRAINOUTPUT_ONLY
#define FABLE_RETAIL_CBRAINOUTPUT_ACTION_CALLED_PASS24
#define FABLE_RETAIL_CTCPHYSICSBASE_RADIUS_ACCESS
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_C3DVECTOR_ISNONZERO
#define FABLE_RETAIL_C3DVECTOR_NOT_EQUAL
#define FABLE_RETAIL_CLOOKDEF
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_STLP_BITSET_INDEX
#define FABLE_RETAIL_CAIBRAIN_CREATURE_REFERENCE
#define FABLE_RETAIL_CAIBRAIN_CTOR_PASS23
#define FABLE_RETAIL_SENSED_PASS23
#define FABLE_RETAIL_KID_EVADE_LIFETIMES_PASS23
#define FABLE_RETAIL_CTHING_ORIGINAL_VTABLE
#define FABLE_RETAIL_CTHINGAICREATURE_STATTYPEFLAGS
#define private public
#define protected public
#include "fable_string.h"
#include "engine/CThingAICreature.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/ThingAlive.h"
#include "engine/EAIAction.h"
#include "engine/EMovementType.h"
#include "engine/ENavigatorReturn.h"
#include "engine/CTCCreatureNavigation.h"
#include "engine/CCreatureActionBase.h"
#include "engine/campfire_types.h"
#include "engine/CTCLook.h"
#include "engine/C2DVector.h"
#include "engine/GFSqr.h"
#undef private
#undef protected
#include <stdio.h>
#include <stdlib.h>
#include "engine/CWorld.h"
inline ETCInterfaceType FABLE_FASTCALL CTCLook::StatGetInterfaceType() { return TCI_LOOK; }
inline CNavigationInput::CNavigationInput(const C3DVector& destination,float proximity,EMovementType movement) : Destination(destination),Proximity(proximity),MovementType(movement),Speed(0.0f) {}
inline void CBrainOutput::SetAsMoveToPos(const C3DVector& pos,const CNavigationInput& input) { Types |= 1; MoveToPos = pos; NavigationInput = input; }
inline void CBrainOutput::SetAsFaceMovementDirection() { Types |= 0x10; LookFlags = 0x51; }
namespace { CThing* FABLE_FASTCALL GetPBestTargetToFaceForCreature(const CThingCreatureBase&); }
template<class T> inline bool FABLE_FASTCALL IS_THING_DEAD(const T& thing){if(thing!=0) { if(!thing->IsDead()) return false; } return true;}
EAIActionReturn CAIStateGroupBase::AIAction_MoveToThingProximity(
    EAIActionRepeat type, CIntelligentPointer<const CThing> target_thing,
    float proximity, bool& finished_calculating_route, EMovementType movement_type,
    bool face_movement_direction, bool want_line_of_sight, bool avoid_dynamic_obstacles,
    bool reduce_speed_when_near, float proximity_for_direct_approach)
{
    const float MIN_DISTANCE_OF_OLD_AND_NEW_TARGET_POS_TO_RESTART_NAV = 8.5f;
    const float MIN_DISTANCE_FROM_TARGET_TO_NOT_RESTART_NAV = 3.5f;
    SetThingToNotAvoid(target_thing.Get());
    SetProximityForDirectApproach(proximity_for_direct_approach);
    if (IsStopping())
        return AI_ACTION_TERMINATE_PROCESS;
    EAIActionReturn ret = AI_ACTION_IN_PROGRESS;
    if (IS_THING_DEAD(target_thing))
        return AI_ACTION_FAILED;
    bool look_at_interesting_things = true;
    do {
        const CThing* plookat_target = GetPBestTargetToFaceForCreature(GetCreature());
        CTCLook* ptc_look;
        if (GetCreature().GetTC(&ptc_look))
            ptc_look->TrackNearestInterestingThing(look_at_interesting_things);
        if (IS_THING_ALIVE(target_thing)) {
            C3DVector target_pos = target_thing->GetPos();
            CTCCreatureNavigation* pnav;
            GetCreature().GetTC(&pnav);
            if (pnav->IsNewDestinationGoingToResetNavigation(target_pos, proximity)) {
                C3DVector old_target_pos = pnav->GetCurrentDestination();
                if ((GetCreature().GetPos() - old_target_pos).GetSquaredMagnitude() >
                        GFSqr(GetCreature().GetPhysicsTC()->GetRadius() + proximity) &&
                    GFGet2DDistanceSquared(old_target_pos, target_pos) <
                        GFSqr(MIN_DISTANCE_OF_OLD_AND_NEW_TARGET_POS_TO_RESTART_NAV) &&
                    GFGet2DDistanceSquared(GetCreature().GetPos(), target_pos) >
                        GFSqr(MIN_DISTANCE_FROM_TARGET_TO_NOT_RESTART_NAV))
                    target_pos = old_target_pos;
            }
            CNavigationInput nav_input(target_pos, proximity, movement_type);
            CTCCreatureNavigation::CCreatureNavOutput nav_out = pnav->GetNextNavPosOnRoute(
                nav_input, want_line_of_sight, avoid_dynamic_obstacles, reduce_speed_when_near, true, false);
            finished_calculating_route = nav_out.NavResult == NAV_REACHED_TARGET || nav_out.MovementVector.IsNonZero();
            if (!GetCreature().IsPerformingAnyAction() || !GetCreature().PeekPCurrentAction()->IsPreventingNormalMovement()) {
                switch (nav_out.NavResult) {
                case NAV_MOVING_TO_TARGET: {
                    CBrainOutput output;
                    C3DVector move_to_pos = GetCreature().PeekPos() + nav_out.MovementVector;
                    output.SetAsMoveToPos(move_to_pos, nav_input);
                    if (IS_THING_ALIVE(plookat_target))
                        output.SetAsLookAtThing(*plookat_target, 0x97);
                    else if (face_movement_direction) {
                        output.SetAsFaceMovementDirection();
                        look_at_interesting_things = false;
                    }
                    PBrain->SetOutput(output);
                }
                    ret = AI_ACTION_IN_PROGRESS;
                    break;
                case NAV_REACHED_TARGET:
                    ret = AI_ACTION_FINISHED;
                    break;
                case NAV_CANNOT_REACH_TARGET:
                    ret = AI_ACTION_FAILED;
                    break;
                }
            }
        } else {
            ret = AI_ACTION_FAILED;
        }
        PBrain->SuspendSuspendableProcess();
        if (IsStopping())
            return AI_ACTION_TERMINATE_PROCESS;
    } while (type == ACTION_REPEAT_UNTIL_FINISHED && ret == AI_ACTION_IN_PROGRESS);
    return ret;
}
CBaseClass::CBaseClass() {}
CBaseClass::~CBaseClass() {}
CBaseObject::CBaseObject() : IntelligentPointer(0) {}
CBaseObject::~CBaseObject() {}
void CBaseObject::Validate() const {}
void CClass::Save(CCharString&) {}
CCharString::CCharString() : PStringData(0) {}
CCharString::~CCharString() {}
CBaseClassNonCopyable::CBaseClassNonCopyable() {}
CBaseClassNonCopyable::~CBaseClassNonCopyable() {}
CTCBase::CTCBase(CThing& thing) : ParentThing(&thing) {}
CTCBase::~CTCBase() {}
void CTCBase::OnSerialise(CPersistContext&, bool) {}
void CTCBase::OnRemove() {}
void CTCBase::ConstructFromParams(const CTCParamsBase&) {}
void CTCBase::OnCreate() {}
void CTCBase::InitialActivate() {}
void CTCBase::OnInitialActivate() {}
void CTCBase::RenderUpdate(float) {}
bool CTCBase::HasFrameUpdate() const { return false; }
void CTCBase::FrameUpdate() {}
void CTCBase::BulletTimeFrameUpdate() {}
void CTCBase::DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo&) {}
void CTCBase::Copy(const CTCBase&) {}
void CTCBase::OnDie() {}
void CTCBase::OnKill() {}
void CTCBase::OnLevelUnload() {}
void CTCBase::OnSetInLimbo(bool) {}
bool CTCBase::Validate(CCharString&) const { return true; }
bool CTCBase::IsFreeForDeletion() const { return true; }
bool CTCBase::IsSerialiseCreatable() const { return false; }
bool CTCBase::IsSerialisableForHeroState() const { return false; }
bool CTCBase::IsComponentSerialisable() const { return false; }
bool CTCBase::IsDriver() const { return false; }
bool CTCBase::IsCopyable() const { return false; }
void CTCBase::GlobalUpdate(bool) {}
void CTCBase::HandleMessage(const NTCMessages::CMessageBase&) {}

#include <stdlib.h>
#include "engine/CSoundPair.h"
#include "engine/CAnimationEntry.h"

CBaseIntelligentPointer::CBaseIntelligentPointer() : PData(0) {}
CBaseIntelligentPointer::~CBaseIntelligentPointer() {}
CBaseObject* CBaseIntelligentPointer::GetPItem() const { return reinterpret_cast<CBaseObject*>(PData); }
void CBaseIntelligentPointer::SetPItem(CBaseObject* item) { PData = reinterpret_cast<CBaseObjectPointer*>(item); }


static void RequireAt(bool check, int line)
{
    if (!check) {
        printf("check failed on line %d\n", line);
        fflush(0);
        abort();
    }
}
#define Require(check) RequireAt((check), __LINE__)

void CIVCountedPointeeBase::Delete() const { abort(); }
CAIStateGroupBase::CAIStateGroupBase() {}
CAIBrain::CAIBrain(const CMainGameComponent&, CWorld&, CThingManager&, CPlayerManager&, CThingAICreature& actor,CDefPointer<const CBrainDef>) : CSuspendableProcessMicrothread(false,0,0),Creature(actor) {}
class Nav : public CTCCreatureNavigation {
public:
    Nav(CThing& actor): CTCCreatureNavigation(actor) {}
    ETCInterfaceType GetInterfaceType() const { return TCI_NAVIGATION; }
    CCharString GetClassTypeName() const { return CCharString(); }
};
CTCCreatureNavigation::CTCCreatureNavigation(CThing& actor):CTCBase(actor) {}
CTCCreatureNavigation::~CTCCreatureNavigation() {}
class Actor : public CThingAICreature {
public:
    bool IsControlled() const { abort(); }
    bool IsPlayerCharacter() const { abort(); }
    float GetMaxPushingSpeed(const C3DVector & p0) const { abort(); }
    float GetMaxWalkingSpeed(const C3DVector & p0) const { abort(); }
    float GetMaxJoggingSpeed(const C3DVector & p0) const { abort(); }
    float GetMaxRunningSpeed(const C3DVector & p0) const { abort(); }
    float GetMaxSprintingSpeed(const C3DVector & p0) const { abort(); }
    float GetMaxFlyingSpeed(const C3DVector & p0) const { abort(); }
    float GetMaxHoverSpeedSlow(const C3DVector & p0) const { abort(); }
    float GetMaxHoverSpeedMedium(const C3DVector & p0) const { abort(); }
    float GetMaxHoverSpeedFast(const C3DVector & p0) const { abort(); }
    float GetMaxAcceleration() const { abort(); }
    bool IsToUnsheatheItemsAfterCutscenes() const { abort(); }
};
void CAIStateGroupBase::Initialise(CAIBrain& brain, CThingAICreature& creature, long priority, EScriptAIPriority scriptPriority, const CCharString& name) { fprintf(stderr, "unexpected CAIStateGroupBase::Initialise\n"); abort(); }
void CAIStateGroupBase::OnActivate() { fprintf(stderr, "unexpected CAIStateGroupBase::OnActivate\n"); abort(); }
bool CAIStateGroupBase::InitCreaturePosition() { fprintf(stderr, "unexpected CAIStateGroupBase::InitCreaturePosition\n"); abort(); }
bool CAIStateGroupBase::AllowPriorityOverlap() { fprintf(stderr, "unexpected CAIStateGroupBase::AllowPriorityOverlap\n"); abort(); }
bool CAIStateGroupBase::IsStillValid() const { fprintf(stderr, "unexpected CAIStateGroupBase::IsStillValid\n"); abort(); }
const CCharString* CAIStateGroupBase::GetDebugText() const { fprintf(stderr, "unexpected CAIStateGroupBase::GetDebugText\n"); abort(); }
bool CAIStateGroupBase::IsTimingCritical() const { fprintf(stderr, "unexpected CAIStateGroupBase::IsTimingCritical\n"); abort(); }
EAIStateGroupType CAIStateGroupBase::GetStateGroupType() const { fprintf(stderr, "unexpected CAIStateGroupBase::GetStateGroupType\n"); abort(); }
void CAIStateGroupBase::Cleanup() { fprintf(stderr, "unexpected CAIStateGroupBase::Cleanup\n"); abort(); }
void CAIStateGroupBase::OnReplace() { fprintf(stderr, "unexpected CAIStateGroupBase::OnReplace\n"); abort(); }
bool CAIStateGroupBase::IsRunnable() { fprintf(stderr, "unexpected CAIStateGroupBase::IsRunnable\n"); abort(); }
void CAIStateGroupBase::InitStateGroup(const CWorld& world, CThingAICreature& creature) { fprintf(stderr, "unexpected CAIStateGroupBase::InitStateGroup\n"); abort(); }

#include "engine/CSoundPair.h"
CBaseObject::CBaseObject(const CBaseObject&) : IntelligentPointer(0) {}
bool CThingAICreature::FeelsGravity() const { abort(); }
CEngineGraphic const * CThingAICreature::GetCurrentGraphic() const { abort(); }
CSoundPair CThingCreatureBase::GetSoundPair(CCharString const &) const { abort(); }
bool CThingCreatureBase::IsWillable() const { abort(); }
bool CThingCreatureBase::IsNewAttackerGreaterThreat(CThingCreatureBase const &,CThingCreatureBase const &) const { abort(); }
bool CThingCreatureBase::AnimExists(CCharString const &,bool) const { abort(); }
bool CThingCreatureBase::SetActionForMelee(CCreatureAbilityDef const &) { abort(); }
unsigned long CThingCreatureBase::GetPhonemeAnim(CCharString const &) const { abort(); }
long CThingGameObject::GetMaterialOnHitByMelee() const { abort(); }
void CThingGameObject::ForceAnim(CCharString const &,bool) { abort(); }
void CThingGameObject::ForceAnim(long,bool) { abort(); }
bool CThingGameObject::IsIndestructible() const { abort(); }
bool CThingGameObject::IsGameObject() const { abort(); }
void CThingCreatureBase::OnHit(CHitParameters &) { abort(); }
bool CThingGameObject::IsCover() const { abort(); }
void CThingGameObject::SetTriggerRadius(float) { abort(); }
float CThingGameObject::GetTriggerRadius() const { abort(); }
bool CThingGameObject::IsUsingTriggerRadius() const { abort(); }
bool CThingGameObject::DrawIsTargeted() const { abort(); }
void CThingPhysical::ModifyHealth(float,bool) { abort(); }
CDefPointer<CMaterialDef const> CThingPhysical::PeekPMaterial() const { abort(); }
bool CThingPhysical::UseHighDetailQuadTree() const { abort(); }
bool CThingPhysical::CanBlockUpperLevelNavigatingCreature() const { abort(); }
bool CThingPhysical::CanBlockNavigatingCreature() const { abort(); }
bool CThingPhysical::IsFragile() const { abort(); }
void CThingPhysical::ClearTargetedByPlayer(long) { abort(); }
void CThingPhysical::SetTargetedByPlayer(long) { abort(); }
bool CThingPhysical::IsTargetedByPlayer(long) const { abort(); }
bool CThingPhysical::IsHidden() const { abort(); }
bool CThingPhysical::IsCrop() const { abort(); }
unsigned long CThingPhysical::GetProperties() const { abort(); }
bool CThingPhysical::IsAffectedByDamage() const { abort(); }
bool CThingPhysical::IsSolidToThing(CThingPhysical const *) const { abort(); }
void CThing::SetPositionUsingBrain() { abort(); }
bool CThing::IsFrameUpdated() const { abort(); }
void CThing::OnLevelUnload() { abort(); }
void CThing::OnNonKillingLevelUnload() { abort(); }
void CThing::OnCollision(ECollideType,C3DVector const &,C3DVector const &,CThing *) { abort(); }
long CThing::GetFloorMaterial() const { abort(); }
CThing const * CThing::GetThingStandingOn() const { abort(); }
CThing * CThing::GetThingStandingOn() { abort(); }
void CThing::SetThingStandingOn(CThing *) { abort(); }
bool CThingCreatureBase::DrawPreparePrimitivesForRendering(CEnginePrimitivePrepareInfo const &) { abort(); }
bool CThing::DrawGetPhysicsDebugInfo(CThingPhysicalDebugInfo &) const { abort(); }
bool CThing::DrawGetThingDisplayInformation(CThingDrawInformation *) const { abort(); }
bool CThing::DrawIsDisplayingInformation() const { abort(); }
bool CThing::DrawIsShowingFacingDirection() const { abort(); }
unsigned long CThing::DrawGetPhysicsMesh() const { abort(); }
bool CThing::DrawIsInDrawList() const { abort(); }
bool CThing::DrawIsEditDrawable() const { abort(); }
void CThing::SetAsDrawable(bool) { abort(); }
bool CThing::DrawIsDrawable() const { abort(); }
void CThing::DrawUpdate(CInterpolationInfo const *) { abort(); }
bool CThing::DrawGetEditDrawInfo(float,CEditEngineDrawInfo *) const { abort(); }
void CThing::GetInfo(CThingInfo *) const { abort(); }
unsigned long CThing::GetChecksum() const { abort(); }
bool CThing::IsEntitySerialisable() const { abort(); }
bool CThing::RenderUpdate(CInterpolationInfo const *) { abort(); }
void CThing::WorldUpdateBulletTime() { abort(); }
bool CThing::WorldUpdate() { abort(); }
bool CThing::ValidateThing(CCharString &) const { abort(); }
bool CThing::IsToBePlacedInMap() const { abort(); }
void CThing::ConstructFromBinarySave(CMemoryDataInputStream &,CThing::CSerialiseInfo const &) { abort(); }
void CThing::CreateFromDef(CThingBaseDef const &) { abort(); }
void CThing::OnSetInLimbo(bool) { abort(); }
bool CThing::PreOnKill() { abort(); }
void CThing::OnKill() { abort(); }
void CThing::OnDie() { abort(); }
void CThing::OnInitialActivate() { abort(); }
void CThing::InitialActivate() { abort(); }
bool CThing::OnCreate() { abort(); }
void CThing::CopyGameState(CThing const &) { abort(); }
bool CThing::ConstructFromGameStateScript(char const *) { abort(); }
bool CThing::ConstructAlreadyExistingThingFromLevelScript(char const *) { abort(); }
bool CThing::ConstructFromLevelScript(char const *) { abort(); }
bool CThing::SerialiseGameState(CPersistContext &) { abort(); }
bool CThing::DrawPreparePrimitivesForRendering(CEnginePrimitivePrepareInfo const &) { abort(); }
void CThingGameObject::OnHit(CHitParameters &) { abort(); }

static CThing* target;
static CThing* face;
static C3DVector current(0,0,0), destination(12,0,0), previous(10,0,0);
static unsigned stops,stop_at,outputs,ticks,queries,tracked;
static unsigned flags,result_id;
static bool reset_route;
static float expected_destination;
class Physics : public CTCPhysicsBase {
public:
    Physics(CThing& actor) : CTCPhysicsBase(actor) {}
    ETCInterfaceType GetInterfaceType() const { return TCI_PHYSICS; }
    CCharString GetClassTypeName() const { return CCharString(); }
};
CTCPhysicsBase::CTCPhysicsBase(CThing& t) : CTCBase(t) {}
CTCPhysicsBase::~CTCPhysicsBase() {}
const float DELTA_NOTIONAL_ZERO = 0.0001f;
CASuspendableProcess::CASuspendableProcess(bool,long,float) {}
CASuspendableProcess::~CASuspendableProcess() {}
void CASuspendableProcess::SuspendableProcess() {abort();}
CSuspendableProcessMicrothread::CSuspendableProcessMicrothread(bool a,long b,float c):CASuspendableProcess(a,b,c) {}
CSuspendableProcessMicrothread::~CSuspendableProcessMicrothread() {}
void CSuspendableProcessMicrothread::RunSuspendableProcess() {abort();}
bool CSuspendableProcessMicrothread::SuspendSuspendableProcess() {++ticks;return true;}
void CSuspendableProcessMicrothread::Restart() {abort();}
CAIBrain::~CAIBrain() {}
void CAIBrain::SuspendableProcess() {abort();}
bool CAIStateGroupBase::IsStopping() const {return ++stops==stop_at;}
void CAIStateGroupBase::SetThingToNotAvoid(const CThing* p) {Require(p==target);}
void CAIStateGroupBase::SetProximityForDirectApproach(float p) {Require(p==3.0f);}
namespace { CThing* FABLE_FASTCALL GetPBestTargetToFaceForCreature(const CThingCreatureBase&) {return face;} }
const C3DVector& CThing::GetPos() const {return this==target?destination:current;}
const C3DVector& CThingPhysical::PeekPos() const {return current;}
bool CThingCreatureBase::IsPerformingAnyAction() const {return false;}
bool CTCLook::TrackNearestInterestingThing(bool) {++tracked;return true;}
bool CTCCreatureNavigation::IsNewDestinationGoingToResetNavigation(const C3DVector& p,float r) const {Require(p.X==destination.X && r==1);return reset_route;}
const C3DVector& CTCCreatureNavigation::GetCurrentDestination() const {return previous;}
CTCCreatureNavigation::CCreatureNavOutput CTCCreatureNavigation::GetNextNavPosOnRoute(const CNavigationInput& in,bool a,bool b,bool c,bool d,bool e) {
  Require(in.Destination.X==expected_destination && in.Proximity==1 && in.MovementType==AI_MOVEMENT_RUNNING && in.Speed==0);
  Require(a==bool(flags&1) && b==bool(flags&2) && c==bool(flags&4) && d && !e);
  CCreatureNavOutput out;out.NavResult=ENavigatorReturn(queries++ ? 0:result_id);out.MovementVector=C3DVector((flags&8)?2.0f:0,0,0);return out;
}
CBrainOutput::CBrainOutput() : Types(0), LookFlags(0) {}
CNavigationInput::CNavigationInput() {}
void CBrainOutput::SetAsLookAtThing(const CThing& p,long f) {Require(&p==face && f==0x97); Types|=4;}
void CAIBrain::SetOutput(const CBrainOutput& v) {
 ++outputs;Require((v.Types&1) && v.MoveToPos.X==((flags&8)?2.0f:0));
 Require(bool(v.Types&4)==bool(face) && bool(v.Types&0x10)==(!face && bool(flags&16)));
}
class State : public CAIStateGroupBase { public: void Update() {abort();} };
int main() {
 Actor actor;CThing thing;CThing looked;Nav nav(actor);
 static unsigned __int64 backing[4][256];
 CAIBrain ai(*reinterpret_cast<CMainGameComponent*>(backing[0]),*reinterpret_cast<CWorld*>(backing[1]),*reinterpret_cast<CThingManager*>(backing[2]),*reinterpret_cast<CPlayerManager*>(backing[3]),actor,CDefPointer<const CBrainDef>());
 State state;state.PBrain=&ai; Physics physics(actor); actor.PhysicsTC=&physics; physics.Radius=1.0f; expected_destination=destination.X;
 memset(actor.UsingComponents._M_w,0,sizeof(actor.UsingComponents._M_w));
 actor.UsingComponents[TCI_NAVIGATION]=true;
 actor.Components.push_back(std::pair<ETCInterfaceType,CTCBase*>(TCI_NAVIGATION,&nav));
 for(flags=0;flags<64;++flags) for(result_id=0;result_id<3;++result_id)
 for(unsigned t=0;t<3;++t) for(unsigned policy=0;policy<4;++policy) {
   target=t?&thing:0;thing.Dead=t==2;looked.Dead=false;face=flags&32?&looked:0;
   stops=outputs=ticks=queries=tracked=0;stop_at=policy==0?1:policy==1?2:0;
   CIntelligentPointer<const CThing> ptr;ptr.SetPItem(target);bool finished=false;
   EAIActionReturn r=state.AIAction_MoveToThingProximity(policy==3?ACTION_REPEAT_UNTIL_FINISHED:ACTION_REPEAT_CALLBACK,ptr,1,finished,AI_MOVEMENT_RUNNING,!!(flags&16),!!(flags&1),!!(flags&2),!!(flags&4),3);
   unsigned n=!policy || t!=1 ?0:policy==3 && result_id==2?2:1;
   Require(queries==n && ticks==n && tracked==0 && outputs==(n && result_id==2));
   Require(finished==(n && (result_id==0 || n==2 || (flags&8))));
   Require(r==(!policy?AI_ACTION_TERMINATE_PROCESS:t!=1?AI_ACTION_FAILED:policy==1?AI_ACTION_TERMINATE_PROCESS:result_id==0||n==2?AI_ACTION_FINISHED:result_id==1?AI_ACTION_FAILED:AI_ACTION_IN_PROGRESS));
 }
 flags=0;result_id=2;target=&thing;thing.Dead=false;face=0;
 for(unsigned allow=0;allow<2;++allow) for(int old=-10;old<=12;++old) for(int to=-7;to<=17;++to) {
   reset_route=allow!=0;current=C3DVector(0,0,0);previous=C3DVector(float(old),0,0);destination=C3DVector(float(to),0,0);
   expected_destination=reset_route && old*old>4 && (old-to)*(old-to)<72.25f && to*to>12.25f ? float(old) : float(to);
   stops=stop_at=queries=ticks=outputs=0;
   bool completed=true; CIntelligentPointer<const CThing> ptr;ptr.SetPItem(target);
   Require(state.AIAction_MoveToThingProximity(ACTION_REPEAT_CALLBACK,ptr,1,completed,AI_MOVEMENT_RUNNING,false,false,false,false,3)==AI_ACTION_IN_PROGRESS);
   Require(!completed && outputs==1 && ticks==1);
 }
 puts("0097e380_TEST PASS");
}
float CThingAICreature::GetFOV() const {abort();}

#include "engine/CSoundPair.h"

void CTCPhysicsBase::UpdateOldPos() { abort(); }
void CTCPhysicsBase::UpdateOldRHSet() { abort(); }
void CTCPhysicsBase::UpdateInterpolation() { abort(); }
CThingPhysicsMeshInfo const * CTCPhysicsBase::GetPhysicsMeshInfo() const { abort(); }
bool CTCPhysicsBase::HasCollidedWithLandscapeAt(C3DVector const &,CPhysicsCollideResponse &) const { abort(); }
bool CTCPhysicsBase::HasCollidedWithThingAt(CTCPhysicsBase const &,C3DVector const &,CPhysicsCollideResponse &) const { abort(); }
bool CTCPhysicsBase::HasCollidedWithThingFromTo(CTCPhysicsBase const &,C3DVector const &,C3DVector const &,CPhysicsCollideResponse &) const { abort(); }
void CTCPhysicsBase::FillPhysicsDebugInfoForSelf(CPhysicsDebugInfo &) const { abort(); }
bool CTCPhysicsBase::DrawGetPhysicsDebugInfo(CThingPhysicalDebugInfo &) const { abort(); }
void CTCPhysicsBase::Move() { abort(); }
void CTCPhysicsBase::SetScaleFromOwner() { abort(); }
void CTCPhysicsBase::SetPhysicsMeshBankIndex(long) { abort(); }
void CTCPhysicsBase::SetAsPushableByHero(bool) { abort(); }
bool CTCPhysicsBase::IsPushableByHero() const { abort(); }
void CTCPhysicsBase::ApplyControlledAcceleration(C3DVector const &,float) { abort(); }
void CTCPhysicsBase::ApplyControlledImpulse(C3DVector const &) { abort(); }
void CTCPhysicsBase::ResetInterpolation() { abort(); }
float CTCPhysicsBase::GetRenderZOffset(float) const { abort(); }
void CTCPhysicsBase::SetRHSetAsFlushOnGround() { abort(); }
void CTCPhysicsBase::SetRHSetAsFlushOnNormal(C3DVector const &) { abort(); }
C3DVector CTCPhysicsBase::GetImpulse() const { abort(); }
void CTCPhysicsBase::SetRHSet(CRightHandedSet const &) { abort(); }
void CTCPhysicsBase::SetFacingAnglesFromRHSet(CRightHandedSet const &) { abort(); }
CRightHandedSet const & CTCPhysicsBase::GetLastRHSet() const { abort(); }
CRightHandedSet const & CTCPhysicsBase::GetRHSet() const { abort(); }
#include "engine/CRightHandedSet.h"
CRightHandedSet CTCPhysicsBase::GetRHSet(float) const { abort(); }
C3DVector CTCPhysicsBase::GetFacingVector() const { abort(); }
C3DVector CTCPhysicsBase::GetFacingVector(float) const { abort(); }
void CTCPhysicsBase::SetFacingAngleXZ(float) { abort(); }
void CTCPhysicsBase::SetFacingAngleYZ(float) { abort(); }
void CTCPhysicsBase::SetFacingAngleXY(float) { abort(); }
float CTCPhysicsBase::GetFacingAngleXZ() const { abort(); }
float CTCPhysicsBase::GetFacingAngleYZ() const { abort(); }
float CTCPhysicsBase::GetFacingAngleXY() const { abort(); }
bool CTCPhysicsBase::IsPhysicsMeshImpassable() const { abort(); }
bool CTCPhysicsBase::IsExcludingCreatures() const { abort(); }
bool CTCPhysicsBase::CanInteractWithThings() const { abort(); }
bool CTCPhysicsBase::CanCollideWithLandscape() const { abort(); }
bool CTCPhysicsBase::CanCollideWithThingsOneWay() const { abort(); }
bool CTCPhysicsBase::CanCollideWithThings() const { abort(); }
bool CTCPhysicsBase::IsOnGroundSurface() const { abort(); }
bool CTCPhysicsBase::IsOnMeshSurface() const { abort(); }
bool CTCPhysicsBase::IsOnAnyStationarySurface() const { abort(); }
bool CTCPhysicsBase::IsOnAnySurface() const { abort(); }
bool CTCPhysicsBase::IsImmoveable() const { abort(); }
bool CTCPhysicsBase::IsAngledWithGround() const { abort(); }
bool CTCPhysicsBase::IsAffectedByWind() const { abort(); }
bool CTCPhysicsBase::IsAffectedByAir() const { abort(); }
bool CTCPhysicsBase::IsAffectedByGravity() const { abort(); }
bool CTCPhysicsBase::IsAlwaysFacingMovementDirection() const { abort(); }
void CTCPhysicsBase::OnAddedToUpdateList() { abort(); }
void CTCPhysicsBase::OnDroppedFromUpdateList() { abort(); }
void CTCPhysicsBase::ReturnToDefaultInteractionFlags() { abort(); }
void CTCPhysicsBase::SetInteractionFlags(long) { abort(); }
bool CTCPhysicsBase::HasNavigationLayers() const { abort(); }
bool CTCPhysicsBase::IsNavigationEnabled() const { abort(); }
void CTCPhysicsBase::EnableNavigator(bool) { abort(); }
bool CTCPhysicsBase::IsGravityEnabled() const { abort(); }
void CTCPhysicsBase::EnableGravity(bool) { abort(); }
bool CTCPhysicsBase::IsPhysicsEnabled() const { abort(); }
void CTCPhysicsBase::EnablePhysics(bool) { abort(); }
void CTCPhysicsBase::ResetMovementState() { abort(); }
void CTCPhysicsBase::SetForcedToMovePos(C3DVector const &,bool) { abort(); }
void CTCPhysicsBase::SetPositionToNavigateTo(C3DVector const &) { abort(); }
void CTCPhysicsBase::ResetRHSet(CRightHandedSet const &) { abort(); }
void CTCPhysicsBase::ResetPosition(C3DVector const &) { abort(); }
void CTCPhysicsBase::SetPosition(C3DVector const &) { abort(); }
C3DVector CTCPhysicsBase::GetOffsetVector3D() const { abort(); }
C2DVector CTCPhysicsBase::GetOffsetVector() const { abort(); }
bool CTCPhysicsBase::IsCopyable() const { abort(); }
bool CTCPhysicsBase::IsComponentSerialisable() const { abort(); }
ETCInterfaceType CTCPhysicsBase::GetInterfaceType() const { abort(); }
bool CTCPhysicsBase::IsSerialisableForHeroState() const { abort(); }
void CTCPhysicsBase::FrameUpdate() { abort(); }
bool CTCPhysicsBase::HasFrameUpdate() const { abort(); }
void CTCPhysicsBase::InitialActivate() { abort(); }
void CTCPhysicsBase::OnCreate() { abort(); }
void CTCPhysicsBase::ConstructFromParams(CTCParamsBase const &) { abort(); }
void CTCPhysicsBase::OnSerialise(CPersistContext &,bool) { abort(); }