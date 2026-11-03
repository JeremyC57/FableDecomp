#include <cstdio>
#include <cassert>
#include <string>
#include <vector>
struct Event{bool talk,hit,magic,heal;Event():talk(false),hit(false),magic(false),heal(false){}};
struct State{Event e[128];int frame,limit,checks,stopCheck,starts,startFailures,resources,movies,pauses,predicate,speaks,convs,releases,taskFrames,taskUntil,moves,moveFailures,scared,killed,activeUntil;long initialTimer;float health;std::vector<std::string>text;std::vector<int>pace;std::vector<long>timers;State():frame(0),limit(8),checks(0),stopCheck(0),starts(0),startFailures(0),resources(0),movies(0),pauses(0),predicate(0),speaks(0),convs(0),releases(0),taskFrames(0),taskUntil(0),moves(0),moveFailures(0),scared(0),killed(0),activeUntil(0),initialTimer(0),health(1){}bool term(){++checks;return killed||frame>=limit||(stopCheck&&checks>=stopCheck);}};State*state;
class CCharString{public:std::string text;CCharString(const char*s):text(s){}};
enum EScriptAIPriority{SCRIPT_AI_PRIORITY_HIGHEST=4};enum ETextGroupSelectionMethod{GROUP_SELECT_NONE=0};enum EHeroAbility{HERO_ABILITY_HEAL_LIFE_SPELL=14};enum EScriptEntityMoveType{ENTITY_MOVE_WALK=0,ENTITY_MOVE_RUN=1};class TRetVoid{public:void Return(){}};class C3DVector{public:float X,Y,Z;C3DVector():X(1),Y(2),Z(3){}};
class CScriptThing{public:int kind;CScriptThing(int k=0):kind(k){}bool operator==(const CScriptThing&b)const{return kind==b.kind;}C3DVector GetPos()const{assert(kind==4);return C3DVector();}
 bool MsgIsTalkedToBy(const CCharString&s)const{assert(s.text=="SCRIPT_NAME_HERO");return state->e[state->frame].talk;}
 bool MsgIsHitBy(const CCharString&s)const{assert(s.text=="SCRIPT_NAME_HERO");return state->e[state->frame].hit;}
 bool MsgIsHitByAnySpecialAbilityFrom(const CCharString&s)const{assert(s.text=="SCRIPT_NAME_HERO");return state->e[state->frame].magic;}
 bool MsgIsHitBySpecialAbilityFrom(EHeroAbility a,const CCharString&s)const{assert(a==HERO_ABILITY_HEAL_LIFE_SPELL&&s.text=="SCRIPT_NAME_HERO");return state->e[state->frame].heal;}
};
class CTimer{long end;public:CTimer():end(state->initialTimer){}CTimer&operator=(long n){state->timers.push_back(n);end=state->frame+n;return *this;}bool operator==(long n)const{long t=end<0?end:end-state->frame<0?0:end-state->frame;return t==n;}};
class CScriptGameResourceObjectScriptedThingBase{public:bool valid;CScriptGameResourceObjectScriptedThingBase():valid(false){++state->resources;}~CScriptGameResourceObjectScriptedThingBase(){--state->resources;}bool IsValid()const{return valid;}void Release(){assert(valid);valid=false;++state->releases;}CScriptThing GetScriptThing()const{return CScriptThing(5);}bool IsPerformingScriptTask()const{return state->frame<state->taskUntil;}
 void Speak(const CScriptThing&h,const char*s,ETextGroupSelectionMethod m,bool a,bool b,bool c){assert(h.kind==1&&m==GROUP_SELECT_NONE&&!a&&b&&!c&&state->pauses==1&&state->movies==1);++state->speaks;state->text.push_back(s);state->taskUntil=state->frame+state->taskFrames;}
 void MoveToPosition(const C3DVector&p,float r,EScriptEntityMoveType m,bool a,bool b){assert(p.X==1&&r==0&&m==ENTITY_MOVE_RUN&&!a&&b);++state->moves;state->taskUntil=state->frame+state->taskFrames;}
 void MoveToThing(const CScriptThing&t,float r,EScriptEntityMoveType m,bool a,bool b,bool c,bool d){assert((t.kind==2||t.kind==3)&&r==1&&m==ENTITY_MOVE_WALK&&!a&&b&&!c&&d);state->pace.push_back(t.kind);state->taskUntil=state->frame+state->taskFrames;}
};
class CScriptGameResourceObjectMovieBase{public:CScriptGameResourceObjectMovieBase(){++state->movies;}~CScriptGameResourceObjectMovieBase(){--state->movies;}};
bool IsDistanceFromThingToPositionOver(const CScriptThing&t,const C3DVector&p,float r){assert(t.kind==5&&p.Y==2&&r==2);return state->moves<state->moveFailures;}
void FadeOutAndKillEntity(const CScriptThing&t,float f,bool a,bool b){assert(t.kind==0&&f==1&&a&&b);++state->killed;}
class CGameScriptInterfaceBase{public:CScriptThing hero;CGameScriptInterfaceBase():hero(1){}void NewScriptFrame(){assert(state->frame<127);++state->frame;}
 bool StartScriptingEntity(const CScriptThing&t,CScriptGameResourceObjectScriptedThingBase&r,EScriptAIPriority p){assert(t.kind==0&&p==SCRIPT_AI_PRIORITY_HIGHEST);++state->starts;if(state->starts<=state->startFailures)return false;r.valid=true;return true;}
 CScriptThing GetThingWithScriptName(const CCharString&s){if(s.text=="BS_ImpatientMarker1")return CScriptThing(2);if(s.text=="BS_ImpatientMarker2")return CScriptThing(3);assert(s.text=="BS_RunOffMarker");return CScriptThing(4);}
 void StartMovieSequence(const CCharString&s,CScriptGameResourceObjectMovieBase&){assert(s.text.empty());}void PauseAllNonScriptedEntities(bool b){state->pauses+=b?1:-1;assert(state->pauses>=0&&state->pauses<=1);}float GetHealth(const CScriptThing&t){assert(t.kind==5);return state->health;}CScriptThing&GetHero(){return hero;}
 void EntitySetAsScared(const CScriptThing&t,bool b){assert(t.kind==0&&b);++state->scared;}
 bool IsConversationActive(long c){assert(c>=0);return state->frame<state->activeUntil;}
 long AddNewConversation(const CScriptThing&t,bool a,bool b){assert(t.kind==0&&!a&&!b);return ++state->convs;}
 void AddPersonToConversation(long c,const CScriptThing&t){assert(c==state->convs&&t.kind==1);}
 void AddLineToConversation(long c,const CCharString&s,bool b,const CScriptThing&t,const CScriptThing&h){assert(c==state->convs&&!b&&t.kind==0&&h.kind==1&&s.text=="TEXT_QST_018_TRADER_MUTTERS");}
};
class CWideScreenMagicPauseEntities{CGameScriptInterfaceBase*Game;public:CWideScreenMagicPauseEntities(CGameScriptInterfaceBase&g):Game(&g){Game->PauseAllNonScriptedEntities(true);}~CWideScreenMagicPauseEntities(){Game->PauseAllNonScriptedEntities(false);}};
namespace NScript{class CIsThingAlive{public:CIsThingAlive(const CScriptThing&t){assert(t.kind==0);}};class CQ_BreakSiegeScript{public:class CDisplacedTrader{public:CGameScriptInterfaceBase&Game;CScriptThing Me;long TimesSpokenTo;CTimer RandomMutteringsDelay;CDisplacedTrader(CGameScriptInterfaceBase&g):Game(g),TimesSpokenTo(0){}void SetPredicate(const CIsThingAlive&){++state->predicate;}bool IsActiveThreadTerminating()const{return state->term();}void Main();};};}

void NScript::CQ_BreakSiegeScript::CDisplacedTrader::Main()
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
    CScriptThing PacingMarker=Game.GetThingWithScriptName("BS_ImpatientMarker2");
    CScriptThing Pacing1=Game.GetThingWithScriptName("BS_ImpatientMarker1");
    CScriptThing Pacing2=Game.GetThingWithScriptName("BS_ImpatientMarker2");
    long HLStyleConversationIndex=0;
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    do {
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
            switch (TimesSpokenTo) {
            case 0:
                if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                    seh_me.Speak(Game.GetHero(),"TEXT_QST_018_TRADER_INTRO",GROUP_SELECT_NONE,false,true,false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game.NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                break;
            case 1:
                if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                    seh_me.Speak(Game.GetHero(),"TEXT_QST_018_TRADER_INTRO_REP",GROUP_SELECT_NONE,false,true,false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game.NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                break;
            default:
                if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                    seh_me.Speak(Game.GetHero(),"TEXT_QST_018_TRADER_ON_TALK_AGAIN",GROUP_SELECT_NONE,false,true,false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game.NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                break;
            }
            ++TimesSpokenTo;
        }
        if (Me.MsgIsHitBy("SCRIPT_NAME_HERO") ||
            (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") &&
             !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL,"SCRIPT_NAME_HERO"))) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            Game.EntitySetAsScared(Me,true);
            CScriptThing RunOffPos=Game.GetThingWithScriptName("BS_RunOffMarker");
            {
                EScriptEntityMoveType local_movetype=ENTITY_MOVE_RUN;
                float local_range=0.0f;
                C3DVector local_position=RunOffPos.GetPos();
                while (IsDistanceFromThingToPositionOver(seh_me.GetScriptThing(),local_position,2.0f)) {
                    Game.NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    seh_me.MoveToPosition(local_position,local_range,local_movetype,false,true);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game.NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            FadeOutAndKillEntity(Me,1.0f,true,true);
        }
        if (!seh_me.IsPerformingScriptTask()) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (RandomMutteringsDelay==0 && !Game.IsConversationActive(HLStyleConversationIndex)) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                HLStyleConversationIndex=Game.AddNewConversation(Me,false,false);
                Game.AddPersonToConversation(HLStyleConversationIndex,Game.GetHero());
                Game.AddLineToConversation(HLStyleConversationIndex,"TEXT_QST_018_TRADER_MUTTERS",false,Me,Game.GetHero());
                RandomMutteringsDelay=30;
            }
            seh_me.MoveToThing(PacingMarker,1.0f,ENTITY_MOVE_WALK,false,true,false,true);
            if (PacingMarker==Pacing1) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                PacingMarker=Pacing2;
            } else {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                PacingMarker=Pacing1;
            }
        }
        Game.NewScriptFrame();
    } while (!IsActiveThreadTerminating());
}

long run(State&s,long initial=0){state=&s;CGameScriptInterfaceBase g;NScript::CQ_BreakSiegeScript::CDisplacedTrader t(g);t.TimesSpokenTo=initial;t.Main();assert(s.resources==0&&s.movies==0&&s.pauses==0&&s.predicate==1);return t.TimesSpokenTo;}
int main(){int cases=0;{State s;run(s);assert(s.convs==1&&s.timers.size()==1&&s.timers[0]==30&&s.pace.size()==7);for(unsigned n=0;n<s.pace.size();++n)assert(s.pace[n]==(n%2?2:3));++cases;}
 {State s;for(int n=1;n<4;++n)s.e[n].talk=true;assert(run(s)==3&&s.speaks==3&&s.releases==3&&s.starts==4);assert(s.text[0]=="TEXT_QST_018_TRADER_INTRO"&&s.text[1]=="TEXT_QST_018_TRADER_INTRO_REP"&&s.text[2]=="TEXT_QST_018_TRADER_ON_TALK_AGAIN");++cases;}
 for(int initial=-1;initial<=3;++initial)for(int alive=0;alive<2;++alive){State s;s.e[1].talk=true;s.health=alive?1:0;assert(run(s,initial)==initial+1&&s.speaks==alive);if(alive){std::string expected=initial==0?"TEXT_QST_018_TRADER_INTRO":initial==1?"TEXT_QST_018_TRADER_INTRO_REP":"TEXT_QST_018_TRADER_ON_TALK_AGAIN";assert(s.text[0]==expected);}++cases;}
 for(int kind=0;kind<3;++kind){State s;s.e[1].hit=kind==0;s.e[1].magic=kind>0;s.e[1].heal=kind==2;s.moveFailures=2;s.taskFrames=1;run(s);assert(s.killed==(kind<2)&&s.scared==(kind<2)&&s.moves==(kind<2?2:0));++cases;}
 {State s;s.initialTimer=-1;run(s);assert(!s.convs&&s.pace.size()==7);++cases;}{State s;s.activeUntil=100;run(s);assert(!s.convs);++cases;}{State s;s.taskFrames=2;run(s);assert(s.pace.size()==4);++cases;}
 for(int stop=1;stop<=100;++stop)for(int mode=0;mode<3;++mode){State s;s.stopCheck=stop;s.startFailures=mode;s.taskFrames=mode;s.moveFailures=mode;for(int n=1;n<8;++n){s.e[n].talk=mode==1;s.e[n].magic=mode==2;}run(s);assert(s.killed<=1);++cases;}
 assert(cases==318);std::puts("00d1fd30_TEST PASS");}
