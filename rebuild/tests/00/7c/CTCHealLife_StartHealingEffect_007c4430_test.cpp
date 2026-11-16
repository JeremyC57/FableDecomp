// Self-contained full-body test: real shared class models and controlled external operations.
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <new>
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __stdcall
#define __int64 long long
#define __declspec(x)
#define FABLE_PORTABLE
#include <strings.h>
#define _stricmp strcasecmp
enum ESex{};
#endif
#include "engine/EPlayerMode.h"
#define FABLE_RETAIL_PASS23_HOST_TEST_ABI
#define FABLE_RETAIL_STLP_HOST_TAGS_PASS23
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_STLP_TREE_CONSTFIND
#define FABLE_RETAIL_DEF_CLASS_BASE_CTOR_DECL
#define FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_CTOR
#define FABLE_RETAIL_CDEFPOINTER_POINTER_CTOR_ADOPT
#define FABLE_RETAIL_CDEFPOINTER_RESET
#define FABLE_RETAIL_CTCHEALLIFE_CTOR_PASS23
#define FABLE_RETAIL_CTCSOUNDPLAYER_CTOR_PASS23
#define FABLE_RETAIL_CTCENTITYEVENTS_CTOR_PASS23
#define FABLE_RETAIL_CTCTARGETINGBASE_CTOR_PASS23
#define FABLE_RETAIL_CTCDPARTICLEEMITTER_FULL
#define FABLE_RETAIL_CTCBOASTINGCROWDCONTROL
#define FABLE_RETAIL_CCLASSFACTORY
#define FABLE_RETAIL_CGAME_CORE_PASS23
#define FABLE_RETAIL_CINITBASECLASS
#define FABLE_RETAIL_CGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define private public
#define protected public
// 007C4430 CTCHealLife::StartHealingEffect() -- begin targeted or area healing.
// Leaf: ?StartHealingEffect@CTCHealLife@@QAEXXZ
// Flags: /Gr
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_STATEACCESS
#define FABLE_RETAIL_CTHING_GETTC_ORIGINAL_CHAIN
#define FABLE_RETAIL_CTHING_PEEKTC_ORIGINAL_CHAIN
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY_ORIGINAL_CHAIN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_CONVERT_MUTABLE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_GET_ORIGINAL_CHAIN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_DEREFERENCE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_EQUAL_RAW
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTCSOUNDPLAYER
#define FABLE_RETAIL_CHITPARAMETERS_CTOR
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CTCENTITYEVENTS
#define FABLE_RETAIL_CTCENTITYEVENTS_ADDEVENT
#define FABLE_RETAIL_CTCENEMY_QUERIES
#define FABLE_RETAIL_CTCSPECIALABILITIES_FULL
#define FABLE_RETAIL_CTCHEALLIFE_START_ORIGINAL
#define FABLE_RETAIL_CSPECIALEFFECTSDEF_TYPENAME
#define FABLE_RETAIL_CSPECIALEFFECTSDEF_GETEFFECT_BODY
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE
#define FABLE_RETAIL_IS_THING_ALIVE
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/CThing.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CTCSoundPlayer.h"
#include "engine/CTCTargetingBase.h"
#include "engine/CPlayer.h"
#include "engine/CDefClassBase.h"
#include "engine/CParentDefClassBase.h"
#include "engine/CSpecialEffectsDef.h"
#include "engine/CDefClassBase_GetSubDef.h"
#include "engine/CSpecialAbilitiesHealLifeDef.h"
#include "engine/CHitParameters.h"
#include "engine/EHeroAbility.h"
#include "engine/C2DCoordI.h"
#include "engine/C2DBoxI.h"
#include "engine/CFilter_Bind2.h"
#include "engine/CThingFilter_IsNot.h"
#include "engine/CThingFilter_In2DRange.h"
#include "engine/CThingSearchTools.h"
#include "engine/CTCEnemy.h"
#include "engine/CTCSpecialAbilities.h"
#include "engine/CTCDParticleEmitter.h"
#include "engine/NEntityEvents_CEventHitBy.h"
#include "engine/CTCEntityEvents.h"
#include "engine/ThingAlive.h"
#include "engine/CTCHealLife.h"

void CTCHealLife::StartHealingEffect()
{
    CThingCreatureBase& parent_creature = *GFDebugDynamicCast(&GetParentThing(), (CThingCreatureBase*)0);
    CTCSoundPlayer* ptc_sound_player = 0;
    if (parent_creature.GetTC(&ptc_sound_player)) {
        switch (SpellLevel) {
        case 0: LoopingSound = ptc_sound_player->PlayAttachedCriteria("HERO_ABILITY_HEAL_LIFE_CONTINUOUS_LEVEL_1", false, false, false, -1); break;
        case 1: LoopingSound = ptc_sound_player->PlayAttachedCriteria("HERO_ABILITY_HEAL_LIFE_CONTINUOUS_LEVEL_2", false, false, false, -1); break;
        case 2: LoopingSound = ptc_sound_player->PlayAttachedCriteria("HERO_ABILITY_HEAL_LIFE_CONTINUOUS_LEVEL_3", false, false, false, -1); break;
        case 3: LoopingSound = ptc_sound_player->PlayAttachedCriteria("HERO_ABILITY_HEAL_LIFE_CONTINUOUS_LEVEL_4", false, false, false, -1); break;
        }
    }
    long effect = 0;
    CDefPointer<const CSpecialEffectsDef> pspecial_effects;
    CCharString effectname = GetLevelEffectName();
    float search_radius = PDef->RadiusOfEffect[SpellLevel];
    PHealingTarget = &parent_creature;
    const CTCTargetingBase* ptc_targeting = 0;
    if (parent_creature.PeekTC(&ptc_targeting)) {
        CThing* ptarget = ptc_targeting->GetPTargetedThing();
        if (IS_THING_ALIVE(ptarget) && parent_creature.GetPlayer().IsZTargeting())
            PHealingTarget = ptarget;
    }
    if (CTCEnemy::AreThingsEnemies(*PHealingTarget, parent_creature))
        PHealingTarget = &parent_creature;
    CThingCreatureBase* phealingtarget_creature = 0;
    if (PHealingTarget->GetTypeFlags() & 8)
        phealingtarget_creature = GFDebugPDynamicCast(PHealingTarget.Get(), (CThingCreatureBase*)0);
    if (IS_THING_ALIVE(phealingtarget_creature)) {
        if (IS_THING_ALIVE(phealingtarget_creature)) {
            ThingsToHeal.push_back(phealingtarget_creature);
            HealingEffects.push_back(CIntelligentPointer<CThing>(0));
            HealthIncreaseAccum.push_back(0.0f);
            if (PDef->GetSubDef(&pspecial_effects) && pspecial_effects->GetEffectFromString(effectname, effect)) {
                CIntelligentPointer<CThing> fx(CTCSpecialAbilities::CreateParticleEffect(effect, phealingtarget_creature->GetPos()));
                if (IS_THING_ALIVE(fx)) {
                    CTCDParticleEmitter* ptc_emitter;
                    fx->GetTC(&ptc_emitter);
                    ptc_emitter->AttachToThing(*phealingtarget_creature, 0x28, "", 0, 0.0f);
                    HealingEffects.back() = fx;
                }
            }
            CTCEntityEvents* ptc_entity_events = 0;
            if (phealingtarget_creature->GetTC(&ptc_entity_events)) {
                CHitParameters hit_params;
                hit_params.SetThingHitting(parent_creature);
                ptc_entity_events->AddEvent(new NEntityEvents::CEventHitBySpecialAbility(hit_params, HERO_ABILITY_HEAL_LIFE_SPELL));
            }
            if (phealingtarget_creature == &parent_creature) {
                C3DVector targetpos = phealingtarget_creature->GetPos();
                C2DBoxI search_area(targetpos.ToC2DCoordI(), (long)search_radius);
                PeekThingSearchTools().GetAllThingsInRoughArea<CThingCreatureBase>(search_area,
                    GFFilter_Bind2(CThingFilter_IsNot(*phealingtarget_creature), CThingFilter_In2DRange(targetpos, search_radius)), ThingsToHeal);
                // Original donor02713298 has a constant-false arm before the iterator scope.
                if (false) {} else for (std::list<CIntelligentPointer<CThingCreatureBase> >::iterator it = ThingsToHeal.begin(), endit = ThingsToHeal.end(); it != endit; ++it) {
                    CIntelligentPointer<CThingCreatureBase> something = *it;
                    if (something == phealingtarget_creature)
                        continue;
                    HealingEffects.push_back(CIntelligentPointer<CThing>(0));
                    HealthIncreaseAccum.push_back(0.0f);
                    if (IS_THING_ALIVE(something) && !CTCEnemy::AreThingsEnemies(*phealingtarget_creature, *something)) {
                        if (PDef->GetSubDef(&pspecial_effects) && pspecial_effects->GetEffectFromString(effectname, effect)) {
                            CIntelligentPointer<CThing> fx(CTCSpecialAbilities::CreateParticleEffect(effect, something->GetPos()));
                            if (IS_THING_ALIVE(fx)) {
                                CTCDParticleEmitter* ptc_emitter = 0;
                                fx->GetTC(&ptc_emitter);
                                ptc_emitter->AttachToThing(*something, 0x28, "", 0, 0.0f);
                                HealingEffects.back() = fx;
                            }
                        }
                        CTCEntityEvents* ptc_loop_entity_events = 0;
                        if (something->GetTC(&ptc_loop_entity_events)) {
                            CHitParameters hit_params;
                            hit_params.SetThingHitting(parent_creature);
                            ptc_loop_entity_events->AddEvent(new NEntityEvents::CEventHitBySpecialAbility(hit_params, HERO_ABILITY_HEAL_LIFE_SPELL));
                        }
                    }
                }
            }
        }
    }
}

#include "engine/CWorld.h"
#include "engine/CWorldMap.h"
#include "engine/CGame.h"
#include "engine/CMainGameComponent.h"
#include "engine/CMainGameComponentInit.h"
#include "engine/CThingManager.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CPlayerManager.h"
#include "engine/CNavigatorManager.h"
#include "engine/CDefString.h"

static int SoundCalls,SoundLevel,CreateCalls,AttachCalls,EventCalls,SearchCalls,ExpectedEvents,SoundAvailable,TargetingAvailable,ZTargeting,ParticleMode,EventMask;
static int AttachedIDs[16],EventIDs[16],CreateIDs[16],CurrentRecipient;

struct ObjectState {const CBaseObject*Object;int ID,References;bool Enemy;C3DVector Pos;};
static ObjectState States[128];static int StateCount;
static ObjectState& StateOf(const CBaseObject*p){for(int i=0;i<StateCount;++i)if(States[i].Object==p)return States[i];assert(StateCount<128);ObjectState&s=States[StateCount++];s.Object=p;s.ID=-1;s.References=0;s.Enemy=false;s.Pos=C3DVector(0,0,0);return s;}
static int ID(const CBaseObject*p){return StateOf(p).ID;}static int Refs(const CBaseObject*p){return StateOf(p).References;}static bool Enemy(const CBaseObject*p){return StateOf(p).Enemy;}
static const char*EffectName="HEAL_FIXTURE_EFFECT";
CBaseClass::CBaseClass(){} CBaseClass::~CBaseClass(){}
CBaseClassNonCopyable::CBaseClassNonCopyable(){} CBaseClassNonCopyable::~CBaseClassNonCopyable(){}
CBaseObject::CBaseObject():IntelligentPointer(0){} CBaseObject::~CBaseObject(){for(int i=0;i<StateCount;++i)if(States[i].Object==this){assert(States[i].References==0);States[i]=States[--StateCount];return;}}
CInitBaseClass::CInitBaseClass():Valid(false){}CInitBaseClass::~CInitBaseClass(){}
CCharString::CCharString():PStringData(0){}
CCharString::CCharString(const char*s,long n){assert(n==-1);PStringData=new CCharStringData;PStringData->Data.PData=const_cast<char*>(s);PStringData->Data.StringLength=(unsigned long)strlen(s);PStringData->NoRefs=1;}
CCharString::CCharString(const CCharString&s):PStringData(s.PStringData){if(PStringData)++PStringData->NoRefs;}
CCharString::~CCharString(){if(PStringData&&--PStringData->NoRefs==0)delete PStringData;}
const CCharString&CCharString::operator=(const CCharString&s){if(PStringData!=s.PStringData){if(PStringData&&--PStringData->NoRefs==0)delete PStringData;PStringData=s.PStringData;if(PStringData)++PStringData->NoRefs;}return *this;}
const char CCharString::NullChar=0;

CWideString::CWideString():PStringData(0){}CWideString::~CWideString(){}
CBaseIntelligentPointer::CBaseIntelligentPointer():PData(0){}
CBaseObject*CBaseIntelligentPointer::GetPItem()const{return reinterpret_cast<CBaseObject*>(PData);}
void CBaseIntelligentPointer::SetPItem(CBaseObject*p){CBaseObject*old=GetPItem();if(old)--StateOf(old).References;PData=reinterpret_cast<CBaseObjectPointer*>(p);if(p)++StateOf(p).References;}
CBaseIntelligentPointer::~CBaseIntelligentPointer(){SetPItem(0);}
CTCBase::CTCBase(CThing&t):ParentThing(&t){}CTCBase::~CTCBase(){}
CTCDriverBase::CTCDriverBase(CThing&t):CTCBase(t){}CTCDriverBase::~CTCDriverBase(){}
CTCHealLife::CTCHealLife(CThing&t):CTCBase(t){}
CTCSoundPlayer::CTCSoundPlayer(CThing&t):CTCBase(t){}
CTCTargetingBase::CTCTargetingBase(CThing&t):CTCBase(t){}CTCTargetingBase::~CTCTargetingBase(){}
CTCEntityEvents::CTCEntityEvents(CThing&t):CTCBase(t){}
CTCDParticleEmitter::CTCDParticleEmitter(CThing&t):CTCDriverBase(t){}CTCDParticleEmitter::~CTCDParticleEmitter(){}
CResource::CResource():ResourceList(0),PrevResource(0),NextResource(0),ResourceSize(0),LastUsedFrame(0){}CResource::~CResource(){}
CDefClassBase::CDefClassBase():PDefManager(0),GlobalIndex(0),Setup(false),Template(false),DefaultValsApplied(false){}
CSubDefClassBase::CSubDefClassBase(){}CParentDefClassBase::CParentDefClassBase(){}CParentDefClassBase::~CParentDefClassBase(){}
CDefinitionManager::~CDefinitionManager(){}CFailedAllocationHandler::~CFailedAllocationHandler(){}CResourceList::~CResourceList(){}
void CIVCountedPointeeBase::Delete()const{abort();}
CClassFactory::CClassFactory(unsigned char){}CClassFactory::~CClassFactory(){}
CGameDefinitionManager::CGameDefinitionManager(){}CGameDefinitionManager::~CGameDefinitionManager(){}
CGame::CGame():CurrentGameComponent(0),Quit(false){}CGame::~CGame(){}
CGameComponent::CGameComponent(CGame&g):Quit(false),Running(false),Game(g){}CGameComponent::~CGameComponent(){}
void CGameComponent::Init(){}void CGameComponent::SetQuit(){Quit=true;}
CMainGameComponent::CMainGameComponent(CGame&g,const CMainGameComponentInit&):CGameComponent(g){}CMainGameComponent::~CMainGameComponent(){}
CMainGameComponentInit::CMainGameComponentInit(){}CMainGameComponentInit::~CMainGameComponentInit(){}
CThingManager::CThingManager(const CMainGameComponent&c,const CGameDefinitionManager&d,CWorld&w,CPlayerManager&p,CNavigatorManager&n,unsigned long*seed,unsigned long,unsigned long,unsigned long):CClassFactory(0),Component(c),DefinitionManager(d),World(w),WorldMap(*w.PWorldMap_ptr),WorldSeed(seed),PlayerManager(p),NavigationManager(n){}CThingManager::~CThingManager(){}
CThingSearchTools::CThingSearchTools(CThingManager&t,CWorldMap&w):WorldMap(w),PThingMapManager(0),ThingManager(t){}CThingSearchTools::~CThingSearchTools(){}
static CThingSearchTools*ActiveSearch;
const CThingSearchTools&CTCBase::PeekThingSearchTools()const{return *ActiveSearch;}
static CPlayer Player;static CThing* Target;
CPlayer&CThing::GetPlayer(){Player.ZTargeting=ZTargeting!=0;return Player;}
const C3DVector&CThing::GetPos()const{CurrentRecipient=ID(this);return StateOf(this).Pos;}
CThing*CTCTargetingBase::GetPTargetedThing()const{return Target;}
bool FABLE_FASTCALL CTCEnemy::AreThingsEnemies(const CThing&a,const CThing&b){return Enemy(&a)!=Enemy(&b);}
CCharString CTCHealLife::GetLevelEffectName(){return CCharString(EffectName);}
unsigned long CTCSoundPlayer::PlayAttachedCriteria(const CCharString&n,bool a,bool b,bool c,long d){assert(!a&&!b&&!c&&d==-1);const char*p=strrchr(n.GetAsCharArray(),'_');assert(p&&p[1]>='1'&&p[1]<='4');SoundLevel=p[1]-'1';++SoundCalls;return 700+SoundLevel;}
void CTCDParticleEmitter::AttachToThing(CThing&t,long n,const CCharString&dummy,long index,float delay){assert(n==0x28&&dummy.GetLength()==0&&index==0&&delay==0);AttachedIDs[AttachCalls++]=ID(&t);}
CHitParameters::CHitParameters(){}CHitParameters::~CHitParameters(){}
void CHitParameters::SetThingHitting(CThing&t){PThingHitting.Set(&t);}
namespace NEntityEvents {CEventBase::~CEventBase(){}CEventHitBase::~CEventHitBase(){}CEventHitBySpecialAbility::CEventHitBySpecialAbility(const CHitParameters&p,EHeroAbility a):Ability(a),Aggressive(false){assert(a==HERO_ABILITY_HEAL_LIFE_SPELL);PHitParams.Reset(new CHitParameters(p));}}
void CTCEntityEvents::AddEvent(NEntityEvents::CEventBase*e){NEntityEvents::CEventHitBySpecialAbility*v=static_cast<NEntityEvents::CEventHitBySpecialAbility*>(e);assert(v->PHitParams->PThingHitting.Get()&&ID(v->PHitParams->PThingHitting.Get())==0);EventIDs[EventCalls++]=ID(ParentThing);delete e;}
unsigned long FABLE_FASTCALL CCRC::Calc(unsigned long seed,const void*data,unsigned long length){assert(seed==0);const char*p=static_cast<const char*>(data);unsigned long n=0;for(unsigned long i=0;i<length;++i)n=(n*33+(unsigned char)p[i])&0xffffffffUL;return n;}
static CThingCreatureBase*Candidates[8];static int CandidateCount;static long SearchRadius;
bool CThingFilter_IsNot::operator()(const CThing&t)const{return &t!=&Thing;}
bool CThingFilter_In2DRange::operator()(const CThing&t)const{const C3DVector&p=StateOf(&t).Pos;float x=p.X-Pos.X,y=p.Y-Pos.Y;return x*x+y*y<=Range*Range;}
template<class T,class Container,class Filter>void CThingSearchTools::GetAllThingsInRoughArea(const C2DBoxI&area,const Filter&filter,Container&out)const{++SearchCalls;assert(area.TLX==10-SearchRadius&&area.TLY==-3-SearchRadius&&area.BRX==10+SearchRadius&&area.BRY==-3+SearchRadius);for(int i=0;i<CandidateCount;++i)if(filter(*Candidates[i]))out.push_back(CIntelligentPointer<T>(Candidates[i]));}
static CSpecialEffectsDef*FixtureEffectsPtr;
CDefPointer<CDefClassBase>CDefinitionManager::GetPDefFromGlobalIndex(long n){assert(n==19&&FixtureEffectsPtr);FixtureEffectsPtr->AddRef();return CDefPointer<CDefClassBase>(FixtureEffectsPtr);}
class FixtureHeal:public CTCHealLife{public:FixtureHeal(CThing&t):CTCHealLife(t){}ETCInterfaceType GetInterfaceType() const{abort();}CCharString GetClassTypeName() const{abort();}};
class FixtureSound:public CTCSoundPlayer{public:FixtureSound(CThing&t):CTCSoundPlayer(t){}ETCInterfaceType GetInterfaceType() const{abort();}CCharString GetClassTypeName() const{abort();}};
class FixtureTarget:public CTCTargetingBase{public:FixtureTarget(CThing&t):CTCTargetingBase(t){}ETCInterfaceType GetInterfaceType() const{abort();}CCharString GetClassTypeName() const{abort();}};
class FixtureEvents:public CTCEntityEvents{public:FixtureEvents(CThing&t):CTCEntityEvents(t){}ETCInterfaceType GetInterfaceType() const{abort();}CCharString GetClassTypeName() const{abort();}};
class FixtureEffects:public CSpecialEffectsDef{public:void Transfer(CPersistContext&) {abort();}void Copy(const CDefClassBase*) {abort();}unsigned long GetSizeofClass() {abort();}};
class FixtureHealDef:public CSpecialAbilitiesHealLifeDef{public:bool Available;CDefClassBase::CSubDefInfo Info;FixtureHealDef(){Info.DefIndex=19;Info.OriginalParentDefIndex=0;}const CDefClassBase::CSubDefInfo*GetPSubDefInfo(const CCharString&n)const{assert(strcmp(n.GetAsCharArray(),"CSpecialEffectsDef")==0);return Available?&Info:0;}void Transfer(CPersistContext&) {abort();}void Copy(const CDefClassBase*) {abort();}unsigned long GetSizeofClass() {abort();}};

static CThing Particles[16];
static CTCDParticleEmitter*Emitters[16];
CThing*FABLE_FASTCALL CTCSpecialAbilities::CreateParticleEffect(long n,const C3DVector&p){assert(n==37&&CreateCalls<16);CreateIDs[CreateCalls]=CurrentRecipient;CThing*q=&Particles[CreateCalls++];StateOf(q).Pos=p;return ParticleMode?q:0;}
template<class T>static void SeedVector(std::vector<T>&v,unsigned n){assert(!v._M_start);v._M_start=std::allocator<T>().allocate(n);v._M_finish=v._M_start;v._M_end_of_storage._M_data=v._M_start+n;}
static void AddTC(CThing&t,ETCInterfaceType id,CTCBase*p){if(!t.Components._M_start)SeedVector(static_cast<std::vector<std::pair<ETCInterfaceType,CTCBase*> >&>(t.Components),17);bool exists=false;for(unsigned k=0;k<t.Components.size();++k)if(t.Components[k].first==id){t.Components[k].second=p;exists=true;}if(!exists)t.Components.push_back(std::make_pair(id,p));for(unsigned k=1;k<t.Components.size();++k)for(unsigned j=k;j>0&&t.Components[j].first<t.Components[j-1].first;--j){std::pair<ETCInterfaceType,CTCBase*> tmp=t.Components[j];t.Components[j]=t.Components[j-1];t.Components[j-1]=tmp;}unsigned n=(unsigned)id;t.UsingComponents._M_w[n/(8*sizeof(unsigned long))]|=1UL<<(n%(8*sizeof(unsigned long)));}
static void InitThing(CThing&t,int id){StateOf(&t).ID=id;StateOf(&t).References=0;StateOf(&t).Enemy=false;memset(&t.UsingComponents,0,sizeof t.UsingComponents);t.Dead=false;t.ThingTypeFlags=8;}

void CBaseObject::Validate() const { abort(); }
void CClass::Save(CCharString&) { abort(); }
void CThingGameObject::OnHit(CHitParameters&) { abort(); }
void CThingCreatureBase::OnHit(CHitParameters&) { abort(); }
bool CThingCreatureBase::DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo& info) { abort(); }
void CTCBase::OnSerialise(CPersistContext&, bool) { abort(); }
void CTCBase::OnRemove() { abort(); }
void CTCBase::ConstructFromParams(const CTCParamsBase&) { abort(); }
void CTCBase::OnCreate() { abort(); }
void CTCBase::InitialActivate() { abort(); }
void CTCBase::OnInitialActivate() { abort(); }
void CTCBase::RenderUpdate(float) { abort(); }
bool CTCBase::HasFrameUpdate() const { abort(); }
void CTCBase::FrameUpdate() { abort(); }
void CTCBase::BulletTimeFrameUpdate() { abort(); }
void CTCBase::DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo&) { abort(); }
void CTCBase::Copy(const CTCBase&) { abort(); }
void CTCBase::OnDie() { abort(); }
void CTCBase::OnKill() { abort(); }
void CTCBase::OnLevelUnload() { abort(); }
void CTCBase::OnSetInLimbo(bool) { abort(); }
bool CTCBase::Validate(CCharString&) const { abort(); }
bool CTCBase::IsFreeForDeletion() const { abort(); }
bool CTCBase::IsSerialiseCreatable() const { abort(); }
bool CTCBase::IsSerialisableForHeroState() const { abort(); }
bool CTCBase::IsComponentSerialisable() const { abort(); }
bool CTCBase::IsDriver() const { abort(); }
bool CTCBase::IsCopyable() const { abort(); }
void CTCBase::GlobalUpdate(bool) { abort(); }
void CTCBase::HandleMessage(const NTCMessages::CMessageBase&) { abort(); }
bool CTCDriverBase::IsDriver()const { abort(); }
bool CTCDriverBase::IsDriverType(long)const { abort(); }
void CTCDriverBase::DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo&) { abort(); }
void CTCDriverBase::CreateAdditionalTCs() { abort(); }
ETCInterfaceType CTCTargetingBase::GetInterfaceType() const { abort(); }
void CTCTargetingBase::OnInitialActivate() { abort(); }
void CTCTargetingBase::BulletTimeFrameUpdate() { abort(); }
CThing * CTCTargetingBase::GetPBestThingForMeleeAttack() const { abort(); }
CThingCreatureBase * CTCTargetingBase::GetPBestEnemyCreatureForAttack() const { abort(); }
CThing * CTCTargetingBase::GetPBestThingForProjectileTargetingLock() const { abort(); }
CThingCreatureBase * CTCTargetingBase::GetPBestCreatureForInteractionTypeWithinRangeLimit(ECreatureInteractionType,float,float) const { abort(); }
CThingCreatureBase * CTCTargetingBase::GetPBestCreatureForInteractionType(ECreatureInteractionType) const { abort(); }
void CTCTargetingBase::SetPTargetedThing(CThing *) { abort(); }
CThing * CTCTargetingBase::GetPProjectileTargetedThing() const { abort(); }
CThing * CTCTargetingBase::GetPTargetLockedThing() const { abort(); }
bool CTCTargetingBase::GetTargetedPos(C3DVector &,float) const { abort(); }
bool CTCTargetingBase::IsTargetedThingInRange(unsigned long) const { abort(); }
void CTCTargetingBase::UpdateNearestInterestingThing() { abort(); }
ETCInterfaceType CTCDParticleEmitter::GetInterfaceType() const { abort(); }
CCharString CTCDParticleEmitter::GetClassTypeName() const { abort(); }
bool CTCDParticleEmitter::IsDriverType(long p0) const { abort(); }
void CTCDParticleEmitter::FrameUpdate() { abort(); }
bool CTCDParticleEmitter::HasFrameUpdate() const { abort(); }
void CTCDParticleEmitter::OnCreate() { abort(); }
void CTCDParticleEmitter::OnKill() { abort(); }
void CTCDParticleEmitter::OnDie() { abort(); }
void CTCDParticleEmitter::CreateAdditionalTCs() { abort(); }
void CTCDParticleEmitter::OnSerialise(CPersistContext & p0, bool p1) { abort(); }
bool CTCDParticleEmitter::IsComponentSerialisable() const { abort(); }
void CTCDParticleEmitter::DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo & p0) { abort(); }
void CTCDParticleEmitter::OnSetInLimbo(bool p0) { abort(); }
bool CResource::UnloadResource() { abort(); }
unsigned long CResource::GetUnloadableMemory() const { abort(); }
bool CResource::IsResourceLoaded() const { abort(); }
void CResource::OnResourceLoaded() { abort(); }
bool CResource::CanAbortResourceLoad(long) const { abort(); }
void CResource::OnResourceLoadAborted() { abort(); }
unsigned long CResource::GetDebugMemoryInUse() const { abort(); }
void CResource::ResetDebugMemoryInUse() { abort(); }
bool CResourceList::HandleFailedAllocation(unsigned long) { abort(); }
bool CDefClassBase::UnloadResource() { abort(); }
CDefString CDefClassBase::GetInstantiationName() const { abort(); }
void CDefClassBase::SetInstantiationName(CDefString) { abort(); }
void CDefClassBase::AddSubDef(const CCharString&, long, long) { abort(); }
void CDefClassBase::RemoveSubDef(const CCharString&) { abort(); }
CDefClassBase::CSubDefInfo* CDefClassBase::GetPSubDefInfo(const CCharString&) { abort(); }
const CDefClassBase::CSubDefInfo* CDefClassBase::GetPSubDefInfo(const CCharString&) const { abort(); }
bool CDefClassBase::IsParentDef() const { abort(); }
void CDefClassBase::PostTransfer() { abort(); }
void CDefClassBase::InternalSaveBinary(CDataOutputStream&) { abort(); }
void CDefClassBase::InternalLoadBinary(CDataInputStream&) { abort(); }
void CDefClassBase::Validate(const CValidateDefinitionInitBase&, const CCharString&) const { abort(); }
CVectorMap<unsigned long, CDefClassBase::CSubDefInfo, CKeyPairCompareLess<unsigned long, CDefClassBase::CSubDefInfo> >* CDefClassBase::GetPSubDefInfoMap() { abort(); }
CDefString CParentDefClassBase::GetInstantiationName() const { abort(); }
void CParentDefClassBase::SetInstantiationName(CDefString) { abort(); }
void CParentDefClassBase::AddSubDef(const CCharString&, long, long) { abort(); }
void CParentDefClassBase::RemoveSubDef(const CCharString&) { abort(); }
CDefClassBase::CSubDefInfo* CParentDefClassBase::GetPSubDefInfo(const CCharString&) { abort(); }
const CDefClassBase::CSubDefInfo* CParentDefClassBase::GetPSubDefInfo(const CCharString&) const { abort(); }
bool CParentDefClassBase::IsParentDef() const { abort(); }
void CParentDefClassBase::InternalSaveBinary(CDataOutputStream&) { abort(); }
void CParentDefClassBase::InternalLoadBinary(CDataInputStream&) { abort(); }
CVectorMap<unsigned long, CDefClassBase::CSubDefInfo, CKeyPairCompareLess<unsigned long, CDefClassBase::CSubDefInfo> >* CParentDefClassBase::GetPSubDefInfoMap() { abort(); }
void CClassFactory::AddNewClassType(const CClassTypeInit&) { abort(); }
void CClassFactory::OnCreateClass(CClass&) { abort(); }
void CClassFactory::OnDeleteClass(CClass&) { abort(); }
void CThingManager::OnCreateClass(CClass&) { abort(); }
void CThingManager::OnDeleteClass(CClass&) { abort(); }
void CGameDefinitionManager::InitAndCompile() { abort(); }
bool CMainGameComponent::Run(CGameComponent**) { abort(); }
void CMainGameComponent::ChangeTextureColourDepth(long) { abort(); }
void CMainGameComponent::OnPreDeviceReset() { abort(); }
bool CMainGameComponent::OnPostDeviceReset() { abort(); }
static void PopulateEffect(CSpecialEffectsDef&effect,bool found){typedef std::pair<const unsigned long,long> Value;typedef std::_Rb_tree_node<Value> Node;Node*h=effect.SpecialEffectsMap.ContainmentMap._M_t._M_header._M_data;Node*n=std::allocator<Node>().allocate(1);unsigned long key=CCRC::Calc(0,EffectName,(unsigned long)strlen(EffectName))+(found?0:1);new(&n->_M_value_field)Value(key,37);n->_M_color=true;n->_M_parent=h;n->_M_left=n->_M_right=0;h->_M_parent=h->_M_left=h->_M_right=n;effect.SpecialEffectsMap.ContainmentMap._M_t._M_node_count=1;}
static void RunCase(unsigned code){unsigned q=code;int level=q%4;q/=4;SoundAvailable=q%2;q/=2;TargetingAvailable=q%2;q/=2;ZTargeting=q%2;q/=2;int targetChoice=q%6;q/=6;bool available=q%2;q/=2;bool found=q%2;q/=2;ParticleMode=q%3;q/=3;int masks[4]={0,63,21,42};EventMask=masks[q%4];q/=4;int layout=q%3;q/=3;assert(q==0);
 SoundCalls=CreateCalls=AttachCalls=EventCalls=SearchCalls=0;SoundLevel=-1;CurrentRecipient=-1;
 CThingCreatureBase people[6];FixtureEvents e0(people[0]),e1(people[1]),e2(people[2]),e3(people[3]),e4(people[4]),e5(people[5]);FixtureEvents*events[6]={&e0,&e1,&e2,&e3,&e4,&e5};FixtureSound sound(people[0]);FixtureTarget targeting(people[0]);Target=0;for(int z=0;z<6;++z)InitThing(people[z],z);
 const float radii[4]={2.25f,4.5f,8.0f,12.5f};float radius=radii[level];SearchRadius=(long)radius;
 for(int i=0;i<6;++i){StateOf(&people[i]).Pos=C3DVector(10.5f+float(i),-3.5f,2.0f);if(EventMask&(1<<i))AddTC(people[i],TCI_ENTITY_EVENTS,events[i]);}
 StateOf(&people[3]).Enemy=true;people[4].Dead=true;people[5].ThingTypeFlags=0;StateOf(&people[5]).Pos.X=30.5f;
 if(layout==1){StateOf(&people[2]).Pos.X=10.5f+radius;StateOf(&people[3]).Pos.X=10.5f;StateOf(&people[4]).Pos.X=10.5f;}
 if(layout==2){StateOf(&people[1]).Pos.X=100.5f;StateOf(&people[2]).Pos.X=100.5f;StateOf(&people[3]).Pos.X=100.5f;StateOf(&people[4]).Pos.X=100.5f;}
 CThing*choices[6]={0,&people[0],&people[1],&people[3],&people[4],&people[5]};Target=choices[targetChoice];
 if(SoundAvailable)AddTC(people[0],TCI_SOUND_PLAYER,&sound);if(TargetingAvailable)AddTC(people[0],TCI_TARGETING,&targeting);
 CandidateCount=0;for(int i=0;i<6;++i)if(people[i].ThingTypeFlags&8)Candidates[CandidateCount++]=&people[i];
 for(int i=0;i<16;++i){assert(Refs(&Particles[i])==0);InitThing(Particles[i],100+i);Particles[i].Dead=ParticleMode==1;if(!Emitters[i])Emitters[i]=new CTCDParticleEmitter(Particles[i]);AddTC(Particles[i],TCI_PARTICLE_EMITTER,Emitters[i]);;}
 CDefinitionManager manager;FixtureHealDef def;FixtureEffects effects;FixtureEffectsPtr=&effects;def.PDefManager=&manager;def.Available=available;PopulateEffect(effects,found);
 SeedVector(def.HealthIncreasePerFrame,1);SeedVector(def.TotalHealthIncrease,1);SeedVector(def.RadiusOfEffect,4);for(int i=0;i<4;++i)def.RadiusOfEffect.push_back(radii[i]);
 // Independent outcome oracle: choose the target first, then enumerate recipient identities.
 CThing*selected=&people[0];if(TargetingAvailable&&ZTargeting&&Target&&!Target->Dead)selected=Target;if(Enemy(selected))selected=&people[0];
 int recipients[8],count=0,expectedCreates[8],createCount=0,expectedAttach[8],attachCount=0,expectedEvent[8],eventCount=0;
 if((selected->ThingTypeFlags&8)&&!selected->Dead){recipients[count++]=ID(selected);if(selected==&people[0])for(int i=0;i<CandidateCount;++i){CThing*t=Candidates[i];float dx=StateOf(t).Pos.X-StateOf(&people[0]).Pos.X,dy=StateOf(t).Pos.Y-StateOf(&people[0]).Pos.Y;if(t!=selected&&dx*dx+dy*dy<=radius*radius)recipients[count++]=ID(t);}}
 for(int j=0;j<count;++j){int id=recipients[j];if(!people[id].Dead&&StateOf(&people[id]).Enemy==Enemy(selected)){if(available&&found){expectedCreates[createCount++]=id;if(ParticleMode==2)expectedAttach[attachCount++]=id;}if(EventMask&(1<<id))expectedEvent[eventCount++]=id;}}
 {
  FixtureHeal heal(people[0]);heal.SpellLevel=level;heal.LoopingSound=987;heal.PDef.Reset(&def);SeedVector(heal.HealingEffects,1);SeedVector(heal.HealthIncreaseAccum,1);heal.StartHealingEffect();
  assert(SoundCalls==SoundAvailable&&SoundLevel==(SoundAvailable?level:-1));assert(heal.LoopingSound==(unsigned long)(SoundAvailable?700+level:987));assert(heal.PHealingTarget.Get()==selected);assert(SearchCalls==((count&&selected==&people[0])?1:0));assert((int)heal.HealingEffects.size()==count&&(int)heal.HealthIncreaseAccum.size()==count);
  int j=0,fxIndex=0;for(std::list<CIntelligentPointer<CThingCreatureBase> >::const_iterator it=heal.ThingsToHeal.begin();it!=heal.ThingsToHeal.end();++it,++j){assert(j<count&&ID((*it).Get())==recipients[j]);assert(heal.HealthIncreaseAccum[j]==0.0f);int id=recipients[j];bool gets=available&&found&&!people[id].Dead&&StateOf(&people[id]).Enemy==Enemy(selected);CThing*expected=gets&&ParticleMode==2?&Particles[fxIndex]:0;assert(heal.HealingEffects[j].Get()==expected);if(gets)++fxIndex;}
  assert(j==count);assert(CreateCalls==createCount&&AttachCalls==attachCount&&EventCalls==eventCount);for(j=0;j<createCount;++j)assert(CreateIDs[j]==expectedCreates[j]);for(j=0;j<attachCount;++j)assert(AttachedIDs[j]==expectedAttach[j]);for(j=0;j<eventCount;++j)assert(EventIDs[j]==expectedEvent[j]);
  for(j=0;j<6;++j){int expected=&people[j]==selected?1:0;for(int k=0;k<count;++k)if(recipients[k]==j)++expected;assert(Refs(&people[j])==expected);}for(j=0;j<CreateCalls;++j)assert(Refs(&Particles[j])==(ParticleMode==2?1:0));assert(def.IVRefCount==2&&effects.IVRefCount==1);
 }
 for(int i=0;i<6;++i)assert(Refs(&people[i])==0);for(int i=0;i<16;++i)assert(Refs(&Particles[i])==0);assert(def.IVRefCount==1&&effects.IVRefCount==1);
}
static void TestBounds(){for(int n=0;n<=16;++n){CVectorMap<ETCInterfaceType,CTCBase*> map;SeedVector(static_cast<std::vector<std::pair<ETCInterfaceType,CTCBase*> >&>(map),17);for(int i=0;i<n;++i)map.push_back(std::pair<ETCInterfaceType,CTCBase*>((ETCInterfaceType)(i*2),0));for(int k=0;k<=34;++k){int expected=0;while(expected<n&&expected*2<k)++expected;ETCInterfaceType key=(ETCInterfaceType)k;assert(map.LowerBound(key)==map.begin()+expected);const CVectorMap<ETCInterfaceType,CTCBase*>&cm=map;assert(cm.LowerBound(key)==cm.begin()+expected);}}}
static void TestOverflow(){CThing objects[3];for(int old=0;old<=6;++old)for(int pos=0;pos<=old;++pos)for(int fill=1;fill<=3;++fill){std::vector<CIntelligentPointer<CThing> > v;SeedVector(v,(unsigned)(old+2));for(int i=0;i<old;++i)v.push_back(CIntelligentPointer<CThing>(&objects[i%3]));CIntelligentPointer<CThing> value(&objects[1]);v._M_insert_overflow(v.begin()+pos,value,std::__false_type(),fill,pos==old);assert((int)v.size()==old+fill);for(int i=0;i<old+fill;++i){CThing*expected=i<pos?&objects[i%3]:i<pos+fill?&objects[1]:&objects[(i-fill)%3];assert(v[i].Get()==expected);}int counts[3]={0,1,0};for(int i=0;i<old;++i)++counts[i%3];counts[1]+=fill;for(int i=0;i<3;++i)assert(Refs(&objects[i])==counts[i]);}for(int i=0;i<3;++i)assert(Refs(&objects[i])==0);
for(int old=0;old<=6;++old)for(int pos=0;pos<=old;++pos)for(int fill=1;fill<=3;++fill){std::vector<float> v;SeedVector(v,(unsigned)(old+2));for(int i=0;i<old;++i)v.push_back(float(i)+0.5f);float value=-2.25f;v._M_insert_overflow(v.begin()+pos,value,std::__true_type(),fill,pos==old);assert((int)v.size()==old+fill);for(int i=0;i<old+fill;++i){float expected=i<pos?float(i)+0.5f:i<pos+fill?value:float(i-fill)+0.5f;assert(v[i]==expected);}}}
int main(){CGame game;CMainGameComponentInit ci;CMainGameComponent component(game,ci);CWorld world;CWorldMap map;world.PWorldMap_ptr=&map;CGameDefinitionManager definitions;CPlayerManager players;CNavigatorManager navigation;unsigned long seed=0;CThingManager things(component,definitions,world,players,navigation,&seed,0,0,0);CThingSearchTools search(things,map);ActiveSearch=&search;TestBounds();TestOverflow();for(unsigned i=0;i<27648;++i)RunCase(i);for(int z=0;z<16;++z){delete Emitters[z];Emitters[z]=0;}printf("007c4430_TEST PASS\n");return 0;}