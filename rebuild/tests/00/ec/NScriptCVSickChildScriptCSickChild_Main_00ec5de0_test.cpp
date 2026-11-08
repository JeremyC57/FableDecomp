// Standalone exact selected Main and math bodies; external engine services are deterministic dependency doubles.
#include "fable_compiler.h"
#include <cassert>
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
#include <cmath>
#ifndef _MSC_VER
#define __fastcall
#define __forceinline inline
#endif
struct Config {
    bool talk,hit,special,heal,nearHero,physics; int fail,wait,stop,limit,cureAt,maxLine,direction; float health;
    Config():talk(false),hit(false),special(false),heal(false),nearHero(false),physics(false),fail(0),wait(0),stop(0),limit(8),cureAt(2),maxLine(20),direction(0),health(100.0f){}
};
static Config config;
static int strings,things,handles,movies,pauses,predicates,owners,timers;
static int frame,checks,failures,tasks,starts,talkCalls,hitCalls,anyCalls,healCalls,healthCalls,timerValue,timerReads,nearCalls,conversations,timerSets,allyCalls;
static bool stopped;static bool* finished;
static std::vector<std::string> events,speeches,lines;
static void action(const char*s){assert(!stopped);events.push_back(s);}
class TRetVoid{public:void Return(){}};
class CCharString{public:std::string text;CCharString():text(){++strings;}CCharString(const char*s):text(s){++strings;}CCharString(const CCharString&s):text(s.text){++strings;}CCharString&operator=(const CCharString&s){text=s.text;return *this;}~CCharString(){--strings;}operator const char*()const{return text.c_str();}};
static CCharString GFIntToCharString(long n){char b[50];std::sprintf(b,"%ld",n);return CCharString(b);}
static CCharString operator+(const char*s,const CCharString&t){return CCharString((std::string(s)+t.text).c_str());}
enum EScriptAIPriority{SCRIPT_AI_PRIORITY_HIGHEST=4};enum ETextGroupSelectionMethod{GROUP_SELECT_NONE=0};enum EHeroAbility{HERO_ABILITY_HEAL_LIFE_SPELL=14};
class C3DVector{public:float X,Y,Z;float GetAngleXY()const;};
class CScriptThing{public:int id;CScriptThing(int i=0):id(i){++things;}CScriptThing(const CScriptThing&t):id(t.id){++things;}~CScriptThing(){--things;}bool MsgIsTalkedToBy(const CCharString&s)const{assert(id==1&&s.text=="SCRIPT_NAME_HERO");++talkCalls;return config.talk;}bool MsgIsHitBy(const CCharString&s)const{assert(id==1&&s.text=="SCRIPT_NAME_HERO");++hitCalls;return config.hit;}bool MsgIsHitByAnySpecialAbilityFrom(const CCharString&s)const{assert(id==1&&s.text=="SCRIPT_NAME_HERO");++anyCalls;return config.special;}bool MsgIsHitBySpecialAbilityFrom(EHeroAbility a,const CCharString&s)const{assert(id==1&&a==14&&s.text=="SCRIPT_NAME_HERO");++healCalls;return config.heal;}};
class CScriptGameResourceObjectScriptedThingBase{bool owned;public:CScriptGameResourceObjectScriptedThingBase():owned(false){++handles;}~CScriptGameResourceObjectScriptedThingBase(){if(owned)--owners;--handles;events.push_back("resource_release");}void acquire(){assert(!owned);owned=true;++owners;}bool IsValid()const{return owned;}void Release(){action("release");if(owned)--owners;owned=false;}CScriptThing GetScriptThing()const{assert(owned);return CScriptThing(1);}void Speak(const CScriptThing&t,const char*s,ETextGroupSelectionMethod m,bool a,bool b,bool c){assert(owned&&t.id==2&&m==0&&!a&&b&&!c);action("speak");speeches.push_back(s);tasks=config.wait;}bool IsPerformingScriptTask()const{return tasks>0;}void ClearAllActionsIncludingLoopingAnimations(){assert(owned);action("clear");}void PlayLoopingAnimation(const CCharString&s,long n,bool a,bool b,bool c,bool d,bool e,bool f,bool g){assert(owned&&s.text=="sleep"&&n==-1&&!a&&!b&&!c&&d&&e==config.physics&&!f&&!g);action("animation");}};
class CScriptGameResourceObjectMovieBase{public:CScriptGameResourceObjectMovieBase(){++movies;}~CScriptGameResourceObjectMovieBase(){assert(pauses==0);--movies;events.push_back("movie_release");}};
class CGameScriptInterfaceBase{CScriptThing hero;public:CGameScriptInterfaceBase():hero(2){}
void NewScriptFrame(){action("frame");++frame;if(tasks)--tasks;if(timerValue>0)--timerValue;if(finished&&frame>=config.cureAt)*finished=true;}
bool StartScriptingEntity(const CScriptThing&t,CScriptGameResourceObjectScriptedThingBase&s,EScriptAIPriority p){assert(p==4&&t.id==1);action("start");++starts;if(failures){--failures;return false;}s.acquire();return true;}
void StartMovieSequence(const CCharString&s,CScriptGameResourceObjectMovieBase&){assert(s.text.empty()&&movies==1);action("movie");}
void PauseAllNonScriptedEntities(bool b){if(b){action("pause");++pauses;}else{assert(pauses==1&&movies==1);--pauses;events.push_back("unpause");}}
float GetHealth(const CScriptThing&t){assert(t.id==1);++healthCalls;return config.health;}
CScriptThing&GetHero(){return hero;}
CScriptThing GetThingWithScriptName(const CCharString&s){if(s.text=="SickChildBed"){action("bed_lookup");return CScriptThing(3);}assert(s.text=="MK_SC_BOY");action("marker_lookup");return CScriptThing(4);}
void GetSleepingPositionAndOrientationFromBed(const CScriptThing&t,const CScriptThing&b,C3DVector&p,C3DVector&d){assert(t.id==1&&b.id==3);action("bed_position");p.X=2;p.Y=4;p.Z=8;d.X=config.direction==1?1.0f:config.direction==3?-1.0f:0.0f;d.Y=config.direction==0?1.0f:config.direction==2?-1.0f:0.0f;d.Z=0;}
void EntityTeleportToPosition(const CScriptThing&t,const C3DVector&p,float angle,bool a,bool b){assert(t.id==1&&p.X==2&&p.Y==4&&p.Z==8&&!a&&!b);assert(std::fabs(angle-config.direction*0.25f)<0.000001f);action("teleport_bed");}
void SetBedAvailability(const CScriptThing&t,bool b){assert(t.id==3);action(b?"bed_available":"bed_unavailable");}
void SetThingAsUsable(const CScriptThing&t,bool b){assert(t.id==3&&!b);action("bed_unusable");}
void EntitySetTargetable(const CScriptThing&t,bool b){assert(t.id==1);action(b?"target_on":"target_off");}
void EntitySetAsRespondToHit(const CScriptThing&t,bool b){assert(t.id==1);action(b?"respond_on":"respond_off");}
void EntitySetAsKillable(const CScriptThing&t,bool a,bool b){assert(t.id==1&&!a&&!b);action("unkillable");}
void EntitySetAsToAddToComboMultiplierWhenHit(const CScriptThing&t,bool a){assert(t.id==1&&!a);action("combo_off");}
long AddNewConversation(const CScriptThing&t,bool a,bool b){assert(t.id==1&&!a&&!b);action("conversation");return ++conversations;}
void AddPersonToConversation(long n,const CScriptThing&t){assert(n==conversations&&t.id==2);action("person");}
bool TextEntryExists(const CCharString&s){const std::string prefix="TEXT_QST_B10_BOY_JABBERS_";assert(s.text.find(prefix)==0);action("text");long n=0;std::sscanf(s.text.c_str()+prefix.size(),"%ld",&n);return n<=config.maxLine;}
void AddLineToConversation(long n,const CCharString&s,bool a,const CScriptThing&t,const CScriptThing&h){assert(n==conversations&&!a&&t.id==1&&h.id==2);action("line");lines.push_back(s.text);}
void EntityTeleportToThing(const CScriptThing&t,const CScriptThing&m,bool b){assert(t.id==1&&m.id==4&&!b);action("teleport_marker");}
void EntitySetThingAsAllyOfThing(const CScriptThing&a,const CScriptThing&b){assert((a.id==1&&b.id==2)||(a.id==2&&b.id==1));assert((allyCalls%2==0?a.id==1:a.id==2));++allyCalls;action("ally");}
long RegisterTimer(){assert(timers==0);action("timer_register");++timers;timerValue=0;return 37;}
void DeregisterTimer(long n){assert(n==37&&timers==1&&movies==0&&pauses==0&&owners==1);--timers;events.push_back("timer_release");}
void SetTimer(long n,long v){assert(n==37&&v==20);action("timer_set");timerValue=(int)v;++timerSets;}
long GetTimer(long n){assert(n==37&&timers==1);++timerReads;return timerValue;}
};
static CGameScriptInterfaceBase* pDSTGame;
class CTimer{long TimerIndex;public:CTimer();~CTimer();CTimer&operator=(long);long GetTime()const;bool operator==(long)const;};
inline CTimer::CTimer(){TimerIndex=pDSTGame->RegisterTimer();}
inline CTimer::~CTimer(){pDSTGame->DeregisterTimer(TimerIndex);}
inline CTimer& CTimer::operator=(long time){pDSTGame->SetTimer(TimerIndex,time);return *this;}
inline long CTimer::GetTime()const{return pDSTGame->GetTimer(TimerIndex);}
inline bool CTimer::operator==(long time)const{return GetTime()==time;}
class CWideScreenMagicPauseEntities{CGameScriptInterfaceBase*Game;public:CWideScreenMagicPauseEntities(CGameScriptInterfaceBase&game):Game(&game){Game->PauseAllNonScriptedEntities(true);}~CWideScreenMagicPauseEntities(){Game->PauseAllNonScriptedEntities(false);}};
class CScriptDef{ public:const char*SickChildSleepingAnim;};static CScriptDef definition={"sleep"};static CScriptDef*pGlobalScriptDef=&definition;
namespace NGlobalConsole{bool GDefaultToUsingPhysicsInCutsceneAnimations;}
static bool IsDistanceBetweenThingsUnder(const CScriptThing&a,const CScriptThing&b,float d){assert(a.id==1&&b.id==2&&d==14);++nearCalls;return config.nearHero;}
namespace NScript{class CIsThingAlive{public:CIsThingAlive(const CScriptThing&t){assert(t.id==1);++predicates;}~CIsThingAlive(){--predicates;}};class CV_SickChildScript{public:bool FinishedQuest;class CSickChild{CGameScriptInterfaceBase&Game;CScriptThing Me;CV_SickChildScript&ParentClass;void Main();public:CSickChild(CGameScriptInterfaceBase&g,CV_SickChildScript&p):Game(g),Me(1),ParentClass(p){}void Run(){Main();}void SetPredicate(const CIsThingAlive&){action("predicate");}bool IsActiveThreadTerminating()const{++checks;if((config.stop&&checks>=config.stop)||frame>=config.limit){stopped=true;return true;}return false;}};};}
inline float __fastcall GFArcTan(double opposite,double adjacent)
{ return (float)atan2(opposite,adjacent)/(2.0f*3.14159265358979323846f); }
// Fidelity: proven original header math; stronger expansion follows the owner header-helper rule, not a recovered keyword.
FABLE_FORCEINLINE float __fastcall GET_ROUNDED_ANGLE(float angle)
{
    if(angle<0.0f || angle>=1.0f) {
        angle=(float)fmod(angle,1.0f);
        if(angle<0.0f) angle+=1.0f;
    }
    return angle;
}
FABLE_FORCEINLINE float C3DVector::GetAngleXY() const { return GET_ROUNDED_ANGLE(GFArcTan(X,Y)); }

void NScript::CV_SickChildScript::CSickChild::Main()
{
    TRetVoid _Fail_Val;
    SetPredicate(CIsThingAlive(Me));
    Game.NewScriptFrame();
    if(IsActiveThreadTerminating()) return _Fail_Val.Return();
    CScriptGameResourceObjectScriptedThingBase seh_me;
    if(seh_me.IsValid()) seh_me.Release();
    while(!Game.StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
        Game.NewScriptFrame();
        if(IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
    if(IsActiveThreadTerminating()) return _Fail_Val.Return();
    CTimer GarbledSpeak;
    C3DVector Position,Direction;
    Game.GetSleepingPositionAndOrientationFromBed(Me,Game.GetThingWithScriptName("SickChildBed"),Position,Direction);
    Game.EntityTeleportToPosition(Me,Position,Direction.GetAngleXY(),false,false);
    seh_me.PlayLoopingAnimation(pGlobalScriptDef->SickChildSleepingAnim,-1,false,false,false,true,NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations,false,false);
    Game.SetBedAvailability(Game.GetThingWithScriptName("SickChildBed"),false);
    Game.SetThingAsUsable(Game.GetThingWithScriptName("SickChildBed"),false);
    Game.EntitySetTargetable(Me,false);
    Game.EntitySetAsRespondToHit(Me,false);
    Game.EntitySetAsKillable(Me,false,false);
    Game.EntitySetAsToAddToComboMultiplierWhenHit(Me,false);
    long LineNumber=10;
    while(!ParentClass.FinishedQuest) {
        Game.NewScriptFrame();
        if(IsActiveThreadTerminating()) return _Fail_Val.Return();
        if((GarbledSpeak==0 && IsDistanceBetweenThingsUnder(Me,Game.GetHero(),14.0f)) || Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if(IsActiveThreadTerminating()) return _Fail_Val.Return();
            long HLStyleConversationIndex=Game.AddNewConversation(Me,false,false);
            Game.AddPersonToConversation(HLStyleConversationIndex,Game.GetHero());
            CCharString Text;
            CCharString FullText;
            FullText="TEXT_QST_B10_BOY_JABBERS_"+GFIntToCharString(LineNumber);
            if(!Game.TextEntryExists(FullText)) {
                LineNumber=10;
                FullText="TEXT_QST_B10_BOY_JABBERS_"+GFIntToCharString(LineNumber);
            }
            LineNumber+=10;
            Game.AddLineToConversation(HLStyleConversationIndex,FullText,false,Me,Game.GetHero());
            GarbledSpeak=20;
        }
    }
    if(IsActiveThreadTerminating()) return _Fail_Val.Return();
    Game.SetBedAvailability(Game.GetThingWithScriptName("SickChildBed"),true);
    Game.EntityTeleportToThing(Me,Game.GetThingWithScriptName("MK_SC_BOY"),false);
    seh_me.ClearAllActionsIncludingLoopingAnimations();
    Game.EntitySetAsRespondToHit(Me,true);
    Game.EntitySetTargetable(Me,true);
    for(;true;Game.NewScriptFrame()) {
        if(IsActiveThreadTerminating()) return _Fail_Val.Return();
        if(Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if(IsActiveThreadTerminating()) return _Fail_Val.Return();
            CScriptGameResourceObjectMovieBase speech_movie;
            Game.StartMovieSequence("",speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
            if(Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                seh_me.Speak(Game.GetHero(),"TEXT_QST_B10_BOY_CURED_10",GROUP_SELECT_NONE,false,true,false);
                while(seh_me.IsPerformingScriptTask()) {
                    Game.NewScriptFrame();
                    if(IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if(IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
        }
        if(Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL,"SCRIPT_NAME_HERO"))) {
            if(IsActiveThreadTerminating()) return _Fail_Val.Return();
            CScriptGameResourceObjectMovieBase speech_movie;
            Game.StartMovieSequence("",speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
            if(Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                seh_me.Speak(Game.GetHero(),"TEXT_QST_B10_MOTHER_HIT",GROUP_SELECT_NONE,false,true,false);
                while(seh_me.IsPerformingScriptTask()) {
                    Game.NewScriptFrame();
                    if(IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if(IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            Game.EntitySetThingAsAllyOfThing(Me,Game.GetHero());
            Game.EntitySetThingAsAllyOfThing(Game.GetHero(),Me);
        }
    }
    if(IsActiveThreadTerminating()) return _Fail_Val.Return();
}

struct Result{std::vector<std::string> events,speech,line;int checks,starts,talk,hit,any,heal,health,nearReads,timerSets,allies;};
static int scenarios;
static Result run(const Config&c){config=c;frame=-1;checks=tasks=starts=talkCalls=hitCalls=anyCalls=healCalls=healthCalls=timerReads=nearCalls=conversations=timerSets=allyCalls=0;failures=c.fail;events.clear();speeches.clear();lines.clear();stopped=false;NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations=c.physics;Result r;{CGameScriptInterfaceBase g;pDSTGame=&g;NScript::CV_SickChildScript p;p.FinishedQuest=c.cureAt<=0;finished=&p.FinishedQuest;NScript::CV_SickChildScript::CSickChild t(g,p);t.Run();}finished=0;pDSTGame=0;assert(strings==0&&things==0&&handles==0&&movies==0&&pauses==0&&predicates==0&&owners==0&&timers==0);r.events=events;r.speech=speeches;r.line=lines;r.checks=checks;r.starts=starts;r.talk=talkCalls;r.hit=hitCalls;r.any=anyCalls;r.heal=healCalls;r.health=healthCalls;r.nearReads=nearCalls;r.timerSets=timerSets;r.allies=allyCalls;++scenarios;return r;}
static int count(const Result&r,const char*s){int n=0;for(unsigned i=0;i<r.events.size();++i)if(r.events[i]==s)++n;return n;}
static void sweep(Config c){Result r=run(c);for(int i=1;i<=r.checks;++i){c.stop=i;Result q=run(c);assert(q.checks>=i||frame>=c.limit);}}
int main(){Config c;Result r;
 c=Config();r=run(c);assert(r.speech.empty()&&r.line.empty()&&r.allies==0&&r.starts==1);assert(count(r,"bed_unavailable")==1&&count(r,"bed_available")==1&&count(r,"clear")==1);sweep(c);
 for(int dir=0;dir<4;++dir)for(int physics=0;physics<2;++physics){c=Config();c.direction=dir;c.physics=physics!=0;sweep(c);}
 c=Config();c.talk=true;c.cureAt=5;r=run(c);assert(r.line.size()==5&&r.speech.size()==3&&r.timerSets==5);for(unsigned i=0;i<r.line.size();++i)assert(r.line[i]==(i%2?"TEXT_QST_B10_BOY_JABBERS_20":"TEXT_QST_B10_BOY_JABBERS_10"));for(unsigned i=0;i<r.speech.size();++i)assert(r.speech[i]=="TEXT_QST_B10_BOY_CURED_10");sweep(c);
 c=Config();c.nearHero=true;c.cureAt=50;c.limit=45;r=run(c);assert(r.line.size()==3&&r.timerSets==3&&r.speech.empty()&&count(r,"bed_available")==0);assert(r.nearReads==3);sweep(c);
 c=Config();c.talk=true;c.maxLine=0;c.cureAt=4;r=run(c);for(unsigned i=0;i<r.line.size();++i)assert(r.line[i]=="TEXT_QST_B10_BOY_JABBERS_10");sweep(c);
 for(int flags=0;flags<8;++flags)for(int h=0;h<4;++h){c=Config();c.cureAt=0;c.hit=(flags&1)!=0;c.special=(flags&2)!=0;c.heal=(flags&4)!=0;union{unsigned int bits;float value;}n;n.bits=0x7fc00000;c.health=h==0?-1.0f:h==1?0.0f:h==2?1.0f:n.value;r=run(c);bool eligible=c.hit||(c.special&&!c.heal);assert(r.allies==int(eligible)*16);assert(r.speech.size()==unsigned(eligible&&c.health>0)*8);assert(r.any==(c.hit?0:8)&&r.heal==(!c.hit&&c.special?8:0));sweep(c);}
 for(int fail=0;fail<3;++fail)for(int wait=0;wait<3;++wait)for(int cue=0;cue<2;++cue){c=Config();c.talk=c.hit=true;c.fail=fail;c.wait=wait;c.cureAt=cue?4:0;r=run(c);assert(r.starts==fail+1);sweep(c);}
 for(int wait=0;wait<3;++wait){c=Config();c.talk=true;c.cureAt=0;c.wait=wait;c.health=0.0f;r=run(c);assert(r.speech.empty()&&r.health==8&&count(r,"movie")==8);sweep(c);}
 assert(GET_ROUNDED_ANGLE(-0.25f)==0.75f);assert(GET_ROUNDED_ANGLE(1.25f)==0.25f);assert(GET_ROUNDED_ANGLE(2.0f)==0.0f);assert(GET_ROUNDED_ANGLE(0.5f)==0.5f);
 assert(scenarios>1000);std::puts("00ec5de0_TEST PASS");}