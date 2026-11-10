#include <cstdio>
#include <cstdlib>
static void check(bool b){if(!b)std::abort();}
static int target_reads,def_calls,deletes;
enum EMessageEventType{MESSAGE_EVENT_OTHER=8,MESSAGE_EVENT_DEATH=9,MESSAGE_EVENT_LATER=10};
class CThingCreatureDef { public:
 mutable unsigned long IVRefCount;long LeaderTypeDef;
 void AddRef()const{++IVRefCount;}
 long Release()const{if(--IVRefCount==0){++deletes;return 0;}return IVRefCount;}
};
template<class T>class CDefPointer{ public:
 T* Object;CDefPointer():Object(0){}~CDefPointer(){Release();}
 T* Get()const{return Object;}T* operator->()const{return Get();}
 void Release(){if(Object){Object->Release();Object=0;}}
};
class CGameDefinitionManager { public:
 bool success;const CThingCreatureDef* found;mutable long last_index;
 template<class T>bool GetDef(long index,CDefPointer<const T>* out)const{
  ++def_calls;last_index=index;
  if(index<=0||!success||!found)return false;
  out->Object=found;found->AddRef();return true;
 }
};
class CThing { public:
 unsigned short DefGlobalIndex;unsigned long ThingTypeFlags;
 unsigned long GetTypeFlags()const{return ThingTypeFlags;}
 static unsigned long StatGetTypeFlags(){return 0;}
 long GetDefGlobalIndex()const{return DefGlobalIndex;}
};
class CThingCreatureBase:public CThing{ public:
 const CGameDefinitionManager* manager;
 const CGameDefinitionManager& PeekDefinitionManager()const{return *manager;}
};
template<class T>class CIntelligentPointer{ public:
 T* p;bool operator!=(int n)const{check(n==0);return Get()!=0;}
 T* Get()const{++target_reads;return p;}T* operator->()const{return Get();}
};
template<class A,class B>B* GFDebugPDynamicCast(A* p,B*){return static_cast<B*>(p);}
class CMessageEvent{ public:
 EMessageEventType Type;CIntelligentPointer<CThing> PTarget;
 template<class T>T* GetPTarget()const{
  T* ret=0;
  if(PTarget!=0){
   unsigned long things_flags=PTarget->GetTypeFlags();
   unsigned long cast_to_flags=T::StatGetTypeFlags();
   unsigned long xored=things_flags^cast_to_flags;
   if(!(xored&cast_to_flags))ret=GFDebugPDynamicCast(PTarget.Get(),(T*)0);
  }
  return ret;
 }
};
class CMessageEventFilter_IsMyDeadLeader{ public:
 const CThingCreatureBase& Creature;
 CMessageEventFilter_IsMyDeadLeader(const CThingCreatureBase& c):Creature(c){}
 bool operator()(const CMessageEvent&)const;
};
bool CMessageEventFilter_IsMyDeadLeader::operator()(const CMessageEvent& test_event)const
{
 if(test_event.Type==MESSAGE_EVENT_DEATH){
  CThing* dead_thing=test_event.GetPTarget<CThing>();
  if(dead_thing){
   CDefPointer<const CThingCreatureDef> def;
   if(Creature.PeekDefinitionManager().GetDef(Creature.GetDefGlobalIndex(),&def)){
    if(def.Get()&&def->LeaderTypeDef==dead_thing->GetDefGlobalIndex())return true;
   }
  }
 }
 return false;
}

int main(){
 CThingCreatureBase creature;CThing target;CThingCreatureDef def;CGameDefinitionManager manager;CMessageEvent event;
 creature.manager=&manager;target.DefGlobalIndex=65535;target.ThingTypeFlags=0xffffffffUL;
 const unsigned short indices[3]={0,1,65535};const long leaders[3]={65535,1,-1};
 CMessageEventFilter_IsMyDeadLeader filter(creature);int cases=0;
 for(int type=8;type<=10;++type)for(int live=0;live<2;++live)for(int ix=0;ix<3;++ix)
 for(int lookup=0;lookup<3;++lookup)for(int leader=0;leader<3;++leader)for(int refs=0;refs<2;++refs){
  creature.DefGlobalIndex=indices[ix];def.LeaderTypeDef=leaders[leader];def.IVRefCount=refs;
  event.Type=EMessageEventType(type);event.PTarget.p=live?&target:0;
  manager.success=lookup!=0;manager.found=lookup==2?&def:0;manager.last_index=-1;
  target_reads=def_calls=deletes=0;
  bool reached=type==9&&live!=0;bool acquired=reached&&ix!=0&&lookup==2;
  bool expected=acquired&&leader==0;
  check(filter(event)==expected);
  check(target_reads==(type==9?(live?3:1):0));check(def_calls==(reached?1:0));
  check(manager.last_index==(reached?long(indices[ix]):-1));
  check(def.IVRefCount==unsigned(refs));check(deletes==((acquired&&refs==0)?1:0));
  ++cases;
 }
 check(cases==324);std::puts("006af3f0_TEST PASS");return 0;
}