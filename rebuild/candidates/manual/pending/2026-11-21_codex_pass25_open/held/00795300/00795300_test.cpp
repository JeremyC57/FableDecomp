// Deterministic dependencies for executing the exact final FrameUpdate body.
// These test doubles model caller-visible contracts, not native object layouts.
#define _HAS_EXCEPTIONS 0
#include <assert.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <string>
#include <vector>
#include <list>
#include <map>
#define FABLE_RETAIL_ASSERT(c) ((void)0)
#define FABLE_RETAIL_ASSERT_ARM(c) ((void)0)
struct CCharString {
    std::string text;
    CCharString(const char* s=""):text(s){}
    const char* c_str()const{return text.c_str();}
};
struct CDefString { CCharString text; CDefString(const char* s="sound"):text(s){} operator CCharString()const{return text;} };
struct C2DCoordI { long X,Y;C2DCoordI(long x=0,long y=0):X(x),Y(y){} };
struct C3DVector {
    float X,Y,Z;C3DVector(float x=0,float y=0,float z=0):X(x),Y(y),Z(z){}
    C3DVector operator-(const C3DVector& b)const{return C3DVector(X-b.X,Y-b.Y,Z-b.Z);}
    C3DVector& Normalise(){float m=(float)sqrt(X*X+Y*Y+Z*Z);if(m>0){X/=m;Y/=m;Z/=m;}return *this;}
    C2DCoordI ToC2DCoordI()const{return C2DCoordI((long)X,(long)Y);}
};
struct C2DBoxI { C2DCoordI center;long radius;C2DBoxI(const C2DCoordI& p,long r):center(p),radius(r){} };
template<class T> struct CIntelligentPointer {
    T* p;CIntelligentPointer(T* v=0):p(v){}
    template<class U> CIntelligentPointer(const CIntelligentPointer<U>& v):p(static_cast<T*>(v.Get())){}
    T* Get()const{return p;}T* operator->()const{return p;}T& operator*()const{return *p;}
    CIntelligentPointer& operator=(T* v){p=v;return *this;}
};
template<class T>struct CDefPointer {
    T* p;CDefPointer(T* v=0):p(v){}T* Get()const{return p;}T* operator->()const{return p;}T& operator*()const{return *p;}
};
template<class F,class T>T* GFDebugDynamicCast(F* p,T*){return static_cast<T*>(p);}
template<class F,class T>T GFDebugPDynamicCast(F p,T){return static_cast<T>(p);}
enum ETCInterfaceType { TCI_HERO_STATS=1,TCI_HERO_EXPERIENCE=2,TCI_ENTITY_EVENTS=3,TCI_VOLUME=4,TCI_MODE=5,TCI_WIFE=6,TCI_DEEDS=7,TCI_ENEMY=8,TCI_CONTAINER=60,TCI_NYMPH=61,TCI_CREATURE_TINT_EFFECT=139,TCI_UNHOLY_POWER=140,TCI_DIVINE_WRATH=141 };
enum EDamageAttribute { DAMAGE_ZERO };
enum EQuakeStrength { QUAKE_ZERO };
enum EQuakeLength { LENGTH_ZERO };
enum EMessageEventType { MESSAGE_ZERO };
enum EOpinionDeedType { DEED_ZERO };
enum EOpinionPostDeedType { POST_ZERO };
enum { HERO_ABILITY_UNHOLY_POWER_SPELL=200,HERO_ABILITY_DIVINE_WRATH_SPELL=201 };
class CThing;class CThingCreatureBase;class CHitParameters;
struct CTCBase { virtual ~CTCBase(){} virtual void OnDie(){} };
struct CTCPhysicsBase { float radius;C3DVector position;CTCPhysicsBase():radius(1),position(){}float GetRadius()const{return radius;}const C3DVector& GetPosition()const{return position;} };
namespace NGraphicAppearance {
struct CTCGraphicAppearance {
    mutable int helper_calls;bool helper_ok;int channel,channel_sets;
    CTCGraphicAppearance():helper_calls(0),helper_ok(true),channel(7),channel_sets(0){}
    int PeekLightingChannel()const{return channel;}
    void SetLightingChannel(int n,bool,bool){channel=n;++channel_sets;}
    bool GetHelperPoint(const CCharString&,long,unsigned long,C3DVector& out,float)const{++helper_calls;if(helper_ok)out.Z+=2;return helper_ok;}
};
}
struct CSpecialEffectsDef {
    std::map<std::string,long> entries;
    bool GetEffectFromString(const CCharString& n,long& effect)const{std::map<std::string,long>::const_iterator i=entries.find(n.text);if(i==entries.end())return false;effect=i->second;return true;}
};
struct CThingBaseDef {
    const CSpecialEffectsDef* effects;
    CThingBaseDef():effects(0){}
    bool GetSubDef(CDefPointer<const CSpecialEffectsDef>* out)const{out->p=effects;return effects!=0;}
};
struct CThingCreatureDef:public CThingBaseDef {bool LeaveDeadCreature;CThingCreatureDef():LeaveDeadCreature(false){} };
struct CMessageEventDef {};
struct CHeroAbilityDef {long Stat;float StatExperienceGainedOnUse;CHeroAbilityDef():Stat(12),StatExperienceGainedOnUse(4){} };
struct CTCVolumeContainmentTracker:public CTCBase {static ETCInterfaceType StatGetInterfaceType(){return TCI_VOLUME;}bool update;int writes;CTCVolumeContainmentTracker():update(true),writes(0){}void SetUpdateLightingChannels(bool v){update=v;++writes;} };
struct CTCHeroStats:public CTCBase {static ETCInterfaceType StatGetInterfaceType(){return TCI_HERO_STATS;}long stamina;int changes;CTCHeroStats():stamina(10),changes(0){}void ModifyStaminaBy(long n){stamina+=n;++changes;}long GetStamina()const{return stamina;} };
struct CTCHeroExperience:public CTCBase {static ETCInterfaceType StatGetInterfaceType(){return TCI_HERO_EXPERIENCE;}int calls;float amount;CTCHeroExperience():calls(0),amount(0){}void AddTrainableStatExperience(long,float n){++calls;amount+=n;} };
struct CTCCreatureModeManager:public CTCBase {static ETCInterfaceType StatGetInterfaceType(){return TCI_MODE;}bool active;int adds;CTCCreatureModeManager():active(false),adds(0){}bool IsModeActive(int)const{return active;}void AddMode(int){active=true;++adds;} };
struct CTCNymph:public CTCBase {static ETCInterfaceType StatGetInterfaceType(){return TCI_NYMPH;}int stops;CTCNymph():stops(0){}void StopFlyingAround(long){++stops;} };
struct CTCCreatureTintEffect:public CTCBase {static ETCInterfaceType StatGetInterfaceType(){return TCI_CREATURE_TINT_EFFECT;} };
struct CTCWife:public CTCBase {static ETCInterfaceType StatGetInterfaceType(){return TCI_WIFE;}int murdered;CTCWife():murdered(0){}void Murdered(){++murdered;} };
struct CFaction {};
struct CTCEnemy:public CTCBase {static ETCInterfaceType StatGetInterfaceType(){return TCI_ENEMY;}const CFaction* faction;CTCEnemy():faction(0){}const CFaction* GetPFaction()const{return faction;} };
struct CTCHeroOpinionDeedLog:public CTCBase {static ETCInterfaceType StatGetInterfaceType(){return TCI_DEEDS;}int posts;CTCHeroOpinionDeedLog():posts(0){}void PostDeed(EOpinionDeedType,const C3DVector&,CThing*,int,EOpinionPostDeedType,bool,bool,const CFaction*,bool,int){++posts;} };
struct CTCContainerRewardHero:public CTCBase {int deaths;CTCContainerRewardHero():deaths(0){}void OnDie(){++deaths;} };
namespace NEntityEvents {
struct CEventBase {int kind;CEventBase(int k):kind(k){}virtual ~CEventBase(){} };
struct CEventHitBySpecialAbility:public CEventBase {int ability;CEventHitBySpecialAbility(const CHitParameters&,int a):CEventBase(1),ability(a){} };
struct CEventKilledCreature:public CEventBase {CEventKilledCreature(const CThing&,const CThing&):CEventBase(2){} };
struct CEventDied:public CEventBase {CEventDied(CThing*,CThing*):CEventBase(3){} };
}
struct CTCEntityEvents:public CTCBase {
    static ETCInterfaceType StatGetInterfaceType(){return TCI_ENTITY_EVENTS;}
    int hits,killed,died,last_ability;CTCEntityEvents():hits(0),killed(0),died(0),last_ability(-1){}
    void AddEvent(NEntityEvents::CEventBase* e){if(e->kind==1){++hits;last_ability=static_cast<NEntityEvents::CEventHitBySpecialAbility*>(e)->ability;}if(e->kind==2)++killed;if(e->kind==3)++died;delete e;}
};
struct FakeAction {int kind;bool item;FakeAction(int k):kind(k),item(false){}void SetPItemRemoved(bool v){item=v;} };
struct CThing {
    C3DVector pos;bool dead,immune,spellable,explodable,attackable,underground,player,leave_orbs,influenced;
    float health,ground;unsigned long type_flags;int hits,kills,resets,orbs,removed,last_removed,finished_actions,add_tint;
    float last_damage;int last_attribute;bool saw_orbs,hit_particle,hit_epic,hit_move,hit_knock;
    CTCBase* components[256];CTCBase* owned_tint;CThingCreatureDef definition;CTCPhysicsBase physics;
    NGraphicAppearance::CTCGraphicAppearance graphics;bool graphics_present;
    CThing():pos(),dead(false),immune(false),spellable(true),explodable(false),attackable(true),underground(false),player(false),leave_orbs(true),influenced(false),health(10),ground(0),type_flags(0),hits(0),kills(0),resets(0),orbs(0),removed(0),last_removed(-1),finished_actions(0),add_tint(0),last_damage(0),last_attribute(-1),saw_orbs(true),hit_particle(true),hit_epic(false),hit_move(true),hit_knock(false),owned_tint(0),graphics_present(true){for(int i=0;i<256;++i)components[i]=0;}
    virtual ~CThing(){delete owned_tint;}
    const C3DVector& GetPos()const{return pos;}const C3DVector& PeekPos()const{return pos;}
    bool IsDead()const{return dead;}unsigned long GetTypeFlags()const{return type_flags;}
    const CThingBaseDef& PeekBaseDef()const{return definition;}const CThingBaseDef* GetPBaseDef()const{return &definition;}
    NGraphicAppearance::CTCGraphicAppearance* GetTCGraphicAppearance(){return graphics_present?&graphics:0;}
    CTCPhysicsBase* GetPhysicsTC(){physics.position=pos;return &physics;}
    float GetHeightZBelowAt(const C3DVector&,bool,bool)const{return ground;}
    template<class T>bool GetTC(T** out){return GetTC(T::StatGetInterfaceType(),out);}
    template<class T>bool GetTC(ETCInterfaceType id,T** out){if(!components[id])return false;*out=static_cast<T*>(components[id]);return true;}
    template<class T>bool PeekTC(ETCInterfaceType id,const T** out)const{if(!components[id])return false;*out=static_cast<const T*>(components[id]);return true;}
    bool HasTC(ETCInterfaceType id)const{return components[id]!=0;}
    CTCBase* AddTC(const CCharString&,bool,int){++add_tint;if(!owned_tint)owned_tint=new CTCCreatureTintEffect;components[TCI_CREATURE_TINT_EFFECT]=owned_tint;return owned_tint;}
    void RemoveTC(ETCInterfaceType id){++removed;last_removed=id;}
    void FinishCurrentAction(){++finished_actions;}
    void Kill(bool){++kills;dead=true;}
    virtual void OnHit(CHitParameters& h);
};
struct CThingGameObject:public CThing {};
struct CThingObject:public CThingGameObject {};
// A fixture creature also satisfies the searched object interface for the flag8 object branch.
struct CThingCreatureBase:public CThingObject {
    CThingCreatureBase* attacker;int action_attempts,release_attempts,loop_attempts,build_attempts;bool accept_release,last_item;
    CThingCreatureBase():attacker(0),action_attempts(0),release_attempts(0),loop_attempts(0),build_attempts(0),accept_release(true),last_item(false){type_flags=8;}
    float GetHealth()const{return health;}bool IsPlayerCharacter()const{return player;}
    bool GetLeaveExperienceOrbsOverride()const{return leave_orbs;}void SetLeaveExperienceOrbsOverride(bool v){leave_orbs=v;}
    void SetUnderInfluenceOfEpicSpell(bool v){influenced=v;}
    CThingCreatureBase* GetPLastAttackedByCreature()const{return attacker;}
    bool SetCurrentAction(const FakeAction& a){++action_attempts;if(a.kind==1)++build_attempts;if(a.kind==2)++release_attempts;if(a.kind==3){++loop_attempts;last_item=a.item;}return a.kind!=2||accept_release;}
};
struct CHitParameters {
    float damage;int attribute;bool particle,epic,move,knock;
    CHitParameters(void*,CThing*,CThing*,const C3DVector&,const C3DVector&,bool k,bool,bool,bool):damage(0),attribute(-1),particle(true),epic(false),move(true),knock(k){}
    void AddDamageWithAttribute(EDamageAttribute a,float n){attribute=(int)a;damage+=n;}
    void SetCreateParticleEffectOnHit(bool v){particle=v;}void SetEpicSpell(bool v){epic=v;}void SetMoveBackOnHit(bool v){move=v;}
};
void CThing::OnHit(CHitParameters& h){++hits;last_damage=h.damage;last_attribute=h.attribute;saw_orbs=leave_orbs;hit_particle=h.particle;hit_epic=h.epic;hit_move=h.move;hit_knock=h.knock;health-=h.damage;}
template<class T>bool IS_THING_ALIVE(T* p){return p&&!p->IsDead();}
template<class T>bool IS_THING_ALIVE(const CIntelligentPointer<T>& p){return IS_THING_ALIVE(p.Get());}
struct CTCWillResponse {static bool IsThingNotBloodyWillableAtAll(const CThing* t){return t->immune;}static bool IsThingEpicSpellable(const CThing* t){return t->spellable;}static bool IsThingEpicPowerExplodable(const CThing* t){return t->explodable;} };
namespace NCreatureMode {static bool IsUnderground(const CThingCreatureBase& t){return t.underground;} }
struct CWillButtonHelperTimer {enum EButtonHeldState{BEING_HELD_DOWN,RELEASED};EButtonHeldState state;int updates;CWillButtonHelperTimer():state(BEING_HELD_DOWN),updates(0){}void FrameUpdate(){++updates;}EButtonHeldState GetCurrentButtonHeldState()const{return state;} };
struct CCreatureAction_BuildUpUnholyPowerSpell:public FakeAction {CCreatureAction_BuildUpUnholyPowerSpell(CThingCreatureBase&,const CCharString&):FakeAction(1){} };
struct CCreatureAction_ReleaseUnholyPowerSpell:public FakeAction {CCreatureAction_ReleaseUnholyPowerSpell(CThingCreatureBase&):FakeAction(2){} };
struct CCreatureAction_ReleaseAndLoopUnholyPowerSpell:public FakeAction {CCreatureAction_ReleaseAndLoopUnholyPowerSpell(CThingCreatureBase&):FakeAction(3){} };
struct SpellDef {
    float RadiusOfEffect[4],DamagePerHit[4],RandomFactorForNumSecsBetweenDamageHit;
    CDefString CancelSound[4],ReleaseSound[4],ReleaseLoopSound[4],FinishSound[4];const CSpecialEffectsDef* effects;
    SpellDef():RandomFactorForNumSecsBetweenDamageHit(0),effects(0){for(int i=0;i<4;++i){RadiusOfEffect[i]=5;DamagePerHit[i]=1;}}
    bool GetSubDef(CDefPointer<const CSpecialEffectsDef>* p)const{p->p=effects;return effects!=0;}
};
struct CMainGameComponent {static float GetConstantFPS(){return 25.0f;} };
struct CMessageEventManager {int epic,murder;CMessageEventManager():epic(0),murder(0){}void AddMessage(EMessageEventType,float,const C3DVector&,CThing*,CThing*,int,int,int){++epic;}void AddMessage(const CMessageEventDef&,const C3DVector&,CThing*,CThing*,int){++murder;} };
struct CWorld {CMessageEventManager messages;CMessageEventManager& GetMessageEventManager(){return messages;} };
struct CGameDefinitionManager {
    bool murder_found;CHeroAbilityDef ability;CMessageEventDef message;CGameDefinitionManager():murder_found(true){}
    static CGameDefinitionManager& Get();
    bool GetDef(const CCharString&,CDefPointer<const CHeroAbilityDef>* p)const{p->p=&ability;return true;}
    bool GetDef(const CCharString&,CDefPointer<const CMessageEventDef>* p)const{p->p=murder_found?&message:0;return murder_found;}
};
static CWorld world;static CGameDefinitionManager definitions;
CGameDefinitionManager& CGameDefinitionManager::Get(){return definitions;}
static std::vector<CThingCreatureBase*> search_creatures;static std::vector<CThingObject*> search_objects;
struct CThingFilter_OverlappingCircle {CThingFilter_OverlappingCircle(const C3DVector&,float){} };
struct CThingFilter_CanBeHitByEpicSpell {};
template<class A,class B>struct CFilter_Bind2 {CFilter_Bind2(const A&,const B&){} };
static void Fill(std::list<CThingCreatureBase*>& out){for(unsigned int i=0;i<search_creatures.size();++i)out.push_back(search_creatures[i]);}
static void Fill(std::list<CThingObject*>& out){for(unsigned int i=0;i<search_objects.size();++i)out.push_back(search_objects[i]);}
struct CThingSearchTools {template<class T,class F>void GetAllThingsInRoughArea(const C2DBoxI&,const F&,std::list<T*>& out)const{Fill(out);} };
static CThingSearchTools searches;
struct EffectRecord {long effect;C3DVector pos;EffectRecord(long e,const C3DVector& p):effect(e),pos(p){} };
static std::vector<CThing*> owned_effects;static std::vector<EffectRecord> effect_records;static int fades,rumble_strong,rumble_weak;
struct CTCSpecialAbilities {
    static CThing* CreateParticleEffect(long e,const C3DVector& p){CThing* t=new CThing;t->pos=p;owned_effects.push_back(t);effect_records.push_back(EffectRecord(e,p));return t;}
    static void FadeOrKillEffect(CIntelligentPointer<CThing>& p){++fades;if(p.Get())p->dead=true;}
};
struct CTCDRumble {static CThing* Create(const C3DVector&,EQuakeStrength s,EQuakeLength,bool){if((int)s==3)++rumble_strong;else ++rumble_weak;return 0;} };
struct CTCDExperienceOrb {static void Create(CThing& t){++t.orbs;} };
static const CCharString MAIN_FOCAL_POSITION_NAME("FOCAL");
class CTCUnholyPower {
public:
    struct PerCreatureInfo {CIntelligentPointer<CThingCreatureBase> ActualCreature;long FramesToGetHit;CIntelligentPointer<CThing> Effect;int DefaultChannel;PerCreatureInfo():FramesToGetHit(0),DefaultChannel(0){} };
    struct PerEffectInfo {C3DVector Position;CIntelligentPointer<CThing> Effect;};
    typedef std::map<CThingCreatureBase*,PerCreatureInfo> MapOfCreaturesToPerCreatureInfo;
    typedef std::vector<PerEffectInfo> VectorOfCreaturesToPerEffectInfo;
    MapOfCreaturesToPerCreatureInfo LastFrameCreaturesHit;VectorOfCreaturesToPerEffectInfo HitEffects;
    bool initialized,charging,Finished,PerformedReleaseAction,PItemRemoved;long NumFramesToWaitForFadeOut;
    int CurFramesForEnvChange,NumFramesForEnvChange,SpellLevel,StaminaModify,DurationOfSpellFrames,NumFramesBetweenDamageHit,SpellExclusionChannel;
    float AccModifyStamina,PerFrameStaminaModify;unsigned long LoopingSound;
    CDefPointer<const SpellDef> PDef;CWillButtonHelperTimer Timer;CThingCreatureBase& parent;
    int targets,unsheathes,finishes,sounds,stops;unsigned long last_stopped;
    CTCUnholyPower(CThingCreatureBase& p,const SpellDef& d):initialized(true),charging(true),Finished(false),PerformedReleaseAction(false),PItemRemoved(false),NumFramesToWaitForFadeOut(0),CurFramesForEnvChange(0),NumFramesForEnvChange(3),SpellLevel(0),StaminaModify(0),DurationOfSpellFrames(3),NumFramesBetweenDamageHit(3),SpellExclusionChannel(99),AccModifyStamina(0),PerFrameStaminaModify(0),LoopingSound(17),PDef(&d),parent(p),targets(0),unsheathes(0),finishes(0),sounds(0),stops(0),last_stopped(0){}
    bool HasUnholyPowerActionInitialised()const{return initialized;}bool IsCharging()const{return charging;}
    CThing& GetParentThing(){return parent;}CWorld& GetWorld(){return world;}
    CGameDefinitionManager& GetDefinitionManager(){return definitions;}
    const CThingSearchTools& PeekThingSearchTools()const{return searches;}
    bool Attackable(CThingCreatureBase* t)const{return t->attackable;}
    void SetPTarget(){++targets;}void UnsheatheWeapon(){++unsheathes;}void FinishMove(){++finishes;Finished=true;}
    unsigned long PlaySoundW(const CCharString&,bool){return (unsigned long)(100+(++sounds));}
    void StopSound(unsigned long n){++stops;last_stopped=n;}
    void ResetCreatureHitState(CIntelligentPointer<CThingCreatureBase> p){if(p.Get()){++p->resets;p->influenced=false;}}
    void FrameUpdate();
};

// Exact complete production body; source SHA256: 763733e57b859122badd4138040783c0fa4b4632968d9db63c7c94449b3fafba
// PRODUCTION_BODY_BEGIN
void CTCUnholyPower::FrameUpdate()
{
    if (!HasUnholyPowerActionInitialised())
        return;
    Timer.FrameUpdate();
    CThingCreatureBase& parent_creature = *GFDebugDynamicCast(&GetParentThing(), (CThingCreatureBase*)0);
    CWillButtonHelperTimer::EButtonHeldState buttonstate = Timer.GetCurrentButtonHeldState();
    if (Finished) {
        if (NumFramesToWaitForFadeOut <= 0)
            parent_creature.RemoveTC(TCI_UNHOLY_POWER);
        else
            --NumFramesToWaitForFadeOut;
        return;
    }
    if (IsCharging()) {
        if (++CurFramesForEnvChange >= NumFramesForEnvChange)
            SetPTarget();
        parent_creature.SetCurrentAction(
            CCreatureAction_BuildUpUnholyPowerSpell(parent_creature, "ST_BUILD_UNHOLY_POWER"));
        if (buttonstate != CWillButtonHelperTimer::BEING_HELD_DOWN) {
            PlaySoundW(PDef->CancelSound[SpellLevel], false);
            parent_creature.FinishCurrentAction();
            UnsheatheWeapon();
            FinishMove();
        } else {
            CTCHeroStats* ptc_hero_stats;
            if (!parent_creature.GetTC(&ptc_hero_stats))
                FABLE_RETAIL_ASSERT(false);
            long modify = StaminaModify;
            AccModifyStamina += PerFrameStaminaModify;
            while (AccModifyStamina > 1.0f) {
                AccModifyStamina -= 1.0f;
                ++modify;
            }
            ptc_hero_stats->ModifyStaminaBy(-modify);
            if (ptc_hero_stats->GetStamina() <= 0) {
                PlaySoundW(PDef->CancelSound[SpellLevel], false);
                parent_creature.FinishCurrentAction();
                UnsheatheWeapon();
                FinishMove();
            }
        }
        float search_radius = PDef->RadiusOfEffect[SpellLevel];
        C3DVector targetpos = parent_creature.GetPos();
        C2DBoxI search_area(targetpos.ToC2DCoordI(), (long)(search_radius + 20.0f));
        std::list<CThingCreatureBase*> creatures;
        PeekThingSearchTools().GetAllThingsInRoughArea<CThingCreatureBase>(
            search_area,
            CFilter_Bind2<CThingFilter_OverlappingCircle, CThingFilter_CanBeHitByEpicSpell>(
                CThingFilter_OverlappingCircle(targetpos, search_radius), CThingFilter_CanBeHitByEpicSpell()),
            creatures);
        for (std::list<CThingCreatureBase*>::iterator it = creatures.begin(), endit = creatures.end();
             it != endit; ++it) {
            CThingCreatureBase* something = *it;
            if (something == &parent_creature)
                continue;
            if (CTCWillResponse::IsThingNotBloodyWillableAtAll(something))
                continue;
            if (!CTCWillResponse::IsThingEpicSpellable(something))
                continue;
            if (!Attackable(something))
                continue;
            if (NCreatureMode::IsUnderground(*something))
                continue;
            MapOfCreaturesToPerCreatureInfo::iterator findit = LastFrameCreaturesHit.find(something);
            if (findit != LastFrameCreaturesHit.end())
                continue;
            if (!IS_THING_ALIVE(something))
                continue;
            if (!(something->GetHealth() > 0.0f))
                continue;
            LastFrameCreaturesHit[something].ActualCreature = something;
            LastFrameCreaturesHit[something].FramesToGetHit = 0;
            NGraphicAppearance::CTCGraphicAppearance* ptc_graphic_appearance =
                something->GetTCGraphicAppearance();
            if (ptc_graphic_appearance) {
                LastFrameCreaturesHit[something].DefaultChannel =
                    ptc_graphic_appearance->PeekLightingChannel();
                ptc_graphic_appearance->SetLightingChannel(SpellExclusionChannel, true, true);
            }
            CTCVolumeContainmentTracker* ptc_volume_contain = 0;
            if (something->GetTC(&ptc_volume_contain))
                ptc_volume_contain->SetUpdateLightingChannels(false);
        }
    } else {

        CDefPointer<const CSpecialEffectsDef> pspecial_effects;
        if (!PDef->GetSubDef(&pspecial_effects))
            FABLE_RETAIL_ASSERT(false);
        if (CurFramesForEnvChange == NumFramesForEnvChange) {
            ++CurFramesForEnvChange;
            if (pspecial_effects.Get()) {
                long effect = 0;
                if (pspecial_effects->GetEffectFromString("RELEASE_EFFECT", effect)) {
                    CThing* fx = CTCSpecialAbilities::CreateParticleEffect(effect, parent_creature.GetPos());
                }
                if (pspecial_effects->GetEffectFromString("RELEASE_COLUMN_EFFECT", effect)) {
                    CThing* fx = CTCSpecialAbilities::CreateParticleEffect(effect, parent_creature.GetPos());
                }
            }
            CTCDRumble::Create(parent_creature.GetPos(), (EQuakeStrength)3, (EQuakeLength)0, false);
            PlaySoundW(PDef->ReleaseSound[SpellLevel], false);
            StopSound(LoopingSound);
            LoopingSound = PlaySoundW(PDef->ReleaseLoopSound[SpellLevel], false);
        }
        if (DurationOfSpellFrames > 0) {
            --DurationOfSpellFrames;
            float damage = PDef->DamagePerHit[SpellLevel];
            float search_radius = PDef->RadiusOfEffect[SpellLevel];
            C3DVector targetpos = parent_creature.GetPos();
            C2DBoxI search_area(targetpos.ToC2DCoordI(), (long)(search_radius + 20.0f));
            if (!PerformedReleaseAction) {
                PerformedReleaseAction = parent_creature.SetCurrentAction(
                    CCreatureAction_ReleaseUnholyPowerSpell(parent_creature));
                if (PerformedReleaseAction) {
                    CTCHeroExperience* ptc_experience;
                    if (parent_creature.GetTC(&ptc_experience)) {
                        CDefPointer<const CHeroAbilityDef> pdef;
                        CGameDefinitionManager::Get().GetDef("HERO_ABILITY_SKILL_UNHOLY_POWER_SPELL", &pdef);
                        ptc_experience->AddTrainableStatExperience(pdef->Stat,
                                                                   pdef->StatExperienceGainedOnUse);
                    }
                    std::list<CThingObject*> objects;
                    PeekThingSearchTools().GetAllThingsInRoughArea<CThingObject>(
                        search_area,
                        CFilter_Bind2<CThingFilter_OverlappingCircle, CThingFilter_CanBeHitByEpicSpell>(
                            CThingFilter_OverlappingCircle(targetpos, search_radius),
                            CThingFilter_CanBeHitByEpicSpell()),
                        objects);
                    std::list<CThingObject*>::iterator it = objects.begin(), endit = objects.end();
                    while (it != endit) {
                        CThingObject* something = *it;
                        if (IS_THING_ALIVE(something) &&
                            !CTCWillResponse::IsThingNotBloodyWillableAtAll(something) &&
                            CTCWillResponse::IsThingEpicSpellable(something)) {
                            C3DVector direction = something->GetPos() - parent_creature.GetPos();
                            direction.Normalise();
                            C3DVector point_of_hit = something->GetPos() - direction;
                            CHitParameters hit_param(0, &parent_creature, 0, point_of_hit, direction, true,
                                                     false, false, false);
                            hit_param.AddDamageWithAttribute((EDamageAttribute)10, damage * 100.0f);
                            hit_param.SetCreateParticleEffectOnHit(false);
                            hit_param.SetEpicSpell(true);
                            bool do_exp_override = false;
                            CThingGameObject* somethingobject = something;
                            if (somethingobject->GetTypeFlags() & 8) {
                                CThingCreatureBase* somethingcreaturebase =
                                    GFDebugPDynamicCast(somethingobject, (CThingCreatureBase*)0);
                                const CThingCreatureDef& creature_def = *GFDebugDynamicCast(
                                    &somethingcreaturebase->PeekBaseDef(), (const CThingCreatureDef*)0);
                                if (!creature_def.LeaveDeadCreature &&
                                    CTCWillResponse::IsThingEpicPowerExplodable(somethingcreaturebase))
                                    do_exp_override = true;
                            }
                            bool prev_exp_override_flag = true;
                            if (do_exp_override) {
                                CThingCreatureBase* somethingcreaturebase =
                                    GFDebugPDynamicCast(somethingobject, (CThingCreatureBase*)0);
                                prev_exp_override_flag =
                                    somethingcreaturebase->GetLeaveExperienceOrbsOverride();
                                somethingcreaturebase->SetLeaveExperienceOrbsOverride(false);
                            }
                            somethingobject->OnHit(hit_param);
                            if (do_exp_override) {
                                CThingCreatureBase* somethingcreaturebase =
                                    GFDebugPDynamicCast(somethingobject, (CThingCreatureBase*)0);
                                somethingcreaturebase->SetLeaveExperienceOrbsOverride(prev_exp_override_flag);
                            }
                            CTCEntityEvents* ptc_entity_events;
                            if (something->GetTC(&ptc_entity_events))
                                ptc_entity_events->AddEvent(new NEntityEvents::CEventHitBySpecialAbility(
                                    hit_param, HERO_ABILITY_UNHOLY_POWER_SPELL));
                        }
                        ++it;
                    }
                }
            } else {
                CCreatureAction_ReleaseAndLoopUnholyPowerSpell finalaction(parent_creature);
                finalaction.SetPItemRemoved(PItemRemoved);
                parent_creature.SetCurrentAction(finalaction);
            }
            CTCDRumble::Create(parent_creature.GetPos(), (EQuakeStrength)0, (EQuakeLength)0, false);

            std::list<CThingCreatureBase*> creatures;
            PeekThingSearchTools().GetAllThingsInRoughArea<CThingCreatureBase>(
                search_area,
                CFilter_Bind2<CThingFilter_OverlappingCircle, CThingFilter_CanBeHitByEpicSpell>(
                    CThingFilter_OverlappingCircle(targetpos, search_radius),
                    CThingFilter_CanBeHitByEpicSpell()),
                creatures);
            for (std::list<CThingCreatureBase*>::iterator it = creatures.begin(), endit = creatures.end();
                 it != endit; ++it) {
                CThingCreatureBase* something = *it;
                if (something == &parent_creature)
                    continue;
                if (CTCWillResponse::IsThingNotBloodyWillableAtAll(something))
                    continue;
                if (!CTCWillResponse::IsThingEpicSpellable(something))
                    continue;
                if (!Attackable(something))
                    continue;
                if (NCreatureMode::IsUnderground(*something))
                    continue;
                MapOfCreaturesToPerCreatureInfo::iterator findit = LastFrameCreaturesHit.find(something);
                if (findit == LastFrameCreaturesHit.end()) {
                    if (!IS_THING_ALIVE(something))
                        continue;
                    if (!(something->GetHealth() > 0.0f))
                        continue;
                    LastFrameCreaturesHit[something].ActualCreature = something;
                    LastFrameCreaturesHit[something].FramesToGetHit = 0;
                    long effect = 0;
                    CCharString effectname("VICTIM_EFFECT");
                    if (pspecial_effects.Get() && pspecial_effects->GetEffectFromString(effectname, effect)) {
                        C3DVector effect_pos = something->GetPos();
                        const NGraphicAppearance::CTCGraphicAppearance* ptc_graphic_appearance =
                            something->GetTCGraphicAppearance();
                        if (ptc_graphic_appearance) {
                            if (!ptc_graphic_appearance->GetHelperPoint("TARGET_CURSOR_POINT", 0, 0,
                                                                        effect_pos, 1.0f))
                                FABLE_RETAIL_ASSERT_ARM(false);
                        }
                        float groundZ = something->GetHeightZBelowAt(effect_pos, false, true);
                        effect_pos.Z = groundZ;
                        CThing* fx = CTCSpecialAbilities::CreateParticleEffect(effect, effect_pos);
                        LastFrameCreaturesHit[something].Effect = fx;
                        PerEffectInfo info;
                        info.Position = effect_pos;
                        info.Effect = LastFrameCreaturesHit[something].Effect;
                        HitEffects.push_back(info);
                    }
                    NGraphicAppearance::CTCGraphicAppearance* ptc_graphic_appearance =
                        something->GetTCGraphicAppearance();
                    if (ptc_graphic_appearance) {
                        LastFrameCreaturesHit[something].DefaultChannel =
                            ptc_graphic_appearance->PeekLightingChannel();
                        ptc_graphic_appearance->SetLightingChannel(SpellExclusionChannel, true, true);
                    }
                    CTCVolumeContainmentTracker* ptc_volume_contain = 0;
                    if (something->GetTC(&ptc_volume_contain))
                        ptc_volume_contain->SetUpdateLightingChannels(false);
                } else {
                    if (!IS_THING_ALIVE(something))
                        continue;
                    if (IS_THING_ALIVE(findit->second.Effect))
                        continue;
                    long effect = 0;
                    CCharString effectname("VICTIM_EFFECT");
                    if (pspecial_effects.Get() && pspecial_effects->GetEffectFromString(effectname, effect)) {
                        C3DVector effect_pos = something->GetPos();
                        const NGraphicAppearance::CTCGraphicAppearance* ptc_graphic_appearance =
                            something->GetTCGraphicAppearance();
                        if (ptc_graphic_appearance) {
                            if (!ptc_graphic_appearance->GetHelperPoint("TARGET_CURSOR_POINT", 0, 0,
                                                                        effect_pos, 1.0f))
                                FABLE_RETAIL_ASSERT_ARM(false);
                        }
                        float groundZ = something->GetHeightZBelowAt(effect_pos, false, true);
                        effect_pos.Z = groundZ;
                        CThing* fx = CTCSpecialAbilities::CreateParticleEffect(effect, effect_pos);
                        findit->second.Effect = fx;
                        PerEffectInfo info;
                        info.Position = effect_pos;
                        info.Effect = findit->second.Effect;
                        HitEffects.push_back(info);
                    }
                };
                ;
            }

            {
                MapOfCreaturesToPerCreatureInfo::iterator it = LastFrameCreaturesHit.begin();
                if (it != LastFrameCreaturesHit.end())
                    for (; it != LastFrameCreaturesHit.end();) {
                        CIntelligentPointer<CThingCreatureBase> something = it->second.ActualCreature;
                        if (!IS_THING_ALIVE(something)) {
                            ++it;
                        } else {
                            float radiusPlusRadius = search_radius + something->GetPhysicsTC()->GetRadius();
                            C3DVector d = something->GetPos() - parent_creature.GetPos();
                            static const float GUMPH = 5.0f;
                            if (!((d.Y * d.Y + d.Z * d.Z + d.X * d.X) >
                                  radiusPlusRadius * radiusPlusRadius + GUMPH) &&
                                !CTCWillResponse::IsThingNotBloodyWillableAtAll(something.Get()) &&
                                CTCWillResponse::IsThingEpicSpellable(something.Get())) {
                                ++it;
                            } else {
                                ResetCreatureHitState(something);
                                CTCSpecialAbilities::FadeOrKillEffect(it->second.Effect);
                                if (IS_THING_ALIVE(something)) {
                                    NGraphicAppearance::CTCGraphicAppearance* ptc_graphic_appearance =
                                        something->GetTCGraphicAppearance();
                                    if (ptc_graphic_appearance)
                                        ptc_graphic_appearance->SetLightingChannel(it->second.DefaultChannel,
                                                                                   true, true);
                                    CTCVolumeContainmentTracker* ptc_volume_contain = 0;
                                    if (something->GetTC(&ptc_volume_contain))
                                        ptc_volume_contain->SetUpdateLightingChannels(true);
                                    something->SetUnderInfluenceOfEpicSpell(false);
                                }
                                MapOfCreaturesToPerCreatureInfo::iterator tmp = it;
                                ++it;
                                LastFrameCreaturesHit.erase(tmp);
                            }
                        }
                    }
            }
            {
                VectorOfCreaturesToPerEffectInfo::iterator it = HitEffects.begin(), endit = HitEffects.end();
                while (it != endit) {
                    PerEffectInfo& info = *it;
                    if (!IS_THING_ALIVE(info.Effect)) {
                        long effect = 0;
                        CCharString effectname("VICTIM_EFFECT");
                        if (pspecial_effects.Get() &&
                            pspecial_effects->GetEffectFromString(effectname, effect))
                            info.Effect = CTCSpecialAbilities::CreateParticleEffect(effect, info.Position);
                    }
                    ++it;
                }
            }
            for (MapOfCreaturesToPerCreatureInfo::iterator it = LastFrameCreaturesHit.begin();
                 it != LastFrameCreaturesHit.end();) {
                CIntelligentPointer<CThingCreatureBase> something = it->second.ActualCreature;
                if (IS_THING_ALIVE(something)) {
                    CTCNymph* ptc_nymph;
                    if (something->GetTC(&ptc_nymph))
                        ptc_nymph->StopFlyingAround(0);
                    if (--it->second.FramesToGetHit <= 0) {
                        it->second.FramesToGetHit = (long)(CMainGameComponent::GetConstantFPS() *
                                                           PDef->RandomFactorForNumSecsBetweenDamageHit) +
                                                    NumFramesBetweenDamageHit;
                        C3DVector direction = something->GetPos() - parent_creature.GetPos();
                        direction.Normalise();
                        C3DVector point_of_hit = something->GetPos() - direction;
                        CHitParameters hit_param(0, &parent_creature, 0, point_of_hit, direction, false,
                                                 false, false, false);
                        hit_param.AddDamageWithAttribute((EDamageAttribute)10, damage);
                        hit_param.SetCreateParticleEffectOnHit(false);
                        hit_param.SetMoveBackOnHit(false);
                        hit_param.SetEpicSpell(true);
                        CTCCreatureModeManager* ptc_cmm;
                        if (something->GetTC(&ptc_cmm)) {
                            if (!ptc_cmm->IsModeActive(0x5F)) {
                                something->FinishCurrentAction();
                                ptc_cmm->AddMode(0x5F);
                            }
                        }
                        bool do_exp_override = false;
                        const CThingCreatureDef& creature_def =
                            *GFDebugDynamicCast(&something->PeekBaseDef(), (const CThingCreatureDef*)0);
                        if (!creature_def.LeaveDeadCreature &&
                            CTCWillResponse::IsThingEpicPowerExplodable(something.Get()))
                            do_exp_override = true;
                        bool prev_exp_override_flag = true;
                        if (do_exp_override) {
                            prev_exp_override_flag = something->GetLeaveExperienceOrbsOverride();
                            something->SetLeaveExperienceOrbsOverride(false);
                        }
                        something->OnHit(hit_param);
                        CTCCreatureTintEffect* ptc_tint;
                        if (!something->GetTC(&ptc_tint))
                            ptc_tint = GFDebugPDynamicCast(something->AddTC("CTCCreatureTintEffect", true, 0),
                                                           (CTCCreatureTintEffect*)0);
                        something->SetUnderInfluenceOfEpicSpell(true);
                        if (do_exp_override)
                            something->SetLeaveExperienceOrbsOverride(prev_exp_override_flag);
                        CTCEntityEvents* ptc_entity_events;
                        if (something->GetTC(&ptc_entity_events))
                            ptc_entity_events->AddEvent(new NEntityEvents::CEventHitBySpecialAbility(
                                hit_param, HERO_ABILITY_UNHOLY_POWER_SPELL));
                    }
                }
                if (something.Get() && !something->IsDead() && !(something->GetHealth() <= 0.0f)) {
                    ++it;
                } else {
                    if (something.Get()) {
                        if (CTCWillResponse::IsThingEpicPowerExplodable(something.Get())) {

                            GetWorld().GetMessageEventManager().AddMessage(
                                (EMessageEventType)9, CMainGameComponent::GetConstantFPS(),
                                something->GetPhysicsTC()->GetPosition(), &GetParentThing(), something.Get(),
                                0, 0, 0);
                            CTCEntityEvents* ptc_attacker_entity_events;
                            if (GetParentThing().GetTC(&ptc_attacker_entity_events))
                                ptc_attacker_entity_events->AddEvent(new NEntityEvents::CEventKilledCreature(
                                    GetParentThing(), *something.Get()));
                            CTCDExperienceOrb::Create(*something.Get());
                            CTCContainerRewardHero* ptc_reward;
                            if (something->GetTC(TCI_CONTAINER, &ptc_reward))
                                ptc_reward->OnDie();
                            something->Kill(false);
                            CDefPointer<const CMessageEventDef> pdef;
                            CThingCreatureBase* my_attacker = something->GetPLastAttackedByCreature();
                            if (my_attacker && my_attacker->IsPlayerCharacter()) {
                                if (GetDefinitionManager().GetDef("STANDARD_MESSAGE_EVENT_MURDER", &pdef))
                                    GetWorld().GetMessageEventManager().AddMessage(
                                        *pdef, something->PeekPos(), my_attacker, something.Get(), 0);
                                else
                                    FABLE_RETAIL_ASSERT_ARM(false);
                            }
                            CTCWife* tc_wife = 0;
                            something->GetTC(&tc_wife);
                            CTCHeroOpinionDeedLog* ptc_deed_log;
                            if (my_attacker && my_attacker->GetTC(&ptc_deed_log)) {
                                const CTCEnemy* pmy_enemy_tc;
                                if (something->PeekTC(TCI_ENEMY, &pmy_enemy_tc)) {
                                    if (pmy_enemy_tc->GetPFaction()) {
                                        ptc_deed_log->PostDeed((EOpinionDeedType)9, something->PeekPos(),
                                                               something.Get(), 0, (EOpinionPostDeedType)2,
                                                               true, false, pmy_enemy_tc->GetPFaction(),
                                                               false, 0);
                                        ptc_deed_log->PostDeed((EOpinionDeedType)0x3A, something->PeekPos(),
                                                               something.Get(), 0, (EOpinionPostDeedType)7,
                                                               false, false, pmy_enemy_tc->GetPFaction(),
                                                               true, 0);
                                    }
                                }
                                if (tc_wife)
                                    tc_wife->Murdered();
                            }
                            CTCEntityEvents* ptc_entity_events;
                            if (something->GetTC(&ptc_entity_events))
                                ptc_entity_events->AddEvent(
                                    new NEntityEvents::CEventDied(my_attacker, something.Get()));
                            CDefPointer<const CSpecialEffectsDef> pcreature_special_effects;
                            bool create_custom_explode_effect = false;
                            if (something->GetPBaseDef()->GetSubDef(&pcreature_special_effects)) {
                                long effect = 0;
                                if (pcreature_special_effects->GetEffectFromString("VICTIM_EXPLODE_EFFECT",
                                                                                   effect)) {
                                    C3DVector focus = something->GetPos();
                                    const NGraphicAppearance::CTCGraphicAppearance* ptc_graphic_appearance =
                                        something->GetTCGraphicAppearance();
                                    if (ptc_graphic_appearance) {
                                        if (!ptc_graphic_appearance->GetHelperPoint(MAIN_FOCAL_POSITION_NAME,
                                                                                    0, 0, focus, 1.0f))
                                            FABLE_RETAIL_ASSERT_ARM(false);
                                    }
                                    CThing* fx = CTCSpecialAbilities::CreateParticleEffect(effect, focus);
                                    create_custom_explode_effect = true;
                                }
                            }
                            if (!create_custom_explode_effect && pspecial_effects.Get()) {
                                long effect = 0;
                                if (pspecial_effects->GetEffectFromString("VICTIM_EXPLODE_EFFECT", effect)) {
                                    C3DVector focus = something->GetPos();
                                    const NGraphicAppearance::CTCGraphicAppearance* ptc_graphic_appearance =
                                        something->GetTCGraphicAppearance();
                                    if (ptc_graphic_appearance) {
                                        if (!ptc_graphic_appearance->GetHelperPoint(MAIN_FOCAL_POSITION_NAME,
                                                                                    0, 0, focus, 1.0f))
                                            FABLE_RETAIL_ASSERT_ARM(false);
                                    }
                                    CThing* fx = CTCSpecialAbilities::CreateParticleEffect(effect, focus);
                                }
                            }
                        } else {
                            ResetCreatureHitState(something);
                            CTCSpecialAbilities::FadeOrKillEffect(it->second.Effect);
                        }
                    }
                    MapOfCreaturesToPerCreatureInfo::iterator tmp = it;
                    ++it;
                    LastFrameCreaturesHit.erase(tmp);
                }
            }
        } else if (DurationOfSpellFrames <= 0) {
            float damage = PDef->DamagePerHit[SpellLevel];
            for (MapOfCreaturesToPerCreatureInfo::iterator it = LastFrameCreaturesHit.begin(),
                                                           endit = LastFrameCreaturesHit.end();
                 it != endit; ++it) {
                CIntelligentPointer<CThingCreatureBase> something = it->second.ActualCreature;
                if (IS_THING_ALIVE(something)) {
                    CTCNymph* ptc_nymph;
                    if (something->GetTC(&ptc_nymph))
                        ptc_nymph->StopFlyingAround(0);
                    ResetCreatureHitState(something);
                    C3DVector direction = something->GetPos() - parent_creature.GetPos();
                    direction.Normalise();
                    C3DVector point_of_hit = something->GetPos() - direction;
                    CHitParameters hit_param(0, &parent_creature, 0, point_of_hit, direction, true, false,
                                             false, false);
                    hit_param.AddDamageWithAttribute((EDamageAttribute)10, damage);
                    hit_param.SetCreateParticleEffectOnHit(false);
                    hit_param.SetEpicSpell(true);
                    bool do_exp_override = false;
                    const CThingCreatureDef& creature_def =
                        *GFDebugDynamicCast(&something->PeekBaseDef(), (const CThingCreatureDef*)0);
                    if (!creature_def.LeaveDeadCreature &&
                        CTCWillResponse::IsThingEpicPowerExplodable(something.Get()))
                        do_exp_override = true;
                    bool prev_exp_override_flag = true;
                    if (do_exp_override) {
                        prev_exp_override_flag = something->GetLeaveExperienceOrbsOverride();
                        something->SetLeaveExperienceOrbsOverride(false);
                    }
                    something->OnHit(hit_param);
                    CTCCreatureTintEffect* ptc_tint;
                    if (!something->GetTC(&ptc_tint))
                        ptc_tint = GFDebugPDynamicCast(something->AddTC("CTCCreatureTintEffect", true, 0),
                                                       (CTCCreatureTintEffect*)0);
                    if (do_exp_override)
                        something->SetLeaveExperienceOrbsOverride(prev_exp_override_flag);
                    CTCEntityEvents* ptc_entity_events;
                    if (something->GetTC(&ptc_entity_events))
                        ptc_entity_events->AddEvent(new NEntityEvents::CEventHitBySpecialAbility(
                            hit_param, HERO_ABILITY_UNHOLY_POWER_SPELL));
                    if (something->GetHealth() <= 0.0f &&
                        CTCWillResponse::IsThingEpicPowerExplodable(something.Get())) {

                        GetWorld().GetMessageEventManager().AddMessage(
                            (EMessageEventType)9, CMainGameComponent::GetConstantFPS(),
                            something->GetPhysicsTC()->GetPosition(), &GetParentThing(), something.Get(), 0,
                            0, 0);
                        CTCEntityEvents* ptc_attacker_entity_events;
                        if (GetParentThing().GetTC(&ptc_attacker_entity_events))
                            ptc_attacker_entity_events->AddEvent(
                                new NEntityEvents::CEventKilledCreature(GetParentThing(), *something.Get()));
                        CTCDExperienceOrb::Create(*something.Get());
                        CTCContainerRewardHero* ptc_reward;
                        if (something->GetTC(TCI_CONTAINER, &ptc_reward))
                            ptc_reward->OnDie();
                        something->Kill(false);
                        CDefPointer<const CMessageEventDef> pdef;
                        CThingCreatureBase* my_attacker = something->GetPLastAttackedByCreature();
                        if (my_attacker && my_attacker->IsPlayerCharacter()) {
                            if (GetDefinitionManager().GetDef("STANDARD_MESSAGE_EVENT_MURDER", &pdef))
                                GetWorld().GetMessageEventManager().AddMessage(
                                    *pdef, something->PeekPos(), my_attacker, something.Get(), 0);
                            else
                                FABLE_RETAIL_ASSERT_ARM(false);
                        }
                        CTCWife* tc_wife = 0;
                        something->GetTC(&tc_wife);
                        CTCHeroOpinionDeedLog* ptc_deed_log;
                        if (my_attacker && my_attacker->GetTC(&ptc_deed_log)) {
                            const CTCEnemy* pmy_enemy_tc;
                            if (something->PeekTC(TCI_ENEMY, &pmy_enemy_tc)) {
                                if (pmy_enemy_tc->GetPFaction()) {
                                    ptc_deed_log->PostDeed((EOpinionDeedType)9, something->PeekPos(),
                                                           something.Get(), 0, (EOpinionPostDeedType)2, true,
                                                           false, pmy_enemy_tc->GetPFaction(), false, 0);
                                    ptc_deed_log->PostDeed((EOpinionDeedType)0x3A, something->PeekPos(),
                                                           something.Get(), 0, (EOpinionPostDeedType)7, false,
                                                           false, pmy_enemy_tc->GetPFaction(), true, 0);
                                }
                            }
                            if (tc_wife)
                                tc_wife->Murdered();
                        }
                        CTCEntityEvents* ptc_entity_events;
                        if (something->GetTC(&ptc_entity_events))
                            ptc_entity_events->AddEvent(
                                new NEntityEvents::CEventDied(my_attacker, something.Get()));
                        if (pspecial_effects.Get()) {
                            long effect = 0;
                            if (pspecial_effects->GetEffectFromString("VICTIM_EXPLODE_EFFECT", effect)) {
                                C3DVector focus = something->GetPos();
                                const NGraphicAppearance::CTCGraphicAppearance* ptc_graphic_appearance =
                                    something->GetTCGraphicAppearance();
                                if (ptc_graphic_appearance) {
                                    if (!ptc_graphic_appearance->GetHelperPoint(MAIN_FOCAL_POSITION_NAME, 0,
                                                                                0, focus, 1.0f))
                                        FABLE_RETAIL_ASSERT_ARM(false);
                                }
                                CThing* fx = CTCSpecialAbilities::CreateParticleEffect(effect, focus);
                            }
                        }
                    }
                }
            }
            parent_creature.FinishCurrentAction();
            UnsheatheWeapon();
            FinishMove();
            PlaySoundW(PDef->FinishSound[SpellLevel], false);
        }
    }
}
// PRODUCTION_BODY_END
static void ResetGlobals(){
    for(unsigned int i=0;i<owned_effects.size();++i)delete owned_effects[i];
    owned_effects.clear();effect_records.clear();search_creatures.clear();search_objects.clear();
    fades=rumble_strong=rumble_weak=0;world=CWorld();definitions=CGameDefinitionManager();
}
struct Fixture {
    CThingCreatureBase owner,victim,killer;CThingObject object;
    CTCHeroStats stats;CTCHeroExperience experience;CTCVolumeContainmentTracker volume;
    CTCEntityEvents victim_events,owner_events,object_events;CTCCreatureModeManager modes;
    CTCNymph nymph;CTCWife wife;CTCHeroOpinionDeedLog deeds;CTCEnemy enemy;CFaction faction;CTCContainerRewardHero reward;
    CSpecialEffectsDef effects,custom;SpellDef def;CTCUnholyPower spell;
    Fixture():spell(owner,def){
        ResetGlobals();def.effects=&effects;victim.definition.effects=&custom;victim.pos=C3DVector(1,0,0);object.pos=C3DVector(2,0,0);object.health=1000;
        owner.components[TCI_HERO_STATS]=&stats;owner.components[TCI_HERO_EXPERIENCE]=&experience;owner.components[TCI_ENTITY_EVENTS]=&owner_events;
        victim.components[TCI_VOLUME]=&volume;victim.components[TCI_ENTITY_EVENTS]=&victim_events;victim.components[TCI_MODE]=&modes;
        victim.components[TCI_NYMPH]=&nymph;victim.components[TCI_WIFE]=&wife;victim.components[TCI_ENEMY]=&enemy;victim.components[TCI_CONTAINER]=&reward;
        object.components[TCI_ENTITY_EVENTS]=&object_events;killer.components[TCI_DEEDS]=&deeds;killer.player=true;enemy.faction=&faction;victim.attacker=&killer;
    }
    void release(){spell.charging=false;}
    void mark(long frames=99){CTCUnholyPower::PerCreatureInfo& p=spell.LastFrameCreaturesHit[&victim];p.ActualCreature=&victim;p.FramesToGetHit=frames;p.DefaultChannel=7;}
    void die_on_tick(){release();mark(1);victim.health=1;victim.explodable=true;}
};
static void FullStateChecks(){
    {Fixture f;f.spell.initialized=false;f.spell.FrameUpdate();assert(f.spell.Timer.updates==0&&f.owner.action_attempts==0);}
    {Fixture f;f.spell.Finished=true;f.spell.NumFramesToWaitForFadeOut=1;f.spell.FrameUpdate();assert(f.spell.NumFramesToWaitForFadeOut==0&&!f.owner.removed);f.spell.FrameUpdate();assert(f.owner.removed==1&&f.owner.last_removed==TCI_UNHOLY_POWER);}
    {Fixture f;f.spell.CurFramesForEnvChange=2;f.spell.FrameUpdate();assert(f.spell.targets==1&&f.owner.build_attempts==1);f.spell.FrameUpdate();assert(f.spell.targets==2);}
    {Fixture f;f.spell.Timer.state=CWillButtonHelperTimer::RELEASED;f.spell.FrameUpdate();assert(f.spell.sounds==1&&f.spell.finishes==1&&f.spell.unsheathes==1&&f.owner.finished_actions==1&&f.stats.changes==0);}
    {Fixture f;f.spell.StaminaModify=1;f.spell.PerFrameStaminaModify=2.5f;f.spell.FrameUpdate();assert(f.stats.stamina==7&&f.spell.AccModifyStamina==0.5f&&!f.spell.finishes);}
    {Fixture f;f.spell.PerFrameStaminaModify=1;f.spell.FrameUpdate();assert(f.stats.stamina==10&&f.spell.AccModifyStamina==1);}
    {Fixture f;f.stats.stamina=1;f.spell.StaminaModify=1;f.spell.FrameUpdate();assert(f.stats.stamina==0&&f.spell.finishes==1&&f.spell.sounds==1);}
    for(int phase=0;phase<2;++phase){for(int k=0;k<8;++k){Fixture f;if(phase)f.release();search_creatures.push_back(k==0?&f.owner:&f.victim);if(k==1)f.victim.immune=true;if(k==2)f.victim.spellable=false;if(k==3)f.victim.attackable=false;if(k==4)f.victim.underground=true;if(k==5)f.victim.dead=true;if(k==6)f.victim.health=0;if(k==7)f.victim.health=-1;f.spell.FrameUpdate();assert(f.spell.LastFrameCreaturesHit.empty()&&f.victim.hits==0);}}
    {Fixture f;search_creatures.push_back(&f.victim);f.spell.FrameUpdate();assert(f.spell.LastFrameCreaturesHit.size()==1&&f.spell.LastFrameCreaturesHit[&f.victim].FramesToGetHit==0&&f.victim.graphics.channel==99&&!f.volume.update);int n=f.victim.graphics.channel_sets;f.spell.FrameUpdate();assert(f.victim.graphics.channel_sets==n);}
    {Fixture f;f.victim.graphics_present=false;f.victim.components[TCI_VOLUME]=0;search_creatures.push_back(&f.victim);f.spell.FrameUpdate();assert(f.spell.LastFrameCreaturesHit.size()==1);}
}
static void FullReleaseChecks(){
    for(int k=0;k<4;++k){Fixture f;f.release();f.spell.CurFramesForEnvChange=3;if(k&1)f.effects.entries["RELEASE_EFFECT"]=11;if(k&2)f.effects.entries["RELEASE_COLUMN_EFFECT"]=12;f.spell.FrameUpdate();assert(effect_records.size()==(unsigned int)(((k&1)?1:0)+((k&2)?1:0)));assert(rumble_strong==1&&rumble_weak==1&&f.spell.sounds==2&&f.spell.stops==1&&f.spell.last_stopped==17&&f.spell.LoopingSound==102);f.spell.FrameUpdate();assert(rumble_strong==1&&f.spell.stops==1&&f.owner.loop_attempts==1);}
    {Fixture f;f.release();f.def.effects=0;f.spell.CurFramesForEnvChange=3;f.spell.FrameUpdate();assert(effect_records.empty()&&f.spell.sounds==2);}
    {Fixture f;f.release();f.owner.accept_release=false;search_objects.push_back(&f.object);f.spell.FrameUpdate();f.spell.FrameUpdate();assert(f.owner.release_attempts==2&&f.object.hits==0&&f.experience.calls==0);}
    {Fixture f;f.release();f.owner.components[TCI_HERO_EXPERIENCE]=0;f.spell.FrameUpdate();assert(f.experience.calls==0&&f.spell.PerformedReleaseAction);}
    {Fixture f;f.release();f.spell.FrameUpdate();assert(f.experience.calls==1&&f.experience.amount==4);f.spell.PItemRemoved=true;f.spell.FrameUpdate();assert(f.owner.loop_attempts==1&&f.owner.last_item);}
    for(int k=0;k<4;++k){Fixture f;f.release();search_objects.push_back(&f.object);if(k==1)f.object.dead=true;if(k==2)f.object.immune=true;if(k==3)f.object.spellable=false;f.spell.FrameUpdate();assert(f.object.hits==(k==0?1:0));if(k==0){assert(f.object.last_damage==100&&f.object.last_attribute==10&&f.object.hit_knock&&f.object.hit_epic&&!f.object.hit_particle&&f.object_events.hits==1&&f.object_events.last_ability==HERO_ABILITY_UNHOLY_POWER_SPELL);}}
    for(int k=0;k<4;++k){Fixture f;f.release();f.victim.health=1000;f.victim.explodable=true;f.victim.leave_orbs=(k&1)!=0;f.victim.definition.LeaveDeadCreature=(k&2)!=0;bool old=f.victim.leave_orbs;search_objects.push_back(&f.victim);f.spell.FrameUpdate();assert(f.victim.hits==1&&f.victim.leave_orbs==old);assert(f.victim.saw_orbs==(f.victim.definition.LeaveDeadCreature?old:false));}
}
static void FullTrackingChecks(){
    {Fixture f;f.release();f.effects.entries["VICTIM_EFFECT"]=21;f.victim.ground=7;search_creatures.push_back(&f.victim);f.spell.FrameUpdate();assert(f.spell.LastFrameCreaturesHit.size()==1&&f.spell.HitEffects.size()==1&&effect_records.size()==1&&effect_records[0].pos.Z==7&&f.victim.graphics.helper_calls==1&&f.victim.hits==1);}
    {Fixture f;f.release();f.effects.entries["VICTIM_EFFECT"]=21;f.victim.graphics.helper_ok=false;search_creatures.push_back(&f.victim);f.spell.FrameUpdate();assert(f.victim.graphics.helper_calls==1&&effect_records.size()==1);}
    {Fixture f;f.release();f.effects.entries["VICTIM_EFFECT"]=21;f.victim.graphics_present=false;search_creatures.push_back(&f.victim);f.spell.FrameUpdate();assert(effect_records.size()==1&&f.victim.graphics.helper_calls==0);}
    for(int live=0;live<2;++live){Fixture f;f.release();f.mark();CThing old;old.dead=live==0;f.spell.LastFrameCreaturesHit[&f.victim].Effect=&old;f.effects.entries["VICTIM_EFFECT"]=21;search_creatures.push_back(&f.victim);f.spell.FrameUpdate();assert(effect_records.size()==(unsigned int)(live?0:1));}
    {Fixture f;f.release();f.mark();search_creatures.push_back(&f.victim);f.spell.FrameUpdate();assert(effect_records.empty());}
    for(int reason=0;reason<3;++reason){Fixture f;f.release();f.mark();CThing fx;f.spell.LastFrameCreaturesHit[&f.victim].Effect=&fx;f.victim.influenced=true;f.victim.graphics.channel=99;if(reason==0)f.victim.pos=C3DVector(20,0,0);if(reason==1)f.victim.immune=true;if(reason==2)f.victim.spellable=false;f.spell.FrameUpdate();assert(f.spell.LastFrameCreaturesHit.empty()&&f.victim.resets==1&&fades==1&&fx.dead&&f.volume.update&&!f.victim.influenced&&f.victim.graphics.channel==7&&f.victim.hits==0);}
    {Fixture f;f.release();f.mark();f.def.RadiusOfEffect[0]=0;f.victim.physics.radius=0;f.victim.pos=C3DVector(1,2,0);f.spell.FrameUpdate();assert(f.spell.LastFrameCreaturesHit.size()==1&&f.victim.resets==0);}
    {Fixture f;f.release();f.mark();f.victim.pos=C3DVector(20,0,0);f.victim.graphics_present=false;f.victim.components[TCI_VOLUME]=0;f.spell.FrameUpdate();assert(f.spell.LastFrameCreaturesHit.empty()&&f.victim.resets==1);}
    for(int effect=0;effect<3;++effect){Fixture f;f.release();CTCUnholyPower::PerEffectInfo p;CThing old;old.dead=effect!=0;p.Position=C3DVector(8,9,10);p.Effect=&old;f.spell.HitEffects.push_back(p);if(effect!=2)f.effects.entries["VICTIM_EFFECT"]=27;f.spell.FrameUpdate();assert(effect_records.size()==(unsigned int)(effect==1?1:0));if(effect==1)assert(effect_records[0].effect==27&&effect_records[0].pos.Z==10);}
    {Fixture f;f.release();f.mark();f.spell.LastFrameCreaturesHit[&f.victim].ActualCreature=0;f.spell.FrameUpdate();assert(f.spell.LastFrameCreaturesHit.empty()&&f.victim.hits==0&&f.victim.resets==0&&fades==0);}
}
static void FullTickChecks(){
    {Fixture f;f.release();f.mark(2);f.spell.FrameUpdate();assert(f.victim.hits==0&&f.nymph.stops==1);f.spell.FrameUpdate();assert(f.victim.hits==1&&f.victim.last_damage==1&&f.victim.last_attribute==10&&!f.victim.hit_knock&&!f.victim.hit_move&&f.victim.hit_epic&&!f.victim.hit_particle&&f.victim.influenced&&f.spell.LastFrameCreaturesHit[&f.victim].FramesToGetHit==3&&f.modes.adds==1&&f.victim.finished_actions==1&&f.victim.add_tint==1&&f.victim_events.hits==1);}
    {Fixture f;f.release();f.mark(1);f.modes.active=true;CTCCreatureTintEffect tint;f.victim.components[TCI_CREATURE_TINT_EFFECT]=&tint;f.spell.FrameUpdate();assert(f.modes.adds==0&&f.victim.finished_actions==0&&f.victim.add_tint==0);}
    {Fixture f;f.release();f.mark(1);f.victim.components[TCI_MODE]=0;f.victim.components[TCI_NYMPH]=0;f.victim.components[TCI_ENTITY_EVENTS]=0;f.spell.FrameUpdate();assert(f.victim.hits==1&&f.nymph.stops==0&&f.victim_events.hits==0);}
    for(int k=0;k<4;++k){Fixture f;f.release();f.mark(1);f.victim.explodable=true;f.victim.leave_orbs=(k&1)!=0;f.victim.definition.LeaveDeadCreature=(k&2)!=0;bool prior=f.victim.leave_orbs;f.spell.FrameUpdate();assert(f.victim.leave_orbs==prior&&f.victim.saw_orbs==(f.victim.definition.LeaveDeadCreature?prior:false));}
    {Fixture f;f.release();f.mark(1);f.spell.DurationOfSpellFrames=1;f.spell.FrameUpdate();assert(f.spell.DurationOfSpellFrames==0&&f.victim.hits==1&&!f.spell.Finished);f.spell.FrameUpdate();assert(f.victim.hits==2&&f.spell.Finished&&f.spell.finishes==1);}
}
static void FullDeathChecks(){
    {Fixture f;f.die_on_tick();f.spell.FrameUpdate();assert(f.spell.LastFrameCreaturesHit.empty()&&f.victim.kills==1&&f.victim.orbs==1&&f.reward.deaths==1&&f.owner_events.killed==1&&f.victim_events.died==1&&world.messages.epic==1&&world.messages.murder==1&&f.deeds.posts==2&&f.wife.murdered==1);}
    {Fixture f;f.die_on_tick();f.victim.components[TCI_CONTAINER]=0;f.owner.components[TCI_ENTITY_EVENTS]=0;f.victim.components[TCI_ENTITY_EVENTS]=0;f.spell.FrameUpdate();assert(f.victim.kills==1&&f.reward.deaths==0&&f.owner_events.killed==0&&f.victim_events.died==0);}
    for(int k=0;k<8;++k){Fixture f;f.die_on_tick();if(!(k&1))f.victim.attacker=0;if(!(k&2))f.killer.components[TCI_DEEDS]=0;if(!(k&4))f.victim.components[TCI_WIFE]=0;f.spell.FrameUpdate();assert(f.wife.murdered==(k==7?1:0));assert(f.deeds.posts==((k&3)==3?2:0));}
    for(int k=0;k<4;++k){Fixture f;f.die_on_tick();if(k&1)f.victim.components[TCI_ENEMY]=0;if(k&2)f.enemy.faction=0;f.spell.FrameUpdate();assert(f.deeds.posts==(k==0?2:0));assert(f.wife.murdered==1);}
    {Fixture f;f.die_on_tick();f.killer.player=false;f.spell.FrameUpdate();assert(world.messages.murder==0&&f.wife.murdered==1&&f.deeds.posts==2);}
    {Fixture f;f.die_on_tick();definitions.murder_found=false;f.spell.FrameUpdate();assert(world.messages.murder==0&&f.victim.kills==1);}
    for(int k=0;k<4;++k){Fixture f;f.die_on_tick();if(k==0)f.custom.entries["VICTIM_EXPLODE_EFFECT"]=31;if(k==1)f.victim.definition.effects=0;if(k==3)f.def.effects=0;if(k!=3)f.effects.entries["VICTIM_EXPLODE_EFFECT"]=32;f.spell.FrameUpdate();assert(effect_records.size()==(unsigned int)(k==3?0:1));if(k!=3)assert(effect_records[0].effect==(k==0?31:32));}
    {Fixture f;f.die_on_tick();f.effects.entries["VICTIM_EXPLODE_EFFECT"]=32;f.victim.graphics.helper_ok=false;f.spell.FrameUpdate();assert(effect_records.size()==1&&f.victim.graphics.helper_calls==1);}
    {Fixture f;f.die_on_tick();f.effects.entries["VICTIM_EXPLODE_EFFECT"]=32;f.victim.graphics_present=false;f.spell.FrameUpdate();assert(effect_records.size()==1&&f.victim.graphics.helper_calls==0);}
    {Fixture f;f.release();f.mark(1);f.victim.health=1;CThing fx;f.spell.LastFrameCreaturesHit[&f.victim].Effect=&fx;f.spell.FrameUpdate();assert(f.spell.LastFrameCreaturesHit.empty()&&!f.victim.kills&&f.victim.resets==1&&fades==1&&fx.dead&&!f.victim.orbs&&!f.victim_events.died);}
    {Fixture f;f.release();f.mark();f.victim.dead=true;f.spell.FrameUpdate();assert(f.spell.LastFrameCreaturesHit.empty()&&f.victim.hits==0&&f.victim.resets==1);}
}
static void FullFinalChecks(){
    for(int outcome=0;outcome<3;++outcome){Fixture f;f.release();f.mark();f.spell.DurationOfSpellFrames=0;f.victim.health=outcome?1:10;f.victim.explodable=outcome==2;f.effects.entries["VICTIM_EXPLODE_EFFECT"]=45;f.spell.FrameUpdate();assert(f.victim.hits==1&&f.victim.resets==1&&f.victim.hit_knock&&f.victim.hit_move&&f.victim.last_attribute==10&&f.spell.finishes==1&&f.spell.unsheathes==1&&f.owner.finished_actions==1&&f.spell.sounds==1);assert(f.victim.kills==(outcome==2?1:0));assert(effect_records.size()==(unsigned int)(outcome==2?1:0));}
    for(int dead=0;dead<2;++dead){Fixture f;f.release();f.mark();f.spell.DurationOfSpellFrames=-1;if(dead)f.victim.dead=true;else f.spell.LastFrameCreaturesHit[&f.victim].ActualCreature=0;f.spell.FrameUpdate();assert(f.victim.hits==0&&f.victim.resets==0&&f.spell.finishes==1);}
    {Fixture f;f.release();f.mark();f.spell.DurationOfSpellFrames=0;f.victim.health=1;f.victim.explodable=true;f.custom.entries["VICTIM_EXPLODE_EFFECT"]=77;f.spell.FrameUpdate();assert(f.victim.kills==1&&effect_records.empty());}
    {Fixture f;f.release();f.mark();f.spell.DurationOfSpellFrames=0;f.victim.health=1;f.victim.explodable=true;f.def.effects=0;f.victim.attacker=0;f.victim.components[TCI_CONTAINER]=0;f.victim.components[TCI_ENTITY_EVENTS]=0;f.owner.components[TCI_ENTITY_EVENTS]=0;f.spell.FrameUpdate();assert(f.victim.kills==1&&f.wife.murdered==0&&f.deeds.posts==0&&effect_records.empty());}
}
static void FullAdditionalChecks(){
    for(int k=0;k<4;++k){Fixture f;f.release();f.mark();f.spell.DurationOfSpellFrames=0;f.victim.explodable=true;f.victim.leave_orbs=(k&1)!=0;f.victim.definition.LeaveDeadCreature=(k&2)!=0;bool prior=f.victim.leave_orbs;f.spell.FrameUpdate();assert(f.victim.leave_orbs==prior&&f.victim.saw_orbs==(f.victim.definition.LeaveDeadCreature?prior:false)&&f.victim.kills==0);}
    for(int k=0;k<8;++k){Fixture f;f.release();f.mark();f.spell.DurationOfSpellFrames=0;f.victim.health=1;f.victim.explodable=true;if(!(k&1))f.victim.attacker=0;if(!(k&2))f.killer.components[TCI_DEEDS]=0;if(!(k&4))f.victim.components[TCI_WIFE]=0;f.spell.FrameUpdate();assert(f.wife.murdered==(k==7?1:0)&&f.deeds.posts==((k&3)==3?2:0));}
    for(int k=0;k<4;++k){Fixture f;f.release();f.mark();f.spell.DurationOfSpellFrames=0;f.victim.health=1;f.victim.explodable=true;if(k&1)f.victim.components[TCI_ENEMY]=0;if(k&2)f.enemy.faction=0;f.spell.FrameUpdate();assert(f.deeds.posts==(k==0?2:0)&&f.wife.murdered==1);}
    {Fixture f;f.release();f.mark();f.spell.DurationOfSpellFrames=0;f.victim.health=1;f.victim.explodable=true;definitions.murder_found=false;f.spell.FrameUpdate();assert(world.messages.murder==0&&f.victim.kills==1&&f.wife.murdered==1);}
    {Fixture f;f.release();f.mark();f.spell.DurationOfSpellFrames=0;f.victim.health=1;f.victim.explodable=true;f.killer.player=false;f.spell.FrameUpdate();assert(world.messages.murder==0&&f.deeds.posts==2);}
    for(int k=0;k<3;++k){Fixture f;f.release();f.mark();f.spell.DurationOfSpellFrames=0;f.victim.health=1;f.victim.explodable=true;f.effects.entries["VICTIM_EXPLODE_EFFECT"]=88;if(k==1)f.victim.graphics.helper_ok=false;if(k==2)f.victim.graphics_present=false;f.spell.FrameUpdate();assert(effect_records.size()==1&&effect_records[0].effect==88&&f.victim.graphics.helper_calls==(k==2?0:1));}
    {Fixture f;f.release();f.mark();f.spell.DurationOfSpellFrames=0;f.victim.components[TCI_NYMPH]=0;CTCCreatureTintEffect tint;f.victim.components[TCI_CREATURE_TINT_EFFECT]=&tint;f.spell.FrameUpdate();assert(f.nymph.stops==0&&f.victim.add_tint==0&&f.victim.hits==1);}
    {Fixture f;f.release();f.object.components[TCI_ENTITY_EVENTS]=0;search_objects.push_back(&f.object);f.spell.FrameUpdate();assert(f.object.hits==1&&f.object_events.hits==0);}
    {Fixture f;f.release();f.mark();f.victim.dead=true;search_creatures.push_back(&f.victim);f.spell.FrameUpdate();assert(f.spell.LastFrameCreaturesHit.empty()&&f.victim.hits==0&&f.victim.resets==1);}
    {Fixture f;f.release();f.def.effects=0;search_creatures.push_back(&f.victim);f.spell.FrameUpdate();assert(f.spell.LastFrameCreaturesHit.size()==1&&f.victim.hits==1&&effect_records.empty());}
    {Fixture f;unsigned int bits=0x7fc00000u;memcpy(&f.victim.health,&bits,sizeof(f.victim.health));search_creatures.push_back(&f.victim);f.spell.FrameUpdate();assert(f.spell.LastFrameCreaturesHit.empty());}
}

int main(){FullStateChecks();FullReleaseChecks();FullTrackingChecks();FullTickChecks();FullDeathChecks();FullFinalChecks();FullAdditionalChecks();ResetGlobals();puts("00795300_TEST PASS");return 0;}
