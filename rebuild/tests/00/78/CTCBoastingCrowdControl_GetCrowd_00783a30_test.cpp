#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <map>
#include <string>
#ifdef _MSC_VER
typedef unsigned __int64 TestUID;
#else
typedef unsigned long TestUID;
#endif
static void check(bool value){if(!value)std::abort();}
static std::string trace;
class C3DVector{ public:float X,Y,Z;};
class CThing;class CTCCreatureGroupBoastingCrowd;
class CTCBase{ public:virtual ~CTCBase(){}};
class CCharString{ public:std::string text;CCharString(const char* s):text(s){}};
class CThing{ public:
 TestUID uid;bool dead,hasTC;C3DVector pos;CTCCreatureGroupBoastingCrowd* tc;
 CThing():uid(0),dead(false),hasTC(true),tc(0){pos.X=pos.Y=pos.Z=0;}
 TestUID GetUniqueID()const{return uid;}
 bool IsDead()const{return dead;}
 const C3DVector& GetPos()const{return pos;}
 bool GetTC(CTCCreatureGroupBoastingCrowd** out){trace+='G';if(hasTC){*out=tc;return true;}return false;}
 CTCBase* AddTC(const CCharString&,bool,const void*);
};
class CTCCreatureGroupBoastingCrowd:public CTCBase{ public:CThing* hero;int sets;CTCCreatureGroupBoastingCrowd():hero(0),sets(0){}void SetHero(CThing& h){trace+='H';hero=&h;++sets;}};
CTCBase* CThing::AddTC(const CCharString& name,bool b,const void* p){check(name.text=="CTCCreatureGroupBoastingCrowd"&&!b&&!p);trace+='A';return tc;}
template<class T>class CIntelligentPointer{ public:T* ptr;CIntelligentPointer(T* p=0):ptr(p){}T* Get()const{return ptr;}};
template<class T>bool IS_THING_ALIVE(T* p){return p!=0&&!p->IsDead();}
template<class A,class B>B GFDebugPDynamicCast(A p,B){return static_cast<B>(p);}
class CGameDefinitionManager{ public:long index;long GetDefGlobalIndexFromName(const CCharString& name)const{check(name.text=="INTERNAL_MARKER_LIGHT");trace+='D';return index;}};
class CWorld{ public:CGameDefinitionManager def;const CGameDefinitionManager& PeekDefinitionManager()const{return def;}};
static CThing* madeThing;static C3DVector expectedPos;static long expectedIndex;
class CTCDInternalMarker{ public:static CThing* Create(long d,const C3DVector& pos){check(d==expectedIndex&&pos.X==expectedPos.X&&pos.Y==expectedPos.Y&&pos.Z==expectedPos.Z);trace+='C';return madeThing;}};
class CTCBoastingCrowdControl{ public:
 std::map<TestUID,CIntelligentPointer<CThing> > HeroCrowdThings;
 CThing* parent;CWorld* world;
 CThing& GetParentThing(){return *parent;}
 const CWorld& PeekWorld()const{return *world;}
 bool GetCrowd(CThing&,CThing**,CTCCreatureGroupBoastingCrowd**,bool);
};
bool CTCBoastingCrowdControl::GetCrowd(CThing& hero_creature,CThing** ppcrowd_thing,CTCCreatureGroupBoastingCrowd** pptc_crowd,bool create_as_needed)
{
 std::map<TestUID,CIntelligentPointer<CThing> >::iterator crowd_it=HeroCrowdThings.find(hero_creature.GetUniqueID());
 if(crowd_it!=HeroCrowdThings.end()) {
  *ppcrowd_thing=crowd_it->second.Get();
  if(IS_THING_ALIVE(*ppcrowd_thing)) {
   (*ppcrowd_thing)->GetTC(pptc_crowd);
   return true;
  }
  HeroCrowdThings.erase(crowd_it);
 }
 if(create_as_needed) {
  long def_num=PeekWorld().PeekDefinitionManager().GetDefGlobalIndexFromName("INTERNAL_MARKER_LIGHT");
  *ppcrowd_thing=CTCDInternalMarker::Create(def_num,GetParentThing().GetPos());
  *pptc_crowd=GFDebugPDynamicCast((*ppcrowd_thing)->AddTC("CTCCreatureGroupBoastingCrowd",false,0),(CTCCreatureGroupBoastingCrowd*)0);
  (*pptc_crowd)->SetHero(hero_creature);
  HeroCrowdThings.insert(std::make_pair(hero_creature.GetUniqueID(),*ppcrowd_thing));
  return true;
 }
 return false;
}

int main(){
 int cases=0;
 for(int id=0;id<8;++id)for(int state=0;state<5;++state)for(int create=0;create<2;++create)for(int idx=0;idx<5;++idx)for(int origin=0;origin<4;++origin){
  CThing hero,otherHero,parent,oldThing,createdThing,sentinel;CTCCreatureGroupBoastingCrowd oldTC,newTC,sentinelTC;
  hero.uid=(TestUID(id+1)<<32)|TestUID(id*17+5);otherHero.uid=hero.uid+1;
  oldThing.tc=&oldTC;createdThing.tc=&newTC;oldThing.dead=state==3;oldThing.hasTC=state!=4;
  parent.pos.X=origin*0.5f;parent.pos.Y=-origin*0.25f;parent.pos.Z=origin*2.0f;
  CWorld world;world.def.index=idx*13-17;expectedIndex=world.def.index;expectedPos=parent.pos;madeThing=&createdThing;
  CTCBoastingCrowdControl control;control.parent=&parent;control.world=&world;
  control.HeroCrowdThings.insert(std::make_pair(otherHero.uid,CIntelligentPointer<CThing>(&sentinel)));
  if(state!=0)control.HeroCrowdThings.insert(std::make_pair(hero.uid,CIntelligentPointer<CThing>(state==1?0:&oldThing)));
  CThing* crowd=&sentinel;CTCCreatureGroupBoastingCrowd* component=&sentinelTC;trace="";
  bool result=control.GetCrowd(hero,&crowd,&component,create!=0);
  const bool alive=state==2||state==4;
  check(result==(alive||create!=0));
  check(control.HeroCrowdThings.find(otherHero.uid)->second.Get()==&sentinel);
  if(alive){check(trace=="G"&&crowd==&oldThing&&component==(state==4?&sentinelTC:&oldTC));check(control.HeroCrowdThings.size()==2&&newTC.sets==0);}
  else if(create){check(trace=="DCAH"&&crowd==&createdThing&&component==&newTC&&newTC.hero==&hero&&newTC.sets==1);check(control.HeroCrowdThings.size()==2&&control.HeroCrowdThings.find(hero.uid)->second.Get()==&createdThing);trace="";CThing* again=0;CTCCreatureGroupBoastingCrowd* againTC=0;check(control.GetCrowd(hero,&again,&againTC,true)&&again==&createdThing&&againTC==&newTC&&trace=="G"&&newTC.sets==1);}
  else{check(trace.empty()&&component==&sentinelTC);check(crowd==(state==0?&sentinel:state==1?0:&oldThing));check(control.HeroCrowdThings.size()==1&&control.HeroCrowdThings.find(hero.uid)==control.HeroCrowdThings.end());}
  ++cases;
 }
 check(cases==1600);std::puts("00783a30_TEST PASS");return 0;
}