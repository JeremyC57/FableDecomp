// Selected source 0289ea39270cd15764397a10944904448de19854b8e2e67d6a7c220cdf12365f

#include <cstdio>
#include <cassert>
#include <string>
#include <vector>
#include <limits>
struct Conversation{long id;bool person;std::vector<int>lines;Conversation(long n):id(n),person(false){}};
struct State{int frame,limit,checks,stopCheck,things,strings,resources,movies,pauseDepth,pauseCalls,predicates,starts,startFailures,taskDelay,taskUntil,released,targetCalls,timer,timerSets,timerGets,aDies,bDies,aReads,bReads,fredReads,talkReads,hitReads,anyReads,healReads;bool aAlive,bAlive,fredAlive,hitA,hitB,attacks,talk,hit,ability,heal;float distance,health;std::vector<Conversation>conversations;std::vector<int>speeches;State():frame(0),limit(8),checks(0),stopCheck(0),things(0),strings(0),resources(0),movies(0),pauseDepth(0),pauseCalls(0),predicates(0),starts(0),startFailures(0),taskDelay(0),taskUntil(0),released(0),targetCalls(0),timer(0),timerSets(0),timerGets(0),aDies(50),bDies(50),aReads(0),bReads(0),fredReads(0),talkReads(0),hitReads(0),anyReads(0),healReads(0),aAlive(false),bAlive(false),fredAlive(true),hitA(false),hitB(false),attacks(false),talk(false),hit(false),ability(false),heal(false),distance(10.0f),health(10.0f){}bool term(){++checks;return frame>=limit||(stopCheck&&checks>=stopCheck);}};State*state;
class CCharString{public:std::string s;CCharString(const char*p):s(p){++state->strings;}CCharString(const CCharString&o):s(o.s){++state->strings;}~CCharString(){--state->strings;}};enum EScriptAIPriority{SCRIPT_AI_PRIORITY_HIGHEST=4};enum ETextGroupSelectionMethod{GROUP_SELECT_NONE=0};enum EHeroAbility{HERO_ABILITY_HEAL_LIFE_SPELL=14};class TRetVoid{public:void Return(){}};
class CScriptThing{public:int id;CScriptThing(int n=-1):id(n){++state->things;}CScriptThing(const CScriptThing&o):id(o.id){++state->things;}~CScriptThing(){--state->things;}bool IsAlive()const{if(id==1){++state->aReads;return state->aAlive;}if(id==2){++state->bReads;return state->bAlive;}assert(id==3);++state->fredReads;return state->fredAlive;}void chk(const CCharString&s)const{assert(id==-1&&s.s=="SCRIPT_NAME_HERO");}bool MsgIsTalkedToBy(const CCharString&s)const{chk(s);++state->talkReads;return state->talk;}bool MsgIsHitBy(const CCharString&s)const{chk(s);++state->hitReads;return state->hit;}bool MsgIsHitByAnySpecialAbilityFrom(const CCharString&s)const{chk(s);++state->anyReads;return state->ability;}bool MsgIsHitBySpecialAbilityFrom(EHeroAbility a,const CCharString&s)const{chk(s);assert(a==HERO_ABILITY_HEAL_LIFE_SPELL);++state->healReads;return state->heal;}};
class CTimer{public:bool operator==(long n)const{++state->timerGets;return state->timer==n;}CTimer&operator=(long n){assert(n==30);state->timer=static_cast<int>(n);++state->timerSets;return*this;}};
class CScriptGameResourceObjectScriptedThingBase{public:bool valid;CScriptGameResourceObjectScriptedThingBase():valid(false){++state->resources;}~CScriptGameResourceObjectScriptedThingBase(){assert(state->pauseDepth==0);--state->resources;}bool IsValid()const{return valid;}void Release(){assert(valid);valid=false;++state->released;}CScriptThing GetScriptThing()const{assert(valid);return CScriptThing(-1);}void Speak(const CScriptThing&h,const char*s,ETextGroupSelectionMethod m,bool a,bool b,bool c){assert(valid&&h.id==-2&&m==GROUP_SELECT_NONE&&!a&&b&&!c&&state->pauseDepth==1&&state->health>0);std::string t(s);int code=t=="TEXT_QST_B04_FIGHTINGGUARD_ENEMYDEADPLAYERNOHELP_ONTALKED_10"?8:t=="TEXT_QST_B04_FIGHTINGGUARD_ENEMYDEADPLAYERHELPED_ONTALKED_10"?9:-1;assert(code>=0&&code==((state->hitA||state->hitB)?9:8));state->speeches.push_back(code);state->taskUntil=state->frame+state->taskDelay;}bool IsPerformingScriptTask()const{assert(valid);return state->frame<state->taskUntil;}};
class CScriptGameResourceObjectMovieBase{public:CScriptGameResourceObjectMovieBase(){assert(state->movies==0);++state->movies;}~CScriptGameResourceObjectMovieBase(){assert(state->pauseDepth==0);--state->movies;}};
class CGameScriptInterfaceBase{CScriptThing hero;public:CGameScriptInterfaceBase():hero(-2){}void self(const CScriptThing&m)const{assert(m.id==-1);}void NewScriptFrame(){assert(state->frame<255);++state->frame;if(state->timer>0)--state->timer;if(state->frame>=state->aDies)state->aAlive=false;if(state->frame>=state->bDies)state->bAlive=false;}CScriptThing GetThingWithScriptName(const CCharString&s){if(s.s=="Summoner1")return CScriptThing(1);assert(s.s=="Summoner2");return CScriptThing(2);}CScriptThing GetNearestWithScriptName(const CScriptThing&m,const CCharString&s){self(m);assert(s.s=="HOHGuard");return CScriptThing(3);}CScriptThing&GetHero(){return hero;}long AddNewConversation(const CScriptThing&m,bool a,bool b){self(m);assert(!a&&!b&&state->pauseDepth==0);long id=100+static_cast<long>(state->conversations.size());state->conversations.push_back(Conversation(id));return id;}Conversation&conv(long id){assert(id>=100&&id<100+static_cast<long>(state->conversations.size()));return state->conversations[static_cast<unsigned>(id-100)];}void AddPersonToConversation(long id,const CScriptThing&h){assert(h.id==-2&&!conv(id).person);conv(id).person=true;}void AddLineToConversation(long id,const CCharString&s,bool b,const CScriptThing&m,const CScriptThing&h){self(m);assert(h.id==-2&&!b&&conv(id).person&&conv(id).lines.empty());const char*texts[]={"TEXT_QST_B04_FIGHTINGGUARD_COMMENT_10","TEXT_QST_B04_FIGHTINGGUARD_NOTHELPED_10","TEXT_QST_B04_FIGHTINGGUARD_ATTACKEDENEMY_10","TEXT_QST_B04_FIGHTINGGUARD_ONTALKED_10","TEXT_QST_B04_FIGHTINGGUARD_ONHIT_10","TEXT_QST_B04_FIGHTINGGUARD_ENEMYDEADPLAYERNOHELP_COMMENT_10","TEXT_QST_B04_FIGHTINGGUARD_ENEMYDEADPLAYERHELPED_COMMENT_10","TEXT_QST_B04_FIGHTINGGUARD_ENEMYDEAD_ONHIT_10"};int code=-1;for(int i=0;i<8;++i)if(s.s==texts[i])code=i;assert(code>=0);conv(id).lines.push_back(code);}bool StartScriptingEntity(const CScriptThing&m,CScriptGameResourceObjectScriptedThingBase&r,EScriptAIPriority p){self(m);assert(p==SCRIPT_AI_PRIORITY_HIGHEST);return r.valid=++state->starts>state->startFailures;}void StartMovieSequence(const CCharString&s,CScriptGameResourceObjectMovieBase&){assert(s.s.empty()&&state->movies==1);}void PauseAllNonScriptedEntities(bool b){if(b){assert(state->pauseDepth==0);++state->pauseDepth;++state->pauseCalls;}else{assert(state->pauseDepth==1);--state->pauseDepth;}}float GetHealth(const CScriptThing&m){self(m);return state->health;}void GiveThingBestEnemyTarget(const CScriptThing&m,const CScriptThing&h){self(m);assert(h.id==-2&&state->attacks&&state->pauseDepth==0&&state->targetCalls==0);++state->targetCalls;}};
class CWideScreenMagicPauseEntities{CGameScriptInterfaceBase*Game;public:CWideScreenMagicPauseEntities(CGameScriptInterfaceBase&g):Game(&g){Game->PauseAllNonScriptedEntities(true);}~CWideScreenMagicPauseEntities(){Game->PauseAllNonScriptedEntities(false);}};
bool IsDistanceBetweenThingsUnder(const CScriptThing&a,const CScriptThing&b,float n){assert(a.id==-1&&b.id==-2&&(n==15||n==30));return state->distance<n;}bool IsDistanceBetweenThingsOver(const CScriptThing&a,const CScriptThing&b,float n){assert(a.id==-1&&b.id==-2&&n==15);return state->distance>n;}
namespace NScript{class CIsThingAlive{CScriptThing Thing;public:CIsThingAlive(const CScriptThing&m):Thing(m){assert(m.id==-1);}};class CQ_HeroSoulsArenaScript{public:bool&PlayerHitSummoner1;bool&PlayerHitSummoner2;bool&PlayerAttacksGuard;CTimer GuardChatTimer;CQ_HeroSoulsArenaScript():PlayerHitSummoner1(state->hitA),PlayerHitSummoner2(state->hitB),PlayerAttacksGuard(state->attacks){}class CHOHGuard{public:CGameScriptInterfaceBase&Game;CScriptThing Me;CQ_HeroSoulsArenaScript&ParentClass;CHOHGuard(CGameScriptInterfaceBase&g,CQ_HeroSoulsArenaScript&p):Game(g),Me(-1),ParentClass(p){}void SetPredicate(const CIsThingAlive&){++state->predicates;}bool IsActiveThreadTerminating()const{return state->term();}void Main();};};}
void NScript::CQ_HeroSoulsArenaScript::CHOHGuard::Main()
{
    TRetVoid _Fail_Val;
    SetPredicate(CIsThingAlive(Me));
    Game.NewScriptFrame();
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    CScriptGameResourceObjectScriptedThingBase seh_me;
    CScriptThing SummonerA=Game.GetThingWithScriptName("Summoner1");
    CScriptThing SummonerB=Game.GetThingWithScriptName("Summoner2");
    CScriptThing Fred=Game.GetNearestWithScriptName(Me,"HOHGuard");
    while (SummonerA.IsAlive() || SummonerB.IsAlive()) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        if (ParentClass.GuardChatTimer==0 && IsDistanceBetweenThingsUnder(Me,Game.GetHero(),30.0f)) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            long HLStyleConversationIndex=Game.AddNewConversation(Me,false,false);
            Game.AddPersonToConversation(HLStyleConversationIndex,Game.GetHero());
            if (IsDistanceBetweenThingsOver(Me,Game.GetHero(),15.0f)) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (Fred.IsAlive()) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    Game.AddLineToConversation(HLStyleConversationIndex,"TEXT_QST_B04_FIGHTINGGUARD_COMMENT_10",false,Me,Game.GetHero());
                }
            } else if (!ParentClass.PlayerHitSummoner1 && !ParentClass.PlayerHitSummoner2) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                Game.AddLineToConversation(HLStyleConversationIndex,"TEXT_QST_B04_FIGHTINGGUARD_NOTHELPED_10",false,Me,Game.GetHero());
            } else {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                Game.AddLineToConversation(HLStyleConversationIndex,"TEXT_QST_B04_FIGHTINGGUARD_ATTACKEDENEMY_10",false,Me,Game.GetHero());
            }
            ParentClass.GuardChatTimer=30;
        }
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            long HLStyleConversationIndex=Game.AddNewConversation(Me,false,false);
            Game.AddPersonToConversation(HLStyleConversationIndex,Game.GetHero());
            Game.AddLineToConversation(HLStyleConversationIndex,"TEXT_QST_B04_FIGHTINGGUARD_ONTALKED_10",false,Me,Game.GetHero());
        }
        if (Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL,"SCRIPT_NAME_HERO"))) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            long HLStyleConversationIndex=Game.AddNewConversation(Me,false,false);
            Game.AddPersonToConversation(HLStyleConversationIndex,Game.GetHero());
            Game.AddLineToConversation(HLStyleConversationIndex,"TEXT_QST_B04_FIGHTINGGUARD_ONHIT_10",false,Me,Game.GetHero());
            ParentClass.PlayerAttacksGuard=true;
        }
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    for (; !false; Game.NewScriptFrame()) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        if (ParentClass.GuardChatTimer==0 && IsDistanceBetweenThingsUnder(Me,Game.GetHero(),15.0f)) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            long HLStyleConversationIndex=Game.AddNewConversation(Me,false,false);
            Game.AddPersonToConversation(HLStyleConversationIndex,Game.GetHero());
            if (!ParentClass.PlayerHitSummoner1 && !ParentClass.PlayerHitSummoner2) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                Game.AddLineToConversation(HLStyleConversationIndex,"TEXT_QST_B04_FIGHTINGGUARD_ENEMYDEADPLAYERNOHELP_COMMENT_10",false,Me,Game.GetHero());
            } else {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                Game.AddLineToConversation(HLStyleConversationIndex,"TEXT_QST_B04_FIGHTINGGUARD_ENEMYDEADPLAYERHELPED_COMMENT_10",false,Me,Game.GetHero());
            }
            ParentClass.GuardChatTimer=30;
        }
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (seh_me.IsValid()) seh_me.Release();
            while (!Game.StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
                Game.NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            CScriptGameResourceObjectMovieBase speech_movie;
            Game.StartMovieSequence("",speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
            if (!ParentClass.PlayerHitSummoner1 && !ParentClass.PlayerHitSummoner2) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (Game.GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                    seh_me.Speak(Game.GetHero(),"TEXT_QST_B04_FIGHTINGGUARD_ENEMYDEADPLAYERNOHELP_ONTALKED_10",GROUP_SELECT_NONE,false,true,false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game.NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
            } else {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (Game.GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                    seh_me.Speak(Game.GetHero(),"TEXT_QST_B04_FIGHTINGGUARD_ENEMYDEADPLAYERHELPED_ONTALKED_10",GROUP_SELECT_NONE,false,true,false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game.NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
            }
            if (seh_me.IsValid()) seh_me.Release();
        }
        if (Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL,"SCRIPT_NAME_HERO"))) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            long HLStyleConversationIndex=Game.AddNewConversation(Me,false,false);
            Game.AddPersonToConversation(HLStyleConversationIndex,Game.GetHero());
            Game.AddLineToConversation(HLStyleConversationIndex,"TEXT_QST_B04_FIGHTINGGUARD_ENEMYDEAD_ONHIT_10",false,Me,Game.GetHero());
            ParentClass.PlayerAttacksGuard=true;
        }
        if (ParentClass.PlayerAttacksGuard) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            Game.GiveThingBestEnemyTarget(Me,Game.GetHero());
            while (!false) {
                Game.NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
}


int scenarios=0;int lines(const State&s,int n){int r=0;for(unsigned i=0;i<s.conversations.size();++i)for(unsigned j=0;j<s.conversations[i].lines.size();++j)if(s.conversations[i].lines[j]==n)++r;return r;}
void run(State&s){++scenarios;state=&s;{CGameScriptInterfaceBase game;NScript::CQ_HeroSoulsArenaScript parent;NScript::CQ_HeroSoulsArenaScript::CHOHGuard guard(game,parent);guard.Main();}assert(s.things==0&&s.strings==0&&s.resources==0&&s.movies==0&&s.pauseDepth==0&&s.predicates==1);assert(s.targetCalls<=1&&s.speeches.size()<=static_cast<unsigned>(s.pauseCalls));if(s.targetCalls)assert(s.attacks);for(unsigned i=0;i<s.conversations.size();++i)assert(s.conversations[i].person);}
int main(){
 {State s;run(s);assert(lines(s,5)==1&&s.timerSets==1&&s.conversations.size()==1&&s.starts==0);}
 {State s;s.talk=true;run(s);assert(s.speeches.size()==7&&s.starts==7&&s.released==7&&s.pauseCalls==7);}
 {State s;s.aAlive=true;s.bAlive=true;s.distance=20;run(s);assert(lines(s,0)==1&&s.timerSets==1&&s.bReads==0&&s.starts==0);}
 {State s;s.aAlive=true;s.fredAlive=false;s.distance=20;run(s);assert(s.conversations.size()==1&&s.conversations[0].lines.empty()&&s.timerSets==1);}
 for(int flags=0;flags<64;++flags){State s;s.aAlive=(flags&1)!=0;s.bAlive=(flags&2)!=0;s.hitA=(flags&4)!=0;s.hitB=(flags&8)!=0;s.talk=(flags&16)!=0;s.hit=(flags&32)!=0;run(s);bool alive=(flags&3)!=0;bool help=(flags&12)!=0;assert(lines(s,alive?(help?2:1):(help?6:5))==1);if(s.hit){assert(s.attacks&&lines(s,alive?4:7)>0);assert(s.targetCalls==(alive?0:1));}if(s.talk){if(alive)assert(lines(s,3)>0&&s.speeches.empty());else assert(!s.speeches.empty()&&s.speeches[0]==(help?9:8));}}
 float dist[8]={-1.0f,14.5f,15.0f,15.5f,29.5f,30.0f,30.5f,std::numeric_limits<float>::quiet_NaN()};
 for(int alive=0;alive<2;++alive)for(int fred=0;fred<2;++fred)for(int h=0;h<4;++h)for(int d=0;d<8;++d){State s;s.aAlive=alive!=0;s.fredAlive=fred!=0;s.hitA=(h&1)!=0;s.hitB=(h&2)!=0;s.distance=dist[d];run(s);bool ambient=alive?d<5:d<2;assert(s.timerSets==(ambient?1:0));if(ambient){assert(s.conversations.size()==1);int code=alive?(d>=3?0:(h?2:1)):(h?6:5);assert(lines(s,code)==(alive&&d>=3&&!fred?0:1));}else assert(s.conversations.empty());}
 for(int bits=0;bits<8;++bits){State s;s.hit=(bits&1)!=0;s.ability=(bits&2)!=0;s.heal=(bits&4)!=0;run(s);bool attacked=s.hit||(s.ability&&!s.heal);assert(s.attacks==attacked&&s.targetCalls==(attacked?1:0));if(s.hit)assert(s.anyReads==0&&s.healReads==0);}
 float health[5]={-1.0f,0.0f,0.5f,12.0f,std::numeric_limits<float>::quiet_NaN()};
 for(int h=0;h<5;++h)for(int help=0;help<2;++help){State s;s.talk=true;s.health=health[h];s.hitA=help!=0;run(s);assert(s.pauseCalls==7&&s.released==7&&s.speeches.size()==static_cast<unsigned>(h==2||h==3?7:0));}
 for(int fail=0;fail<5;++fail)for(int delay=0;delay<5;++delay){State s;s.limit=20;s.talk=true;s.startFailures=fail;s.taskDelay=delay;run(s);assert(!s.speeches.empty()&&s.starts>fail);}
 for(int a=2;a<6;++a)for(int b=2;b<6;++b){State s;s.aAlive=true;s.bAlive=true;s.aDies=a;s.bDies=b;s.talk=true;s.hit=true;run(s);assert(lines(s,3)>0&&lines(s,4)>0&&s.targetCalls==1&&!s.speeches.empty()&&lines(s,7)==1);}
 {State s;s.attacks=true;run(s);assert(s.targetCalls==1&&s.frame==8&&s.conversations.size()==1);}
 {State s;s.timer=40;run(s);assert(s.conversations.empty()&&s.timerSets==0);}
 for(int stop=1;stop<=180;++stop)for(int mode=0;mode<18;++mode){State s;s.stopCheck=stop;s.limit=12;switch(mode){case 0:break;case 1:s.aAlive=true;break;case 2:s.bAlive=true;s.distance=20;break;case 3:s.aAlive=true;s.bAlive=true;s.fredAlive=false;s.distance=20;break;case 4:s.talk=true;break;case 5:s.talk=true;s.hitA=true;break;case 6:s.talk=true;s.taskDelay=3;break;case 7:s.hit=true;break;case 8:s.ability=true;break;case 9:s.ability=true;s.heal=true;break;case 10:s.aAlive=true;s.talk=true;s.hit=true;break;case 11:s.aAlive=true;s.aDies=4;s.talk=true;s.hit=true;break;case 12:s.talk=true;s.startFailures=30;break;case 13:s.talk=true;s.health=0;break;case 14:s.attacks=true;break;case 15:s.timer=40;break;case 16:s.distance=15;break;case 17:s.aAlive=true;s.distance=30;break;}run(s);}
 assert(scenarios==3497);std::puts("00d7e0c0_TEST PASS");return 0;
}