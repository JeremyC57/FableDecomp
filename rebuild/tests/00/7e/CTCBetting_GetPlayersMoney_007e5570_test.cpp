// Selected target and original GetHeroStats expression copied; collaborators are observable test fixtures.
#include <stdio.h>
#include <stdlib.h>
#include <string>
#define CHECK(c) do{if(!(c)){printf("FAIL %d\n",__LINE__);abort();}}while(0)
static int liveStrings,heroReads,definitionReads,textCalls,numberCalls,joinCalls;
class CWideString {public:std::wstring Text;CWideString(const wchar_t* p):Text(p){++liveStrings;}CWideString(const CWideString& p):Text(p.Text){++liveStrings;}~CWideString(){--liveStrings;}};
class CTCAIScratchpad {public:static bool BettingActive,CountBetMoneyDown;static long BettingAmount;};
bool CTCAIScratchpad::BettingActive;bool CTCAIScratchpad::CountBetMoneyDown;long CTCAIScratchpad::BettingAmount;
class CBettingDef {public:mutable int References;unsigned long PlayersMoney;};
template<class T>class CDefPointer {public:T* Object;CDefPointer():Object(0){}CDefPointer(const CDefPointer& p):Object(p.Object){if(Object)++Object->References;}~CDefPointer(){if(Object)--Object->References;}T* operator->()const{return Object;}};
class CThingBaseDef {public:bool Found;CBettingDef* Betting;bool GetSubDef(CDefPointer<const CBettingDef>* out)const{++definitionReads;if(!Found)return false;CHECK(!out->Object);out->Object=Betting;++Betting->References;return true;}};
class CTCHeroStats {public:long Money;long GetMoney()const{++heroReads;return Money;}};
class CThing {public:CThingBaseDef* Definition;const CThingBaseDef& PeekBaseDef()const{return *Definition;}};
class CThingPlayerCreature {public:CTCHeroStats* Stats;bool GetTC(CTCHeroStats** out){*out=Stats;return true;}};
class CPlayer {public:CThingPlayerCreature* Hero;CThingPlayerCreature* GetPControlledCreature()const{return Hero;}};
class CPlayerManager {public:CPlayer Player;const CPlayer& GetMainPlayer()const{return Player;}};
class CMainGameComponent {public:static CMainGameComponent* Main;CPlayerManager Manager;static CMainGameComponent* Get(){return Main;}const CPlayerManager& PeekPlayerManager()const{return Manager;}};
CMainGameComponent* CMainGameComponent::Main;
class CTCBetting {public:CThing* ParentThing;unsigned long LastTextId;CThing& GetParentThing(){return *ParentThing;}void GetHeroStats(CTCHeroStats**);CWideString GetPlayersMoney();CWideString GetText(unsigned long id){++textCalls;LastTextId=id;return CWideString(id==91?L"Coins: ":L"Gold: ");}};
inline void CTCBetting::GetHeroStats(CTCHeroStats** ptc_hero_stats)
{
 CMainGameComponent::Get()->PeekPlayerManager().GetMainPlayer().GetPControlledCreature()->GetTC(ptc_hero_stats);
}
CWideString GFIntToWideString(long number){++numberCalls;char bytes[64];sprintf(bytes,"%ld",number);std::wstring text;for(int i=0;bytes[i];++i)text+=(wchar_t)bytes[i];return CWideString(text.c_str());}
CWideString operator+(const CWideString& a,const CWideString& b){++joinCalls;std::wstring text=a.Text+b.Text;return CWideString(text.c_str());}
CWideString CTCBetting::GetPlayersMoney()
{
 CTCHeroStats* ptc_hero_stats=0;
 GetHeroStats(&ptc_hero_stats);
 CDefPointer<const CBettingDef> betting_def;
 long money=ptc_hero_stats->GetMoney();
 if(CTCAIScratchpad::CountBetMoneyDown)money-=CTCAIScratchpad::BettingAmount;
 if(CTCAIScratchpad::BettingActive){
  if(GetParentThing().PeekBaseDef().GetSubDef(&betting_def))
   return GetText(betting_def->PlayersMoney)+GFIntToWideString(money);
 }
 return CWideString(L"");
}

int main(){
 CMainGameComponent game;CMainGameComponent::Main=&game;CThingPlayerCreature hero;CTCHeroStats stats;
 hero.Stats=&stats;game.Manager.Player.Hero=&hero;
 CBettingDef def;def.References=1;CThingBaseDef base;base.Betting=&def;CThing parent;parent.Definition=&base;CTCBetting betting;betting.ParentThing=&parent;
 long amounts[]={-100,0,1,123,100000},stakes[]={-30,0,30};int cases=0;
 for(int m=0;m<5;++m)for(int s=0;s<3;++s)for(int active=0;active<2;++active)for(int count=0;count<2;++count)for(int found=0;found<2;++found)for(int repeat=0;repeat<2;++repeat){
  stats.Money=amounts[m];CTCAIScratchpad::BettingAmount=stakes[s];CTCAIScratchpad::BettingActive=active!=0;CTCAIScratchpad::CountBetMoneyDown=count!=0;base.Found=found!=0;def.PlayersMoney=repeat?91:777;
  heroReads=definitionReads=textCalls=numberCalls=joinCalls=0;betting.LastTextId=0;
  CHECK(liveStrings==0);{
   CWideString result=betting.GetPlayersMoney();CHECK(liveStrings==1);++cases;
   if(active&&found){
    char number[64];sprintf(number,"%ld",amounts[m]-(count?stakes[s]:0));std::wstring expected=repeat?L"Coins: ":L"Gold: ";for(int j=0;number[j];++j)expected+=(wchar_t)number[j];
    CHECK(result.Text==expected&&betting.LastTextId==def.PlayersMoney);
   }else CHECK(result.Text.empty()&&betting.LastTextId==0);
  }
  CHECK(liveStrings==0&&def.References==1&&heroReads==1&&definitionReads==active);
  CHECK(textCalls==(active&&found)&&numberCalls==textCalls&&joinCalls==textCalls);
  CHECK(stats.Money==amounts[m]&&CTCAIScratchpad::BettingAmount==stakes[s]);
 }
 CHECK(cases==240);puts("007e5570_TEST PASS");return 0;
}