// 00eeef70: exact selected Main body, dependency doubles, independent native event plans.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
static int errors, strings, handles, allocations, scenario_count;
static int plan[192], answer[192], count, cursor, checks, ending, frames, trigger;
static int chest_state, allocation_fails, retained_threads;
enum Event { LOOKUP=1, ALIVE, ANIMATION, BOAST, SPAWN, OBJECTIVE, MESSAGE, FRAME, TERMINATING, COMPLETE, FAILED, KILL, QUEST_NAME, QUEST_COMPLETE };
static void check(bool value) { if(!value)++errors; }
static int next(int kind) {
    if(cursor>=count) { ++errors;return kind==TERMINATING||kind==MESSAGE; }
    check(plan[cursor]==kind);int value=answer[cursor++];
    if(kind==TERMINATING) { ++checks;return ending!=0&&checks==ending; }
    return value;
}
static void append(int kind,int value=0) { check(count<192);plan[count]=kind;answer[count++]=value; }
class CCharString {
public:
    const char* text;
    CCharString(const char* p,long n=-1):text(p) { check(n==-1);++strings; }
    CCharString(const CCharString& p):text(p.text) { ++strings; }
    ~CCharString() { --strings; }
};
class CScriptThing {
public:
    int state;
    CScriptThing(int n=0):state(n) { ++handles; }
    CScriptThing(const CScriptThing& other):state(other.state) { ++handles; }
    ~CScriptThing() { --handles; }
    bool IsAlive() const { next(ALIVE);return state==1; }
};
class CSpawnedFuncBase {
public:
    CCharString Name;
    unsigned long ID;
    CSpawnedFuncBase(const CCharString& name,unsigned long id):Name(name),ID(id) { ++retained_threads; }
    virtual ~CSpawnedFuncBase() { --retained_threads; }
    virtual void Run()=0;
};
template<class T> class CSpawnedFunc:public CSpawnedFuncBase {
public:
    void (T::*Func)();T* PClass;
    CSpawnedFunc(const CCharString& name,unsigned long id,T* p,void(T::*f)())
        :CSpawnedFuncBase(name,id),Func(f),PClass(p) {}
    virtual void Run() { (PClass->*Func)(); }
    static void* operator new(size_t n) throw() { ++allocations;return allocation_fails?0:malloc(n); }
    static void operator delete(void* p) { free(p); }
};
static CSpawnedFuncBase* thread;
static void on_frame();
static const char* boast_text[4]={
    "I'll do this quest without trousers!",
    "I'll do this quest without using the flame sword!",
    "I'm not going to do this quest at all! Screw you all!",
    "I'll do this quest in 4 seconds flat!"};
class CGameScriptInterfaceBase {
public:
    CScriptThing GetThingWithScriptName(const CCharString& name) const {
        next(LOOKUP);check(!strcmp(name.text,"MagicChest"));return CScriptThing(chest_state);
    }
    void EntityPlayObjectAnimation(const CScriptThing& chest,const CCharString& name,bool loop) const {
        next(ANIMATION);check(chest.state==1&&!strcmp(name.text,"CHEST_OPEN")&&!loop);
    }
    void AddBoast(const CCharString& name,long id,long stake,long reward,bool required,const CCharString& extra,long mode) const {
        int b=next(BOAST);static const int stakes[4]={80,100,1,150},rewards[4]={150,180,-5,500};
        check(b>=1&&b<=4);if(b<1||b>4)return;
        check(id==b&&!strcmp(name.text,boast_text[b-1]));check(stake==stakes[b-1]&&reward==rewards[b-1]);
        check(!required&&extra.text[0]==0&&mode==0);check(strings==2);
    }
    void GiveHeroNewQuestObjective(const CCharString& text,unsigned long id) const {
        int expected=next(OBJECTIVE);const char* names[3]={"Go to the teddy bear region","Then come back to the region you came from!","Do it again, idiot!"};
        check(id==(unsigned long)expected);check(expected>=1&&expected<=3);
        if(expected>=1&&expected<=3)check(!strcmp(text.text,names[expected-1]));
    }
    bool MsgIsRegionLoaded(const CCharString& name) const {
        int encoded=next(MESSAGE);check(!strcmp(name.text,(encoded>>1)==1?"Region 1":"Region 2"));return (encoded&1)!=0;
    }
    void NewScriptFrame() const { next(FRAME);on_frame(); }
    void TellHeroQuestObjectiveCompleted(unsigned long id) const { check(id==(unsigned long)next(COMPLETE)); }
    void TellHeroQuestObjectiveFailed(unsigned long id) const { check(id==(unsigned long)next(FAILED)); }
    CCharString GetActiveQuestName() const { next(QUEST_NAME);return CCharString("TestQuestCard1"); }
    void SetQuestAsCompleted(const CCharString& name,bool first,bool second,bool third) const {
        next(QUEST_COMPLETE);check(!strcmp(name.text,"TestQuestCard1")&&first&&second&&!third);
    }
};
class CScriptBase {
public:
    bool IsActiveThreadTerminating() { return next(TERMINATING)!=0; }
    void AddSpawnedFunction(CSpawnedFuncBase*,const CCharString&);
    void KillSpawnedFunction(const CCharString& name,unsigned long id) {
        next(KILL);check(!strcmp(name.text,"WaitForBoasts")&&id==0);
    }
};
namespace NScript {
class CTestQuestCard1Script:public CScriptBase {
public:
    CTestQuestCard1Script(CGameScriptInterfaceBase& game) : Game(game) {}
    CGameScriptInterfaceBase& Game;bool Accepted4SecondBoast;
    void Main();
    void WaitForBoasts() { }
};
}
typedef NScript::CTestQuestCard1Script Script;
static Script* owner;
void CScriptBase::AddSpawnedFunction(CSpawnedFuncBase* p,const CCharString& region) {
    next(SPAWN);check(region.text[0]==0);check((p==0)==(allocation_fails!=0));thread=p;
    check(strings==(p?3:1));
    if(p) {
        CSpawnedFunc<Script>* f=static_cast<CSpawnedFunc<Script>*>(p);
        check(!strcmp(f->Name.text,"WaitForBoasts")&&f->ID==0&&f->PClass==owner);
        check(f->Func==&Script::WaitForBoasts);
        bool previous=owner->Accepted4SecondBoast;f->Run();check(owner->Accepted4SecondBoast==previous);
        // Controlled test input only: exercise the dormant public-state guard.
        // Retail WaitForBoasts is empty and does not perform this mutation.
        if(trigger==0)owner->Accepted4SecondBoast=true;
    }
}
static void on_frame() {
    ++frames;
    if(thread&&frames==trigger) {
        bool previous=owner->Accepted4SecondBoast;thread->Run();check(owner->Accepted4SecondBoast==previous);
        owner->Accepted4SecondBoast=true; // Deliberate external-state test input.
    }
}

void NScript::CTestQuestCard1Script::Main()
{
    Accepted4SecondBoast = false;
    CScriptThing chest = Game.GetThingWithScriptName("MagicChest");
    if (chest.IsAlive())
        Game.EntityPlayObjectAnimation(chest, "CHEST_OPEN", false);
    Game.AddBoast("I'll do this quest without trousers!", 1, 80, 150, false, "", 0);
    Game.AddBoast("I'll do this quest without using the flame sword!", 2, 100, 180, false, "", 0);
    Game.AddBoast("I'm not going to do this quest at all! Screw you all!", 3, 1, -5, false, "", 0);
    Game.AddBoast("I'll do this quest in 4 seconds flat!", 4, 150, 500, false, "", 0);
    AddSpawnedFunction(new CSpawnedFunc<CTestQuestCard1Script>("WaitForBoasts", 0, this, &CTestQuestCard1Script::WaitForBoasts), CCharString(""));
    Game.GiveHeroNewQuestObjective("Go to the teddy bear region", 1);
    Game.GiveHeroNewQuestObjective("Then come back to the region you came from!", 2);
    while (!Game.MsgIsRegionLoaded("Region 1")) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
    }
    if (IsActiveThreadTerminating()) return;
    Game.TellHeroQuestObjectiveCompleted(1);
    while (!Game.MsgIsRegionLoaded("Region 2")) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
    }
    if (IsActiveThreadTerminating()) return;
    if (Accepted4SecondBoast) {
        if (IsActiveThreadTerminating()) return;
    }
    Game.TellHeroQuestObjectiveFailed(2);
    Game.GiveHeroNewQuestObjective("Do it again, idiot!", 3);
    Game.NewScriptFrame();
    if (IsActiveThreadTerminating()) return;
    Game.NewScriptFrame();
    if (IsActiveThreadTerminating()) return;
    Game.NewScriptFrame();
    if (IsActiveThreadTerminating()) return;
    while (!Game.MsgIsRegionLoaded("Region 2")) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
    }
    if (IsActiveThreadTerminating()) return;
    KillSpawnedFunction("WaitForBoasts", 0);
    Game.TellHeroQuestObjectiveCompleted(3);
    Game.SetQuestAsCompleted(Game.GetActiveQuestName(), true, true, false);
}


static void make_plan(int first_delay,int second_delay,int third_delay) {
    count=0;append(LOOKUP);append(ALIVE);if(chest_state==1)append(ANIMATION);
    for(int i=1;i<=4;++i)append(BOAST,i);
    append(SPAWN);append(OBJECTIVE,1);append(OBJECTIVE,2);
    for(int i=0;i<first_delay;++i) { append(MESSAGE,2);append(FRAME);append(TERMINATING); }
    append(MESSAGE,3);append(TERMINATING);append(COMPLETE,1);
    for(int i=0;i<second_delay;++i) { append(MESSAGE,4);append(FRAME);append(TERMINATING); }
    append(MESSAGE,5);append(TERMINATING);
    if(!allocation_fails&&(trigger==0||(trigger>0&&trigger<=first_delay+second_delay)))append(TERMINATING);
    append(FAILED,2);append(OBJECTIVE,3);
    for(int i=0;i<3;++i) { append(FRAME);append(TERMINATING); }
    for(int i=0;i<third_delay;++i) { append(MESSAGE,4);append(FRAME);append(TERMINATING); }
    append(MESSAGE,5);append(TERMINATING);append(KILL);append(COMPLETE,3);append(QUEST_NAME);append(QUEST_COMPLETE);
}
int main() {
    CGameScriptInterfaceBase game;Script script(game);owner=&script;
    const int schedule[3]={-1,0,1};
    for(chest_state=0;chest_state<3;++chest_state)for(allocation_fails=0;allocation_fails<2;++allocation_fails)
    for(int sched=0;sched<3;++sched)for(int d1=0;d1<2;++d1)for(int d2=0;d2<2;++d2)for(int d3=0;d3<2;++d3) {
        trigger=schedule[sched];make_plan(d1,d2,d3);
        int guards=0;for(int i=0;i<count;++i)if(plan[i]==TERMINATING)++guards;
        for(ending=0;ending<=guards;++ending) {
            ++scenario_count;cursor=checks=frames=allocations=0;thread=0;script.Accepted4SecondBoast=true;
            script.Main();int expected_end=count;
            if(ending) { int g=0;for(int i=0;i<count;++i)if(plan[i]==TERMINATING&&++g==ending) { expected_end=i+1;break; } }
            check(cursor==expected_end&&checks==(ending?ending:guards));check(allocations==1);
            bool expected=!allocation_fails&&(trigger==0||(trigger>0&&trigger<=frames));
            check(script.Accepted4SecondBoast==expected);check(handles==0);check(strings==retained_threads);
            delete thread;thread=0;check(strings==0&&retained_threads==0);
        }
    }
    check(scenario_count==1266);
    if(errors) { fprintf(stderr,"checks failed: %d\n",errors);return 1; }
    puts("00eeef70_TEST PASS");return 0;
}
