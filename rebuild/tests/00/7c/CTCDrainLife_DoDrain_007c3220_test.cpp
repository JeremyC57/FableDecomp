#include <assert.h>
#include <stdio.h>
#include <string.h>
static unsigned mask;
static int diagnostics, helpers, draws, events, attachments, hits, created, healed, souls, particles, noise, indices;
static float expectedDamage;
#define FABLE_RETAIL_ASSERT(x) ((void)((x) || (++diagnostics,0)))
enum { DAMAGE_DRAIN=6,HERO_ABILITY_DRAIN_LIFE_SPELL=6,NULL_DEF=0 };
struct CCharString {const char* s;CCharString(const char* v):s(v){}};
const CCharString MAIN_FOCAL_POSITION_NAME("MAIN_FOCAL_POSITION");
struct C3DVector {int x; C3DVector(int v=0):x(v){} C3DVector operator-(C3DVector o)const{return C3DVector(x-o.x);} };
template<class T> struct CDefPointer { T* p; CDefPointer():p(0){} T* operator->()const{return p;} };
struct CSpecialAbilitiesDrainLifeDataDef {float Damage;};
struct CSpecialEffectsDef {bool GetEffectFromString(CCharString n,long& e) const {assert(!strcmp(n.s,"DAMAGE_EFFECT"));e=72;return !!(mask&64);}};
struct Def {CSpecialAbilitiesDrainLifeDataDef dmg;CSpecialEffectsDef effect;float AmountOfHealthSucked[2];int NumEffects[2];const char* ShotName[2];
bool GetSubDef(CDefPointer<const CSpecialAbilitiesDrainLifeDataDef>* v){v->p=&dmg;return !!(mask&1);}
bool GetSubDef(CDefPointer<const CSpecialEffectsDef>* v){v->p=&effect;return !!(mask&32);}
} def;
struct Manager {long GetDefGlobalIndexFromName(const char* n){++indices;assert(!strcmp(n,def.ShotName[!!(mask&4096)]));return 99;}} manager;
struct CThing;struct CThingCreatureBase;
static CThingCreatureBase *parent,*victim;
struct CHitParameters {float amount; bool response,dust;
CHitParameters(int a,CThingCreatureBase* b,int c,C3DVector p,C3DVector d,bool e,bool f,bool g,bool h):amount(-1),response(true),dust(true){
assert(!a&&b==parent&&!c&&p.x==((mask&6)==6?41:21)&&d.x==16&&!e&&!f&&!g&&!h);}
void AddDamageWithAttribute(int a,float v){assert(a==6&&v==expectedDamage);amount=v;}
void SetPlayHitResponse(bool v){response=v;}void SetCreateDustParticleEffectOnHit(bool v){dust=v;}
bool IsBlocked(){return !!(mask&8);}bool IsHitNegated(){return !!(mask&16);}
};
namespace NGraphicAppearance {struct CTCGraphicAppearance {bool GetHelperPoint(const CCharString& n,int a,int b,C3DVector& p,float scale){
++helpers;assert(&n==&MAIN_FOCAL_POSITION_NAME&&!a&&!b&&p.x==21&&scale==1);if(mask&4){p.x=41;return true;}return false;}};} 
namespace NEntityEvents {struct CEventHitBySpecialAbility {CEventHitBySpecialAbility(CHitParameters& h,int t){assert(h.amount==expectedDamage&&t==6);}};}
struct CTCEntityEvents {void AddEvent(NEntityEvents::CEventHitBySpecialAbility* v){++events;delete v;}};
struct CTCDParticleEmitter {void AttachToThing(CThingCreatureBase& v,int a,const CCharString& n,int b,float c){++attachments;assert(&v==victim&&a==12&&&n==&MAIN_FOCAL_POSITION_NAME&&!b&&c==0);}};
namespace NThingShot {struct CTCShotDefinitionDrainLife {void SetAmountToHeal(float v){++healed;float expected=expectedDamage/def.NumEffects[!!(mask&4096)];assert(memcmp(&v, &expected, sizeof(expected)) == 0);}void SetSoulEffect(long s){++souls;assert(s==853);}};}
struct CThing {bool alive,hasDrain;int loc;CTCDParticleEmitter emit;NThingShot::CTCShotDefinitionDrainLife drain;
bool GetTC(CTCDParticleEmitter** p){*p=&emit;return !!(mask&256);}
bool GetTC(NThingShot::CTCShotDefinitionDrainLife** p){*p=&drain;return hasDrain;}
void SetAsDrawable(bool v){++draws;assert(!v);} C3DVector GetPos(){return loc;}};
#define IS_THING_ALIVE(p) ((p)&&(p)->alive)
struct Physics {int rh;int& GetRHSet(){return rh;}};
struct CThingCreatureBase:CThing {NGraphicAppearance::CTCGraphicAppearance appearance;CTCEntityEvents ev;Physics phys;
void MakeNoise(const char* n){++noise;assert(this==parent&&!strcmp(n,"SIMPLE_NOISE"));}
Def* GetPBaseDef(){return &def;}float GetHealth(){return 10;}
NGraphicAppearance::CTCGraphicAppearance* GetTCGraphicAppearance(){return mask&2?&appearance:0;}
void OnHit(CHitParameters& h){++hits;assert(this==victim&&h.amount==expectedDamage&&!h.response&&!h.dust);alive=!(mask&1024);}
bool GetTC(CTCEntityEvents** p){*p=&ev;return !!(mask&2048);}
bool IsDamageable(){return !!(mask&512);}Manager& PeekDefinitionManager(){assert(this==parent);return manager;}
Physics* GetPhysicsTC(){return &phys;} };
CThingCreatureBase* GFDebugDynamicCast(CThingCreatureBase* p,CThingCreatureBase*){assert(p==parent);return p;}
static CThing fx,projectiles[4];static int pattern;
struct CTCSpecialAbilities {static CThing* CreateParticleEffect(long e,C3DVector p){++particles;assert(e==72&&p.x==21);fx.alive=!!(mask&128);return mask&8192?0:&fx;}};
namespace NThingShot {CThing* CreateShot(long idx,CThingCreatureBase* v,int o,CThingCreatureBase* pc,float a,float b,float c,int nd,C3DVector* where,int* rh){
assert(idx==99&&v==victim&&!o&&pc==parent&&a==100&&b==1&&c==0&&!nd&&where->x==((mask&6)==6?41:21)&&rh==&victim->phys.rh);
int i=created++,state=(pattern+i)%4;CThing& t=projectiles[i];t.alive=state>=2;t.hasDrain=state==3;return state?&t:0;}}
template<class T> struct CIntelligentPointer {T* p;CIntelligentPointer(T* v):p(v){}};
struct ShotList {int size;CThing* p[4];void push_back(CIntelligentPointer<CThing> v){p[size++]=v.p;}};
struct CTCDrainLife {Def* PDef;int SpellLevel;ShotList Shots;long GetSoulEffect()const{return 853;}CThingCreatureBase& GetParentThing(){return *parent;}
void DoDrain(CThingCreatureBase*);};
void CTCDrainLife::DoDrain(CThingCreatureBase* pcreature)
{
    CThingCreatureBase& parent_creature = *GFDebugDynamicCast(&GetParentThing(), (CThingCreatureBase*)0);
    parent_creature.MakeNoise("SIMPLE_NOISE");
    float suck = PDef->AmountOfHealthSucked[SpellLevel];
    CDefPointer<const CSpecialAbilitiesDrainLifeDataDef> psubdef;
    if (GetParentThing().GetPBaseDef()->GetSubDef(&psubdef))
        suck = psubdef->Damage;
    float health = pcreature->GetHealth();
    if (suck > health)
        suck = health;
    C3DVector hit_pos = pcreature->GetPos();
    NGraphicAppearance::CTCGraphicAppearance* ptc_graphic_appearance = pcreature->GetTCGraphicAppearance();
    if (ptc_graphic_appearance)
        if (!ptc_graphic_appearance->GetHelperPoint(MAIN_FOCAL_POSITION_NAME,0,0,hit_pos,1.0f)) { FABLE_RETAIL_ASSERT(!"Thingy doesn't have a focus point for drainlife HIT"); }
    CHitParameters hit_params(0,&parent_creature,0,hit_pos,pcreature->GetPos()-parent_creature.GetPos(),false,false,false,false);
    hit_params.AddDamageWithAttribute(DAMAGE_DRAIN,suck);
    hit_params.SetPlayHitResponse(false);
    hit_params.SetCreateDustParticleEffectOnHit(false);
    pcreature->OnHit(hit_params);
    if (IS_THING_ALIVE(pcreature)) {
        CTCEntityEvents* ptc_entity_events;
        if (pcreature->GetTC(&ptc_entity_events))
            ptc_entity_events->AddEvent(new NEntityEvents::CEventHitBySpecialAbility(hit_params,HERO_ABILITY_DRAIN_LIFE_SPELL));
    }
    if (!hit_params.IsBlocked() && !hit_params.IsHitNegated() && IS_THING_ALIVE(pcreature)) {
        CDefPointer<const CSpecialEffectsDef> pspecial_effects;
        long effect=0;
        CCharString effectname="DAMAGE_EFFECT";
        if (PDef->GetSubDef(&pspecial_effects) && pspecial_effects->GetEffectFromString(effectname,effect)) {
            CThing* fx=CTCSpecialAbilities::CreateParticleEffect(effect,pcreature->GetPos());
            CTCDParticleEmitter* emitter;
            if (IS_THING_ALIVE(fx) && fx->GetTC(&emitter))
                emitter->AttachToThing(*pcreature,12,MAIN_FOCAL_POSITION_NAME,0,0.0f);
        }
        if (pcreature->IsDamageable()) {
            long num_effects = PDef->NumEffects[SpellLevel];
            float per_effect_drain = suck/num_effects;
            long global_index = parent_creature.PeekDefinitionManager().GetDefGlobalIndexFromName(PDef->ShotName[SpellLevel]);
            if (false) {} else for (long i=0;i<num_effects;++i) {
                C3DVector creation_pos=pcreature->GetPos();
                if (ptc_graphic_appearance)
                    if (!ptc_graphic_appearance->GetHelperPoint(MAIN_FOCAL_POSITION_NAME,0,0,creation_pos,1.0f)) { FABLE_RETAIL_ASSERT(!"Thingy doesn't have a focus point for drainlife"); }
                CThing* pshot=NThingShot::CreateShot(global_index,pcreature,0,&parent_creature,100.0f,1.0f,0.0f,
                    NULL_DEF,&creation_pos,&pcreature->GetPhysicsTC()->GetRHSet());
                if (IS_THING_ALIVE(pshot)) {
                    pshot->SetAsDrawable(false);
                    NThingShot::CTCShotDefinitionDrainLife* ptc_shot_drainlife;
                    if (pshot->GetTC(&ptc_shot_drainlife)) {
                        ptc_shot_drainlife->SetAmountToHeal(per_effect_drain);
                        ptc_shot_drainlife->SetSoulEffect(GetSoulEffect());
                    }
                    Shots.push_back(CIntelligentPointer<CThing>(pshot));
                }
            }
        }
    }
}
int main(){
 CThingCreatureBase pc,vc;parent=&pc;victim=&vc;pc.loc=5;vc.loc=21;
 def.AmountOfHealthSucked[0]=4;def.AmountOfHealthSucked[1]=13;def.dmg.Damage=9;
 def.ShotName[0]="A";def.ShotName[1]="B";
 for(mask=0;mask<16384;++mask)for(pattern=0;pattern<4;++pattern){
  diagnostics=helpers=draws=events=attachments=hits=created=healed=souls=particles=noise=indices=0;
  CTCDrainLife d;d.PDef=&def;d.SpellLevel=!!(mask&4096);d.Shots.size=0;
  int n=pattern==0?0:pattern==1?1:3;def.NumEffects[d.SpellLevel]=n;
  expectedDamage=mask&1?9.0f:d.SpellLevel?10.0f:4.0f;vc.alive=true;d.DoDrain(&vc);
  bool alive=!(mask&1024),gate=alive&&!(mask&24),shoot=gate&&!!(mask&512);
  assert(noise==1&&hits==1&&events==(alive&&!!(mask&2048)));
  assert(indices==int(shoot)&&created==(shoot?n:0)&&particles==(gate&&(mask&96)==96));
  assert(attachments==(particles&&!(mask&8192)&&(mask&384)==384));
  assert(helpers==(mask&2?1+created:0)&&diagnostics==(mask&4?0:helpers));
  int visible=0,full=0;
  for(int i=0;i<created;++i){int state=(i+pattern)%4;if(state>=2){assert(d.Shots.p[visible]==projectiles+i);++visible;}if(state==3)++full;}
  assert(draws==visible&&d.Shots.size==visible&&healed==full&&souls==full);
 }
 puts("007c3220_TEST PASS");return 0;
}