#include <cstdio>
#include <cstdlib>
#include <cmath>
static void check(bool b){if(!b)std::abort();}
enum EAIActionReturn{AI_ACTION_NULL=0,AI_ACTION_TERMINATE_PROCESS=1,AI_ACTION_FINISHED=2,AI_ACTION_FAILED=3,AI_ACTION_IN_PROGRESS=4};
enum EAIActionRepeat{ACTION_REPEAT_CALLBACK=2};enum EMovementType{AI_MOVEMENT_WALKING=3};
class C3DVector{ public:float X,Y,Z;C3DVector(float x=0,float y=0,float z=0):X(x),Y(y),Z(z){}C3DVector operator+(const C3DVector& b)const{return C3DVector(X+b.X,Y+b.Y,Z+b.Z);}};
class CRightHandedSet{ public:C3DVector forward;const C3DVector& GetForward()const{return forward;}};
class CThing{ public:bool dead;CThing():dead(false){}bool IsDead()const{return dead;}};
class CThingBuilding:public CThing{ public:};class CThingObject:public CThing{ public:};
class CThingAICreature:public CThing{ public:C3DVector pos;const C3DVector& PeekPos()const{return pos;}};
template<class T>class CIntelligentPointer{ public:T* ptr;CIntelligentPointer(T* p=0):ptr(p){}T* Get()const{return ptr;}T* operator->()const{return ptr;}};
template<class T>bool operator!=(const CIntelligentPointer<T>& p,int){return p.Get()!=0;}
template<class T>bool IS_THING_ALIVE(const T& t){if(t!=0){if(!t->IsDead())return true;}return false;}
struct CAIStateGroup_TavernServer{
 CIntelligentPointer<CThingBuilding> PPub;CIntelligentPointer<CThingObject>PBar;CThingAICreature* creature;long ServingActionPointIndex;
 bool valid;int moveCalls,lookCalls,moveRetries,lookRetries,validCalls;EAIActionReturn moveFinal,lookFinal;
 C3DVector expectedMove,expectedLook;
 CThingAICreature& GetCreature()const{return *creature;}
 bool IsServingPointValid(){++validCalls;return valid;}
 EAIActionReturn AIAction_MoveToPos(EAIActionRepeat repeat,C3DVector p,bool& calculated,EMovementType movement,bool a,bool b,const CThing* avoid,bool c,float speed){
  check(repeat==ACTION_REPEAT_CALLBACK&&movement==AI_MOVEMENT_WALKING&&a&&b&&!avoid&&c&&speed==0);
  check(p.X==expectedMove.X&&p.Y==expectedMove.Y&&p.Z==expectedMove.Z);calculated=true;
  ++moveCalls;return moveCalls<=moveRetries?AI_ACTION_IN_PROGRESS:moveFinal;
 }
 EAIActionReturn AIAction_LookAtPos(EAIActionRepeat repeat,C3DVector p,long flags,float tolerance){
  check(repeat==ACTION_REPEAT_CALLBACK&&flags==17&&tolerance==0.01f);
  check(p.X==expectedLook.X&&p.Y==expectedLook.Y&&p.Z==expectedLook.Z);
  ++lookCalls;return lookCalls<=lookRetries?AI_ACTION_IN_PROGRESS:lookFinal;
 }
 bool WalkToPoint(const C3DVector&,const CRightHandedSet&);
};
bool CAIStateGroup_TavernServer::WalkToPoint(const C3DVector& pos,const CRightHandedSet& rhset)
{
 EAIActionReturn ret=AI_ACTION_IN_PROGRESS;
 do {
  if(!(IS_THING_ALIVE(PPub)&&IS_THING_ALIVE(PBar)))return false;
  if(ret==AI_ACTION_TERMINATE_PROCESS)return false;
  if(ServingActionPointIndex!=-1&&!IsServingPointValid())return false;
  bool nav_route_calculated;
  ret=AIAction_MoveToPos(ACTION_REPEAT_CALLBACK,pos,nav_route_calculated,AI_MOVEMENT_WALKING,true,true,0,true,0.0f);
 }while(ret==AI_ACTION_IN_PROGRESS);
 if(ret!=AI_ACTION_FINISHED)return false;
 C3DVector required_facing_dir=rhset.GetForward();
 C3DVector pos_to_look_at=GetCreature().PeekPos()+required_facing_dir;
 ret=AI_ACTION_FAILED;
 do {
  if(!(IS_THING_ALIVE(PPub)&&IS_THING_ALIVE(PBar)))return false;
  if(ServingActionPointIndex!=-1&&!IsServingPointValid())return false;
  ret=AIAction_LookAtPos(ACTION_REPEAT_CALLBACK,pos_to_look_at,0x11,0.01f);
 }while(ret==AI_ACTION_IN_PROGRESS);
 if(ret!=AI_ACTION_FINISHED)return false;
 return true;
}

int main(){
 CThingBuilding pub;CThingObject bar;CThingAICreature creature;CAIStateGroup_TavernServer state;state.creature=&creature;
 creature.pos=C3DVector(10,-20,30);C3DVector pos(2,4,8);CRightHandedSet rhset;rhset.forward=C3DVector(0.5f,-0.25f,1.5f);
 state.expectedMove=pos;state.expectedLook=creature.pos+rhset.forward;
 int cases=0;
 for(int bits=0;bits<64;++bits)for(int move=0;move<4;++move)for(int look=0;look<4;++look){
  state.PPub.ptr=(bits&1)?&pub:0;state.PBar.ptr=(bits&2)?&bar:0;pub.dead=(bits&4)!=0;bar.dead=(bits&8)!=0;
  state.ServingActionPointIndex=(bits&16)?3:-1;state.valid=(bits&32)!=0;
  state.moveCalls=state.lookCalls=state.validCalls=0;state.moveRetries=(bits>>4)%3;state.lookRetries=(bits>>2)%3;
  state.moveFinal=EAIActionReturn(move);state.lookFinal=EAIActionReturn(look);
  bool allowed=(bits&1)&&(bits&2)&&!(bits&4)&&!(bits&8)&&(!(bits&16)||(bits&32));
  bool expected=allowed&&move==2&&look==2;
  bool got=state.WalkToPoint(pos,rhset);check(got==expected);
  check(state.moveCalls==(allowed?state.moveRetries+1:0));
  check(state.lookCalls==((allowed&&move==2)?state.lookRetries+1:0));
  if(allowed)check(state.validCalls==((bits&16)?state.moveCalls+state.lookCalls:0));
  check(pos.X==2&&pos.Y==4&&pos.Z==8);++cases;
 }
 check(cases==1024);std::puts("009677a0_TEST PASS");return 0;
}