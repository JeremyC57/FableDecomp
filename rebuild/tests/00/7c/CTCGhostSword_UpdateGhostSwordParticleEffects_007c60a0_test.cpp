#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <setjmp.h>
static jmp_buf stop;static bool missing;
void diagnostic();
#define FABLE_RETAIL_ASSERT(c) ((void)((c)||(diagnostic(),0)))
static int diagnostics,inserts,pushes,updates,effects,componentQueries,lookups,keyCalls;
static unsigned config;
void diagnostic(){++diagnostics;if(missing)longjmp(stop,1);}
struct C3DVector {int x;C3DVector(int v=0):x(v){}};
struct CRightHandedSet {};
namespace std {template<class A,class B>struct pair {A first;B second;pair(const A& a,const B& b):first(a),second(b){}};}
namespace NParticleEngine {
unsigned long MAKE_VECTOR_PARAM_FROM_BASE(unsigned long v,const char* s){assert(v==0);++keyCalls;return !strcmp(s,"EMITTER_BASE")?101:!strcmp(s,"EMITTER_TIP")?202:0;}
struct Map {int size,as,bs;Map():size(0),as(0),bs(0){}
void Insert(const std::pair<unsigned long,C3DVector>& v){assert(size++==0&&v.first==101);as=v.second.x;++inserts;}
void push_back(const std::pair<unsigned long,C3DVector>& v){assert(size++==1&&v.first==202);bs=v.second.x;++pushes;}};
template<class A,class B>struct CParticleUpdateParameters{Map VectorParams;void Update(){assert(VectorParams.size==2);++updates;}};
}
namespace NGraphicAppearance {struct CTCGraphicAppearance{int id;
bool GetDummyObject(const char* key,long n,unsigned long flags,C3DVector& pos,CRightHandedSet&,float scale){
assert(pos.x==id&&!n&&!flags&&scale==1);bool start=!strcmp(key,"EMITTER_BASE");assert(start||!strcmp(key,"EMITTER_TIP"));++lookups;
bool ok=!!(config&(start?4:8));if(ok)pos.x+=start?100:200;return ok;}
};}
struct CTCDParticleEmitter{int id;void SetEmitterUpdateParameters(const NParticleEngine::CParticleUpdateParameters<unsigned long,unsigned long>& p){
assert(p.VectorParams.as==id+((config&6)==6?100:0));assert(p.VectorParams.bs==id+((config&10)==10?200:0));++effects;}};
struct Creature {bool alive;int id;NGraphicAppearance::CTCGraphicAppearance app;C3DVector GetPos(){return id;}NGraphicAppearance::CTCGraphicAppearance* GetTCGraphicAppearance(){return config&2?&app:0;}};
struct EmitterThing {bool alive;CTCDParticleEmitter component;bool GetTC(CTCDParticleEmitter** p){++componentQueries;missing=!!(config&16);*p=missing?0:&component;return !missing;}};
template<class T>struct Array{int n;T* items[3];unsigned size()const{return (unsigned)n;}T* operator[](long i){assert(i>=0&&i<n);return items[i];}};
#define IS_THING_ALIVE(p) ((p)&&(p)->alive)
struct CTCGhostSword {Array<Creature> Minions;Array<EmitterThing> MinionEffects;void UpdateGhostSwordParticleEffects();};
void CTCGhostSword::UpdateGhostSwordParticleEffects()
{
 for(long i=0;i<Minions.size();++i) {
  if(IS_THING_ALIVE(Minions[i])) {
   C3DVector startpos=Minions[i]->GetPos();
   NGraphicAppearance::CTCGraphicAppearance* ptc_graphic_appearance_creature=Minions[i]->GetTCGraphicAppearance();
   if(ptc_graphic_appearance_creature) {
    CRightHandedSet rhs;
    if (!ptc_graphic_appearance_creature->GetDummyObject("EMITTER_BASE",0,0,startpos,rhs,1.0f)) { FABLE_RETAIL_ASSERT(!"Ghost sword spline 1"); }
   }
   std::pair<unsigned long,C3DVector> vtStartPos(NParticleEngine::MAKE_VECTOR_PARAM_FROM_BASE(0,"EMITTER_BASE"),startpos);
   C3DVector endpos=Minions[i]->GetPos();
   if(ptc_graphic_appearance_creature) {
    CRightHandedSet rhs;
    if (!ptc_graphic_appearance_creature->GetDummyObject("EMITTER_TIP",0,0,endpos,rhs,1.0f)) { FABLE_RETAIL_ASSERT(!"Ghost Sword spline 2"); }
   }
   std::pair<unsigned long,C3DVector> vtEndPos(NParticleEngine::MAKE_VECTOR_PARAM_FROM_BASE(0,"EMITTER_TIP"),endpos);
   NParticleEngine::CParticleUpdateParameters<unsigned long,unsigned long> upParams;
   upParams.VectorParams.Insert(vtStartPos);
   upParams.VectorParams.push_back(vtEndPos);
   upParams.Update();
   if(IS_THING_ALIVE(MinionEffects[i])) {
    CTCDParticleEmitter* ptcdEmitter=0;
    if(!MinionEffects[i]->GetTC(&ptcdEmitter)) { FABLE_RETAIL_ASSERT(!"MinionEffects[i]->GetTC(&ptcd_" "emitter)"); }
    ptcdEmitter->SetEmitterUpdateParameters(upParams);
   }
  }
 }
}
int main(){
 for(config=0;config<16;++config)for(int length=0;length<=3;++length)for(int pattern=0;pattern<729;++pattern) {
  CTCGhostSword tc;tc.Minions.n=tc.MinionEffects.n=length;
  Creature c[3];EmitterThing e[3];int live=0,emit=0,num=pattern;
  for(int i=0;i<length;++i){int a=num%3;num/=3;int b=num%3;num/=3;
   c[i].alive=(a==2);c[i].id=c[i].app.id=10+i;e[i].component.id=10+i;e[i].alive=(b==2);
   tc.Minions.items[i]=a?c+i:0;tc.MinionEffects.items[i]=b?e+i:0;live+=a==2;emit+=(a==2&&b==2);}
  diagnostics=inserts=pushes=updates=effects=componentQueries=lookups=keyCalls=0;tc.UpdateGhostSwordParticleEffects();
  assert(inserts==live&&pushes==live&&updates==live&&keyCalls==2*live&&effects==emit&&componentQueries==emit);
  assert(lookups==((config&2)?2*live:0));assert(diagnostics==((config&2)?live*((!(config&4))+(!(config&8))):0));
 }
 CTCGhostSword tc;Creature c;EmitterThing e;tc.Minions.n=tc.MinionEffects.n=1;tc.Minions.items[0]=&c;tc.MinionEffects.items[0]=&e;
 c.id=c.app.id=e.component.id=4;c.alive=e.alive=true;config=16;diagnostics=effects=componentQueries=inserts=pushes=updates=0;
 if(setjmp(stop)==0){tc.UpdateGhostSwordParticleEffects();assert(false);}
 assert(diagnostics==1&&componentQueries==1&&effects==0&&inserts==1&&pushes==1&&updates==1);
 puts("007c60a0_TEST PASS");return 0;
}