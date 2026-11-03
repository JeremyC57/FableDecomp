#include <cstdio>
#include <cassert>
#include <string>
#include <vector>
#include <cmath>
struct State {int frame,limit,checks,stopCheck,trainingAt,lightningAt,meleeAt,conversations,pauses;long timerEnd;int registered,deregistered;bool apprenticeAlive;float angle;int things,strings;bool*training;std::vector<float>angles;std::vector<bool>targets;std::vector<std::string>animations,text;std::vector<long>timers;State():timerEnd(0),registered(0),deregistered(0),frame(0),limit(35),checks(0),stopCheck(0),trainingAt(2),lightningAt(-1),meleeAt(-1),conversations(0),pauses(0),apprenticeAlive(true),angle(0.125f),things(0),strings(0),training(0){} };
State*state;
class CCharString {public:std::string text;CCharString(const char*s):text(s){++state->strings;}CCharString(const CCharString&s):text(s.text){++state->strings;}~CCharString(){--state->strings;}};
enum EHeroAbility {HERO_ABILITY_LIGHTNING_SPELL=11};class TRetVoid{public:void Return(){}};
class CScriptThing {public:int kind;CScriptThing(int k=0):kind(k){++state->things;}CScriptThing(const CScriptThing&s):kind(s.kind){++state->things;}~CScriptThing(){--state->things;}float GetAngleXY()const{assert(kind==0);return state->angle;}
 bool MsgIsHitBy(const CCharString&s)const{assert(s.text=="SCRIPT_NAME_HERO");return state->frame==state->meleeAt;}
 bool MsgIsHitBySpecialAbilityFrom(EHeroAbility e,const CCharString&s)const{assert(e==HERO_ABILITY_LIGHTNING_SPELL&&s.text=="SCRIPT_NAME_HERO");return state->frame==state->lightningAt;}
 bool IsAlive()const{assert(kind==2);return state->apprenticeAlive;}
};
class CGameScriptInterfaceBase;extern CGameScriptInterfaceBase* pDSTGame;class CTimer {long TimerIndex;public:CTimer();~CTimer();CTimer& operator=(long);long GetTime()const;bool operator<=(long)const;};
class CScriptDef {public:float GUI_WillDummySpinTimer;};template<class T>class CDefPointer {public:T*Object;T*operator->()const{return Object;}};CDefPointer<const CScriptDef>pGlobalScriptDef;
class CGameScriptInterfaceBase {public:CScriptThing hero;CGameScriptInterfaceBase():hero(1){}
 long RegisterTimer(){++state->registered;return 0;}
 void DeregisterTimer(long n){assert(n==0);++state->deregistered;}
 void SetTimer(long i,long n){assert(i==0);state->timers.push_back(n);state->timerEnd=state->frame+n;}
 long GetTimer(long i)const{assert(i==0);return state->timerEnd-state->frame;}
 void NewScriptFrame(){assert(state->frame<255);++state->frame;if(state->frame>=state->trainingAt)*state->training=true;}
 void EntityPlayObjectAnimation(const CScriptThing&t,const CCharString&s,bool b){assert(t.kind==0&&!b);state->animations.push_back(s.text);}
 void EntitySetFacingAngle(const CScriptThing&t,float f,bool b){assert(t.kind==0&&b);state->angles.push_back(f);}
 CScriptThing GetThingWithScriptName(const CCharString&s){assert(s.text=="WillApprentice"||s.text=="TheRealGuildmaster");return CScriptThing(s.text=="WillApprentice"?2:3);}
 CScriptThing& GetHero(){return hero;}
 long AddNewConversation(const CScriptThing&s,bool a,bool b){assert(s.kind==(state->apprenticeAlive?2:3)&&!a&&!b);return ++state->conversations;}
 void AddPersonToConversation(long c,const CScriptThing&s){assert(c==state->conversations&&s.kind==1);}
 void AddLineToConversation(long c,const CCharString&s,bool b,const CScriptThing&a,const CScriptThing&h){assert(c==state->conversations&&!b&&a.kind==(state->apprenticeAlive?2:3)&&h.kind==1);state->text.push_back(s.text);}
 void EntitySetTargetable(const CScriptThing&t,bool b){assert(t.kind==0);state->targets.push_back(b);}
 void Pause(float f){assert(f==0.75f);++state->pauses;}
};
CGameScriptInterfaceBase* pDSTGame;
#ifndef _MSC_VER
#define __forceinline inline
#endif
inline CTimer::CTimer():TimerIndex(pDSTGame->RegisterTimer()) {}
inline CTimer::~CTimer(){pDSTGame->DeregisterTimer(TimerIndex);}
__forceinline CTimer& CTimer::operator=(long n) { pDSTGame->SetTimer(TimerIndex,n); return *this; }
__forceinline long CTimer::GetTime() const { return pDSTGame->GetTimer(TimerIndex); }
inline bool CTimer::operator<=(long n) const { return GetTime()<=n; }

namespace NScript {class CQ_SunnyvaleMasterData{public:bool WillTrainingStarted;long WillScore;CQ_SunnyvaleMasterData():WillTrainingStarted(false),WillScore(0){}};
class CQ_GuildTrainingScript {public:CTimer WillHelpTimer;class CWillDummy {public:CGameScriptInterfaceBase&Game;CScriptThing Me;CQ_GuildTrainingScript&ParentClass;CQ_SunnyvaleMasterData*PMasterData;CWillDummy(CGameScriptInterfaceBase&g,CQ_GuildTrainingScript&p,CQ_SunnyvaleMasterData&d):Game(g),ParentClass(p),PMasterData(&d){}bool IsActiveThreadTerminating()const{++state->checks;return state->frame>=state->limit||(state->stopCheck&&state->checks>=state->stopCheck);}void Main();};};}

void NScript::CQ_GuildTrainingScript::CWillDummy::Main()
{
    float initial_angle=Me.GetAngleXY();
    TRetVoid _Fail_Val;
    Game.NewScriptFrame();
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    Game.NewScriptFrame();
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    ParentClass.WillHelpTimer=15;
    while (PMasterData->WillTrainingStarted != true) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    for (;!false;Game.NewScriptFrame()) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        while (!(Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_LIGHTNING_SPELL,"SCRIPT_NAME_HERO") || Me.MsgIsHitBy("SCRIPT_NAME_HERO"))) {
            Game.NewScriptFrame();
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        if (Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_LIGHTNING_SPELL,"SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            Game.EntityPlayObjectAnimation(Me,"GET_HIT_SPIN",false);
            Game.EntitySetFacingAngle(Me,initial_angle+0.25,true);
            Game.NewScriptFrame();
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            Game.NewScriptFrame();
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            Game.EntitySetFacingAngle(Me,initial_angle+0.5,true);
            ++PMasterData->WillScore;
            if (ParentClass.WillHelpTimer.GetTime()<=0) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (Game.GetThingWithScriptName("WillApprentice").IsAlive()) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    long Conversation=Game.AddNewConversation(Game.GetThingWithScriptName("WillApprentice"),false,false);
                    Game.AddPersonToConversation(Conversation,Game.GetHero());
                    Game.AddLineToConversation(Conversation,"TEXT_QST_028_APPRENTICE_WILL_GOOD_HIT",false,Game.GetThingWithScriptName("WillApprentice"),Game.GetHero());
                } else {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    long Conversation=Game.AddNewConversation(Game.GetThingWithScriptName("TheRealGuildmaster"),false,false);
                    Game.AddPersonToConversation(Conversation,Game.GetHero());
                    Game.AddLineToConversation(Conversation,"TEXT_QST_028_GUILDMASTER_WILL_GOOD_HIT",false,Game.GetThingWithScriptName("TheRealGuildmaster"),Game.GetHero());
                }
                ParentClass.WillHelpTimer=7;
            }
            Game.EntitySetTargetable(Me,false);
            Game.Pause(pGlobalScriptDef->GUI_WillDummySpinTimer);
            Game.EntitySetFacingAngle(Me,initial_angle+0.25,true);
            Game.NewScriptFrame();
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            Game.NewScriptFrame();
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            Game.EntitySetFacingAngle(Me,initial_angle,true);
            Game.EntitySetTargetable(Me,true);
        } else if (Me.MsgIsHitBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            Game.EntityPlayObjectAnimation(Me,"GET_HIT",false);
            if (ParentClass.WillHelpTimer.GetTime()<=0) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (Game.GetThingWithScriptName("WillApprentice").IsAlive()) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    long Conversation=Game.AddNewConversation(Game.GetThingWithScriptName("WillApprentice"),false,false);
                    Game.AddPersonToConversation(Conversation,Game.GetHero());
                    Game.AddLineToConversation(Conversation,"TEXT_QST_028_APPRENTICE_WILL_TROUBLE",false,Game.GetThingWithScriptName("WillApprentice"),Game.GetHero());
                } else {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    long Conversation=Game.AddNewConversation(Game.GetThingWithScriptName("TheRealGuildmaster"),false,false);
                    Game.AddPersonToConversation(Conversation,Game.GetHero());
                    Game.AddLineToConversation(Conversation,"TEXT_QST_028_GUILDMASTER_WILL_TROUBLE",false,Game.GetThingWithScriptName("TheRealGuildmaster"),Game.GetHero());
                }
                ParentClass.WillHelpTimer=7;
            }
        }
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
}

long run(State&s){state=&s;CScriptDef def;def.GUI_WillDummySpinTimer=0.75f;pGlobalScriptDef.Object=&def;long score;{CGameScriptInterfaceBase g;pDSTGame=&g;NScript::CQ_GuildTrainingScript p;NScript::CQ_SunnyvaleMasterData d;state->training=&d.WillTrainingStarted;NScript::CQ_GuildTrainingScript::CWillDummy x(g,p,d);x.Main();score=d.WillScore;assert(state->strings==0&&state->things==2);}assert(state->things==0&&state->registered==1&&state->deregistered==1);return score;}
int main(){int cases=0;{State s;assert(run(s)==0&&s.animations.empty()&&s.timers.size()==1&&s.timers[0]==15);++cases;}
 for(int alive=0;alive<2;++alive)for(int hit=0;hit<2;++hit)for(int early=0;early<2;++early){State s;s.apprenticeAlive=alive!=0;int f=early?4:20;if(hit)s.lightningAt=f;else s.meleeAt=f;long n=run(s);assert(n==hit&&s.animations.size()==1&&s.animations[0]==(hit?"GET_HIT_SPIN":"GET_HIT"));assert(s.conversations==(early?0:1));if(!early){std::string expect=alive?"TEXT_QST_028_APPRENTICE_WILL_":"TEXT_QST_028_GUILDMASTER_WILL_";expect+=hit?"GOOD_HIT":"TROUBLE";assert(s.text[0]==expect&&s.timers.back()==7);}if(hit){assert(s.angles.size()==4&&s.angles[0]==s.angle+0.25f&&s.angles[1]==s.angle+0.5f&&s.angles[2]==s.angle+0.25f&&s.angles[3]==s.angle);assert(s.pauses==1&&s.targets.size()==2&&!s.targets[0]&&s.targets[1]);}else assert(s.angles.empty()&&s.targets.empty()&&!s.pauses);++cases;}
 {State s;s.lightningAt=20;s.meleeAt=20;assert(run(s)==1&&s.animations.size()==1&&s.animations[0]=="GET_HIT_SPIN");++cases;}
 {State s;s.trainingAt=10;s.meleeAt=4;assert(run(s)==0&&s.animations.empty());++cases;}
 for(int stop=1;stop<=100;++stop)for(int mode=0;mode<3;++mode){State s;s.stopCheck=stop;s.trainingAt=mode+2;s.apprenticeAlive=mode!=2;s.meleeAt=mode==0?20:-1;s.lightningAt=mode?20:-1;run(s);assert(s.conversations<=1&&s.pauses<=1);++cases;}
 std::fprintf(stderr,"SCENARIOS %d\n",cases);assert(cases==311);std::puts("00d43450_TEST PASS");}