#include <cstdio>
#include <cstdlib>
#include <cmath>
static void check(bool b){if(!b)std::abort();}
enum EAIActionReturn{AI_ACTION_NULL=0,AI_ACTION_TERMINATE_PROCESS=1,AI_ACTION_FINISHED=2,AI_ACTION_FAILED=3,AI_ACTION_IN_PROGRESS=4};
static int trace[32],used;static void log(int n){trace[used++]=n;}
class CBossDef{ public:long PhaseHealthPercent[4];};
class CTCBoss{ public:CBossDef* def;long phase;int gets,sets;long GetBossPhase()const{return phase;}bool HasBossDef()const{return def!=0;}const CBossDef& GetBossDef()const{return *def;}void SetNextBossPhase(long p,bool b){check(!b&&p==phase+1);++sets;phase=p;}};
class CThing{ public:virtual ~CThing(){}};
class CThingCreatureBase:public CThing { public:
 int busy,queries;float maxHealth,health;CTCBoss* boss;
 bool IsPerformingAnyAction(){++queries;return busy>0;}
 float GetMaxHealth()const{return maxHealth;}void SetHealth(float h){health=h;}
 bool GetTC(CTCBoss** b){if(boss){*b=boss;return true;}return false;}
};
class CThingAICreature:public CThingCreatureBase{ public:};
class CThingPlayerCreature:public CThingCreatureBase{ public:};
template<class T> class CIntelligentPointer{ public:T* ptr;CIntelligentPointer(T* p):ptr(p){}template<class U>CIntelligentPointer(CIntelligentPointer<U>& p):ptr(p.ptr){}T& operator*()const{return *ptr;}};
class CCreatureActionBase{ public:int kind;CThingCreatureBase* owner;CThingCreatureBase* target;CCreatureActionBase(int k,CThingCreatureBase& c,CThingCreatureBase* t):kind(k),owner(&c),target(t){log(k==1?101:104);}virtual ~CCreatureActionBase(){log(kind==1?103:106);}};
class CCreatureAction_BanditKingThrowWeapons:public CCreatureActionBase{ public:CCreatureAction_BanditKingThrowWeapons(CThingCreatureBase& c,CThingCreatureBase& t):CCreatureActionBase(1,c,&t){}};
class CCreatureAction_BanditKingChangeWeapons:public CCreatureActionBase{ public:CCreatureAction_BanditKingChangeWeapons(CThingCreatureBase& c):CCreatureActionBase(2,c,0){}};
struct CAIStateGroup_BanditKingThrowWeapons{
 CThingAICreature* creature;CIntelligentPointer<CThingPlayerCreature> Player;int waits,terminated,actions;EAIActionReturn result;
 CAIStateGroup_BanditKingThrowWeapons(CThingAICreature& c,CThingPlayerCreature& p):creature(&c),Player(&p){}
 CThingAICreature& GetCreature()const{return *creature;}
 EAIActionReturn AIAction_DoNothing(){++waits;--creature->busy;return result;}
 EAIActionReturn AIAction_PerformActionTargetFaceAndWait(const CCreatureActionBase& a,CIntelligentPointer<const CThing> p,bool b){check(a.kind==1&&a.owner==creature&&a.target==Player.ptr&&p.ptr==Player.ptr&&!b);log(102);++actions;return AI_ACTION_FINISHED;}
 EAIActionReturn AIAction_PerformActionAndWait(const CCreatureActionBase& a,bool b){check(a.kind==2&&a.owner==creature&&!b);log(105);++actions;return AI_ACTION_FINISHED;}
 void Terminate(){++terminated;}
 void Update();
};
void CAIStateGroup_BanditKingThrowWeapons::Update()
{
 while(GetCreature().IsPerformingAnyAction()) {
  EAIActionReturn ret=AIAction_DoNothing();
  if(ret!=AI_ACTION_IN_PROGRESS&&ret!=AI_ACTION_FINISHED){Terminate();return;}
 }
 AIAction_PerformActionTargetFaceAndWait(CCreatureAction_BanditKingThrowWeapons(GetCreature(),*Player),Player,false);
 AIAction_PerformActionAndWait(CCreatureAction_BanditKingChangeWeapons(GetCreature()),false);
 CTCBoss* ptcBoss;
 if(GetCreature().GetTC(&ptcBoss)) {
  long phase=ptcBoss->GetBossPhase();
  if(ptcBoss->HasBossDef()) GetCreature().SetHealth(float(ptcBoss->GetBossDef().PhaseHealthPercent[phase])*GetCreature().GetMaxHealth()*0.01f);
  ptcBoss->SetNextBossPhase(phase+1,false);
 }
 Terminate();
}

int main(){
 CThingAICreature creature;CThingPlayerCreature player;CTCBoss boss;CBossDef def;CAIStateGroup_BanditKingThrowWeapons state(creature,player);
 const long pct[4]={0,25,150,-10};for(int j=0;j<4;++j)def.PhaseHealthPercent[j]=pct[j];
 int cases=0;
 for(int busy=0;busy<4;++busy)for(int result=0;result<5;++result)for(int flags=0;flags<4;++flags)for(int phase=0;phase<4;++phase)for(int health=0;health<3;++health){
  creature.busy=busy;creature.queries=0;creature.maxHealth=health==0?0.0f:health==1?200.0f:128.0f;creature.health=77.0f;creature.boss=(flags&1)?&boss:0;
  boss.phase=phase;boss.def=(flags&2)?&def:0;boss.sets=0;used=0;
  state.waits=state.terminated=state.actions=0;state.result=EAIActionReturn(result);
  bool abort=busy>0&&result!=2&&result!=4;
  state.Update();
  check(state.terminated==1&&state.actions==(abort?0:2));
  check(state.waits==(abort?1:busy));
  check(creature.queries==(abort?1:busy+1));
  check(boss.sets==((!abort&&(flags&1))?1:0));
  check(boss.phase==phase+((!abort&&(flags&1))?1:0));
  float expected=(!abort&&flags==3)?float(pct[phase])*creature.maxHealth*0.01f:77.0f;
  check(std::fabs(creature.health-expected)<0.0001f);
  check(used==(abort?0:6));for(int j=0;j<used;++j)check(trace[j]==101+j);
  ++cases;
 }
 check(cases==960);std::puts("00961640_TEST PASS");return 0;
}