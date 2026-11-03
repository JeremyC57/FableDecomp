#include <cstdio>
#include <cassert>
#include <string>
#include <vector>
#include <limits>
struct Event {int wave,count,ai,hits,magic;bool heal,summonerAlive,minionAlive;float summonerDistance,minionDistance;Event():wave(0),count(0),ai(0),hits(0),magic(0),heal(false),summonerAlive(true),minionAlive(true),summonerDistance(10),minionDistance(5){}};
struct State{Event e[256];int frame,limit,checks,stopCheck,readyAt,predicates,things,timers,follow,objectives,commentFailures;long timerUntil;long initialTimer;std::vector<std::string> attempts,accepted;std::vector<int> targets,nearest;std::vector<long>assigned;State():frame(0),limit(6),checks(0),stopCheck(0),readyAt(1),predicates(0),things(0),timers(0),follow(0),objectives(0),commentFailures(0),initialTimer(0){}bool term(){++checks;return frame>=limit||(stopCheck&&checks>=stopCheck);}};State*state;
class CCharString{public:std::string text;CCharString():text(){}CCharString(const char*s):text(s){}CCharString&operator=(const char*s){text=s;return *this;}};
class TRetVoid{public:void Return(){}};enum EHeroAbility{HERO_ABILITY_HEAL_LIFE_SPELL=14};
int who(const std::string&s){if(s=="SummonerAttacker")return 1;if(s=="SummonerMinion")return 2;assert(s=="SCRIPT_NAME_HERO");return 4;}
class CScriptThing{public:int kind;CScriptThing(int k=-1):kind(k){++state->things;}CScriptThing(const CScriptThing&t):kind(t.kind){++state->things;}~CScriptThing(){--state->things;}bool IsAlive()const{assert(kind!=0);return kind==1?state->e[state->frame].summonerAlive:kind==2?state->e[state->frame].minionAlive:false;}bool IsDead()const{return !IsAlive();}long GetCurrentStateGroupType()const{assert(kind==0);return state->e[state->frame].ai;}
 bool MsgIsHitBy(const CCharString&s)const{assert(kind==0);return(state->e[state->frame].hits&who(s.text))!=0;}bool MsgIsHitByAnySpecialAbilityFrom(const CCharString&s)const{assert(kind==0);return(state->e[state->frame].magic&who(s.text))!=0;}bool MsgIsHitBySpecialAbilityFrom(EHeroAbility a,const CCharString&s)const{assert(kind==0&&a==HERO_ABILITY_HEAL_LIFE_SPELL&&s.text=="SCRIPT_NAME_HERO");return state->e[state->frame].heal;}};
float GetDistanceBetweenThings(const CScriptThing&m,const CScriptThing&t){assert(m.kind==0&&(t.kind==1||t.kind==2));return t.kind==1?state->e[state->frame].summonerDistance:state->e[state->frame].minionDistance;}
void advance();
class CGameScriptInterfaceBase{public:long RegisterTimer(){++state->timers;state->timerUntil=state->initialTimer<0?state->initialTimer:state->frame+state->initialTimer;return 17;}void DeregisterTimer(long index){assert(index==17);--state->timers;}void SetTimer(long index,long n){assert(index==17);state->assigned.push_back(n);state->timerUntil=state->frame+n;}long GetTimer(long index)const{assert(index==17);return state->timerUntil<0?state->timerUntil:state->timerUntil<state->frame?0:state->timerUntil-state->frame;}void NewScriptFrame(){assert(state->frame<255);++state->frame;advance();}CCharString GetActiveQuestName()const{return CCharString("QUEST");}void SetQuestCardObjective(const CCharString&q,const CCharString&o,const CCharString&a,const CCharString&b){assert(q.text=="QUEST"&&o.text=="TEXT_QUEST_SUMMONING_SHIP_OBJECTIVE_03"&&a.text=="HookCoast"&&b.text=="HookCoast");++state->objectives;}CScriptThing GetNearestWithScriptName(const CScriptThing&t,const CCharString&s)const{assert(t.kind==0);int k=who(s.text);assert(k==1||k==2);state->nearest.push_back(k);return CScriptThing(k);}void GiveThingBestEnemyTarget(const CScriptThing&m,const CScriptThing&t){assert(m.kind==0&&t.IsAlive());state->targets.push_back(t.kind);}};
CGameScriptInterfaceBase* pDSTGame;
#ifndef _MSC_VER
#define __forceinline inline
#endif
class CTimer{long TimerIndex;public:CTimer();~CTimer();CTimer&operator=(long);long GetTime()const;bool operator==(long)const;};
inline CTimer::CTimer():TimerIndex(pDSTGame->RegisterTimer()) {}
inline CTimer::~CTimer() { pDSTGame->DeregisterTimer(TimerIndex); }
__forceinline CTimer& CTimer::operator=(long time) { pDSTGame->SetTimer(TimerIndex,time); return *this; }
inline long CTimer::GetTime() const { return pDSTGame->GetTimer(TimerIndex); }
inline bool CTimer::operator==(long time) const { return GetTime()==time; }
namespace NScript{class CIsThingAlive{public:CIsThingAlive(const CScriptThing&t){assert(t.kind==0);}};class CQ_SummoningTheShipScript{public:long CurrentAttackWave,SummonersAlive;bool SummonerAttacksStarted;CQ_SummoningTheShipScript():CurrentAttackWave(0),SummonersAlive(0),SummonerAttacksStarted(false){}bool MakeBriarRoseComment(const CCharString&s){state->attempts.push_back(s.text);if(state->commentFailures){--state->commentFailures;return false;}state->accepted.push_back(s.text);return true;}
 class CSTS_BriarRose{public:CGameScriptInterfaceBase&Game;CScriptThing Me;CQ_SummoningTheShipScript&ParentClass;long BrainState;CSTS_BriarRose(CGameScriptInterfaceBase&g,CQ_SummoningTheShipScript&p):Game(g),Me(0),ParentClass(p),BrainState(1){}void SetPredicate(const CIsThingAlive&){++state->predicates;}bool IsActiveThreadTerminating()const{return state->term();}void FollowHero(bool b){assert(b&&BrainState==2);++state->follow;}void Main();};};}
NScript::CQ_SummoningTheShipScript*parent;void advance(){parent->CurrentAttackWave=state->e[state->frame].wave;parent->SummonersAlive=state->e[state->frame].count;parent->SummonerAttacksStarted=state->frame>=state->readyAt;}

void NScript::CQ_SummoningTheShipScript::CSTS_BriarRose::Main()
{
    TRetVoid _Fail_Val;
    SetPredicate(CIsThingAlive(Me));
    Game.NewScriptFrame();
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    while (!ParentClass.SummonerAttacksStarted) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    long CurrentAIState=0;
    long PreviousAIState=0;
    CTimer AttackedByEnemyTimer;
    long SummonerCountComment=0;
    BrainState=2;
    Game.SetQuestCardObjective(Game.GetActiveQuestName(),"TEXT_QUEST_SUMMONING_SHIP_OBJECTIVE_03","HookCoast","HookCoast");
    FollowHero(true);
    long BriarMode=0;
    CScriptThing Enemy;
    for (;!false; Game.NewScriptFrame()) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        if (ParentClass.CurrentAttackWave==1 && BriarMode!=1) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            Enemy=Game.GetNearestWithScriptName(Me,"SummonerAttacker");
            if (Enemy.IsAlive()) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                Game.GiveThingBestEnemyTarget(Me,Enemy);
            }
            BriarMode=1;
        } else if (ParentClass.CurrentAttackWave!=1 && BriarMode==1 && Enemy.IsDead()) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            CScriptThing Summoner=Game.GetNearestWithScriptName(Me,"SummonerAttacker");
            CScriptThing Minion=Game.GetNearestWithScriptName(Me,"SummonerMinion");
            if (GetDistanceBetweenThings(Me,Summoner)>GetDistanceBetweenThings(Me,Minion)) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                Enemy=Minion;
            } else {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                Enemy=Summoner;
            }
            if (Enemy.IsAlive()) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                Game.GiveThingBestEnemyTarget(Me,Enemy);
            }
        }
        if (SummonerCountComment!=ParentClass.SummonersAlive) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            CCharString SummonerCountCommentString;
            switch (ParentClass.SummonersAlive) {
            case 1: SummonerCountCommentString="SUMMONER_ONE_LEFT"; break;
            case 2: SummonerCountCommentString="SUMMONER_TWO_LEFT"; break;
            case 3: SummonerCountCommentString="SUMMONER_THREE_LEFT"; break;
            }
            if (ParentClass.SummonersAlive>0 && ParentClass.SummonersAlive<=3 && ParentClass.MakeBriarRoseComment(SummonerCountCommentString)) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                SummonerCountComment=ParentClass.SummonersAlive;
            }
        }
        CurrentAIState=Me.GetCurrentStateGroupType();
        if (PreviousAIState!=CurrentAIState) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (CurrentAIState==2) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                ParentClass.MakeBriarRoseComment("FOLLOWING");
            } else if (CurrentAIState==1) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                ParentClass.MakeBriarRoseComment("ATTACKING");
            }
        }
        PreviousAIState=CurrentAIState;
        if (AttackedByEnemyTimer==0) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (Me.MsgIsHitBy("SummonerAttacker") || (Me.MsgIsHitByAnySpecialAbilityFrom("SummonerAttacker") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL,"SCRIPT_NAME_HERO"))) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (ParentClass.MakeBriarRoseComment("SUMMONER_ATTACKED")) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    AttackedByEnemyTimer=30;
                }
            }
            if (Me.MsgIsHitBy("SummonerMinion") || (Me.MsgIsHitByAnySpecialAbilityFrom("SummonerMinion") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL,"SCRIPT_NAME_HERO"))) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (ParentClass.MakeBriarRoseComment("MINION_ATTACKED")) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    AttackedByEnemyTimer=30;
                }
            }
        }
        if (Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL,"SCRIPT_NAME_HERO"))) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            ParentClass.MakeBriarRoseComment("ATTACKED");
        }
    }
}

void run(State&s){state=&s;{CGameScriptInterfaceBase g;pDSTGame=&g;NScript::CQ_SummoningTheShipScript p;parent=&p;advance();NScript::CQ_SummoningTheShipScript::CSTS_BriarRose t(g,p);t.Main();assert(s.things==1&&s.timers==0&&s.predicates==1&&s.follow==s.objectives);}assert(!s.things);}
int occurrences(const std::vector<std::string>&v,const char*s){int n=0;for(unsigned i=0;i<v.size();++i)if(v[i]==s)++n;return n;}
int main(){int cases=0;
 {State s;run(s);assert(s.follow==1&&s.accepted.empty()&&s.targets.empty());++cases;}
 {State s;s.readyAt=4;run(s);assert(s.follow==1&&s.frame==6);++cases;}
 for(int n=-1;n<=4;++n){State s;s.limit=2;s.e[1].count=n;run(s);assert(s.accepted.size()==unsigned(n>=1&&n<=3));if(n>=1&&n<=3){const char*text[]={"","SUMMONER_ONE_LEFT","SUMMONER_TWO_LEFT","SUMMONER_THREE_LEFT"};assert(s.accepted[0]==text[n]);}++cases;}
 {State s;s.commentFailures=1;for(int n=1;n<6;++n)s.e[n].count=3;run(s);assert(s.attempts.size()==2&&s.accepted.size()==1);++cases;}
 {State s;s.e[1].ai=2;s.e[2].ai=2;s.e[3].ai=1;s.e[4].ai=0;s.e[5].ai=1;run(s);assert(s.accepted.size()==3&&s.accepted[0]=="FOLLOWING"&&s.accepted[1]=="ATTACKING"&&s.accepted[2]=="ATTACKING");++cases;}
 for(int hit=0;hit<8;++hit)for(int magic=0;magic<8;++magic)for(int heal=0;heal<2;++heal){State s;s.limit=2;s.e[1].hits=hit;s.e[1].magic=magic;s.e[1].heal=heal!=0;run(s);int active=hit|(heal?0:magic);assert(occurrences(s.accepted,"SUMMONER_ATTACKED")==!!(active&1));assert(occurrences(s.accepted,"MINION_ATTACKED")==!!(active&2));assert(occurrences(s.accepted,"ATTACKED")==!!(active&4));assert(s.assigned.size()==unsigned(!!(active&1)+!!(active&2)));for(unsigned j=0;j<s.assigned.size();++j)assert(s.assigned[j]==30);++cases;}
 {State s;for(int n=1;n<6;++n)s.e[n].hits=7;run(s);assert(occurrences(s.accepted,"SUMMONER_ATTACKED")==1&&occurrences(s.accepted,"MINION_ATTACKED")==1&&occurrences(s.accepted,"ATTACKED")==5);++cases;}
 {State s;s.initialTimer=-1;s.e[1].hits=7;run(s);assert(s.accepted.size()==1&&s.accepted[0]=="ATTACKED"&&s.assigned.empty());++cases;}
 {State s;s.commentFailures=1;s.e[1].hits=1;s.e[2].hits=1;run(s);assert(s.attempts.size()==2&&s.accepted.size()==1&&s.assigned.size()==1);++cases;}
 for(int mode=0;mode<4;++mode){State s;s.limit=3;s.e[1].wave=1;s.e[2].wave=2;s.e[2].summonerAlive=false;if(mode==0){s.e[2].summonerDistance=10;s.e[2].minionDistance=5;}if(mode==1){s.e[2].summonerDistance=5;s.e[2].minionDistance=10;}if(mode==2){s.e[2].summonerDistance=5;s.e[2].minionDistance=5;}if(mode==3){s.e[2].summonerDistance=std::numeric_limits<float>::quiet_NaN();}run(s);assert(s.nearest.size()==3&&s.targets[0]==1);assert(s.targets.size()==unsigned(mode==0?2:1));if(mode==0)assert(s.targets[1]==2);++cases;}
 {State s;for(int n=1;n<6;++n)s.e[n].wave=1;run(s);assert(s.nearest.size()==1&&s.targets.size()==1);++cases;}
 for(int mode=0;mode<5;++mode)for(int stop=1;stop<=100;++stop){State s;s.limit=12;s.stopCheck=stop;s.readyAt=mode==0?4:1;for(int n=1;n<12;++n){s.e[n].wave=n==1?1:2;s.e[n].summonerAlive=n==1;s.e[n].count=mode==1?3:0;s.e[n].ai=mode==2?n%3:0;s.e[n].hits=mode==3?7:0;s.e[n].magic=mode==4?7:0;}run(s);assert(s.checks<=stop);++cases;}
 assert(cases==646);std::puts("00df1830_TEST PASS");}
