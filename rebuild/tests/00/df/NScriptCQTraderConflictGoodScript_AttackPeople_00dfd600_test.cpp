#include <cassert>
#include <cstdio>
#include <string>
#include <vector>

typedef std::string CCharString;
struct TRetVoid { void Return() {} };
enum { hero = 1, trader = 2, first_enemy = 10 };
struct Scenario {
    int nearby, visible, follows, alive, initially_busy, guard, skip_enemy;
    bool entered;
    int count, rescued, region_frame;
    Scenario() : nearby(0), visible(0), follows(1), alive(1), initially_busy(0), guard(0),
        skip_enemy(-1), entered(false), count(1), rescued(0), region_frame(0) {}
};
struct Trace {
    Scenario input;
    int checks, frames, stop, timer, allocated, released, scans, updates;
    int targets, last_target, conversations, lines, markers, resets;
    bool stopped, entered;
    std::string last_line;
    Trace() : checks(0), frames(0), stop(0), timer(0), allocated(0), released(0), scans(0),
        updates(0), targets(0), last_target(0), conversations(0), lines(0), markers(0),
        resets(0), stopped(false), entered(false) {}
    void active() const { assert(!stopped); }
} trace;
static int live_handles;
struct CScriptThing {
    int id;
    explicit CScriptThing(int value=0) : id(value) { ++live_handles; }
    CScriptThing(const CScriptThing& other) : id(other.id) { ++live_handles; }
    ~CScriptThing() { --live_handles; }
    CScriptThing& operator=(const CScriptThing& other) { id=other.id; return *this; }
    bool operator==(const CScriptThing& other) const { return id==other.id; }
    bool IsAlive() const { trace.active(); return id==trader && trace.input.alive; }
    CCharString GetName() const { trace.active(); return trace.input.guard ? "TC_BanditGuard" : "Enemy"; }
};
class CTimer {
public:
    CTimer() { ++trace.allocated; trace.timer=trace.input.initially_busy; }
    ~CTimer() { ++trace.released; }
    bool operator==(long time) const { trace.active(); return trace.timer==time; }
    CTimer& operator=(long time) {
        trace.active(); assert(time==4); trace.timer=time; ++trace.resets; return *this;
    }
};
static bool IsDistanceBetweenThingsUnder(const CScriptThing& e,const CScriptThing& t,float distance) {
    trace.active(); assert(e.id>=first_enemy && distance==15.0f);
    return e.id!=trace.input.skip_enemy && (trace.input.nearby & t.id)!=0;
}
struct GameFixture {
    bool* region;
    GameFixture() : region(0) {}
    void NewScriptFrame() {
        trace.active(); ++trace.frames; assert(trace.frames<=64);
        if (trace.frames==trace.input.region_frame) *region=true;
    }
    CScriptThing GetNearestWithScriptName(const CScriptThing& e,const char* name) {
        trace.active(); assert(e.id>=first_enemy && std::string(name)=="TraderToRescue");
        ++trace.scans; return CScriptThing(trader);
    }
    bool IsEntityFollowingHero(const CScriptThing& t) {
        trace.active(); assert(t.id==trader); return trace.input.follows!=0;
    }
    CScriptThing GetHero() { trace.active(); return CScriptThing(hero); }
    bool IsThingAwareOfOtherThingInAnyWay(const CScriptThing& e,const CScriptThing& t) {
        trace.active(); return e.id!=trace.input.skip_enemy && (trace.input.visible & t.id)!=0;
    }
    void GiveThingBestEnemyTarget(const CScriptThing& e,const CScriptThing& t) {
        trace.active(); assert(e.id>=first_enemy && (t.id==hero || t.id==trader));
        ++trace.targets; trace.last_target=t.id;
    }
    long AddNewConversation(const CScriptThing& e,bool first,bool second) {
        trace.active(); assert(e.id>=first_enemy && !first && !second); return ++trace.conversations;
    }
    void AddPersonToConversation(long index,const CScriptThing& t) {
        trace.active(); assert(index==trace.conversations && t.id==hero);
    }
    void AddLineToConversation(long index,const char* text,bool flag,const CScriptThing& e,const CScriptThing& t) {
        trace.active(); assert(index==trace.conversations && !flag && e.id>=first_enemy && t.id==hero);
        trace.last_line=text; ++trace.lines;
    }
    void MiniMapAddMarker(const CScriptThing& e,const char* marker) {
        trace.active(); assert(e.id>=first_enemy && std::string(marker)=="HUD_ORB_RED_SMALL"); ++trace.markers;
    }
};
namespace NScript {
struct CQ_TraderConflictGoodScript {
    GameFixture& Game;
    std::vector<CScriptThing> AllCreatures;
    bool EnteredNewRegion;
    long TradersReachedTeleporter, TradersFollowing;
    explicit CQ_TraderConflictGoodScript(GameFixture& game) : Game(game), EnteredNewRegion(trace.input.entered),
        TradersReachedTeleporter(trace.input.rescued), TradersFollowing(1) { game.region=&EnteredNewRegion; }
    void UpdateLiveEnemies() { trace.active(); ++trace.updates; }
    bool IsActiveThreadTerminating() {
        trace.active(); trace.stopped=(++trace.checks==trace.stop); return trace.stopped;
    }
    void AttackPeople();
};
}

void NScript::CQ_TraderConflictGoodScript::AttackPeople()
{
    TRetVoid _Fail_Val;
    UpdateLiveEnemies();
    long CreaturesAttacking = 0;
    long TotalCreaturesToAttack = AllCreatures.size();
    std::vector<CScriptThing> CurrentCreatures(AllCreatures);
    CTimer AttackCryDelay;
    CCharString NewRegionLoaded;
    while (!EnteredNewRegion && CurrentCreatures.size() > 0 && TradersReachedTeleporter < TradersFollowing) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        for (long i = 0; i < CurrentCreatures.size(); ++i) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            CScriptThing Trader = Game.GetNearestWithScriptName(CurrentCreatures[i], "TraderToRescue");
            if (!Game.IsEntityFollowingHero(Trader)) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                Trader = CScriptThing();
            }
            if (IsDistanceBetweenThingsUnder(CurrentCreatures[i], Game.GetHero(), 15.0f)
                || IsDistanceBetweenThingsUnder(CurrentCreatures[i], Trader, 15.0f)
                || Game.IsThingAwareOfOtherThingInAnyWay(CurrentCreatures[i], Game.GetHero())
                || (Game.IsThingAwareOfOtherThingInAnyWay(CurrentCreatures[i], Trader) && CurrentCreatures[i].GetName() != "TC_BanditGuard")) {
                if (IsActiveThreadTerminating()) {
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
                    goto trader_cleanup;
#ifdef _MSC_VER
#pragma inline_depth()
#endif
                }
                if (IsDistanceBetweenThingsUnder(CurrentCreatures[i], Trader, 15.0f)
                    || (Game.IsThingAwareOfOtherThingInAnyWay(CurrentCreatures[i], Trader) && Trader.IsAlive())) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    Game.GiveThingBestEnemyTarget(CurrentCreatures[i], Trader);
                    if (AttackCryDelay == 0) {
                        if (IsActiveThreadTerminating()) {
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
                            return _Fail_Val.Return();
                        }
#ifdef _MSC_VER
#pragma inline_depth()
#endif
                        long HLStyleConversationIndex = Game.AddNewConversation(CurrentCreatures[i], false, false);
                        Game.AddPersonToConversation(HLStyleConversationIndex, Game.GetHero());
                        Game.AddLineToConversation(HLStyleConversationIndex, "TEXT_QST_B11_BANDIT_ATTACK_TRADER", false, CurrentCreatures[i], Game.GetHero());
                        AttackCryDelay = 4;
                    }
                } else {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    Game.GiveThingBestEnemyTarget(CurrentCreatures[i], Game.GetHero());
                    if (AttackCryDelay == 0) {
                        if (IsActiveThreadTerminating()) {
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
                            return _Fail_Val.Return();
                        }
#ifdef _MSC_VER
#pragma inline_depth()
#endif
                        long HLStyleConversationIndex = Game.AddNewConversation(CurrentCreatures[i], false, false);
                        Game.AddPersonToConversation(HLStyleConversationIndex, Game.GetHero());
                        Game.AddLineToConversation(HLStyleConversationIndex, "TEXT_QST_B11_BANDIT_ATTACK_HERO", false, CurrentCreatures[i], Game.GetHero());
                        AttackCryDelay = 4;
                    }
                }
                Game.MiniMapAddMarker(CurrentCreatures[i], "HUD_ORB_RED_SMALL");
                for (std::vector<CScriptThing>::iterator it = CurrentCreatures.begin(); it != CurrentCreatures.end(); ++it) {
                    if (CurrentCreatures[i] == *it) {
                        CurrentCreatures.erase(it);
                        break;
                    }
                }
                ++CreaturesAttacking;
                break;
            }
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    if (EnteredNewRegion) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        EnteredNewRegion = false;
    }
    return;
trader_cleanup:
    return _Fail_Val.Return();
}

static Trace run(const Scenario& input,int stop) {
    trace=Trace(); trace.input=input; trace.stop=stop;
    assert(live_handles==0);
    {
        GameFixture game;
        NScript::CQ_TraderConflictGoodScript quest(game);
        for (int i=0; i<input.count; ++i) quest.AllCreatures.push_back(CScriptThing(first_enemy+i));
        quest.AttackPeople();
        trace.entered=quest.EnteredNewRegion;
        assert(quest.AllCreatures.size()==static_cast<unsigned>(input.count));
        for (int i=0; i<input.count; ++i) assert(quest.AllCreatures[i].id==first_enemy+i);
    }
    assert(live_handles==0 && trace.allocated==1 && trace.released==1 && trace.updates==1);
    if (stop) assert(trace.stopped && trace.checks==stop);
    return trace;
}
static Trace exercise(const Scenario& input) {
    Trace complete=run(input,0);
    for (int stop=1; stop<=complete.checks; ++stop) run(input,stop);
    return complete;
}
int main() {
    Scenario in; in.count=2; in.nearby=trader;
    Trace done=exercise(in);
    assert(done.targets==2 && done.markers==2 && done.scans==2 && done.frames==12);
    assert(done.last_target==trader && done.lines==1 && done.resets==1);
    assert(done.last_line=="TEXT_QST_B11_BANDIT_ATTACK_TRADER");
    in=Scenario(); in.follows=0; in.nearby=hero; done=exercise(in);
    assert(done.last_target==hero && done.markers==1 && done.lines==1);
    assert(done.last_line=="TEXT_QST_B11_BANDIT_ATTACK_HERO");
    in=Scenario(); in.visible=trader; done=exercise(in); assert(done.last_target==trader);
    in.alive=0; done=exercise(in); assert(done.last_target==hero);
    in.guard=1; in.region_frame=6; done=exercise(in); assert(done.markers==0 && !done.entered);
    in=Scenario(); in.visible=hero; in.initially_busy=3; done=exercise(in); assert(done.last_target==hero && done.lines==0);
    in=Scenario(); in.nearby=trader; in.count=2; in.skip_enemy=first_enemy; in.region_frame=6;
    done=exercise(in); assert(done.scans==2 && done.targets==1 && done.markers==1);
    in=Scenario(); in.entered=true; done=exercise(in);
    assert(done.frames==0 && done.checks==2 && !done.entered);
    assert(run(in,1).entered && run(in,2).entered);
    in=Scenario(); in.count=0; done=exercise(in); assert(done.frames==0 && done.targets==0);
    in=Scenario(); in.rescued=1; done=exercise(in); assert(done.frames==0 && done.scans==0);
    std::puts("00dfd600_TEST PASS");
    return 0;
}