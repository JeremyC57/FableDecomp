#include <cstdio>
#include <cstdlib>
#include <cstddef>
enum ETCInterfaceType {TCI_PRE_CALCULATED_NAVIGATION_ROUTE=166};
enum EScriptEntityMoveType {ENTITY_MOVE_WALK,ENTITY_MOVE_RUN,ENTITY_MOVE_SNEAK};
enum EMovementType {AI_MOVEMENT_SNEAKING=1,AI_MOVEMENT_WALKING=3,AI_MOVEMENT_RUNNING=5};
class CThing;
class CTCScriptedControl {public:class CActionBase {public:virtual ~CActionBase(){}};CActionBase* queued;int calls;CTCScriptedControl():queued(0),calls(0){}void AddAction(CActionBase* p){queued=p;++calls;}};
class CThing {public:bool alive,has_route;CTCScriptedControl* control;mutable int lookups,route_checks;CThing():alive(true),has_route(false),control(0),lookups(0),route_checks(0){}bool IsAlive()const{return alive;}bool HasTC(ETCInterfaceType t)const{++route_checks;return t==TCI_PRE_CALCULATED_NAVIGATION_ROUTE&&has_route;}bool GetTC(CTCScriptedControl** out){++lookups;*out=control;return control!=0;}};
template<class T> class CIntelligentPointer {public:T* p;mutable int reads;CIntelligentPointer():p(0),reads(0){}bool operator!=(int)const{++reads;return p!=0;}T* operator->()const{++reads;return p;}};
template<class T> inline bool IS_THING_ALIVE(const T& p){return p!=0&&p->IsAlive();}
class CScriptThing {public:bool alive;CThing* p;mutable int alive_calls,get_calls;CScriptThing():alive(false),p(0),alive_calls(0),get_calls(0){}virtual bool IsAlive()const{++alive_calls;return alive;}virtual CThing* GetPThing()const{++get_calls;return p;}};
static bool fail_allocation;static int allocations,constructions;
namespace NTCScriptedControl {class CActionMoveTo:public CTCScriptedControl::CActionBase {public:const CThing* route;EMovementType movement;bool face,avoid;
 static void* operator new(std::size_t n)throw(){++allocations;return fail_allocation?0:std::malloc(n);}static void operator delete(void* p)throw(){std::free(p);}
 CActionMoveTo(const CThing& r,EMovementType m,bool f,bool a):route(&r),movement(m),face(f),avoid(a){++constructions;}
};}
class CScriptGameResourceObjectScriptedThing {public:CIntelligentPointer<CThing> PThing;int waits;CScriptGameResourceObjectScriptedThing():waits(0){}virtual void WaitWhilePerformingTasks(){++waits;}virtual void FollowPreCalculatedRoute(const CScriptThing&,EScriptEntityMoveType,bool,bool);};
inline EMovementType GFConvertScriptMoveTypeToAIMoveType(EScriptEntityMoveType move_type)
{
 switch(move_type){
 case ENTITY_MOVE_WALK:return AI_MOVEMENT_WALKING;
 case ENTITY_MOVE_RUN:return AI_MOVEMENT_RUNNING;
 default:return AI_MOVEMENT_SNEAKING;
 }
}
void CScriptGameResourceObjectScriptedThing::FollowPreCalculatedRoute(const CScriptThing& route,EScriptEntityMoveType move_type,bool wait,bool avoid_dynamic_obstacles)
{
 if(IS_THING_ALIVE(PThing)&&route.IsAlive()){
  const CThing* proute=route.GetPThing();
  const bool& is_route=proute->HasTC(TCI_PRE_CALCULATED_NAVIGATION_ROUTE);
  if(is_route){
   EMovementType ai_move_type=GFConvertScriptMoveTypeToAIMoveType(move_type);
   CTCScriptedControl* ptc_script_control;
   PThing->GetTC(&ptc_script_control);
   ptc_script_control->AddAction(new NTCScriptedControl::CActionMoveTo(*proute,ai_move_type,true,avoid_dynamic_obstacles));
   if(wait)WaitWhilePerformingTasks();
  }
 }
}

int main(){int ok=1,cases=0;
for(int state=0;state<3;++state)for(int route_alive=0;route_alive<2;++route_alive)for(int route_present=0;route_present<2;++route_present)for(int movement=0;movement<3;++movement)for(int wait=0;wait<2;++wait)for(int avoid=0;avoid<2;++avoid)for(int fail=0;fail<2;++fail){
 CThing owner,route_thing;CTCScriptedControl control;owner.control=&control;owner.alive=state==2;route_thing.has_route=route_present!=0;
 CScriptThing route;route.alive=route_alive!=0;route.p=&route_thing;CScriptGameResourceObjectScriptedThing object;object.PThing.p=state?&owner:0;
 fail_allocation=fail!=0;int old_allocations=allocations,old_constructions=constructions;
 object.FollowPreCalculatedRoute(route,(EScriptEntityMoveType)movement,wait!=0,avoid!=0);
 bool active=state==2&&route_alive&&route_present;int expected_reads=state==0?1:(active?3:2);
 ok &= object.PThing.reads==expected_reads&&route.alive_calls==(state==2)&&route.get_calls==(state==2&&route_alive);
 ok &= route_thing.route_checks==(state==2&&route_alive)&&owner.lookups==active;
 ok &= control.calls==active&&object.waits==(active&&wait)&&allocations==old_allocations+active&&constructions==old_constructions+(active&&!fail);
 if(active&&!fail){NTCScriptedControl::CActionMoveTo* action=static_cast<NTCScriptedControl::CActionMoveTo*>(control.queued);int mapped=movement==0?3:movement==1?5:1;ok &= action&&action->route==&route_thing&&action->movement==mapped&&action->face&&action->avoid==(avoid!=0);}
 else ok &= control.queued==0;
 delete control.queued;++cases;
}
ok &= cases==288;std::printf(ok?"00903f30_TEST PASS\n":"00903f30_TEST FAIL\n");return ok?0:1;}
