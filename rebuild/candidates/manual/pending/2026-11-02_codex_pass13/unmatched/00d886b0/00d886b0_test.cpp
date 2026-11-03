#include <cstdio>
#include <cassert>
#include <string>
#include <vector>
class CCharString {
public:
    std::string text;
    CCharString() {}
    CCharString(const char* p):text(p) {}
    CCharString(const std::string& p):text(p) {}
    operator const char*() const { return text.c_str(); }
};
CCharString operator+(const char* a,const CCharString& b){return CCharString(std::string(a)+b.text);}
CCharString operator+(const CCharString& a,const char* b){return CCharString(a.text+std::string(b));}
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_HIGHEST=4 };
enum ETextGroupSelectionMethod { GROUP_SELECT_NONE=0 };
enum EHeroAbility { HERO_ABILITY_HEAL_LIFE_SPELL=14 };
class TRetVoid { public: void Return() {} };
struct Event { bool talk,hit,magic,heal,cutscene; Event():talk(false),hit(false),magic(false),heal(false),cutscene(false){} };
struct State {
    Event e[16]; int frame,limit,checks,stopCheck,startFailures,starts,sets,unsets,enemies,resourceCount,movieCount,pauses,speaks,predicate;
    int releases,pauseStarts,movieStarts,taskFrames,taskUntil;
    float ownHealth,scriptHealth;
    std::vector<std::string> text;
    State():frame(0),limit(6),checks(0),stopCheck(0),startFailures(0),starts(0),sets(0),unsets(0),enemies(0),resourceCount(0),movieCount(0),pauses(0),speaks(0),predicate(0),releases(0),pauseStarts(0),movieStarts(0),taskFrames(0),taskUntil(0),ownHealth(1),scriptHealth(1){}
    bool terminating(){++checks; return frame>=limit||(stopCheck&&checks>=stopCheck);}
};
State* state;
class CScriptThing {
public:
    int kind;
    CScriptThing(int k=0):kind(k){}
    bool MsgIsTalkedToBy(const CCharString& s) const {assert(s.text=="SCRIPT_NAME_HERO");return state->e[state->frame].talk;}
    bool MsgIsHitBy(const CCharString& s) const {assert(s.text=="SCRIPT_NAME_HERO");return state->e[state->frame].hit;}
    bool MsgIsHitByAnySpecialAbilityFrom(const CCharString& s) const {assert(s.text=="SCRIPT_NAME_HERO");return state->e[state->frame].magic;}
    bool MsgIsHitBySpecialAbilityFrom(EHeroAbility a,const CCharString& s) const {assert(a==HERO_ABILITY_HEAL_LIFE_SPELL&&s.text=="SCRIPT_NAME_HERO");return state->e[state->frame].heal;}
    CCharString GetDataString() const {return CCharString("GUARD");}
};
class CScriptGameResourceObjectScriptedThingBase {
public:
    bool valid;
    CScriptGameResourceObjectScriptedThingBase():valid(false){++state->resourceCount;}
    ~CScriptGameResourceObjectScriptedThingBase(){--state->resourceCount;}
    bool IsValid()const{return valid;}
    void Release(){assert(valid);valid=false;++state->releases;}
    CScriptThing GetScriptThing()const{return CScriptThing(2);}
    void Speak(const CScriptThing& hero,const char* t,ETextGroupSelectionMethod m,bool a,bool b,bool c){assert(hero.kind==1&&m==GROUP_SELECT_NONE&&!a&&b&&!c);assert(state->pauses==1&&state->movieCount==1);++state->speaks;state->text.push_back(t);state->taskUntil=state->frame+state->taskFrames;}
    bool IsPerformingScriptTask()const{return state->frame<state->taskUntil;}
};
class CScriptGameResourceObjectMovieBase {public: CScriptGameResourceObjectMovieBase(){++state->movieCount;} ~CScriptGameResourceObjectMovieBase(){--state->movieCount;}};
class CGameScriptInterfaceBase {
public:
    CScriptThing hero;
    CGameScriptInterfaceBase():hero(1){}
    void NewScriptFrame(){assert(state->frame<15);++state->frame;}
    bool StartScriptingEntity(const CScriptThing&,CScriptGameResourceObjectScriptedThingBase& r,EScriptAIPriority p){assert(p==SCRIPT_AI_PRIORITY_HIGHEST);++state->starts;if(state->starts<=state->startFailures)return false;r.valid=true;return true;}
    void SetThingHasInformation(const CScriptThing&,bool a,bool b,bool c){assert(!a&&!b&&!c);}
    void StartMovieSequence(const CCharString& s,CScriptGameResourceObjectMovieBase&){assert(s.text.empty());++state->movieStarts;}
    void PauseAllNonScriptedEntities(bool p){if(p){assert(state->pauses==0);++state->pauseStarts;++state->pauses;}else{assert(state->pauses==1);--state->pauses;}}
    float GetHealth(const CScriptThing& s){return s.kind==2?state->scriptHealth:state->ownHealth;}
    CScriptThing& GetHero(){return hero;}
    bool IsInCutscene(){return state->e[state->frame].cutscene;}
    void EntitySetThingAsAllyOfThing(const CScriptThing&a,const CScriptThing&b){assert(a.kind!=b.kind);++state->sets;}
    void EntityUnsetThingAsAllyOfThing(const CScriptThing&a,const CScriptThing&b){assert(a.kind!=b.kind);++state->unsets;}
    void GiveThingBestEnemyTarget(const CScriptThing&a,const CScriptThing&b){assert(a.kind==0&&b.kind==1);++state->enemies;}
};
class CWideScreenMagicPauseEntities { CGameScriptInterfaceBase* Game; public: CWideScreenMagicPauseEntities(CGameScriptInterfaceBase& g):Game(&g){Game->PauseAllNonScriptedEntities(true);} ~CWideScreenMagicPauseEntities(){Game->PauseAllNonScriptedEntities(false);} };
namespace NScript {
class CIsThingAlive {public: explicit CIsThingAlive(const CScriptThing&s){assert(s.kind==0);}};
class CQ_HeroSoulsNostroScript {public: class CGuardNearGraveyardPath {
public:
    CGameScriptInterfaceBase& Game; CScriptThing Me;
    CGuardNearGraveyardPath(CGameScriptInterfaceBase& g):Game(g){}
    void SetPredicate(const CIsThingAlive&){++state->predicate;}
    bool IsActiveThreadTerminating()const{return state->terminating();}
    void Main();
};};
}

void NScript::CQ_HeroSoulsNostroScript::CGuardNearGraveyardPath::Main()
{
    TRetVoid _Fail_Val;
    long TimesHit;
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
    TimesHit=0;
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    do {
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            CScriptGameResourceObjectMovieBase speech_movie;
            Game.StartMovieSequence("",speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
            if (Game.GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                seh_me.Speak(Game.GetHero(),"TEXT_QST_B04_" + Me.GetDataString() + "_ONTALK",GROUP_SELECT_NONE,false,true,false);
                while (seh_me.IsPerformingScriptTask()) {
                    Game.NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
        }
        if (Me.MsgIsHitBy("SCRIPT_NAME_HERO") ||
            (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") &&
             !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL,"SCRIPT_NAME_HERO"))) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            ++TimesHit;
            CCharString OnHitText;
            switch (TimesHit) {
            case 1: OnHitText="TEXT_QST_B04_" + Me.GetDataString() + "_ONHIT_10"; break;
            case 2: OnHitText="TEXT_QST_B04_" + Me.GetDataString() + "_ONHIT_20"; break;
            case 3: OnHitText="TEXT_QST_B04_" + Me.GetDataString() + "_ONHIT_30"; break;
            default: OnHitText="TEXT_QST_B04_" + Me.GetDataString() + "_ONHIT_30"; break;
            }
            if (!Game.IsInCutscene() && Game.GetHealth(Me) > 0.0001f) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                CScriptGameResourceObjectMovieBase speech_movie;
                Game.StartMovieSequence("",speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
                if (Game.GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                    seh_me.Speak(Game.GetHero(),OnHitText,GROUP_SELECT_NONE,false,true,false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game.NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
            }
            if (TimesHit >= 3) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                Game.EntityUnsetThingAsAllyOfThing(Me,Game.GetHero());
                Game.EntityUnsetThingAsAllyOfThing(Game.GetHero(),Me);
                Game.GiveThingBestEnemyTarget(Me,Game.GetHero());
                if (seh_me.IsValid()) seh_me.Release();
                do { Game.NewScriptFrame(); } while (!IsActiveThreadTerminating());
                return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            Game.EntitySetThingAsAllyOfThing(Me,Game.GetHero());
            Game.EntitySetThingAsAllyOfThing(Game.GetHero(),Me);
        }
        Game.NewScriptFrame();
    } while (!IsActiveThreadTerminating());
}

void run(State& st){state=&st;CGameScriptInterfaceBase game;NScript::CQ_HeroSoulsNostroScript::CGuardNearGraveyardPath guard(game);guard.Main();assert(st.predicate==1);assert(st.resourceCount==0&&st.movieCount==0&&st.pauses==0);}
int main(){
    int cases=0;
    {State s;run(s);assert(s.speaks==0&&s.sets==0&&s.unsets==0&&s.enemies==0);++cases;}
    for(int magic=0;magic<2;++magic)for(int cut=0;cut<2;++cut)for(int health=0;health<3;++health){
        State s;s.ownHealth=health==0?0.0f:(health==1?0.0001f:1.0f);
        for(int f=1;f<=3;++f){s.e[f].hit=!magic;s.e[f].magic=magic!=0;s.e[f].cutscene=cut!=0;}
        run(s);assert(s.sets==4&&s.unsets==2&&s.enemies==1&&s.releases==1);
        const bool speech=!cut&&health==2;assert(s.speaks==(speech?3:0));
        if(speech){assert(s.text[0]=="TEXT_QST_B04_GUARD_ONHIT_10");assert(s.text[1]=="TEXT_QST_B04_GUARD_ONHIT_20");assert(s.text[2]=="TEXT_QST_B04_GUARD_ONHIT_30");}
        ++cases;
    }
    {State s;for(int f=1;f<=5;++f){s.e[f].magic=true;s.e[f].heal=true;}run(s);assert(s.speaks==0&&s.sets==0&&s.enemies==0);++cases;}
    for(int tasks=0;tasks<3;++tasks)for(int alive=0;alive<2;++alive){State s;s.e[1].talk=true;s.taskFrames=tasks;s.scriptHealth=alive?1.0f:0.0f;run(s);assert(s.speaks==alive);assert(s.movieStarts==1&&s.pauseStarts==1);if(alive)assert(s.text[0]=="TEXT_QST_B04_GUARD_ONTALK");++cases;}
    for(int stop=1;stop<=80;++stop)for(int mode=0;mode<3;++mode){State s;s.stopCheck=stop;s.taskFrames=mode==1?2:0;s.startFailures=mode==2?2:0;for(int f=1;f<=5;++f){s.e[f].hit=true;s.e[f].talk=mode==1;}run(s);assert(s.unsets==0||s.unsets==2);assert(s.enemies<=1);++cases;}
    assert(cases==260);std::puts("00d886b0_TEST PASS");
}
