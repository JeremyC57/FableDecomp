static const float DELTA_NOTIONAL_ZERO=0.0001f;
#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>
#include <string>
#include <vector>
static void check(bool ok){if(!ok)abort();}
enum E{CHECK,FAIL,ALIVE,HERO_HIT,SPECIAL,ANY,HEALTH,ATTACKER,HERO,READ_TIMER,CONVERSATION,PERSON,LINE,SET_TIMER,FRAME};
static std::vector<int> got,want;static int scenario,cut,ordinal,world,objects,timer,duration,healthReads;
static bool bit(int n){return (scenario&(1<<n))!=0;}
static int limit(){return bit(0)?(bit(1)?2:1):0;}
static bool heroHit(int f){return bit(2)^(f==1);}
static bool ability(int f){return bit(3)&&f!=1;}
static bool anyHit(int f){return bit(4)||f==1;}
static float health(bool hero){if(hero)return bit(5)?0.00011f:0.0001f;return bit(6)?1.f:(bit(8)?0.f:0.0001f);}
struct CScriptThing{int id;CScriptThing(int n=0):id(n){++objects;}CScriptThing(const CScriptThing& o):id(o.id){++objects;}~CScriptThing(){--objects;}
 bool IsAlive()const{check(id==1);got.push_back(ALIVE);return world<limit();}
 bool MsgIsHitBy(const char* n)const{check(id==1);if(*n){check(std::string(n)=="SCRIPT_NAME_HERO");got.push_back(HERO_HIT);return heroHit(world);}got.push_back(ANY);healthReads=1;return anyHit(world);}
 bool MsgIsHitByAnyAggressiveSpecialAbilityFrom(const char* n)const{check(id==1&&std::string(n)=="SCRIPT_NAME_HERO");got.push_back(SPECIAL);return ability(world);}
 CScriptThing MsgWhoHitMe()const{check(id==1);got.push_back(ATTACKER);return CScriptThing(bit(7)?2:3);}
 std::string GetDefName()const{check(id==2||id==3);return id==2?"HERO":"NPC";}};
struct GameAPI{
 void NewScriptFrame(){got.push_back(FRAME);++world;healthReads=0;check(world<=2);}
 float GetHealth(const CScriptThing& v){check(v.id==1);got.push_back(HEALTH);return health(healthReads==0);}
 CScriptThing GetHero(){got.push_back(HERO);return CScriptThing(2);}
 long AddNewConversation(const CScriptThing& v,bool a,bool b){check(v.id==1&&!a&&!b);got.push_back(CONVERSATION);return 80+world;}
 void AddPersonToConversation(long idx,const CScriptThing& v){check(idx==80+world&&v.id==2);got.push_back(PERSON);}
 void AddLineToConversation(long idx,const char* txt,bool b,const CScriptThing& v,const CScriptThing& h){check(idx==80+world&&std::string(txt)=="TEXT_QST_053_COMPLAIN"&&!b&&v.id==1&&h.id==2);got.push_back(LINE);}
} api;
struct Def{int EscortTraderAttackComplaintTime;} def;static Def* pGlobalScriptDef=&def;
struct Timer{bool operator==(int n)const{check(n==0);got.push_back(READ_TIMER);return timer==n;}Timer& operator=(int x){check(x==duration);got.push_back(SET_TIMER);timer=x;return *this;}};
struct TRetVoid{void Return(){got.push_back(FAIL);}};
namespace NScript{struct CQR_EscortTraderScript{
 bool TraderHit;long TraderHitCount;Timer TraderComplainTimer;GameAPI* Game;
 bool IsActiveThreadTerminating(){got.push_back(CHECK);return ++ordinal==cut;}
 void WatchForTraderHit(CScriptThing trader);
};}
void NScript::CQR_EscortTraderScript::WatchForTraderHit(CScriptThing trader)
{
    TRetVoid _Fail_Val;
    if (false) {} else for (;; Game->NewScriptFrame()) {
        if (IsActiveThreadTerminating()) { _Fail_Val.Return(); return; }
        if (trader.IsAlive()) {
            if (IsActiveThreadTerminating()) { _Fail_Val.Return(); return; }
            if (trader.MsgIsHitBy("SCRIPT_NAME_HERO") ||
                trader.MsgIsHitByAnyAggressiveSpecialAbilityFrom("SCRIPT_NAME_HERO")) {
                if (IsActiveThreadTerminating()) { _Fail_Val.Return(); return; }
                if (Game->GetHealth(trader) > DELTA_NOTIONAL_ZERO) {
                    if (IsActiveThreadTerminating()) { _Fail_Val.Return(); return; }
                    TraderHit = true;
                }
            }
            if (trader.MsgIsHitBy("")) {
                if (IsActiveThreadTerminating()) { _Fail_Val.Return(); return; }
                if (Game->GetHealth(trader) > DELTA_NOTIONAL_ZERO) {
                    if (IsActiveThreadTerminating()) { _Fail_Val.Return(); return; }
                    CScriptThing Attacker = trader.MsgWhoHitMe();
                    if (Attacker.GetDefName() != Game->GetHero().GetDefName()) {
                        if (IsActiveThreadTerminating()) { _Fail_Val.Return(); return; }
                        ++TraderHitCount;
                        if (TraderComplainTimer == 0) {
                            if (IsActiveThreadTerminating()) { _Fail_Val.Return(); return; }
                            long HLStyleConversationIndex = Game->AddNewConversation(trader, false, false);
                            Game->AddPersonToConversation(HLStyleConversationIndex, Game->GetHero());
                            Game->AddLineToConversation(HLStyleConversationIndex, "TEXT_QST_053_COMPLAIN", false, trader, Game->GetHero());
                            TraderComplainTimer = pGlobalScriptDef->EscortTraderAttackComplaintTime;
                        }
                    }
                }
            }
        } else {
            if (IsActiveThreadTerminating()) { _Fail_Val.Return(); return; }
            return;
        }
    }
    if (IsActiveThreadTerminating()) { _Fail_Val.Return(); return; }
}
static jmp_buf halt;static int checkpoints,ecount,etime;static bool ehit;
static void term(){want.push_back(CHECK);if(++checkpoints==cut){want.push_back(FAIL);longjmp(halt,1);}}
static void oracle(){
 for(int f=0;;++f){
  term();want.push_back(ALIVE);if(f>=limit()){term();return;}
  term();want.push_back(HERO_HIT);bool hit=heroHit(f);if(!hit){want.push_back(SPECIAL);hit=ability(f);}
  if(hit){term();want.push_back(HEALTH);if(health(true)>0.0001f){term();ehit=true;}}
  want.push_back(ANY);
  if(anyHit(f)){term();want.push_back(HEALTH);if(health(false)>0.0001f){term();want.push_back(ATTACKER);want.push_back(HERO);
   if(!bit(7)){term();++ecount;want.push_back(READ_TIMER);if(!etime){term();want.push_back(CONVERSATION);want.push_back(HERO);want.push_back(PERSON);want.push_back(HERO);want.push_back(LINE);want.push_back(SET_TIMER);etime=duration;}}
  }}
  want.push_back(FRAME);
 }
}
int main(){
 for(scenario=0;scenario<2048;++scenario)for(cut=0;cut<32;++cut){
  got.clear();want.clear();objects=world=ordinal=checkpoints=healthReads=0;duration=def.EscortTraderAttackComplaintTime=bit(8)?0:9;
  NScript::CQR_EscortTraderScript s;s.Game=&api;etime=timer=bit(9)?5:0;ehit=s.TraderHit=bit(10);ecount=s.TraderHitCount=3;
  if(setjmp(halt)==0)oracle();
  {CScriptThing trader(1);s.WatchForTraderHit(trader);check(objects==1);}
  check(objects==0&&got==want&&s.TraderHit==ehit&&s.TraderHitCount==ecount&&timer==etime);
 }
 puts("00e20210_TEST PASS");return 0;
}