#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <string>
static void check(bool b){if(!b)std::abort();}
enum EAIActionReturn{AI_ACTION_NULL=0,AI_ACTION_TERMINATE_PROCESS=1,AI_ACTION_FINISHED=2,AI_ACTION_FAILED=3,AI_ACTION_IN_PROGRESS=4};
enum EAIActionRepeat{ACTION_REPEAT_CALLBACK=2};enum EMovementType{AI_MOVEMENT_RUNNING=5};
struct Fixture{int flags,result,steps,iteration,conversions;bool navSeen;float radius;std::string trace;}fixture;
static bool Los(){return ((fixture.flags&16)!=0)^((fixture.flags&64)&&((fixture.iteration&1)!=0));}
static bool Outer(){return ((fixture.flags&32)!=0)^((fixture.flags&128)&&((fixture.iteration&1)!=0));}
class C3DVector{ public:float X,Y,Z;C3DVector(float x=0,float y=0,float z=0):X(x),Y(y),Z(z){}};
class CThing{ public:virtual ~CThing(){}};
class CTCCreatureNavigation{ public:bool IsPosNavLineOfSightTo(const C3DVector& p)const{check(p.X==2&&p.Y==4&&p.Z==8);fixture.trace+='L';return Los();}}navigation;
struct Physics{float GetDiameter()const{return (fixture.radius*2.0f);}}physics;
class CThingCreatureBase:public CThing{ public:
 bool IsInMeleeAsAttacker()const{fixture.trace+='R';return (fixture.flags&1)&&!(fixture.iteration&&(fixture.flags&4));}
 Physics* GetPhysicsTC(){return &physics;}
 void PeekTC(const CTCCreatureNavigation** p)const{fixture.trace+='N';*p=&navigation;}
}creature,enemy;
template<class T>class CIntelligentPointer{ public:
 T* ptr;CIntelligentPointer(T* p=0):ptr(p){}
 template<class U>CIntelligentPointer(CIntelligentPointer<U>& p):ptr(p.ptr){++fixture.conversions;}
};
class CCombatManager{ public:
 C3DVector GetPreferredPos(const CThingCreatureBase& c)const{check(&c==&creature);fixture.trace+='P';return C3DVector(2,4,8);}
 bool IsCreatureOnOuterRing(const CThingCreatureBase& c)const{check(&c==&creature);fixture.trace+='O';return Outer();}
}combat;
struct CAIStateGroup_MeleeCombatNearby{
 CIntelligentPointer<CThingCreatureBase>PEnemy;
 CAIStateGroup_MeleeCombatNearby():PEnemy(&enemy){}
 CThingCreatureBase& GetCreature()const{return creature;}
 CCombatManager& GetCombatManager()const{fixture.trace+='W';return combat;}
 bool IsEnemyCreatureInCombatPosition()const{fixture.trace+='C';return (fixture.flags&2)||(fixture.iteration&&(fixture.flags&8));}
 float GetProximityToBeConsideredNearToEnemyCreature()const{return GetCreature().GetPhysicsTC()->GetDiameter()/1.2f+1.0f;}
 static void NavFlag(bool& b){check(b==fixture.navSeen);b=true;fixture.navSeen=true;}
 EAIActionReturn AIAction_MoveToPos(EAIActionRepeat repeat,C3DVector p,bool& b,EMovementType movement,bool a,bool c,const CThing* avoid,bool d,float speed){
  check(repeat==2&&movement==5&&a&&c&&!avoid&&d&&speed==0&&p.X==2&&p.Y==4&&p.Z==8);NavFlag(b);fixture.trace+='M';return EAIActionReturn(fixture.result);
 }
 EAIActionReturn AIAction_MoveToThingProximity(EAIActionRepeat repeat,CIntelligentPointer<const CThing> target,float proximity,bool& b,EMovementType movement,bool a,bool c,bool d,bool e,float speed){
  check(repeat==2&&target.ptr==&enemy&&movement==5&&a&&c&&d&&e&&speed==0);
  check(std::fabs(proximity-(1.0+fixture.radius*5.0/3.0))<1.0/65536.0);NavFlag(b);fixture.trace+='X';return EAIActionReturn(fixture.result);
 }
 EAIActionReturn AIAction_DoNothing(){fixture.trace+='D';return EAIActionReturn(fixture.result);}
 bool IsStillRunnable(bool testEnemy){check(testEnemy);fixture.trace+='S';++fixture.iteration;return fixture.iteration<fixture.steps;}
 bool MoveToEnemyCreatureCombatPosition();
};
bool CAIStateGroup_MeleeCombatNearby::MoveToEnemyCreatureCombatPosition()
{
 EAIActionReturn ret=AI_ACTION_IN_PROGRESS;
 bool nav_route_calculated=false;
 const CTCCreatureNavigation* ptc_creature_nav;
 do {
  if(!GetCreature().IsInMeleeAsAttacker()) return false;
  if(IsEnemyCreatureInCombatPosition()) break;
  C3DVector pos=GetCombatManager().GetPreferredPos(GetCreature());
  GetCreature().PeekTC(&ptc_creature_nav);
  if(ptc_creature_nav->IsPosNavLineOfSightTo(pos))
   ret=AIAction_MoveToPos(ACTION_REPEAT_CALLBACK,pos,nav_route_calculated,AI_MOVEMENT_RUNNING,true,true,0,true,0.0f);
  else if(!GetCombatManager().IsCreatureOnOuterRing(GetCreature()))
   ret=AIAction_MoveToThingProximity(ACTION_REPEAT_CALLBACK,PEnemy,GetProximityToBeConsideredNearToEnemyCreature(),nav_route_calculated,AI_MOVEMENT_RUNNING,true,true,true,true,0.0f);
  else ret=AIAction_DoNothing();
  if(ret!=AI_ACTION_IN_PROGRESS&&ret!=AI_ACTION_FINISHED) return false;
 }while(IsStillRunnable(true));
 return true;
}

int main(){
 const int results[]={4,2,3,1};int cases=0;
 for(int flags=0;flags<256;++flags)for(int ret=0;ret<4;++ret)for(int steps=1;steps<=3;++steps)for(int r=0;r<5;++r){
  fixture.flags=flags;fixture.result=results[ret];fixture.steps=steps;fixture.iteration=fixture.conversions=0;
  fixture.navSeen=false;fixture.radius=r*0.75f;fixture.trace="";
  std::string expected;bool answer=true;int conversions=0;
  for(int i=0;i<steps;++i){
   expected+='R';if(!(flags&1)||(i&&(flags&4))){answer=false;break;}
   expected+='C';if((flags&2)||(i&&(flags&8)))break;
   expected+="WPNL";
   bool los=((flags&16)!=0)^((flags&64)&&((i&1)!=0));
   bool outer=((flags&32)!=0)^((flags&128)&&((i&1)!=0));
   if(los)expected+='M';else{expected+="WO";if(outer)expected+='D';else{expected+='X';++conversions;}}
   if(ret>=2){answer=false;break;}expected+='S';
  }
  CAIStateGroup_MeleeCombatNearby state;bool got=state.MoveToEnemyCreatureCombatPosition();
  check(got==answer&&fixture.trace==expected&&fixture.conversions==conversions);++cases;
 }
 check(cases==15360);std::puts("0096ec10_TEST PASS");return 0;
}