#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <new>
#include <math.h>
#include "rebuild_abi.h"
#include "fable_compiler.h"
#ifndef _MSC_VER
#define __declspec(x)
#endif
// Fixture-only access allows valid nonnull backing even for an empty STLport range.
#define private public
#define protected public
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFPOINTER_POINTER_CTOR_ADOPT
#define FABLE_RETAIL_CDEFPOINTER_RESET
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY_ORIGINAL_CHAIN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_CONVERT_MUTABLE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_GET_ORIGINAL_CHAIN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_DEREFERENCE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_EQUAL_RAW
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CTCHEALLIFE_START_ORIGINAL
#define FABLE_RETAIL_CSPECIALEFFECTSDEF_TYPENAME
#define FABLE_RETAIL_CSPECIALEFFECTSDEF_GETEFFECT_BODY
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_STLP_TREE_CONSTFIND
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/C2DCoordI.h"
#include "engine/C2DBoxI.h"
#include "engine/C3DVector.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CDefClassBase.h"
#include "engine/CDefinitionManager.h"
#include "engine/CSpecialEffectsDef.h"
#include "engine/CDefClassBase_GetSubDef.h"
#include "engine/CSpecialAbilitiesHealLifeDef.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CTCBase.h"
#include "engine/EHeroAbility.h"
class CBaseObject {public:int References;CBaseObject():References(0){}virtual~CBaseObject(){assert(References==0);}};
CBaseIntelligentPointer::CBaseIntelligentPointer():PData(0){}
CBaseObject*CBaseIntelligentPointer::GetPItem()const{return reinterpret_cast<CBaseObject*>(PData);}
void CBaseIntelligentPointer::SetPItem(CBaseObject*p){CBaseObject*old=GetPItem();if(old)--old->References;PData=reinterpret_cast<CBaseObjectPointer*>(p);if(p)++p->References;}
CBaseIntelligentPointer::~CBaseIntelligentPointer(){SetPItem(0);}
static int SoundCalls,SoundLevel,CreateCalls,AttachCalls,EventCalls,SearchCalls,ExpectedEvents,SoundAvailable,TargetingAvailable,ZTargeting,ParticleMode,EventMask;
static int AttachedIDs[16],EventIDs[16],CreateIDs[16],CurrentRecipient;
class CPlayer {public:bool IsZTargeting()const{return ZTargeting!=0;}};static CPlayer Player;
class CThing:public CBaseObject {public:int ID;bool Dead,Enemy;unsigned long Flags;C3DVector Pos;void*Components[272];CThing():ID(-1),Dead(false),Enemy(false),Flags(8),Pos(0,0,0){memset(Components,0,sizeof Components);}bool IsDead()const{return Dead;}unsigned long GetTypeFlags()const{return Flags;}const C3DVector&GetPos()const{CurrentRecipient=ID;return Pos;}CPlayer&GetPlayer(){return Player;}template<class T>bool GetTC(T**out){int n=T::StatGetInterfaceType();if(!Components[n])return false;*out=static_cast<T*>(Components[n]);return true;}template<class T>bool PeekTC(T**out)const{int n=T::StatGetInterfaceType();if(!Components[n])return false;*out=static_cast<T*>(Components[n]);return true;}};
class CThingCreatureBase:public CThing {};
template<class F,class T>inline T*FABLE_FASTCALL GFDebugDynamicCast(F*p,T*){return static_cast<T*>(p);}
template<class F,class T>inline T FABLE_FASTCALL GFDebugPDynamicCast(F p,T){return static_cast<T>(p);}
class CTCSoundPlayer {public:static ETCInterfaceType StatGetInterfaceType(){return TCI_SOUND_PLAYER;}unsigned long PlayAttachedCriteria(const CCharString&n,bool a,bool b,bool c,long d){assert(!a&&!b&&!c&&d==-1);const char*p=strrchr(n.Text,'_');assert(p&&p[1]>='1'&&p[1]<='4');SoundLevel=p[1]-'1';++SoundCalls;return 700+SoundLevel;}};
class CTCTargetingBase {public:CThing*Target;static ETCInterfaceType StatGetInterfaceType(){return TCI_TARGETING;}virtual CThing*GetPTargetedThing()const{return Target;}};
class CTCEnemy {public:static bool AreThingsEnemies(const CThing&a,const CThing&b){return a.Enemy!=b.Enemy;}};
class CTCDParticleEmitter {public:static ETCInterfaceType StatGetInterfaceType(){return TCI_PARTICLE_EMITTER;}void AttachToThing(CThing&t,long n,const CCharString&dummy,long index,float delay){assert(n==0x28&&dummy.Text[0]==0&&index==0&&delay==0);AttachedIDs[AttachCalls++]=t.ID;}};
static CThing Particles[16];static CTCDParticleEmitter Emitters[16];
class CTCSpecialAbilities {public:static CThing*CreateParticleEffect(long n,const C3DVector&p){assert(n==37);assert(CreateCalls<16);CreateIDs[CreateCalls]=CurrentRecipient;CThing*q=&Particles[CreateCalls++];q->Pos=p;return ParticleMode? q:0;}};
class CHitParameters {public:CThing*Hitter;CHitParameters():Hitter(0){}void SetThingHitting(CThing&t){Hitter=&t;}};
namespace NEntityEvents {class CEventBase {public:virtual~CEventBase(){}};class CEventHitBySpecialAbility:public CEventBase {public:CThing*Hitter;CEventHitBySpecialAbility(const CHitParameters&p,EHeroAbility a):Hitter(p.Hitter){assert(a==HERO_ABILITY_HEAL_LIFE_SPELL);}};}
class CTCEntityEvents {public:int ID;static ETCInterfaceType StatGetInterfaceType(){return TCI_ENTITY_EVENTS;}void AddEvent(NEntityEvents::CEventBase*e){NEntityEvents::CEventHitBySpecialAbility*v=static_cast<NEntityEvents::CEventHitBySpecialAbility*>(e);assert(v->Hitter&&v->Hitter->ID==0);EventIDs[EventCalls++]=ID;delete e;}};
#include "engine/CFilter_Bind2.h"
#include "engine/CThingFilter_IsNot.h"
#include "engine/CThingFilter_In2DRange.h"
bool CThingFilter_IsNot::operator()(const CThing&t)const{return &t!=&Thing;}
bool CThingFilter_In2DRange::operator()(const CThing&t)const{float x=t.Pos.X-Pos.X,y=t.Pos.Y-Pos.Y;return x*x+y*y<=Range*Range;}
static CThingCreatureBase*Candidates[8];static int CandidateCount;static long SearchRadius;
class CThingSearchTools {public:template<class T,class Container,class Filter>void GetAllThingsInRoughArea(const C2DBoxI&area,const Filter&filter,Container&out)const{++SearchCalls;assert(area.TLX==10-SearchRadius&&area.TLY==-3-SearchRadius&&area.BRX==10+SearchRadius&&area.BRY==-3+SearchRadius);for(int i=0;i<CandidateCount;++i)if(filter(*Candidates[i]))out.push_back(CIntelligentPointer<T>(Candidates[i]));}};CThingSearchTools SearchTools;
#include "engine/ThingAlive.h"
#include "engine/CVectorMap.h"
#include "engine/CTCHealLife.h"
static const char*EffectName="HEAL_FIXTURE_EFFECT";
CCharString CTCHealLife::GetLevelEffectName(){return CCharString(EffectName);}
unsigned long CCRC::Calc(unsigned long seed,const void*data,unsigned long length){assert(seed==0);const char*p=static_cast<const char*>(data);unsigned long n=0;for(unsigned long i=0;i<length;++i)n=(n*33+(unsigned char)p[i])&0xffffffffUL;return n;}
#undef protected
#undef private

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

template<class T>static void SeedVector(std::vector<T>&v,unsigned n){assert(!v._M_start);v._M_start=std::allocator<T>().allocate(n);v._M_finish=v._M_start;v._M_end_of_storage._M_data=v._M_start+n;}
static void PopulateEffect(CSpecialEffectsDef&effect,bool found){typedef std::pair<const unsigned long,long> Value;typedef std::_Rb_tree_node<Value> Node;Node*h=effect.SpecialEffectsMap.ContainmentMap._M_t._M_header._M_data;Node*n=std::allocator<Node>().allocate(1);unsigned long key=CCRC::Calc(0,EffectName,(unsigned long)strlen(EffectName))+(found?0:1);new(&n->_M_value_field)Value(key,37);n->_M_color=true;n->_M_parent=h;n->_M_left=n->_M_right=0;h->_M_parent=h->_M_left=h->_M_right=n;effect.SpecialEffectsMap.ContainmentMap._M_t._M_node_count=1;}
static void RunCase(unsigned code){unsigned q=code;int level=q%4;q/=4;SoundAvailable=q%2;q/=2;TargetingAvailable=q%2;q/=2;ZTargeting=q%2;q/=2;int targetChoice=q%6;q/=6;bool available=q%2;q/=2;bool found=q%2;q/=2;ParticleMode=q%3;q/=3;int masks[4]={0,63,21,42};EventMask=masks[q%4];q/=4;int layout=q%3;q/=3;assert(q==0);
 SoundCalls=CreateCalls=AttachCalls=EventCalls=SearchCalls=0;SoundLevel=-1;CurrentRecipient=-1;
 CThingCreatureBase people[6];CTCEntityEvents events[6];CTCSoundPlayer sound;CTCTargetingBase targeting;targeting.Target=0;
 const float radii[4]={2.25f,4.5f,8.0f,12.5f};float radius=radii[level];SearchRadius=(long)radius;
 for(int i=0;i<6;++i){people[i].ID=i;people[i].Pos=C3DVector(10.5f+float(i),-3.5f,2.0f);events[i].ID=i;if(EventMask&(1<<i))people[i].Components[TCI_ENTITY_EVENTS]=&events[i];}
 people[3].Enemy=true;people[4].Dead=true;people[5].Flags=0;people[5].Pos.X=30.5f;
 if(layout==1){people[2].Pos.X=10.5f+radius;people[3].Pos.X=10.5f;people[4].Pos.X=10.5f;}
 if(layout==2){people[1].Pos.X=100.5f;people[2].Pos.X=100.5f;people[3].Pos.X=100.5f;people[4].Pos.X=100.5f;}
 CThing*choices[6]={0,&people[0],&people[1],&people[3],&people[4],&people[5]};targeting.Target=choices[targetChoice];
 if(SoundAvailable)people[0].Components[TCI_SOUND_PLAYER]=&sound;if(TargetingAvailable)people[0].Components[TCI_TARGETING]=&targeting;
 CandidateCount=0;for(int i=0;i<6;++i)if(people[i].Flags&8)Candidates[CandidateCount++]=&people[i];
 for(int i=0;i<16;++i){assert(Particles[i].References==0);Particles[i].ID=100+i;Particles[i].Dead=ParticleMode==1;Particles[i].Components[TCI_PARTICLE_EMITTER]=&Emitters[i];}
 CDefinitionManager manager;CSpecialAbilitiesHealLifeDef def;CSpecialEffectsDef effects;manager.Effect=&effects;def.PDefManager=&manager;def.Available=available;PopulateEffect(effects,found);
 SeedVector(def.HealthIncreasePerFrame,1);SeedVector(def.TotalHealthIncrease,1);SeedVector(def.RadiusOfEffect,4);for(int i=0;i<4;++i)def.RadiusOfEffect.push_back(radii[i]);
 // Independent outcome oracle: choose the target first, then enumerate recipient identities.
 CThing*selected=&people[0];if(TargetingAvailable&&ZTargeting&&targeting.Target&&!targeting.Target->Dead)selected=targeting.Target;if(selected->Enemy)selected=&people[0];
 int recipients[8],count=0,expectedCreates[8],createCount=0,expectedAttach[8],attachCount=0,expectedEvent[8],eventCount=0;
 if((selected->Flags&8)&&!selected->Dead){recipients[count++]=selected->ID;if(selected==&people[0])for(int i=0;i<CandidateCount;++i){CThing*t=Candidates[i];float dx=t->Pos.X-people[0].Pos.X,dy=t->Pos.Y-people[0].Pos.Y;if(t!=selected&&dx*dx+dy*dy<=radius*radius)recipients[count++]=t->ID;}}
 for(int j=0;j<count;++j){int id=recipients[j];if(!people[id].Dead&&people[id].Enemy==selected->Enemy){if(available&&found){expectedCreates[createCount++]=id;if(ParticleMode==2)expectedAttach[attachCount++]=id;}if(EventMask&(1<<id))expectedEvent[eventCount++]=id;}}
 {
  CTCHealLife heal;heal.ParentThing=&people[0];heal.SpellLevel=level;heal.LoopingSound=987;heal.PDef.Reset(&def);SeedVector(heal.HealingEffects,1);SeedVector(heal.HealthIncreaseAccum,1);heal.StartHealingEffect();
  assert(SoundCalls==SoundAvailable&&SoundLevel==(SoundAvailable?level:-1));assert(heal.LoopingSound==(unsigned long)(SoundAvailable?700+level:987));assert(heal.PHealingTarget.Get()==selected);assert(SearchCalls==((count&&selected==&people[0])?1:0));assert((int)heal.HealingEffects.size()==count&&(int)heal.HealthIncreaseAccum.size()==count);
  int j=0,fxIndex=0;for(std::list<CIntelligentPointer<CThingCreatureBase> >::const_iterator it=heal.ThingsToHeal.begin();it!=heal.ThingsToHeal.end();++it,++j){assert(j<count&&(*it)->ID==recipients[j]);assert(heal.HealthIncreaseAccum[j]==0.0f);int id=recipients[j];bool gets=available&&found&&!people[id].Dead&&people[id].Enemy==selected->Enemy;CThing*expected=gets&&ParticleMode==2?&Particles[fxIndex]:0;assert(heal.HealingEffects[j].Get()==expected);if(gets)++fxIndex;}
  assert(j==count);assert(CreateCalls==createCount&&AttachCalls==attachCount&&EventCalls==eventCount);for(j=0;j<createCount;++j)assert(CreateIDs[j]==expectedCreates[j]);for(j=0;j<attachCount;++j)assert(AttachedIDs[j]==expectedAttach[j]);for(j=0;j<eventCount;++j)assert(EventIDs[j]==expectedEvent[j]);
  for(j=0;j<6;++j){int expected=&people[j]==selected?1:0;for(int k=0;k<count;++k)if(recipients[k]==j)++expected;assert(people[j].References==expected);}for(j=0;j<CreateCalls;++j)assert(Particles[j].References==(ParticleMode==2?1:0));assert(def.IVRefCount==2&&effects.IVRefCount==1);
 }
 for(int i=0;i<6;++i)assert(people[i].References==0);for(int i=0;i<16;++i)assert(Particles[i].References==0);assert(def.IVRefCount==1&&effects.IVRefCount==1);
}
static void TestBounds(){for(int n=0;n<=16;++n){CVectorMap<ETCInterfaceType,CTCBase*> map;SeedVector(static_cast<std::vector<std::pair<ETCInterfaceType,CTCBase*> >&>(map),17);for(int i=0;i<n;++i)map.push_back(std::pair<ETCInterfaceType,CTCBase*>((ETCInterfaceType)(i*2),0));for(int k=0;k<=34;++k){int expected=0;while(expected<n&&expected*2<k)++expected;ETCInterfaceType key=(ETCInterfaceType)k;assert(map.LowerBound(key)==map.begin()+expected);const CVectorMap<ETCInterfaceType,CTCBase*>&cm=map;assert(cm.LowerBound(key)==cm.begin()+expected);}}}
static void TestOverflow(){CThing objects[3];for(int old=0;old<=6;++old)for(int pos=0;pos<=old;++pos)for(int fill=1;fill<=3;++fill){std::vector<CIntelligentPointer<CThing> > v;SeedVector(v,(unsigned)(old+2));for(int i=0;i<old;++i)v.push_back(CIntelligentPointer<CThing>(&objects[i%3]));CIntelligentPointer<CThing> value(&objects[1]);v._M_insert_overflow(v.begin()+pos,value,std::__false_type(),fill,pos==old);assert((int)v.size()==old+fill);for(int i=0;i<old+fill;++i){CThing*expected=i<pos?&objects[i%3]:i<pos+fill?&objects[1]:&objects[(i-fill)%3];assert(v[i].Get()==expected);}int counts[3]={0,1,0};for(int i=0;i<old;++i)++counts[i%3];counts[1]+=fill;for(int i=0;i<3;++i)assert(objects[i].References==counts[i]);}for(int i=0;i<3;++i)assert(objects[i].References==0);
for(int old=0;old<=6;++old)for(int pos=0;pos<=old;++pos)for(int fill=1;fill<=3;++fill){std::vector<float> v;SeedVector(v,(unsigned)(old+2));for(int i=0;i<old;++i)v.push_back(float(i)+0.5f);float value=-2.25f;v._M_insert_overflow(v.begin()+pos,value,std::__true_type(),fill,pos==old);assert((int)v.size()==old+fill);for(int i=0;i<old+fill;++i){float expected=i<pos?float(i)+0.5f:i<pos+fill?value:float(i-fill)+0.5f;assert(v[i]==expected);}}}
int main(){TestBounds();TestOverflow();for(unsigned i=0;i<27648;++i)RunCase(i);printf("007c4430_TEST PASS\n");return 0;}
