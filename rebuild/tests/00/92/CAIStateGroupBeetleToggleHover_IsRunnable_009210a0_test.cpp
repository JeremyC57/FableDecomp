#include <cstdio>
#include <cstdlib>
#include <cmath>
static void check(bool b){if(!b)std::abort();}
class C3DVector{ public:float X,Y,Z;C3DVector(float x=0,float y=0,float z=0):X(x),Y(y),Z(z){}C3DVector operator-(const C3DVector& b)const{return C3DVector(X-b.X,Y-b.Y,Z-b.Z);}float GetSquaredMagnitude2D()const{return X*X+Y*Y;}};
class CThing{ public:C3DVector pos;bool dead;CThing():dead(false){}bool IsDead()const{return dead;}const C3DVector& GetPos()const{return pos;}};
class CThingCreatureBase:public CThing{ public:};
class CTCTargetingBase{ public:CThingCreatureBase* enemy;int calls;CTCTargetingBase():enemy(0),calls(0){}CThingCreatureBase* GetPBestEnemyCreatureForAttack(){++calls;return enemy;}};
class CTCCreatureModeManager{ public:class CModeBase{ public:unsigned long type;unsigned long GetType()const{return type;}} mode;int calls;CTCCreatureModeManager():calls(0){}CModeBase& GetCurrentMode(){++calls;return mode;}};
class CThingAICreature:public CThingCreatureBase{ public:CTCTargetingBase* targeting;CTCCreatureModeManager* modes;int queries;CThingAICreature():targeting(0),modes(0),queries(0){}bool GetTC(CTCTargetingBase** p){++queries;if(targeting){*p=targeting;return true;}return false;}bool GetTC(CTCCreatureModeManager** p){++queries;if(modes){*p=modes;return true;}return false;}};
namespace NCreatureMode{enum{MODE_HOVER=26};}
namespace NCreatureAI{static const CThingAICreature* expectedViewer;static const CThing* expectedTarget;static bool aware;static int calls;bool IsAwareOfThing(const CThingAICreature* a,const CThing* b,bool c){check(a==expectedViewer&&b==expectedTarget&&!c);++calls;return aware;}}
template<class T> bool IS_THING_ALIVE(const T& t){if(t!=0){if(!t->IsDead())return true;}return false;}
struct CAIStateGroup_BeetleToggleHover{CThingAICreature* creature;bool IsLanding;CThingAICreature& GetCreature()const{return *creature;}bool IsRunnable();};
bool CAIStateGroup_BeetleToggleHover::IsRunnable()
{
 float distToEnemySq=1e10f;
 CTCTargetingBase* ptc_targ;
 CTCCreatureModeManager* pTCModeMan;
 if(GetCreature().GetTC(&ptc_targ)) {
  CThingCreatureBase* pcret_attack=ptc_targ->GetPBestEnemyCreatureForAttack();
  if(IS_THING_ALIVE(pcret_attack)&&NCreatureAI::IsAwareOfThing(&GetCreature(),pcret_attack,false)) {
   C3DVector diff=GetCreature().GetPos()-pcret_attack->GetPos();
   distToEnemySq=diff.GetSquaredMagnitude2D();
  }
 }
 if(GetCreature().GetTC(&pTCModeMan)) {
  IsLanding=pTCModeMan->GetCurrentMode().GetType()==NCreatureMode::MODE_HOVER;
  if(IsLanding) {
   if(distToEnemySq>100.0f) return true;
  } else if(distToEnemySq<25.0f) return true;
 }
 return false;
}

int main(){
 CThingAICreature creature;CThingCreatureBase enemy;CTCTargetingBase targeting;CTCCreatureModeManager modes;CAIStateGroup_BeetleToggleHover state;state.creature=&creature;
 const float distances[8]={0,4.75f,5,5.25f,9.75f,10,10.25f,20};
 int cases=0;
 for(int bits=0;bits<64;++bits)for(int d=0;d<8;++d){
  creature.targeting=(bits&1)?&targeting:0;creature.modes=(bits&2)?&modes:0;
  targeting.enemy=(bits&4)?&enemy:0;enemy.dead=(bits&8)!=0;NCreatureAI::aware=(bits&16)!=0;
  modes.mode.type=(bits&32)?26:1;
  creature.pos=C3DVector(distances[d],0,100);enemy.pos=C3DVector(0,0,-100);
  NCreatureAI::expectedViewer=&creature;NCreatureAI::expectedTarget=&enemy;NCreatureAI::calls=0;creature.queries=0;targeting.calls=0;modes.calls=0;
  state.IsLanding=(cases&1)!=0;bool old=state.IsLanding;
  bool live=(bits&1)&&(bits&4)&&!(bits&8);
  float distance=(live&&(bits&16))?distances[d]*distances[d]:1e10f;
  bool hover=modes.mode.type==26;
  bool expected=(bits&2)&&(hover?distance>100:distance<25);
  check(state.IsRunnable()==expected);
  check(state.IsLanding==((bits&2)?hover:old));
  check(creature.queries==2&&targeting.calls==((bits&1)?1:0)&&modes.calls==((bits&2)?1:0)&&NCreatureAI::calls==(live?1:0));
  ++cases;
 }
 check(cases==512);std::puts("009210a0_TEST PASS");return 0;
}