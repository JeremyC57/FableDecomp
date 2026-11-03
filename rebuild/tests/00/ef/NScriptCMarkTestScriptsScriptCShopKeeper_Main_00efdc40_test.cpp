#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
static void check(bool value) { if(!value) { puts("00efdc40_TEST FAIL"); exit(1); } }
struct Plan {
    bool talk,hit,special,heal,dead,terminated;
    int acquire_failures,busy,items,stop_at,checks,frames,frame_limit;
    int resources,movies,pauses,predicates,copies,information,acquires,speeches,lists,prices,sales;
    int special_queries,heal_queries,initial[3],stock[3];
    int trace[256],ntrace;
    Plan():talk(false),hit(false),special(false),heal(false),dead(false),terminated(false),
      acquire_failures(0),busy(1),items(3),stop_at(0),checks(0),frames(0),frame_limit(1),
      resources(0),movies(0),pauses(0),predicates(0),copies(0),information(0),acquires(0),speeches(0),lists(0),prices(0),sales(0),
      special_queries(0),heal_queries(0),ntrace(0) {initial[0]=5;initial[1]=0;initial[2]=-3;for(int i=0;i<3;++i)stock[i]=initial[i];}
    void event(int e) {check(ntrace<256);trace[ntrace++]=e;}
    void action(int e) {check(!terminated);event(e);}
};
static Plan* plan;
class TRetVoid {public:void Return(){}};
class CCharString {
public: const char* text;
    CCharString(const char* p):text(p) {++plan->copies;}
    ~CCharString() {--plan->copies;}
};
enum EScriptAIPriority {SCRIPT_AI_PRIORITY_HIGHEST=4};
enum ETextGroupSelectionMethod {GROUP_SELECT_NONE=0};
enum EHeroAbility {HERO_ABILITY_HEAL_LIFE_SPELL=14};
class CScriptThing {
public: int id;
    CScriptThing(int i=9):id(i) {}
    bool MsgIsTalkedToBy(const CCharString& s) const {check(!strcmp(s.text,"SCRIPT_NAME_HERO"));plan->action(10);return plan->talk;}
    bool MsgIsHitBy(const CCharString& s) const {check(!strcmp(s.text,"SCRIPT_NAME_HERO"));plan->action(11);return plan->hit;}
    bool MsgIsHitByAnySpecialAbilityFrom(const CCharString& s) const {check(!strcmp(s.text,"SCRIPT_NAME_HERO"));plan->action(12);++plan->special_queries;return plan->special;}
    bool MsgIsHitBySpecialAbilityFrom(EHeroAbility a,const CCharString& s) const {check(a==HERO_ABILITY_HEAL_LIFE_SPELL&&!strcmp(s.text,"SCRIPT_NAME_HERO"));plan->action(13);++plan->heal_queries;return plan->heal;}
};
class CScriptGameResourceObjectScriptedThingBase {
public: int pending; bool active;
    CScriptGameResourceObjectScriptedThingBase():pending(0),active(false) {++plan->resources;plan->action(3);}
    ~CScriptGameResourceObjectScriptedThingBase() {--plan->resources;plan->event(103);}
    bool IsValid() const {plan->action(4);return active;}
    void Release() {active=false;plan->action(5);}
    CScriptThing GetScriptThing() const {check(active);plan->action(16);return CScriptThing(9);}
    void Speak(const CScriptThing& hero,const char* text,ETextGroupSelectionMethod mode,bool a,bool b,bool c) {
        check(active&&hero.id==99&&mode==GROUP_SELECT_NONE&&!a&&b&&!c&&plan->pauses==1);
        check(!strcmp(text,"Double the prices for you!")||!strcmp(text,"No shopping for you m'lad"));
        plan->action(!strcmp(text,"Double the prices for you!")?18:19);++plan->speeches;pending=plan->busy;
    }
    bool IsPerformingScriptTask() {plan->action(20);if(pending) {--pending;return true;}return false;}
};
class CScriptGameResourceObjectMovieBase {
public: CScriptGameResourceObjectMovieBase() {++plan->movies;plan->action(14);}
    ~CScriptGameResourceObjectMovieBase() {check(plan->pauses==0);--plan->movies;plan->event(114);}
};
class CGameScriptInterfaceBase {
public:
    CScriptThing hero;
    CGameScriptInterfaceBase():hero(99) {}
    void NewScriptFrame() const {plan->action(2);++plan->frames;}
    void SetThingHasInformation(const CScriptThing& t,bool a,bool b,bool c) {check(t.id==9&&!a&&b&&!c);plan->action(6);++plan->information;}
    bool StartScriptingEntity(const CScriptThing& t,CScriptGameResourceObjectScriptedThingBase& h,EScriptAIPriority p) {
        check(t.id==9&&p==SCRIPT_AI_PRIORITY_HIGHEST);plan->action(7);++plan->acquires;
        if(plan->acquire_failures) {--plan->acquire_failures;return false;}h.active=true;return true;
    }
    void StartMovieSequence(const CCharString& s,CScriptGameResourceObjectMovieBase&) {check(!s.text[0]&&plan->movies==1);plan->action(15);}
    void PauseAllNonScriptedEntities(bool pause) {if(pause) {plan->action(21);++plan->pauses;}else {check(plan->pauses==1);--plan->pauses;plan->event(121);}}
    float GetHealth(const CScriptThing& t) const {check(t.id==9);plan->action(17);return plan->dead?0.0f:1.0f;}
    const CScriptThing& GetHero() const {plan->action(22);return hero;}
    long GetAllThingsWithScriptName(const CCharString& s,std::vector<CScriptThing>& items) const {
        check(!strcmp(s.text,"ShopItem")&&plan->movies==1&&plan->pauses==1);plan->action(23);++plan->lists;
        for(int i=0;i<plan->items;++i)items.push_back(CScriptThing(i));return plan->items;
    }
    long EntityGetStockItemPrice(const CScriptThing& t) const {check(t.id>=0&&t.id<plan->items);plan->action(24);return plan->stock[t.id];}
    void EntitySetStockItemPrice(const CScriptThing& t,long p) const {check(t.id>=0&&t.id<plan->items&&plan->pauses==1);plan->action(25);plan->stock[t.id]=int(p);++plan->prices;}
    void EntitySetAsForSale(const CScriptThing& t,bool p) const {check(t.id>=0&&t.id<plan->items&&!p&&plan->pauses==1);plan->action(26);++plan->sales;}
};
class CWideScreenMagicPauseEntities {
    CGameScriptInterfaceBase& game;
public: CWideScreenMagicPauseEntities(CGameScriptInterfaceBase& g):game(g) {game.PauseAllNonScriptedEntities(true);}
    ~CWideScreenMagicPauseEntities() {game.PauseAllNonScriptedEntities(false);}
};
namespace NScript {
class CIsThingConscious {
public: CIsThingConscious(const CScriptThing& t) {check(t.id==9);++plan->predicates;}
    ~CIsThingConscious() {--plan->predicates;}
};
class CMarkTestScriptsScript {
public: class CShopKeeper {
public:
    CGameScriptInterfaceBase& Game; CScriptThing Me;
    CShopKeeper(CGameScriptInterfaceBase& g):Game(g),Me(9) {}
    void SetPredicate(const CIsThingConscious&) {check(plan->predicates==1);plan->action(1);}
    bool IsActiveThreadTerminating() const {
        ++plan->checks;plan->event(8);
        bool result=(plan->stop_at && plan->checks==plan->stop_at)||plan->frames>plan->frame_limit;
        if(result)plan->terminated=true;return result;
    }
    void Main();
};
};
}

// Selected source body copied without changes.
void NScript::CMarkTestScriptsScript::CShopKeeper::Main()
{
    TRetVoid _Fail_Val;
    SetPredicate(CIsThingConscious(Me));
    Game.NewScriptFrame();
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    Game.SetThingHasInformation(Me,false,true,false);
    CScriptGameResourceObjectScriptedThingBase seh_me;
    if (seh_me.IsValid()) seh_me.Release();
    while (!Game.StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    for (;!false;Game.NewScriptFrame()) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            CScriptGameResourceObjectMovieBase speech_movie;
            Game.StartMovieSequence("",speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
            if (Game.GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                seh_me.Speak(Game.GetHero(),"Double the prices for you!",GROUP_SELECT_NONE,false,true,false);
                while (seh_me.IsPerformingScriptTask()) {
                    Game.NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            std::vector<CScriptThing> ShopList;
            Game.GetAllThingsWithScriptName("ShopItem",ShopList);
            for (int i=0;i<ShopList.size();++i) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                Game.EntitySetStockItemPrice(ShopList[i],Game.EntityGetStockItemPrice(ShopList[i])*2);
            }
        }
        if (Me.MsgIsHitBy("SCRIPT_NAME_HERO") ||
            (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") &&
             !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL,"SCRIPT_NAME_HERO"))) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            CScriptGameResourceObjectMovieBase speech_movie;
            Game.StartMovieSequence("",speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
            if (Game.GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                seh_me.Speak(Game.GetHero(),"No shopping for you m'lad",GROUP_SELECT_NONE,false,true,false);
                while (seh_me.IsPerformingScriptTask()) {
                    Game.NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            std::vector<CScriptThing> ShopList;
            Game.GetAllThingsWithScriptName("ShopItem",ShopList);
            for (int i=0;i<ShopList.size();++i) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                Game.EntitySetAsForSale(ShopList[i],false);
            }
        }
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
}

static int run(bool talk,bool hit,bool special,bool heal,bool dead,int items,int failures,int busy,int stop,int iterations=1) {
    Plan p;plan=&p;p.talk=talk;p.hit=hit;p.special=special;p.heal=heal;p.dead=dead;p.items=items;p.acquire_failures=failures;p.busy=busy;p.stop_at=stop;
    int reacts=(talk?1:0)+((hit||(special&&!heal))?1:0);
    p.frame_limit=failures+iterations*(1+(dead?0:reacts*busy));
    CGameScriptInterfaceBase game;NScript::CMarkTestScriptsScript::CShopKeeper actor(game);actor.Main();
    check(p.terminated&&p.resources==0&&p.movies==0&&p.pauses==0&&p.predicates==0&&p.copies==0);
    if(stop) {check(p.checks==stop);return p.checks;}
    check(p.information==1&&p.acquires==1+failures&&p.lists==reacts*iterations&&p.speeches==(dead?0:reacts*iterations));
    check(p.prices==(talk?items*iterations:0)&&p.sales==((hit||(special&&!heal))?items*iterations:0));
    check(p.special_queries==(hit?0:iterations)&&p.heal_queries==(!hit&&special?iterations:0));
    int scale=1;for(int j=0;j<iterations;++j)scale*=talk?2:1;
    for(int i=0;i<items;++i)check(p.stock[i]==p.initial[i]*scale);
    check(p.checks==failures+3+iterations+iterations*reacts*(1+(dead?0:busy+1)+items));
    check(p.trace[0]==1&&p.trace[1]==2&&p.trace[p.ntrace-1]==103);
    return p.checks;
}
int main() {
    int total=0;
    for(int mask=0;mask<32;++mask) {
        bool talk=(mask&1)!=0,hit=(mask&2)!=0,special=(mask&4)!=0,heal=(mask&8)!=0,dead=(mask&16)!=0;
        for(int empty=0;empty<2;++empty) {
            int n=run(talk,hit,special,heal,dead,empty?0:3,1,1,0);++total;
            for(int stop=1;stop<=n;++stop) {run(talk,hit,special,heal,dead,empty?0:3,1,1,stop);++total;}
        }
    }
    for(int iterations=2;iterations<=3;++iterations)for(int mask=0;mask<32;++mask)for(int empty=0;empty<2;++empty) {
        bool talk=(mask&1)!=0,hit=(mask&2)!=0,special=(mask&4)!=0,heal=(mask&8)!=0,dead=(mask&16)!=0;
        int n=run(talk,hit,special,heal,dead,empty?0:3,1,1,0,iterations);++total;
        for(int stop=1;stop<=n;++stop) {run(talk,hit,special,heal,dead,empty?0:3,1,1,stop,iterations);++total;}
    }
    run(true,true,false,false,false,3,0,0,0);++total;
    fprintf(stderr,"SCENARIOS %d\n",total);
    check(total>400);puts("00efdc40_TEST PASS");return 0;
}