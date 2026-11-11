#include <list>
#include <vector>
#include <string>
#include <algorithm>
#include <cstdio>
#include <cstdlib>
static void check(bool x){if(!x){std::puts("004b2890_TEST FAIL");std::exit(1);}}
typedef std::string CCharString;
struct C3DVector{float X,Y,Z;C3DVector(float x,float y,float z):X(x),Y(y),Z(z){}};
enum EMessageEventType{MESSAGE_EVENT_BOAST_MADE=41,MESSAGE_EVENT_LEAVING_QUEST_START_SCREEN=73};
struct CTCQuestCompletionUI{CCharString ScriptName;int sets;void SetScriptName(const CCharString& s){ScriptName=s;++sets;}};
struct CThingPlayerCreature{bool dead;CTCQuestCompletionUI*ui;int lookups;bool IsDead()const{return dead;}bool GetTC(CTCQuestCompletionUI**p){++lookups;if(!ui)return false;*p=ui;return true;}};
template<class T>bool IS_THING_ALIVE(const T&p){if(p!=0){if(!p->IsDead())return true;}return false;}
struct CPlayer{CThingPlayerCreature*hero;int gets;CThingPlayerCreature*GetPControlledCreature()const{++const_cast<CPlayer*>(this)->gets;return hero;}};
struct CPlayerManager{CPlayer player;int gets;CPlayer&GetMainPlayer(){++gets;return player;}};
struct CMainGameComponent{CPlayerManager manager;static CMainGameComponent main;static CMainGameComponent*Get(){return &main;}CPlayerManager&GetPlayerManager(){return manager;}};CMainGameComponent CMainGameComponent::main;
template<class T>struct CCountedPointer{T*Data;CCountedPointer(T*p=0):Data(p){}T*operator->()const{return Data;}};
struct Message{EMessageEventType type;CThingPlayerCreature*source;long extra;};
struct CMessageEventManager{std::vector<Message>messages;void AddMessage(EMessageEventType t,unsigned long delay,const C3DVector&p,CThingPlayerCreature*a,CThingPlayerCreature*b,float f,float g,long d){check(delay==5000&&p.X==0&&p.Y==0&&p.Z==0&&b==0&&f==0&&g==0);Message m;m.type=t;m.source=a;m.extra=d;messages.push_back(m);}};
struct CWorld{CMessageEventManager events;CMessageEventManager&GetMessageEventManager(){return events;}};
struct CQuestManager{
 struct CBoast{long ID,Cost,Reward,GroupIndex;CCharString ScriptName,Text,Title;bool Evil,Failed,Completed;
 CBoast():ID(0),Cost(7),Reward(11),GroupIndex(13),ScriptName("A"),Text("text"),Title("title"),Evil(false),Failed(false),Completed(false){}
 long GetID()const{return ID;}const CCharString&GetScriptName()const{return ScriptName;}bool IsFailed()const{return Failed;}bool IsCompleted()const{return Completed;}void SetAsFailed(){Failed=true;}void SetAsCompleted(){Completed=true;}
 };
 struct CActiveQuest{CCharString QuestName;const CCharString&GetName()const{return QuestName;}};
 std::list<CBoast>TempBoasts,AcceptedBoasts;
 std::list<CCountedPointer<CActiveQuest> >ActiveQuests;
 std::vector<CCharString>BetweenStartAndEndScreenQuests;
 CWorld&World;
 CBoast offered[2];bool available[2];int remove_mode;std::vector<long>gets,removed;
 CQuestManager(CWorld&w):World(w),remove_mode(0){available[0]=available[1]=false;}
 CBoast*GetBoast(long id,CCharString script){gets.push_back(id);for(int i=0;i<2;++i)if(available[i]&&offered[i].ID==id&&offered[i].ScriptName==script)return &offered[i];return 0;}
 void RemoveBoast(long id,CCharString script){removed.push_back(id);for(int i=0;i<2;++i)if(available[i]&&offered[i].ID==id&&offered[i].ScriptName==script){available[i]=false;if(remove_mode&1)offered[i].Failed=true;if(remove_mode&2)offered[i].Completed=true;return;}check(false);}
 void GetAcceptedBoastsForScript(const CCharString&,std::list<CBoast>&)const;
 void PostBoastTransfer();
};
void CQuestManager::GetAcceptedBoastsForScript(const CCharString& script_name,std::list<CBoast>& boasts)const
{
 boasts.clear();
 for(std::list<CBoast>::const_iterator it=AcceptedBoasts.begin();it!=AcceptedBoasts.end();++it)
  if(it->GetScriptName()==script_name)boasts.push_back(*it);
}

void CQuestManager::PostBoastTransfer()
{
 for(std::list<CBoast>::iterator it=TempBoasts.begin();it!=TempBoasts.end();++it) {
  CBoast* pboast=GetBoast(it->GetID(),it->GetScriptName());
  if(pboast) {
   if(it->IsFailed())pboast->SetAsFailed();
   if(it->IsCompleted())pboast->SetAsCompleted();
   AcceptedBoasts.push_back(*pboast);
   RemoveBoast(pboast->GetID(),pboast->GetScriptName());
   if(pboast->IsCompleted())continue;
   if(pboast->IsFailed())continue;
   {
    long extra_data=(long)&AcceptedBoasts.back();
    World.GetMessageEventManager().AddMessage(MESSAGE_EVENT_BOAST_MADE,5000,C3DVector(0,0,0),0,0,0,0,extra_data);
   }
  }
 }
 CThingPlayerCreature* pplayer=CMainGameComponent::Get()->GetPlayerManager().GetMainPlayer().GetPControlledCreature();
 if(IS_THING_ALIVE(pplayer)) {
 for(std::list<CCountedPointer<CActiveQuest> >::iterator qit=ActiveQuests.begin();qit!=ActiveQuests.end();++qit) {
  std::vector<CCharString>::iterator last=BetweenStartAndEndScreenQuests.end();
  std::vector<CCharString>::iterator fit=std::find(BetweenStartAndEndScreenQuests.begin(),last,(*qit)->GetName());
  if(fit==last)continue;
  {
   std::list<CBoast> boasts;
   GetAcceptedBoastsForScript((*qit)->GetName(),boasts);
   if(!boasts.empty()) {
    CTCQuestCompletionUI* ptc_ui;
    if(pplayer->GetTC(&ptc_ui)) {
     ptc_ui->SetScriptName((*qit)->GetName());
     World.GetMessageEventManager().AddMessage(MESSAGE_EVENT_LEAVING_QUEST_START_SCREEN,5000,C3DVector(0,0,0),pplayer,0,0,0,0);
    }
    break;
   }
  }
 }
}
}

int main(){unsigned cases=0;
for(int temp_count=0;temp_count<3;++temp_count)for(int found=0;found<4;++found)for(int before=0;before<4;++before)for(int transferred=0;transferred<4;++transferred)for(int mutation=0;mutation<4;++mutation)for(int hero_state=0;hero_state<3;++hero_state)for(int ui_state=0;ui_state<2;++ui_state)for(int quest_case=0;quest_case<4;++quest_case){
 CWorld world;CQuestManager q(world);q.remove_mode=mutation;
 CTCQuestCompletionUI ui;ui.ScriptName="unchanged";ui.sets=0;
 CThingPlayerCreature hero;hero.dead=hero_state==1;hero.ui=ui_state?&ui:0;hero.lookups=0;
 CMainGameComponent::main.manager.gets=0;CMainGameComponent::main.manager.player.gets=0;CMainGameComponent::main.manager.player.hero=hero_state?&hero:0;
 for(int i=0;i<2;++i){q.offered[i].ID=i+1;q.offered[i].ScriptName=i?"B":"A";q.offered[i].Failed=(before&1)!=0;q.offered[i].Completed=(before&2)!=0;q.available[i]=(found&(1<<i))!=0;
  if(i<temp_count){CQuestManager::CBoast temp;temp.ID=i+1;temp.ScriptName=q.offered[i].ScriptName;temp.Failed=(transferred&1)!=0;temp.Completed=(transferred&2)!=0;q.TempBoasts.push_back(temp);}}
 CQuestManager::CActiveQuest quests[3];quests[0].QuestName="missing";quests[1].QuestName="B";quests[2].QuestName="A";
 for(int i=0;i<3;++i)q.ActiveQuests.push_back(CCountedPointer<CQuestManager::CActiveQuest>(&quests[i]));
 q.BetweenStartAndEndScreenQuests.push_back("missing");if(quest_case&1)q.BetweenStartAndEndScreenQuests.push_back(std::string(1,'A'));if(quest_case&2)q.BetweenStartAndEndScreenQuests.push_back(std::string(1,'B'));
 // Existing accepted entry proves helper clearing/filtering and first eligible quest behavior even without transfers.
 if(quest_case==3){CQuestManager::CBoast accepted;accepted.ID=99;accepted.ScriptName="B";q.AcceptedBoasts.push_back(accepted);}
 unsigned initial=q.AcceptedBoasts.size();q.PostBoastTransfer();
 int additions=0;bool have_a=false,have_b=quest_case==3;for(int i=0;i<temp_count;++i)if(found&(1<<i)){++additions;if(i)have_b=true;else have_a=true;}
 check(q.TempBoasts.size()==unsigned(temp_count)&&q.AcceptedBoasts.size()==initial+additions&&q.gets.size()==unsigned(temp_count)&&q.removed.size()==unsigned(additions));
 for(int i=0;i<temp_count;++i)check(q.gets[i]==i+1);
 int ri=0;for(int i=0;i<temp_count;++i)if(found&(1<<i))check(q.removed[ri++]==i+1);
 std::list<CQuestManager::CBoast>::const_iterator it=q.AcceptedBoasts.begin();if(initial)++it;
 for(int i=0;i<temp_count;++i)if(found&(1<<i)){check(it!=q.AcceptedBoasts.end());check(it->ID==i+1&&it->ScriptName==(i?"B":"A")&&it->Failed==((before|transferred)&1?true:false)&&it->Completed==((before|transferred)&2?true:false));check(it->Cost==7&&it->Reward==11&&it->GroupIndex==13&&it->Text=="text"&&it->Title=="title"&&!it->Evil);++it;}
 bool eligible_b=(quest_case&2)&&have_b;bool eligible_a=(quest_case&1)&&have_a;bool screen=hero_state==2&&(eligible_b||eligible_a);check(hero.lookups==int(screen));check(ui.sets==int(screen&&ui_state));check(ui.ScriptName==(screen&&ui_state?(eligible_b?"B":"A"):"unchanged"));
 unsigned made=(before|transferred|mutation)?0:unsigned(additions);check(world.events.messages.size()==made+unsigned(screen&&ui_state));
 it=q.AcceptedBoasts.begin();if(initial)++it;for(unsigned j=0;j<made;++j,++it){const Message&m=world.events.messages[j];check(m.type==MESSAGE_EVENT_BOAST_MADE&&m.source==0&&reinterpret_cast<const CQuestManager::CBoast*>(m.extra)==&*it);}
 if(screen&&ui_state){const Message&m=world.events.messages.back();check(m.type==MESSAGE_EVENT_LEAVING_QUEST_START_SCREEN&&m.source==&hero&&m.extra==0);}
 check(CMainGameComponent::main.manager.gets==1&&CMainGameComponent::main.manager.player.gets==1);
 // Real helper filters non-interned equal names and clears stale output.
 std::list<CQuestManager::CBoast>result;result.push_back(CQuestManager::CBoast());q.GetAcceptedBoastsForScript(std::string(1,'A'),result);check(result.size()==unsigned(have_a));if(have_a)check(result.front().ScriptName=="A");
 ++cases;
}
check(cases==18432);std::puts("004b2890_TEST PASS");return 0;}
