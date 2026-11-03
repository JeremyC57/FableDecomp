#include <cstdio>
#include <cassert>
#include <string>
#include <vector>
#include <map>
#include <limits>
struct State{int frame,limit,checks,stopCheck,things,resources,movies,pauseDepth,paused,unpaused,initialFailures,heroFailures,startMe,startHero,doneFrame,nearFrame,taskDelay,taskUntil,enters,leaves,reminders,faces,fixDepth,clearInfo;bool arena,complete,faceAlive,talk;float health,distance;std::vector<int>unset;std::vector<std::string>events;State():frame(0),limit(4),checks(0),stopCheck(0),things(0),resources(0),movies(0),pauseDepth(0),paused(0),unpaused(0),initialFailures(0),heroFailures(0),startMe(0),startHero(0),doneFrame(0),nearFrame(0),taskDelay(0),taskUntil(0),enters(0),leaves(0),reminders(0),faces(0),fixDepth(0),clearInfo(0),arena(true),complete(false),faceAlive(false),talk(false),health(100),distance(1){}bool term(){++checks;return frame>=limit||(stopCheck&&checks>=stopCheck);}};State*state;
class CCharString{public:std::string text;CCharString(const char*s):text(s){}bool operator<(const CCharString&o)const{return text<o.text;}};
class CScriptThing{public:int id;CScriptThing(int n=-1):id(n){++state->things;}CScriptThing(const CScriptThing&o):id(o.id){++state->things;}~CScriptThing(){--state->things;}bool IsAlive()const{assert(id==1);return state->faceAlive;}bool MsgIsTalkedToBy(const CCharString&s)const{assert(id==-1&&s.text=="SCRIPT_NAME_HERO");return state->talk;}};
class TRetVoid{public:void Return(){}};enum EScriptAIPriority{SCRIPT_AI_PRIORITY_HIGHEST=4};enum ETextGroupSelectionMethod{GROUP_SELECT_RANDOM_NO_REPEAT=2};
class CScriptGameResourceObjectScriptedThingBase{public:bool valid;int id;CScriptGameResourceObjectScriptedThingBase():valid(false),id(0){++state->resources;}CScriptGameResourceObjectScriptedThingBase(const CScriptGameResourceObjectScriptedThingBase&o):valid(o.valid),id(o.id){++state->resources;}~CScriptGameResourceObjectScriptedThingBase(){--state->resources;}CScriptGameResourceObjectScriptedThingBase&operator=(const CScriptGameResourceObjectScriptedThingBase&o){valid=o.valid;id=o.id;return *this;}bool IsValid()const{return valid;}void Release(){assert(valid);valid=false;}CScriptThing GetScriptThing()const{assert(valid);return CScriptThing(id);}void Speak(const CScriptThing&h,const char*s,ETextGroupSelectionMethod m,bool a,bool b,bool c){assert(id==-1&&valid&&h.id==-2&&std::string(s)=="TEXT_QST_037_GUARD_POSH_REMINDER"&&m==GROUP_SELECT_RANDOM_NO_REPEAT&&!a&&b&&!c&&state->pauseDepth==1);++state->reminders;state->taskUntil=state->frame+state->taskDelay;}bool IsPerformingScriptTask()const{return state->frame<state->taskUntil;}};
class CScriptGameResourceObjectMovieBase{public:CScriptGameResourceObjectMovieBase(){++state->movies;}~CScriptGameResourceObjectMovieBase(){assert(state->pauseDepth==0);--state->movies;}};
struct CScriptDef{float BowerstoneIntroPoshGuardProximity;CScriptDef():BowerstoneIntroPoshGuardProximity(10){}};CScriptDef scriptDef;CScriptDef*pGlobalScriptDef=&scriptDef;
bool IsDistanceBetweenThingsUnder(const CScriptThing&h,const CScriptThing&m,float d){assert(h.id==-2&&m.id==-1&&d==10);return state->frame>=state->nearFrame&&state->distance<d;}
namespace NScript{class CQ_BowerstoneTownLifeIntroScript;}
class CGameScriptInterfaceBase{CScriptThing hero;public:bool*done;CGameScriptInterfaceBase():hero(-2),done(0){}void NewScriptFrame(){assert(state->frame<255);++state->frame;if(done&&state->frame>=state->doneFrame)*done=true;}bool StartScriptingEntity(const CScriptThing&m,CScriptGameResourceObjectScriptedThingBase&r,EScriptAIPriority p){assert(p==SCRIPT_AI_PRIORITY_HIGHEST&&(m.id==-1||m.id==-2));int n=m.id==-1?++state->startMe:++state->startHero;int failures=m.id==-1?state->initialFailures:state->heroFailures;r.valid=n>failures;r.id=m.id;return r.valid;}CScriptThing&GetHero(){return hero;}CScriptThing GetThingWithScriptName(const CCharString&s){assert(s.text=="M_BTLI_GuardPoshFaceMe");return CScriptThing(1);}void EntitySetFacingAngleTowardsThing(const CScriptThing&m,const CScriptThing&f,bool b){assert(m.id==-1&&f.id==1&&b);++state->faces;}void StartMovieSequence(const CCharString&s,CScriptGameResourceObjectMovieBase&){assert(s.text.empty()&&state->movies==1);}void PauseAllNonScriptedEntities(bool b){if(b){assert(state->pauseDepth==0);++state->pauseDepth;++state->paused;}else{assert(state->pauseDepth==1);--state->pauseDepth;++state->unpaused;}}void FixMovieSequenceCamera(bool b){assert(state->pauseDepth==1);state->fixDepth+=b?1:-1;assert(state->fixDepth==int(b));}void EntityUnsetThingAsAllyOfThing(const CScriptThing&a,const CScriptThing&b){assert((a.id==-1&&b.id==-2)||(a.id==-2&&b.id==-1));state->unset.push_back(a.id);}void ClearThingHasInformation(const CScriptThing&m){assert(m.id==-1);++state->clearInfo;}float GetHealth(const CScriptThing&m){assert(m.id==-1);return state->health;}};
class CWideScreenMagicPauseEntities{CGameScriptInterfaceBase*Game;public:CWideScreenMagicPauseEntities(CGameScriptInterfaceBase&g):Game(&g){Game->PauseAllNonScriptedEntities(true);}~CWideScreenMagicPauseEntities(){Game->PauseAllNonScriptedEntities(false);}};
void RunCutsceneMacro(const CCharString&s,std::map<CCharString,CScriptGameResourceObjectScriptedThingBase>&m,std::map<CCharString,bool>*b,std::map<CCharString,CCharString>*c,bool d,bool e){assert(!b&&!c&&!d&&e&&state->pauseDepth==1&&state->fixDepth==1&&m.size()==2&&m["HERO"].valid&&m["HERO"].id==-2&&m["Guard"].valid&&m["Guard"].id==-1);if(s.text=="CS_BOWERSTONE_INTRO_POSH_ENTER")++state->enters;else{assert(s.text=="CS_BOWERSTONE_INTRO_POSH_LEAVE");++state->leaves;}}
namespace NScript{class CQ_BowerstoneTownLifeIntroScript{public:bool DoneGuardSpeech,ArenaFinished,PoshGuardComplete;CQ_BowerstoneTownLifeIntroScript():DoneGuardSpeech(state->doneFrame==0),ArenaFinished(state->arena),PoshGuardComplete(state->complete){}class CBowerstoneIntroGuardToPosh{public:CGameScriptInterfaceBase&Game;CScriptThing Me;CQ_BowerstoneTownLifeIntroScript&ParentClass;CBowerstoneIntroGuardToPosh(CGameScriptInterfaceBase&g,CQ_BowerstoneTownLifeIntroScript&p):Game(g),Me(-1),ParentClass(p){}bool IsActiveThreadTerminating()const{return state->term();}void Main();};};}

void NScript::CQ_BowerstoneTownLifeIntroScript::CBowerstoneIntroGuardToPosh::Main()
{
    TRetVoid _Fail_Val;
    CScriptGameResourceObjectScriptedThingBase sehMe;
    if (sehMe.IsValid()) sehMe.Release();
    while (!Game.StartScriptingEntity(Me,sehMe,SCRIPT_AI_PRIORITY_HIGHEST)) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    while (!ParentClass.DoneGuardSpeech) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    CScriptThing faceMe=Game.GetThingWithScriptName("M_BTLI_GuardPoshFaceMe");
    if (faceMe.IsAlive()) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        Game.EntitySetFacingAngleTowardsThing(Me,faceMe,true);
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
    for (;!false;Game.NewScriptFrame()) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        while (!IsDistanceBetweenThingsUnder(Game.GetHero(),Me,pGlobalScriptDef->BowerstoneIntroPoshGuardProximity)) {
            Game.NewScriptFrame();
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        if (ParentClass.ArenaFinished) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (!ParentClass.PoshGuardComplete) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                std::map<CCharString,CScriptGameResourceObjectScriptedThingBase> mymap;
                CScriptGameResourceObjectScriptedThingBase seh_Hero;
                while (!Game.StartScriptingEntity(Game.GetHero(),seh_Hero,SCRIPT_AI_PRIORITY_HIGHEST)) {
                    Game.NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                mymap["HERO"]=seh_Hero;
                mymap["Guard"]=sehMe;
                CScriptGameResourceObjectMovieBase speech_movie;
                Game.StartMovieSequence("",speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
                Game.FixMovieSequenceCamera(true);
                RunCutsceneMacro("CS_BOWERSTONE_INTRO_POSH_ENTER",mymap,0,0,false,true);
                Game.FixMovieSequenceCamera(false);
                ParentClass.PoshGuardComplete=true;
                if (sehMe.IsValid()) sehMe.Release();
                Game.EntityUnsetThingAsAllyOfThing(Me,Game.GetHero());
                Game.EntityUnsetThingAsAllyOfThing(Game.GetHero(),Me);
                Game.ClearThingHasInformation(Me);
            } else if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                CScriptGameResourceObjectMovieBase speech_movie;
                Game.StartMovieSequence("",speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
                if (sehMe.IsValid()) sehMe.Release();
                while (!Game.StartScriptingEntity(Me,sehMe,SCRIPT_AI_PRIORITY_HIGHEST)) {
                    Game.NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (Game.GetHealth(sehMe.GetScriptThing())>0.0f) {
                    sehMe.Speak(Game.GetHero(),"TEXT_QST_037_GUARD_POSH_REMINDER",GROUP_SELECT_RANDOM_NO_REPEAT,false,true,false);
                    while (sehMe.IsPerformingScriptTask()) {
                        Game.NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (sehMe.IsValid()) sehMe.Release();
            }
        } else {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            std::map<CCharString,CScriptGameResourceObjectScriptedThingBase> mymap;
            CScriptGameResourceObjectScriptedThingBase seh_Hero;
            while (!Game.StartScriptingEntity(Game.GetHero(),seh_Hero,SCRIPT_AI_PRIORITY_HIGHEST)) {
                Game.NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            mymap["HERO"]=seh_Hero;
            mymap["Guard"]=sehMe;
            CScriptGameResourceObjectMovieBase speech_movie;
            Game.StartMovieSequence("",speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
            Game.FixMovieSequenceCamera(true);
            RunCutsceneMacro("CS_BOWERSTONE_INTRO_POSH_LEAVE",mymap,0,0,false,true);
            Game.FixMovieSequenceCamera(false);
        }
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
}

bool run(State&s){state=&s;bool complete;{CGameScriptInterfaceBase g;NScript::CQ_BowerstoneTownLifeIntroScript p;g.done=&p.DoneGuardSpeech;NScript::CQ_BowerstoneTownLifeIntroScript::CBowerstoneIntroGuardToPosh t(g,p);t.Main();complete=p.PoshGuardComplete;assert(s.things==2&&s.resources==0&&s.movies==0&&s.pauseDepth==0&&s.paused==s.unpaused&&s.fixDepth==0);}assert(s.things==0);return complete;}
int main(){int cases=0;
 {State s;assert(run(s));assert(s.enters==1&&s.leaves==0&&s.reminders==0&&s.clearInfo==1&&s.unset.size()==2&&s.unset[0]==-1&&s.unset[1]==-2&&s.startHero==1);++cases;}
 {State s;s.arena=false;assert(!run(s));assert(s.leaves==4&&s.enters==0&&s.clearInfo==0&&s.unset.empty());++cases;}
 {State s;s.complete=true;assert(run(s));assert(s.paused==0&&s.startHero==0&&s.reminders==0);++cases;}
 for(int h=0;h<4;++h){State s;s.complete=true;s.talk=true;s.health=h==0?100.0f:h==1?0.0f:h==2?-1.0f:std::numeric_limits<float>::quiet_NaN();run(s);assert(s.reminders==(h==0?4:0)&&s.paused==4&&s.startMe==5&&s.startHero==0);++cases;}
 for(int d=0;d<4;++d){State s;s.distance=d==0?9.5f:d==1?10.0f:d==2?10.5f:std::numeric_limits<float>::quiet_NaN();run(s);assert(s.enters==(d==0?1:0));++cases;}
 {State s;s.faceAlive=true;s.limit=8;run(s);assert(s.faces==1&&s.enters==1&&s.frame==8);++cases;}
 {State s;s.doneFrame=3;s.limit=4;run(s);assert(s.enters==1&&s.frame==4);++cases;}
 {State s;s.initialFailures=3;s.limit=4;run(s);assert(s.startMe==4&&s.enters==1);++cases;}
 {State s;s.heroFailures=3;s.limit=4;run(s);assert(s.startHero==4&&s.enters==1);++cases;}
 {State s;s.nearFrame=3;s.limit=4;run(s);assert(s.enters==1);++cases;}
 {State s;s.complete=true;s.talk=true;s.taskDelay=100;s.limit=4;run(s);assert(s.reminders==1&&s.paused==1);++cases;}
 for(int mode=0;mode<10;++mode)for(int stop=1;stop<=120;++stop){State s;s.limit=20;s.stopCheck=stop;s.arena=mode!=1;s.complete=mode==2||mode==3||mode==9;s.talk=mode==2||mode==3||mode==9;s.taskDelay=mode==3?5:0;s.initialFailures=mode==4?5:0;s.heroFailures=mode==5?5:0;s.doneFrame=mode==6?5:0;s.nearFrame=mode==7?5:0;s.faceAlive=mode==8;s.health=mode==9?0:100;run(s);assert(s.checks<=stop);++cases;}
 std::fprintf(stderr,"SCENARIOS %d\n",cases);assert(cases==1217);std::puts("00d24a40_TEST PASS");}
