// CAIStateGroup_JackOfBladesStage1::PowerUpSword()
// Leaf: ?PowerUpSword@CAIStateGroup_JackOfBladesStage1@@IAEXXZ
// Flags: default
#define FABLE_RETAIL_JACK_STAGE1_UPDATE
#define FABLE_RETAIL_CAISTATEGROUP_JACKOFBLADESBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_TCDRIVER
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHING_GETTCDRIVER
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_DEREFERENCE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CINTELLIGENTPOINTER_GET_ORIGINAL_CHAIN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_CTOR_OUTOFLINE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN_OUTOFLINE
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE_PERFORMACTION_NOWAIT
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTION_PLAYCOMBATANIMATION
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTHINGBASEDEF
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CTCBOSS_LIVINGHELPERS
#define FABLE_RETAIL_CTCDPHYSICALOBSTRUCTION_CREATE
#define FABLE_RETAIL_CTCDPARTICLEEMITTER_FULL
#define FABLE_RETAIL_CSPECIALEFFECTSDEF_TYPENAME
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE
#define FABLE_RETAIL_CAIBRAIN_CREATURE_REFERENCE
#define FABLE_RETAIL_CAIBRAIN_CTOR_PASS23
#define FABLE_RETAIL_CCREATUREACTIONBASE_ANIMCTOR
#define FABLE_RETAIL_CDEFSTRING
#define FABLE_RETAIL_ONHIT_TEST_ACCESS
#define FABLE_ONHIT_TEST_ACCESS
#define FABLE_RETAIL_DEF_CLASS_BASE_CTOR_DECL
#define FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_CTOR
#define FABLE_RETAIL_STLP_BITSET_INDEX
#define FABLE_RETAIL_POWERUP_SWORD_FIXTURE_PASS27
#define private public
#define protected public
#include "engine/CThingAICreature.h"
#include "engine/CAIStateGroup_JackOfBladesStage1.h"
#include "engine/CAIBrain.h"
#include "engine/C3DVector.h"
#include "engine/EActionRegisteredType.h"
#include "engine/CSoundPair.h"
#include "engine/CAnimationEntry.h"
#include "engine/CWorld.h"
#include "engine/EAIAction.h"
#include "engine/CCreatureAction_PlayCombatAnimation.h"
#include "engine/CTCBoss.h"
#include "engine/CTCDPhysicalObstruction.h"
#include "engine/CTCDParticleEmitter.h"
#include "engine/CTCCarrying.h"
#include "engine/CThingBaseDef.h"
#include "engine/CSpecialEffectsDef.h"
#include "engine/ThingAlive.h"
#undef private
#undef protected

template<> bool CDefClassBase::GetSubDef(CDefPointer<const CSpecialEffectsDef>* out) const;

bool CAIStateGroup_JackOfBladesBase::IsStillRunnable()
{
    if (!IS_THING_ALIVE(PTargetThing) || IsStopping() || !IS_THING_ALIVE(PArenaCentre))
        return false;
    return true;
}

void CAIStateGroup_JackOfBladesStage1::PowerUpSword()
{
    CTCBoss* ptc_boss;
    GetCreature().GetTC(&ptc_boss);
    AIAction_PerformAction(CCreatureAction_PlayCombatAnimation(GetCreature(), "SUMMON_INTO", true));

    do {
        if (!IsStillRunnable()) {
            Terminate();
            return;
        }

        if (IS_THING_ALIVE(PObstruction)) {
            CTCDPhysicalObstruction* ptcd_obstruction;
            CThing* thing = PObstruction.Get();
            if (thing->HasTC(CTCDPhysicalObstruction::StatGetInterfaceType()))
                ptcd_obstruction = static_cast<CTCDPhysicalObstruction*>(&thing->GetTCDriver());
            ptcd_obstruction->SetLifeTimeSeconds(60.0f);
        }
        AIAction_PerformAction(CCreatureAction_PlayCombatAnimation(GetCreature(), "SUMMON_LOOP", true));
    } while (ptc_boss->CountLivingHelpers() > 0);

    if (!IsStillRunnable()) {
        Terminate();
        return;
    }

    CDefPointer<const CSpecialEffectsDef> pspecial_fx;
    GetCreature().PeekBaseDef().GetSubDef(&pspecial_fx);
    if (pspecial_fx.Get()) {
        long fx_index;
        if (pspecial_fx->GetEffectFromString("SWORD_OF_AEONS", fx_index)) {
            CThing* p_emitter = CTCDParticleEmitter::Create(fx_index, GetCreature().GetPos(), false);
            if (IS_THING_ALIVE(p_emitter)) {
                PSwordEmitter = p_emitter;
                CTCDParticleEmitter* ptcd_emitter = 0;
                if (p_emitter->GetTCDriver(&ptcd_emitter)) {
                    CTCCarrying* ptc_carrying = 0;
                    GetCreature().GetTC(&ptc_carrying);
                    CThing* psword = ptc_carrying->GetThingInPrimarySlot();
                    ptcd_emitter->AttachToThing(*psword, 10, "WEAPON_FOCUS_01", 0, 0.0f);
                }
            }
        }
    }
    AIAction_PerformAction(CCreatureAction_PlayCombatAnimation(GetCreature(), "SUMMON_OUTOF", true));
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


CResource::CResource() {}
CResource::~CResource() {}
CDefClassBase::CDefClassBase() {}
CSubDefClassBase::CSubDefClassBase() {}
CParentDefClassBase::CParentDefClassBase() {}
CParentDefClassBase::~CParentDefClassBase() {}
CAIStateGroupBase::CAIStateGroupBase() {}
CAIBrain::CAIBrain(const CMainGameComponent&, CWorld&, CThingManager&, CPlayerManager&,
    CThingAICreature& creature, CDefPointer<const CBrainDef>) : Creature(creature) {}
void CIVCountedPointeeBase::Delete() const { fprintf(stderr, "unexpected counted delete at %p\n", this); abort(); }

CCharString::CCharString(const char* text, long length)
    : PStringData(reinterpret_cast<CCharStringData*>(const_cast<char*>(text))) { if (length != -1) { fprintf(stderr, "unexpected string length %d\n", length); abort(); } }
static const char* Text(const CCharString& text) { return reinterpret_cast<const char*>(text.PStringData); }
static void RequireAt(bool check, int line)
{
    if (!check) {
        fprintf(stderr, "check failed on line %d\n", line);
        abort();
    }
}
#define Require(check) RequireAt((check), __LINE__)

CTCBoss::CTCBoss(CThing& thing) : CTCBase(thing) {}
ETCInterfaceType CTCBoss::GetInterfaceType() const { return StatGetInterfaceType(); }
CCharString CTCBoss::GetClassTypeName() const { return CCharString(); }
CTCCarrying::CTCCarrying(CThing& thing) : CTCBase(thing) {}
CTCDriverBase::CTCDriverBase(CThing& thing) : CTCBase(thing) {}
CTCDPhysicalObstruction::CTCDPhysicalObstruction(CThing& thing) : CTCDriverBase(thing) {}
CTCDParticleEmitter::CTCDParticleEmitter(CThing& thing) : CTCDriverBase(thing) {}
CTCDParticleEmitter::~CTCDParticleEmitter() {}
ETCInterfaceType CTCDParticleEmitter::GetInterfaceType() const { return StatGetInterfaceType(); }
CCharString CTCDParticleEmitter::GetClassTypeName() const { return CCharString(); }

class TestCarrying : public CTCCarrying {
public:
    TestCarrying(CThing& thing) : CTCCarrying(thing) {}
    ETCInterfaceType GetInterfaceType() const { return StatGetInterfaceType(); }
    CCharString GetClassTypeName() const { return CCharString(); }
};
class TestObstruction : public CTCDPhysicalObstruction {
public:
    TestObstruction(CThing& thing) : CTCDPhysicalObstruction(thing) {}
    ETCInterfaceType GetInterfaceType() const { return StatGetInterfaceType(); }
    CCharString GetClassTypeName() const { return CCharString(); }
};
class TestBaseDef : public CThingBaseDef {
public:
    virtual void Transfer(CPersistContext&) { abort(); }
    virtual void Copy(const CDefClassBase*) { abort(); }
    virtual unsigned long GetSizeofClass() { return sizeof(*this); }
};
class TestEffects : public CSpecialEffectsDef {
public:
    virtual void Transfer(CPersistContext&) { abort(); }
    virtual void Copy(const CDefClassBase*) { abort(); }
    virtual unsigned long GetSizeofClass() { return sizeof(*this); }
};

static CThingAICreature* fighter;
static CThingAICreature* target;
static CThing* weapon;
static CThing* sword_effect;
static CThing* arena;
static CThing* obstruction_thing;
static TestBaseDef* base_def;
static TestEffects* fx_def;
static CTCBoss* creature_boss;
static TestCarrying* carrying;
static TestObstruction* obstruction;
static CTCDParticleEmitter* emitter;
static bool stop, has_def, has_fx, create_fx, kill_after_wait;
static int living_helpers;
static int performed, life_calls, terminated, helpers_checked, looked_up, created, attached;
static const char* animations[8];
static const CCreatureActionBase* current_action;
static CAnimationEntry animation;
static const C3DVector spawn_pos(7.0f, -2.0f, 3.5f);

CCreatureActionBase::CCreatureActionBase(CThingCreatureBase&, float, float,
    const CAnimationEntry&, const CSoundPair&, long, long) {}
CCreatureActionBase::~CCreatureActionBase() {}
CCombatActionBase::CCombatActionBase(CThingCreatureBase& creature, long, const CSoundPair& sound)
    : CCreatureActionBase(creature, 0.0f, 0.0f, animation, sound, 0, 0) {}
CCombatActionBase::~CCombatActionBase() {}
CCreatureAction_PlayCombatAnimation::CCreatureAction_PlayCombatAnimation(
    CThingCreatureBase& creature, const CCharString& name, bool loop)
    : CCombatActionBase(creature, 0, CSoundPair())
{
    Require(&creature == fighter && loop && performed < 8);
    animations[performed] = Text(name);
    current_action = this;
}
EAIActionReturn CAIStateGroupBase::AIAction_PerformAction(const CCreatureActionBase& action)
{
    Require(&action == current_action);
    ++performed;
    return AI_ACTION_FINISHED;
}

bool CAIStateGroupBase::IsStopping() const { return stop; }
void CAIStateGroupBase::Terminate() { ++terminated; }
void CTCDPhysicalObstruction::SetLifeTimeSeconds(float seconds)
{
    Require(this == obstruction && seconds == 60.0f);
    ++life_calls;
}
long CTCBoss::CountLivingHelpers()
{
    Require(this == creature_boss);
    ++helpers_checked;
    if (kill_after_wait)
        target->Dead = true;
    return living_helpers--;
}

template<> bool CDefClassBase::GetSubDef(CDefPointer<const CSpecialEffectsDef>* def) const
{
    Require(this == base_def && def->IsNull());
    if (!has_def)
        return false;
    def->Object = fx_def;
    fx_def->AddRef();
    return true;
}
bool CSpecialEffectsDef::GetEffectFromString(const CCharString& text, long& index) const
{
    Require(this == fx_def && strcmp(Text(text), "SWORD_OF_AEONS") == 0);
    ++looked_up;
    if (!has_fx)
        return false;
    index = 64;
    return true;
}
const C3DVector& CThing::GetPos() const
{
    Require(this == fighter);
    return spawn_pos;
}
CThing* __fastcall CTCDParticleEmitter::Create(long effect, const C3DVector& pos, bool flag)
{
    Require(effect == 64 && &pos == &spawn_pos && !flag);
    ++created;
    return create_fx ? sword_effect : 0;
}
CThing* CTCCarrying::GetThingInPrimarySlot() const
{
    Require(this == carrying);
    return weapon;
}
void CTCDParticleEmitter::AttachToThing(CThing& thing, long type, const CCharString& name, long index, float offset)
{
    Require(this == emitter && &thing == weapon && type == 10 && index == 0 &&
        offset == 0.0f && strcmp(Text(name), "WEAPON_FOCUS_01") == 0);
    ++attached;
}
template<> CIntelligentPointer<CThing>::CIntelligentPointer(CThing* thing) { SetPointer(thing); }
template<> CIntelligentPointer<CThing>& CIntelligentPointer<CThing>::operator=(const CIntelligentPointer<CThing>& other)
{
    SetPointer(other.Get());
    return *this;
}
void CAIStateGroup_JackOfBladesStage1::Update() { abort(); }

static void Reset(CAIStateGroup_JackOfBladesStage1& state)
{
    state.PTargetThing.SetPItem(target);
    state.PArenaCentre.SetPItem(arena);
    state.PObstruction.SetPItem(obstruction_thing);
    state.PSwordEmitter.SetPItem(0);
    target->Dead = false;
    arena->Dead = false;
    sword_effect->Dead = false;
    sword_effect->UsingComponents[TCI_PARTICLE_EMITTER] = true;
    stop = kill_after_wait = false;
    has_def = has_fx = create_fx = true;
    living_helpers = 0;
    performed = life_calls = terminated = helpers_checked = looked_up = created = attached = 0;
}
static void Check(CAIStateGroup_JackOfBladesStage1& state, int loops, int stop_count, int lifetime,
                  int lookup_count, int create_count, int attach_count, bool keep_emitter)
{
    state.PowerUpSword();
    const int expected_animations = 1 + loops + (stop_count ? 0 : 1);
    Require(performed == expected_animations && terminated == stop_count && helpers_checked == loops);
    Require(life_calls == lifetime && looked_up == lookup_count && created == create_count && attached == attach_count);
    Require(state.PSwordEmitter.Get() == (keep_emitter ? sword_effect : 0) && fx_def->GetRefCount() == 1);
    Require(strcmp(animations[0], "SUMMON_INTO") == 0);
    for (int i = 1; i <= loops; ++i)
        Require(strcmp(animations[i], "SUMMON_LOOP") == 0);
    if (!stop_count)
        Require(strcmp(animations[expected_animations - 1], "SUMMON_OUTOF") == 0);
}
int main()
{
    static CThingAICreature ai, victim;
    static CThing arena_storage, sword_storage, obstruction_storage, weapon_storage;
    static CTCBoss boss_component(ai);
    static TestCarrying carrying_component(ai);
    static TestObstruction obstruction_component(obstruction_storage);
    static CTCDParticleEmitter emitter_component(sword_storage);
    static TestBaseDef ai_def;
    static TestEffects fx;
    static CAIStateGroup_JackOfBladesStage1 state;
    static unsigned __int64 game_storage[4], world_storage[4], manager_storage[4], player_storage[4];
    CAIBrain brain(*reinterpret_cast<CMainGameComponent*>(game_storage), *reinterpret_cast<CWorld*>(world_storage),
        *reinterpret_cast<CThingManager*>(manager_storage), *reinterpret_cast<CPlayerManager*>(player_storage),
        ai, CDefPointer<const CBrainDef>());
    state.PBrain = &brain;
    fighter = &ai;
    target = &victim;
    weapon = &weapon_storage;
    arena = &arena_storage;
    sword_effect = &sword_storage;
    obstruction_thing = &obstruction_storage;
    base_def = &ai_def;
    fx_def = &fx;
    creature_boss = &boss_component;
    carrying = &carrying_component;
    emitter = &emitter_component;
    obstruction = &obstruction_component;
    ai.PBaseDef.Object = base_def;
    base_def->AddRef();
    memset(ai.UsingComponents._M_w, 0, sizeof(ai.UsingComponents._M_w));
    memset(obstruction_thing->UsingComponents._M_w, 0, sizeof(obstruction_thing->UsingComponents._M_w));
    memset(sword_effect->UsingComponents._M_w, 0, sizeof(sword_effect->UsingComponents._M_w));
    const ETCInterfaceType boss_key = CTCBoss::StatGetInterfaceType();
    ai.UsingComponents[TCI_CARRYING] = ai.UsingComponents[boss_key] = true;
    ai.Components.push_back(std::pair<ETCInterfaceType, CTCBase*>(TCI_CARRYING, &carrying_component));
    ai.Components.push_back(std::pair<ETCInterfaceType, CTCBase*>(boss_key, &boss_component));
    obstruction_thing->UsingComponents[TCI_PHYSICAL_OBSTRUCTION] = true;
    obstruction_thing->PTCDriver = &obstruction_component;
    sword_effect->PTCDriver = &emitter_component;

    Reset(state); state.PTargetThing.SetPItem(0); Check(state, 0, 1, 0, 0, 0, 0, false);
    Reset(state); arena->Dead = true; Check(state, 0, 1, 0, 0, 0, 0, false);
    Reset(state); stop = true; Check(state, 0, 1, 0, 0, 0, 0, false);
    Reset(state); kill_after_wait = true; Check(state, 1, 1, 1, 0, 0, 0, false);
    Reset(state); has_def = false; Check(state, 1, 0, 1, 0, 0, 0, false);
    Reset(state); has_fx = false; Check(state, 1, 0, 1, 1, 0, 0, false);
    Reset(state); create_fx = false; Check(state, 1, 0, 1, 1, 1, 0, false);
    Reset(state); sword_effect->Dead = true; Check(state, 1, 0, 1, 1, 1, 0, false);
    Reset(state); sword_effect->UsingComponents[TCI_PARTICLE_EMITTER] = false; Check(state, 1, 0, 1, 1, 1, 0, true);
    Reset(state); living_helpers = 1; Check(state, 2, 0, 2, 1, 1, 1, true);
    Reset(state); state.PObstruction.SetPItem(0); Check(state, 1, 0, 0, 1, 1, 1, true);
    ai.PBaseDef.Release();
    puts("00974770_TEST PASS");
}

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
void CCombatActionBase::FrameUpdate() { fprintf(stderr, "unexpected CCombatActionBase::FrameUpdate\n"); abort(); }
void CCombatActionBase::UpdateActionRunningTime() { fprintf(stderr, "unexpected CCombatActionBase::UpdateActionRunningTime\n"); abort(); }
float CCombatActionBase::GetFrameUpdateTime() const { fprintf(stderr, "unexpected CCombatActionBase::GetFrameUpdateTime\n"); abort(); }
void CCombatActionBase::PerformAction() { fprintf(stderr, "unexpected CCombatActionBase::PerformAction\n"); abort(); }
void CCombatActionBase::Initialise() { fprintf(stderr, "unexpected CCombatActionBase::Initialise\n"); abort(); }
long CCombatActionBase::GetAnimationIndex() const { fprintf(stderr, "unexpected CCombatActionBase::GetAnimationIndex\n"); abort(); }
float CCombatActionBase::GetTotalTimeSeconds() const { fprintf(stderr, "unexpected CCombatActionBase::GetTotalTimeSeconds\n"); abort(); }
long CCombatActionBase::GetAnimationTransitionInTime() const { fprintf(stderr, "unexpected CCombatActionBase::GetAnimationTransitionInTime\n"); abort(); }
long CCombatActionBase::GetAnimDelayTime() const { fprintf(stderr, "unexpected CCombatActionBase::GetAnimDelayTime\n"); abort(); }
bool CCombatActionBase::AllowsCombinationAnims() const { fprintf(stderr, "unexpected CCombatActionBase::AllowsCombinationAnims\n"); abort(); }
bool CCombatActionBase::IsActionReadyForInterruption() const { fprintf(stderr, "unexpected CCombatActionBase::IsActionReadyForInterruption\n"); abort(); }
bool CCombatActionBase::IsActionReadyForInterruptionByMovement(const C3DVector & p0) const { fprintf(stderr, "unexpected CCombatActionBase::IsActionReadyForInterruptionByMovement\n"); abort(); }
unsigned long CCombatActionBase::GetAnimExtraFlags() const { fprintf(stderr, "unexpected CCombatActionBase::GetAnimExtraFlags\n"); abort(); }
void CCombatActionBase::OnBlowBlocked(bool p0) { fprintf(stderr, "unexpected CCombatActionBase::OnBlowBlocked\n"); abort(); }
C3DVector CCombatActionBase::GetNextPos() const { fprintf(stderr, "unexpected CCombatActionBase::GetNextPos\n"); abort(); }
bool CCombatActionBase::IsStrikeAction() const { fprintf(stderr, "unexpected CCombatActionBase::IsStrikeAction\n"); abort(); }
bool CCombatActionBase::IsStrikeEvent(long p0) const { fprintf(stderr, "unexpected CCombatActionBase::IsStrikeEvent\n"); abort(); }
bool CCombatActionBase::IsStrikeEventActive() const { fprintf(stderr, "unexpected CCombatActionBase::IsStrikeEventActive\n"); abort(); }
bool CCombatActionBase::IsStrikeEventFinishedBeingActive() const { fprintf(stderr, "unexpected CCombatActionBase::IsStrikeEventFinishedBeingActive\n"); abort(); }
bool CCombatActionBase::IsToUseRotationInAnimation() const { fprintf(stderr, "unexpected CCombatActionBase::IsToUseRotationInAnimation\n"); abort(); }
bool CCombatActionBase::IsToAlignWithGroundNormal() const { fprintf(stderr, "unexpected CCombatActionBase::IsToAlignWithGroundNormal\n"); abort(); }
bool CCombatActionBase::IsToFaceTarget() const { fprintf(stderr, "unexpected CCombatActionBase::IsToFaceTarget\n"); abort(); }
bool CCombatActionBase::IsToFaceTargetAtStart() const { fprintf(stderr, "unexpected CCombatActionBase::IsToFaceTargetAtStart\n"); abort(); }
C3DVector CCombatActionBase::GetActionMovementVector() const { fprintf(stderr, "unexpected CCombatActionBase::GetActionMovementVector\n"); abort(); }
bool CCombatActionBase::IsToPerformActionOnStrike() const { fprintf(stderr, "unexpected CCombatActionBase::IsToPerformActionOnStrike\n"); abort(); }
bool CCombatActionBase::IsToHitProneThings() const { fprintf(stderr, "unexpected CCombatActionBase::IsToHitProneThings\n"); abort(); }
float CCombatActionBase::GetIsToFaceTargetCone() const { fprintf(stderr, "unexpected CCombatActionBase::GetIsToFaceTargetCone\n"); abort(); }
float CCombatActionBase::GetIsToFaceTargetMinDist() const { fprintf(stderr, "unexpected CCombatActionBase::GetIsToFaceTargetMinDist\n"); abort(); }
CCharString CCombatActionBase::GetStrikeEventHelperName() const { fprintf(stderr, "unexpected CCombatActionBase::GetStrikeEventHelperName\n"); abort(); }
float CCombatActionBase::GetDamageMultiplier() const { fprintf(stderr, "unexpected CCombatActionBase::GetDamageMultiplier\n"); abort(); }
CCreatureActionBase * CCombatActionBase::Clone() const { fprintf(stderr, "unexpected CCombatActionBase::Clone\n"); abort(); }
void CCombatActionBase::UpdateActionMoveToPos() { fprintf(stderr, "unexpected CCombatActionBase::UpdateActionMoveToPos\n"); abort(); }
bool CCombatActionBase::IsVitalStrike() const { fprintf(stderr, "unexpected CCombatActionBase::IsVitalStrike\n"); abort(); }
bool CCombatActionBase::IsKnockdownStrike() const { fprintf(stderr, "unexpected CCombatActionBase::IsKnockdownStrike\n"); abort(); }
bool CCombatActionBase::IsBlockCounter() const { fprintf(stderr, "unexpected CCombatActionBase::IsBlockCounter\n"); abort(); }
bool CCombatActionBase::IsMeleeFlourish() const { fprintf(stderr, "unexpected CCombatActionBase::IsMeleeFlourish\n"); abort(); }
bool CCombatActionBase::IsDecapitatingAttack() const { fprintf(stderr, "unexpected CCombatActionBase::IsDecapitatingAttack\n"); abort(); }
bool CCombatActionBase::IsBlockable() const { fprintf(stderr, "unexpected CCombatActionBase::IsBlockable\n"); abort(); }
bool CCombatActionBase::IsUsingAnimationMovement() const { fprintf(stderr, "unexpected CCombatActionBase::IsUsingAnimationMovement\n"); abort(); }
bool CCombatActionBase::IsMovementUsingPhysics() const { fprintf(stderr, "unexpected CCombatActionBase::IsMovementUsingPhysics\n"); abort(); }
void CCombatActionBase::InitialiseActionMovement() { fprintf(stderr, "unexpected CCombatActionBase::InitialiseActionMovement\n"); abort(); }
void CCombatActionBase::FinishAction() { fprintf(stderr, "unexpected CCombatActionBase::FinishAction\n"); abort(); }
CTCWeaponTrail * CCombatActionBase::GetWeaponTrailUsedInStrikeAction(long & p0) const { fprintf(stderr, "unexpected CCombatActionBase::GetWeaponTrailUsedInStrikeAction\n"); abort(); }
bool CCombatActionBase::IsActionWithStrikeEventHelperName() const { fprintf(stderr, "unexpected CCombatActionBase::IsActionWithStrikeEventHelperName\n"); abort(); }
void CCombatActionBase::OnAnimationLoaded() { fprintf(stderr, "unexpected CCombatActionBase::OnAnimationLoaded\n"); abort(); }
CCreatureActionBase* CCreatureAction_PlayCombatAnimation::Clone()const { fprintf(stderr, "unexpected CCreatureAction_PlayCombatAnimation::Clone\n"); abort(); }
bool CCreatureAction_PlayCombatAnimation::IsToFaceTarget()const { fprintf(stderr, "unexpected CCreatureAction_PlayCombatAnimation::IsToFaceTarget\n"); abort(); }
bool CCreatureAction_PlayCombatAnimation::IsMovementUsingPhysics()const { fprintf(stderr, "unexpected CCreatureAction_PlayCombatAnimation::IsMovementUsingPhysics\n"); abort(); }
bool CCreatureAction_PlayCombatAnimation::IsAlwaysUseAnimationMovement()const { fprintf(stderr, "unexpected CCreatureAction_PlayCombatAnimation::IsAlwaysUseAnimationMovement\n"); abort(); }
bool CTCBoss::HasFrameUpdate()const { fprintf(stderr, "unexpected CTCBoss::HasFrameUpdate\n"); abort(); }
void CTCBoss::FrameUpdate() { fprintf(stderr, "unexpected CTCBoss::FrameUpdate\n"); abort(); }
void CTCDPhysicalObstruction::OnInitialActivate() { fprintf(stderr, "unexpected CTCDPhysicalObstruction::OnInitialActivate\n"); abort(); }
bool CTCDParticleEmitter::IsDriverType(long p0) const { fprintf(stderr, "unexpected CTCDParticleEmitter::IsDriverType\n"); abort(); }
void CTCDParticleEmitter::FrameUpdate() { fprintf(stderr, "unexpected CTCDParticleEmitter::FrameUpdate\n"); abort(); }
bool CTCDParticleEmitter::HasFrameUpdate() const { fprintf(stderr, "unexpected CTCDParticleEmitter::HasFrameUpdate\n"); abort(); }
void CTCDParticleEmitter::OnCreate() { fprintf(stderr, "unexpected CTCDParticleEmitter::OnCreate\n"); abort(); }
void CTCDParticleEmitter::OnKill() { fprintf(stderr, "unexpected CTCDParticleEmitter::OnKill\n"); abort(); }
void CTCDParticleEmitter::OnDie() { fprintf(stderr, "unexpected CTCDParticleEmitter::OnDie\n"); abort(); }
void CTCDParticleEmitter::CreateAdditionalTCs() { fprintf(stderr, "unexpected CTCDParticleEmitter::CreateAdditionalTCs\n"); abort(); }
void CTCDParticleEmitter::OnSerialise(CPersistContext & p0, bool p1) { fprintf(stderr, "unexpected CTCDParticleEmitter::OnSerialise\n"); abort(); }
bool CTCDParticleEmitter::IsComponentSerialisable() const { fprintf(stderr, "unexpected CTCDParticleEmitter::IsComponentSerialisable\n"); abort(); }
void CTCDParticleEmitter::DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo & p0) { fprintf(stderr, "unexpected CTCDParticleEmitter::DrawPreparePrimitivesForRendering\n"); abort(); }
void CTCDParticleEmitter::OnSetInLimbo(bool p0) { fprintf(stderr, "unexpected CTCDParticleEmitter::OnSetInLimbo\n"); abort(); }