#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>

static void check(bool condition) { if (!condition) abort(); }
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_HIGHEST=4 };
enum EScriptEntityMoveType { ENTITY_MOVE_RUN=1 };
struct TRetVoid { void Return() {} };
struct C3DVector { float X,Y,Z; };
enum Event { StopCheck,Frame,Release,Start,Nearest,Alive,Valid,Position,Move,Distance,Fade };
struct Scenario { int stop,startDelay,distanceDelay; bool failed,finished,alive,initialControl,acquiredControl,hasPosition; };
static Scenario config;
static std::vector<Event> calls;
static int checks,startCalls,distanceCalls,liveHandles,liveExits;
static C3DVector position={2,3,4}, zero={0,0,0};
struct CScriptThing {
    bool exit;
    CScriptThing(bool e=false) : exit(e) { if(exit) ++liveExits; }
    CScriptThing(const CScriptThing& o) : exit(o.exit) { if(exit) ++liveExits; }
    ~CScriptThing() { if(exit) --liveExits; }
    bool IsAlive() const { check(exit);calls.push_back(Alive);return config.alive; }
    const C3DVector& GetPos() const { check(exit);calls.push_back(Position);return config.hasPosition?position:zero; }
};
struct CScriptGameResourceObjectScriptedThingBase {
    bool controlled;
    CScriptGameResourceObjectScriptedThingBase():controlled(config.initialControl){++liveHandles;}
    ~CScriptGameResourceObjectScriptedThingBase(){--liveHandles;}
    bool IsValid() const { calls.push_back(Valid);return controlled; }
    void Release(){check(controlled);controlled=false;calls.push_back(::Release);}
    void MoveToPosition(const C3DVector& pos,float speed,EScriptEntityMoveType mode,bool a,bool b) {
        check(controlled && &pos==(config.hasPosition?&position:&zero) && speed==1.0f && mode==ENTITY_MOVE_RUN && !a && b);
        calls.push_back(Move);
    }
};
struct CGameScriptInterfaceBase {
    void NewScriptFrame(){calls.push_back(Frame);}
    bool StartScriptingEntity(const CScriptThing& me,CScriptGameResourceObjectScriptedThingBase& h,EScriptAIPriority p) {
        check(!me.exit && p==SCRIPT_AI_PRIORITY_HIGHEST && !h.controlled);
        calls.push_back(Start);
        if (++startCalls<=config.startDelay) return false;
        h.controlled=config.acquiredControl;return true;
    }
    CScriptThing GetNearestWithDefName(const CScriptThing& me,const char* name) {
        check(!me.exit && strcmp(name,"REGION_EXIT_POINT")==0);calls.push_back(Nearest);return CScriptThing(true);
    }
};
static bool IsDistanceBetweenThingsUnder(const CScriptThing& me,const CScriptThing& e,float distance) {
    check(!me.exit&&e.exit&&distance==2.0f);calls.push_back(Distance);return ++distanceCalls>config.distanceDelay;
}
static void FadeOutAndKillEntity(const CScriptThing& me,float time,bool a,bool b) {
    check(!me.exit&&time==1.0f&&a&&b);calls.push_back(Fade);
}
namespace NScript {
struct CQ_AmbushTradersScript {
    bool QuestFailed,QuestFinished;
    struct CAT_Phase1Archer {
        CQ_AmbushTradersScript& ParentClass;
        CGameScriptInterfaceBase& Game;
        CScriptThing Me;
        CAT_Phase1Archer(CQ_AmbushTradersScript& parent,CGameScriptInterfaceBase& g):ParentClass(parent),Game(g) {}
        bool IsActiveThreadTerminating() const { calls.push_back(StopCheck);return ++checks==config.stop; }
        void Main();
    };
};
}

namespace NScript {
void CQ_AmbushTradersScript::CAT_Phase1Archer::Main()
{
    TRetVoid _Fail_Val;
    CScriptGameResourceObjectScriptedThingBase sehMe;
    if (false) {
    } else for (;; Game.NewScriptFrame()) {
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
        if (ParentClass.QuestFailed || ParentClass.QuestFinished) {
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
            if (sehMe.IsValid())
                sehMe.Release();
            while (!Game.StartScriptingEntity(Me, sehMe, SCRIPT_AI_PRIORITY_HIGHEST)) {
                Game.NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
            CScriptThing Exit = Game.GetNearestWithDefName(Me, "REGION_EXIT_POINT");
            if (Exit.IsAlive()) {
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
                if (sehMe.IsValid()) {
                    if (IsActiveThreadTerminating())
                        return _Fail_Val.Return();
                    sehMe.MoveToPosition(Exit.GetPos(), 1.0f, ENTITY_MOVE_RUN, false, true);
                }
                while (!IsDistanceBetweenThingsUnder(Me, Exit, 2.0f)) {
                    Game.NewScriptFrame();
                    if (IsActiveThreadTerminating())
                        return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
                FadeOutAndKillEntity(Me, 1.0f, true, true);
            }
        }
    }
    if (IsActiveThreadTerminating())
        return _Fail_Val.Return();
    while (true) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating())
        return _Fail_Val.Return();
}
}

static void expected(std::vector<Event>& events)
{
    enum State { Begin, QuestReady, TryStart, AfterStart, ReadyMove, BeforeMove, WaitDistance, Near, FinishCycle };
    State state=Begin;
    int checkCount=0, attempt=0, distances=0;
    bool control=config.initialControl;
    for (;;) {
        if (state!=TryStart && state!=WaitDistance && state!=FinishCycle) {
            events.push_back(StopCheck);if (++checkCount==config.stop) return;
        }
        switch(state) {
        case Begin: state=(config.failed||config.finished)?QuestReady:FinishCycle;break;
        case QuestReady:
            events.push_back(Valid);if(control)events.push_back(Release);control=false;state=TryStart;break;
        case TryStart:
            events.push_back(Start);
            if(++attempt>config.startDelay){control=config.acquiredControl;state=AfterStart;}
            else {events.push_back(Frame);events.push_back(StopCheck);if(++checkCount==config.stop)return;}
            break;
        case AfterStart: events.push_back(Nearest);events.push_back(Alive);state=config.alive?ReadyMove:FinishCycle;break;
        case ReadyMove: events.push_back(Valid);state=control?BeforeMove:WaitDistance;break;
        case BeforeMove: events.push_back(Position);events.push_back(Move);state=WaitDistance;break;
        case WaitDistance:
            events.push_back(Distance);
            if(++distances>config.distanceDelay)state=Near;
            else {events.push_back(Frame);events.push_back(StopCheck);if(++checkCount==config.stop)return;}
            break;
        case Near: events.push_back(Fade);state=FinishCycle;break;
        case FinishCycle: events.push_back(Frame);state=Begin;break;
        }
    }
}
int main()
{
    for(int flags=0; flags<64; ++flags) for(int start=0; start<3; ++start)
        for(int distance=0; distance<3; ++distance) for(int stop=1; stop<=20; ++stop) {
            config.stop=stop; config.startDelay=start; config.distanceDelay=distance;
            config.failed=(flags&1)!=0;config.finished=(flags&2)!=0;config.alive=(flags&4)!=0;
            config.initialControl=(flags&8)!=0;config.acquiredControl=(flags&16)!=0;config.hasPosition=(flags&32)!=0;
            checks=startCalls=distanceCalls=0;calls.clear();
            std::vector<Event> reference;expected(reference);
            NScript::CQ_AmbushTradersScript parent={config.failed,config.finished};
            CGameScriptInterfaceBase game;
            NScript::CQ_AmbushTradersScript::CAT_Phase1Archer archer(parent,game);
            archer.Main();
            check(calls==reference && checks==stop && liveHandles==0 && liveExits==0);
        }
    puts("00f04000_TEST PASS");
    return 0;
}