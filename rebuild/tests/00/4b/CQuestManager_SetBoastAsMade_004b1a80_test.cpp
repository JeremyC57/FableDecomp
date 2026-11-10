// Selected target copied verbatim; true std::list and observable message/lookup fixtures.
#include <stdio.h>
#include <stdlib.h>
#include <list>
#include <string>
#define CHECK(x) do{if(!(x)){printf("FAIL %d\n",__LINE__);abort();}}while(0)
static int liveStrings,lookupCalls,removeCalls,messageCalls,componentCalls;
class CCharString{public:std::string Text;CCharString(const char* s=""):Text(s){++liveStrings;}CCharString(const CCharString& s):Text(s.Text){++liveStrings;}~CCharString(){--liveStrings;}};
class C3DVector{public:float X,Y,Z;C3DVector(float x,float y,float z):X(x),Y(y),Z(z){}};
class CTCHeroStats{public:long NumBoastsTaken;long GetNumBoastsTaken()const{return NumBoastsTaken;}void SetNumBoastsTaken(long n){NumBoastsTaken=n;}};
class CThing{};class CThingPlayerCreature:public CThing{public:bool Dead;CTCHeroStats* Stats;bool IsDead()const{return Dead;}bool GetTC(CTCHeroStats** out){++componentCalls;if(!Stats)return false;*out=Stats;return true;}};
template<class T>inline bool IS_THING_ALIVE(const T& thing){if(thing!=0){if(!thing->IsDead())return true;}return false;}
class CPlayer{public:CThingPlayerCreature* Hero;CThingPlayerCreature* GetPControlledCreature()const{return Hero;}};
class CPlayerManager{public:CPlayer Player;CPlayer& GetMainPlayer(){return Player;}};
class CMainGameComponent{public:static CMainGameComponent* Main;CPlayerManager Manager;static CMainGameComponent* Get(){return Main;}CPlayerManager& GetPlayerManager(){return Manager;}};
CMainGameComponent* CMainGameComponent::Main;
enum EMessageEventType{MESSAGE_EVENT_BOAST_MADE=41};class CMessageEvent{};class CQuestManager;
class CMessageEventManager{public:CQuestManager* Quest;long Payload;CMessageEvent* AddMessage(EMessageEventType,unsigned long,const C3DVector&,CThing*,CThing*,float,float,long);};
class CWorld{public:CMessageEventManager Messages;CMessageEventManager& GetMessageEventManager(){return Messages;}};
class CQuestManager{public:
 class CBoast{public:long ID,Cost,Reward;CCharString ScriptName;std::string Text,Title;bool Evil,Failed,Completed;long GroupIndex;CBoast():ID(0),Cost(0),Reward(0),Evil(false),Failed(false),Completed(false),GroupIndex(0){}};
 std::list<CBoast> Boasts,AcceptedBoasts;CWorld* World;
 CBoast* GetBoast(long id,CCharString script){++lookupCalls;for(std::list<CBoast>::iterator i=Boasts.begin();i!=Boasts.end();++i)if(i->ID==id&&i->ScriptName.Text==script.Text)return &*i;return 0;}
 void RemoveBoast(long id,CCharString script){++removeCalls;CHECK(messageCalls==1);for(std::list<CBoast>::iterator i=Boasts.begin();i!=Boasts.end();++i)if(i->ID==id&&i->ScriptName.Text==script.Text){Boasts.erase(i);return;}CHECK(false);}
 void SetBoastAsMade(long,CCharString);
};
CMessageEvent* CMessageEventManager::AddMessage(EMessageEventType type,unsigned long delay,const C3DVector& pos,CThing* from,CThing* to,float first,float second,long data){++messageCalls;CHECK(!removeCalls&&type==MESSAGE_EVENT_BOAST_MADE&&delay==50&&pos.X==0&&pos.Y==0&&pos.Z==0&&!from&&!to&&first==0&&second==0);CHECK(data==(long)&Quest->AcceptedBoasts.back());CHECK(Quest->Boasts.size()==2);Payload=data;return 0;}
void CQuestManager::SetBoastAsMade(long index,CCharString script_name)
{
 CBoast* pboast=GetBoast(index,script_name);
 if(pboast){
  CThingPlayerCreature* phero=CMainGameComponent::Get()->GetPlayerManager().GetMainPlayer().GetPControlledCreature();
  if(IS_THING_ALIVE(phero)){
   CTCHeroStats* ptc_hero_stats;
   if(phero->GetTC(&ptc_hero_stats))ptc_hero_stats->SetNumBoastsTaken(ptc_hero_stats->GetNumBoastsTaken()+1);
  }
  AcceptedBoasts.push_back(*pboast);
  long extra_data=(long)&AcceptedBoasts.back();
  World->GetMessageEventManager().AddMessage(MESSAGE_EVENT_BOAST_MADE,50,C3DVector(0.0f,0.0f,0.0f),0,0,0.0f,0.0f,extra_data);
  RemoveBoast(index,script_name);
 }
}

int main(){int cases=0;CMainGameComponent game;CMainGameComponent::Main=&game;CThingPlayerCreature hero;CTCHeroStats stats;
for(int found=0;found<2;++found)for(int state=0;state<4;++state)for(int previous=0;previous<3;++previous)for(int amount=-1;amount<2;++amount){
 CHECK(!liveStrings);{
 CWorld world;CQuestManager quest;quest.World=&world;world.Messages.Quest=&quest;world.Messages.Payload=0;
 CQuestManager::CBoast boast;boast.ID=7;boast.Cost=15;boast.Reward=40;boast.ScriptName.Text="quest";boast.Text="No armour";boast.Title="Challenge";boast.Evil=true;boast.Completed=true;boast.GroupIndex=3;
 quest.Boasts.push_back(boast);boast.ID=8;quest.Boasts.push_back(boast);for(int i=0;i<previous;++i)quest.AcceptedBoasts.push_back(boast);
 game.Manager.Player.Hero=state?&hero:0;hero.Dead=state==1;hero.Stats=state==3?&stats:0;stats.NumBoastsTaken=amount;
 lookupCalls=removeCalls=messageCalls=componentCalls=0;
 quest.SetBoastAsMade(7,CCharString(found?"quest":"other"));
 CHECK(lookupCalls==1&&removeCalls==found&&messageCalls==found);CHECK(quest.Boasts.size()==(found?1:2)&&quest.AcceptedBoasts.size()==previous+found);
 CHECK(componentCalls==(found&&state>=2));CHECK(stats.NumBoastsTaken==amount+(found&&state==3));
 if(found){const CQuestManager::CBoast& accepted=quest.AcceptedBoasts.back();CHECK(accepted.ID==7&&accepted.Cost==15&&accepted.Reward==40&&accepted.ScriptName.Text=="quest"&&accepted.Text=="No armour"&&accepted.Title=="Challenge"&&accepted.Evil&&!accepted.Failed&&accepted.Completed&&accepted.GroupIndex==3);CHECK(world.Messages.Payload==(long)&accepted&&quest.Boasts.front().ID==8);}
 else CHECK(!world.Messages.Payload);
 ++cases;
 }CHECK(!liveStrings);
}
CHECK(cases==72);puts("004b1a80_TEST PASS");return 0;}