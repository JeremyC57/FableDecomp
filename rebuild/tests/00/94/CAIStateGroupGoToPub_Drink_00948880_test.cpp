// CAIStateGroup_GoToPub::Drink()
// Leaf: ?Drink@CAIStateGroup_GoToPub@@IAE_NXZ
// Flags: default
#define FABLE_RETAIL_GOTOPUB_DRINK_PASS27
#define FABLE_RETAIL_CAISTATEGROUP_GOTOPUB
#define FABLE_RETAIL_CAISTATEGROUP_GOTOPUB_UPDATE_PASS24
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_DEREFERENCE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CINTELLIGENTPOINTER_GET_ORIGINAL_CHAIN
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE_GOTOBED_WORLD
#define FABLE_RETAIL_CAISTATEGROUPBASE_PERFORMACTION
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTION_PLAYANIMATION
#define FABLE_RETAIL_CCREATUREACTION_PLAYANIMATION_IMPLICIT_DTOR
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCTAVERNGAME
#define FABLE_RETAIL_CTCTALK
#define FABLE_RETAIL_GREETING_PASS23
#define FABLE_RETAIL_CTCCREATUREMODEMANAGER
#define FABLE_RETAIL_CSOUNDPAIR_ORIGINAL_CTORS
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_NSPEECHGAINMANAGER_ACTIVEDIALOGUEINFO
#define FABLE_RETAIL_DRINK_FIXTURE_PASS27
#define FABLE_RETAIL_CAIBRAIN_CREATURE_REFERENCE
#define FABLE_RETAIL_CAIBRAIN_CTOR_PASS23
#define FABLE_RETAIL_CCREATUREACTIONBASE_ANIMCTOR
#define FABLE_RETAIL_CTCCREATUREMODEMANAGER_CTOR_PASS23
#define FABLE_RETAIL_STLP_BITSET_INDEX
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define private public
#define protected public
#include "engine/CThingAICreature.h"
#include "engine/CThingObject.h"
#include "engine/CAIStateGroup_GoToPub.h"
#include "engine/CAIBrain.h"
#include "engine/CMainGameComponent.h"
#include "engine/CWorld.h"
#include "engine/ThingAlive.h"
#include "engine/EAIAction.h"
#include "engine/CTCTavernTankard.h"
#include "engine/CTCTavernGame.h"
#include "engine/CTCAIScratchpad.h"
#include "engine/CTCTalk.h"
#include "engine/NSpeechGainManager.h"
#include "engine/CTCDrunkenness.h"
#include "engine/CTCCreatureModeManager.h"
#include "engine/CCreatureAction_Tankard.h"
#include "engine/CCreatureAction_PlayAnimation.h"
#include "engine/CSoundPair.h"
#include "engine/GFRandom.h"
#include "engine/CAnimationEntry.h"
#undef private
#undef protected

bool CAIStateGroup_GoToPub::Drink()
{
    if (!IS_THING_ALIVE(PTankard))
        return false;

    CTCTavernTankard* tc_tavern_tankard;
    PTankard->GetTC(&tc_tavern_tankard);
    if (tc_tavern_tankard->Fullness == 0.0f) {
        if (!tc_tavern_tankard->GetBarmaid() && TimeBeforeAskingForBeer <= GetWorld().GetFrame()) {
            static long time_to_next_ask_for_beer = 20;
            TimeBeforeAskingForBeer = GetWorld().GetFrame() + CMainGameComponent::GetConstantFPS() * time_to_next_ask_for_beer;
            CTCTalk* tc_talk;
            GetCreature().GetTC(&tc_talk);
            CTCAIScratchpad* tc_ai_scratchpad;
            GetCreature().GetTC(&tc_ai_scratchpad);
            CTCTalk::ESpeechPlaybackResult ret;
            do {
                ret = tc_talk->PlayAISpeech(tc_ai_scratchpad->GetBeerRequestPhrase(), false, NSpeechGainManager::DIALOGUE_LAYER_BACKGROUND, false, true);
                if (ret == CTCTalk::SPEECH_PLAYBACK_FAILED_RETRY) {
                    if (AIAction_DoNothing() != AI_ACTION_IN_PROGRESS)
                        return false;
                }
            } while (ret == CTCTalk::SPEECH_PLAYBACK_FAILED_RETRY);
            if (ret == CTCTalk::SPEECH_PLAYBACK_FAILED_FINAL
                || AIAction_PerformActionAndWait(CCreatureAction_PlayAnimation(GetCreature(), "SHOUT_FOR_BEER",
                    CSoundPair(0), false, true, false, 0, 0), false) != AI_ACTION_FINISHED)
                return false;
            return true;
        } else {
            return AIAction_DoNothing() != AI_ACTION_TERMINATE_PROCESS;
        }
    }

    if (AIAction_PerformActionAndWait(CCreatureAction_PickUpTankard(GetCreature(), *PTankard), false) != AI_ACTION_FINISHED) {
        CTCCreatureModeManager* ptc_mode_manager;
        GetCreature().GetTC(&ptc_mode_manager);
        if (ptc_mode_manager->IsModeActive(NCreatureMode::MODE_DRINKING_FROM_TANKARD))
            ptc_mode_manager->RemoveMode(NCreatureMode::MODE_DRINKING_FROM_TANKARD);
        return false;
    }

    long secs_to_drink = GFRandom(9, GetPWorldSeed()) + 2;
    long drink_until = CWorld::GetFrame() + CMainGameComponent::GetConstantFPS() * secs_to_drink;
    CTCTavernGame* tc_tavern_game = 0;
    PTable->GetTC(&tc_tavern_game);
    do {
        if (AIAction_DoNothing() != AI_ACTION_IN_PROGRESS) {
            CTCCreatureModeManager* ptc_mode_manager;
            GetCreature().GetTC(&ptc_mode_manager);
            if (ptc_mode_manager->IsModeActive(NCreatureMode::MODE_DRINKING_FROM_TANKARD))
                ptc_mode_manager->RemoveMode(NCreatureMode::MODE_DRINKING_FROM_TANKARD);
            return false;
        }
    } while (drink_until > CWorld::GetFrame() && (!tc_tavern_game || !tc_tavern_game->IsGameBeingPlayed()));

    tc_tavern_tankard->Fullness -= secs_to_drink * 0.1f;
    CTCDrunkenness* tc_drunkenness;
    GetCreature().GetTC(&tc_drunkenness);
    tc_drunkenness->DrankSecs((float)secs_to_drink);
    if (tc_tavern_tankard->Fullness < 0.0f)
        tc_tavern_tankard->Fullness = 0.0f;

    if (AIAction_PerformActionAndWait(CCreatureAction_PutDownTankard(GetCreature()), false) != AI_ACTION_FINISHED) {
        CTCCreatureModeManager* ptc_mode_manager;
        GetCreature().GetTC(&ptc_mode_manager);
        if (ptc_mode_manager->IsModeActive(NCreatureMode::MODE_DRINKING_FROM_TANKARD))
            ptc_mode_manager->RemoveMode(NCreatureMode::MODE_DRINKING_FROM_TANKARD);
        return false;
    }
    return true;
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
CWideString::CWideString() {}
CWideString::~CWideString() {}
void CIVCountedPointeeBase::Delete() const { abort(); }

CAIStateGroupBase::CAIStateGroupBase() {}
CAIBrain::CAIBrain(const CMainGameComponent&, CWorld&, CThingManager&, CPlayerManager&,
    CThingAICreature& creature, CDefPointer<const CBrainDef>) : Creature(creature) {}
CAIStateGroup_GoToPub::CAIStateGroup_GoToPub() {}
CTCTavernTankard::CTCTavernTankard(CThing& thing) : CTCBase(thing) {}
CTCTavernGame::CTCTavernGame(CThing& thing) : CTCBase(thing) {}
CTCTavernGame::~CTCTavernGame() {}
CTCTalk::CTCTalk(CThing& thing) : CTCBase(thing) {}
CTCAIScratchpad::CTCAIScratchpad(CThing& thing) : CTCBase(thing) {}
CTCCreatureModeManager::CTCCreatureModeManager(CThing& thing) : CTCBase(thing) {}
CTCCreatureModeManager::~CTCCreatureModeManager() {}
CTCDrunkenness::CTCDrunkenness(CThing& thing) : CTCBase(thing), SecondsPerFrame(1.0f / 3.0f) {}
const float CTCDrunkenness::DrunkennessDeltaPerSecondOfDrinking = 0.1f;

class TestTavernGame : public CTCTavernGame {
public:
    TestTavernGame(CThing& thing) : CTCTavernGame(thing) {}
    CCharString GetClassTypeName() const { return CCharString(); }
    CWideString GetBestScore() { abort(); }
    CWideString GetCurrentScore() { abort(); }
    CWideString GetRequiredScore() { abort(); }
};
class TestTalk : public CTCTalk {
public:
    TestTalk(CThing& thing) : CTCTalk(thing) {}
    ETCInterfaceType GetInterfaceType() const { return TCI_TALK; }
    CCharString GetClassTypeName() const { return CCharString(); }
};
class TestScratch : public CTCAIScratchpad {
public:
    TestScratch(CThing& thing) : CTCAIScratchpad(thing) {}
    ETCInterfaceType GetInterfaceType() const { return TCI_AI_SCRATCHPAD; }
    CCharString GetClassTypeName() const { return CCharString(); }
};
class TestModeManager : public CTCCreatureModeManager {
public:
    TestModeManager(CThing& thing) : CTCCreatureModeManager(thing) {}
    ETCInterfaceType GetInterfaceType() const { return TCI_ENTITY_MODE_MANAGER; }
    CCharString GetClassTypeName() const { return CCharString(); }
};
class TestDrunkenness : public CTCDrunkenness {
public:
    TestDrunkenness(CThing& thing) : CTCDrunkenness(thing) {}
    ETCInterfaceType GetInterfaceType() const { return TCI_DRUNKENNESS; }
    CCharString GetClassTypeName() const { return CCharString(); }
};

enum Action { SHOUT, PICK_UP, PUT_DOWN };
static long frame;
static CThingAICreature* creature;
static CThingAICreature* barmaid;
static CThing* tankard_thing;
static CTCTavernTankard* tankard;
static CThingObject* table;
static TestTavernGame* game;
static TestTalk* talk;
static TestModeManager* mode_manager;
static TestDrunkenness* drunk;
static CWorld* world;
static CAnimationEntry anim;
static unsigned long seed;
static int waits, speeches, actions, mode_removed, rotations;
static bool mode_active;
static EAIActionReturn idle_result, shout_result, pickup_result, putdown_result;
static CTCTalk::ESpeechPlaybackResult speech_results[2];
static const CCreatureActionBase* last_action;
static Action action_kind;
long CMainGameComponent::ConstantFPS = 3;

CCreatureActionBase::CCreatureActionBase(CThingCreatureBase&, float, float,
    const CAnimationEntry&, const CSoundPair&, long, long) {}
CCreatureActionBase::~CCreatureActionBase() {}
CCreatureAction_PlayAnimation::CCreatureAction_PlayAnimation(CThingCreatureBase& owner,
    const CCharString& name, const CSoundPair& sound, bool a, bool b, bool c, long x, long y)
    : CCreatureActionBase(owner, 0.0f, 0.0f, anim, sound, x, y)
{
    Require(&owner == creature && !a && b && !c && x == 0 && y == 0 && sound.FirstSound == 0 && sound.LastSound == 0);
    Require(strcmp(reinterpret_cast<const char*>(name.PStringData), "SHOUT_FOR_BEER") == 0);
    last_action = this;
    action_kind = SHOUT;
}
CCreatureAction_PickUpThingAtTable::CCreatureAction_PickUpThingAtTable(CThingCreatureBase& owner,
    CThing& item, const CCharString&, NCreatureMode::EMode mode)
    : CCreatureActionBase(owner, 0.0f, 0.0f, anim, CSoundPair(0), 0, 0), PThingToPickUp(&item), Mode(mode) {}
CCreatureAction_PutDownThingAtTable::CCreatureAction_PutDownThingAtTable(CThingCreatureBase& owner,
    const CCharString&, NCreatureMode::EMode mode)
    : CCreatureActionBase(owner, 0.0f, 0.0f, anim, CSoundPair(0), 0, 0), Mode(mode) {}
CCreatureAction_PickUpTankard::CCreatureAction_PickUpTankard(CThingCreatureBase& owner, CThing& item)
    : CCreatureAction_PickUpThingAtTable(owner, item, CCharString(), NCreatureMode::MODE_DRINKING_FROM_TANKARD)
{
    Require(&owner == creature && &item == tankard_thing);
    last_action = this;
    action_kind = PICK_UP;
}
CCreatureAction_PutDownTankard::CCreatureAction_PutDownTankard(CThingCreatureBase& owner)
    : CCreatureAction_PutDownThingAtTable(owner, CCharString(), NCreatureMode::MODE_DRINKING_FROM_TANKARD)
{
    Require(&owner == creature);
    last_action = this;
    action_kind = PUT_DOWN;
}
EAIActionReturn CAIStateGroupBase::AIAction_PerformActionAndWait(const CCreatureActionBase& action, bool wait)
{
    Require(&action == last_action && !wait);
    ++actions;
    if (action_kind == SHOUT)
        return shout_result;
    return action_kind == PICK_UP ? pickup_result : putdown_result;
}
EAIActionReturn CAIStateGroupBase::AIAction_DoNothing()
{
    ++waits;
    ++frame;
    return idle_result;
}
CWorld& CAIStateGroupBase::GetWorld() const { return *world; }
unsigned long* CAIStateGroupBase::GetPWorldSeed() const { return &seed; }
long __fastcall CWorld::GetFrame() { return frame; }
long __fastcall GFROR13(unsigned long value)
{
    ++rotations;
    return (value >> 13) | (value << 19);
}
bool CTCCreatureModeManager::IsModeActive(unsigned long mode) const
{
    Require(this == mode_manager && mode == NCreatureMode::MODE_DRINKING_FROM_TANKARD);
    return mode_active;
}
void CTCCreatureModeManager::RemoveMode(unsigned long mode)
{
    Require(this == mode_manager && mode == NCreatureMode::MODE_DRINKING_FROM_TANKARD);
    ++mode_removed;
}
CTCTalk::ESpeechPlaybackResult CTCTalk::PlayAISpeech(unsigned long group, bool a, NSpeechGainManager::EDialogueLayer layer, bool b, bool c)
{
    Require(this == talk && group == 400 && !a && !b && c && layer == NSpeechGainManager::DIALOGUE_LAYER_BACKGROUND && speeches < 2);
    return speech_results[speeches++];
}

static void Put(CThing& thing, ETCInterfaceType key, CTCBase* component)
{
    thing.UsingComponents[key] = true;
    thing.Components.push_back(std::pair<ETCInterfaceType, CTCBase*>(key, component));
}
static void Reset(CAIStateGroup_GoToPub& state)
{
    state.PTankard.SetPItem(tankard_thing);
    state.PTable.SetPItem(table);
    state.TimeBeforeAskingForBeer = 0;
    frame = 100;
    seed = 18;
    tankard_thing->Dead = false;
    tankard->Fullness = 1.0f;
    tankard->PBarmaid.SetPItem(0);
    drunk->Drunkenness = 0.0f;
    game->IsBeingPlayed = false;
    mode_active = true;
    waits = speeches = actions = mode_removed = rotations = 0;
    idle_result = AI_ACTION_IN_PROGRESS;
    shout_result = pickup_result = putdown_result = AI_ACTION_FINISHED;
    speech_results[0] = CTCTalk::SPEECH_PLAYBACK_STARTED;
    speech_results[1] = CTCTalk::SPEECH_PLAYBACK_STARTED;
}
static void Check(CAIStateGroup_GoToPub& state, bool result, int wait_count, int speech_count, int action_count, int remove_count, int random_count)
{
    Require(state.Drink() == result);
    Require(waits == wait_count && speeches == speech_count && actions == action_count &&
        mode_removed == remove_count && random_count == rotations);
}
int main()
{
    CThingAICreature me, server;
    CThing item;
    CThingObject pub_table;
    CTCTavernTankard tankard_component(item);
    TestTavernGame game_component(pub_table);
    TestTalk talk_component(me);
    TestScratch scratch(me);
    TestModeManager modes(me);
    TestDrunkenness drunkenness(me);
    static unsigned __int64 game_storage[4], world_storage[4], manager_storage[4], player_storage[4];
    world = reinterpret_cast<CWorld*>(world_storage);
    CAIBrain brain(*reinterpret_cast<CMainGameComponent*>(game_storage), *world,
        *reinterpret_cast<CThingManager*>(manager_storage), *reinterpret_cast<CPlayerManager*>(player_storage),
        me, CDefPointer<const CBrainDef>());
    CAIStateGroup_GoToPub state;
    state.PBrain = &brain;
    creature = &me;
    barmaid = &server;
    tankard_thing = &item;
    tankard = &tankard_component;
    table = &pub_table;
    game = &game_component;
    talk = &talk_component;
    mode_manager = &modes;
    drunk = &drunkenness;
    scratch.BeerRequestPhrase = 400;
    memset(me.UsingComponents._M_w, 0, sizeof(me.UsingComponents._M_w));
    memset(item.UsingComponents._M_w, 0, sizeof(item.UsingComponents._M_w));
    memset(pub_table.UsingComponents._M_w, 0, sizeof(pub_table.UsingComponents._M_w));
    Put(me, TCI_TALK, &talk_component);
    Put(me, TCI_ENTITY_MODE_MANAGER, &modes);
    Put(me, TCI_AI_SCRATCHPAD, &scratch);
    Put(me, TCI_DRUNKENNESS, &drunkenness);
    Put(item, TCI_TAVERN_TANKARD, &tankard_component);
    Put(pub_table, TCI_TAVERN_GAME, &game_component);

    Reset(state); state.PTankard.SetPItem(0); Check(state, false, 0, 0, 0, 0, 0);
    Reset(state); item.Dead = true; Check(state, false, 0, 0, 0, 0, 0);
    Reset(state); tankard->Fullness = 0.0f; tankard->PBarmaid.SetPItem(barmaid); Check(state, true, 1, 0, 0, 0, 0);
    Reset(state); tankard->Fullness = 0.0f; state.TimeBeforeAskingForBeer = 120;
    idle_result = AI_ACTION_TERMINATE_PROCESS; Check(state, false, 1, 0, 0, 0, 0);
    Reset(state); tankard->Fullness = 0.0f; Check(state, true, 0, 1, 1, 0, 0);
    Require(state.TimeBeforeAskingForBeer == 160);
    Reset(state); tankard->Fullness = 0.0f; speech_results[0] = CTCTalk::SPEECH_PLAYBACK_FAILED_RETRY;
    Check(state, true, 1, 2, 1, 0, 0);
    Reset(state); tankard->Fullness = 0.0f; speech_results[0] = CTCTalk::SPEECH_PLAYBACK_FAILED_FINAL;
    Check(state, false, 0, 1, 0, 0, 0);
    Reset(state); tankard->Fullness = 0.0f; speech_results[0] = CTCTalk::SPEECH_PLAYBACK_FAILED_RETRY;
    idle_result = AI_ACTION_FAILED; Check(state, false, 1, 1, 0, 0, 0);
    Reset(state); tankard->Fullness = 0.0f; shout_result = AI_ACTION_FAILED; Check(state, false, 0, 1, 1, 0, 0);

    Reset(state); pickup_result = AI_ACTION_FAILED; Check(state, false, 0, 0, 1, 1, 0);
    Reset(state); idle_result = AI_ACTION_TERMINATE_PROCESS; Check(state, false, 1, 0, 1, 1, 1);
    Reset(state);
    const unsigned long stepped = 18 * 0x24a1U + 0x24df;
    const unsigned long seconds = ((stepped >> 13) | (stepped << 19)) % 9 + 2;
    Check(state, true, static_cast<int>(seconds * 3), 0, 2, 0, 1);
    Require(tankard->Fullness <= 1.001f - seconds * 0.1f && tankard->Fullness >= 0.999f - seconds * 0.1f);
    Require(drunk->Drunkenness <= seconds * 0.1f + 0.001f && drunk->Drunkenness >= seconds * 0.1f - 0.001f);
    Reset(state); game->IsBeingPlayed = true; tankard->Fullness = 0.05f;
    Check(state, true, 1, 0, 2, 0, 1);
    Require(tankard->Fullness == 0.0f && drunk->Drunkenness > 0.0f);
    Reset(state); game->IsBeingPlayed = true; putdown_result = AI_ACTION_FAILED; Check(state, false, 1, 0, 2, 1, 1);
    Reset(state); mode_active = false; pickup_result = AI_ACTION_FAILED; Check(state, false, 0, 0, 1, 0, 0);
    puts("00948880_TEST PASS");
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
bool CAIStateGroup_GoToPub::IsRunnable() { fprintf(stderr, "unexpected CAIStateGroup_GoToPub::IsRunnable\n"); abort(); }
void CAIStateGroup_GoToPub::Update() { fprintf(stderr, "unexpected CAIStateGroup_GoToPub::Update\n"); abort(); }
void CThingGameObject::OnHit(CHitParameters&) { fprintf(stderr, "unexpected CThingGameObject::OnHit\n"); abort(); }
void CThingCreatureBase::OnHit(CHitParameters&) { fprintf(stderr, "unexpected CThingCreatureBase::OnHit\n"); abort(); }
bool CThingCreatureBase::DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo& info) { fprintf(stderr, "unexpected CThingCreatureBase::DrawPreparePrimitivesForRendering\n"); abort(); }
void CCreatureActionBase::EditUpdate(float) { fprintf(stderr, "unexpected CCreatureActionBase::EditUpdate\n"); abort(); }
void CCreatureActionBase::FinishAction() { fprintf(stderr, "unexpected CCreatureActionBase::FinishAction\n"); abort(); }
bool CCreatureActionBase::IsPlayingAnimation() const { fprintf(stderr, "unexpected CCreatureActionBase::IsPlayingAnimation\n"); abort(); }
long CCreatureActionBase::GetAnimDelayTime() const { fprintf(stderr, "unexpected CCreatureActionBase::GetAnimDelayTime\n"); abort(); }
long CCreatureActionBase::GetAnimationIndex() const { fprintf(stderr, "unexpected CCreatureActionBase::GetAnimationIndex\n"); abort(); }
long CCreatureActionBase::GetAnimationTransitionInTime() const { fprintf(stderr, "unexpected CCreatureActionBase::GetAnimationTransitionInTime\n"); abort(); }
long CCreatureActionBase::GetAnimationTransitionOutTime() const { fprintf(stderr, "unexpected CCreatureActionBase::GetAnimationTransitionOutTime\n"); abort(); }
unsigned long CCreatureActionBase::GetAnimExtraFlags() const { fprintf(stderr, "unexpected CCreatureActionBase::GetAnimExtraFlags\n"); abort(); }
void CCreatureActionBase::Initialise() { fprintf(stderr, "unexpected CCreatureActionBase::Initialise\n"); abort(); }
bool CCreatureActionBase::IsStrikeEvent(long) const { fprintf(stderr, "unexpected CCreatureActionBase::IsStrikeEvent\n"); abort(); }
bool CCreatureActionBase::IsStrikeEventActive() const { fprintf(stderr, "unexpected CCreatureActionBase::IsStrikeEventActive\n"); abort(); }
bool CCreatureActionBase::IsStrikeAction() const { fprintf(stderr, "unexpected CCreatureActionBase::IsStrikeAction\n"); abort(); }
bool CCreatureActionBase::IsPartOfComboChain() const { fprintf(stderr, "unexpected CCreatureActionBase::IsPartOfComboChain\n"); abort(); }
bool CCreatureActionBase::IsStrikeEventFinishedBeingActive() const { fprintf(stderr, "unexpected CCreatureActionBase::IsStrikeEventFinishedBeingActive\n"); abort(); }
bool CCreatureActionBase::IsVitalStrike() const { fprintf(stderr, "unexpected CCreatureActionBase::IsVitalStrike\n"); abort(); }
bool CCreatureActionBase::IsKnockdownStrike() const { fprintf(stderr, "unexpected CCreatureActionBase::IsKnockdownStrike\n"); abort(); }
bool CCreatureActionBase::IsBlockCounter() const { fprintf(stderr, "unexpected CCreatureActionBase::IsBlockCounter\n"); abort(); }
bool CCreatureActionBase::IsMeleeFlourish() const { fprintf(stderr, "unexpected CCreatureActionBase::IsMeleeFlourish\n"); abort(); }
bool CCreatureActionBase::IsActionWithStrikeEventHelperName() const { fprintf(stderr, "unexpected CCreatureActionBase::IsActionWithStrikeEventHelperName\n"); abort(); }
CCharString CCreatureActionBase::GetStrikeEventHelperName() const { fprintf(stderr, "unexpected CCreatureActionBase::GetStrikeEventHelperName\n"); abort(); }
bool CCreatureActionBase::IsOverrideableByActionOfSamePriority() const { fprintf(stderr, "unexpected CCreatureActionBase::IsOverrideableByActionOfSamePriority\n"); abort(); }
void CCreatureActionBase::OnFrameUpdate(float) { fprintf(stderr, "unexpected CCreatureActionBase::OnFrameUpdate\n"); abort(); }
bool CCreatureActionBase::IsQueuable() const { fprintf(stderr, "unexpected CCreatureActionBase::IsQueuable\n"); abort(); }
bool CCreatureActionBase::IsActionReadyForInterruption() const { fprintf(stderr, "unexpected CCreatureActionBase::IsActionReadyForInterruption\n"); abort(); }
bool CCreatureActionBase::IsActionReadyForInterruptionByMovement(const C3DVector &) const { fprintf(stderr, "unexpected CCreatureActionBase::IsActionReadyForInterruptionByMovement\n"); abort(); }
bool CCreatureActionBase::IsBlockable() const { fprintf(stderr, "unexpected CCreatureActionBase::IsBlockable\n"); abort(); }
bool CCreatureActionBase::IsAffectedByHaste() const { fprintf(stderr, "unexpected CCreatureActionBase::IsAffectedByHaste\n"); abort(); }
bool CCreatureActionBase::IsAffectedByHeroStats() const { fprintf(stderr, "unexpected CCreatureActionBase::IsAffectedByHeroStats\n"); abort(); }
bool CCreatureActionBase::IsPhysicsPushableByHero() const { fprintf(stderr, "unexpected CCreatureActionBase::IsPhysicsPushableByHero\n"); abort(); }
bool CCreatureActionBase::IsAlwaysPhysicsPushableBySelfAvoidance() const { fprintf(stderr, "unexpected CCreatureActionBase::IsAlwaysPhysicsPushableBySelfAvoidance\n"); abort(); }
bool CCreatureActionBase::IsAlwaysUseAnimationMovement() const { fprintf(stderr, "unexpected CCreatureActionBase::IsAlwaysUseAnimationMovement\n"); abort(); }
bool CCreatureActionBase::IsClearingMovementVector() const { fprintf(stderr, "unexpected CCreatureActionBase::IsClearingMovementVector\n"); abort(); }
bool CCreatureActionBase::IsToHaveEmptyHands() const { fprintf(stderr, "unexpected CCreatureActionBase::IsToHaveEmptyHands\n"); abort(); }
bool CCreatureActionBase::IsToReplaceItemIfRemoved() const { fprintf(stderr, "unexpected CCreatureActionBase::IsToReplaceItemIfRemoved\n"); abort(); }
bool CCreatureActionBase::IsAllowingAnimationMovementToMoveSpeakAndWaitCameras() const { fprintf(stderr, "unexpected CCreatureActionBase::IsAllowingAnimationMovementToMoveSpeakAndWaitCameras\n"); abort(); }
bool CCreatureActionBase::IsPreventingQuickAccessItemUsage() const { fprintf(stderr, "unexpected CCreatureActionBase::IsPreventingQuickAccessItemUsage\n"); abort(); }
void CCreatureActionBase::SetHitter(const CThing &) { fprintf(stderr, "unexpected CCreatureActionBase::SetHitter\n"); abort(); }
void CCreatureActionBase::SetHitDirection(C3DVector) { fprintf(stderr, "unexpected CCreatureActionBase::SetHitDirection\n"); abort(); }
void CCreatureActionBase::SetHitPosition(C3DVector) { fprintf(stderr, "unexpected CCreatureActionBase::SetHitPosition\n"); abort(); }
CTCWeaponTrail * CCreatureActionBase::GetWeaponTrailUsedInStrikeAction(long &) const { fprintf(stderr, "unexpected CCreatureActionBase::GetWeaponTrailUsedInStrikeAction\n"); abort(); }
float CCreatureActionBase::GetTotalTimeSeconds() const { fprintf(stderr, "unexpected CCreatureActionBase::GetTotalTimeSeconds\n"); abort(); }
bool CCreatureActionBase::IsToEvadeAllHits() const { fprintf(stderr, "unexpected CCreatureActionBase::IsToEvadeAllHits\n"); abort(); }
bool CCreatureActionBase::IsToCauseRecoil() const { fprintf(stderr, "unexpected CCreatureActionBase::IsToCauseRecoil\n"); abort(); }
bool CCreatureActionBase::IsAbleToBeAttacked() const { fprintf(stderr, "unexpected CCreatureActionBase::IsAbleToBeAttacked\n"); abort(); }
bool CCreatureActionBase::IsPlayHitResponse() const { fprintf(stderr, "unexpected CCreatureActionBase::IsPlayHitResponse\n"); abort(); }
bool CCreatureActionBase::IsPreventingNormalMovement() const { fprintf(stderr, "unexpected CCreatureActionBase::IsPreventingNormalMovement\n"); abort(); }
bool CCreatureActionBase::IsPreventingLookTurning() const { fprintf(stderr, "unexpected CCreatureActionBase::IsPreventingLookTurning\n"); abort(); }
void CCreatureActionBase::OnBlowBlocked(bool) { fprintf(stderr, "unexpected CCreatureActionBase::OnBlowBlocked\n"); abort(); }
void CCreatureActionBase::OnFollowUpActionFailedToAdd() { fprintf(stderr, "unexpected CCreatureActionBase::OnFollowUpActionFailedToAdd\n"); abort(); }
void CCreatureActionBase::UpdateActionRunningTime() { fprintf(stderr, "unexpected CCreatureActionBase::UpdateActionRunningTime\n"); abort(); }
bool CCreatureActionBase::IsDecapitatingAttack() const { fprintf(stderr, "unexpected CCreatureActionBase::IsDecapitatingAttack\n"); abort(); }
bool CCreatureActionBase::PreventSneaking() const { fprintf(stderr, "unexpected CCreatureActionBase::PreventSneaking\n"); abort(); }
void CCreatureActionBase::FrameUpdate() { fprintf(stderr, "unexpected CCreatureActionBase::FrameUpdate\n"); abort(); }
bool CCreatureActionBase::IsUsingAnimationMovement() const { fprintf(stderr, "unexpected CCreatureActionBase::IsUsingAnimationMovement\n"); abort(); }
bool CCreatureActionBase::IsUsingForcedMovement() const { fprintf(stderr, "unexpected CCreatureActionBase::IsUsingForcedMovement\n"); abort(); }
bool CCreatureActionBase::IsMovementUsingPhysics() const { fprintf(stderr, "unexpected CCreatureActionBase::IsMovementUsingPhysics\n"); abort(); }
void CCreatureActionBase::UpdateActionMoveToPos() { fprintf(stderr, "unexpected CCreatureActionBase::UpdateActionMoveToPos\n"); abort(); }
void CCreatureActionBase::InitialiseActionMovement() { fprintf(stderr, "unexpected CCreatureActionBase::InitialiseActionMovement\n"); abort(); }
float CCreatureActionBase::GetFrameUpdateTime() const { fprintf(stderr, "unexpected CCreatureActionBase::GetFrameUpdateTime\n"); abort(); }
float CCreatureActionBase::GetActionSpeedParameter() const { fprintf(stderr, "unexpected CCreatureActionBase::GetActionSpeedParameter\n"); abort(); }
float CCreatureActionBase::GetActionStartRunningTimeSeconds() const { fprintf(stderr, "unexpected CCreatureActionBase::GetActionStartRunningTimeSeconds\n"); abort(); }
bool CCreatureActionBase::OverrideFaceWithDefault() const { fprintf(stderr, "unexpected CCreatureActionBase::OverrideFaceWithDefault\n"); abort(); }
bool CCreatureActionBase::AllowsCombinationAnims() const { fprintf(stderr, "unexpected CCreatureActionBase::AllowsCombinationAnims\n"); abort(); }
bool CCreatureActionBase::CanAnimationTransitionOut() const { fprintf(stderr, "unexpected CCreatureActionBase::CanAnimationTransitionOut\n"); abort(); }
void CCreatureActionBase::OnAnimationLoaded() { fprintf(stderr, "unexpected CCreatureActionBase::OnAnimationLoaded\n"); abort(); }
bool CCreatureActionBase::WaitForMeshToLoad() const { fprintf(stderr, "unexpected CCreatureActionBase::WaitForMeshToLoad\n"); abort(); }
void CCreatureAction_PlayAnimation::PerformAction() { fprintf(stderr, "unexpected CCreatureAction_PlayAnimation::PerformAction\n"); abort(); }
void CCreatureAction_PlayAnimation::FrameUpdate() { fprintf(stderr, "unexpected CCreatureAction_PlayAnimation::FrameUpdate\n"); abort(); }
bool CCreatureAction_PlayAnimation::IsUsingAnimationMovement() const { fprintf(stderr, "unexpected CCreatureAction_PlayAnimation::IsUsingAnimationMovement\n"); abort(); }
bool CCreatureAction_PlayAnimation::IsMovementUsingPhysics() const { fprintf(stderr, "unexpected CCreatureAction_PlayAnimation::IsMovementUsingPhysics\n"); abort(); }
void CCreatureAction_PlayAnimation::FinishAction() { fprintf(stderr, "unexpected CCreatureAction_PlayAnimation::FinishAction\n"); abort(); }
bool CCreatureAction_PlayAnimation::IsAllowingAnimationMovementToMoveSpeakAndWaitCameras() const { fprintf(stderr, "unexpected CCreatureAction_PlayAnimation::IsAllowingAnimationMovementToMoveSpeakAndWaitCameras\n"); abort(); }
CCreatureActionBase* CCreatureAction_PlayAnimation::Clone() const { fprintf(stderr, "unexpected CCreatureAction_PlayAnimation::Clone\n"); abort(); }
void CCreatureAction_PickUpThingAtTable::PerformAction() { fprintf(stderr, "unexpected CCreatureAction_PickUpThingAtTable::PerformAction\n"); abort(); }
CCreatureActionBase* CCreatureAction_PickUpThingAtTable::Clone() const { fprintf(stderr, "unexpected CCreatureAction_PickUpThingAtTable::Clone\n"); abort(); }
void CCreatureAction_PutDownThingAtTable::PerformAction() { fprintf(stderr, "unexpected CCreatureAction_PutDownThingAtTable::PerformAction\n"); abort(); }
CCreatureActionBase* CCreatureAction_PutDownThingAtTable::Clone() const { fprintf(stderr, "unexpected CCreatureAction_PutDownThingAtTable::Clone\n"); abort(); }
CCreatureActionBase* CCreatureAction_PickUpTankard::Clone() const { fprintf(stderr, "unexpected CCreatureAction_PickUpTankard::Clone\n"); abort(); }
CCreatureActionBase* CCreatureAction_PutDownTankard::Clone() const { fprintf(stderr, "unexpected CCreatureAction_PutDownTankard::Clone\n"); abort(); }
void CTCTavernTankard::OnInitialActivate() { fprintf(stderr, "unexpected CTCTavernTankard::OnInitialActivate\n"); abort(); }
ETCInterfaceType CTCTavernTankard::GetInterfaceType() const { fprintf(stderr, "unexpected CTCTavernTankard::GetInterfaceType\n"); abort(); }
CCharString CTCTavernTankard::GetClassTypeName() const { fprintf(stderr, "unexpected CTCTavernTankard::GetClassTypeName\n"); abort(); }
void CTCTavernGame::OnSerialise(CPersistContext&, bool) { fprintf(stderr, "unexpected CTCTavernGame::OnSerialise\n"); abort(); }
void CTCTavernGame::OnInitialActivate() { fprintf(stderr, "unexpected CTCTavernGame::OnInitialActivate\n"); abort(); }
bool CTCTavernGame::HasFrameUpdate() const { fprintf(stderr, "unexpected CTCTavernGame::HasFrameUpdate\n"); abort(); }
void CTCTavernGame::FrameUpdate() { fprintf(stderr, "unexpected CTCTavernGame::FrameUpdate\n"); abort(); }
void CTCTavernGame::DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo&) { fprintf(stderr, "unexpected CTCTavernGame::DrawPreparePrimitivesForRendering\n"); abort(); }
ETCInterfaceType CTCTavernGame::GetInterfaceType() const { fprintf(stderr, "unexpected CTCTavernGame::GetInterfaceType\n"); abort(); }
void CTCTavernGame::ProcessInput(const CInputEvent&, CProcessedInput&, CInputProcessTavernGame*) { fprintf(stderr, "unexpected CTCTavernGame::ProcessInput\n"); abort(); }
void CTCTavernGame::EnterGame() { fprintf(stderr, "unexpected CTCTavernGame::EnterGame\n"); abort(); }
void CTCTavernGame::LeaveGame() { fprintf(stderr, "unexpected CTCTavernGame::LeaveGame\n"); abort(); }
void CTCTavernGame::NewGame() { fprintf(stderr, "unexpected CTCTavernGame::NewGame\n"); abort(); }
void CTCTavernGame::NewRound() { fprintf(stderr, "unexpected CTCTavernGame::NewRound\n"); abort(); }
void CTCTavernGame::EndRound() { fprintf(stderr, "unexpected CTCTavernGame::EndRound\n"); abort(); }
void CTCTavernGame::QuitMidGame() { fprintf(stderr, "unexpected CTCTavernGame::QuitMidGame\n"); abort(); }
void CTCTavernGame::Greeting() { fprintf(stderr, "unexpected CTCTavernGame::Greeting\n"); abort(); }
void CTCTavernGame::Options() { fprintf(stderr, "unexpected CTCTavernGame::Options\n"); abort(); }
void CTCTavernGame::Instructions() { fprintf(stderr, "unexpected CTCTavernGame::Instructions\n"); abort(); }
void CTCTavernGame::Betting() { fprintf(stderr, "unexpected CTCTavernGame::Betting\n"); abort(); }
void CTCTavernGame::ReactionAndPayout() { fprintf(stderr, "unexpected CTCTavernGame::ReactionAndPayout\n"); abort(); }
bool CTCTavernGame::UpdateValues() { fprintf(stderr, "unexpected CTCTavernGame::UpdateValues\n"); abort(); }
void CTCTavernGame::Farewell() { fprintf(stderr, "unexpected CTCTavernGame::Farewell\n"); abort(); }
CWideString CTCTavernGame::GetWinPhrase() { fprintf(stderr, "unexpected CTCTavernGame::GetWinPhrase\n"); abort(); }
CWideString CTCTavernGame::GetAdditionalInfo() { fprintf(stderr, "unexpected CTCTavernGame::GetAdditionalInfo\n"); abort(); }
CWideString CTCTavernGame::GetBet() { fprintf(stderr, "unexpected CTCTavernGame::GetBet\n"); abort(); }
CWideString CTCTavernGame::GetPlayersMoney() { fprintf(stderr, "unexpected CTCTavernGame::GetPlayersMoney\n"); abort(); }
CWideString CTCTavernGame::GetTotalWinnings() { fprintf(stderr, "unexpected CTCTavernGame::GetTotalWinnings\n"); abort(); }
void CTCTavernGame::DrawBetAndMoney(const CEnginePrimitivePrepareInfo&, float, float, float, float, float, float) { fprintf(stderr, "unexpected CTCTavernGame::DrawBetAndMoney\n"); abort(); }