// Exact selected00eaafe0 body; six expected literals independently read from native constant addresses.
#include <cstdio>
#include <cassert>
#include <string>
#include <vector>
#include <limits>
struct State{int frame,limit,checks,stopCheck,things,strings,resources,movies,pauseDepth,paused,unpaused,predicates,startAttempts,startFailures,taskDelay,taskUntil,initialBrain,event,firstLow,secondLow,modified,removed,speechChecks,healthMask,physicalChecks,abilityChecks,healChecks;bool physical,ability,heal,talk;float speechHealth,fightHealth;std::vector<std::string>speech;State():frame(0),limit(8),checks(0),stopCheck(0),things(0),strings(0),resources(0),movies(0),pauseDepth(0),paused(0),unpaused(0),predicates(0),startAttempts(0),startFailures(0),taskDelay(0),taskUntil(0),initialBrain(0),event(5),firstLow(100),secondLow(100),modified(0),removed(0),speechChecks(0),healthMask(-1),physicalChecks(0),abilityChecks(0),healChecks(0),physical(false),ability(false),heal(false),talk(false),speechHealth(100),fightHealth(1){}bool term(){++checks;return frame>=limit||(stopCheck&&checks>=stopCheck);}};State*state;
class CCharString{public:std::string text;CCharString(const char*s):text(s){++state->strings;}CCharString(const CCharString&o):text(o.text){++state->strings;}~CCharString(){--state->strings;}};
class CScriptThing{public:int id;bool fromHandle;CScriptThing(int n=-1,bool h=false):id(n),fromHandle(h){++state->things;}CScriptThing(const CScriptThing&o):id(o.id),fromHandle(o.fromHandle){++state->things;}~CScriptThing(){--state->things;}void target(const CCharString&s)const{assert(id==-1&&s.text=="SCRIPT_NAME_HERO");}bool MsgIsTalkedToBy(const CCharString&s)const{target(s);return state->talk;}bool MsgIsHitBy(const CCharString&s)const{target(s);++state->physicalChecks;return state->physical;}bool MsgIsHitByAnySpecialAbilityFrom(const CCharString&s)const{target(s);++state->abilityChecks;return state->ability;}bool MsgIsHitBySpecialAbilityFrom(int a,const CCharString&s)const{target(s);assert(a==14);++state->healChecks;return state->heal;}};
class TRetVoid{public:void Return(){}};enum EScriptAIPriority{SCRIPT_AI_PRIORITY_HIGHEST=4};enum ETextGroupSelectionMethod{GROUP_SELECT_NONE=0};enum EHeroAbility{HERO_ABILITY_HEAL_LIFE_SPELL=14};
class CScriptGameResourceObjectScriptedThingBase{public:bool valid;CScriptGameResourceObjectScriptedThingBase():valid(false){++state->resources;}~CScriptGameResourceObjectScriptedThingBase(){assert(state->movies==0&&state->pauseDepth==0);--state->resources;}bool IsValid()const{return valid;}void Release(){valid=false;}CScriptThing GetScriptThing()const{assert(valid);return CScriptThing(-1,true);}void Speak(const CScriptThing&h,const char*s,ETextGroupSelectionMethod m,bool a,bool b,bool c){assert(h.id==-2&&valid&&m==GROUP_SELECT_NONE&&!a&&b&&!c&&state->pauseDepth==1);state->speech.push_back(s);state->taskUntil=state->frame+state->taskDelay;}bool IsPerformingScriptTask()const{return state->frame<state->taskUntil;}};
class CScriptGameResourceObjectMovieBase{public:CScriptGameResourceObjectMovieBase(){++state->movies;assert(state->movies==1);}~CScriptGameResourceObjectMovieBase(){assert(state->pauseDepth==0);--state->movies;}};
class CGameScriptInterfaceBase{CScriptThing hero;public:CGameScriptInterfaceBase():hero(-2){}void NewScriptFrame(){assert(state->frame<255);++state->frame;}bool StartScriptingEntity(const CScriptThing&m,CScriptGameResourceObjectScriptedThingBase&r,EScriptAIPriority p){assert(m.id==-1&&p==SCRIPT_AI_PRIORITY_HIGHEST);r.valid=++state->startAttempts>state->startFailures;return r.valid;}CScriptThing&GetHero(){return hero;}float GetHealth(const CScriptThing&m){assert(m.id==-1);if(m.fromHandle){assert(state->pauseDepth==1);int n=state->speechChecks++;return state->healthMask>=0?((state->healthMask&(1<<n))?100.0f:0.0f):state->speechHealth;}return state->frame>=(state->modified?state->secondLow:state->firstLow)?state->fightHealth:100.0f;}void StartMovieSequence(const CCharString&s,CScriptGameResourceObjectMovieBase&){assert(s.text.empty()&&state->movies==1);}void PauseAllNonScriptedEntities(bool b){if(b){assert(state->pauseDepth==0);++state->pauseDepth;++state->paused;}else{assert(state->pauseDepth==1);--state->pauseDepth;++state->unpaused;}}void ModifyThingHealth(const CScriptThing&m,float f,bool b){assert(m.id==-1&&f==10.0f&&!b&&state->pauseDepth==1);++state->modified;}void RemoveThing(const CScriptThing&m,bool a,bool b){assert(m.id==-1&&a&&b&&state->pauseDepth==0);++state->removed;}};
class CWideScreenMagicPauseEntities{CGameScriptInterfaceBase*Game;public:CWideScreenMagicPauseEntities(CGameScriptInterfaceBase&g):Game(&g){Game->PauseAllNonScriptedEntities(true);}~CWideScreenMagicPauseEntities(){Game->PauseAllNonScriptedEntities(false);}};
namespace NScript{class CIsThingAlive{public:CIsThingAlive(const CScriptThing&m){assert(m.id==-1);}};class CV_MurderTwistScript{public:enum MURDER_TWIST_BROTHER_STATES{MTBS_NONE=0,MTBS_FIGHTING=1,MTBS_NEARLY_DEAD=2,MTBS_DEAD=3};enum MURDER_TWIST_EVENTS{MTE_FIND_BROTHER_BS_FRIEND=4};long MurderTwistEvent;CV_MurderTwistScript():MurderTwistEvent(state->event){}class CMurderTwistBrother{public:CGameScriptInterfaceBase&Game;CScriptThing Me;CV_MurderTwistScript&ParentClass;long BrainState;CMurderTwistBrother(CGameScriptInterfaceBase&g,CV_MurderTwistScript&p):Game(g),Me(-1),ParentClass(p),BrainState(state->initialBrain){}void SetPredicate(const CIsThingAlive&){++state->predicates;}bool IsActiveThreadTerminating()const{return state->term();}void Main();};};}

void NScript::CV_MurderTwistScript::CMurderTwistBrother::Main()
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
    while (BrainState==MTBS_NONE) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            CScriptGameResourceObjectMovieBase speech_movie;
            Game.StartMovieSequence("",speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
            if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                seh_me.Speak(Game.GetHero(),"I had heard you might be coming for the gem.",GROUP_SELECT_NONE,false,true,false);
                while (seh_me.IsPerformingScriptTask()) {
                    Game.NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                seh_me.Speak(Game.GetHero(),"Haha... you'll never find it...",GROUP_SELECT_NONE,false,true,false);
                while (seh_me.IsPerformingScriptTask()) {
                    Game.NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
        }
        if (Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL,"SCRIPT_NAME_HERO"))) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            CScriptGameResourceObjectMovieBase speech_movie;
            Game.StartMovieSequence("",speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
            if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                seh_me.Speak(Game.GetHero(),"HEY! That Gem belongs to me... I don't know who you are, or who you're working for but that Gem was given to me as an inheritance.",GROUP_SELECT_NONE,false,true,false);
                while (seh_me.IsPerformingScriptTask()) {
                    Game.NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                seh_me.Speak(Game.GetHero(),"If it's a fight you want, it's a fight ye'll get, but you'll not find the Gem!",GROUP_SELECT_NONE,false,true,false);
                while (seh_me.IsPerformingScriptTask()) {
                    Game.NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            BrainState=MTBS_FIGHTING;
        }
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    while (BrainState==MTBS_FIGHTING) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        if (Game.GetHealth(Me)==1.0f) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            CScriptGameResourceObjectMovieBase speech_movie;
            Game.StartMovieSequence("",speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
            if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                seh_me.Speak(Game.GetHero(),"Argghh... okay... I'll tell you where I put it. I gave it to a friend to look after as I knew you were coming. He is in Greatwood...",GROUP_SELECT_NONE,false,true,false);
                while (seh_me.IsPerformingScriptTask()) {
                    Game.NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                seh_me.Speak(Game.GetHero(),"You win hero... there is no need to kill me.",GROUP_SELECT_NONE,false,true,false);
                while (seh_me.IsPerformingScriptTask()) {
                    Game.NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            ParentClass.MurderTwistEvent=MTE_FIND_BROTHER_BS_FRIEND;
            BrainState=MTBS_NEARLY_DEAD;
            Game.ModifyThingHealth(Me,10.0f,false);
        }
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    while (BrainState==MTBS_NEARLY_DEAD) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        if (Game.GetHealth(Me)==1.0f) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            BrainState=MTBS_DEAD;
        }
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    Game.RemoveThing(Me,true,true);
}

long run(State&s){state=&s;long brain;{CGameScriptInterfaceBase g;NScript::CV_MurderTwistScript p;NScript::CV_MurderTwistScript::CMurderTwistBrother t(g,p);t.Main();brain=t.BrainState;s.event=p.MurderTwistEvent;assert(s.things==2&&s.strings==0&&s.resources==0&&s.movies==0&&s.pauseDepth==0&&s.paused==s.unpaused&&s.predicates==1);}assert(s.things==0);return brain;}
int main(){int cases=0;
 const char*lines[]={"I had heard you might be coming for the gem.","Haha... you'll never find it...","HEY! That Gem belongs to me... I don't know who you are, or who you're working for but that Gem was given to me as an inheritance.","If it's a fight you want, it's a fight ye'll get, but you'll not find the Gem!","Argghh... okay... I'll tell you where I put it. I gave it to a friend to look after as I knew you were coming. He is in Greatwood...","You win hero... there is no need to kill me."};
 {State s;assert(run(s)==0&&s.speech.empty()&&s.removed==0&&s.paused==0);++cases;}
 {State s;s.talk=true;s.limit=5;assert(run(s)==0&&s.speech.size()==6&&s.paused==3);for(int i=0;i<6;++i)assert(s.speech[i]==lines[i%2]);++cases;}
 {State s;s.talk=true;s.physical=true;s.firstLow=3;s.secondLow=5;assert(run(s)==3&&s.event==4&&s.modified==1&&s.removed==1&&s.frame==5&&s.speech.size()==6&&s.paused==3);for(int i=0;i<6;++i)assert(s.speech[i]==lines[i]);++cases;}
 for(int initial=-1;initial<=4;++initial){State s;s.initialBrain=initial;s.firstLow=2;s.secondLow=3;long b=run(s);if(initial==0)assert(b==0&&s.removed==0);else if(initial==1)assert(b==3&&s.event==4&&s.modified==1&&s.speech.size()==2);else if(initial==2)assert(b==3&&s.modified==0&&s.event==5&&s.speech.empty());else assert(b==initial&&s.removed==1&&s.frame==1);++cases;}
 for(int flags=0;flags<8;++flags){State s;s.limit=3;s.physical=(flags&1)!=0;s.ability=(flags&2)!=0;s.heal=(flags&4)!=0;bool hit=s.physical||(s.ability&&!s.heal);assert(run(s)==int(hit));assert(s.physicalChecks==1&&s.abilityChecks==int(!s.physical)&&s.healChecks==int(!s.physical&&s.ability)&&s.speech.size()==unsigned(hit?2:0));++cases;}
 for(int h=0;h<4;++h){State s;s.talk=true;s.limit=3;s.speechHealth=h==0?100.0f:h==1?0.0f:h==2?-1.0f:std::numeric_limits<float>::quiet_NaN();run(s);assert(s.speech.size()==unsigned(h==0?2:0)&&s.speechChecks==2&&s.paused==1);++cases;}
 for(int h=0;h<4;++h){State s;s.initialBrain=1;s.firstLow=0;s.fightHealth=h==0?0.5f:h==1?1.0f:h==2?1.5f:std::numeric_limits<float>::quiet_NaN();assert(run(s)==(h==1?2:1));assert(s.modified==int(h==1)&&s.event==(h==1?4:5));++cases;}
 for(int mask=0;mask<64;++mask){State s;s.talk=true;s.physical=true;s.firstLow=3;s.secondLow=5;s.healthMask=mask;assert(run(s)==3&&s.modified==1&&s.event==4&&s.speechChecks==6);int n=0;for(int i=0;i<6;++i)if(mask&(1<<i)){assert(s.speech[n]==lines[i]);++n;}assert(s.speech.size()==unsigned(n));++cases;}
 {State s;s.startFailures=3;s.limit=8;run(s);assert(s.startAttempts==4&&s.paused==0);++cases;}
 {State s;s.startFailures=100;s.limit=5;run(s);assert(s.startAttempts==4&&s.speech.empty());++cases;}
 {State s;s.talk=true;s.taskDelay=100;s.limit=5;run(s);assert(s.speech.size()==1&&s.paused==1&&s.unpaused==1);++cases;}
 {State s;s.physical=true;s.taskDelay=2;s.firstLow=0;s.secondLow=0;s.limit=15;assert(run(s)==3&&s.modified==1&&s.speech.size()==4&&s.removed==1);++cases;}
 for(int mode=0;mode<10;++mode)for(int stop=1;stop<=100;++stop){State s;s.limit=20;s.stopCheck=stop;s.talk=mode==1||mode==4;s.physical=mode>=2&&mode<=5;s.taskDelay=mode==4||mode==5?4:0;s.firstLow=mode>=3?3:100;s.secondLow=mode>=3?6:100;s.initialBrain=mode==6?1:mode==7?2:mode==8?3:0;s.startFailures=mode==9?5:0;run(s);assert(s.checks<=stop);++cases;}
 assert(cases==1093);std::puts("00eaafe0_TEST PASS");}