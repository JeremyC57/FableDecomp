// Selected source f2091bfb1b4d01e953f21b68790399b14cbd0a87c352f10c0560ec10419e9185

#include <cassert>
#include <cstdio>
#include <string>
#include <vector>
#include <limits>
struct State{int frame,limit,checks,stopCheck,things,strings,resources,movies,pauseDepth,pauseCalls,predicates,starts,startFailures,taskDelay,taskUntil,questions,answerDelay,answerReady,answerReads,goldReads,heroObjects,entityGold,removals,info,clears,crime,timers,timerCreates,timerDestroys,pushable,forcePushable,vanishAt;long answer,gold,secondGold;bool bought,stole,talk,alive,changeGold;float health;std::vector<int>speech;std::vector<float>morality;std::vector<int>events;State():frame(0),limit(6),checks(0),stopCheck(0),things(0),strings(0),resources(0),movies(0),pauseDepth(0),pauseCalls(0),predicates(0),starts(0),startFailures(0),taskDelay(0),taskUntil(0),questions(0),answerDelay(0),answerReady(0),answerReads(0),goldReads(0),heroObjects(0),entityGold(0),removals(0),info(0),clears(0),crime(0),timers(0),timerCreates(0),timerDestroys(0),pushable(0),forcePushable(0),vanishAt(99),answer(0),gold(1500),secondGold(1500),bought(false),stole(false),talk(false),alive(true),changeGold(false),health(10.0f){}bool term(){++checks;return frame>=limit||(stopCheck&&checks>=stopCheck);}};State*state;
class CCharString{public:std::string s;CCharString(const char*p):s(p){++state->strings;}CCharString(const CCharString&o):s(o.s){++state->strings;}~CCharString(){--state->strings;}};
enum EScriptAIPriority{SCRIPT_AI_PRIORITY_HIGHEST=4};enum ETextGroupSelectionMethod{GROUP_SELECT_NONE=0};enum ECrime{CRIME_THEFT=6};enum EOpinionPostDeedType{OPINION_POST_DEED_TYPE_ALL=7};class TRetVoid{public:void Return(){}};
class CScriptThing{public:int id;CScriptThing(int n=-1):id(n){++state->things;}CScriptThing(const CScriptThing&o):id(o.id){++state->things;}~CScriptThing(){--state->things;}bool IsDead()const{assert(id==1);return !state->alive;}bool MsgIsTalkedToBy(const CCharString&s)const{assert(id==-1&&s.s=="SCRIPT_NAME_HERO");return state->talk;}};
class CTimer{public:CTimer(){assert(state->timers==0&&state->pushable==1&&state->forcePushable==1);++state->timers;++state->timerCreates;}~CTimer(){assert(state->timers==1&&state->pauseDepth==0);--state->timers;++state->timerDestroys;}};
class CDefString{public:operator CCharString()const{return CCharString("OBJECT_TEST_INGREDIENT");}};
struct CScriptDef{CDefString IngredientObject;float BoughtMushroomInBarrowFields,StoleMushroomInBarrowFields;CScriptDef():BoughtMushroomInBarrowFields(3.5f),StoleMushroomInBarrowFields(-2.5f){}};CScriptDef definitions;CScriptDef*pGlobalScriptDef=&definitions;
class CScriptGameResourceObjectScriptedThingBase{public:bool valid;CScriptGameResourceObjectScriptedThingBase():valid(false){++state->resources;}~CScriptGameResourceObjectScriptedThingBase(){assert(state->pauseDepth==0&&state->timers==0);--state->resources;}bool IsValid()const{return valid;}void Release(){assert(false);}CScriptThing GetScriptThing()const{assert(valid);return CScriptThing(-3);}void Speak(const CScriptThing&h,const char*s,ETextGroupSelectionMethod m,bool a,bool b,bool c){assert(valid&&h.id==-2&&m==GROUP_SELECT_NONE&&!a&&b&&!c&&state->pauseDepth==1&&state->health>0);const char*text[]={"TEXT_QST_B10_TRADER_INTRO","TEXT_QST_B10_TRADER_BOUGHT_MUSHROOM_10","TEXT_QST_B10_TRADER_NOT_ENOUGH_MONEY_10","TEXT_QST_B10_TRADER_NOT_BOUGHT_MUSHROOM_10","TEXT_QST_B10_TRADER_POST_BOUGHT_MUSHROOM_10","TEXT_QST_B10_TRADER_POST_STOLE_MUSHROOM_10","TEXT_QST_B10_TRADER_STOLE_MUSHROOM_10"};int code=-1;for(int i=0;i<7;++i)if(std::string(s)==text[i])code=i;assert(code>=0);state->speech.push_back(code);state->events.push_back(20+code);state->taskUntil=state->frame+state->taskDelay;}bool IsPerformingScriptTask()const{assert(valid);return state->frame<state->taskUntil;}};
class CScriptGameResourceObjectMovieBase{public:CScriptGameResourceObjectMovieBase(){assert(state->movies==0);++state->movies;}~CScriptGameResourceObjectMovieBase(){assert(state->pauseDepth==0);--state->movies;}};
class CGameScriptInterfaceBase{CScriptThing hero;public:CGameScriptInterfaceBase():hero(-2){}void self(const CScriptThing&m)const{assert(m.id==-1);}void NewScriptFrame(){assert(state->frame<255);++state->frame;if(state->frame>=state->vanishAt)state->alive=false;}CScriptThing&GetHero(){return hero;}CScriptThing GetThingWithScriptName(const CCharString&s){if(s.s=="Ingredient")return CScriptThing(1);assert(s.s=="VILLAGE_BARROWFIELDS");return CScriptThing(2);}bool StartScriptingEntity(const CScriptThing&m,CScriptGameResourceObjectScriptedThingBase&r,EScriptAIPriority p){self(m);assert(p==SCRIPT_AI_PRIORITY_HIGHEST&&!r.valid);return r.valid=++state->starts>state->startFailures;}void SetThingHasInformation(const CScriptThing&m,bool a,bool b,bool c){self(m);assert(!a&&b&&!c&&state->pauseDepth==0);++state->info;}void ClearThingHasInformation(const CScriptThing&m){self(m);assert(state->bought&&state->pauseDepth==1);++state->clears;}void SetIsPushableByHero(CScriptThing m,bool b){self(m);assert(!b&&state->pushable==0);++state->pushable;}void SetIsThingForcePushable(const CScriptThing&m,bool b){self(m);assert(!b&&state->forcePushable==0);++state->forcePushable;}void StartMovieSequence(const CCharString&s,CScriptGameResourceObjectMovieBase&){assert(s.s.empty()&&state->movies==1&&state->timers==1);}void PauseAllNonScriptedEntities(bool b){if(b){assert(state->pauseDepth==0);++state->pauseDepth;++state->pauseCalls;}else{assert(state->pauseDepth==1);--state->pauseDepth;}}float GetHealth(const CScriptThing&m){assert(m.id==-3);return state->health;}void GiveHeroYesNoQuestion(const CCharString&q,const CCharString&y,const CCharString&n,const CCharString&e,bool b){assert(q.s=="TEXT_QST_B10_TRADER_BUY_MUSHROOM_QUESTION"&&y.s=="TEXT_OBJECT_HERO_ANSWER_YES"&&n.s=="TEXT_OBJECT_HERO_ANSWER_NO"&&e.s.empty()&&b&&state->pauseDepth==1);++state->questions;state->answerReady=state->frame+state->answerDelay;}long MsgIsQuestionAnsweredYesOrNo(){assert(state->questions&&state->pauseDepth==1);++state->answerReads;return state->frame<state->answerReady?-1:state->answer;}long GetHeroGold(){++state->goldReads;return state->changeGold&&state->goldReads>1?state->secondGold:state->gold;}void GiveHeroObject(const CCharString&s,long n,bool b){assert(s.s=="OBJECT_TEST_INGREDIENT"&&n==-1&&!b&&state->pauseDepth==1);++state->heroObjects;state->events.push_back(1);}void RemoveThing(const CScriptThing&m,bool a,bool b){assert(m.id==1&&!a&&b);state->alive=false;++state->removals;state->events.push_back(2);}void GiveHeroGold(long n){assert(n==-1500&&state->heroObjects>0&&state->removals>0&&state->pauseDepth==1);state->gold+=n;state->events.push_back(3);}void EntityGiveGold(const CScriptThing&m,long n){self(m);assert(n==1500&&state->pauseDepth==1);state->entityGold+=static_cast<int>(n);state->events.push_back(4);}void GiveHeroMorality(float n){assert(state->bought&&state->pauseDepth==1&&(n==3.5f||n==-2.5f));state->morality.push_back(n);state->events.push_back(5);}void AddCrimeCommitted(const CScriptThing&v,ECrime c,bool b,const CScriptThing*x,const CScriptThing*y,EOpinionPostDeedType p){assert(v.id==2&&c==CRIME_THEFT&&b&&!x&&!y&&p==OPINION_POST_DEED_TYPE_ALL&&state->pauseDepth==1&&state->bought&&state->stole);++state->crime;state->events.push_back(6);}};
class CWideScreenMagicPauseEntities{CGameScriptInterfaceBase*Game;public:CWideScreenMagicPauseEntities(CGameScriptInterfaceBase&g):Game(&g){Game->PauseAllNonScriptedEntities(true);}~CWideScreenMagicPauseEntities(){Game->PauseAllNonScriptedEntities(false);}};
namespace NScript{class CIsThingAlive{CScriptThing Thing;public:CIsThingAlive(const CScriptThing&m):Thing(m){assert(m.id==-1);}};class CV_SickChildScript{public:class CIngredientOwner{public:CGameScriptInterfaceBase&Game;CScriptThing Me;bool&BoughtSpecialStuff;bool&StoleSpecialStuff;CIngredientOwner(CGameScriptInterfaceBase&g):Game(g),Me(-1),BoughtSpecialStuff(state->bought),StoleSpecialStuff(state->stole){}void SetPredicate(const CIsThingAlive&){++state->predicates;}bool IsActiveThreadTerminating()const{return state->term();}void Main();};};}
void NScript::CV_SickChildScript::CIngredientOwner::Main()
{
    TRetVoid _Fail_Val;
    SetPredicate(CIsThingAlive(Me));
    Game.NewScriptFrame();
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    CScriptGameResourceObjectScriptedThingBase seh_me;
    if (seh_me.IsValid()) seh_me.Release();
    while (!Game.StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    CScriptThing MySpecialStuff=Game.GetThingWithScriptName("Ingredient");
    if (BoughtSpecialStuff) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        Game.RemoveThing(MySpecialStuff,false,true);
    } else {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        Game.SetThingHasInformation(Me,false,true,false);
    }
    Game.SetIsPushableByHero(Me,false);
    Game.SetIsThingForcePushable(Me,false);
    CTimer HeroTooCloseDelay;
    for (; !false; Game.NewScriptFrame()) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (!BoughtSpecialStuff) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                CScriptGameResourceObjectMovieBase speech_movie;
                Game.StartMovieSequence("",speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
                if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                    seh_me.Speak(Game.GetHero(),"TEXT_QST_B10_TRADER_INTRO",GROUP_SELECT_NONE,false,true,false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game.NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                Game.GiveHeroYesNoQuestion("TEXT_QST_B10_TRADER_BUY_MUSHROOM_QUESTION","TEXT_OBJECT_HERO_ANSWER_YES","TEXT_OBJECT_HERO_ANSWER_NO","",true);
                long YNQAnswer;
                while ((YNQAnswer=Game.MsgIsQuestionAnsweredYesOrNo())<0) {
                    Game.NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (YNQAnswer==1 && Game.GetHeroGold()>=1500) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    Game.GiveHeroObject(pGlobalScriptDef->IngredientObject,-1,false);
                    Game.RemoveThing(MySpecialStuff,false,true);
                    Game.GiveHeroGold(-1500);
                    Game.EntityGiveGold(Me,1500);
                    if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                        seh_me.Speak(Game.GetHero(),"TEXT_QST_B10_TRADER_BOUGHT_MUSHROOM_10",GROUP_SELECT_NONE,false,true,false);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game.NewScriptFrame();
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    BoughtSpecialStuff=true;
                    Game.GiveHeroMorality(pGlobalScriptDef->BoughtMushroomInBarrowFields);
                    Game.ClearThingHasInformation(Me);
                } else if (YNQAnswer==1 && Game.GetHeroGold()<1500) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                        seh_me.Speak(Game.GetHero(),"TEXT_QST_B10_TRADER_NOT_ENOUGH_MONEY_10",GROUP_SELECT_NONE,false,true,false);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game.NewScriptFrame();
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                } else {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                        seh_me.Speak(Game.GetHero(),"TEXT_QST_B10_TRADER_NOT_BOUGHT_MUSHROOM_10",GROUP_SELECT_NONE,false,true,false);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game.NewScriptFrame();
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                }
            } else {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (!StoleSpecialStuff) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    CScriptGameResourceObjectMovieBase speech_movie;
                    Game.StartMovieSequence("",speech_movie);
                    CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
                    if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                        seh_me.Speak(Game.GetHero(),"TEXT_QST_B10_TRADER_POST_BOUGHT_MUSHROOM_10",GROUP_SELECT_NONE,false,true,false);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game.NewScriptFrame();
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                } else {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    CScriptGameResourceObjectMovieBase speech_movie;
                    Game.StartMovieSequence("",speech_movie);
                    CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
                    if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                        seh_me.Speak(Game.GetHero(),"TEXT_QST_B10_TRADER_POST_STOLE_MUSHROOM_10",GROUP_SELECT_NONE,false,true,false);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game.NewScriptFrame();
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                }
            }
        }
        if (!BoughtSpecialStuff && MySpecialStuff.IsDead()) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            CScriptGameResourceObjectMovieBase speech_movie;
            Game.StartMovieSequence("",speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
            if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                seh_me.Speak(Game.GetHero(),"TEXT_QST_B10_TRADER_STOLE_MUSHROOM_10",GROUP_SELECT_NONE,false,true,false);
                while (seh_me.IsPerformingScriptTask()) {
                    Game.NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            BoughtSpecialStuff=true;
            StoleSpecialStuff=true;
            Game.GiveHeroMorality(pGlobalScriptDef->StoleMushroomInBarrowFields);
            Game.ClearThingHasInformation(Me);
            Game.AddCrimeCommitted(Game.GetThingWithScriptName("VILLAGE_BARROWFIELDS"),CRIME_THEFT,true,0,0,OPINION_POST_DEED_TYPE_ALL);
        }
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
}


int scenarios=0;void run(State&s){++scenarios;state=&s;{CGameScriptInterfaceBase game;NScript::CV_SickChildScript::CIngredientOwner owner(game);owner.Main();}assert(s.things==0&&s.strings==0&&s.resources==0&&s.movies==0&&s.pauseDepth==0&&s.predicates==1&&s.timers==0&&s.timerCreates==s.timerDestroys);assert(s.heroObjects<=1&&s.crime<=1);if(s.crime)assert(s.bought&&s.stole);}
int count(const State&s,int code){int n=0;for(unsigned i=0;i<s.speech.size();++i)if(s.speech[i]==code)++n;return n;}
int main(){
 {State s;run(s);assert(s.starts==1&&s.info==1&&s.speech.empty()&&s.timerCreates==1);}
 {State s;s.talk=true;s.answer=1;run(s);assert(s.heroObjects==1&&s.gold==0&&s.entityGold==1500&&s.bought&&!s.stole&&s.morality.size()==1&&s.morality[0]==3.5f&&s.crime==0&&s.clears==1&&count(s,0)==1&&count(s,1)==1&&count(s,4)==4);assert(s.events[1]==1&&s.events[2]==2&&s.events[3]==3&&s.events[4]==4);}
 {State s;s.alive=false;run(s);assert(s.bought&&s.stole&&s.heroObjects==0&&s.gold==1500&&s.crime==1&&count(s,6)==1&&s.morality.size()==1&&s.morality[0]==-2.5f);}
 for(int flags=0;flags<16;++flags)for(int answer=0;answer<4;++answer)for(int g=0;g<4;++g){State s;s.limit=2;s.bought=(flags&1)!=0;s.stole=(flags&2)!=0;s.talk=(flags&4)!=0;s.alive=(flags&8)!=0;s.answer=answer==3?-1:answer;s.gold=g==0?-1:g==1?1499:g==2?1500:1501;bool initialBought=s.bought,initialStole=s.stole,initialAlive=s.alive;run(s);bool buy=!initialBought&&s.talk&&answer==1&&g>=2;bool wait=!initialBought&&s.talk&&answer==3;bool steal=!initialBought&&!buy&&!wait&&!initialAlive;assert(s.heroObjects==(buy?1:0)&&s.crime==(steal?1:0));assert(s.bought==(initialBought||buy||steal)&&s.stole==(initialStole||steal));assert(s.entityGold==(buy?1500:0)&&s.info==(initialBought?0:1)&&s.removals==(initialBought||buy?1:0));if(initialBought&&s.talk)assert(count(s,initialStole?5:4)==1);if(!initialBought&&s.talk){assert(count(s,0)==1);if(!wait)assert(count(s,buy?1:answer==1?2:3)==1);}if(steal)assert(count(s,6)==1);}
 float health[5]={-1.0f,0.0f,0.5f,12.0f,std::numeric_limits<float>::quiet_NaN()};
 for(int h=0;h<5;++h)for(int path=0;path<7;++path){State s;s.limit=2;s.health=health[h];s.talk=path!=6;s.answer=path==0||path==1?1:0;s.gold=path==1?1499:1500;s.bought=path==3||path==4;s.stole=path==4;s.alive=path!=5&&path!=6;run(s);bool positive=h==2||h==3;if(!positive)assert(s.speech.empty());else assert(!s.speech.empty());if(path==0)assert(s.heroObjects==1&&s.bought);if(path>=5)assert(s.crime==1&&s.stole);}
 for(int claim=0;claim<5;++claim)for(int task=0;task<5;++task)for(int delay=0;delay<4;++delay){State s;s.limit=30;s.talk=true;s.answer=1;s.startFailures=claim;s.taskDelay=task;s.answerDelay=delay;run(s);assert(s.heroObjects==1&&s.gold==0&&s.bought&&!s.stole&&s.starts==claim+1);}
 {State s;s.talk=true;s.answer=1;s.gold=1499;s.changeGold=true;s.secondGold=1500;s.limit=2;run(s);assert(s.goldReads==2&&s.heroObjects==0&&count(s,3)==1&&count(s,2)==0);}
 {State s;s.vanishAt=3;s.talk=true;run(s);assert(s.crime==1&&s.stole&&count(s,6)==1&&count(s,5)==2);}
 {State s;s.talk=true;s.answerDelay=99;run(s);assert(s.questions==1&&s.heroObjects==0&&s.pauseCalls==1&&!s.bought);}
 for(int stop=1;stop<=200;++stop)for(int path=0;path<20;++path){State s;s.stopCheck=stop;s.limit=12;s.talk=true;switch(path){case 0:s.answer=1;break;case 1:s.answer=1;s.gold=1499;break;case 2:break;case 3:s.bought=true;break;case 4:s.bought=true;s.stole=true;break;case 5:s.alive=false;s.talk=false;break;case 6:s.alive=false;break;case 7:s.answer=1;s.taskDelay=3;break;case 8:s.answer=1;s.answerDelay=3;break;case 9:s.startFailures=30;break;case 10:s.health=0;s.answer=1;break;case 11:s.answer=-1;break;case 12:s.talk=false;break;case 13:s.vanishAt=5;break;case 14:s.bought=true;s.health=0;break;case 15:s.alive=false;s.health=0;break;case 16:s.stole=true;s.answer=1;break;case 17:s.gold=1499;s.changeGold=true;s.secondGold=1500;s.answer=1;break;case 18:s.health=std::numeric_limits<float>::quiet_NaN();break;case 19:s.answer=2;break;}run(s);}
 assert(scenarios==4397);std::puts("00ecb0e0_TEST PASS");return 0;
}
