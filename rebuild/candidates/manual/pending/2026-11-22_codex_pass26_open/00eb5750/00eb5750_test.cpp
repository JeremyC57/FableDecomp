#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <vector>
static void check(bool ok) { if (!ok) abort(); }
typedef std::string CCharString;
inline CCharString GFIntToCharString(long x) { char s[32];sprintf(s,"%ld",x);return s; }
enum ETextGroupSelectionMethod { GROUP_SELECT_NONE };
enum EHeroAbility { HERO_ABILITY_HEAL_LIFE_SPELL=14 };
struct TRetVoid { void Return() {} };
struct Conditions {
    bool talked,hit,special,heal,enflamed,healthy;
    int stop,frames;
};
static Conditions conditions;
static int terminationChecks,frames,remaining,paused,unpaused,movieStarts,movieEnds,healthChecks;
static int talkedChecks,hitChecks,specialChecks,healChecks,enflameChecks;
static std::vector<std::string> lines;
struct CScriptThing {
    int id;
    explicit CScriptThing(int number=0):id(number){}
    void assertHero(const CCharString& name) const { check(id==1 && name=="SCRIPT_NAME_HERO"); }
    bool MsgIsTalkedToBy(const CCharString& n) const { assertHero(n);++talkedChecks;return conditions.talked; }
    bool MsgIsHitBy(const CCharString& n) const { assertHero(n);++hitChecks;return conditions.hit; }
    bool MsgIsHitByAnySpecialAbilityFrom(const CCharString& n) const { assertHero(n);++specialChecks;return conditions.special; }
    bool MsgIsHitBySpecialAbilityFrom(EHeroAbility ability,const CCharString& n) const {
        assertHero(n);check(ability==HERO_ABILITY_HEAL_LIFE_SPELL);++healChecks;return conditions.heal;
    }
    CCharString GetDataString() const {check(id==1);return "_A"; }
};
struct CScriptGameResourceObjectMovieBase {
    bool started;
    CScriptGameResourceObjectMovieBase():started(false){}
    ~CScriptGameResourceObjectMovieBase(){check(started && paused==unpaused);++movieEnds;}
};
struct CGameScriptInterfaceBase {
    CScriptThing hero;
    CGameScriptInterfaceBase():hero(2){}
    void StartMovieSequence(const std::string& name,CScriptGameResourceObjectMovieBase& movie) {
        check(name.empty() && !movie.started && movieStarts==movieEnds);
        movie.started=true;++movieStarts;
    }
    void PauseAllNonScriptedEntities(bool pause) {
        if(pause){check(paused==unpaused);++paused;}
        else {check(paused==unpaused+1);++unpaused;}
    }
    float GetHealth(const CScriptThing& subject) const {
        check(subject.id==1 && paused==unpaused+1);++healthChecks;
        return conditions.healthy?0.5f:-0.5f;
    }
    const CScriptThing& GetHero() const{return hero;}
    void NewScriptFrame() {check(remaining>0 && paused==unpaused+1);--remaining;++frames;}
};
struct CWideScreenMagicPauseEntities {
    CGameScriptInterfaceBase& game;
    explicit CWideScreenMagicPauseEntities(CGameScriptInterfaceBase& g):game(g){game.PauseAllNonScriptedEntities(true);}
    ~CWideScreenMagicPauseEntities(){game.PauseAllNonScriptedEntities(false);}
};
struct CScriptGameResourceObjectScriptedThingBase {
    CScriptThing GetScriptThing() const{return CScriptThing(1);}
    void Speak(const CScriptThing& hero,const std::string& line,ETextGroupSelectionMethod method,bool a,bool b,bool c) {
        check(hero.id==2 && method==GROUP_SELECT_NONE && !a && b && !c && paused==unpaused+1);
        lines.push_back(line);remaining=conditions.frames;
    }
    bool IsPerformingScriptTask() const {return remaining>0;}
};
namespace NScript {
struct CV_MurderTwistFinalScript {
    bool HasHeroEnflamedEntity(const CScriptThing& me) const {check(me.id==1);++enflameChecks;return conditions.enflamed;}
    struct CMurderTwistAssassinGuard {
        CV_MurderTwistFinalScript& ParentClass;
        CGameScriptInterfaceBase& Game;
        CScriptThing Me;
        CScriptGameResourceObjectScriptedThingBase MyEntityHandle;
        unsigned long NextLine;
        bool HasBeenSpokenTo;
        CMurderTwistAssassinGuard(CV_MurderTwistFinalScript& p,CGameScriptInterfaceBase& g):ParentClass(p),Game(g),Me(1){}
        bool IsActiveThreadTerminating() const { return ++terminationChecks==conditions.stop; }
        void DoAskingHeroToTalkToAssassin();
    };
};
}

namespace NScript {
void CV_MurderTwistFinalScript::CMurderTwistAssassinGuard::DoAskingHeroToTalkToAssassin()
{
    TRetVoid _Fail_Val;
    if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        CScriptGameResourceObjectMovieBase speech_movie;
        Game.StartMovieSequence("", speech_movie);
        CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
        if (HasBeenSpokenTo) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (NextLine > 2) NextLine = 1;
            CCharString full_text_name = "TEXT_QST_B07_HENCHMAN_BUSY" + Me.GetDataString() + GFIntToCharString(NextLine);
            if (Game.GetHealth(MyEntityHandle.GetScriptThing()) > 0.0f) {
                MyEntityHandle.Speak(Game.GetHero(),full_text_name,GROUP_SELECT_NONE,false,true,false);
                while (MyEntityHandle.IsPerformingScriptTask()) {
                    Game.NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            ++NextLine;
        } else {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (Game.GetHealth(MyEntityHandle.GetScriptThing()) > 0.0f) {
                MyEntityHandle.Speak(Game.GetHero(),"TEXT_QST_B07_HENCHMAN_BUSY" + Me.GetDataString(),GROUP_SELECT_NONE,false,true,false);
                while (MyEntityHandle.IsPerformingScriptTask()) {
                    Game.NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            HasBeenSpokenTo = true;
        }
    }
    if ((Me.MsgIsHitBy("SCRIPT_NAME_HERO") ||
        (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") &&
         !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL, "SCRIPT_NAME_HERO"))) &&
        !ParentClass.HasHeroEnflamedEntity(Me)) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        CScriptGameResourceObjectMovieBase speech_movie;
        Game.StartMovieSequence("", speech_movie);
        CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
        if (NextLine > 2) NextLine = 1;
        CCharString full_text_name = "TEXT_QST_B07_HENCHMAN_ATTACKED" + Me.GetDataString() + GFIntToCharString(NextLine);
        if (Game.GetHealth(MyEntityHandle.GetScriptThing()) > 0.0f) {
            MyEntityHandle.Speak(Game.GetHero(),full_text_name,GROUP_SELECT_NONE,false,true,false);
            while (MyEntityHandle.IsPerformingScriptTask()) {
                Game.NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
        ++NextLine;
    }
}
}

struct Expected {
    int checkpoints, starts, advances, speeches, health;
    int talked,hit,special,heal,enflamed;
    bool flag;
    unsigned long next;
    std::vector<std::string> text;
    explicit Expected(unsigned long n,bool known):checkpoints(0),starts(0),advances(0),speeches(0),health(0),talked(0),hit(0),special(0),heal(0),enflamed(0),flag(known),next(n){}
    bool stop(){return ++checkpoints==conditions.stop;}
    bool movie(bool reply){
        if(stop())return false;
        ++starts;
        if(reply && stop())return false;
        bool numbered=!reply||flag;
        if(numbered && next>2)next=1;
        ++health;
        if(conditions.healthy){
            std::string scriptLine=reply?"TEXT_QST_B07_HENCHMAN_BUSY_A":"TEXT_QST_B07_HENCHMAN_ATTACKED_A";
            if(numbered)scriptLine+=GFIntToCharString(next);
            text.push_back(scriptLine);++speeches;
            for(int i=0;i<conditions.frames;++i){++advances;if(stop())return false;}
            if(stop())return false;
        }
        if(numbered)++next;
        else flag=true;
        return true;
    }
    void run(){
        ++talked;
        if(conditions.talked && !movie(true))return;
        ++hit;
        bool attacked=conditions.hit;
        if(!attacked){++special;if(conditions.special){++heal;attacked=!conditions.heal;}}
        if(attacked){++enflamed;if(!conditions.enflamed)movie(false);}
    }
};
int main()
{
    for(int mask=0; mask<128; ++mask) for(int n=0; n<5; ++n)
        for(int wait=0;wait<3;++wait) for(int stop=0;stop<15;++stop){
            conditions.talked=(mask&1)!=0;conditions.hit=(mask&2)!=0;
            conditions.special=(mask&4)!=0;conditions.heal=(mask&8)!=0;
            conditions.enflamed=(mask&16)!=0;conditions.healthy=(mask&32)!=0;
            conditions.frames=wait;conditions.stop=stop;
            frames=remaining=terminationChecks=paused=unpaused=movieStarts=movieEnds=healthChecks=0;
            talkedChecks=hitChecks=specialChecks=healChecks=enflameChecks=0;lines.clear();
            NScript::CV_MurderTwistFinalScript parent;CGameScriptInterfaceBase game;
            NScript::CV_MurderTwistFinalScript::CMurderTwistAssassinGuard guard(parent,game);
            guard.HasBeenSpokenTo=(mask&64)!=0;guard.NextLine=n;
            Expected comparison(n,guard.HasBeenSpokenTo);comparison.run();
            guard.DoAskingHeroToTalkToAssassin();
            check(guard.HasBeenSpokenTo==comparison.flag && guard.NextLine==comparison.next);
            check(terminationChecks==comparison.checkpoints && frames==comparison.advances && lines==comparison.text);
            check(movieStarts==comparison.starts && movieEnds==movieStarts && paused==movieStarts && unpaused==paused);
            check(healthChecks==comparison.health && talkedChecks==comparison.talked && hitChecks==comparison.hit);
            check(specialChecks==comparison.special && healChecks==comparison.heal && enflameChecks==comparison.enflamed);
        }
    puts("00eb5750_TEST PASS");
    return 0;
}
