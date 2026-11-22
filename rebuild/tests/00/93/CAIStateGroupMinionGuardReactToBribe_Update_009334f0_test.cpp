// CAIStateGroup_MinionGuardReactToBribe::Update
// Leaf: ?Update@CAIStateGroup_MinionGuardReactToBribe@@UAEXXZ
#define FABLE_RETAIL_GUARD_REACT_TO_BRIBE_PASS27
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_DEREFERENCE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_CONVERT_MUTABLE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_GET_ORIGINAL_CHAIN
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE_AIACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_SPEECH
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CTCVILLAGEMEMBER

#define FABLE_RETAIL_GUARD_BRIBE_FIXTURE_PASS27
#define FABLE_RETAIL_CAIBRAIN_CREATURE_REFERENCE
#define FABLE_RETAIL_CAIBRAIN_CTOR_PASS23
#define FABLE_RETAIL_SENSED_PASS23
#define FABLE_RETAIL_KID_EVADE_LIFETIMES_PASS23
#define FABLE_RETAIL_STLP_BITSET_INDEX
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define private public
#define protected public
#define FABLE_RETAIL_CINTELLIGENTPOINTER_CREATUREBASE_SIZE_PASS27
#define FABLE_RETAIL_CTHING_HASTC_SPEED_OWNER_PASS27
#include "fable_string.h"
#include "engine/CIntelligentPointer.h"
// Fidelity: scaffolding, not retail. Own the 28-byte deleting destructor
// at 0x004D6915 in size mode. The constructor and method below use the
// original speed mode restored by optimize("", on).
#include "engine/CIntelligentPointer_CreatureSizes.h"

#include "engine/CThingAICreature.h"
inline void CIntelligentPointer<CThingCreatureBase>::SetPointer(const CThingCreatureBase* pointer) {
    SetPItem(const_cast<CBaseObject*>(static_cast<const CBaseObject*>(pointer)));
}
inline CThingCreatureBase* CIntelligentPointer<CThingCreatureBase>::GetPointer() const {
    return static_cast<CThingCreatureBase*>(GetPItem());
}

inline bool CThing::HasTC(ETCInterfaceType type) const { return UsingComponents.test(type); }
inline CIntelligentPointer<CThingCreatureBase>::CIntelligentPointer(CThingCreatureBase* pointer) { SetPointer(pointer); }

#include "engine/CThingPlayerCreature.h"
#include "engine/CAIStateGroup_MinionGuardReactToBribe.h"
#include "engine/CAIBrain.h"
#include "engine/CTCGuard.h"
#include "engine/CTCVillageMember.h"
#include "engine/CMainGameComponent.h"
#include "engine/CWorld.h"
#include "engine/EAIAction.h"
#include "engine/EMovementType.h"
#include "engine/NCreatureAI.h"
#undef private
#undef protected

void CAIStateGroup_MinionGuardReactToBribe::Update()
{
    CTCGuard* ptc_guard = 0;
    GetCreature().GetTC(&ptc_guard);
    EAIActionReturn ret = AIAction_LookAtThing(ACTION_REPEAT_UNTIL_FINISHED, PHero, 0x13, 0.0027777f);
    if (ret == AI_ACTION_TERMINATE_PROCESS) {
        Terminate();
        return;
    }
    if (HasRecievedBribe) {
        long first_frame_to_react = 0;
        long last_frame_to_react = 0;
        long frame_to_react_again = FrameLastReactedToReceiveing + CMainGameComponent::GetConstantFPS() * 7;
        long current_frame = 0;
        do {
            first_frame_to_react = ptc_guard->GetFrameLastBribeAdded() + CMainGameComponent::GetConstantFPS() * 2;
            last_frame_to_react = ptc_guard->GetFrameLastBribeAdded() + CMainGameComponent::GetConstantFPS() * 4;
            current_frame = CWorld::GetFrame();
            if (current_frame > last_frame_to_react || ptc_guard->GetBribePool() < 0 || frame_to_react_again > last_frame_to_react)
                break;
            if (current_frame >= first_frame_to_react && current_frame >= frame_to_react_again) {
                ret = AIAction_SpeakAISpeechToTarget(ACTION_REPEAT_UNTIL_FINISHED, &GetCreature(), PHero,
                                                    "TEXT_AI_GUARD_RECEIVE_BRIBE", false, 0x93);
                FrameLastReactedToReceiveing = CWorld::GetFrame();
                break;
            }
            ret = AIAction_LookAtThing(ACTION_REPEAT_UNTIL_FINISHED, PHero, 0x13, 0.0027777f);
        } while (ret != AI_ACTION_TERMINATE_PROCESS);
    } else {
        ret = AIAction_SpeakAISpeechToTarget(ACTION_REPEAT_UNTIL_FINISHED, &GetCreature(), PHero,
                                            "TEXT_AI_GUARD_DEMAND_REBRIBE_FIRST", false, 0x93);
        if (ret == AI_ACTION_TERMINATE_PROCESS) {
            Terminate();
            return;
        }
        long frame_pending_crimes_added = ptc_guard->GetFramePendingCrimesAdded() - 5;
        long frame_to_stop_waiting = frame_pending_crimes_added + ptc_guard->GetWaitForRebribeInSeconds() * CMainGameComponent::GetConstantFPS();
        long frame_to_give_second_warning = (long)(frame_pending_crimes_added + (ptc_guard->GetWaitForRebribeInSeconds() * CMainGameComponent::GetConstantFPS()) / 2.0f);
        C3DVector position_of_player_when_started_waiting = PHero->GetPos();
        bool said_second_warning = false;
        do {
            if (ptc_guard->GetBribePool() >= 0) {
                ret = AIAction_SpeakAISpeechToTarget(ACTION_REPEAT_UNTIL_FINISHED, &GetCreature(), PHero,
                                                    "TEXT_AI_GUARD_GIVE_REBRIBE", false, 0x93);
                FrameLastReactedToReceiveing = CWorld::GetFrame();
                break;
            }
            if (NCreatureAI::IsThingRunningAwayFromThing(*PHero, GetCreature(), position_of_player_when_started_waiting)) {
                ret = AIAction_SpeakAISpeechToTarget(ACTION_REPEAT_UNTIL_FINISHED, &GetCreature(), PHero,
                                                    "TEXT_AI_GUARD_RUN_AWAY_WHILE_REBRIBE", false, 0x93);
                CTCVillageMember* ptc_village_member = 0;
                GetCreature().GetTC(&ptc_village_member);
                ptc_guard->ReportAllPendingCrimes(ptc_village_member->GetPVillage());
                break;
            }
            bool finished_calculating_route;
            ret = AIAction_MoveToThingProximity(ACTION_REPEAT_CALLBACK, PHero, 3.0f, finished_calculating_route,
                                                AI_MOVEMENT_WALKING, true, false, true, true, 0.0f);
            if (ret == AI_ACTION_FINISHED)
                ret = AIAction_LookAtThing(ACTION_REPEAT_CALLBACK, PHero, 0x13, 0.0027777f);
            if (!said_second_warning && ret == AI_ACTION_FINISHED && CWorld::GetFrame() >= frame_to_give_second_warning) {
                ret = AIAction_SpeakAISpeechToTarget(ACTION_REPEAT_UNTIL_FINISHED, &GetCreature(), PHero,
                                                    "TEXT_AI_GUARD_DEMAND_REBRIBE_SECOND", false, 0x93);
                said_second_warning = true;
            }
        } while (CWorld::GetFrame() < frame_to_stop_waiting && ret != AI_ACTION_TERMINATE_PROCESS);
        if (ret != AI_ACTION_TERMINATE_PROCESS && CWorld::GetFrame() >= frame_to_stop_waiting) {
            ret = AIAction_SpeakAISpeechToTarget(ACTION_REPEAT_UNTIL_FINISHED, &GetCreature(), PHero,
                                                "TEXT_AI_GUARD_IGNORE_REBRIBE", false, 0x93);
        }
    }
    Terminate();
}
#include <stdio.h>

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

CCharString::CCharString(const char* text, long length)
    : PStringData(reinterpret_cast<CCharStringData*>(const_cast<char*>(text))) { Require(length == -1); }
void CIVCountedPointeeBase::Delete() const { abort(); }
CAIStateGroupBase::CAIStateGroupBase() {}
CAIBrain::CAIBrain(const CMainGameComponent&, CWorld&, CThingManager&, CPlayerManager&,
    CThingAICreature& creature, CDefPointer<const CBrainDef>) : Creature(creature) {}
CThingPlayerCreature::CThingPlayerCreature() {}
CTCGuard::CTCGuard(CThing& thing) : CTCBase(thing) {}
CTCVillageMember::CTCVillageMember(CThing& thing) : CTCBase(thing) {}

class TestGuard : public CTCGuard {
public:
    TestGuard(CThing& thing) : CTCGuard(thing) {}
    ETCInterfaceType GetInterfaceType() const { return TCI_GUARD; }
    CCharString GetClassTypeName() const { return CCharString(); }
};
static CThingAICreature* speaker;
static CThingPlayerCreature* hero;
static TestGuard* guard;
static CTCVillageMember* member;
static CTCVillage* village;
static C3DVector hero_pos(3.0f, -7.0f, 4.0f);
static long simulation_frame, frame_after_look, frame_after_move[3];
static bool fleeing, offer_after_move;
static unsigned looks, moves, speeches, reports, village_queries, terminations;
static EAIActionReturn look_result[4], move_result[3], speech_result[4];
static const char* speech_log[4];
long CMainGameComponent::ConstantFPS = 10;
static const char* thank = "TEXT_AI_GUARD_RECEIVE_BRIBE";
static const char* demand = "TEXT_AI_GUARD_DEMAND_REBRIBE_FIRST";
static const char* accept = "TEXT_AI_GUARD_GIVE_REBRIBE";
static const char* escaped = "TEXT_AI_GUARD_RUN_AWAY_WHILE_REBRIBE";
static const char* remind = "TEXT_AI_GUARD_DEMAND_REBRIBE_SECOND";
static const char* ignore = "TEXT_AI_GUARD_IGNORE_REBRIBE";

long __fastcall CWorld::GetFrame() { return simulation_frame; }
const C3DVector& CThing::GetPos() const { Require(this == hero); return hero_pos; }
long CTCGuard::GetWaitForRebribeInSeconds() const { Require(this == guard); return 15; }
CTCVillage* CTCVillageMember::GetPVillage() const { Require(this == member); ++village_queries; return village; }
void CTCGuard::ReportAllPendingCrimes(CTCVillage* owner)
{
    Require(this == guard && owner == village && speeches == 2 && strcmp(speech_log[1], escaped) == 0);
    ++reports;
}
bool __fastcall NCreatureAI::IsThingRunningAwayFromThing(const CThing& player, const CThing& from, const C3DVector& old_pos)
{
    Require(&player == hero && &from == speaker);
    Require(old_pos.X == 3.0f && old_pos.Y == -7.0f && old_pos.Z == 4.0f);
    return fleeing;
}
void CAIStateGroupBase::Terminate() { ++terminations; }
EAIActionReturn CAIStateGroupBase::AIAction_LookAtThing(EAIActionRepeat repeat, CIntelligentPointer<const CThing> thing,
    long flags, float speed)
{
    Require(thing.Get() == hero && flags == 0x13 && speed == 0.0027777f && looks < 4);
    Require(repeat == (moves ? ACTION_REPEAT_CALLBACK : ACTION_REPEAT_UNTIL_FINISHED));
    if (looks == 1 && frame_after_look >= 0) simulation_frame = frame_after_look;
    return look_result[looks++];
}
EAIActionReturn CAIStateGroupBase::AIAction_MoveToThingProximity(EAIActionRepeat repeat,
    CIntelligentPointer<const CThing> thing, float proximity, bool& nav, EMovementType movement,
    bool a, bool b, bool c, bool d, float speed)
{
    Require(repeat == ACTION_REPEAT_CALLBACK && thing.Get() == hero && proximity == 3.0f);
    Require(movement == AI_MOVEMENT_WALKING && a && !b && c && d && speed == 0.0f && moves < 3);
    if (frame_after_move[moves] >= 0) simulation_frame = frame_after_move[moves];
    if (offer_after_move) guard->BribePool = 0;
    nav = true;
    return move_result[moves++];
}
EAIActionReturn CAIStateGroupBase::AIAction_SpeakAISpeechToTarget(EAIActionRepeat repeat,
    CIntelligentPointer<CThingCreatureBase> creature, CIntelligentPointer<const CThing> recipient,
    const CCharString& speech, bool interrupt, long priority)
{
    Require(repeat == ACTION_REPEAT_UNTIL_FINISHED && creature.Get() == speaker && recipient.Get() == hero);
    Require(!interrupt && priority == 0x93 && speeches < 4);
    speech_log[speeches] = reinterpret_cast<const char*>(speech.PStringData);
    return speech_result[speeches++];
}
static void Put(CThing& thing, ETCInterfaceType key, CTCBase* component)
{
    thing.UsingComponents[key] = true;
    thing.Components.push_back(std::pair<ETCInterfaceType, CTCBase*>(key, component));
}
static void Reset(CAIStateGroup_MinionGuardReactToBribe& state, bool paid, long frame)
{
    state.HasRecievedBribe = paid;
    state.FrameLastReactedToReceiveing = 10;
    guard->BribePool = paid ? 2 : -1;
    guard->FramePendingCrimesAdded = 105;
    guard->FrameLastBribeAdded = 100;
    simulation_frame = frame;
    frame_after_look = -1;
    fleeing = offer_after_move = false;
    looks = moves = speeches = reports = village_queries = terminations = 0;
    for (int i=0; i<4; ++i) look_result[i] = speech_result[i] = AI_ACTION_FINISHED;
    for (int i=0; i<3; ++i) {
        move_result[i] = AI_ACTION_FINISHED;
        frame_after_move[i] = -1;
    }
}
static void Check(CAIStateGroup_MinionGuardReactToBribe& state, unsigned look, unsigned move, unsigned report,
    const char* first=0, const char* second=0, const char* third=0)
{
    state.Update();
    Require(looks == look && moves == move && reports == report && village_queries == report && terminations == 1);
    Require(speeches == unsigned((first != 0) + (second != 0) + (third != 0)));
    if (first) Require(strcmp(speech_log[0], first) == 0);
    if (second) Require(strcmp(speech_log[1], second) == 0);
    if (third) Require(strcmp(speech_log[2], third) == 0);
}
int main()
{
    CThingAICreature actor;
    CThingPlayerCreature player;
    TestGuard guard_component(actor);
    CTCVillageMember village_member(actor);
    static unsigned __int64 game_storage[4], world_storage[4], manager_storage[4], player_storage[4], village_storage[4];
    village = reinterpret_cast<CTCVillage*>(village_storage);
    CAIBrain brain(*reinterpret_cast<CMainGameComponent*>(game_storage), *reinterpret_cast<CWorld*>(world_storage),
        *reinterpret_cast<CThingManager*>(manager_storage), *reinterpret_cast<CPlayerManager*>(player_storage),
        actor, CDefPointer<const CBrainDef>());
    CAIStateGroup_MinionGuardReactToBribe state;
    state.PBrain = &brain;
    state.PHero.SetPItem(&player);
    speaker = &actor; hero = &player; guard = &guard_component; member = &village_member;
    memset(actor.UsingComponents._M_w, 0, sizeof(actor.UsingComponents._M_w));
    Put(actor, TCI_VILLAGE_MEMBER, &village_member);
    Put(actor, TCI_GUARD, &guard_component);

    Reset(state, true, 120); look_result[0] = AI_ACTION_TERMINATE_PROCESS; Check(state, 1, 0, 0);
    Reset(state, false, 120); actor.UsingComponents[TCI_GUARD] = false;
    look_result[0] = AI_ACTION_TERMINATE_PROCESS; Check(state, 1, 0, 0); actor.UsingComponents[TCI_GUARD] = true;
    Reset(state, true, 120); Check(state, 1, 0, 0, thank); Require(state.FrameLastReactedToReceiveing == 120);
    Reset(state, true, 119); frame_after_look = 120; Check(state, 2, 0, 0, thank);
    Reset(state, true, 140); Check(state, 1, 0, 0, thank);
    Reset(state, true, 141); Check(state, 1, 0, 0);
    Reset(state, true, 120); guard->BribePool = -1; Check(state, 1, 0, 0);
    Reset(state, true, 120); state.FrameLastReactedToReceiveing = 71; Check(state, 1, 0, 0);
    Reset(state, true, 118); look_result[1] = AI_ACTION_TERMINATE_PROCESS; Check(state, 2, 0, 0);
    Reset(state, false, 110); speech_result[0] = AI_ACTION_TERMINATE_PROCESS; Check(state, 1, 0, 0, demand);
    Reset(state, false, 110); offer_after_move = true; frame_after_move[0] = 111;
    Check(state, 2, 1, 0, demand, accept); Require(state.FrameLastReactedToReceiveing == 111);
    Reset(state, false, 111); fleeing = true; Check(state, 1, 0, 1, demand, escaped);
    Reset(state, false, 250); fleeing = true; Check(state, 1, 0, 1, demand, escaped, ignore);
    Reset(state, false, 111); move_result[0] = AI_ACTION_TERMINATE_PROCESS; Check(state, 1, 1, 0, demand);
    Reset(state, false, 111); frame_after_move[0] = 175; frame_after_move[1] = 250; Check(state, 3, 2, 0, demand, remind, ignore);
    Reset(state, false, 111); frame_after_move[0] = 251; move_result[0] = AI_ACTION_IN_PROGRESS; Check(state, 1, 1, 0, demand, ignore);
    Reset(state, false, 111); look_result[1] = AI_ACTION_TERMINATE_PROCESS; Check(state, 2, 1, 0, demand);
    puts("009334f0_TEST PASS");
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
void CAIStateGroup_MinionGuardReactToBribe::InitStateGroup(const CWorld&, CThingAICreature&) { fprintf(stderr, "unexpected CAIStateGroup_MinionGuardReactToBribe::InitStateGroup\n"); abort(); }
bool CAIStateGroup_MinionGuardReactToBribe::IsRunnable() { fprintf(stderr, "unexpected CAIStateGroup_MinionGuardReactToBribe::IsRunnable\n"); abort(); }
bool CAIStateGroup_MinionGuardReactToBribe::IsStillValid() const { fprintf(stderr, "unexpected CAIStateGroup_MinionGuardReactToBribe::IsStillValid\n"); abort(); }
void CThingGameObject::OnHit(CHitParameters&) { fprintf(stderr, "unexpected CThingGameObject::OnHit\n"); abort(); }
void CThingCreatureBase::OnHit(CHitParameters&) { fprintf(stderr, "unexpected CThingCreatureBase::OnHit\n"); abort(); }
bool CThingCreatureBase::DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo& info) { fprintf(stderr, "unexpected CThingCreatureBase::DrawPreparePrimitivesForRendering\n"); abort(); }
bool CThingPlayerCreature::DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo& info) { fprintf(stderr, "unexpected CThingPlayerCreature::DrawPreparePrimitivesForRendering\n"); abort(); }
bool CThingPlayerCreature::SetActionForMelee(const CCreatureAbilityDef& ability) { fprintf(stderr, "unexpected CThingPlayerCreature::SetActionForMelee\n"); abort(); }
void CTCVillageMember::OnCreate() { fprintf(stderr, "unexpected CTCVillageMember::OnCreate\n"); abort(); }
void CTCVillageMember::OnInitialActivate() { fprintf(stderr, "unexpected CTCVillageMember::OnInitialActivate\n"); abort(); }
void CTCVillageMember::OnKill() { fprintf(stderr, "unexpected CTCVillageMember::OnKill\n"); abort(); }
void CTCVillageMember::OnSerialise(CPersistContext&, bool) { fprintf(stderr, "unexpected CTCVillageMember::OnSerialise\n"); abort(); }
ETCInterfaceType CTCVillageMember::GetInterfaceType() const { fprintf(stderr, "unexpected CTCVillageMember::GetInterfaceType\n"); abort(); }
CCharString CTCVillageMember::GetClassTypeName() const { fprintf(stderr, "unexpected CTCVillageMember::GetClassTypeName\n"); abort(); }
bool CTCVillageMember::IsComponentSerialisable() const { fprintf(stderr, "unexpected CTCVillageMember::IsComponentSerialisable\n"); abort(); }