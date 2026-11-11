// Self-contained C++03 fixture; selected production function body copied unchanged.
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <map>
#include <vector>
static long cases=0;
#define CHECK(x)do{if(!(x)){std::fprintf(stderr,"case %ld line %d: %s\n",cases,__LINE__,#x);std::exit(1);}}while(0)
static std::vector<int>trace;
struct Ref{mutable int refs;Ref():refs(100){}};
template<class T>class CDefPointer{ public:T*p;CDefPointer(T*q=0):p(q){if(p)++p->refs;}CDefPointer(const CDefPointer&o):p(o.p){if(p)++p->refs;}~CDefPointer(){if(p)--p->refs;}CDefPointer&operator=(const CDefPointer&o){if(o.p)++o.p->refs;if(p)--p->refs;p=o.p;return*this;}T*operator->()const{return p;}};
class C3DVector{ public:float X,Y,Z;C3DVector(float x=0,float y=0,float z=0):X(x),Y(y),Z(z){}};
class CCharString{ public:const char*p;CCharString(const char*q,long n=-1):p(q){CHECK(n==-1);}const char*GetAsCharArray()const{return p;}unsigned long GetLength()const{return(unsigned long)std::strlen(p);}};
class CCRC{public:static unsigned long Calc(unsigned long seed,const void*p,unsigned long n){CHECK(seed==0&&n==std::strlen((const char*)p));if(std::strcmp((const char*)p,"LIGHTNING_SKY_BEAM_EFFECT")==0)return 1;CHECK(std::strcmp((const char*)p,"LIGHTNING_CREATOR_EFFECT")==0);return 2;}};
class CSpecialEffectsStringMap{ public:std::map<unsigned long,long>ContainmentMap;};
class CSpecialEffectsDef:public Ref{ public:CSpecialEffectsStringMap SpecialEffectsMap;bool GetEffectFromString(const CCharString&name,long&effect)const{CHECK(effect==0);unsigned long key=CCRC::Calc(0,name.GetAsCharArray(),name.GetLength());trace.push_back(key==1?20:40);std::map<unsigned long,long>::const_iterator it=SpecialEffectsMap.ContainmentMap.find(key);if(it!=SpecialEffectsMap.ContainmentMap.end()){effect=it->second;return true;}return false;}};
class CSpecialAbilitiesThunderLightningStormDef:public Ref{ public:CSpecialEffectsDef*effects;bool GetSubDef(CDefPointer<const CSpecialEffectsDef>*out)const{trace.push_back(10);*out=CDefPointer<const CSpecialEffectsDef>(effects);return true;}};
class CThing;
class CTCDParticleEmitter{ public:int attachments;CTCDParticleEmitter():attachments(0){}static CThing*Create(long,const C3DVector&,bool);void AttachToThing(CThing&,long,const CCharString&,long,float);};
enum ETCInterfaceType{TCI_ELECTROCUTE_ON_TOUCH=238};
class CThing{ public:mutable int refs;bool dead,touch,emitter_present;CTCDParticleEmitter*emitter;C3DVector pos;int additions;CThing():refs(0),dead(false),touch(false),emitter_present(false),emitter(0),additions(0){}bool IsDead()const{return dead;}const C3DVector&GetPos()const{return pos;}bool GetTC(CTCDParticleEmitter**p){trace.push_back(60);if(!emitter_present)return false;*p=emitter;return true;}bool HasTC(ETCInterfaceType t)const{CHECK(t==TCI_ELECTROCUTE_ON_TOUCH);trace.push_back(80);return touch;}void*AddTC(const CCharString&n,bool active,const void*params){CHECK(std::strcmp(n.p,"CTCElectrocuteOnTouch")==0&&!active&&!params);trace.push_back(90);++additions;touch=true;return 0;}};
template<class T>class CIntelligentPointer{ public:T*p;CIntelligentPointer(T*q=0):p(q){if(p)++p->refs;}CIntelligentPointer(const CIntelligentPointer&o):p(o.p){if(p)++p->refs;}~CIntelligentPointer(){if(p)--p->refs;}CIntelligentPointer&operator=(const CIntelligentPointer&o){if(o.p)++o.p->refs;if(p)--p->refs;p=o.p;return*this;}T*Get()const{return p;}};
template<class T>bool IS_THING_ALIVE(const T&p){return p!=0&&!p->IsDead();}
static CThing*parent_thing;static CThing*sky_result;static CThing*creator_result;static long sky_index,creator_index;static bool mutate_position;static int create_calls,touch_callback;static C3DVector initial_pos;
CThing*CTCDParticleEmitter::Create(long effect,const C3DVector&pos,bool attached){CHECK(!attached);bool sky=trace.back()==20;CHECK(effect==(sky?sky_index:creator_index));if(sky){CHECK(&pos!=&parent_thing->pos);CHECK(pos.X==initial_pos.X&&pos.Y==initial_pos.Y&&pos.Z==initial_pos.Z);trace.push_back(30);if(mutate_position)parent_thing->pos=C3DVector(8.0f,16.0f,32.0f);if(touch_callback==1)parent_thing->touch=true;}else{CHECK(&pos==&parent_thing->pos);CHECK(pos.X==parent_thing->pos.X&&pos.Y==parent_thing->pos.Y&&pos.Z==parent_thing->pos.Z);trace.push_back(50);if(touch_callback==2)parent_thing->touch=true;}++create_calls;return sky?sky_result:creator_result;}
void CTCDParticleEmitter::AttachToThing(CThing&thing,long bone,const CCharString&name,long flags,float scale){CHECK(&thing==parent_thing&&bone==12&&name.p[0]==0&&flags==2&&scale==0);trace.push_back(70);++attachments;if(touch_callback==3)thing.touch=true;}
namespace NCreatureMode{struct CModeThunderLightningStorm{CThing&parent;CDefPointer<const CSpecialAbilitiesThunderLightningStormDef>PLightningStormDef;CIntelligentPointer<CThing>PSkyBeam,PCreatorEffect;CModeThunderLightningStorm(CThing&p,CSpecialAbilitiesThunderLightningStormDef&def):parent(p),PLightningStormDef(&def){}CThing&GetParentThing(){return parent;}void CreateSkyBeam();};}

void NCreatureMode::CModeThunderLightningStorm::CreateSkyBeam()
{
 CDefPointer<const CSpecialEffectsDef> pspecial_fx_def;
 long effect_index=0;
 PLightningStormDef->GetSubDef(&pspecial_fx_def);
 if(pspecial_fx_def->GetEffectFromString("LIGHTNING_SKY_BEAM_EFFECT",effect_index)) {
  C3DVector weapon_pos=GetParentThing().GetPos();
  CThing* pEmitterThing=CTCDParticleEmitter::Create(effect_index,weapon_pos,false);
  if(IS_THING_ALIVE(pEmitterThing))PSkyBeam=CIntelligentPointer<CThing>(pEmitterThing);
 }
 effect_index=0;
 if(pspecial_fx_def->GetEffectFromString("LIGHTNING_CREATOR_EFFECT",effect_index)) {
  CThing* pTargetEmitterThing=CTCDParticleEmitter::Create(effect_index,GetParentThing().GetPos(),false);
  if(IS_THING_ALIVE(pTargetEmitterThing)) {
   PCreatorEffect=CIntelligentPointer<CThing>(pTargetEmitterThing);
   CTCDParticleEmitter* ptcEmitter=0;
   pTargetEmitterThing->GetTC(&ptcEmitter);
   if(ptcEmitter)ptcEmitter->AttachToThing(GetParentThing(),12,"",2,0.0f);
  }
 }
 if(!GetParentThing().HasTC(TCI_ELECTROCUTE_ON_TOUCH))GetParentThing().AddTC("CTCElectrocuteOnTouch",false,0);
}
int main(){
 const long indices[3]={-7,0,37};
 for(int found=0;found<4;++found)for(int sk=0;sk<4;++sk)for(int cr=0;cr<4;++cr)for(int emitter_state=0;emitter_state<3;++emitter_state)for(int initial=0;initial<4;++initial)for(int touch=0;touch<2;++touch)for(int change=0;change<2;++change)for(int callback=0;callback<4;++callback)for(int si=0;si<3;++si)for(int ci=0;ci<3;++ci){
  ++cases;trace.clear();CThing parent,sky_new,creator_new,old_sky,old_creator;CTCDParticleEmitter emitter;CSpecialEffectsDef effects;CSpecialAbilitiesThunderLightningStormDef storm;storm.effects=&effects;sky_index=indices[si];creator_index=indices[ci];if(found&1)effects.SpecialEffectsMap.ContainmentMap[1]=sky_index;if(found&2)effects.SpecialEffectsMap.ContainmentMap[2]=creator_index;
  parent_thing=&parent;parent.touch=touch!=0;initial_pos=C3DVector(-1.5f,2.25f,4.0f);parent.pos=initial_pos;mutate_position=change!=0;touch_callback=callback;create_calls=0;
  sky_new.dead=sk==1;creator_new.dead=cr==1;sky_result=sk==0?0:sk==3?&old_sky:&sky_new;creator_result=cr==0?0:cr==3?&old_creator:&creator_new;if(creator_result){creator_result->emitter_present=emitter_state!=0;creator_result->emitter=emitter_state==2?&emitter:0;}
  NCreatureMode::CModeThunderLightningStorm mode(parent,storm);mode.PSkyBeam=CIntelligentPointer<CThing>((initial&1)?&old_sky:0);mode.PCreatorEffect=CIntelligentPointer<CThing>((initial&2)?&old_creator:0);
  CThing*expected_sky=(found&1)&&sk>=2?sky_result:mode.PSkyBeam.Get();CThing*expected_creator=(found&2)&&cr>=2?creator_result:mode.PCreatorEffect.Get();bool attach=(found&2)&&cr>=2&&emitter_state==2;bool expected_touch=touch||((found&1)&&callback==1)||((found&2)&&callback==2)||(attach&&callback==3);
  std::vector<int>wanted;wanted.push_back(10);wanted.push_back(20);if(found&1)wanted.push_back(30);wanted.push_back(40);if(found&2){wanted.push_back(50);if(cr>=2){wanted.push_back(60);if(attach)wanted.push_back(70);}}wanted.push_back(80);if(!expected_touch)wanted.push_back(90);
  mode.CreateSkyBeam();CHECK(trace==wanted);CHECK(mode.PSkyBeam.Get()==expected_sky&&mode.PCreatorEffect.Get()==expected_creator);CHECK(parent.additions==(!expected_touch?1:0)&&parent.touch);CHECK(emitter.attachments==(attach?1:0));CHECK(create_calls==((found&1)?1:0)+((found&2)?1:0));CHECK(effects.refs==100&&storm.refs==101);
  CThing*all[4]={&sky_new,&creator_new,&old_sky,&old_creator};for(int k=0;k<4;++k)CHECK(all[k]->refs==(expected_sky==all[k]?1:0)+(expected_creator==all[k]?1:0));CHECK(parent.pos.X==(((found&1)&&change)?8.0f:initial_pos.X));
 }
 CHECK(cases==110592);std::puts("008daa80_TEST PASS");return 0;
}