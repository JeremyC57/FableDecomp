#include <cstdio>
#include <cstdlib>
#include <string>
static void check(bool value){if(!value)std::abort();}
enum EAIActionReturn{AI_ACTION_NULL=0,AI_ACTION_TERMINATE_PROCESS=1,AI_ACTION_FINISHED=2,AI_ACTION_FAILED=3,AI_ACTION_IN_PROGRESS=4};enum EAIActionRepeat{ACTION_REPEAT_CALLBACK=2};enum EMovementType{AI_MOVEMENT_WALKING=3};
struct Fixture{int flags,retries,moveResult,useResult,failAt,checks,moves,uses,villages,copies,complete,terminated,constructed,destroyed;std::string trace;}fixture;
class C3DVector{ public:float X,Y,Z;C3DVector(float x=0,float y=0,float z=0):X(x),Y(y),Z(z){}};
class CRightHandedSet{ public:C3DVector a,b;};class CCharString{ public:};const CCharString ACTION_POINT_01;
struct Graphic;class CThing{ public:Graphic* GetTCGraphicAppearance();};class CThingCreatureBase:public CThing{ public:}creature;
struct Graphic{bool GetDummyObject(const CCharString& name,long index,unsigned long flags,C3DVector& pos,CRightHandedSet& rhs,float scale)const{
 check(&name==&ACTION_POINT_01&&index==0&&flags==0&&scale==1&&pos.X==0&&pos.Y==0&&pos.Z==0);fixture.trace+='D';
 if(!(fixture.flags&64))pos=C3DVector(2,4,8);rhs.a=C3DVector(0,0,1);rhs.b=C3DVector(0,1,0);return !(fixture.flags&64);
}}graphic;
Graphic* CThing::GetTCGraphicAppearance(){return &graphic;}struct Torch:CThing{}torchObject;
class CVillageTaskTorch{ public:CThing* torch;CThing* GetTorch()const{fixture.trace+='H';return torch;}
 bool IsCreatureAssignedToTask(const CThing& c)const{check(&c==&creature);fixture.trace+='C';return fixture.checks++!=fixture.failAt;}
}task;
template<class T>class CIntelligentPointer{ public:T* ptr;CIntelligentPointer(T* p=0):ptr(p){}
 template<class U>CIntelligentPointer(CIntelligentPointer<U>& p):ptr(p.ptr){fixture.trace+='K';++fixture.copies;}
 T* operator->()const{return ptr;}T& operator*()const{return *ptr;}};
template<class T>bool operator==(const CIntelligentPointer<T>& p,int){return p.ptr==0;}
template<class T>bool operator!=(const CIntelligentPointer<T>& p,int){return p.ptr!=0;}
class CTCVillage{ public:
 bool SetTorchTaskAsAcceptedByThing(CVillageTaskTorch& t,const CThing& c){check(&t==&task&&&c==&creature);fixture.trace+='A';return (fixture.flags&2)!=0;}
 void SetTorchTaskAsCompleted(CIntelligentPointer<CVillageTaskTorch> p){check(p.ptr==&task);fixture.trace+='F';++fixture.complete;}
}village;
class CTCVillageMember{ public:CTCVillage* GetPVillage()const{fixture.trace+='G';++fixture.villages;return fixture.villages==1||(fixture.flags&8)?&village:0;}}villageMember;
class CCreatureActionBase{ public:virtual ~CCreatureActionBase(){}};
class CCreatureAction_UseObject:public CCreatureActionBase{ public:
 CCreatureAction_UseObject(CThingCreatureBase& c,CThing& t,bool allow){check(&c==&creature&&&t==&torchObject&&allow);fixture.trace+='U';++fixture.constructed;}
 ~CCreatureAction_UseObject(){fixture.trace+='V';++fixture.destroyed;}
};
struct CAIStateGroup_VillageTaskTorch{
 CTCVillageMember*PTCVillageMember;CIntelligentPointer<CVillageTaskTorch>PTask;
 CThingCreatureBase& GetCreature()const{return creature;}
 void Terminate(){fixture.trace+='T';++fixture.terminated;}
 EAIActionReturn AIAction_MoveToPos(EAIActionRepeat repeat,C3DVector pos,bool& nav,EMovementType movement,bool a,bool b,const CThing* avoid,bool c,float speed){
  check(repeat==2&&movement==3&&a&&b&&!avoid&&c&&speed==0&&!nav);float x=(fixture.flags&64)?0.0f:2.0f;
  check(pos.X==x&&pos.Y==x*2&&pos.Z==x*4);nav=true;fixture.trace+='M';++fixture.moves;
  if(fixture.flags&32)PTask.ptr=0;
  return EAIActionReturn(fixture.moves<=fixture.retries?4:fixture.moveResult);
 }
 EAIActionReturn AIAction_PerformActionAndWait(const CCreatureActionBase&,bool wait){check(!wait);fixture.trace+='W';++fixture.uses;if(fixture.flags&16)PTask.ptr=0;return EAIActionReturn(fixture.useResult);}
 void Update();
};
void CAIStateGroup_VillageTaskTorch::Update()
{
 if(PTask==0) { Terminate(); return; }
 CTCVillage* pvillage=PTCVillageMember->GetPVillage();
 if(pvillage->SetTorchTaskAsAcceptedByThing(*PTask,GetCreature())) {
  CThing* torch=PTask->GetTorch();
  if(!torch) { Terminate(); return; }
  C3DVector action_pos(0.0f,0.0f,0.0f);
  CRightHandedSet rhset;
  torch->GetTCGraphicAppearance()->GetDummyObject(ACTION_POINT_01,0,0,action_pos,rhset,1.0f);
  EAIActionReturn ret=AI_ACTION_FAILED;
  do {
   if(PTask==0 || !PTask->IsCreatureAssignedToTask(GetCreature())) { Terminate(); return; }
   bool nav_route_calculated=false;
   ret=AIAction_MoveToPos(ACTION_REPEAT_CALLBACK,action_pos,nav_route_calculated,AI_MOVEMENT_WALKING,true,true,0,true,0.0f);
  }while(ret==AI_ACTION_IN_PROGRESS);
  if(ret!=AI_ACTION_FINISHED || PTask==0 || !PTask->IsCreatureAssignedToTask(GetCreature())) { Terminate(); return; }
  if(ret!=AI_ACTION_FINISHED || PTask==0 || !PTask->IsCreatureAssignedToTask(GetCreature())) { Terminate(); return; }
  if(AIAction_PerformActionAndWait(CCreatureAction_UseObject(GetCreature(),*torch,true),false)==AI_ACTION_FINISHED) {
   pvillage=PTCVillageMember->GetPVillage();
   if(pvillage && PTask!=0) {
#ifdef _MSC_VER
    // Select the original mutable same-type constructor template on VC71.
    pvillage->SetTorchTaskAsCompleted(CIntelligentPointer<CVillageTaskTorch>::CIntelligentPointer<CVillageTaskTorch>(PTask));
#else
    pvillage->SetTorchTaskAsCompleted(CIntelligentPointer<CVillageTaskTorch>(PTask));
#endif
   }
  }
 }
 Terminate();
}

int main(){
 const int returns[]={2,3,1,0};int cases=0;
 for(int flags=0;flags<128;++flags)for(int retries=0;retries<3;++retries)for(int move=0;move<4;++move)for(int use=0;use<4;++use)for(int fail=0;fail<6;++fail){
  fixture.flags=flags;fixture.retries=retries;fixture.moveResult=returns[move];fixture.useResult=returns[use];fixture.failAt=fail;
  fixture.checks=fixture.moves=fixture.uses=fixture.villages=fixture.copies=fixture.complete=fixture.terminated=fixture.constructed=fixture.destroyed=0;fixture.trace="";
  task.torch=(flags&4)?static_cast<CThing*>(&torchObject):0;CAIStateGroup_VillageTaskTorch state;state.PTCVillageMember=&villageMember;state.PTask.ptr=(flags&1)?&task:0;
  std::string expected;int checks=0,expectedMoves=0,expectedUses=0,expectedComplete=0;bool active=(flags&1)!=0,success=false;
  if(active){expected+="GA";if(flags&2){expected+='H';if(flags&4){expected+='D';
   bool done=false;
   while(!done){
    if(!active)break;expected+='C';if(checks++==fail)break;
    expected+='M';++expectedMoves;if(flags&32)active=false;
    if(expectedMoves<=retries)continue;
    if(move!=0||!active)break;
    expected+='C';if(checks++==fail)break;
    expected+='C';if(checks++==fail)break;
    expected+="UWV";++expectedUses;if(flags&16)active=false;
    if(use==0){expected+='G';if((flags&8)&&active){expected+="KF";++expectedComplete;}}
    done=true;
   }
  }}}
  expected+='T';state.Update();
  check(fixture.trace==expected&&fixture.moves==expectedMoves&&fixture.uses==expectedUses&&fixture.complete==expectedComplete);
  check(fixture.copies==expectedComplete&&fixture.terminated==1&&fixture.constructed==expectedUses&&fixture.destroyed==expectedUses);++cases;
 }
 check(cases==36864);std::puts("0093ea90_TEST PASS");return 0;
}