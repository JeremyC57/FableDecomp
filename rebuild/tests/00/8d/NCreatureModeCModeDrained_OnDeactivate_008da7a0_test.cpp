#include <cstdio>
#include <string>
static std::string trace;static int pointer_reads,ctors,dtors,assignments;
enum EPlayerMode{PLAYER_MODE_PARALYSED=31};
class CPlayer{public:int calls;EPlayerMode removed;CPlayer():calls(0),removed(PLAYER_MODE_PARALYSED){}void RemoveMode(EPlayerMode n){trace+='P';++calls;removed=n;}};
class CTCCreatureModeManager{public:int calls;unsigned long removed;CTCCreatureModeManager():calls(0),removed(0){}void RemoveMode(unsigned long n){trace+='M';++calls;removed=n;}};
class CThing{public:unsigned long Type;bool dead;CTCCreatureModeManager*manager;int lookups;CPlayer player;CThing():Type(0),dead(false),manager(0),lookups(0){}unsigned long GetType()const{return Type;}bool IsDead()const{return dead;}CPlayer&GetPlayer(){return player;}bool GetTC(CTCCreatureModeManager**p){++lookups;if(manager){*p=manager;return true;}return false;}};
class CThingCreatureBase:public CThing{};
class CThingAICreature:public CThingCreatureBase{public:int suspends,resets;bool last_suspend;void*last_owner;CThingAICreature():suspends(0),resets(0),last_suspend(true),last_owner(this){}void SuspendAI(bool b,void*p){++suspends;last_suspend=b;last_owner=p;trace+='S';}void ForceAIReset(){++resets;trace+='R';}};
class CThingPlayerCreature:public CThingCreatureBase{};
template<class T>class CIntelligentPointer{public:T*item;CIntelligentPointer(T*p=0):item(p){++ctors;}~CIntelligentPointer(){++dtors;}T*Get()const{++pointer_reads;return item;}T*operator->()const{return Get();}CIntelligentPointer&operator=(const CIntelligentPointer&x){item=x.Get();++assignments;trace+='C';return*this;}};
template<class T>bool operator!=(const CIntelligentPointer<T>&p,int){return p.Get()!=0;}
template<class T>bool IS_THING_ALIVE(const T&thing){if(thing!=0){if(!thing->IsDead())return true;}return false;}
class CModeStandBase{public:CThing&ParentThing;int basecalls;bool Active;CModeStandBase(CThing&p):ParentThing(p),basecalls(0),Active(true){}CThing&GetParentThing(){return ParentThing;}void OnDeactivate(){++basecalls;Active=false;trace+='B';}};
namespace NCreatureMode{class CModeDrained:public CModeStandBase{public:CIntelligentPointer<CThing>PDrainer;unsigned long DrainerMode;CModeDrained(CThing&p):CModeStandBase(p),DrainerMode(0){}void OnDeactivate();};}
void NCreatureMode::CModeDrained::OnDeactivate()
{
 if(GetParentThing().GetType()==1){
  CThingAICreature& pai_creature=static_cast<CThingAICreature&>(GetParentThing());
  pai_creature.SuspendAI(false,0);
  pai_creature.ForceAIReset();
 }else if(GetParentThing().GetType()==2){
  CThingPlayerCreature& pplayer_creature=static_cast<CThingPlayerCreature&>(GetParentThing());
  pplayer_creature.GetPlayer().RemoveMode(PLAYER_MODE_PARALYSED);
 }
 if(IS_THING_ALIVE(PDrainer)){
  CThingCreatureBase& creature=static_cast<CThingCreatureBase&>(GetParentThing());
  CTCCreatureModeManager* pTCModeMan=0;
  if(PDrainer->GetTC(&pTCModeMan))pTCModeMan->RemoveMode(DrainerMode);
  PDrainer=CIntelligentPointer<CThing>(0);
 }
 CModeStandBase::OnDeactivate();
}

int main(){bool ok=true;int cases=0;unsigned long modes[]={0,31,0x80000000UL};for(int type=0;type<4;++type)for(int state=0;state<3;++state)for(int has=0;has<2;++has)for(int m=0;m<3;++m){
 CThingAICreature ai;CThingPlayerCreature player;CThingCreatureBase ordinary;CThing*parent=type==1?static_cast<CThing*>(&ai):type==2?static_cast<CThing*>(&player):static_cast<CThing*>(&ordinary);parent->Type=type;
 CThing drainer;CTCCreatureModeManager manager;drainer.manager=has?&manager:0;drainer.dead=state==1;
 NCreatureMode::CModeDrained mode(*parent);mode.PDrainer.item=state?&drainer:0;mode.DrainerMode=modes[m];trace.erase();pointer_reads=ctors=dtors=assignments=0;
 mode.OnDeactivate();std::string expected=type==1?"SR":type==2?"P":"";if(state==2){if(has)expected+='M';expected+='C';}expected+='B';
 ok &= trace==expected&&mode.basecalls==1&&!mode.Active&&mode.DrainerMode==modes[m]&&parent->Type==(unsigned long)type;
 ok &= ai.suspends==(type==1?1:0)&&ai.resets==(type==1?1:0);if(type==1)ok &= !ai.last_suspend&&ai.last_owner==0;
 ok &= player.player.calls==(type==2?1:0)&&player.player.removed==PLAYER_MODE_PARALYSED;
 ok &= manager.calls==((state==2&&has)?1:0);if(state==2&&has)ok &= manager.removed==modes[m];
 ok &= drainer.lookups==(state==2?1:0)&&mode.PDrainer.item==(state==1?&drainer:0);
 ok &= pointer_reads==(state==0?1:state==1?2:4)&&ctors==(state==2?1:0)&&dtors==(state==2?1:0)&&assignments==(state==2?1:0);
 ++cases;
 }ok &= cases==72;std::printf(ok?"008da7a0_TEST PASS\n":"008da7a0_TEST FAIL\n");return ok?0:1;}