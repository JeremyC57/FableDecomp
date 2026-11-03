// Selected20: same original Main body; stronger timer helper audited independently.
#include <cstdio>
#include <cassert>
#include <string>
#include <vector>
#include <limits>
struct Item{std::string name;long slot,priority;Item(const std::string&n,long s,long p):name(n),slot(s),priority(p){}};
struct State{int frame,limit,checks,stopCheck,things,timers,resources,movies,pauseDepth,paused,unpaused,predicates,startAttempts,startFailures,information,questions,questionFrame,answer,answerDelay,taskDelay,taskUntil,gameDelay,gameUntil,games,fadeout,fadein,pauses,conversations,people;bool initialIntro,talk[256],teleportActive;float health,distance;std::vector<std::string>speech,lines,cleanup,teleports;std::vector<int>toutFrames;std::vector<long>timerSets;std::vector<Item>items;State():frame(0),limit(40),checks(0),stopCheck(0),things(0),timers(0),resources(0),movies(0),pauseDepth(0),paused(0),unpaused(0),predicates(0),startAttempts(0),startFailures(0),information(0),questions(0),questionFrame(0),answer(0),answerDelay(0),taskDelay(0),taskUntil(0),gameDelay(0),gameUntil(0),games(0),fadeout(0),fadein(0),pauses(0),conversations(0),people(0),initialIntro(false),teleportActive(true),health(100),distance(5){for(int i=0;i<256;++i)talk[i]=false;}bool term(){++checks;return frame>=limit||(stopCheck&&checks>=stopCheck);}};State*state;
class CCharString{public:std::string text;CCharString(const char*s):text(s){}};
class CScriptThing{public:int id;CScriptThing(int i=-1):id(i){++state->things;}CScriptThing(const CScriptThing&t):id(t.id){++state->things;}~CScriptThing(){--state->things;}bool MsgIsTalkedToBy(const CCharString&s)const{assert(id==-1&&s.text=="SCRIPT_NAME_HERO");return state->talk[state->frame];}};
class TRetVoid{public:void Return(){}};enum EScriptAIPriority{SCRIPT_AI_PRIORITY_HIGHEST=4};enum ETextGroupSelectionMethod{GROUP_SELECT_NONE=0};
class CTimer{long until;public:CTimer():until(0){++state->timers;}~CTimer(){--state->timers;state->cleanup.push_back("timer");}CTimer&operator=(long n){assert(n==10||n==25);until=state->frame+n;state->timerSets.push_back(n);return *this;}bool operator==(long n)const{return(until<=state->frame?0:until-state->frame)==n;}};
class CScriptGameResourceObjectScriptedThingBase{public:bool valid;CScriptGameResourceObjectScriptedThingBase():valid(false){++state->resources;}~CScriptGameResourceObjectScriptedThingBase(){assert(state->pauseDepth==0&&state->movies==0&&state->timers==0);--state->resources;state->cleanup.push_back("resource");}bool IsValid(){return valid;}void Release(){assert(valid);valid=false;}CScriptThing GetScriptThing(){assert(valid);return CScriptThing(-1);}void Speak(const CScriptThing&h,const char*s,ETextGroupSelectionMethod method,bool a,bool b,bool c){assert(h.id==-2&&method==GROUP_SELECT_NONE&&!a&&b&&!c&&valid&&state->pauseDepth==1);state->speech.push_back(s);state->taskUntil=state->frame+state->taskDelay;}bool IsPerformingScriptTask(){return state->frame<state->taskUntil;}};
class CScriptGameResourceObjectMovieBase{public:CScriptGameResourceObjectMovieBase(){++state->movies;}~CScriptGameResourceObjectMovieBase(){assert(state->pauseDepth==0);--state->movies;state->cleanup.push_back("movie");}};
struct CRGBColour{unsigned char R,G,B,A;CRGBColour(unsigned char r,unsigned char g,unsigned char b,unsigned char a):R(r),G(g),B(b),A(a){}};
bool IsDistanceBetweenThingsUnder(const CScriptThing&m,const CScriptThing&h,float d){assert(m.id==-1&&h.id==-2&&d==20);return state->distance<d;}
class CGameScriptInterfaceBase{CScriptThing hero;public:CGameScriptInterfaceBase():hero(-2){}void NewScriptFrame(){assert(state->frame<255);++state->frame;}bool StartScriptingEntity(const CScriptThing&m,CScriptGameResourceObjectScriptedThingBase&r,EScriptAIPriority p){assert(m.id==-1&&p==SCRIPT_AI_PRIORITY_HIGHEST);++state->startAttempts;r.valid=state->startAttempts>state->startFailures;return r.valid;}void SetThingHasInformation(const CScriptThing&m,bool a,bool b,bool c){assert(m.id==-1&&!a&&!b&&!c);++state->information;}void StartMovieSequence(const CCharString&s,CScriptGameResourceObjectMovieBase&){assert(s.text.empty()&&state->movies==1);}void PauseAllNonScriptedEntities(bool b){if(b){assert(!state->pauseDepth);++state->paused;++state->pauseDepth;}else{assert(state->pauseDepth==1);++state->unpaused;--state->pauseDepth;state->cleanup.push_back("unpause");}}float GetHealth(const CScriptThing&m){assert(m.id==-1);return state->health;}CScriptThing&GetHero(){return hero;}void GiveHeroYesNoQuestion(const CCharString&q,const CCharString&y,const CCharString&n,const CCharString&empty,bool b){assert(q.text=="TEXT_QST_B03_MINIGAME_OFFER_GAME"&&y.text=="TEXT_OBJECT_HERO_ANSWER_YES"&&n.text=="TEXT_OBJECT_HERO_ANSWER_NO"&&empty.text.empty()&&b&&state->pauseDepth==1);++state->questions;state->questionFrame=state->frame;}long MsgIsQuestionAnsweredYesOrNo(){return state->frame<state->questionFrame+state->answerDelay?-1:state->answer;}void SetPreferredQuickAccessItem(const CCharString&s,long slot,long priority){assert(slot>=0&&slot<4&&(priority==1000000||priority==-1));const char*names[]={"OBJECT_EXPRESSION_Y_DUMMY","OBJECT_EXPRESSION_A_DUMMY","OBJECT_EXPRESSION_M_DUMMY","OBJECT_EXPRESSION_C_DUMMY"};assert(s.text==names[slot]);state->items.push_back(Item(s.text,slot,priority));}bool FadeScreenOut(float a,float b,CRGBColour c){assert(a==0.5f&&b==0.5f&&c.R==0&&c.G==0&&c.B==0&&c.A==255);++state->fadeout;return true;}void FadeScreenIn(){++state->fadein;}void Pause(float n){assert(n==0.5f);++state->pauses;}void SetTeleportingAsActive(bool b){state->teleportActive=b;}CScriptThing GetThingWithScriptName(const CCharString&s){int id=s.text=="HeroOracleMarker"?1:s.text=="ScytheOracleMarker"?2:s.text=="Cam_ForSymbols"?3:0;assert(id);return CScriptThing(id);}void EntityTeleportToThing(const CScriptThing&m,const CScriptThing&t,bool b){assert(!b&&!state->teleportActive);assert((m.id==-2&&t.id==1)||(m.id==-1&&t.id==2));state->teleports.push_back(m.id==-2?"hero":"gamesmaster");}void CameraUseCameraPoint(const CScriptThing&t,const void*a,const void*b,float f,long n,long z){assert(t.id==3&&!a&&!b&&f==-1.0f&&n==0&&z==-1);}void HeroPlayOracleMinigame(){++state->games;state->gameUntil=state->frame+state->gameDelay;}bool IsHeroPlayingOracleMinigame(){return state->frame<state->gameUntil;}long AddNewConversation(const CScriptThing&m,bool a,bool b){assert(m.id==-1&&!a&&!b&&state->pauseDepth==0);state->toutFrames.push_back(state->frame);return ++state->conversations;}void AddPersonToConversation(long i,const CScriptThing&h){assert(i==state->conversations&&h.id==-2);++state->people;}void AddLineToConversation(long i,const CCharString&s,bool b,const CScriptThing&m,const CScriptThing&h){assert(i==state->conversations&&!b&&m.id==-1&&h.id==-2&&s.text=="TEXT_QST_B03_GAMEMASTER_TOUTING");state->lines.push_back(s.text);}};
class CWideScreenMagicPauseEntities{CGameScriptInterfaceBase*Game;public:CWideScreenMagicPauseEntities(CGameScriptInterfaceBase&g):Game(&g){Game->PauseAllNonScriptedEntities(true);}~CWideScreenMagicPauseEntities(){Game->PauseAllNonScriptedEntities(false);}};
namespace NScript{class CIsThingAlive{public:CIsThingAlive(const CScriptThing&m){assert(m.id==-1);}};class COracleMinigameTestScript{public:class COracleGamesmaster{public:CGameScriptInterfaceBase&Game;CScriptThing Me;bool SaidIntro;COracleGamesmaster(CGameScriptInterfaceBase&g):Game(g),Me(-1),SaidIntro(state->initialIntro){}void SetPredicate(const CIsThingAlive&){++state->predicates;}bool IsActiveThreadTerminating()const{return state->term();}void Main();};};}

void NScript::COracleMinigameTestScript::COracleGamesmaster::Main()
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
    Game.SetThingHasInformation(Me,false,false,false);
    CTimer ToutingTimer;
    ToutingTimer=10;
    for (;!false;Game.NewScriptFrame()) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            CScriptGameResourceObjectMovieBase speech_movie;
            Game.StartMovieSequence("",speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
            if (!SaidIntro) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                    seh_me.Speak(Game.GetHero(),"TEXT_QST_B03_GAMEMASTER_INTRO",GROUP_SELECT_NONE,false,true,false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game.NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                SaidIntro=true;
            } else {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                    seh_me.Speak(Game.GetHero(),"TEXT_QST_B03_GAMEMASTER_RETURN",GROUP_SELECT_NONE,false,true,false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game.NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
            }
            Game.GiveHeroYesNoQuestion("TEXT_QST_B03_MINIGAME_OFFER_GAME","TEXT_OBJECT_HERO_ANSWER_YES","TEXT_OBJECT_HERO_ANSWER_NO","",true);
            long YNQAnswer;
            while (!((YNQAnswer=Game.MsgIsQuestionAnsweredYesOrNo())>=0)) {
                Game.NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (YNQAnswer==1) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                Game.SetPreferredQuickAccessItem("OBJECT_EXPRESSION_Y_DUMMY",0,1000000);
                Game.SetPreferredQuickAccessItem("OBJECT_EXPRESSION_A_DUMMY",1,1000000);
                Game.SetPreferredQuickAccessItem("OBJECT_EXPRESSION_M_DUMMY",2,1000000);
                Game.SetPreferredQuickAccessItem("OBJECT_EXPRESSION_C_DUMMY",3,1000000);
                Game.FadeScreenOut(0.5f,0.5f,CRGBColour(0,0,0,255));
                Game.Pause(0.5f);
                Game.SetTeleportingAsActive(false);
                Game.EntityTeleportToThing(Game.GetHero(),Game.GetThingWithScriptName("HeroOracleMarker"),false);
                Game.EntityTeleportToThing(Me,Game.GetThingWithScriptName("ScytheOracleMarker"),false);
                Game.CameraUseCameraPoint(Game.GetThingWithScriptName("Cam_ForSymbols"),0,0,-1.0f,0,-1);
                Game.FadeScreenIn();
                Game.Pause(0.5f);
                Game.HeroPlayOracleMinigame();
                while (Game.IsHeroPlayingOracleMinigame()) {
                    Game.NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                Game.FadeScreenOut(0.5f,0.5f,CRGBColour(0,0,0,255));
                Game.Pause(0.5f);
                Game.SetPreferredQuickAccessItem("OBJECT_EXPRESSION_Y_DUMMY",0,-1);
                Game.SetPreferredQuickAccessItem("OBJECT_EXPRESSION_A_DUMMY",1,-1);
                Game.SetPreferredQuickAccessItem("OBJECT_EXPRESSION_M_DUMMY",2,-1);
                Game.SetPreferredQuickAccessItem("OBJECT_EXPRESSION_C_DUMMY",3,-1);
                Game.FadeScreenIn();
                Game.Pause(0.5f);
            }
            Game.SetTeleportingAsActive(true);
        }
        if (ToutingTimer==0 && IsDistanceBetweenThingsUnder(Me,Game.GetHero(),20.0f)) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            long HLStyleConversationIndex=Game.AddNewConversation(Me,false,false);
            Game.AddPersonToConversation(HLStyleConversationIndex,Game.GetHero());
            Game.AddLineToConversation(HLStyleConversationIndex,"TEXT_QST_B03_GAMEMASTER_TOUTING",false,Me,Game.GetHero());
            ToutingTimer=25;
        }
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
}

bool run(State&s){state=&s;bool said;{CGameScriptInterfaceBase g;NScript::COracleMinigameTestScript::COracleGamesmaster t(g);t.Main();said=t.SaidIntro;assert(s.things==2&&s.timers==0&&s.resources==0&&s.movies==0&&s.pauseDepth==0&&s.predicates==1&&s.paused==s.unpaused&&s.conversations==s.people&&s.conversations==int(s.lines.size()));}assert(s.things==0);return said;}
int main(){int cases=0;
 {State s;assert(!run(s));assert(s.conversations==2&&s.toutFrames[0]==11&&s.toutFrames[1]==36&&s.timerSets[0]==10&&s.timerSets[1]==25&&s.questions==0);++cases;}
 for(int mode=0;mode<4;++mode){State s;s.distance=mode==0?19.5f:mode==1?20.0f:mode==2?21.0f:std::numeric_limits<float>::quiet_NaN();run(s);assert(s.conversations==(mode==0?2:0));++cases;}
 for(int intro=0;intro<2;++intro)for(int answer=0;answer<3;++answer)for(int health=0;health<4;++health){State s;s.limit=4;s.talk[1]=true;s.initialIntro=intro!=0;s.answer=answer;s.health=health==0?100.0f:health==1?0.0f:health==2?-1.0f:std::numeric_limits<float>::quiet_NaN();assert(run(s));assert(s.questions==1&&s.speech.size()==unsigned(health==0)&&s.paused==1&&s.teleportActive);if(health==0)assert(s.speech[0]==(intro?"TEXT_QST_B03_GAMEMASTER_RETURN":"TEXT_QST_B03_GAMEMASTER_INTRO"));if(answer==1){assert(s.games==1&&s.items.size()==8&&s.fadeout==2&&s.fadein==2&&s.pauses==4&&s.teleports.size()==2);for(int i=0;i<8;++i)assert(s.items[i].slot==i%4&&s.items[i].priority==(i<4?1000000:-1));}else assert(s.games==0&&s.items.empty()&&s.fadeout==0);++cases;}
 {State s;s.talk[1]=true;s.talk[2]=true;s.limit=4;assert(run(s));assert(s.speech.size()==2&&s.speech[0]=="TEXT_QST_B03_GAMEMASTER_INTRO"&&s.speech[1]=="TEXT_QST_B03_GAMEMASTER_RETURN"&&s.questions==2);++cases;}
 {State s;s.talk[1]=true;s.answer=1;s.answerDelay=3;s.taskDelay=2;s.gameDelay=5;s.limit=20;run(s);assert(s.games==1&&s.items.size()==8&&s.teleportActive&&s.conversations==1&&s.toutFrames[0]==11);++cases;}
 {State s;s.startFailures=3;s.limit=20;run(s);assert(s.startAttempts==4&&s.information==1&&s.toutFrames[0]==14);++cases;}
 {State s;s.startFailures=999;s.limit=5;run(s);assert(s.startAttempts==4&&s.information==0&&s.timerSets.empty());++cases;}
 {State s;s.talk[1]=true;s.answer=-1;s.limit=5;run(s);assert(s.questions==1&&s.games==0&&s.teleportActive&&s.paused==1);++cases;}
 {State s;s.talk[1]=true;s.answer=1;s.gameDelay=999;s.limit=5;run(s);assert(s.games==1&&s.items.size()==4&&!s.teleportActive&&s.paused==1&&s.fadein==1);++cases;}
 for(int mode=0;mode<8;++mode)for(int stop=1;stop<=100;++stop){State s;s.stopCheck=stop;s.limit=40;s.talk[1]=mode!=0;s.initialIntro=mode==1;s.answer=mode>=2?1:0;s.taskDelay=mode==3?5:0;s.answerDelay=mode==4?5:0;s.gameDelay=mode==5?5:0;s.startFailures=mode==6?5:0;s.health=mode==7?0:100;run(s);assert(s.checks<=stop);++cases;}
 assert(cases==835);std::puts("00f000e0_TEST PASS");}
