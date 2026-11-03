#include <cassert>
#include <cstdio>
#include <string>
#include <vector>
static int strings_alive=0,things_alive=0;
static std::vector<std::string> trace;
static std::string number(long n){char b[40];std::sprintf(b,"%ld",n);return b;}
static void event(const std::string&s){trace.push_back(s);}
class CCharString {
public: std::string s;
 CCharString(const char*p=""):s(p){++strings_alive;} CCharString(const std::string&p):s(p){++strings_alive;}
 CCharString(const CCharString&o):s(o.s){++strings_alive;} ~CCharString(){--strings_alive;}
 bool operator==(const char*p)const{return s==p;} bool operator!=(const char*p)const{return s!=p;}
};
CCharString operator+(const char*a,const CCharString&b){return CCharString(a+b.s);}
CCharString operator+(const CCharString&a,const char*b){return CCharString(a.s+b);}
CCharString operator+(const CCharString&a,const CCharString&b){return CCharString(a.s+b.s);}
struct State { bool alive;std::string data;State():alive(false){} };
static State states[100];
class CScriptThing {
public: long id;
 CScriptThing(long n=0):id(n){++things_alive;} CScriptThing(const CScriptThing&o):id(o.id){++things_alive;}
 ~CScriptThing(){--things_alive;} CScriptThing&operator=(const CScriptThing&o){id=o.id;return *this;}
 bool IsAlive()const{event("alive:"+number(id));return states[id].alive;}
 bool IsDead()const{return !IsAlive();}
 CCharString GetDataString()const{event("data:"+number(id));return CCharString(states[id].data);}
 bool operator==(const CScriptThing&o)const{event("equal:"+number(id)+":"+number(o.id));return id==o.id;}
 bool operator!=(const CScriptThing&o)const{return !(*this==o);}
};
template<class T> class TRet { T val;public:TRet(const T&v):val(v){}T Return(){return val;} };
struct CTimer {
 long time;CTimer():time(0){}
 bool operator>(long n)const{event("timer-read");return time>n;}
 CTimer&operator=(long n){event("timer-set:"+number(n));time=n;return *this;}
};
struct CScriptDef { long TE_TraderCommentDelay; };
static CScriptDef definition;static CScriptDef*pGlobalScriptDef=&definition;
struct Line {
 std::string text;long who,listener;
 Line(const std::string&t,long w,long l):text(t),who(w),listener(l){}
 bool operator==(const Line&o)const{return text==o.text&&who==o.who&&listener==o.listener;}
};
struct CGameScriptInterfaceBase {
 CScriptThing hero;long fallback,nearby,furthest;bool entry_exists;int sound,pause,conversations;long center;std::vector<long> people;std::vector<Line> lines;
 CGameScriptInterfaceBase():hero(99),fallback(2),nearby(3),furthest(4),entry_exists(false),sound(0),pause(0),conversations(0),center(-1){}
 CScriptThing&GetHero(){event("hero");return hero;}
 CScriptThing GetNearestWithScriptName(const CScriptThing&t,const CCharString&n)const{assert(n.s=="DarkwoodTrader");event("nearest:"+number(t.id));return CScriptThing(t.id==99?fallback:nearby);}
 CScriptThing GetFurthestWithScriptName(const CScriptThing&t,const CCharString&n)const{assert(n.s=="DarkwoodTrader");event("furthest:"+number(t.id));return CScriptThing(furthest);}
 CScriptThing GetThingWithScriptName(const CCharString&n)const{assert(n.s=="RockTrollTrigger");event("rock-trigger");return CScriptThing(88);}
 unsigned long PlayCriteriaSoundOnThing(const CScriptThing&t,const CCharString&n){assert(t.id==88&&n.s=="EARTH_TROLL_OFFSCREEN_ROAR");event("sound");++sound;return 1;}
 void Pause(float n){assert(n==1);event("pause");++pause;}
 long AddNewConversation(const CScriptThing&t,bool a,bool b){assert(!a&&!b);event("new:"+number(t.id));++conversations;center=t.id;return 47;}
 void AddPersonToConversation(long n,const CScriptThing&t){assert(n==47);event("person:"+number(t.id));people.push_back(t.id);}
 void AddLineToConversation(long n,const CCharString&s,bool a,const CScriptThing&w,const CScriptThing&l){assert(n==47&&!a&&l.id==99);event("line:"+s.s+":"+number(w.id));lines.push_back(Line(s.s,w.id,l.id));}
 bool TextEntryExists(const CCharString&s){event("exists:"+s.s);return entry_exists;}
};
namespace NScript { class CQ_TraderEscortScript {
public:
 enum ETraderComment { TRADER_COMMENT_SINGLE=0,TRADER_COMMENT_MULTIPLE=1,TRADER_COMMENT_ATTACKED_SPEECH=2 };
 CGameScriptInterfaceBase&Game;CTimer CommentTimer;int fail_at,checks;
 CQ_TraderEscortScript(CGameScriptInterfaceBase&g):Game(g),fail_at(0),checks(0){}
 bool IsActiveThreadTerminating(){event("check");return ++checks==fail_at;}
 bool MakeTraderComment(const CCharString&,const CScriptThing&,ETraderComment);
}; }
bool NScript::CQ_TraderEscortScript::MakeTraderComment(const CCharString& comment_to_make,const CScriptThing& speaker,ETraderComment comment_type)
{
    TRet<bool> _Fail_Val(false);
    CScriptThing Trader=speaker;
    if(Trader.IsDead()) {
        if(IsActiveThreadTerminating()) return _Fail_Val.Return();
        Trader=Game.GetNearestWithScriptName(Game.GetHero(),"DarkwoodTrader");
    }
    if(Trader.IsDead()) {
        if(IsActiveThreadTerminating()) return _Fail_Val.Return();
        return false;
    }
    if(comment_type==TRADER_COMMENT_ATTACKED_SPEECH) {
        if(IsActiveThreadTerminating()) return _Fail_Val.Return();
        CScriptThing TraderA=Game.GetNearestWithScriptName(Trader,"DarkwoodTrader");
        CScriptThing TraderB=Game.GetFurthestWithScriptName(Trader,"DarkwoodTrader");
        if(TraderA==TraderB) {
            if(IsActiveThreadTerminating()) return _Fail_Val.Return();
            TraderB=CScriptThing();
        }
        if(TraderA.GetDataString()!="SCARED") {
            if(IsActiveThreadTerminating()) return _Fail_Val.Return();
            CScriptThing temp=TraderA;
            TraderA=TraderB;
            TraderB=temp;
        }
        if(comment_to_make=="ROCK_TROLL_CLOSE") {
            if(IsActiveThreadTerminating()) return _Fail_Val.Return();
            Game.PlayCriteriaSoundOnThing(Game.GetThingWithScriptName("RockTrollTrigger"),"EARTH_TROLL_OFFSCREEN_ROAR");
            Game.Pause(1.0f);
        }
        long HLStyleConversationIndex=Game.AddNewConversation(speaker,false,false);
        Game.AddPersonToConversation(HLStyleConversationIndex,Game.GetHero());
        if(TraderA.IsAlive()) {
            if(IsActiveThreadTerminating()) return _Fail_Val.Return();
            Game.AddPersonToConversation(HLStyleConversationIndex,TraderA);
            if(TraderB.IsAlive()) {
                if(IsActiveThreadTerminating()) return _Fail_Val.Return();
                Game.AddPersonToConversation(HLStyleConversationIndex,TraderB);
                Game.AddLineToConversation(HLStyleConversationIndex,"TEXT_QST_067_"+Trader.GetDataString()+"_"+comment_to_make,false,Trader,Game.GetHero());
                Game.AddLineToConversation(HLStyleConversationIndex,"TEXT_QST_067_"+TraderA.GetDataString()+"_INTRO_RESPONSE_NO",false,TraderA,Game.GetHero());
                Game.AddLineToConversation(HLStyleConversationIndex,"TEXT_QST_067_"+TraderB.GetDataString()+"_INTRO_RESPONSE_OATH",false,TraderB,Game.GetHero());
                Game.AddLineToConversation(HLStyleConversationIndex,"TEXT_QST_067_"+TraderA.GetDataString()+"_INTRO_RESPONSE_BAD_NEWS",false,TraderA,Game.GetHero());
                Game.AddLineToConversation(HLStyleConversationIndex,"TEXT_QST_067_"+Trader.GetDataString()+"_INTRO_RESPONSE_REWARD",false,Trader,Game.GetHero());
            } else {
                if(IsActiveThreadTerminating()) return _Fail_Val.Return();
                Game.AddLineToConversation(HLStyleConversationIndex,"TEXT_QST_067_"+Trader.GetDataString()+"_"+comment_to_make,false,Trader,Game.GetHero());
                Game.AddLineToConversation(HLStyleConversationIndex,"TEXT_QST_067_"+TraderA.GetDataString()+"_INTRO_RESPONSE_NO",false,TraderA,Game.GetHero());
                Game.AddLineToConversation(HLStyleConversationIndex,"TEXT_QST_067_"+Trader.GetDataString()+"_INTRO_RESPONSE_REWARD",false,Trader,Game.GetHero());
            }
        }
        CommentTimer=pGlobalScriptDef->TE_TraderCommentDelay;
        return true;
    }
    if(CommentTimer>0) {
        if(IsActiveThreadTerminating()) return _Fail_Val.Return();
        return false;
    }
    long HLStyleConversationIndex=Game.AddNewConversation(Trader,false,false);
    Game.AddPersonToConversation(HLStyleConversationIndex,Game.GetHero());
    Game.AddLineToConversation(HLStyleConversationIndex,"TEXT_QST_067_"+Trader.GetDataString()+"_"+comment_to_make,false,Trader,Game.GetHero());
    if(comment_type==TRADER_COMMENT_MULTIPLE) {
        if(IsActiveThreadTerminating()) return _Fail_Val.Return();
        CScriptThing near_trader=Game.GetNearestWithScriptName(Trader,"DarkwoodTrader");
        if(near_trader.IsAlive() && near_trader!=Trader && Game.TextEntryExists("TEXT_QST_067_"+near_trader.GetDataString()+"_"+comment_to_make+"_RESPONSE")) {
            if(IsActiveThreadTerminating()) return _Fail_Val.Return();
            Game.AddPersonToConversation(HLStyleConversationIndex,near_trader);
            Game.AddLineToConversation(HLStyleConversationIndex,"TEXT_QST_067_"+near_trader.GetDataString()+"_"+comment_to_make+"_RESPONSE",false,near_trader,Game.GetHero());
        }
    }
    CommentTimer=pGlobalScriptDef->TE_TraderCommentDelay;
    return true;
}

struct Expected { bool result;long center;int sound;std::vector<long>people;std::vector<Line>lines;Expected():result(false),center(-1),sound(0){} };
static std::string key(long id,const std::string&ending){return "TEXT_QST_067_"+states[id].data+ending;}
static Expected expected(long speaker,const CGameScriptInterfaceBase&g,int type,long timer,const std::string&comment){
 Expected e;const long trader=states[speaker].alive?speaker:g.fallback;if(!states[trader].alive)return e;
 if(type!=2&&timer>0)return e;e.result=true;e.center=type==2?speaker:trader;e.people.push_back(99);
 if(type==2){
  long actors[2]={g.nearby,g.furthest};if(actors[0]==actors[1])actors[1]=0;
  if(states[actors[0]].data!="SCARED"){long n=actors[0];actors[0]=actors[1];actors[1]=n;}
  e.sound=comment=="ROCK_TROLL_CLOSE"?1:0;if(!states[actors[0]].alive)return e;
  e.people.push_back(actors[0]);if(states[actors[1]].alive)e.people.push_back(actors[1]);
  e.lines.push_back(Line(key(trader,"_"+comment),trader,99));e.lines.push_back(Line(key(actors[0],"_INTRO_RESPONSE_NO"),actors[0],99));
  if(states[actors[1]].alive){e.lines.push_back(Line(key(actors[1],"_INTRO_RESPONSE_OATH"),actors[1],99));e.lines.push_back(Line(key(actors[0],"_INTRO_RESPONSE_BAD_NEWS"),actors[0],99));}
  e.lines.push_back(Line(key(trader,"_INTRO_RESPONSE_REWARD"),trader,99));
 }else{
  e.lines.push_back(Line(key(trader,"_"+comment),trader,99));long near=g.nearby;
  if(type==1&&states[near].alive&&near!=trader&&g.entry_exists){e.people.push_back(near);e.lines.push_back(Line(key(near,"_"+comment+"_RESPONSE"),near,99));}
 }
 return e;
}
int main(){
 unsigned runs=0;
 for(unsigned scenario=0;scenario<3072;++scenario){
  states[0].alive=false;states[0].data="";states[99].alive=true;states[99].data="HERO";
  for(unsigned j=1;j<=4;++j){states[j].alive=(scenario&(1u<<(j-1)))!=0;states[j].data=(scenario&(1u<<(j+2)))?"SCARED":"BOLD";}
  long speaker=scenario%17?1:0;int type=scenario%3;long timer=(scenario/3)%3;timer=timer==0?-1:timer==1?0:7;definition.TE_TraderCommentDelay=static_cast<long>(scenario%5)-1;
  const std::string comment=(scenario&256)?"ROCK_TROLL_CLOSE":scenario%7?"BANTER":"";
  std::vector<std::string>baseline;bool base_result=false;int available_checks=0;
  for(int cancel=0;cancel<=12;++cancel){
   CGameScriptInterfaceBase game;game.fallback=scenario%19?2:0;game.nearby=scenario%23?3:0;game.furthest=scenario%29?4:0;
   if(scenario&128)game.nearby=states[speaker].alive?speaker:game.fallback;if(scenario&64)game.furthest=game.nearby;game.entry_exists=(scenario&512)!=0;
   NScript::CQ_TraderEscortScript script(game);script.CommentTimer.time=timer;script.fail_at=cancel;
   CScriptThing argument(speaker);CCharString text(comment);int before_strings=strings_alive,before_things=things_alive;trace.clear();
   Expected e=expected(speaker,game,type,timer,comment);bool result=script.MakeTraderComment(text,argument,static_cast<NScript::CQ_TraderEscortScript::ETraderComment>(type));++runs;
   assert(strings_alive==before_strings&&things_alive==before_things&&argument.id==speaker&&text.s==comment);
   if(cancel==0){
    assert(result==e.result&&game.center==e.center&&game.sound==e.sound&&game.pause==e.sound);
    assert(game.conversations==(e.result?1:0)&&game.people==e.people&&game.lines==e.lines);
    assert(script.CommentTimer.time==(e.result?definition.TE_TraderCommentDelay:timer));baseline=trace;base_result=result;available_checks=script.checks;
   }else if(cancel<=available_checks){
    assert(!result&&script.checks==cancel&&script.CommentTimer.time==timer);unsigned end=0;int seen=0;
    for(;end<baseline.size();++end)if(baseline[end]=="check"&&++seen==cancel){++end;break;}
    assert(trace.size()==end);for(unsigned i=0;i<end;++i)assert(trace[i]==baseline[i]);
   }else{assert(result==base_result&&trace==baseline);assert(script.CommentTimer.time==(e.result?definition.TE_TraderCommentDelay:timer));}
  }
  assert(strings_alive==0&&things_alive==0);
 }
 assert(runs==39936);std::puts("00e01900_TEST PASS");return 0;
}
