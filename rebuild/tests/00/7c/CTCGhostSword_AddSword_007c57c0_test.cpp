// CTCGhostSword::AddSword
// Leaf: ?AddSword@CTCGhostSword@@AAEXABVC3DVector@@@Z
#define FABLE_RETAIL_GHOST_SWORD_ADD_PASS27
#define FABLE_RETAIL_STLP_VECTOR_LONG_PUSHBACK_OWNER_PASS26
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTHING_BULLETTIMEOVERRIDE
#define FABLE_RETAIL_CTHING_STATEACCESS
#define FABLE_RETAIL_CTHINGGAMEOBJECT_COMBOMULTIPLIER
#define FABLE_RETAIL_CTHINGCREATUREBASE_FLAGSETTERS
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_DEREFERENCE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_GET_ORIGINAL_CHAIN
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CTCGHOSTSWORD
#define FABLE_RETAIL_CTCGHOSTSWORD_FRAME
#define FABLE_RETAIL_CTHINGAICREATUREINIT_CTOR_BODY
#define FABLE_RETAIL_CTCSOUNDPLAYER
#define FABLE_RETAIL_CTCENEMY_QUERIES
#define FABLE_RETAIL_CTCENEMY_ADDALLYTHING
#define FABLE_RETAIL_CTCREGIONFOLLOWER
#define FABLE_RETAIL_CTCCREATURENAVIGATION
#define FABLE_RETAIL_CTCCREATURENAVIGATION_NAVIGABLE
#define FABLE_RETAIL_CTCSPECIALABILITIES_FULL
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE
#define FABLE_RETAIL_CSPECIALEFFECTSDEF_TYPENAME

#define FABLE_RETAIL_GHOST_SWORD_FIXTURE_PASS27
#define FABLE_RETAIL_SENSED_PASS23
#define FABLE_RETAIL_KID_EVADE_LIFETIMES_PASS23
#define FABLE_RETAIL_CTCDPARTICLEEMITTER_FULL
#define FABLE_RETAIL_CTCSOUNDPLAYER_CTOR_PASS23
#define FABLE_RETAIL_STLP_BITSET_INDEX
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CDEFSTRING
#define FABLE_ONHIT_TEST_ACCESS
#define FABLE_RETAIL_DEFINITION_ORIGINAL_USER_CONSTRUCTORS_PASS22
#define private public
#define protected public
#define FABLE_RETAIL_CINTELLIGENTPOINTER_AICREATURE_SIZE_PASS27
#include "fable_string.h"
#include "engine/CIntelligentPointer.h"
// Fidelity: scaffolding, not retail. Own the 28-byte deleting destructor
// at 0x004D6931 in size mode. The constructor and method below use the
// original speed mode restored by optimize("", on).
#include "engine/CIntelligentPointer_CreatureSizes.h"

#include "engine/CThingAICreature.h"
inline void CIntelligentPointer<CThingAICreature>::SetPointer(const CThingAICreature* pointer) {
    SetPItem(const_cast<CBaseObject*>(static_cast<const CBaseObject*>(pointer)));
}
inline CThingAICreature* CIntelligentPointer<CThingAICreature>::GetPointer() const {
    return static_cast<CThingAICreature*>(GetPItem());
}

inline CIntelligentPointer<CThingAICreature>::CIntelligentPointer(CThingAICreature* pointer) { SetPointer(pointer); }

#include "engine/CThingAICreatureInit.h"
#include "engine/CTCGhostSword.h"
#include "engine/CTCSoundPlayer.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CTCEnemy.h"
#include "engine/CFaction.h"
#include "engine/CTCCreatureNavigation.h"
#include "engine/CTCRegionFollower.h"
#include "engine/CTCSpecialAbilities.h"
#include "engine/CSpecialEffectsDef.h"
#include "engine/CDefClassBase_GetSubDef.h"
#include "engine/_quarantine/CSpecialAbilitiesGhostSwordDef.h"
#include "engine/CTCDParticleEmitter.h"
#include "engine/CTCEffectOnDie.h"
#include "engine/CMainGameComponent.h"
#include "engine/ThingAlive.h"
#undef private
#undef protected

template<> bool CDefClassBase::GetSubDef(CDefPointer<const CSpecialEffectsDef>*) const;


void CTCGhostSword::AddSword(const C3DVector& createPos)
{
    CThingCreatureBase* pcreature = GFDebugPDynamicCast(&GetParentThing(), (CThingCreatureBase*)0);

    CTCSoundPlayer* ptc_sound_player;
    if (pcreature->GetTC(&ptc_sound_player))
        ptc_sound_player->PlayAttachedCriteria("HERO_ABILITY_GHOSTLY_SWORD_APPEAR", true, false, false, -1);

    long def_num = CGameDefinitionManager::Get().GetDefGlobalIndexFromName("GHOSTLY_SWORD");
    CThingAICreatureInit init;
    CIntelligentPointer<CThingAICreature> minion(CThingAICreature::Create(def_num, createPos, pcreature->GetPlayerNumber(), init));
    minion->SetScriptName("HeroSummonnedCreature");

    CTCEnemy* ptc_enemy = 0;
    CTCEnemy* ptc_enemy_parent = 0;
    minion->GetTC(&ptc_enemy);
    pcreature->GetTC(&ptc_enemy_parent);
    ptc_enemy->SetFaction(ptc_enemy_parent->GetPFaction()->GetName());
    ptc_enemy->AddAllyThing(*pcreature);

    CTCCreatureNavigation* ptc_navigation;
    if (minion->GetTC(&ptc_navigation))
        ptc_navigation->SetAsAbleToMoveThroughSolidObjects(true);

    CTCRegionFollower* ptc_region_follower;
    if (pcreature->GetTC(&ptc_region_follower))
        ptc_region_follower->AddFollower(*minion);
    minion->SetAsFollowingHeroThroughTeleporters(true);

    if (!minion->HasTC(TCI_PARTICLE_ATTACHMENT_TRACKER))
        minion->AddTC("CTCParticleAttachmentTracker", false, 0);

    CDefPointer<const CSpecialEffectsDef> pspecial_effects;
    long effect = 0;
    if (PDef->GetSubDef(&pspecial_effects)) {
        if (pspecial_effects->GetEffectFromString("GHOST_SWORD_APPEARING", effect)) {
            CThing* thing = CTCSpecialAbilities::CreateParticleEffect(effect, createPos);
        }
        if (pspecial_effects->GetEffectFromString("GHOST_SWORD_ALIVE_EFFECT", effect)) {
            CThing* sword_effect = CTCSpecialAbilities::CreateParticleEffect(effect, createPos);
            if (IS_THING_ALIVE(sword_effect)) {
                CTCDParticleEmitter* pemitter;
                sword_effect->GetTC(&pemitter);
                if (pemitter)
                    pemitter->AttachToThing(*minion, 10, "EMITTER_BASE", 0, 0.0f);
            }
            MinionEffects.push_back(CIntelligentPointer<CThing>(sword_effect));
        }

        CTCEffectOnDie* ptc_ghost_fx = GFDebugPDynamicCast(minion->AddTC("CTCEffectOnDie", true, 0), (CTCEffectOnDie*)0);
        if (pspecial_effects->GetEffectFromString("GHOST_SWORD_ON_DIE_EFFECT", effect))
            ptc_ghost_fx->SetEffect(effect);
        ptc_ghost_fx->SetTimer((long)(CMainGameComponent::GetConstantFPS() * PDef->TimeOfSummon[SpellLevel]));
        ptc_ghost_fx->SetSoundEffect("HERO_ABILITY_GHOSTLY_SWORD_DISAPPEAR");
        ptc_ghost_fx->SetAlphaFadeOutTimer((long)(CMainGameComponent::GetConstantFPS() * PDef->FadeOutTime));
    }

    minion->SetLeaveDeadBodyOverride(false);
    minion->SetLeaveExperienceOrbsOverride(false);
    minion->SetIsAffectedByBulletTimeOverride(false);
    minion->SetAddToComboMultiplierOnHit(false);

    Minions.push_back(minion);
    CurFadeInTimer.push_back(FadeInTimer);
    pcreature->MakeNoise("SIMPLE_NOISE");
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
CCharString::CCharString(const CCharString& text) : PStringData(text.PStringData) {}
const CCharString& CCharString::operator=(const CCharString& text) { PStringData=text.PStringData; return *this; }
static bool Equals(const CCharString& text, const char* value)
{
    return text.PStringData && strcmp(reinterpret_cast<const char*>(text.PStringData), value) == 0;
}
void CIVCountedPointeeBase::Delete() const { abort(); }
CResource::CResource() {}
CResource::~CResource() {}
CDefPointeeBase::CDefPointeeBase() {}
CDefClassBase::CDefClassBase() {}
CParentDefClassBase::CParentDefClassBase() {}
CParentDefClassBase::~CParentDefClassBase() {}
CSubDefClassBase::CSubDefClassBase() {}
CFaction::~CFaction() {}
CTCGhostSword::CTCGhostSword(CThing& thing) : CTCBase(thing) {}
CTCEnemy::CTCEnemy(CThing& thing) : CTCBase(thing) {}
CTCEnemy::~CTCEnemy() {}
CTCRegionFollower::CTCRegionFollower(CThing& thing) : CTCBase(thing) {}
CTCSoundPlayer::CTCSoundPlayer(CThing& thing) : CTCBase(thing) {}
CTCCreatureNavigation::CTCCreatureNavigation(CThing& thing) : CTCBase(thing) {}
CTCCreatureNavigation::~CTCCreatureNavigation() {}
CTCDriverBase::CTCDriverBase(CThing& thing) : CTCBase(thing) {}
CTCDParticleEmitter::CTCDParticleEmitter(CThing& thing) : CTCDriverBase(thing) {}
CTCDParticleEmitter::~CTCDParticleEmitter() {}
CTCEffectOnDie::CTCEffectOnDie(CThing& thing) : CTCBase(thing) {}

class TestSpecialDef : public CSpecialAbilitiesGhostSwordDef {
public:
    void Transfer(CPersistContext&) { abort(); }
    void Copy(const CDefClassBase*) { abort(); }
    unsigned long GetSizeofClass() { return sizeof(*this); }
};
class TestEffects : public CSpecialEffectsDef {
public:
    void Transfer(CPersistContext&) { abort(); }
    void Copy(const CDefClassBase*) { abort(); }
    unsigned long GetSizeofClass() { return sizeof(*this); }
};
class TestSound : public CTCSoundPlayer {
public:
    TestSound(CThing& thing) : CTCSoundPlayer(thing) {}
    ETCInterfaceType GetInterfaceType() const { return TCI_SOUND_PLAYER; }
    CCharString GetClassTypeName() const { return CCharString(); }
};
class TestNav : public CTCCreatureNavigation {
public:
    TestNav(CThing& thing) : CTCCreatureNavigation(thing) {}
    ETCInterfaceType GetInterfaceType() const { return TCI_NAVIGATION; }
    CCharString GetClassTypeName() const { return CCharString(); }
};
class TestFollower : public CTCRegionFollower {
public:
    TestFollower(CThing& thing) : CTCRegionFollower(thing) {}
    ETCInterfaceType GetInterfaceType() const { return TCI_REGION_FOLLOWER; }
    CCharString GetClassTypeName() const { return CCharString(); }
};
class TestEmitter : public CTCDParticleEmitter {
public:
    TestEmitter(CThing& thing) : CTCDParticleEmitter(thing) {}
    ETCInterfaceType GetInterfaceType() const { return TCI_PARTICLE_EMITTER; }
    CCharString GetClassTypeName() const { return CCharString(); }
};
class TestDie : public CTCEffectOnDie {
public:
    TestDie(CThing& thing) : CTCEffectOnDie(thing) {}
    ETCInterfaceType GetInterfaceType() const { return TCI_EFFECT_ON_DIE; }
    CCharString GetClassTypeName() const { return CCharString(); }
};
class TestSword : public CTCGhostSword {
public:
    TestSword(CThing& thing) : CTCGhostSword(thing) {}
    ETCInterfaceType GetInterfaceType() const { return TCI_GHOST_SWORD; }
    CCharString GetClassTypeName() const { return CCharString(); }
};

static TestEffects* effects;
static TestSpecialDef* ghost_def;
static CThingAICreature* summoner;
static CThingAICreature* sword;
static CThing* effect_actor;
static CThing* active_effect;
static CTCEnemy* sword_enemy;
static CFaction* faction;
static TestSound* sound;
static TestNav* nav;
static TestFollower* follower;
static TestEmitter* emitter;
static TestDie* effect_on_die;
static C3DVector pos(1.0f, 2.0f, 3.0f);
static bool have_defs, have_appear, have_alive, have_die;
static unsigned creates, sounds, factions, allies, navs, followers, trackers, effects_on_die, appears;
static unsigned actives, attaches, combos, noises;
long CMainGameComponent::ConstantFPS=4;

CGameDefinitionManager& __fastcall CGameDefinitionManager::Get()
{
    static unsigned __int64 unused_definition_manager[64];
    return *reinterpret_cast<CGameDefinitionManager*>(unused_definition_manager);
}
long CDefinitionManager::GetDefGlobalIndexFromName(const CCharString& name) const
{
    Require(Equals(name, "GHOSTLY_SWORD"));
    return 91;
}
CThingAICreature* __fastcall CThingAICreature::Create(long definition, const C3DVector& where,
    long player, const CThingAICreatureInit& init)
{
    Require(definition==91 && &where==&pos && player==2);
    Require(!init.OriginalThing && !init.PreviousThing && !init.PlayerCharacter && init.Drawable && init.PlaceAtEndOfList);
    Require(!init.EnableCreatureAutoPlacing && !init.HomeBuilding && !init.WorkBuilding);
    Require(Equals(init.ScriptName,"") && Equals(init.OverridingBrainName,""));
    ++creates;
    return sword;
}
unsigned long CTCSoundPlayer::PlayAttachedCriteria(const CCharString& criteria, bool a, bool b, bool c, long index)
{
    Require(this==sound && Equals(criteria,"HERO_ABILITY_GHOSTLY_SWORD_APPEAR") && a && !b && !c && index==-1);
    ++sounds;
    return 7;
}
void CTCEnemy::SetFaction(const CCharString& name) { Require(this==sword_enemy && Equals(name,"hero-faction")); ++factions; }
void CTCEnemy::AddAllyThing(const CThing& ally) { Require(this==sword_enemy && &ally==summoner); ++allies; }
void CTCCreatureNavigation::SetAsAbleToMoveThroughSolidObjects(bool enabled) { Require(this==nav && enabled); ++navs; }
void CTCRegionFollower::AddFollower(CThing& thing) { Require(this==follower && &thing==sword); ++followers; }
CTCBase* CThing::AddTC(const CCharString& name, bool persistent, const CTCParamsBase* params)
{
    Require(this==sword && !params);
    if (Equals(name,"CTCParticleAttachmentTracker")) {
        Require(!persistent);
        UsingComponents[TCI_PARTICLE_ATTACHMENT_TRACKER]=true;
        ++trackers;
        return 0;
    }
    Require(Equals(name,"CTCEffectOnDie") && persistent);
    ++effects_on_die;
    return effect_on_die;
}
template<> bool CDefClassBase::GetSubDef(CDefPointer<const CSpecialEffectsDef>* out) const
{
    Require(this==ghost_def);
    if (!have_defs) return false;
    out->Object=effects;
    ++effects->IVRefCount;
    return true;
}
bool CSpecialEffectsDef::GetEffectFromString(const CCharString& name, long& index) const
{
    Require(this==effects);
    if (Equals(name,"GHOST_SWORD_APPEARING")) { index=12; return have_appear; }
    if (Equals(name,"GHOST_SWORD_ALIVE_EFFECT")) { index=13; return have_alive; }
    Require(Equals(name,"GHOST_SWORD_ON_DIE_EFFECT"));
    index=14;
    return have_die;
}
CThing* __fastcall CTCSpecialAbilities::CreateParticleEffect(long id, const C3DVector& where)
{
    Require((&where==&pos) && (id==12 || id==13));
    if (id==12) { ++appears; return 0; }
    ++actives;
    return active_effect;
}
void CTCDParticleEmitter::AttachToThing(CThing& thing, long type, const CCharString& bone, long index, float offset)
{
    Require(this==emitter && &thing==sword && type==10 && Equals(bone,"EMITTER_BASE") && index==0 && offset==0.0f);
    ++attaches;
}
void CThingGameObject::SetAddToComboMultiplierOnHit(bool enabled) { Require(this==sword && !enabled); ++combos; }
void CThingCreatureBase::MakeNoise(const CCharString& name) { Require(this==summoner && Equals(name,"SIMPLE_NOISE")); ++noises; }
namespace std {
template<> void vector<CIntelligentPointer<CThing> >::push_back(const CIntelligentPointer<CThing>& value)
{
    insert(end(), value);
}
template<> void vector<CIntelligentPointer<CThingAICreature> >::push_back(const CIntelligentPointer<CThingAICreature>& value)
{
    insert(end(), value);
}
}
static void Put(CThing& thing, ETCInterfaceType key, CTCBase* component)
{
    thing.UsingComponents[key]=true;
    thing.Components.push_back(std::pair<ETCInterfaceType, CTCBase*>(key, component));
}
static void Reset(CTCGhostSword& group)
{
    group.Minions.clear();
    group.MinionEffects.clear();
    group.CurFadeInTimer.clear();
    group.FadeInTimer=17;
    group.SpellLevel=2;
    active_effect=effect_actor;
    effect_actor->Dead=false;
    effect_on_die->Effect=3;
    have_defs=have_appear=have_alive=have_die=true;
    sword->LeaveDeadBodyOverride=sword->LeaveExperienceOrbsOverride=sword->IsAffectedByBulletTimeOverride=true;
    sword->FollowHeroThroughTeleporters=false;
    sword->UsingComponents[TCI_PARTICLE_ATTACHMENT_TRACKER]=false;
    sword->UsingComponents[TCI_NAVIGATION]=true;
    summoner->UsingComponents[TCI_SOUND_PLAYER]=true;
    summoner->UsingComponents[TCI_REGION_FOLLOWER]=true;
    creates=sounds=factions=allies=navs=followers=trackers=effects_on_die=appears=actives=attaches=combos=noises=0;
}
static void Check(CTCGhostSword& group, unsigned sound, unsigned nav, unsigned follow, unsigned tracker,
    unsigned on_die, unsigned appear, unsigned alive, unsigned attach)
{
    group.AddSword(pos);
    Require(creates==1 && factions==1 && allies==1 && combos==1 && noises==1);
    Require(sounds==sound && navs==nav && followers==follow && trackers==tracker);
    Require(effects_on_die==on_die && appears==appear && actives==alive && attaches==attach);
    Require(sword->FollowHeroThroughTeleporters && !sword->LeaveDeadBodyOverride && !sword->LeaveExperienceOrbsOverride && !sword->IsAffectedByBulletTimeOverride);
    Require(Equals(sword->ScriptName,"HeroSummonnedCreature"));
    Require(group.Minions.size()==1 && group.Minions[0].Get()==sword);
    Require(group.CurFadeInTimer.size()==1 && group.CurFadeInTimer[0]==17);
    Require(group.MinionEffects.size()==alive && (alive==0 || group.MinionEffects[0].Get()==active_effect));
    Require(effects->IVRefCount==1 && ghost_def->IVRefCount==2);
    if (on_die) {
        Require(effect_on_die->Effect==(have_die?14:3) && effect_on_die->Timer==26 && effect_on_die->FadeOutTimer==5);
        Require(Equals(effect_on_die->DeathSoundCriteria,"HERO_ABILITY_GHOSTLY_SWORD_DISAPPEAR"));
    }
}
int main()
{
    CThingAICreature owner, summoned;
    CThing tracked_effect;
    TestSpecialDef def;
    TestEffects effect_def;
    CTCEnemy owner_enemy(owner), summoned_enemy(summoned);
    CFaction hero_faction;
    TestSound owner_sound(owner);
    TestNav minion_nav(summoned);
    TestFollower region_follower(owner);
    TestEmitter particle(tracked_effect);
    TestDie death_effect(summoned);
    TestSword group(owner);
    summoner=&owner; sword=&summoned; effect_actor=&tracked_effect; ghost_def=&def; effects=&effect_def;
    sword_enemy=&summoned_enemy; faction=&hero_faction; sound=&owner_sound; nav=&minion_nav; follower=&region_follower;
    emitter=&particle; effect_on_die=&death_effect;
    def.IVRefCount=2;
    effect_def.IVRefCount=1;
    def.TimeOfSummon[2]=6.5f;
    def.FadeOutTime=1.25f;
    group.PDef.Object=&def;
    owner.PlayerNumber=2;
    hero_faction.Name=CCharString("hero-faction");
    owner_enemy.PFaction=faction;
    memset(owner.UsingComponents._M_w,0,sizeof(owner.UsingComponents._M_w));
    memset(summoned.UsingComponents._M_w,0,sizeof(summoned.UsingComponents._M_w));
    memset(tracked_effect.UsingComponents._M_w,0,sizeof(tracked_effect.UsingComponents._M_w));
    Put(owner,TCI_SOUND_PLAYER,&owner_sound);
    Put(owner,TCI_ENEMY,&owner_enemy);
    Put(owner,TCI_REGION_FOLLOWER,&region_follower);
    Put(summoned,TCI_NAVIGATION,&minion_nav);
    Put(summoned,TCI_ENEMY,&summoned_enemy);
    Put(tracked_effect,TCI_PARTICLE_EMITTER,&particle);

    Reset(group); Check(group,1,1,1,1,1,1,1,1);
    Reset(group); have_defs=false; summoned.UsingComponents[TCI_PARTICLE_ATTACHMENT_TRACKER]=true;
    owner.UsingComponents[TCI_SOUND_PLAYER]=false; owner.UsingComponents[TCI_REGION_FOLLOWER]=false;
    summoned.UsingComponents[TCI_NAVIGATION]=false; Check(group,0,0,0,0,0,0,0,0);
    Reset(group); tracked_effect.Dead=true; Check(group,1,1,1,1,1,1,1,0);
    Reset(group); active_effect=0; Check(group,1,1,1,1,1,1,1,0);
    Reset(group); have_alive=false; Check(group,1,1,1,1,1,1,0,0);
    Reset(group); have_appear=false; have_die=false; Check(group,1,1,1,1,1,0,1,1);
    puts("007c57c0_TEST PASS");
}

void CThingGameObject::OnHit(CHitParameters&) { fprintf(stderr, "unexpected CThingGameObject::OnHit\n"); abort(); }
void CThingCreatureBase::OnHit(CHitParameters&) { fprintf(stderr, "unexpected CThingCreatureBase::OnHit\n"); abort(); }
bool CThingCreatureBase::DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo& info) { fprintf(stderr, "unexpected CThingCreatureBase::DrawPreparePrimitivesForRendering\n"); abort(); }
void CTCGhostSword::FrameUpdate() { fprintf(stderr, "unexpected CTCGhostSword::FrameUpdate\n"); abort(); }
ETCInterfaceType CTCEnemy::GetInterfaceType() const { fprintf(stderr, "unexpected CTCEnemy::GetInterfaceType\n"); abort(); }
CCharString CTCEnemy::GetClassTypeName() const { fprintf(stderr, "unexpected CTCEnemy::GetClassTypeName\n"); abort(); }
void CTCEnemy::OnInitialActivate() { fprintf(stderr, "unexpected CTCEnemy::OnInitialActivate\n"); abort(); }
bool CTCEnemy::IsComponentSerialisable() const { fprintf(stderr, "unexpected CTCEnemy::IsComponentSerialisable\n"); abort(); }
void CTCEnemy::OnSerialise(CPersistContext&,bool) { fprintf(stderr, "unexpected CTCEnemy::OnSerialise\n"); abort(); }
ETCInterfaceType CTCDParticleEmitter::GetInterfaceType() const { fprintf(stderr, "unexpected CTCDParticleEmitter::GetInterfaceType\n"); abort(); }
CCharString CTCDParticleEmitter::GetClassTypeName() const { fprintf(stderr, "unexpected CTCDParticleEmitter::GetClassTypeName\n"); abort(); }
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