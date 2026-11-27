#define FABLE_RETAIL_CDEFSTRINGTABLE
// Leaf: ?Update@CAIStateGroup_KidWatchHopscotch@@MAEXXZ
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
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_MESSAGEEVENTMANAGER
#define FABLE_RETAIL_CWORLD_MESSAGEEVENTMANAGER_CONST
#define FABLE_RETAIL_CTCCREATURENAVIGATION
#define FABLE_RETAIL_CTCCREATURENAVIGATION_NAVIGABLE
#define FABLE_RETAIL_CTCCREATURENAVIGATION_NEARBYDIRECTED
#define FABLE_RETAIL_CTHINGCREATUREBASE_ISPOSINVIEWANGLE
#define FABLE_RETAIL_CTHINGAICREATURE_STATTYPEFLAGS
#define FABLE_RETAIL_CTHING_ORIGINAL_VTABLE
#define FABLE_RETAIL_CMESSAGEEVENT
#define FABLE_RETAIL_CMESSAGEEVENT_ORIGINATOR_ORIGINAL
#define FABLE_RETAIL_CMESSAGEEVENTMANAGER_FINDRECENT
#define FABLE_RETAIL_CMESSAGEEVENTMANAGER_ORIGINAL_LOCAL
#define FABLE_RETAIL_STLP_BITSET_INDEX
#define FABLE_RETAIL_CAIBRAIN_CREATURE_REFERENCE
#define FABLE_RETAIL_CAIBRAIN_CTOR_PASS23
#define FABLE_RETAIL_SENSED_PASS23
#define FABLE_RETAIL_KID_EVADE_LIFETIMES_PASS23
#define FABLE_RETAIL_CMESSAGEEVENTMANAGER_CTOR
#define private public
#define protected public
#include "fable_string.h"
#include "engine/CThingAICreature.h"
#include "engine/CAIStateGroup_KidWatchHopscotch.h"
#include "engine/CAIBrain.h"
#include "engine/ThingAlive.h"
#include "engine/EAIAction.h"
#include "engine/EMovementType.h"
#include "engine/CTCCreatureNavigation.h"
#include "engine/CWorld.h"
#include "engine/CMainGameComponent.h"
#include "engine/GFRandom.h"
#include "engine/GFFloatRandom.h"
#include "engine/CMessageEventFilter_IsHopscotchPlayerOnThisBoard.h"
#include "engine/CMessageEventManager.h"
#undef private
#undef protected
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void CAIStateGroup_KidWatchHopscotch::Update()
{
    long LastWanderFrame = PeekWorld().GetFrame();
    long CurrentWanderInterval = CMainGameComponent::GetConstantFPS()*2.0f + GFRandom(CMainGameComponent::GetConstantFPS()*8.0f,GetPWorldSeed());
    EAIActionReturn ret;
    do {
        ret=AI_ACTION_TERMINATE_PROCESS;
        if(IS_THING_ALIVE(PHopscotchBoard)) {
            bool got_route;
            if((GetCreature().GetPos()-HopscotchCentre).GetMagnitude2D()>HopscotchRadius*2.0f) {
                ret=AIAction_MoveToPosProximity(ACTION_REPEAT_UNTIL_FINISHED,PHopscotchBoard->GetPos(),HopscotchRadius*1.2f+0.25f,got_route,AI_MOVEMENT_RUNNING,true,false,true,0,true,0.0f);
            } else {
                const CTCCreatureNavigation* ptc_nav;
                if((GetCreature().GetPos()-HopscotchCentre).GetMagnitude2D()<HopscotchRadius*1.2f) {
                    GetCreature().GetTC(&ptc_nav);
                    C3DVector watch_pos;
                    if(ptc_nav->GetNearbyDirectedPosition(GetCreature().GetPos(),HopscotchRadius*1.2f,C3DVector(GetCreature().GetPos()-HopscotchCentre).Normalise(),1.0f/12.0f,watch_pos)) {
                        ret=AIAction_MoveToPos(ACTION_REPEAT_UNTIL_FINISHED,watch_pos,got_route,AI_MOVEMENT_RUNNING,true,true,0,true,0.0f);
                        if(ret!=AI_ACTION_FINISHED) continue;
                    }
                }
                if(PeekWorld().GetFrame()-LastWanderFrame>CurrentWanderInterval) {
                    GetCreature().GetTC(&ptc_nav);
                    C3DVector watch_pos;
                    float radius_range=HopscotchRadius*(2.0f-1.2f);
                    if(ptc_nav->GetNearbyWanderPosition(HopscotchCentre,HopscotchRadius*1.2f+GFFloatRandom(radius_range,GetPWorldSeed()),watch_pos,false)) {
                        ret=AIAction_MoveToPos(ACTION_REPEAT_UNTIL_FINISHED,watch_pos,got_route,AI_MOVEMENT_RUNNING,true,true,0,true,0.0f);
                        if(ret!=AI_ACTION_FINISHED) continue;
                        LastWanderFrame=PeekWorld().GetFrame();
                        CurrentWanderInterval=CMainGameComponent::GetConstantFPS()*2.0f+GFRandom(CMainGameComponent::GetConstantFPS()*8.0f,GetPWorldSeed());
                    }
                }
                const CMessageEvent* pevent=PeekWorld().GetMessageEventManager().FindMostRecentMessage(CMessageEventFilter_IsHopscotchPlayerOnThisBoard(*PHopscotchBoard));
                if(pevent && IS_THING_ALIVE(pevent->GetPOriginator<CThingAICreature>()) &&
                    (pevent->GetPOriginator<CThingAICreature>()->GetPos()-HopscotchCentre).GetMagnitude2D()<HopscotchRadius*2.0f)
                    ret=AIAction_LookAtThing(ACTION_REPEAT_UNTIL_FINISHED,pevent->GetPOriginator<CThingAICreature>(),1,5.0f/360.0f);
                else if(!GetCreature().IsPosInViewAngle(PHopscotchBoard->GetPos(),GetCreature().GetFOV()/360.0f,false,false))
                    ret=AIAction_LookAtPos(ACTION_REPEAT_UNTIL_FINISHED,PHopscotchBoard->GetPos(),1,5.0f/360.0f);
                else ret=AIAction_DoNothing();
            }
        }
    } while(ret==AI_ACTION_FINISHED && PeekWorld().GetFrame()-StartFrame<CMainGameComponent::GetConstantFPS()*30.0f);
    Terminate();
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
CAIBrain::CAIBrain(const CMainGameComponent&, CWorld&, CThingManager&, CPlayerManager&, CThingAICreature& actor,CDefPointer<const CBrainDef>) : Creature(actor) {}
class Nav : public CTCCreatureNavigation {
public:
    Nav(CThing& actor): CTCCreatureNavigation(actor) {}
    ETCInterfaceType GetInterfaceType() const { return TCI_NAVIGATION; }
    CCharString GetClassTypeName() const { return CCharString(); }
};
CTCCreatureNavigation::CTCCreatureNavigation(CThing& actor):CTCBase(actor) {}
CTCCreatureNavigation::~CTCCreatureNavigation() {}
CMessageEventManager::CMessageEventManager(const CWorld& world):World(world) {}
long CMainGameComponent::ConstantFPS=1;
unsigned long GFastOneOverSqrtTable[128]={0};
static unsigned long seed;
static CWorld* world;
static CThingAICreature* actor;
static CThing* board;
static CThingAICreature* player;
static Nav* nav;
static C3DVector centre(0,0,0), boardpos(0,2,0), actorpos, playerpos, directedpos(4,2,0), wanderpos(8,2,0);
static bool repeat_once;
static bool timed,has_direct,has_wander,move_success,visible;
static unsigned frames,directed,wander,moves,prox,looks,lookpos,nothing,terms,views,rands;
const CWorld& CAIStateGroupBase::PeekWorld() const { return *world; }
unsigned long* CAIStateGroupBase::GetPWorldSeed() const { return &seed; }
long __fastcall CWorld::GetFrame() { ++frames; if(repeat_once) return frames<4?0:100; return frames==1?100:timed?200:100; }
long __fastcall GFROR13(unsigned long val) { return (val>>13)|(val<<19); }
float __fastcall GFFloatRandom(float max,unsigned long* rng) { Require(rng==&seed && max>0.79f && max<0.81f);++rands;return 0.4f; }
const C3DVector& CThing::GetPos() const { if(this==board) return boardpos; if(this==actor)return actorpos;Require(this==player);return playerpos; }
float CThingAICreature::GetFOV() const { Require(this==actor);return 180.0f; }
bool CThingCreatureBase::IsPosInViewAngle(const C3DVector& pos,float angle,bool a,bool b) const
{ Require(this==actor && &pos==&boardpos && angle==0.5f && !a && !b);++views;return visible; }
bool CTCCreatureNavigation::GetNearbyDirectedPosition(const C3DVector& pos,float rad,const C3DVector& dir,float ang,C3DVector& out) const
{ Require(this==nav && &pos==&actorpos && rad==1.2f && ang==1.0f/12.0f && dir.X>0 && dir.Y==0); ++directed;out=directedpos;return has_direct; }
bool CTCCreatureNavigation::GetNearbyWanderPosition(const C3DVector& pos,float rad,C3DVector& out,bool avoid) const
{ Require(this==nav && pos.X==0 && pos.Y==0 && rad>1.59f && rad<1.61f && !avoid);++wander;out=wanderpos;return has_wander; }
void CAIStateGroupBase::Terminate() { ++terms; }
EAIActionReturn CAIStateGroupBase::AIAction_MoveToPos(EAIActionRepeat rep,C3DVector pos,bool& route,EMovementType mt,bool a,bool b,const CThing* other,bool c,float speed)
{ Require(rep==ACTION_REPEAT_UNTIL_FINISHED && mt==AI_MOVEMENT_RUNNING && a && b && !other && c && speed==0 && (pos.X==4 || pos.X==8));route=true;++moves;return move_success?AI_ACTION_FINISHED:AI_ACTION_FAILED; }
EAIActionReturn CAIStateGroupBase::AIAction_MoveToPosProximity(EAIActionRepeat rep,C3DVector pos,float radius,bool& route,EMovementType mt,bool a,bool b,bool c,const CThing* other,bool d,float speed)
{ Require(rep==ACTION_REPEAT_UNTIL_FINISHED && pos.Y==2 && radius==1.45f && mt==AI_MOVEMENT_RUNNING && a && !b && c && !other && d && speed==0); ++prox;route=true;return AI_ACTION_FINISHED; }
EAIActionReturn CAIStateGroupBase::AIAction_LookAtThing(EAIActionRepeat rep,CIntelligentPointer<const CThing> who,long f,float speed)
{ Require(rep==ACTION_REPEAT_UNTIL_FINISHED && who.Get()==player && f==1 && speed==5.0f/360.0f);++looks;return AI_ACTION_FINISHED; }
EAIActionReturn CAIStateGroupBase::AIAction_LookAtPos(EAIActionRepeat rep,C3DVector pos,long f,float speed)
{ Require(rep==ACTION_REPEAT_UNTIL_FINISHED && pos.Y==2 && f==1 && speed==5.0f/360.0f);++lookpos;return AI_ACTION_FINISHED; }
EAIActionReturn CAIStateGroupBase::AIAction_DoNothing() { ++nothing;return AI_ACTION_FINISHED; }
void CAIStateGroup_KidWatchHopscotch::OnActivate() { abort(); }
bool CAIStateGroup_KidWatchHopscotch::IsRunnable() { abort();return false; }
void CAIStateGroup_KidWatchHopscotch::InitStateGroup(const CWorld&,CThingAICreature&) { abort(); }
class HopscotchActor : public CThingAICreature {
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
int main() {
    HopscotchActor kid, child;
    CThing gameboard;
    Nav navigation(kid);
    actor=&kid;board=&gameboard;player=&child;nav=&navigation;
    static unsigned __int64 world_storage[64], manager_storage[4],root_storage[4],player_storage[4];
    world=reinterpret_cast<CWorld*>(world_storage);
    CMessageEventManager events(*world);
    world->PMessageEventManager_Data=&events;
    CAIBrain ai(*reinterpret_cast<CMainGameComponent*>(root_storage),*world,*reinterpret_cast<CThingManager*>(manager_storage),
                *reinterpret_cast<CPlayerManager*>(player_storage),kid,CDefPointer<const CBrainDef>());
    CAIStateGroup_KidWatchHopscotch state;
    state.PBrain=&ai;state.HopscotchCentre=centre;state.HopscotchRadius=1.0f;state.StartFrame=0;
    memset(kid.UsingComponents._M_w,0,sizeof(kid.UsingComponents._M_w));
    kid.UsingComponents[TCI_NAVIGATION]=true;
    kid.Components.push_back(std::pair<ETCInterfaceType,CTCBase*>(TCI_NAVIGATION,&navigation));
    gameboard.UniqueID=0x12567890abcdef01ui64;
    CMessageEvent ev;ev.Type=MESSAGE_EVENT_HOPSCOTCH_PLAYING;
    ev.POriginator.SetPItem(&child);
    events.Messages.push_back(ev);
    for(unsigned flags=0;flags<64;++flags)
    for(unsigned distance=0;distance<3;++distance)
    for(unsigned board_state=0;board_state<3;++board_state)
    for(unsigned event_state=0;event_state<7;++event_state) {
        state.PHopscotchBoard.SetPItem(board_state==0?0:&gameboard);
        gameboard.Dead=board_state==1;
        actorpos=C3DVector(distance==0?0.5f:distance==1?1.5f:2.5f,0,0);
        timed=(flags&1)!=0;has_direct=(flags&2)!=0;has_wander=(flags&4)!=0;move_success=(flags&8)!=0;visible=(flags&16)!=0;
        seed=flags;
        CMessageEvent& msg=*events.Messages.begin();
        msg.TargetUID=gameboard.UniqueID^(event_state==1?0x100000000ui64:event_state==2?1ui64:0ui64);
        msg.Type=event_state==0?MESSAGE_EVENT_ATTACK:MESSAGE_EVENT_HOPSCOTCH_PLAYING;
        msg.POriginator.SetPItem(event_state==3?0:&child);
        child.ThingTypeFlags=event_state==4?3:31;child.Dead=event_state==5;
        playerpos=C3DVector((flags&32)?1.0f:3.0f,0,0);
        frames=directed=wander=moves=prox=looks=lookpos=nothing=terms=views=rands=0;
        state.Update();
        bool active=board_state==2;
        bool near=active && distance==0;
        bool change=near && has_direct;
        bool roam=active && distance<2 && timed && (!change||move_success);
        bool move_roam=roam&&has_wander;
        bool observe=active && distance<2 && (!(change||move_roam)||move_success);
        bool watch=observe && event_state==6 && (flags&32);
        Require(terms==1 && prox==unsigned(active&&distance==2));
        Require(directed==unsigned(near) && wander==unsigned(roam) && rands==wander);
        Require(moves==unsigned(change)+unsigned(move_roam));
        Require(looks==unsigned(watch) && views==unsigned(observe&&!watch));
        Require(lookpos==unsigned(observe&&!watch&&!visible) && nothing==unsigned(observe&&!watch&&visible));
    }
    // A second event behind a newer unrelated event still identifies the player.
    CMessageEvent& newest=*events.Messages.begin();
    newest.Type=MESSAGE_EVENT_ATTACK;
    ev.Type=MESSAGE_EVENT_HOPSCOTCH_PLAYING;ev.TargetUID=gameboard.UniqueID;
    ev.POriginator.SetPItem(&child);events.Messages.push_back(ev);
    gameboard.Dead=false;child.Dead=false;child.ThingTypeFlags=31;playerpos=C3DVector(1,0,0);
    actorpos=C3DVector(1.5f,0,0);state.PHopscotchBoard.SetPItem(&gameboard);
    timed=false;looks=terms=frames=0;
    state.Update();Require(looks==1 && terms==1);
    // An empty list takes the viewport path for two consecutive frames.
    events.Messages.clear(); repeat_once=true;visible=true;has_wander=false;
    frames=terms=views=nothing=rands=0;
    state.Update();Require(frames==5 && terms==1 && views==2 && nothing==2 && rands==1);
    puts("00940bf0_TEST PASS");
}

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