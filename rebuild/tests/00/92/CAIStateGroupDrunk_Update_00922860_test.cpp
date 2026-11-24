// Behavior fixture for the reconstructed Drunk state-group update.
// Leaf: ?Update@CAIStateGroup_Drunk@@UAEXXZ
// Flags:
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHING_HASTC_MATERIALISED
#define FABLE_RETAIL_CTHING_COMPONENT_QUERY_PASS27
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CINTELLIGENTPOINTER_DEREFERENCE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN_OUTOFLINE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_CTOR_OUTOFLINE
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEALACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_PERFORMACTION
#define FABLE_RETAIL_CAISTATEGROUP_DRUNK_UPDATE_PASS27
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_GREETING_PASS23
#define FABLE_RETAIL_CTCDRUNKENNESS_DRANKFRAMES
#define FABLE_RETAIL_CTCVILLAGE
#define FABLE_RETAIL_CTCVILLAGE_PEEKMARKERS_PASS27
#define FABLE_RETAIL_CTCEXCLUSIVEUSE
#define FABLE_RETAIL_CTCCREATURENAVIGATION
#define FABLE_RETAIL_CTCCREATURENAVIGATION_NAVIGABLE
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CTHINGFILTER_ISNOTBEINGEXCLUSIVELYUSED_PASS24
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE_PCREATURE
#define FABLE_RETAIL_CCREATUREACTION_PLAYANIMATION
#define FABLE_RETAIL_CCREATUREACTION_PLAYANIMATION_IMPLICIT_DTOR
#define FABLE_RETAIL_CSOUNDPAIR_ORIGINAL_CTORS
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_PACKED
#define FABLE_RETAIL_C3DVECTOR_ORIGINAL_COPY_FORMAL
#define FABLE_RETAIL_C3DVECTOR_ORIGINAL_COMPONENT_CTOR
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCREATUREACTIONBASE_ANIMCTOR
#define FABLE_RETAIL_CTCPHYSICSBASE_ORIGINAL_CTOR_PASS23
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_STLP_BITSET_INDEX
#define private public
#define protected public
#include "fable_assert.h"
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CDefPointer.h"
#include "engine/CCountedPointer.h"
#include "engine/CThing.h"
#include "engine/CThingAICreature.h"
#include "engine/CThingMarker.h"
#include "engine/CIntelligentPointer.h"
#include "engine/ThingAlive.h"
#include "engine/EAIAction.h"
#include "engine/EMovementType.h"
#include "engine/CAIStateGroup_Drunk.h"
#include "engine/CAIBrain.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CTCVillageMember.h"
#include "engine/CTCVillage.h"
#include "engine/CThingFilter_IsNotBeingExclusivelyUsed.h"
#include "engine/CTCDrunkenness.h"
#include "engine/CTCExclusiveUse.h"
#include "engine/CTCCreatureNavigation.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CRightHandedSet.h"
#include "engine/CSoundPair.h"
#include "engine/CCreatureAction_PlayAnimation.h"
#include "engine/GFRandom.h"
#include "engine/GFFloatRandom.h"
#include "engine/EAIAction.h"
#include "engine/EMovementType.h"


#undef private
#undef protected

template<> __declspec(noinline) inline bool FABLE_FASTCALL IS_THING_ALIVE(const CIntelligentPointer<CThingMarker>& thing)
{
    if (thing != 0) {
        if (!thing->IsDead())
            return true;
    }
    return false;
}

// Inline-budget: retain the original out-of-line marker predicate and component query.
__declspec(noinline) inline bool CThingFilter_IsNotBeingExclusivelyUsed::operator()(const CThing& thing) const
{
    const CTCExclusiveUse* exclusive;
    return thing.PeekTC(&exclusive) && exclusive->GetUser() == 0;
}

void CAIStateGroup_Drunk::Update()
{
    CTCCreatureNavigation* pnav;
    CTCDrunkenness* tc_drunkenness;
    EAIActionReturn ret;
    float drunkenness;
    C3DVector wander_pos;

    if (!(GetCreature().GetTC(&tc_drunkenness) && tc_drunkenness->IsDrunk())) {
        Terminate();
        return;
    }
    drunkenness = tc_drunkenness->GetDrunkenness();

    if (GFFloatRandom(1.0f,GetPWorldSeed()) < 0.15f) {
        CIntelligentPointer<CThing> best;
        float best_dist = 0.0f;
        long def_index = CGameDefinitionManager::Get().GetDefGlobalIndexFromName("MARKER_DISCHARGE_BODILY_FLUIDS");
        CTCVillageMember* tc_village_member;
        if (GetCreature().GetTC(&tc_village_member) && tc_village_member->GetPVillage()) {
            const std::list<CIntelligentPointer<CThingMarker> >& village_markers = tc_village_member->GetPVillage()->PeekVillageMarkers();
            for (std::list<CIntelligentPointer<CThingMarker> >::const_iterator i=village_markers.begin(); i!=village_markers.end(); ++i) {
                if (IS_THING_ALIVE(*i) && i->Get()->GetDefGlobalIndex()==def_index && CThingFilter_IsNotBeingExclusivelyUsed()(**i)) {
                    float dist = (i->Get()->GetPos()-GetCreature().GetPos()).GetSquaredMagnitude();
                    if (!IS_THING_ALIVE(best) || dist < best_dist) {
                        best = i->Get();
                        best_dist = dist;
                    }
                }
            }
        }
        if (IS_THING_ALIVE(best)) {
            CTCExclusiveUse* exclusive;
            if (IS_THING_ALIVE(best) && best->GetTC(&exclusive))
                exclusive->SetUser(&GetCreature());
            EAIActionReturn ret;
            do {
                bool nav_route_calculated;
                TalkToSelfUpdate();
                ret = AIAction_MoveToPos(ACTION_REPEAT_CALLBACK,best->GetPos(),nav_route_calculated,
                    AI_MOVEMENT_WALKING,true,true,0,true,0.0f);
            } while (ret==AI_ACTION_IN_PROGRESS);

            ret = AIAction_LookAtPos(ACTION_REPEAT_UNTIL_FINISHED,
                best->GetPhysicsTC()->GetRHSet().GetForward()+best->GetPos(),17,0.0027777f);
            if (GFRandom(2,GetPWorldSeed())==0)
                ret = AIAction_PerformActionAndWait(CCreatureAction_PlayAnimation(GetCreature(),
                    "ST_PISSING",CSoundPair(0),false,true,false,0,0),false);
            else
                ret = AIAction_PerformActionAndWait(CCreatureAction_PlayAnimation(GetCreature(),
                    "ST_PUKING",CSoundPair(0),false,true,false,0,0),false);
            if (IS_THING_ALIVE(best) && best->GetTC(&exclusive))
                exclusive->SetUser(0);
            if (ret!=AI_ACTION_FINISHED) {
                Terminate();
                return;
            }
            drunkenness -= 0.6f;
        }
    }

    if (drunkenness<0.0f)
        drunkenness=0.0f;
    tc_drunkenness->SetDrunkenness(drunkenness);
    if (!tc_drunkenness->IsDrunk()) {
        Terminate();
        return;
    }
    ret=AI_ACTION_FAILED;
    if (!GetCreature().GetTC(&pnav))
        FABLE_RETAIL_ASSERT(false);
    if (pnav->GetNearbyWanderPosition(GetCreature().PeekPos(),5.0f,wander_pos,true)) {
        bool nav_route_calculated=false;
        do {
            TalkToSelfUpdate();
            ret = AIAction_MoveToPos(ACTION_REPEAT_CALLBACK,wander_pos,nav_route_calculated,
                AI_MOVEMENT_WALKING,true,true,0,true,0.0f);
        } while (ret==AI_ACTION_IN_PROGRESS);
        if (ret!=AI_ACTION_FINISHED) {
            Terminate();
            return;
        }
        if (GFRandom(100,GetPWorldSeed())<50) {
            unsigned long no_of_idles=GFRandom(2,GetPWorldSeed());
            if (no_of_idles!=0) {
                for (long c1=0;c1<no_of_idles;++c1) {
                    ret=AI_ACTION_FAILED;
                    do {
                        TalkToSelfUpdate();
                        ret = AIAction_PerformActionAndWait(CCreatureAction_PlayAnimation(GetCreature(),
                            "ST_IDLE",0.0f,CSoundPair(0),false,true,false,0,0),false);
                    } while (ret==AI_ACTION_IN_PROGRESS);
                    if (ret!=AI_ACTION_FINISHED) {
                        Terminate();
                        return;
                    }
                }
            }
        }
    }
    Terminate();
}

#include <new>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "engine/CAnimationEntry.h"
#include "engine/C2DVector.h"
#include "engine/CPhysicsCollideResponse.h"

template<class T> union Storage {
    unsigned __int64 align;
    unsigned char bytes[sizeof(T)];
    T* Get() { return reinterpret_cast<T*>(bytes); }
};
template<class T> static void Init(T& value)
{
    new (&value) T;
}

static void Check(bool condition, int line)
{
    if (!condition) {
        printf("check failed on line %d\n", line);
        fflush(stdout);
        abort();
    }
}
#define Require(x) Check((x), __LINE__)

static const C3DVector actor_pos(1,2,3), wander(10,-9,7);
static const C3DVector marker_pos[] = {
    C3DVector(2,3,4), C3DVector(1,2,3), C3DVector(0,0,0), C3DVector(1,2,3),
    C3DVector(20,2,3), C3DVector(3,4,3), C3DVector(90,50,60)
};
static const C3DVector forward(2,-1,4);
static CThingAICreature* actor;
static CThingMarker* markers[7];
static CTCVillage* village;
static CTCVillageMember* villager;
static CTCCreatureNavigation* nav;
static CTCDrunkenness* drunk;
static CTCExclusiveUse* marker_user[7];
static const CThing* use_log[4];
static CTCExclusiveUse* used_components[4];
static CThing* newly_dead;
static const char* animation_log[6];
static const char* current_animation;
static const CAnimationEntry* empty_animation;
static CRightHandedSet rh_set;

static float roll;
static unsigned long seed, random_values[5];
static EAIActionReturn action_results[6], moves[10];
static unsigned terminations, look_calls, move_calls, animations, random_calls, roll_calls,
                talks, def_reads, nav_queries, village_queries, user_changes;
static bool should_wander;

static bool Same(const C3DVector& a, const C3DVector& b)
{
    return a.X == b.X && a.Y == b.Y && a.Z == b.Z;
}

static void AddComponent(CThing& owner, ETCInterfaceType type, CTCBase* component)
{
    owner.UsingComponents[type] = true;
    owner.Components.push_back(std::make_pair(type, component));
}
static void ResetComponent(CThing& owner, ETCInterfaceType type, bool enabled)
{
    owner.UsingComponents[type] = enabled;
}

CBaseIntelligentPointer::CBaseIntelligentPointer() : PData(0) {}
CBaseIntelligentPointer::~CBaseIntelligentPointer() {}
void CBaseIntelligentPointer::SetPItem(CBaseObject* item) { PData = reinterpret_cast<CBaseObjectPointer*>(item); }
CBaseObject* CBaseIntelligentPointer::GetPItem() const { return reinterpret_cast<CBaseObject*>(PData); }

template<class T> CIntelligentPointer<T>::CIntelligentPointer(T* ptr) { SetPointer(ptr); }
template<class T> CIntelligentPointer<T>& CIntelligentPointer<T>::operator=(const CIntelligentPointer& other)
{
    SetPItem(other.GetPItem());
    return *this;
}
CCharString::CCharString() : PStringData(0) {}
CCharString::CCharString(const char* text, long length) : PStringData(reinterpret_cast<CCharStringData*>(const_cast<char*>(text)))
{
    Require(length == -1);
}
CCharString::~CCharString() {}
static const char* Text(const CCharString& str) { return reinterpret_cast<const char*>(str.PStringData); }

CGameDefinitionManager& FABLE_FASTCALL CGameDefinitionManager::Get()
{
    static Storage<CGameDefinitionManager> storage;
    return *storage.Get();
}
long CDefinitionManager::GetDefGlobalIndexFromName(const CCharString& str) const
{
    ++def_reads;
    Require(strcmp(Text(str), "MARKER_DISCHARGE_BODILY_FLUIDS") == 0);
    return 19;
}

CAIStateGroupBase::CAIStateGroupBase() : PBrain(0) {}
void CAIStateGroupBase::Terminate() { ++terminations; }
void CAIStateGroup_Drunk::TalkToSelfUpdate() { ++talks; }
unsigned long* CAIStateGroupBase::GetPWorldSeed() const { return &seed; }
float FABLE_FASTCALL GFFloatRandom(float range, unsigned long* random_seed)
{
    Require(random_seed == &seed && range == 1.0f);
    ++roll_calls;
    return roll;
}
long FABLE_FASTCALL GFROR13(unsigned long)
{
    Require(random_calls < sizeof(random_values)/sizeof(random_values[0]));
    return random_values[random_calls++];
}

CTCVillage* CTCVillageMember::GetPVillage() const
{
    Require(this == villager);
    ++village_queries;
    return village;
}

const C3DVector& CThing::GetPos() const
{
    if (this == actor)
        return actor_pos;
    for (unsigned i=0; i<sizeof(markers)/sizeof(markers[0]); ++i)
        if (this == markers[i])
            return marker_pos[i];
    abort();
    return actor_pos;
}
const C3DVector& CThingPhysical::PeekPos() const
{
    Require(this == actor);
    return actor_pos;
}

bool CTCCreatureNavigation::GetNearbyWanderPosition(const C3DVector& source, float distance,
    C3DVector& result, bool passable) const
{
    Require(this == nav && Same(source, actor_pos) && distance == 5.0f && passable);
    Require(user_changes == 0 || user_changes == 2);
    ++nav_queries;
    if (should_wander)
        result = wander;
    return should_wander;
}
void CTCExclusiveUse::SetUser(CThing* who)
{
    Require(user_changes < 4);
    Require((user_changes == 0 && who == actor) || (user_changes == 1 && !who));
    used_components[user_changes] = this;
    use_log[user_changes++] = who;
    User.Set(who);
}

EAIActionReturn CAIStateGroupBase::AIAction_MoveToPos(EAIActionRepeat repeat, C3DVector target,
    bool& route, EMovementType movement, bool first, bool second, const CThing* ignore, bool fourth, float offset)
{
    Require(repeat == ACTION_REPEAT_CALLBACK && movement == AI_MOVEMENT_WALKING);
    Require(first && second && !ignore && fourth && offset == 0.0f);
    Require(move_calls < sizeof(moves)/sizeof(moves[0]));
    Require(Same(target, nav_queries ? wander : marker_pos[5]));
    if (nav_queries)
        Require(route == (move_calls > 0 && moves[move_calls-1] == AI_ACTION_IN_PROGRESS));
    route = true;
    return moves[move_calls++];
}
EAIActionReturn CAIStateGroupBase::AIAction_LookAtPos(EAIActionRepeat repeat, C3DVector target, long frames, float rate)
{
    Require(repeat == ACTION_REPEAT_UNTIL_FINISHED && frames == 17 && rate == 0.0027777f);
    Require(Same(target, C3DVector(5,3,7)) && !nav_queries && user_changes == 1);
    ++look_calls;
    return AI_ACTION_FINISHED;
}
EAIActionReturn CAIStateGroupBase::AIAction_PerformActionAndWait(const CCreatureActionBase&, bool interrupt)
{
    Require(!interrupt && current_animation && animations < sizeof(action_results)/sizeof(action_results[0]));
    animation_log[animations] = current_animation;
    if (newly_dead && animations == 0)
        newly_dead->Dead = true;
    return action_results[animations++];
}
CCreatureActionBase::CCreatureActionBase(CThingCreatureBase&, float, float, const CAnimationEntry&,
    const CSoundPair&, long, long) {}
CCreatureActionBase::~CCreatureActionBase() {}
CCreatureAction_PlayAnimation::CCreatureAction_PlayAnimation(CThingCreatureBase& creature,
    const CCharString& animation, const CSoundPair& sound, bool run, bool physics, bool stay, long a, long b)
    : CCreatureActionBase(creature, 0.0f, 0.0f, *empty_animation, sound, a, b)
{
    Require(&creature == actor && !run && physics && !stay && a == 0 && b == 0);
    Require(sound.FirstSound == 0 && sound.LastSound == 0 && !nav_queries);
    current_animation = Text(animation);
    Require(strcmp(current_animation,"ST_PISSING") == 0 || strcmp(current_animation,"ST_PUKING") == 0);
}
CCreatureAction_PlayAnimation::CCreatureAction_PlayAnimation(CThingCreatureBase& creature,
    const CCharString& animation, float frame_offset, const CSoundPair& sound,
    bool run, bool physics, bool stay, long a, long b)
    : CCreatureActionBase(creature, 0.0f, 0.0f, *empty_animation, sound, a, b)
{
    Require(&creature == actor && !run && physics && !stay && a == 0 && b == 0);
    Require(frame_offset == 0.0f && sound.FirstSound == 0 && sound.LastSound == 0 && nav_queries == 1);
    current_animation = Text(animation);
    Require(strcmp(current_animation,"ST_IDLE") == 0);
}

class FacingPhysics : public CTCPhysicsBase {
public:
    explicit FacingPhysics(CThing& owner) : CTCPhysicsBase(owner) {}
    virtual CCharString GetClassTypeName() const { abort(); }
    virtual const CRightHandedSet& GetRHSet() const { return rh_set; }
};

void CCreatureAction_PlayAnimation::PerformAction() { abort(); }
void CCreatureAction_PlayAnimation::FrameUpdate() { abort(); }
void CCreatureAction_PlayAnimation::FinishAction() { abort(); }
bool CCreatureAction_PlayAnimation::IsMovementUsingPhysics() const { abort(); }
bool CCreatureAction_PlayAnimation::IsUsingAnimationMovement() const { abort(); }
bool CCreatureAction_PlayAnimation::IsAllowingAnimationMovementToMoveSpeakAndWaitCameras() const { abort(); }
CCreatureActionBase* CCreatureAction_PlayAnimation::Clone() const { abort(); }

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
CTCPhysicsBase::CTCPhysicsBase(CThing& thing) : CTCBase(thing), Radius(1.0f) {}
CTCPhysicsBase::~CTCPhysicsBase() {}
ETCInterfaceType CTCPhysicsBase::GetInterfaceType() const { return TCI_PHYSICS; }
C2DVector CTCPhysicsBase::GetOffsetVector() const { abort(); }
C3DVector CTCPhysicsBase::GetOffsetVector3D() const { abort(); }
void CTCPhysicsBase::SetPosition(const C3DVector &) { abort(); }
void CTCPhysicsBase::ResetPosition(const C3DVector &) { abort(); }
void CTCPhysicsBase::ResetRHSet(const CRightHandedSet &) { abort(); }
void CTCPhysicsBase::SetPositionToNavigateTo(const C3DVector &) { abort(); }
void CTCPhysicsBase::SetForcedToMovePos(const C3DVector &,bool) { abort(); }
void CTCPhysicsBase::ResetMovementState() { abort(); }
void CTCPhysicsBase::EnablePhysics(bool) { abort(); }
bool CTCPhysicsBase::IsPhysicsEnabled() const { abort(); }
void CTCPhysicsBase::EnableGravity(bool) { abort(); }
bool CTCPhysicsBase::IsGravityEnabled() const { abort(); }
void CTCPhysicsBase::EnableNavigator(bool) { abort(); }
bool CTCPhysicsBase::IsNavigationEnabled() const { abort(); }
bool CTCPhysicsBase::HasNavigationLayers() const { abort(); }
void CTCPhysicsBase::SetInteractionFlags(long) { abort(); }
void CTCPhysicsBase::ReturnToDefaultInteractionFlags() { abort(); }
void CTCPhysicsBase::OnDroppedFromUpdateList() { abort(); }
void CTCPhysicsBase::OnAddedToUpdateList() { abort(); }
bool CTCPhysicsBase::IsAlwaysFacingMovementDirection() const { abort(); }
bool CTCPhysicsBase::IsAffectedByGravity() const { abort(); }
bool CTCPhysicsBase::IsAffectedByAir() const { abort(); }
bool CTCPhysicsBase::IsAffectedByWind() const { abort(); }
bool CTCPhysicsBase::IsAngledWithGround() const { abort(); }
bool CTCPhysicsBase::IsImmoveable() const { abort(); }
bool CTCPhysicsBase::IsOnAnySurface() const { abort(); }
bool CTCPhysicsBase::IsOnAnyStationarySurface() const { abort(); }
bool CTCPhysicsBase::IsOnMeshSurface() const { abort(); }
bool CTCPhysicsBase::IsOnGroundSurface() const { abort(); }
bool CTCPhysicsBase::CanCollideWithThings() const { abort(); }
bool CTCPhysicsBase::CanCollideWithThingsOneWay() const { abort(); }
bool CTCPhysicsBase::CanCollideWithLandscape() const { abort(); }
bool CTCPhysicsBase::CanInteractWithThings() const { abort(); }
bool CTCPhysicsBase::IsExcludingCreatures() const { abort(); }
bool CTCPhysicsBase::IsPhysicsMeshImpassable() const { abort(); }
float CTCPhysicsBase::GetFacingAngleXY() const { abort(); }
float CTCPhysicsBase::GetFacingAngleYZ() const { abort(); }
float CTCPhysicsBase::GetFacingAngleXZ() const { abort(); }
void CTCPhysicsBase::SetFacingAngleXY(float) { abort(); }
void CTCPhysicsBase::SetFacingAngleYZ(float) { abort(); }
void CTCPhysicsBase::SetFacingAngleXZ(float) { abort(); }
C3DVector CTCPhysicsBase::GetFacingVector() const { abort(); }
C3DVector CTCPhysicsBase::GetFacingVector(float) const { abort(); }
const CRightHandedSet & CTCPhysicsBase::GetRHSet() const { abort(); }
CRightHandedSet CTCPhysicsBase::GetRHSet(float) const { abort(); }
const CRightHandedSet & CTCPhysicsBase::GetLastRHSet() const { abort(); }
void CTCPhysicsBase::SetFacingAnglesFromRHSet(const CRightHandedSet &) { abort(); }
void CTCPhysicsBase::SetRHSet(const CRightHandedSet &) { abort(); }
C3DVector CTCPhysicsBase::GetImpulse() const { abort(); }
void CTCPhysicsBase::SetRHSetAsFlushOnNormal(const C3DVector &) { abort(); }
void CTCPhysicsBase::SetRHSetAsFlushOnGround() { abort(); }
float CTCPhysicsBase::GetRenderZOffset(float) const { abort(); }
void CTCPhysicsBase::ResetInterpolation() { abort(); }
void CTCPhysicsBase::ApplyControlledImpulse(const C3DVector &) { abort(); }
void CTCPhysicsBase::ApplyControlledAcceleration(const C3DVector &,float) { abort(); }
bool CTCPhysicsBase::IsPushableByHero() const { abort(); }
void CTCPhysicsBase::SetAsPushableByHero(bool) { abort(); }
void CTCPhysicsBase::FrameUpdate() { abort(); }
bool CTCPhysicsBase::HasFrameUpdate() const { abort(); }
void CTCPhysicsBase::SetPhysicsMeshBankIndex(long) { abort(); }
void CTCPhysicsBase::SetScaleFromOwner() { abort(); }
void CTCPhysicsBase::ConstructFromParams(const CTCParamsBase &) { abort(); }
void CTCPhysicsBase::OnCreate() { abort(); }
void CTCPhysicsBase::InitialActivate() { abort(); }
void CTCPhysicsBase::Move() { abort(); }
bool CTCPhysicsBase::DrawGetPhysicsDebugInfo(CThingPhysicalDebugInfo &) const { abort(); }
void CTCPhysicsBase::OnSerialise(CPersistContext &,bool) { abort(); }
bool CTCPhysicsBase::IsComponentSerialisable() const { abort(); }
bool CTCPhysicsBase::IsSerialisableForHeroState() const { abort(); }
bool CTCPhysicsBase::IsCopyable() const { abort(); }
void CTCPhysicsBase::FillPhysicsDebugInfoForSelf(CPhysicsDebugInfo &) const { abort(); }
bool CTCPhysicsBase::HasCollidedWithThingAt(const CTCPhysicsBase &,const C3DVector &,CPhysicsCollideResponse &) const { abort(); }
bool CTCPhysicsBase::HasCollidedWithLandscapeAt(const C3DVector &,CPhysicsCollideResponse &) const { abort(); }
const CThingPhysicsMeshInfo * CTCPhysicsBase::GetPhysicsMeshInfo() const { abort(); }
void CTCPhysicsBase::UpdateInterpolation() { abort(); }
void CTCPhysicsBase::UpdateOldRHSet() { abort(); }
void CTCPhysicsBase::UpdateOldPos() { abort(); }
CBaseClass::CBaseClass() {}
CBaseClass::~CBaseClass() {}
CBaseClassNonCopyable::CBaseClassNonCopyable() {}
CBaseClassNonCopyable::~CBaseClassNonCopyable() {}
CBaseObject::CBaseObject() : IntelligentPointer(0) {}
CBaseObject::~CBaseObject() {}
void CBaseObject::Validate() const {}
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
void CCreatureActionBase::EditUpdate(float) { abort(); }
void CCreatureActionBase::FinishAction() { abort(); }
bool CCreatureActionBase::IsPlayingAnimation() const { abort(); }
long CCreatureActionBase::GetAnimDelayTime() const { abort(); }
long CCreatureActionBase::GetAnimationIndex() const { abort(); }
long CCreatureActionBase::GetAnimationTransitionInTime() const { abort(); }
long CCreatureActionBase::GetAnimationTransitionOutTime() const { abort(); }
unsigned long CCreatureActionBase::GetAnimExtraFlags() const { abort(); }
void CCreatureActionBase::Initialise() { abort(); }
bool CCreatureActionBase::IsStrikeEvent(long) const { abort(); }
bool CCreatureActionBase::IsStrikeEventActive() const { abort(); }
bool CCreatureActionBase::IsStrikeAction() const { abort(); }
bool CCreatureActionBase::IsPartOfComboChain() const { abort(); }
bool CCreatureActionBase::IsStrikeEventFinishedBeingActive() const { abort(); }
bool CCreatureActionBase::IsVitalStrike() const { abort(); }
bool CCreatureActionBase::IsKnockdownStrike() const { abort(); }
bool CCreatureActionBase::IsBlockCounter() const { abort(); }
bool CCreatureActionBase::IsMeleeFlourish() const { abort(); }
bool CCreatureActionBase::IsActionWithStrikeEventHelperName() const { abort(); }
CCharString CCreatureActionBase::GetStrikeEventHelperName() const { abort(); }
bool CCreatureActionBase::IsOverrideableByActionOfSamePriority() const { abort(); }
void CCreatureActionBase::OnFrameUpdate(float) { abort(); }
bool CCreatureActionBase::IsQueuable() const { abort(); }
bool CCreatureActionBase::IsActionReadyForInterruption() const { abort(); }
bool CCreatureActionBase::IsActionReadyForInterruptionByMovement(const C3DVector &) const { abort(); }
bool CCreatureActionBase::IsBlockable() const { abort(); }
bool CCreatureActionBase::IsAffectedByHaste() const { abort(); }
bool CCreatureActionBase::IsAffectedByHeroStats() const { abort(); }
bool CCreatureActionBase::IsPhysicsPushableByHero() const { abort(); }
bool CCreatureActionBase::IsAlwaysPhysicsPushableBySelfAvoidance() const { abort(); }
bool CCreatureActionBase::IsAlwaysUseAnimationMovement() const { abort(); }
bool CCreatureActionBase::IsClearingMovementVector() const { abort(); }
bool CCreatureActionBase::IsToHaveEmptyHands() const { abort(); }
bool CCreatureActionBase::IsToReplaceItemIfRemoved() const { abort(); }
bool CCreatureActionBase::IsAllowingAnimationMovementToMoveSpeakAndWaitCameras() const { abort(); }
bool CCreatureActionBase::IsPreventingQuickAccessItemUsage() const { abort(); }
void CCreatureActionBase::SetHitter(const CThing &) { abort(); }
void CCreatureActionBase::SetHitDirection(C3DVector) { abort(); }
void CCreatureActionBase::SetHitPosition(C3DVector) { abort(); }
CTCWeaponTrail * CCreatureActionBase::GetWeaponTrailUsedInStrikeAction(long &) const { abort(); }
float CCreatureActionBase::GetTotalTimeSeconds() const { abort(); }
bool CCreatureActionBase::IsToEvadeAllHits() const { abort(); }
bool CCreatureActionBase::IsToCauseRecoil() const { abort(); }
bool CCreatureActionBase::IsAbleToBeAttacked() const { abort(); }
bool CCreatureActionBase::IsPlayHitResponse() const { abort(); }
bool CCreatureActionBase::IsPreventingNormalMovement() const { abort(); }
bool CCreatureActionBase::IsPreventingLookTurning() const { abort(); }
void CCreatureActionBase::OnBlowBlocked(bool) { abort(); }
void CCreatureActionBase::OnFollowUpActionFailedToAdd() { abort(); }
void CCreatureActionBase::UpdateActionRunningTime() { abort(); }
bool CCreatureActionBase::IsDecapitatingAttack() const { abort(); }
bool CCreatureActionBase::PreventSneaking() const { abort(); }
void CCreatureActionBase::FrameUpdate() { abort(); }
bool CCreatureActionBase::IsUsingAnimationMovement() const { abort(); }
bool CCreatureActionBase::IsUsingForcedMovement() const { abort(); }
bool CCreatureActionBase::IsMovementUsingPhysics() const { abort(); }
void CCreatureActionBase::UpdateActionMoveToPos() { abort(); }
void CCreatureActionBase::InitialiseActionMovement() { abort(); }
float CCreatureActionBase::GetFrameUpdateTime() const { abort(); }
float CCreatureActionBase::GetActionSpeedParameter() const { abort(); }
float CCreatureActionBase::GetActionStartRunningTimeSeconds() const { abort(); }
bool CCreatureActionBase::OverrideFaceWithDefault() const { abort(); }
bool CCreatureActionBase::AllowsCombinationAnims() const { abort(); }
bool CCreatureActionBase::CanAnimationTransitionOut() const { abort(); }
void CCreatureActionBase::OnAnimationLoaded() { abort(); }
bool CCreatureActionBase::WaitForMeshToLoad() const { abort(); }
bool CTCPhysicsBase::HasCollidedWithThingFromTo(const CTCPhysicsBase&, const C3DVector&, const C3DVector&, CPhysicsCollideResponse&) const { abort(); }
void CClass::Save(CCharString&) {}
static void Reset()
{
    roll = 0.5f;
    seed = 123;
    terminations = look_calls = move_calls = animations = random_calls = roll_calls = 0;
    talks = def_reads = nav_queries = village_queries = user_changes = 0;
    should_wander = false;
    newly_dead = 0;
    for (unsigned i=0; i<6; ++i)
        action_results[i] = AI_ACTION_FINISHED;
    for (unsigned i=0; i<10; ++i)
        moves[i] = AI_ACTION_FINISHED;
    for (unsigned i=0; i<5; ++i)
        random_values[i] = 0;
    ResetComponent(*actor, TCI_DRUNKENNESS, true);
    ResetComponent(*actor, TCI_VILLAGE_MEMBER, true);
    drunk->Drunk = true;
    drunk->Drunkenness = 1.0f;
    markers[5]->Dead = false;
    marker_user[5]->User.Set(0);
}

int main()
{
    static Storage<CThingAICreature> creature_store;
    static Storage<CThingMarker> marker_store[7];
    static Storage<CTCExclusiveUse> users_store[7];
    static Storage<CTCDrunkenness> drunk_store;
    static Storage<CTCVillage> village_store;
    static Storage<CTCVillageMember> villager_store;
    static Storage<CAIBrain> brain_store;
    static Storage<CAnimationEntry> animation_store;
    static Storage<CTCCreatureNavigation> nav_store;

    actor = creature_store.Get();
    drunk = drunk_store.Get();
    village = village_store.Get();
    villager = villager_store.Get();
    nav = nav_store.Get();
    empty_animation = animation_store.Get();

    Init(actor->UsingComponents);
    Init(actor->Components);
    AddComponent(*actor, TCI_NAVIGATION, nav);
    AddComponent(*actor, TCI_VILLAGE_MEMBER, villager);
    AddComponent(*actor, TCI_DRUNKENNESS, drunk);
    Init(village->VillageMarkers);

    for (unsigned i=0; i<7; ++i) {
        CThingMarker& marker = *marker_store[i].Get();
        marker_user[i] = users_store[i].Get();
        Init(marker_user[i]->User);
        Init(marker.UsingComponents);
        Init(marker.Components);
        AddComponent(marker, TCI_EXCLUSIVE_USE, marker_user[i]);
        marker.DefGlobalIndex = 19;
        markers[i] = &marker;
        if (i != 0)
            village->VillageMarkers.push_back(CIntelligentPointer<CThingMarker>(&marker));
        else
            village->VillageMarkers.push_back(CIntelligentPointer<CThingMarker>());
    }
    markers[1]->Dead = true;
    markers[2]->DefGlobalIndex = 20;
    marker_user[3]->User.Set(actor);
    FacingPhysics physics(*markers[5]);
    markers[5]->PhysicsTC = &physics;
    rh_set.SetForward(forward);

    CAIBrain* brain = brain_store.Get();
    brain->Creature = actor;
    CAIStateGroup_Drunk state;
    state.PBrain = brain;

    Reset();
    ResetComponent(*actor, TCI_DRUNKENNESS, false);
    state.Update();
    Require(terminations == 1 && !roll_calls);

    Reset();
    drunk->Drunk = false;
    state.Update();
    Require(terminations == 1 && !roll_calls);

    Reset();
    drunk->Drunkenness = -0.5f;
    state.Update();
    Require(terminations == 1 && nav_queries == 1 && !move_calls && drunk->Drunkenness == 0.0f);

    Reset();
    roll = 0.1f;
    ResetComponent(*actor, TCI_VILLAGE_MEMBER, false);
    state.Update();
    Require(terminations == 1 && def_reads == 1 && !village_queries && nav_queries == 1);

    Reset();
    roll = 0.1f;
    CTCVillage* old_village = village;
    village = 0;
    state.Update();
    Require(terminations == 1 && def_reads == 1 && village_queries == 1 && nav_queries == 1);
    village = old_village;

    Reset();
    roll = 0.1f;
    moves[0] = AI_ACTION_IN_PROGRESS;
    state.Update();
    Require(terminations == 1 && def_reads == 1 && village_queries == 2 && nav_queries == 1);
    Require(move_calls == 2 && look_calls == 1 && animations == 1 && talks == 2);
    Require(strcmp(animation_log[0], "ST_PISSING") == 0 && drunk->Drunkenness == 1.0f-0.6f);
    Require(user_changes == 2 && used_components[0] == marker_user[5] && used_components[1] == marker_user[5]);
    Require(use_log[0] == actor && use_log[1] == 0);

    Reset();
    roll = 0.1f;
    random_values[0] = 1;
    action_results[0] = AI_ACTION_FAILED;
    state.Update();
    Require(terminations == 1 && user_changes == 2 && !nav_queries && animations == 1);
    Require(strcmp(animation_log[0], "ST_PUKING") == 0 && drunk->Drunkenness == 1.0f);

    Reset();
    roll = 0.1f;
    newly_dead = markers[5];
    action_results[0] = AI_ACTION_FAILED;
    state.Update();
    Require(terminations == 1 && user_changes == 1 && !nav_queries && animations == 1);

    Reset();
    should_wander = true;
    moves[0] = AI_ACTION_FAILED;
    state.Update();
    Require(terminations == 1 && nav_queries == 1 && move_calls == 1 && !random_calls && !animations);

    Reset();
    should_wander = true;
    random_values[0] = 75;
    state.Update();
    Require(terminations == 1 && nav_queries == 1 && move_calls == 1 && random_calls == 1 && !animations);

    Reset();
    should_wander = true;
    state.Update();
    Require(terminations == 1 && nav_queries == 1 && move_calls == 1 && random_calls == 2 && !animations);

    Reset();
    should_wander = true;
    moves[0] = AI_ACTION_IN_PROGRESS;
    random_values[1] = 1;
    action_results[0] = AI_ACTION_IN_PROGRESS;
    state.Update();
    Require(terminations == 1 && move_calls == 2 && random_calls == 2 && animations == 2 && talks == 4);
    Require(strcmp(animation_log[0], "ST_IDLE") == 0 && strcmp(animation_log[1], "ST_IDLE") == 0);

    Reset();
    should_wander = true;
    random_values[1] = 1;
    action_results[0] = AI_ACTION_FAILED;
    state.Update();
    Require(terminations == 1 && move_calls == 1 && random_calls == 2 && animations == 1 && talks == 2);

    puts("00922860_TEST PASS");
    return 0;
}