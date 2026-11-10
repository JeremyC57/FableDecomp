// Body-level fixture: real selected callback and complete setter, external services modeled.
#include <cassert>
#include <cstdio>
#include <cstring>
static int live_strings, made_strings, gone_strings;
class CCharString {
public:
 const char* text;
 CCharString(const char* p,long=-1):text(p){++live_strings;++made_strings;}
 ~CCharString(){--live_strings;++gone_strings;}
};
class CScriptDef { public:
 long DBNumMinionsHighHealth,DBNumMinionsMediumHealth,DBNumMinionsLowHealth,DBNumMinionsVeryLowHealth;
 long DBNumSummonersHighHealth,DBNumSummonersMediumHealth,DBNumSummonersLowHealth,DBNumSummonersVeryLowHealth;
};
static CScriptDef* pGlobalScriptDef;
struct TestThing {
 bool result;int calls;
 bool MsgIsKilledBy(const CCharString& s){assert(live_strings==1);assert(s.text && !std::strcmp(s.text,""));++calls;return result;}
};
namespace NScript {
class CQ_DragonBossFightScript {
public:
 enum EDragonState {DS_HEALTH_HIGH,DS_HEALTH_MEDIUM,DS_HEALTH_LOW,DS_HEALTH_VERY_LOW,DS_DEAD};
 EDragonState DragonState;long TargetNumMinions,TargetNumSummoners;
 bool terminating;int queries;
 bool IsActiveThreadTerminating(){assert(live_strings==0);++queries;return terminating;}
 void SetDragonState(EDragonState);
 class CDragon {
 public:
  TestThing Me;CQ_DragonBossFightScript& ParentClass;
  CDragon(CQ_DragonBossFightScript& p):ParentClass(p){Me.calls=0;Me.result=false;}
  void OnPredicateFail();
 };
};
}
void NScript::CQ_DragonBossFightScript::SetDragonState(EDragonState dragonState) {
        DragonState=dragonState;
        switch(dragonState) {
        case DS_HEALTH_HIGH:
            if(IsActiveThreadTerminating()) return;
            TargetNumMinions=pGlobalScriptDef->DBNumMinionsHighHealth;
            TargetNumSummoners=pGlobalScriptDef->DBNumSummonersHighHealth;
            break;
        case DS_HEALTH_MEDIUM:
            if(IsActiveThreadTerminating()) return;
            TargetNumMinions=pGlobalScriptDef->DBNumMinionsMediumHealth;
            TargetNumSummoners=pGlobalScriptDef->DBNumSummonersMediumHealth;
            break;
        case DS_HEALTH_LOW:
            if(IsActiveThreadTerminating()) return;
            TargetNumMinions=pGlobalScriptDef->DBNumMinionsLowHealth;
            TargetNumSummoners=pGlobalScriptDef->DBNumSummonersLowHealth;
            break;
        case DS_HEALTH_VERY_LOW:
            if(IsActiveThreadTerminating()) return;
            TargetNumMinions=pGlobalScriptDef->DBNumMinionsVeryLowHealth;
            TargetNumSummoners=pGlobalScriptDef->DBNumSummonersVeryLowHealth;
            break;
        default: break;
        }
    }
void NScript::CQ_DragonBossFightScript::CDragon::OnPredicateFail() {
    if(Me.MsgIsKilledBy("")) ParentClass.SetDragonState(DS_DEAD);
}

int main(){
 typedef NScript::CQ_DragonBossFightScript Q;
 CScriptDef def={11,12,13,14,21,22,23,24};pGlobalScriptDef=&def;
 int cases=0;
 for(int state=0;state<5;++state)for(int termination=0;termination<2;++termination){
  Q q;q.DragonState=Q::DS_DEAD;q.TargetNumMinions=-99;q.TargetNumSummoners=-88;q.terminating=termination!=0;q.queries=0;
  q.SetDragonState(static_cast<Q::EDragonState>(state));
  assert(q.DragonState==state);assert(q.queries==(state<4?1:0));
  assert(q.TargetNumMinions==((state<4&&!termination)?11+state:-99));
  assert(q.TargetNumSummoners==((state<4&&!termination)?21+state:-88));++cases;
 }
 for(int state=0;state<5;++state)for(int killed=0;killed<2;++killed)for(int termination=0;termination<2;++termination)for(int repeat=1;repeat<4;++repeat){
  Q q;q.DragonState=static_cast<Q::EDragonState>(state);q.TargetNumMinions=101;q.TargetNumSummoners=202;q.terminating=termination!=0;q.queries=0;
  Q::CDragon dragon(q);dragon.Me.result=killed!=0;int before=made_strings;
  for(int i=0;i<repeat;++i)dragon.OnPredicateFail();
  assert(q.DragonState==(killed?Q::DS_DEAD:state));assert(q.TargetNumMinions==101&&q.TargetNumSummoners==202);
  assert(q.queries==0&&dragon.Me.calls==repeat);assert(made_strings-before==repeat&&live_strings==0&&made_strings==gone_strings);++cases;
 }
 assert(cases==70);std::puts("00d259a0_TEST PASS");
}