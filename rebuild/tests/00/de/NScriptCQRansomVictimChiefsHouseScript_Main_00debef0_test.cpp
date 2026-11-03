// 00debef0 behavior fixture. The exact production Main body is pasted below.
// Dependency doubles record registration, allocation failure, and call order.
// Native layout and symbol identity are checked separately on the production object.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#ifndef _MSC_VER
#define __fastcall
#endif
static int errors, string_live, allocation_index, registration_index, retained;
static unsigned failure_mask;
enum Event { REGISTER=1, NOTIFY_SCRIPT, NOTIFY_GAME, FRAME, TERMINATING, REGION, QUEST, LOOKUP, RESIDENCY, ALIVE, LOCK, OPEN };
static int plan_kind[256],plan_value[256],plan_aux[256],plan_size,cursor,checks,stop_at,thing_live;
static void append(int kind,int value=0,int aux=0) { plan_kind[plan_size]=kind;plan_value[plan_size]=value;plan_aux[plan_size++]=aux; }
static int next(int kind);
static int events[32], event_count;
static void check(bool value) { if(!value) ++errors; }
static int next(int kind) {
    if(cursor>=plan_size) { ++errors;return kind==TERMINATING||kind==QUEST||kind==REGION; }
    check(plan_kind[cursor]==kind);int result=plan_value[cursor++];
    if(kind==TERMINATING) { ++checks;return stop_at!=0&&checks>=stop_at; }
    return result;
}
class CCharString {
public:
    const char* text;
    CCharString(const char* p, long n=-1):text(p) { check(n==-1);++string_live; }
    CCharString(const CCharString& other):text(other.text) { ++string_live; }
    ~CCharString() { --string_live; }
};
class CScriptThing {
public: int Identity,State;
    CScriptThing(int id=0,int state=0):Identity(id),State(state) { ++thing_live; }
    CScriptThing(const CScriptThing& other):Identity(other.Identity),State(other.State) { ++thing_live; }
    ~CScriptThing() { --thing_live; }
    bool IsAlive() const { check(Identity==2);next(ALIVE);return State==1; }
};
class CScriptDataBase {};
class CEntityScriptBase {};
template<class T> class CCountedPointer { public: T* Data; CCountedPointer(T* p=0):Data(p) {} };
class CEntityScriptBindingBase { public: virtual ~CEntityScriptBindingBase() {} };
class CGameScriptInterfaceBase {
public:
    virtual ~CGameScriptInterfaceBase() {}
    virtual bool IsRegionLoaded(const CCharString& name) const { check(!strcmp(name.text,"KnotholeGlade"));return next(REGION)!=0; }
    virtual bool IsQuestCompleted(const CCharString& name) const { check(!strcmp(name.text,"Q_RansomVictim"));return next(QUEST)!=0; }
    virtual void NewScriptFrame() const { next(FRAME); }
    virtual CScriptThing GetThingWithScriptName(const CCharString& name) const {
        next(LOOKUP);check(!strcmp(name.text,"ChiefsHut"));return CScriptThing(1,1);
    }
    virtual CScriptThing SetResidency(const CScriptThing& house,bool value) {
        int expected_house=plan_aux[cursor];int state=next(RESIDENCY);
        check(house.Identity==(expected_house&3));check(value==((expected_house&4)!=0));return CScriptThing(2,state);
    }
    virtual void OverrideAutomaticHouseLocking(const CScriptThing& house,bool value) const { next(LOCK);check(house.Identity==1&&value); }
    virtual void OpenHouseDoors(const CScriptThing& house) const { next(OPEN);check(house.Identity==1); }
    virtual void PostAddScriptedEntities() const { next(NOTIFY_GAME);events[event_count++]=2; }
    virtual void SetCreatureGeneratorsEnabled(const CCharString& region,bool on) const {
        check(!strcmp(region.text,"LostBay"));check(!on);events[event_count++]=3;
    }
};
class CScriptBase {
public:
    void AddEntityScriptBinding(CEntityScriptBindingBase*);
    bool IsActiveThreadTerminating() { return next(TERMINATING)!=0; }
    void PostAddScriptedEntities() { check(registration_index==1);next(NOTIFY_SCRIPT);events[event_count++]=1; }
};
namespace NScript {
class CQ_RansomVictimChiefsHouseScript:public CScriptBase {
public:
    CQ_RansomVictimChiefsHouseScript(CGameScriptInterfaceBase& game) : Game(game) {}
    CGameScriptInterfaceBase& Game;
    bool SetChiefsHut;
    void Main();
    class CTest { public: static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CQ_RansomVictimChiefsHouseScript&, CScriptDataBase*, const CScriptThing&) { return CCountedPointer<CEntityScriptBase>(); } };
};
}
template<class T> class CEntityScriptBinding:public CEntityScriptBindingBase {
public:
    typedef CCountedPointer<CEntityScriptBase> (__fastcall *TAllocFunc)(T&,CScriptDataBase*,const CScriptThing&);
    CCharString EntityScriptName;
    T* PParentScript;
    void (T::*Func)(CScriptThing);
    TAllocFunc ClassAllocFunc;
    bool ClassAllocValid;
    unsigned long Flags;
    CEntityScriptBinding(const CCharString& name,T* parent,TAllocFunc alloc,unsigned long flags)
        :EntityScriptName(name),PParentScript(parent),ClassAllocFunc(alloc),ClassAllocValid(true),Flags(flags) {}
    static void* operator new(size_t n) throw() {
        unsigned bit=1u<<allocation_index++;
        return (failure_mask&bit)?0:malloc(n);
    }
    static void operator delete(void* p) { free(p); }
};
typedef NScript::CQ_RansomVictimChiefsHouseScript Script;
typedef CEntityScriptBinding<Script> Binding;
static Script* expected_parent;
static Binding* saved[1];
static const char* expected_names[1]={
"Test"};
static Binding::TAllocFunc expected_factory[1]={
&Script::CTest::Alloc};
void CScriptBase::AddEntityScriptBinding(CEntityScriptBindingBase* p) {
    next(REGISTER);int i=registration_index++;check(i<1);if(i>=1)return;
    check((p==0)==((failure_mask&(1u<<i))!=0));
    Binding* b=static_cast<Binding*>(p);saved[i]=b;
    if(b) {
        check(!strcmp(b->EntityScriptName.text,expected_names[i]));
        check(b->PParentScript==expected_parent);
        check(b->ClassAllocFunc==expected_factory[i]);
        check(b->ClassAllocValid);check(b->Flags==0);
        check(string_live==retained+2);++retained;
    } else check(string_live==retained);
}

void NScript::CQ_RansomVictimChiefsHouseScript::Main()
{
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_RansomVictimChiefsHouseScript>("Test", this, &CTest::Alloc, 0));
    PostAddScriptedEntities();
    Game.PostAddScriptedEntities();
    if (!SetChiefsHut) {
        if (IsActiveThreadTerminating()) return;
        while (!Game.IsRegionLoaded("KnotholeGlade")) {
            Game.NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
        }
        if (IsActiveThreadTerminating()) return;
        CScriptThing ChiefsHouse = Game.GetThingWithScriptName("ChiefsHut");
        CScriptThing SecondaryHouse = Game.SetResidency(ChiefsHouse, false);
        if (SecondaryHouse.IsAlive()) {
            if (IsActiveThreadTerminating()) return;
            Game.SetResidency(SecondaryHouse, false);
        }
        Game.OverrideAutomaticHouseLocking(ChiefsHouse, true);
        Game.OpenHouseDoors(ChiefsHouse);
        SetChiefsHut = true;
    }
    while (!Game.IsQuestCompleted("Q_RansomVictim")) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
        if (Game.IsRegionLoaded("KnotholeGlade")) {
            if (IsActiveThreadTerminating()) return;
            CScriptThing ChiefsHouse = Game.GetThingWithScriptName("ChiefsHut");
            Game.OverrideAutomaticHouseLocking(ChiefsHouse, true);
            Game.OpenHouseDoors(ChiefsHouse);
            while (Game.IsRegionLoaded("KnotholeGlade") && !Game.IsQuestCompleted("Q_RansomVictim")) {
                Game.NewScriptFrame();
                if (IsActiveThreadTerminating()) return;
            }
            if (IsActiveThreadTerminating()) return;
        }
    }
    if (IsActiveThreadTerminating()) return;
    while (!Game.IsRegionLoaded("KnotholeGlade")) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
    }
    if (IsActiveThreadTerminating()) return;
    CScriptThing ChiefsHouse = Game.GetThingWithScriptName("ChiefsHut");
    Game.SetResidency(ChiefsHouse, true);
    CScriptThing SecondaryHouse = Game.SetResidency(ChiefsHouse, false);
    if (SecondaryHouse.IsAlive()) {
        if (IsActiveThreadTerminating()) return;
        Game.SetResidency(SecondaryHouse, false);
    }
}


// Plans are independent native call-block traces, including short-circuit omission
// of QUEST when REGION is false. Each guard is tested as a possible terminating exit.
static void wait_region(int delay) {
    for(int i=0;i<delay;++i) { append(REGION,0);append(FRAME);append(TERMINATING); }
    append(REGION,1);append(TERMINATING);
}
static void make_plan(int initialized,int first_state,int last_state,int route,int delay) {
    plan_size=0;append(REGISTER);append(NOTIFY_SCRIPT);append(NOTIFY_GAME);
    if(!initialized) {
        append(TERMINATING);wait_region(delay);append(LOOKUP);append(RESIDENCY,first_state,1);append(ALIVE);
        if(first_state==1) { append(TERMINATING);append(RESIDENCY,0,2); }
        append(LOCK);append(OPEN,1);
    }
    if(route!=0) {
        append(QUEST,0);append(FRAME);append(TERMINATING);append(REGION,route!=1);
        if(route!=1) {
            append(TERMINATING);append(LOOKUP);append(LOCK);append(OPEN,2);
            for(int i=0;i<delay;++i) { append(REGION,1);append(QUEST,0);append(FRAME);append(TERMINATING); }
            append(REGION,route==3);
            if(route==3)append(QUEST,1);
            append(TERMINATING);
        }
    }
    append(QUEST,1);append(TERMINATING);wait_region(delay);
    append(LOOKUP);append(RESIDENCY,0,5);append(RESIDENCY,last_state,1);append(ALIVE);
    if(last_state==1) { append(TERMINATING);append(RESIDENCY,0,2); }
}
int main() {
    CGameScriptInterfaceBase game;
    Script script(game);expected_parent=&script;
    for(int initialized=0;initialized<2;++initialized)for(int first_state=0;first_state<3;++first_state)for(int last_state=0;last_state<3;++last_state)for(int route=0;route<4;++route)for(int delay=0;delay<2;++delay) {
        make_plan(initialized,first_state,last_state,route,delay);
        int total_checks=0;
        for(int i=0;i<plan_size;++i)if(plan_kind[i]==TERMINATING)++total_checks;
        for(int ending=0;ending<=total_checks;++ending)for(int alloc_case=0;alloc_case<2;++alloc_case) {
            stop_at=ending;failure_mask=alloc_case;script.SetChiefsHut=initialized!=0;
            cursor=checks=allocation_index=registration_index=retained=event_count=0;
            memset(saved,0,sizeof(saved));script.Main();
            int expected_end=plan_size;
            if(ending) {
                int guards=0;
                for(int i=0;i<plan_size;++i)if(plan_kind[i]==TERMINATING&&++guards==ending) { expected_end=i+1;break; }
            }
            check(cursor==expected_end);check(checks==(ending?ending:total_checks));
            bool expected_initialized=initialized!=0;
            for(int j=0;j<expected_end;++j)if(plan_kind[j]==OPEN&&plan_value[j]==1)expected_initialized=true;
            check(script.SetChiefsHut==expected_initialized);check(allocation_index==1&&registration_index==1);
            check(event_count==2&&events[0]==1&&events[1]==2);check(thing_live==0);
            delete saved[0];check(string_live==0);
        }
    }
    if(errors) { fprintf(stderr,"checks failed: %d\n",errors);return 1; }
    puts("00debef0_TEST PASS");return 0;
}