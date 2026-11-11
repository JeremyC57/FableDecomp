#include <string>
#include <list>
#include <vector>
#include <functional>
#include <cstdio>
#include <cstdlib>
#include <cstring>
static void check(bool b){if(!b){std::puts("004b2280_TEST FAIL");std::exit(1);}}
static std::string trace;
static void note(char c){trace+=c;}
typedef std::string CCharString;
class CWideString{ public:int value;};
class C3DVector{ public:float X,Y,Z;C3DVector(float x,float y,float z):X(x),Y(y),Z(z){}};
enum EMessageEventType{MESSAGE_EVENT_QUEST_FAILED_BEFORE_SCREEN_SHOWN=54,MESSAGE_EVENT_QUEST_FAILED=52};
class CTCQuestCard{ public:bool Finished;void SetAsFinished(bool value){note('F');Finished=value;}};
class CTCHeroStats{ public:long NumFailedAQuest;long GetNumFailedAQuest()const{return NumFailedAQuest;}void SetNumFailedAQuest(long n){note('N');NumFailedAQuest=n;}};
static CCharString expected_name;static const CWideString*expected_reason;static bool expected_offer;
class CTCQuestCompletionUI{ public:enum CompletionType{TEST_NONE=0};int calls;void Activate(CompletionType t,const CCharString&name,bool b,const CWideString&reason,bool offer){note('U');++calls;check(int(t)==2&&name==expected_name&&!b&&&reason==expected_reason&&offer==expected_offer);}};
class CDefString{ public:CCharString name;CDefString(const CCharString&s):name(s){}operator CCharString()const{note('S');return name;}};
class CThing{ public:bool dead;CTCQuestCard*card;CTCHeroStats*stats;CTCQuestCompletionUI*ui;CCharString def;int lookup_card,lookup_stats,lookup_ui;
 bool IsDead()const{return dead;}
 bool GetTC(CTCQuestCard**p){note('c');++lookup_card;*p=card;return true;}
 bool GetTC(CTCHeroStats**p){note('h');++lookup_stats;*p=stats;return true;}
 bool GetTC(CTCQuestCompletionUI**p){note('u');++lookup_ui;*p=ui;return true;}
 CDefString GetDefName()const{note('D');return CDefString(def);}
};
class CThingPlayerCreature:public CThing{ public:};
template<class T>bool IS_THING_ALIVE(T p){return p&&!p->IsDead();}
class CPlayer{ public:CThingPlayerCreature*hero;CThingPlayerCreature*GetPControlledCreature()const{note('P');return hero;}};
class CPlayerManager{ public:CPlayer player;CPlayer&GetMainPlayer(){note('M');return player;}};
class CMainGameComponent{ public:CPlayerManager manager;static CMainGameComponent main;static CMainGameComponent*Get(){return &main;}CPlayerManager&GetPlayerManager(){return manager;}};CMainGameComponent CMainGameComponent::main;
struct Event{EMessageEventType type;long data;};
class CMessageEventManager{ public:std::vector<Event>events;void AddMessage(EMessageEventType t,unsigned long delay,const C3DVector&pos,CThing*a,CThing*b,float f,float g,long data){check(delay==50&&pos.X==0&&pos.Y==0&&pos.Z==0&&!a&&!b&&f==0&&g==0);Event e;e.type=t;e.data=data;events.push_back(e);note(t==54?'B':'E');}};
class CWorld{ public:CMessageEventManager events;CMessageEventManager&GetMessageEventManager(){return events;}};
template<class T> struct TestList:public std::list<T>{template<class Pred>void remove_if(Pred pred){typename std::list<T>::iterator it=this->begin();while(it!=this->end()){if(pred(*it))it=this->erase(it);else ++it;}}};
class CQuestManager{ public:class CQuestCard{ public:CCharString DefName,ScriptName;bool Replayable;long FeatIndex;};
 TestList<CQuestCard>AvailableQuestCardTypes;bool QuestCardsNeedUpdating;CWorld&World;long index;CThing*active_card;int finished,retired;CQuestManager(CWorld&w):QuestCardsNeedUpdating(false),World(w),index(-1),active_card(0),finished(0),retired(0){}
 long GetIndexFromRegisteredQuest(const CCharString&name)const{note('I');check(name==expected_name);return index;}
 void SetQuestCardsNeedUpdating(bool value){note('Q');QuestCardsNeedUpdating=value;}
 bool OnQuestFinishScreenDisplayed(const CCharString&name){note('O');check(name==expected_name);++finished;return false;}
 CThing*GetActiveQuestCardFromScriptName(const CCharString&name)const{note('A');check(name==expected_name);return active_card;}
 void RemoveActiveQuestCardAndMakeAvailableAgain(const CCharString&name,bool b){note('R');check(name==expected_name&&!b);++retired;}
 void RemoveAvailableQuestCardType(const CCharString&);
 bool SetQuestAsFailed(const CCharString&,bool,const CWideString&,bool);
};
class CIsQuestCardWithDefName:public std::unary_function<CQuestManager::CQuestCard,bool>{ public:const CCharString&DefName;CIsQuestCardWithDefName(const CCharString&s):DefName(s){}bool operator()(const CQuestManager::CQuestCard&c)const{return c.DefName==DefName;}};
void CQuestManager::RemoveAvailableQuestCardType(const CCharString& def_name)
{
 AvailableQuestCardTypes.remove_if(CIsQuestCardWithDefName(def_name));
 SetQuestCardsNeedUpdating(true);
}
bool CQuestManager::SetQuestAsFailed(const CCharString& script_name,bool display_screen,const CWideString& reason,bool offer_teleport_to_guild)
{
 long index=GetIndexFromRegisteredQuest(script_name);
 if(index>-1) {
  World.GetMessageEventManager().AddMessage(MESSAGE_EVENT_QUEST_FAILED_BEFORE_SCREEN_SHOWN,50,C3DVector(0,0,0),0,0,0,0,index);
  if(!display_screen)
   World.GetMessageEventManager().AddMessage(MESSAGE_EVENT_QUEST_FAILED,50,C3DVector(0,0,0),0,0,0,0,index);
  CThingPlayerCreature* phero=CMainGameComponent::Get()->GetPlayerManager().GetMainPlayer().GetPControlledCreature();
  if(IS_THING_ALIVE(phero)) {
   if(display_screen) {
    CTCQuestCompletionUI* ptc_ui;
    phero->GetTC(&ptc_ui);
    ptc_ui->Activate((CTCQuestCompletionUI::CompletionType)2,script_name,false,reason,offer_teleport_to_guild);
   } else OnQuestFinishScreenDisplayed(script_name);
   CThing* pquest_card=GetActiveQuestCardFromScriptName(script_name);
   if(pquest_card) {
    CTCQuestCard* ptc_quest_card;
    pquest_card->GetTC(&ptc_quest_card);
    if(ptc_quest_card)ptc_quest_card->SetAsFinished(true);
    CTCHeroStats* ptc_hero_stats;
    phero->GetTC(&ptc_hero_stats);
    ptc_hero_stats->SetNumFailedAQuest(ptc_hero_stats->GetNumFailedAQuest()+1);
    RemoveAvailableQuestCardType(pquest_card->GetDefName());
   }
   RemoveActiveQuestCardAndMakeAvailableAgain(script_name,false);
  }
  return true;
 }
 return false;
}

int main(){unsigned cases=0;long indices[]={-2147483647L-1,-1,0,7,2147483647L};long counts[]={-2147483647L-1,-1,0,2147483646L};
for(int ix=0;ix<5;++ix)for(int show=0;show<2;++show)for(int offer=0;offer<2;++offer)for(int hero_state=0;hero_state<3;++hero_state)for(int has_card=0;has_card<2;++has_card)for(int has_tc=0;has_tc<2;++has_tc)for(int count=0;count<4;++count)for(int matches=0;matches<3;++matches)for(int original_flag=0;original_flag<2;++original_flag){
 CWorld world;CQuestManager q(world);q.index=indices[ix];q.QuestCardsNeedUpdating=original_flag!=0;
 CTCQuestCard quest_component;quest_component.Finished=false;CTCHeroStats stats;stats.NumFailedAQuest=counts[count];CTCQuestCompletionUI ui;ui.calls=0;
 CThing card;card.dead=false;card.card=has_tc?&quest_component:0;card.stats=0;card.ui=0;card.def="CARD";card.lookup_card=card.lookup_stats=card.lookup_ui=0;
 CThingPlayerCreature hero;hero.dead=hero_state==1;hero.card=0;hero.stats=&stats;hero.ui=&ui;hero.lookup_card=hero.lookup_stats=hero.lookup_ui=0;
 CMainGameComponent::main.manager.player.hero=hero_state?&hero:0;q.active_card=has_card?&card:0;
 for(int i=0;i<5;++i){CQuestManager::CQuestCard entry;entry.DefName=i<matches?"CARD":"OTHER";entry.ScriptName="unchanged";entry.Replayable=(i&1)!=0;entry.FeatIndex=i;q.AvailableQuestCardTypes.push_back(entry);}
 CWideString reason;reason.value=91;expected_reason=&reason;expected_name="QUEST";expected_offer=offer!=0;trace="";bool result=q.SetQuestAsFailed(expected_name,show!=0,reason,offer!=0);
 bool known=q.index>-1,alive=hero_state==2,work=known&&alive,update=work&&has_card;
 check(result==known&&reason.value==91);check(world.events.events.size()==unsigned(known?(show?1:2):0));if(known){check(world.events.events[0].type==54&&world.events.events[0].data==q.index);if(!show)check(world.events.events[1].type==52&&world.events.events[1].data==q.index);}
 check(ui.calls==int(work&&show)&&q.finished==int(work&&!show)&&q.retired==int(work));check(quest_component.Finished==(update&&has_tc));check(stats.NumFailedAQuest==counts[count]+int(update));check(q.QuestCardsNeedUpdating==(original_flag||update));check(q.AvailableQuestCardTypes.size()==unsigned(5-(update?matches:0)));
 int first=update?matches:0;for(std::list<CQuestManager::CQuestCard>::const_iterator it=q.AvailableQuestCardTypes.begin();it!=q.AvailableQuestCardTypes.end();++it,++first)check(it->FeatIndex==first&&it->ScriptName=="unchanged"&&it->Replayable==((first&1)!=0)&&it->DefName==(first<matches?"CARD":"OTHER"));
 check(card.lookup_card==int(update)&&hero.lookup_stats==int(update)&&hero.lookup_ui==int(work&&show));std::string expected="I";if(known){expected+="B";if(!show)expected+="E";expected+="MP";if(alive){expected+=show?"uU":"O";expected+="A";if(has_card){expected+="c";if(has_tc)expected+="F";expected+="hNDSQ";}expected+="R";}}check(trace==expected);++cases;
}
check(cases==5760);std::puts("004b2280_TEST PASS");return 0;}