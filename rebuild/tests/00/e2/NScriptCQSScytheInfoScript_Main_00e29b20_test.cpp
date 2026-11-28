// Runs the original method with synchronous script, allocation and game-service fixtures.
#include <new>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef _MSC_VER
#define FABLE_RETAIL_PASS23_HOST_TEST_ABI
#endif
#include "engine/TRetVoid.h"
class CCharString {
public:
    CCharString(const char* p="") : text(p) { }
    const char* text;
};
struct Trace {
    int region_frames, wait_frames, stop_at, fail_allocation, tries, binds, polls;
    int region_checks, frames, clock_checks, starts, releases, resets, notices, success_at;
    int completions, deactivations, fades, posts, quest_names, objective;
    bool fail;
    int events[260], used;
    void log(int e) { if(used==260) {fail=true; return;} events[used++]=e; }
};
static Trace* state;
static void verify(bool v) { if(!v)state->fail=true; }
static bool equal(const CCharString& v,const char* wanted) {return strcmp(v.text,wanted)==0;}
class CGameScriptInterfaceBase {
public:
    bool* mission;
    void PostAddScriptedEntities(){ verify(state->posts==1);++state->posts;state->log(4); }
    CCharString GetActiveQuestName(){++state->quest_names;return CCharString("ScytheQuest");}
    void SetQuestCardObjective(const CCharString& q,const CCharString& message,const CCharString& blank,const CCharString& region){
        verify(state->posts==2 && equal(q,"ScytheQuest") && equal(message,"TEXT_QUEST_AWAKEN_ORACLE_OBJECTIVE_03") && equal(blank,"") && equal(region,"NorthernWastes3"));
        ++state->objective;state->log(5);
    }
    bool IsRegionLoaded(const CCharString& place) {
        verify(equal(place,"NorthernWastes2"));state->log(8);return state->region_checks++ < state->region_frames;
    }
    void NewScriptFrame(){++state->frames;state->log(9);if(state->frames>=state->success_at)*mission=true;}
    long RegisterTimer(){++state->starts;state->log(6);return 37;}
    void DeregisterTimer(long n){verify(n==37 && state->releases==0);++state->releases;state->log(19);}
    void SetTimer(long n,long time){verify(n==37 && time==100);++state->resets;state->log(7);}
    long GetTimer(long n){verify(n==37);state->log(11);++state->clock_checks;return state->clock_checks%2 ? 0 : 9;}
    void HeroReceiveMessageFromGuildMaster(const CCharString& label,const CCharString& blank,bool a,bool b){
        verify(equal(label,"TEXT_QST_B03_SCYTHE_RETURN_TO_SNOWSPIRE_REMINDER_10") && equal(blank,"") && a && b);
        ++state->notices;state->log(12);
    }
    void SetQuestAsCompleted(const CCharString& q,bool a,bool b,bool c){
        verify(equal(q,"ScytheQuest")&&!a&&!b&&!c&&*mission);++state->completions;state->log(16);
    }
    void DeactivateQuestLater(const CCharString& q,unsigned long when){
        verify(equal(q,"ScytheQuest")&&when==0);++state->deactivations;state->log(17);
    }
    void FadeScreenIn(){++state->fades;state->log(18);}
};
#define FABLE_ENGINE_CGAMESCRIPTINTERFACEBASE_H
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CTIMER_ORIGINAL_ACCESS
#define FABLE_RETAIL_HANGING_TIMER_INLINES_PASS30
#define FABLE_RETAIL_SCYTHE_INFO_PASS30
#include "engine/CTimer.h"
CGameScriptInterfaceBase* pDSTGame;
class Allocation {
public:
    static void* operator new(size_t size) throw() {
        int n=++state->tries;state->log(1);return n==state->fail_allocation ? 0 : malloc(size);
    }
    static void operator delete(void* p){free(p);}
};
template<class S> class CEntityScriptBinding : public Allocation {
public:
    typedef void (*Factory)();
    CEntityScriptBinding(const CCharString& n,S* p,Factory f,unsigned long flags)
        : name(n),parent(p),factory(f),flags(flags) { }
    CCharString name;
    S* parent;
    Factory factory;
    unsigned long flags;
};
namespace NScript {
class CQS_ScytheInfoScript {
public:
    CGameScriptInterfaceBase& Game;
    bool MissionSucceeded;
    explicit CQS_ScytheInfoScript(CGameScriptInterfaceBase& g):Game(g),MissionSucceeded(false){g.mission=&MissionSucceeded;}
    struct CScytheMarker {static void Alloc() {state->log(81);} };
    struct CScytheNearOracle {static void Alloc() {state->log(82);} };
    void AddEntityScriptBinding(CEntityScriptBinding<CQS_ScytheInfoScript>* binding) {
        ++state->binds;
        verify((binding==0)==(state->binds==state->fail_allocation));
        if(binding) {
            verify(binding->parent==this && binding->flags==1);
            verify(equal(binding->name,state->binds==1 ? "ScytheMarker" : "ScytheNearOracle"));
            verify(binding->factory==(state->binds==1 ? &CScytheMarker::Alloc : &CScytheNearOracle::Alloc));
            delete binding;
        }
        state->log(2);
    }
    void PostAddScriptedEntities(){verify(state->binds==2);++state->posts;state->log(3);}
    bool IsActiveThreadTerminating(){++state->polls;state->log(10);return state->polls==state->stop_at;}
    void Main();
};
}

void NScript::CQS_ScytheInfoScript::Main()
{
    TRetVoid _Fail_Val;
    AddEntityScriptBinding(new CEntityScriptBinding<CQS_ScytheInfoScript>("ScytheMarker", this, &CScytheMarker::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQS_ScytheInfoScript>("ScytheNearOracle", this, &CScytheNearOracle::Alloc, 1));
    PostAddScriptedEntities();
    Game.PostAddScriptedEntities();
    Game.SetQuestCardObjective(Game.GetActiveQuestName(), "TEXT_QUEST_AWAKEN_ORACLE_OBJECTIVE_03", "", "NorthernWastes3");
    CTimer SnowspireReminder;
    SnowspireReminder = 100;
    while (Game.IsRegionLoaded("NorthernWastes2")) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
        if (SnowspireReminder == 0) {
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
            Game.HeroReceiveMessageFromGuildMaster("TEXT_QST_B03_SCYTHE_RETURN_TO_SNOWSPIRE_REMINDER_10", "", true, true);
            SnowspireReminder = 100;
        }
    }
    if (IsActiveThreadTerminating())
        return _Fail_Val.Return();
    while (!MissionSucceeded) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating())
        return _Fail_Val.Return();
    Game.SetQuestAsCompleted(Game.GetActiveQuestName(), false, false, false);
    Game.DeactivateQuestLater(Game.GetActiveQuestName(), 0);
    Game.FadeScreenIn();
}
namespace {
struct Oracle {
    int result[260], count, polls, stop;
    bool done;
    void event(int n){ if(!done) result[count++]=n; }
    void poll(){ if(done)return;event(10);if(++polls==stop)done=true; }
    void build(int region, int wait, int cancel) {
        count=polls=0;stop=cancel;done=false;
        const int prefix[]={1,2,1,2,3,4,5,6,7};
        for(unsigned int i=0;i<sizeof(prefix)/sizeof(prefix[0]);++i)event(prefix[i]);
        for(int i=0;i<region;++i){
            event(8);event(9);poll();event(11);
            if(i%2==0){poll();event(12);event(7);}
        }
        event(8);poll();
        for(int i=0;i<wait;++i){event(9);poll();}
        poll();event(16);event(17);event(18);
        done=false;event(19);
    }
};
}
int main() {
    for(int region=0;region<=4;++region) for(int wait=0;wait<=3;++wait)
    for(int cancel=0;cancel<=13;++cancel) for(int fail=0;fail<=2;++fail) {
        Trace trace;
        memset(&trace,0,sizeof(trace)); state=&trace;trace.region_frames=region;
        trace.success_at=region+wait;trace.stop_at=cancel;trace.fail_allocation=fail;
        CGameScriptInterfaceBase game; pDSTGame=&game;
        NScript::CQS_ScytheInfoScript script(game);
        script.MissionSucceeded=trace.success_at==0;
        script.Main();
        Oracle oracle;oracle.build(region,wait,cancel);
        verify(trace.used==oracle.count && memcmp(trace.events,oracle.result,oracle.count*sizeof(int))==0);
        verify(trace.starts==1&&trace.releases==1&&trace.tries==2&&trace.binds==2&&trace.posts==2&&trace.objective==1);
        verify(trace.completions==trace.deactivations&&trace.fades==trace.completions&&trace.quest_names==1+2*trace.completions);
        if(trace.fail) {fprintf(stderr,"case %d %d %d %d\n",region,wait,cancel,fail);return 1;}
    }
    puts("00e29b20_TEST PASS");
    return 0;
}