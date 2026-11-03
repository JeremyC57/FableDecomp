#include <cstdio>
#include <cassert>
#include <string>
#include <vector>
#include <limits>
struct State{int frame,limit,checks,stopCheck,things,strings,resources,movies,pauseDepth,paused,unpaused,manualUnpause,predicates,startAttempts,startFailures,taskDelay,taskUntil,initialCounter,followers,questions,answerAt,answer,questionChecks,healthChecks,cameras,registered,deregistered,setTimers,followCalls,healthBars,timerBars,displayed,vectorCalls;bool talk;float health;std::vector<std::string>speech;std::vector<int>alphaIds,hiddenIds;std::vector<float>alphas;std::vector<bool>hidden;State():frame(0),limit(2),checks(0),stopCheck(0),things(0),strings(0),resources(0),movies(0),pauseDepth(0),paused(0),unpaused(0),manualUnpause(0),predicates(0),startAttempts(0),startFailures(0),taskDelay(0),taskUntil(0),initialCounter(0),followers(2),questions(0),answerAt(0),answer(1),questionChecks(0),healthChecks(0),cameras(0),registered(0),deregistered(0),setTimers(0),followCalls(0),healthBars(0),timerBars(0),displayed(0),vectorCalls(0),talk(true),health(100.0f){}bool term(){++checks;return frame>=limit||(stopCheck&&checks>=stopCheck);}};State*state;
class CCharString{public:std::string text;CCharString(const char*s):text(s){++state->strings;}CCharString(const CCharString&o):text(o.text){++state->strings;}~CCharString(){--state->strings;}};
class CRGBColour{public:unsigned char B,G,R,A;CRGBColour(unsigned char r,unsigned char g,unsigned char b){Initialise(r,g,b,255);}void Initialise(unsigned char r,unsigned char g,unsigned char b,unsigned char a){R=r;G=g;B=b;A=a;}};
class CScriptThing{public:int id;bool fromHandle;CScriptThing(int n=99,bool h=false):id(n),fromHandle(h){++state->things;}CScriptThing(const CScriptThing&o):id(o.id),fromHandle(o.fromHandle){++state->things;}~CScriptThing(){--state->things;}bool MsgIsTalkedToBy(const CCharString&s)const{assert(id==-1&&s.text=="SCRIPT_NAME_HERO");return state->talk;}};
class TRetVoid{public:void Return(){}};enum EScriptAIPriority{SCRIPT_AI_PRIORITY_HIGHEST=4};enum ETextGroupSelectionMethod{GROUP_SELECT_NONE=0};
class CTimer{public:int index;CTimer():index(17){++state->registered;}~CTimer(){assert(state->registered==1&&state->deregistered==0&&state->resources==0);++state->deregistered;}CTimer&operator=(long n){assert(n==0&&index==17);++state->setTimers;return*this;}};
class CScriptGameResourceObjectScriptedThingBase{public:bool valid;CScriptGameResourceObjectScriptedThingBase():valid(false){++state->resources;}~CScriptGameResourceObjectScriptedThingBase(){assert(state->movies==0&&state->pauseDepth==0);--state->resources;}bool IsValid()const{return valid;}void Release(){assert(valid);valid=false;}CScriptThing GetScriptThing()const{assert(valid);return CScriptThing(-1,true);}void Speak(const CScriptThing&h,const char*s,ETextGroupSelectionMethod m,bool a,bool b,bool c){assert(h.id==-2&&valid&&m==GROUP_SELECT_NONE&&!a&&b&&!c&&state->pauseDepth==1);state->speech.push_back(s);state->taskUntil=state->frame+state->taskDelay;}bool IsPerformingScriptTask()const{return state->frame<state->taskUntil;}};
class CScriptGameResourceObjectMovieBase{public:CScriptGameResourceObjectMovieBase(){++state->movies;assert(state->movies==1);}~CScriptGameResourceObjectMovieBase(){assert(state->pauseDepth==0);--state->movies;}};
class CGameScriptInterfaceBase{CScriptThing hero;public:CGameScriptInterfaceBase():hero(-2){}void NewScriptFrame(){assert(state->frame<255);++state->frame;}bool StartScriptingEntity(const CScriptThing&m,CScriptGameResourceObjectScriptedThingBase&r,EScriptAIPriority p){assert(m.id==-1&&p==SCRIPT_AI_PRIORITY_HIGHEST&&!r.valid);r.valid=++state->startAttempts>state->startFailures;return r.valid;}CScriptThing&GetHero(){return hero;}CScriptThing GetThingWithScriptName(const CCharString&s){assert(s.text=="Barber2");return CScriptThing(2);}void EntitySetAsAllowedToFollowHero(const CScriptThing&t,bool b){assert(b&&t.id==(state->followCalls==0?-1:2));++state->followCalls;}long AddQuestInfoBarHealth(const CScriptThing&t,const CRGBColour&c,const CCharString&s,float f){assert(s.text=="HUD_ICON_MONEY_BAG"&&f==1.0f&&c.A==255);int n=state->healthBars++%3;if(n==0)assert(t.id==-1&&c.R==0&&c.G==0&&c.B==255);else if(n==1)assert(t.id==2&&c.R==255&&c.G==0&&c.B==0);else assert(t.id==-2&&c.R==0&&c.G==255&&c.B==0);return 33;}long AddQuestInfoTimer(const CTimer&t,const CCharString&s,float f){assert(t.index==17&&s.text=="HUD_ICON_MONEY_BAG"&&f==1.0f);++state->timerBars;return 44;}void DisplayQuestInfo(bool b){assert(b);++state->displayed;}long GetAllCreaturesExcludingHero(std::vector<CScriptThing>&v){assert(v.empty());++state->vectorCalls;for(int i=0;i<state->followers;++i)v.push_back(CScriptThing(3+i));return state->followers;}void EntitySetAlpha(const CScriptThing&t,float f,bool b){assert(b&&t.id>=3&&t.id<3+state->followers&&(f==0.0f||f==1.0f));state->alphaIds.push_back(t.id);state->alphas.push_back(f);}void EntitySetAsHiddenOnMiniMap(const CScriptThing&t,bool b){assert(state->alphaIds.size()==state->hiddenIds.size()+1&&state->alphaIds.back()==t.id&&state->alphas.back()==(b?0.0f:1.0f));state->hiddenIds.push_back(t.id);state->hidden.push_back(b);}void GiveHeroYesNoQuestion(const CCharString&a,const CCharString&b,const CCharString&c,const CCharString&d,bool e){assert(a.text=="Sure?"&&b.text=="Yep"&&c.text=="Oh Yes"&&d.text=="Who cares?"&&e);++state->questions;}long MsgIsQuestionAnsweredYesOrNo(){++state->questionChecks;return state->frame>=state->answerAt?state->answer:-1;}float GetHealth(const CScriptThing&m){assert(m.id==-1&&m.fromHandle&&state->pauseDepth==1);++state->healthChecks;return state->health;}void StartMovieSequence(const CCharString&s,CScriptGameResourceObjectMovieBase&){assert(s.text.empty()&&state->movies==1);}void PauseAllNonScriptedEntities(bool b){if(b){assert(state->pauseDepth==0);++state->pauseDepth;++state->paused;}else if(state->pauseDepth){assert(state->movies==1);--state->pauseDepth;++state->unpaused;}else{assert(state->movies==0);++state->manualUnpause;}}void CameraDefault(){assert(state->pauseDepth==0);++state->cameras;}};
class CWideScreenMagicPauseEntities{CGameScriptInterfaceBase*Game;public:CWideScreenMagicPauseEntities(CGameScriptInterfaceBase&g):Game(&g){Game->PauseAllNonScriptedEntities(true);}~CWideScreenMagicPauseEntities(){Game->PauseAllNonScriptedEntities(false);}};
namespace NScript{class CIsThingAlive{public:CIsThingAlive(const CScriptThing&m){assert(m.id==-1);}};class CPersonalScript_alextestScript{public:class CBurningPerson{public:CGameScriptInterfaceBase&Game;CScriptThing Me;long SpeakCounter;CBurningPerson(CGameScriptInterfaceBase&g):Game(g),Me(-1),SpeakCounter(state->initialCounter){}void SetPredicate(const CIsThingAlive&){++state->predicates;}bool IsActiveThreadTerminating()const{return state->term();}void Main();};};}

void NScript::CPersonalScript_alextestScript::CBurningPerson::Main()
{
    TRetVoid _Fail_Val;
    SetPredicate(CIsThingAlive(Me));
    Game.NewScriptFrame();
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    long TimerIndex=-1;
    long CounterIndex=-1;
    long TickIndex=-1;
    CScriptThing light_marker;
    CTimer TestTimer;
    TestTimer=0;
    bool Toggle=false;
    CScriptThing Barber2=Game.GetThingWithScriptName("Barber2");
    Game.EntitySetAsAllowedToFollowHero(Me,true);
    Game.EntitySetAsAllowedToFollowHero(Barber2,true);
    for (; !false; Game.NewScriptFrame()) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        while (!Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            Game.NewScriptFrame();
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        CScriptGameResourceObjectScriptedThingBase seh_me;
        if (seh_me.IsValid()) seh_me.Release();
        while (!Game.StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
            Game.NewScriptFrame();
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        if (SpeakCounter==0) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            SpeakCounter=1;
            Game.AddQuestInfoBarHealth(Me,CRGBColour(0,0,255),"HUD_ICON_MONEY_BAG",1.0f);
            Game.AddQuestInfoTimer(TestTimer,"HUD_ICON_MONEY_BAG",1.0f);
            Game.AddQuestInfoTimer(TestTimer,"HUD_ICON_MONEY_BAG",1.0f);
            Game.AddQuestInfoBarHealth(Barber2,CRGBColour(255,0,0),"HUD_ICON_MONEY_BAG",1.0f);
            Game.AddQuestInfoBarHealth(Game.GetHero(),CRGBColour(0,255,0),"HUD_ICON_MONEY_BAG",1.0f);
            Game.DisplayQuestInfo(true);
            {
                CScriptGameResourceObjectMovieBase speech_movie;
                Game.StartMovieSequence("",speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
                if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                    seh_me.Speak(Game.GetHero(),"TEXT_CS_B13_MAGICMAN_WAKEUP_DRUNK_10",GROUP_SELECT_NONE,false,true,false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game.NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
            }
            std::vector<CScriptThing> Followers;
            Game.GetAllCreaturesExcludingHero(Followers);
            for (long i=0;i<Followers.size();++i) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                Game.EntitySetAlpha(Followers[i],0.0f,true);
                Game.EntitySetAsHiddenOnMiniMap(Followers[i],true);
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        } else if (SpeakCounter==1) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            SpeakCounter=2;
            Game.GiveHeroYesNoQuestion("Sure?","Yep","Oh Yes","Who cares?",true);
            long answer;
            while (!((answer=Game.MsgIsQuestionAnsweredYesOrNo())>=0)) {
                Game.NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            std::vector<CScriptThing> Followers;
            Game.GetAllCreaturesExcludingHero(Followers);
            for (long i=0;i<Followers.size();++i) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                Game.EntitySetAlpha(Followers[i],1.0f,true);
                Game.EntitySetAsHiddenOnMiniMap(Followers[i],false);
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        } else if (SpeakCounter==2) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            SpeakCounter=0;
            CScriptGameResourceObjectMovieBase speech_movie;
            Game.StartMovieSequence("",speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
            if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                seh_me.Speak(Game.GetHero(),"Gottle of geer.",GROUP_SELECT_NONE,false,true,false);
                while (seh_me.IsPerformingScriptTask()) {
                    Game.NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
        }
        while (seh_me.IsPerformingScriptTask()) {
            Game.NewScriptFrame();
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        Game.CameraDefault();
        Game.PauseAllNonScriptedEntities(false);
        if (SpeakCounter==1) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        } else if (SpeakCounter==2) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        } else if (SpeakCounter==0) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
    }
}

long run(State&s){state=&s;long result;{CGameScriptInterfaceBase g;NScript::CPersonalScript_alextestScript::CBurningPerson t(g);t.Main();result=t.SpeakCounter;assert(s.things==2&&s.strings==0&&s.resources==0&&s.movies==0&&s.pauseDepth==0&&s.paused==s.unpaused&&s.predicates==1&&s.registered==s.deregistered);}assert(s.things==0);return result;}
int main(){int cases=0;const char*first="TEXT_CS_B13_MAGICMAN_WAKEUP_DRUNK_10";const char*third="Gottle of geer.";
 {State s;assert(run(s)==1&&s.registered==1&&s.setTimers==1&&s.followCalls==2&&s.healthBars==3&&s.timerBars==2&&s.displayed==1&&s.speech.size()==1&&s.speech[0]==first&&s.alphaIds.size()==2&&s.hidden[0]&&s.hidden[1]&&s.cameras==1&&s.manualUnpause==1);++cases;}
 {State s;s.initialCounter=1;assert(run(s)==2&&s.questions==1&&s.questionChecks==1&&s.healthBars==0&&s.speech.empty()&&s.alphaIds.size()==2&&!s.hidden[0]&&!s.hidden[1]&&s.cameras==1);++cases;}
 {State s;s.initialCounter=2;assert(run(s)==0&&s.speech.size()==1&&s.speech[0]==third&&s.healthBars==0&&s.alphaIds.empty()&&s.cameras==1);++cases;}
 for(int initial=-2;initial<=4;++initial){State s;s.initialCounter=initial;long expect=initial>=0&&initial<=2?(initial+1)%3:initial;assert(run(s)==expect&&s.cameras==1&&s.manualUnpause==1);if(initial<0||initial>2)assert(s.speech.empty()&&s.questions==0&&s.alphaIds.empty());++cases;}
 {State s;s.limit=5;assert(run(s)==1&&s.cameras==4&&s.manualUnpause==4&&s.healthBars==6&&s.timerBars==4&&s.displayed==2&&s.questions==1&&s.speech.size()==3&&s.speech[0]==first&&s.speech[1]==third&&s.speech[2]==first&&s.vectorCalls==3);++cases;}
 for(int count=0;count<=4;++count)for(int stage=0;stage<=2;++stage){State s;s.followers=count;s.initialCounter=stage;assert(run(s)==(stage+1)%3);assert(s.alphaIds.size()==unsigned(stage==2?0:count)&&s.hiddenIds==s.alphaIds);for(unsigned i=0;i<s.alphaIds.size();++i)assert(s.alphaIds[i]==int(i)+3&&s.alphas[i]==(stage==0?0.0f:1.0f));++cases;}
 for(int h=0;h<4;++h)for(int stage=0;stage<=2;stage+=2){State s;s.initialCounter=stage;s.health=h==0?1.0f:h==1?0.0f:h==2?-1.0f:std::numeric_limits<float>::quiet_NaN();assert(run(s)==(stage+1)%3&&s.healthChecks==1&&s.speech.size()==unsigned(h==0)&&s.cameras==1&&s.paused==1);++cases;}
 for(int answer=-2;answer<=2;++answer){State s;s.initialCounter=1;s.answer=answer;s.limit=4;run(s);if(answer<0)assert(s.questionChecks==3&&s.vectorCalls==0&&s.cameras==0);else assert(s.questions==1&&s.vectorCalls==2&&s.cameras==3);++cases;}
 {State s;s.initialCounter=1;s.answerAt=4;s.limit=5;assert(run(s)==2&&s.questionChecks==4&&s.cameras==1&&s.vectorCalls==1);++cases;}
 {State s;s.talk=false;s.limit=5;assert(run(s)==0&&s.startAttempts==0&&s.healthBars==0&&s.registered==1);++cases;}
 {State s;s.startFailures=2;s.limit=4;assert(run(s)==1&&s.startAttempts==3&&s.cameras==1);++cases;}
 {State s;s.startFailures=100;s.limit=5;assert(run(s)==0&&s.startAttempts==4&&s.healthBars==0);++cases;}
 {State s;s.taskDelay=100;s.limit=5;assert(run(s)==1&&s.speech.size()==1&&s.vectorCalls==0&&s.cameras==0&&s.paused==1);++cases;}
 for(int mode=0;mode<12;++mode)for(int stop=1;stop<=120;++stop){State s;s.limit=20;s.stopCheck=stop;s.initialCounter=mode%4;s.talk=mode!=4;s.startFailures=mode==5?5:0;s.taskDelay=mode==6||mode==7?4:0;s.answerAt=mode==8?8:0;s.answer=mode==9?-1:1;s.followers=mode==10?0:mode==11?5:2;s.health=mode==3?0.0f:100.0f;run(s);assert(s.checks<=stop);++cases;}
 assert(cases==1484);std::puts("00eedc20_TEST PASS");}