#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <string>
static void check(bool value){if(!value)std::abort();}
static std::string trace;static int scenario,created,destroyed;static bool* effects=0;
class C3DVector{ public:float X,Y,Z;C3DVector(float x=0,float y=0,float z=0):X(x),Y(y),Z(z){}const C3DVector operator-(const C3DVector& o)const{return C3DVector(X-o.X,Y-o.Y,Z-o.Z);}float GetAngleXY()const{float a=(float)std::atan2((double)X,(double)Y)*0.15915494f;if(a<0||a>=1){a=(float)std::fmod((double)a,1.0);if(a<0)a+=1;}return a;}};
struct Physics{C3DVector pos;float facing;int resets,faces;void ResetPosition(const C3DVector& p){check(trace.empty());trace+='R';pos=p;++resets;}void SetFacingAngleXY(float f){check(trace=="R");trace+='F';facing=f;++faces;}};
class CThing{ public:Physics* physics;bool dead;CThing():physics(0),dead(false){}bool IsDead()const{return dead;}const C3DVector& GetPos()const{check(!trace.empty()&&trace[0]=='R');return physics->pos;}Physics* GetPhysicsTC()const{return physics;}};
class CCreatureActionBase{ public:virtual ~CCreatureActionBase(){}};
class CThingCreatureBase:public CThing{ public:int actions;bool SetCurrentAction(const CCreatureActionBase& action);};
template<class T>class CIntelligentPointer{ public:T* ptr;CIntelligentPointer(T* p=0):ptr(p){}T* Get()const{return ptr;}T* operator->()const{return ptr;}};
template<class T>bool operator!=(const CIntelligentPointer<T>& p,int){return p.Get()!=0;}
template<class T>bool IS_THING_ALIVE(const T& t){if(t!=0){if(!t->IsDead())return true;}return false;}
class CCombatActionBase:public CCreatureActionBase{ public:CIntelligentPointer<CThingCreatureBase> PCreature;CIntelligentPointer<CThing>PTarget;void FinishAction(){check(trace=="R"||trace=="RF");trace+='B';if(scenario&4)PCreature.ptr=0;if(scenario&2)*effects=!*effects;}};
static CThingCreatureBase* expectedCreature=0;
class CCreatureAction_TeleportIn:public CCombatActionBase{ public:bool CreateEffects;CCreatureAction_TeleportIn(CThingCreatureBase& c,bool flag):CreateEffects(flag){check(trace=="RB"||trace=="RFB");check(&c==expectedCreature&&flag==*effects);trace+='I';++created;}~CCreatureAction_TeleportIn(){check(trace=="RBIS"||trace=="RFBIS");trace+='D';++destroyed;}};
bool CThingCreatureBase::SetCurrentAction(const CCreatureActionBase& action){const CCreatureAction_TeleportIn& in=static_cast<const CCreatureAction_TeleportIn&>(action);check(in.CreateEffects==*effects&&this==expectedCreature);trace+='S';++actions;return true;}
class CCreatureAction_TeleportOut:public CCombatActionBase{ public:C3DVector TeleportPos;bool CreateEffects;void FinishAction();};
void CCreatureAction_TeleportOut::FinishAction()
{
 PCreature->GetPhysicsTC()->ResetPosition(TeleportPos);
 if(IS_THING_ALIVE(PTarget)) {
  C3DVector vector_to_target=PTarget->GetPos()-PCreature->GetPos();
  PCreature->GetPhysicsTC()->SetFacingAngleXY(vector_to_target.GetAngleXY());
 }
 CThingCreatureBase* pcreature=PCreature.Get();
 CCombatActionBase::FinishAction();
 pcreature->SetCurrentAction(CCreatureAction_TeleportIn(*pcreature,CreateEffects));
}

int main(){
 const float values[]={-4,-1,0,1,4};int cases=0;created=destroyed=0;
 for(int live=0;live<3;++live)for(int origin=0;origin<8;++origin)for(int x=0;x<5;++x)for(int y=0;y<5;++y)for(int z=0;z<5;++z)for(int flags=0;flags<8;++flags){
  Physics physics,targetPhysics;physics.pos=C3DVector(-16,32,-64);physics.facing=-8;physics.resets=physics.faces=0;
  CThingCreatureBase creature;creature.physics=&physics;creature.actions=0;expectedCreature=&creature;
  CThing target;target.physics=&targetPhysics;target.dead=live==2;
  CCreatureAction_TeleportOut action;action.PCreature.ptr=&creature;action.PTarget.ptr=live?&target:0;
  action.TeleportPos=C3DVector(origin*0.5f,-origin*0.25f,origin*0.125f);
  targetPhysics.pos=C3DVector(action.TeleportPos.X+values[x],action.TeleportPos.Y+values[y],action.TeleportPos.Z+values[z]);
  action.CreateEffects=(flags&1)!=0;effects=&action.CreateEffects;scenario=flags;trace="";
  action.FinishAction();
  check(trace==(live==1?"RFBISD":"RBISD")&&physics.resets==1&&physics.faces==(live==1?1:0)&&creature.actions==1);
  check(physics.pos.X==action.TeleportPos.X&&physics.pos.Y==action.TeleportPos.Y&&physics.pos.Z==action.TeleportPos.Z);
  if(live==1){double expected=std::atan2((double)values[x],(double)values[y])/(2*std::acos(-1.0));if(expected<0)expected+=1;check(std::fabs(physics.facing-expected)<1.0/1048576.0);}else check(physics.facing==-8);
  check(action.CreateEffects==(((flags&1)!=0)^((flags&2)!=0)));
  check(action.PCreature.ptr==((flags&4)?0:&creature));++cases;
 }
 check(cases==24000&&created==24000&&destroyed==24000);std::puts("008bd3f0_TEST PASS");return 0;
}