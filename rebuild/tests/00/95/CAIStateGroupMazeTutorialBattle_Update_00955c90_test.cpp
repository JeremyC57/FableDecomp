#define FABLE_RETAIL_CTHING_ISDEAD_DIRECT
#define FABLE_RETAIL_CDEFPOINTER_SETDYNAMICCAST_REREAD
#define FABLE_RETAIL_MAZE_TUTORIAL_PASS29
#define FABLE_RETAIL_CAISTATEGROUP_MAZETUTORIALBATTLE_VIRTUAL_MODEL
#define FABLE_RETAIL_CAISTATEGROUP_MAZEBASE_VIRTUAL_MODEL
#define FABLE_RETAIL_CTCBOSS_ISREADYFORPHASECHANGE
#define FABLE_RETAIL_CAISTATEGROUPBASE_PEEKDEFINITIONMANAGER
#define FABLE_RETAIL_CAISTATEGROUPBASE_PEEKDEFINITIONMANAGER_PROTECTED
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CTCCOMBAT
#define FABLE_RETAIL_CCREATUREACTION_COMBATBOAST_MAZE
#define FABLE_RETAIL_NCREATUREMODE_QUERIES
// CAIStateGroup_MazeTutorialBattle::Update
// Leaf: ?Update@CAIStateGroup_MazeTutorialBattle@@UAEXXZ
// Flags:
#define FABLE_RETAIL_MAZE_DEFENSIVE_MELEE_PASS27
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGPHYSICAL_HEALTH_PERCENTAGE
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_GETCOMBATTYPEDEF
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING_GETTC_DONOR_NAMES_PASS23
#define FABLE_RETAIL_CVECTORMAP_INLINE_CONST_LOWERBOUND
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_DEREFERENCE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CINTELLIGENTPOINTER_GET_ORIGINAL_CHAIN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_CONVERT_MUTABLE
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE_AIACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_PUMPCOMBATACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_PERFORMACTION
#define FABLE_RETAIL_CAISTATEGROUPBASE_MOVETOPOSPROXIMITY
#define FABLE_RETAIL_CAISTATEGROUPBASE_GOTOBED_WORLD
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CAIBRAIN_SETOUTPUT
#define FABLE_RETAIL_CAISTATEGROUP_MAZEBASE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_MAGNITUDE2D
#define FABLE_RETAIL_C3DVECTOR_PACKED
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_COMBATMANAGER
#define FABLE_RETAIL_CCOMBATMANAGER
#define FABLE_RETAIL_COMBAT_DEFENDER_ZONE_PASS23
#define FABLE_RETAIL_CMAZEBATTLEDEF
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CTCCREATUREMODEMANAGER
#define FABLE_RETAIL_CTCBOSS_GETPHASESTARTPOS
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE_FULL_MEMBERS
#define FABLE_RETAIL_CCOMBATACTIONBASE_MELEE
#define FABLE_RETAIL_CCOMBATACTIONBASE_FRAMEUPDATE
#define FABLE_RETAIL_CCOMBATACTIONBASE_INITIALISE
#define FABLE_RETAIL_CCOMBATACTIONBASE_FINISHACTION
#define FABLE_RETAIL_CCOMBATACTIONBASE_ONANIMATIONLOADED
#define FABLE_RETAIL_CCOMBATACTIONBASE_MELEE_OWN_VIRTUALS
#define FABLE_RETAIL_CCOMBATACTIONBASE_SOUNDCTOR
#define FABLE_RETAIL_CCOMBATACTIONBASE_TARGETCTOR
#define FABLE_RETAIL_CCOMBATACTION_WILLSHIELDINTO
#define FABLE_RETAIL_CCREATUREACTION_THROWSHOTOBJECT_CLASS
#define FABLE_RETAIL_CCREATUREACTION_COMBATIDLE_MAZE
#define FABLE_RETAIL_CBRAINOUTPUT_ONLY

#define FABLE_RETAIL_CCOMBATTYPEDEF
#define FABLE_RETAIL_CDRAWABILITYBUTTONS_TEST_CTORS_PASS27
#define FABLE_RETAIL_MAZE_FIXTURE_PASS27
#define FABLE_RETAIL_CCREATUREACTIONBASE_ANIMCTOR
#define FABLE_RETAIL_CAIBRAIN_CREATURE_REFERENCE
#define FABLE_RETAIL_CAIBRAIN_CTOR_PASS23
#define FABLE_RETAIL_SENSED_PASS23
#define FABLE_RETAIL_KID_EVADE_LIFETIMES_PASS23
#define FABLE_RETAIL_CTCCREATUREMODEMANAGER_CTOR_PASS23
#define FABLE_RETAIL_CCOMBATMANAGER_CTOR
#define FABLE_RETAIL_CDEFENDINGCOMBATANTINFO_PRONG
#define FABLE_RETAIL_STLP_BITSET_INDEX
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEFINITION_ORIGINAL_USER_CONSTRUCTORS_PASS22
#define private public
#define protected public
#include "fable_string.h"
#include "engine/CThingAICreature.h"
#include "engine/CThingBaseDef.h"
#include "engine/C3DVector.h"
#include "engine/CDefPointer.h"
#include "engine/CWorld.h"
#include "engine/CCombatManager.h"
#include "engine/CCombatTypeDef.h"
#include "engine/CMainGameComponent.h"
#include "engine/CAIStateGroup_MazeTutorialBattle.h"
#include "engine/ThingAlive.h"
#include "engine/EAIAction.h"
#include "engine/EMovementType.h"
#include "engine/ECombatZone.h"
#include "engine/CTCBoss.h"
#include "engine/CTCCreatureModeManager.h"
#include "engine/CTCAICreatureWillPowerIndicator.h"
#include "engine/CSoundPair.h"
#include "engine/CCombatAction_WillShieldInto.h"
#include "engine/CCombatAction_WillHealInto.h"
#include "engine/CCreatureAction_ThrowShotObject.h"
#include "engine/CCreatureAction_CombatIdle.h"
#include "engine/CCreatureAction_CombatBoast.h"
#include "engine/ECombatAbilityDataType.h"
#include "engine/CTCCombat.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/NCreatureMode.h"
#include "engine/campfire_types.h"
#include "engine/CAIBrain.h"
#include "engine/_quarantine/CMazeBattleDef.h"
#include "engine/CDefClassBase_GetSubDef.h"

#include "engine/CAnimationEntry.h"
#undef private
#undef protected
static const CThingBaseDef* creature_def;
static CMazeBattleDef* battle_def;
static unsigned definition_reads;
template<> bool CDefClassBase::GetSubDef<CMazeBattleDef>(CDefPointer<const CMazeBattleDef>* destination) const
{
    if (this != creature_def) abort();
    destination->Object = battle_def;
    ++battle_def->IVRefCount;
    ++definition_reads;
    return true;
}


static unsigned combat_def_reads;
static CCombatTypeDef* test_style;
static const char* test_literal;
template<> bool CDefinitionManager::GetDef<CCombatTypeDef>(const CCharString&, CDefPointer<const CCombatTypeDef>* result) const
{
    if (!test_literal || strcmp(test_literal,"HERO_MAZE_TUTORIAL_ATTACK_STYLE2")) abort();
    ++combat_def_reads;
    result->Object = test_style;
    ++test_style->IVRefCount;
    return true;
}
inline bool CAIStateGroup_MazeTutorialBattle::IsStillRunnable()
{
    if (!IS_THING_ALIVE(PTargetThing) || IsStopping()) return false;
    return true;
}
inline bool CAIStateGroup_MazeBase::HasEnemyBeenKnockedDown()
{
    if (NCreatureMode::IsKnockedDown(*PTargetThing)) return true;
    return false;
}
void CAIStateGroup_MazeTutorialBattle::Update()
{
    if (!IsStillRunnable()) { Terminate(); return; }
    CTCBoss* ptc_boss; GetCreature().GetTC(&ptc_boss);
    CDefPointer<const CMazeBattleDef> p_maze_def;
    GetCreature().PeekBaseDef().GetSubDef(&p_maze_def);
    CTCCombat* ptc_combat; GetCreature().GetTC(&ptc_combat);
    switch (ptc_boss->GetBossPhase()) {
    case 0: case 1: case 2: {
        CTCAICreatureWillPowerIndicator* ptc_power_indicator;
        if (GetCreature().GetTC(&ptc_power_indicator)) ptc_power_indicator->Reset(10.0f);
        CDefPointer<const CCombatTypeDef> p_combat_type_def;
        PeekDefinitionManager().GetDef("HERO_MAZE_TUTORIAL_ATTACK_STYLE2", &p_combat_type_def);
        ptc_combat->SetCombatType(p_combat_type_def);
        float dis_to_target=(GetCreature().GetPos()-PTargetThing->GetPos()).GetMagnitude();
        if(dis_to_target < GetCreature().GetCombatTypeDef().FrontMiddleLimit) {
            if(GetCreature().IsInMeleeAsAttacker() && (PTargetThing->GetTypeFlags() & 8)) {
                CThingCreatureBase* pthing_creature=GFDebugPDynamicCast(PTargetThing.Get(),(CThingCreatureBase*)0);
                ECombatZone cur_zone=GetWorld().GetCombatManager().GetDefenderZone(*pthing_creature,GetCreature(),0);
                switch(cur_zone) {
                case COMBAT_ZONE_FRONT_FAR: break;
                case COMBAT_ZONE_FRONT_MIDDLE: case COMBAT_ZONE_FRONT_NEAR: break;
                default:
                    AIAction_LookAtThing(ACTION_REPEAT_UNTIL_FINISHED,PTargetThing,0x21,1.0f/12.0f);
                    if(!IsStillRunnable()) { Terminate();return; }
                    break;
                }
                AIAction_PumpCombatActions();
            }
        } else {
            AIAction_LookAtThing(ACTION_REPEAT_UNTIL_FINISHED,PTargetThing,0x21,1.0f/12.0f);
            if(!IsStillRunnable()) { Terminate();return; }
        }
        if(ptc_boss->IsReadyForPhaseChange()) ptc_boss->SetNextBossPhase(ptc_boss->GetBossPhase()+1,false);
        break;
    }
    default: AIAction_DoNothing();break;
    }
    if(IsStillRunnable() && (PTargetThing->GetTypeFlags() & 8)) {
        CThingCreatureBase* pthing_creature=GFDebugPDynamicCast(PTargetThing.Get(),(CThingCreatureBase*)0);
        if(HasEnemyBeenKnockedDown()) AIAction_PerformActionAndWait(CCreatureAction_CombatBoast(GetCreature(),*pthing_creature),false);
    }
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

void CIVCountedPointeeBase::Delete() const { abort(); }
CResource::CResource() : CIVCountedPointeeBase() {}
CResource::~CResource() {}
CDefPointeeBase::CDefPointeeBase() {}
CDefClassBase::CDefClassBase() {}
CParentDefClassBase::CParentDefClassBase() {}
CParentDefClassBase::~CParentDefClassBase() {}
CAIStateGroupBase::CAIStateGroupBase() {}
CAIBrain::CAIBrain(const CMainGameComponent&, CWorld&, CThingManager&, CPlayerManager&,
    CThingAICreature& creature, CDefPointer<const CBrainDef>) : Creature(creature) {}
CCombatManager::CCombatManager(CWorld& world) : World(world) {}
CTCCreatureModeManager::CTCCreatureModeManager(CThing& thing) : CTCBase(thing) {}
CTCCreatureModeManager::~CTCCreatureModeManager() {}
CTCBoss::CTCBoss(CThing& thing) : CTCBase(thing) {}
CTCAICreatureWillPowerIndicator::CTCAICreatureWillPowerIndicator(CThing& thing) : CTCBase(thing) {}
CBrainOutput::CBrainOutput() : Types(0), LookFlags(0) {}
CBrainOutput::CBrainOutput(const CBrainOutput& source)
    : Types(source.Types), LookFlags(source.LookFlags), NavigationInput(source.NavigationInput),
      PAction(source.PAction), MoveToPos(source.MoveToPos), LookAtPos(source.LookAtPos), LookAtThing(source.LookAtThing) {}
CNavigationInput::CNavigationInput() : Proximity(0), MovementType(AI_MOVEMENT_NULL), Speed(0) {}

class TestCreatureDef : public CThingBaseDef {
public:
    void Transfer(CPersistContext&) { abort(); }
    void Copy(const CDefClassBase*) { abort(); }
    unsigned long GetSizeofClass() { return sizeof(*this); }
};
class TestModes : public CTCCreatureModeManager {
public:
    TestModes(CThing& thing) : CTCCreatureModeManager(thing) {}
    ETCInterfaceType GetInterfaceType() const { return TCI_ENTITY_MODE_MANAGER; }
    CCharString GetClassTypeName() const { return CCharString(); }
};
class TestPower : public CTCAICreatureWillPowerIndicator {
public:
    TestPower(CThing& thing) : CTCAICreatureWillPowerIndicator(thing) {}
    ETCInterfaceType GetInterfaceType() const { return TCI_AICREATURE_WILL_POWER_INDICATOR; }
    CCharString GetClassTypeName() const { return CCharString(); }
};

enum Action { NO_ACTION, SHIELD, HEAL, THROW, IDLE };
static CThingAICreature* maze;
static CThingAICreature* target;
static CWorld* world;
static CCombatManager* combat;
static TestModes* modes;
static TestPower* indicator;
static CAIBrain* brain;
static CTCBoss* boss;
class TestCombatType : public CCombatTypeDef {
public:
    void Transfer(CPersistContext&) { abort(); }
    void Copy(const CDefClassBase*) { abort(); }
    unsigned long GetSizeofClass() { return sizeof(*this); }
};
static TestCombatType combat_def;
static C3DVector maze_pos(0.0f, 0.0f, 2.0f);
static C3DVector target_pos(0.0f, 2.0f, 90.0f);
static CAnimationEntry animation;
static bool melee, strafe, mode, die_on_look, die_on_shot, die_on_heal, die_on_move;
static ECombatZone zone;
static float health;
static long frame;
static unsigned stops, stop_at, moves, looks, pumps, outputs, added, zone_checks, terminations;
static Action built_action, action_log[4];
static unsigned action_count, released;
long CMainGameComponent::ConstantFPS = 30;
CMainGameComponent* CMainGameComponent::PMainGameComponent = 0;

static void Build(Action action, CThingCreatureBase& creature)
{
    Require(&creature == maze && built_action == NO_ACTION);
    built_action = action;
}
CCreatureActionBase::CCreatureActionBase(CThingCreatureBase&, float, float, const CAnimationEntry&,
    const CSoundPair&, long, long) {}
CCreatureActionBase::~CCreatureActionBase() {}
CCombatActionBase::CCombatActionBase(CThingCreatureBase& creature, long, const CSoundPair& sound)
    : CCreatureActionBase(creature, 0.0f, 0.0f, animation, sound, 0, 0) {}
CCombatActionBase::CCombatActionBase(CThingCreatureBase& creature, CThing& enemy, long priority, const CSoundPair& sound)
    : CCreatureActionBase(creature, 0.0f, 0.0f, animation, sound, 0, 0)
{
    Require(&enemy == target && priority == 50);
    Build(IDLE, creature);
}
CCombatActionBase::~CCombatActionBase() { Require(built_action == NO_ACTION); ++released; }
CCombatAction_WillShieldInto::CCombatAction_WillShieldInto(CThingCreatureBase& creature, float time, float hit_points)
    : CCombatActionBase(creature, 0, CSoundPair())
{
    Require(time == 6.0f && hit_points == 5.0f);
    Build(SHIELD, creature);
}
CCombatAction_WillHealInto::CCombatAction_WillHealInto(CThingCreatureBase& creature, long spell_level)
    : CCombatActionBase(creature, 0, CSoundPair())
{
    Require(spell_level == 3);
    Build(HEAL, creature);
}
CCreatureAction_ThrowShotObject::CCreatureAction_ThrowShotObject(CThingCreatureBase& creature, CThing& enemy)
    : CCombatActionBase(creature, 0, CSoundPair())
{
    Require(&enemy == target);
    Build(THROW, creature);
}

const C3DVector& CThing::GetPos() const
{
    Require(this == maze || this == target);
    return this == maze ? maze_pos : target_pos;
}
const CCombatTypeDef& CThingCreatureBase::GetCombatTypeDef() const { Require(this == maze); return combat_def; }
bool CThingCreatureBase::IsInMeleeAsAttacker() const { Require(this == maze); return melee; }
bool CThingCreatureBase::IsAbleToStrafe() const { Require(this == maze); return strafe; }
float CThingPhysical::GetHealthPercentage() const { Require(this == maze); return health; }
ECombatZone CCombatManager::GetDefenderZone(const CThingCreatureBase& enemy,
    const CThingCreatureBase& defender, long flags) const
{
    Require(this == combat && &enemy == target && &defender == maze && flags == 0);
    ++zone_checks;
    return zone;
}
CWorld& CAIStateGroupBase::GetWorld() const { return *world; }
long __fastcall CWorld::GetFrame() { return frame; }
bool CAIStateGroupBase::IsStopping() const
{
    Require(++stops < 8);
    return stops >= stop_at;
}
void CAIStateGroupBase::Terminate() { ++terminations; }
bool CTCCreatureModeManager::IsModeActive(unsigned long kind) const { Require(this == modes && kind == 0x19); return mode; }
bool CTCCreatureModeManager::AddMode(unsigned long kind)
{
    Require(this == modes && kind == 0x19 && !mode);
    ++added;
    mode = true;
    return true;
}
EAIActionReturn CAIStateGroupBase::AIAction_LookAtThing(EAIActionRepeat repeat, CIntelligentPointer<const CThing> thing,
    long flags, float speed)
{
    Require(repeat == ACTION_REPEAT_UNTIL_FINISHED && thing.Get() == target && flags == 0x21 && speed > 0.083f && speed < 0.084f);
    ++looks;
    if (die_on_look) target->Dead = true;
    return AI_ACTION_FINISHED;
}
EAIActionReturn CAIStateGroupBase::AIAction_PumpCombatActions()
{
    ++pumps;
    return AI_ACTION_FINISHED;
}
EAIActionReturn CAIStateGroupBase::AIAction_MoveToPosProximity(EAIActionRepeat repeat, C3DVector position,
    float radius, bool& reached, EMovementType movement, bool a, bool b, bool c, const CThing* leader, bool d, float speed)
{
    Require(repeat == ACTION_REPEAT_CALLBACK && radius == 1.0f && movement == AI_MOVEMENT_RUNNING);
    Require(position.X == 5.0f && position.Y == -6.0f && position.Z == 7.0f);
    Require(!a && !b && c && !leader && d && speed == 0.0f);
    ++moves;
    reached = true;
    if (die_on_move) target->Dead = true;
    return AI_ACTION_FINISHED;
}
void CBrainOutput::SetAsLookAtThing(const CThing& enemy, long flags)
{
    Require(&enemy == target && flags == 0xa7);
    LookAtThing.SetPItem(const_cast<CThing*>(&enemy));
    LookFlags = flags;
}
void CAIBrain::SetOutput(const CBrainOutput& output)
{
    Require(this == brain && output.LookAtThing.Get() == target && output.LookFlags == 0xa7);
    ++outputs;
}
EAIActionReturn CAIStateGroupBase::AIAction_PerformActionAndWait(const CCreatureActionBase&, bool wait)
{
    Require(!wait && built_action != NO_ACTION && action_count < 4);
    action_log[action_count++] = built_action;
    if ((built_action == THROW && die_on_shot) || (built_action == HEAL && die_on_heal) || built_action == IDLE)
        target->Dead = true;
    if (built_action == HEAL) {
        frame += 15;
        health = 1.0f;
    }
    built_action = NO_ACTION;
    return AI_ACTION_FINISHED;
}
static void Put(CThing& thing, ETCInterfaceType key, CTCBase* component)
{
    thing.UsingComponents[key] = true;
    thing.Components.push_back(std::pair<ETCInterfaceType, CTCBase*>(key, component));
}

static long phase;
static bool ready, down;
static unsigned changes, resets, styles, nothing, knock_queries;
class TestCombat : public CTCCombat {
public:
    TestCombat(CThing& thing) : CTCCombat(thing) {}
    ETCInterfaceType GetInterfaceType() const { return TCI_COMBAT; }
    CCharString GetClassTypeName() const { return CCharString(); }
};
static TestCombat* tc_combat;
static CGameDefinitionManager manager;
CGameDefinitionManager::CGameDefinitionManager() {}
CGameDefinitionManager::~CGameDefinitionManager() {}
void CGameDefinitionManager::InitAndCompile() { abort(); }
CDefinitionManager::~CDefinitionManager() {}
const CGameDefinitionManager& CAIStateGroupBase::PeekDefinitionManager() const { return manager; }
CCharString::CCharString(const char* literal, long limit) : PStringData(0) { Require(limit == -1); test_literal = literal; }
CTCCombat::CTCCombat(CThing& actor) : CTCBase(actor), CombatManager(*combat) {}
void CTCCombat::SetCombatType(const CDefPointer<const CCombatTypeDef>& def)
{ Require(this == tc_combat && def.Get() == test_style); ++styles; }
void CTCAICreatureWillPowerIndicator::Reset(float delay) { Require(this == indicator && delay == 10.0f); ++resets; }
long CTCBoss::GetBossPhase() const { Require(this == boss); return phase; }
bool CTCBoss::IsReadyForPhaseChange() const { Require(this == boss); return ready; }
void CTCBoss::SetNextBossPhase(long next, bool immediate) { Require(this == boss && next == phase+1 && !immediate); ++changes; }
bool __fastcall NCreatureMode::IsKnockedDown(const CThing& thing) { Require(&thing == target); ++knock_queries; return down; }
EAIActionReturn CAIStateGroupBase::AIAction_DoNothing() { ++nothing;return AI_ACTION_FINISHED; }
CCreatureAction_CombatBoast::CCreatureAction_CombatBoast(CThingCreatureBase& actor,CThingCreatureBase& enemy)
    : CCombatActionBase(actor,enemy,50,CSoundPair()) {}
void CAIStateGroup_MazeBase::OnActivate() { abort(); }
void CAIStateGroup_MazeBase::OnReplace() { abort(); }
void CAIStateGroup_MazeTutorialBattle::OnActivate() { abort(); }
EAIStateGroupType CAIStateGroup_MazeTutorialBattle::GetStateGroupType() const { abort();return (EAIStateGroupType)0; }
bool CAIStateGroup_MazeTutorialBattle::IsRunnable() { abort();return false; }
int main()
{
    CThingAICreature actor, enemy;
    TestCreatureDef base_def;
    CMazeBattleDef def;
    CTCBoss boss_component(actor);
    TestModes mode_component(actor);
    TestPower power(actor);
    static unsigned __int64 game_storage[4], world_storage[24], manager_storage[4], player_storage[4];
    world = reinterpret_cast<CWorld*>(world_storage);
    CCombatManager combat_manager(*world);
    combat=&combat_manager; TestCombat tc(actor);tc_combat=&tc;test_style=&combat_def;
    CAIBrain ai(*reinterpret_cast<CMainGameComponent*>(game_storage), *world,
        *reinterpret_cast<CThingManager*>(manager_storage), *reinterpret_cast<CPlayerManager*>(player_storage),
        actor, CDefPointer<const CBrainDef>());
    CAIStateGroup_MazeTutorialBattle state;
    brain = &ai; state.PBrain = brain;
    maze = &actor; target = &enemy; modes = &mode_component; indicator = &power; boss = &boss_component;
    creature_def = &base_def; battle_def = &def; combat = &combat_manager;
    actor.PBaseDef.Object = &base_def;
    base_def.IVRefCount = 2;
    def.IVRefCount = 1;
    def.ShieldUpTime = 6.0f;
    def.MinThrowShotRange = 10.0f;
    def.MinTimeBetweenHeals = 4.0f;
    def.HealSpellLevel = 3;
    combat_def.FrontMiddleLimit = 4.0f;
    boss->PhaseStartPos = C3DVector(5.0f, -6.0f, 7.0f);
    world->PThingCombatManager_ptr = combat;
    memset(actor.UsingComponents._M_w, 0, sizeof(actor.UsingComponents._M_w));
    Put(actor, TCI_COMBAT, &tc);
    Put(actor, TCI_ENTITY_MODE_MANAGER, &mode_component);
    Put(actor, TCI_BOSS, &boss_component);
    Put(actor, TCI_AICREATURE_WILL_POWER_INDICATOR, &power);


    combat_def.IVRefCount=1;
    for (phase=-1;phase<=3;++phase)
    for (unsigned flags=0;flags<512;++flags)
    for (unsigned route=0;route<5;++route) {
        const bool active_phase=phase>=0 && phase<=2;
        const bool target_valid=route!=0;
        const bool running=route!=1;
        const bool can_run=target_valid && running && route!=4;
        const bool close=(flags & 1)!=0;
        melee=(flags & 2)!=0;
        const bool creature_target=(flags&4)!=0;
        const bool look_zone=(flags&8)!=0;
        zone=look_zone?COMBAT_ZONE_REAR:(route==2?COMBAT_ZONE_FRONT_NEAR:COMBAT_ZONE_FRONT_FAR);
        const bool power_present=(flags&16)!=0;
        down=(flags&32)!=0; ready=(flags&64)!=0;
        die_on_look=(flags&128)!=0;
        const bool stop_after_look=(flags&256)!=0;
        target->Dead=(route==4); target->ThingTypeFlags=creature_target?15:3;
        state.PTargetThing.SetPItem(target_valid?target:0);
        target_pos=C3DVector(0.0f, close?2.0f:4.0f,2.0f);
        maze->UsingComponents[TCI_AICREATURE_WILL_POWER_INDICATOR]=power_present;
        stops=0; stop_at=!running?1u:(stop_after_look?2u:10u);
        pumps=looks=zone_checks=terminations=released=action_count=definition_reads=combat_def_reads=0;
        changes=resets=styles=nothing=knock_queries=0;built_action=NO_ACTION;
        state.Update();
        const bool test_zone=can_run && active_phase && close && melee && creature_target;
        const bool look=can_run && active_phase && (!close || (test_zone && look_zone));
        const bool stopped_look=look && (die_on_look || stop_after_look);
        const bool check_knock=can_run && !stopped_look && !stop_after_look && creature_target;
        Require(definition_reads==unsigned(can_run) && styles==unsigned(can_run&&active_phase) && combat_def_reads==styles);
        Require(resets==unsigned(can_run && active_phase && power_present));
        Require(looks==unsigned(look) && zone_checks==unsigned(test_zone));
        Require(pumps==unsigned(test_zone&&!stopped_look));
        Require(terminations==unsigned(!can_run||stopped_look));
        Require(changes==unsigned(can_run && active_phase && !stopped_look && ready));
        Require(nothing==unsigned(can_run && !active_phase));
        Require(knock_queries==unsigned(check_knock));
        Require(action_count==unsigned(check_knock && down) && released==action_count);
        Require(def.IVRefCount==1 && combat_def.IVRefCount==1);
    }
    puts("00955c90_TEST PASS");
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
void CThingGameObject::OnHit(CHitParameters&) { fprintf(stderr, "unexpected CThingGameObject::OnHit\n"); abort(); }
void CThingCreatureBase::OnHit(CHitParameters&) { fprintf(stderr, "unexpected CThingCreatureBase::OnHit\n"); abort(); }
bool CThingCreatureBase::DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo& info) { fprintf(stderr, "unexpected CThingCreatureBase::DrawPreparePrimitivesForRendering\n"); abort(); }
void CTCAICreatureWillPowerIndicator::FrameUpdate() { fprintf(stderr, "unexpected CTCAICreatureWillPowerIndicator::FrameUpdate\n"); abort(); }
ETCInterfaceType CTCBoss::GetInterfaceType()const { fprintf(stderr, "unexpected CTCBoss::GetInterfaceType\n"); abort(); }
CCharString CTCBoss::GetClassTypeName()const { fprintf(stderr, "unexpected CTCBoss::GetClassTypeName\n"); abort(); }
bool CTCBoss::HasFrameUpdate()const { fprintf(stderr, "unexpected CTCBoss::HasFrameUpdate\n"); abort(); }
void CTCBoss::FrameUpdate() { fprintf(stderr, "unexpected CTCBoss::FrameUpdate\n"); abort(); }
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
void CCombatActionBase::PerformAction() { fprintf(stderr, "unexpected CCombatActionBase::PerformAction\n"); abort(); }
long CCombatActionBase::GetAnimDelayTime() const { fprintf(stderr, "unexpected CCombatActionBase::GetAnimDelayTime\n"); abort(); }
long CCombatActionBase::GetAnimationIndex() const { fprintf(stderr, "unexpected CCombatActionBase::GetAnimationIndex\n"); abort(); }
long CCombatActionBase::GetAnimationTransitionInTime() const { fprintf(stderr, "unexpected CCombatActionBase::GetAnimationTransitionInTime\n"); abort(); }
unsigned long CCombatActionBase::GetAnimExtraFlags() const { fprintf(stderr, "unexpected CCombatActionBase::GetAnimExtraFlags\n"); abort(); }
bool CCombatActionBase::IsStrikeEvent(long) const { fprintf(stderr, "unexpected CCombatActionBase::IsStrikeEvent\n"); abort(); }
bool CCombatActionBase::IsStrikeEventActive() const { fprintf(stderr, "unexpected CCombatActionBase::IsStrikeEventActive\n"); abort(); }
bool CCombatActionBase::IsStrikeAction() const { fprintf(stderr, "unexpected CCombatActionBase::IsStrikeAction\n"); abort(); }
bool CCombatActionBase::IsStrikeEventFinishedBeingActive() const { fprintf(stderr, "unexpected CCombatActionBase::IsStrikeEventFinishedBeingActive\n"); abort(); }
CCharString CCombatActionBase::GetStrikeEventHelperName() const { fprintf(stderr, "unexpected CCombatActionBase::GetStrikeEventHelperName\n"); abort(); }
bool CCombatActionBase::IsActionReadyForInterruption() const { fprintf(stderr, "unexpected CCombatActionBase::IsActionReadyForInterruption\n"); abort(); }
bool CCombatActionBase::IsActionReadyForInterruptionByMovement(const C3DVector&) const { fprintf(stderr, "unexpected CCombatActionBase::IsActionReadyForInterruptionByMovement\n"); abort(); }
float CCombatActionBase::GetTotalTimeSeconds() const { fprintf(stderr, "unexpected CCombatActionBase::GetTotalTimeSeconds\n"); abort(); }
void CCombatActionBase::OnBlowBlocked(bool) { fprintf(stderr, "unexpected CCombatActionBase::OnBlowBlocked\n"); abort(); }
void CCombatActionBase::UpdateActionRunningTime() { fprintf(stderr, "unexpected CCombatActionBase::UpdateActionRunningTime\n"); abort(); }
float CCombatActionBase::GetFrameUpdateTime() const { fprintf(stderr, "unexpected CCombatActionBase::GetFrameUpdateTime\n"); abort(); }
bool CCombatActionBase::AllowsCombinationAnims() const { fprintf(stderr, "unexpected CCombatActionBase::AllowsCombinationAnims\n"); abort(); }
bool CCombatActionBase::IsVitalStrike() const { fprintf(stderr, "unexpected CCombatActionBase::IsVitalStrike\n"); abort(); }
bool CCombatActionBase::IsKnockdownStrike() const { fprintf(stderr, "unexpected CCombatActionBase::IsKnockdownStrike\n"); abort(); }
bool CCombatActionBase::IsBlockCounter() const { fprintf(stderr, "unexpected CCombatActionBase::IsBlockCounter\n"); abort(); }
bool CCombatActionBase::IsMeleeFlourish() const { fprintf(stderr, "unexpected CCombatActionBase::IsMeleeFlourish\n"); abort(); }
bool CCombatActionBase::IsActionWithStrikeEventHelperName() const { fprintf(stderr, "unexpected CCombatActionBase::IsActionWithStrikeEventHelperName\n"); abort(); }
bool CCombatActionBase::IsBlockable() const { fprintf(stderr, "unexpected CCombatActionBase::IsBlockable\n"); abort(); }
CTCWeaponTrail* CCombatActionBase::GetWeaponTrailUsedInStrikeAction(long&) const { fprintf(stderr, "unexpected CCombatActionBase::GetWeaponTrailUsedInStrikeAction\n"); abort(); }
bool CCombatActionBase::IsDecapitatingAttack() const { fprintf(stderr, "unexpected CCombatActionBase::IsDecapitatingAttack\n"); abort(); }
bool CCombatActionBase::IsUsingAnimationMovement() const { fprintf(stderr, "unexpected CCombatActionBase::IsUsingAnimationMovement\n"); abort(); }
bool CCombatActionBase::IsMovementUsingPhysics() const { fprintf(stderr, "unexpected CCombatActionBase::IsMovementUsingPhysics\n"); abort(); }
void CCombatActionBase::UpdateActionMoveToPos() { fprintf(stderr, "unexpected CCombatActionBase::UpdateActionMoveToPos\n"); abort(); }
void CCombatActionBase::InitialiseActionMovement() { fprintf(stderr, "unexpected CCombatActionBase::InitialiseActionMovement\n"); abort(); }
CCreatureActionBase* CCombatActionBase::Clone() const { fprintf(stderr, "unexpected CCombatActionBase::Clone\n"); abort(); }
void CCombatActionBase::FrameUpdate() { fprintf(stderr, "unexpected CCombatActionBase::FrameUpdate\n"); abort(); }
void CCombatActionBase::Initialise() { fprintf(stderr, "unexpected CCombatActionBase::Initialise\n"); abort(); }
void CCombatActionBase::FinishAction() { fprintf(stderr, "unexpected CCombatActionBase::FinishAction\n"); abort(); }
C3DVector CCombatActionBase::GetNextPos() const { fprintf(stderr, "unexpected CCombatActionBase::GetNextPos\n"); abort(); }
bool CCombatActionBase::IsToUseRotationInAnimation() const { fprintf(stderr, "unexpected CCombatActionBase::IsToUseRotationInAnimation\n"); abort(); }
bool CCombatActionBase::IsToAlignWithGroundNormal() const { fprintf(stderr, "unexpected CCombatActionBase::IsToAlignWithGroundNormal\n"); abort(); }
bool CCombatActionBase::IsToFaceTarget() const { fprintf(stderr, "unexpected CCombatActionBase::IsToFaceTarget\n"); abort(); }
bool CCombatActionBase::IsToFaceTargetAtStart() const { fprintf(stderr, "unexpected CCombatActionBase::IsToFaceTargetAtStart\n"); abort(); }
C3DVector CCombatActionBase::GetActionMovementVector() const { fprintf(stderr, "unexpected CCombatActionBase::GetActionMovementVector\n"); abort(); }
bool CCombatActionBase::IsToPerformActionOnStrike() const { fprintf(stderr, "unexpected CCombatActionBase::IsToPerformActionOnStrike\n"); abort(); }
bool CCombatActionBase::IsToHitProneThings() const { fprintf(stderr, "unexpected CCombatActionBase::IsToHitProneThings\n"); abort(); }
float CCombatActionBase::GetIsToFaceTargetCone() const { fprintf(stderr, "unexpected CCombatActionBase::GetIsToFaceTargetCone\n"); abort(); }
float CCombatActionBase::GetIsToFaceTargetMinDist() const { fprintf(stderr, "unexpected CCombatActionBase::GetIsToFaceTargetMinDist\n"); abort(); }
float CCombatActionBase::GetDamageMultiplier() const { fprintf(stderr, "unexpected CCombatActionBase::GetDamageMultiplier\n"); abort(); }
void CCombatActionBase::OnAnimationLoaded() { fprintf(stderr, "unexpected CCombatActionBase::OnAnimationLoaded\n"); abort(); }
CCreatureActionBase* CCreatureAction_CombatIdle::Clone() const { fprintf(stderr, "unexpected CCreatureAction_CombatIdle::Clone\n"); abort(); }
void CCreatureAction_CombatIdle::Initialise() { fprintf(stderr, "unexpected CCreatureAction_CombatIdle::Initialise\n"); abort(); }
bool CCreatureAction_CombatIdle::IsToFaceTargetAtStart() const { fprintf(stderr, "unexpected CCreatureAction_CombatIdle::IsToFaceTargetAtStart\n"); abort(); }
bool CResource::UnloadResource() { fprintf(stderr, "unexpected CResource::UnloadResource\n"); abort(); }
unsigned long CResource::GetUnloadableMemory() const { fprintf(stderr, "unexpected CResource::GetUnloadableMemory\n"); abort(); }
bool CResource::IsResourceLoaded() const { fprintf(stderr, "unexpected CResource::IsResourceLoaded\n"); abort(); }
void CResource::OnResourceLoaded() { fprintf(stderr, "unexpected CResource::OnResourceLoaded\n"); abort(); }
bool CResource::CanAbortResourceLoad(long) const { fprintf(stderr, "unexpected CResource::CanAbortResourceLoad\n"); abort(); }
void CResource::OnResourceLoadAborted() { fprintf(stderr, "unexpected CResource::OnResourceLoadAborted\n"); abort(); }
unsigned long CResource::GetDebugMemoryInUse() const { fprintf(stderr, "unexpected CResource::GetDebugMemoryInUse\n"); abort(); }
void CResource::ResetDebugMemoryInUse() { fprintf(stderr, "unexpected CResource::ResetDebugMemoryInUse\n"); abort(); }
bool CDefClassBase::UnloadResource() { fprintf(stderr, "unexpected CDefClassBase::UnloadResource\n"); abort(); }
CDefString CDefClassBase::GetInstantiationName() const { fprintf(stderr, "unexpected CDefClassBase::GetInstantiationName\n"); abort(); }
void CDefClassBase::SetInstantiationName(CDefString) { fprintf(stderr, "unexpected CDefClassBase::SetInstantiationName\n"); abort(); }
void CDefClassBase::AddSubDef(const CCharString&, long, long) { fprintf(stderr, "unexpected CDefClassBase::AddSubDef\n"); abort(); }
void CDefClassBase::RemoveSubDef(const CCharString&) { fprintf(stderr, "unexpected CDefClassBase::RemoveSubDef\n"); abort(); }
CDefClassBase::CSubDefInfo* CDefClassBase::GetPSubDefInfo(const CCharString&) { fprintf(stderr, "unexpected CDefClassBase::GetPSubDefInfo\n"); abort(); }
const CDefClassBase::CSubDefInfo* CDefClassBase::GetPSubDefInfo(const CCharString&) const { fprintf(stderr, "unexpected CDefClassBase::GetPSubDefInfo\n"); abort(); }
bool CDefClassBase::IsParentDef() const { fprintf(stderr, "unexpected CDefClassBase::IsParentDef\n"); abort(); }
void CDefClassBase::PostTransfer() { fprintf(stderr, "unexpected CDefClassBase::PostTransfer\n"); abort(); }
void CDefClassBase::InternalSaveBinary(CDataOutputStream&) { fprintf(stderr, "unexpected CDefClassBase::InternalSaveBinary\n"); abort(); }
void CDefClassBase::InternalLoadBinary(CDataInputStream&) { fprintf(stderr, "unexpected CDefClassBase::InternalLoadBinary\n"); abort(); }
void CDefClassBase::Validate(const CValidateDefinitionInitBase&, const CCharString&) const { fprintf(stderr, "unexpected CDefClassBase::Validate\n"); abort(); }
CVectorMap<unsigned long, CDefClassBase::CSubDefInfo, CKeyPairCompareLess<unsigned long, CDefClassBase::CSubDefInfo> >* CDefClassBase::GetPSubDefInfoMap() { fprintf(stderr, "unexpected CDefClassBase::GetPSubDefInfoMap\n"); abort(); }
CDefString CParentDefClassBase::GetInstantiationName() const { fprintf(stderr, "unexpected CParentDefClassBase::GetInstantiationName\n"); abort(); }
void CParentDefClassBase::SetInstantiationName(CDefString) { fprintf(stderr, "unexpected CParentDefClassBase::SetInstantiationName\n"); abort(); }
void CParentDefClassBase::AddSubDef(const CCharString&, long, long) { fprintf(stderr, "unexpected CParentDefClassBase::AddSubDef\n"); abort(); }
void CParentDefClassBase::RemoveSubDef(const CCharString&) { fprintf(stderr, "unexpected CParentDefClassBase::RemoveSubDef\n"); abort(); }
CDefClassBase::CSubDefInfo* CParentDefClassBase::GetPSubDefInfo(const CCharString&) { fprintf(stderr, "unexpected CParentDefClassBase::GetPSubDefInfo\n"); abort(); }
const CDefClassBase::CSubDefInfo* CParentDefClassBase::GetPSubDefInfo(const CCharString&) const { fprintf(stderr, "unexpected CParentDefClassBase::GetPSubDefInfo\n"); abort(); }
bool CParentDefClassBase::IsParentDef() const { fprintf(stderr, "unexpected CParentDefClassBase::IsParentDef\n"); abort(); }
void CParentDefClassBase::InternalSaveBinary(CDataOutputStream&) { fprintf(stderr, "unexpected CParentDefClassBase::InternalSaveBinary\n"); abort(); }
void CParentDefClassBase::InternalLoadBinary(CDataInputStream&) { fprintf(stderr, "unexpected CParentDefClassBase::InternalLoadBinary\n"); abort(); }
CVectorMap<unsigned long, CDefClassBase::CSubDefInfo, CKeyPairCompareLess<unsigned long, CDefClassBase::CSubDefInfo> >* CParentDefClassBase::GetPSubDefInfoMap() { fprintf(stderr, "unexpected CParentDefClassBase::GetPSubDefInfoMap\n"); abort(); }
void CMazeBattleDef::Transfer(CPersistContext&) { fprintf(stderr, "unexpected CMazeBattleDef::Transfer\n"); abort(); }
void CMazeBattleDef::Copy(const CDefClassBase*) { fprintf(stderr, "unexpected CMazeBattleDef::Copy\n"); abort(); }
unsigned long CMazeBattleDef::GetSizeofClass() { fprintf(stderr, "unexpected CMazeBattleDef::GetSizeofClass\n"); abort(); }
bool CResourceList::HandleFailedAllocation(unsigned long) { abort(); return false; }
CResourceList::~CResourceList() {}
CFailedAllocationHandler::~CFailedAllocationHandler() {}