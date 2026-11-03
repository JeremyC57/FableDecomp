// 00ea2e80 behavior fixture. The exact production Main body is pasted below.
// Dependency doubles record registration, allocation failure, and call order.
// Native layout and symbol identity are checked separately on the production object.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#ifndef _MSC_VER
#define __fastcall
#define __forceinline inline
#endif
static int errors, string_live, allocation_index, registration_index, retained;
static unsigned failure_mask;
enum Event { OBJECTIVE=1, FRAME, TERMINATING, LEVEL, LOOKUP, HERO, DISTANCE, REMOVE, MARKER, REGISTER, NOTIFY_SCRIPT, NOTIFY_GAME, SPAWN };
static int plan_kind[256],plan_value[256],plan_size,cursor,checks,stop_at,spawns,thing_live,reach_frames,reach_target;
static void on_frame();
static void append(int kind,int value=0) { plan_kind[plan_size]=kind;plan_value[plan_size++]=value; }
static int next(int kind);
static int events[32], event_count;
static void check(bool value) { if(!value) ++errors; }
static int next(int kind) {
    if(cursor>=plan_size) { ++errors;return kind==TERMINATING; }
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
public: int Identity;
    CScriptThing(int id=0):Identity(id) { ++thing_live; }
    CScriptThing(const CScriptThing& other):Identity(other.Identity) { ++thing_live; }
    ~CScriptThing() { --thing_live; }
};
static CScriptThing hero(1);
class CSpawnedFuncBase { public: CCharString Name; unsigned long ID; CSpawnedFuncBase(const CCharString& name,unsigned long id):Name(name),ID(id) {} virtual ~CSpawnedFuncBase() {} };
bool __fastcall IsDistanceBetweenThingsUnder(const CScriptThing& first,const CScriptThing& second,float distance) {
    check(first.Identity==1&&second.Identity==2&&distance==20.0f);return next(DISTANCE)!=0;
}
class CScriptDataBase {};
class CEntityScriptBase {};
template<class T> class CCountedPointer { public: T* Data; CCountedPointer(T* p=0):Data(p) {} };
class CEntityScriptBindingBase { public: virtual ~CEntityScriptBindingBase() {} };
class CGameScriptInterfaceBase {
public:
    virtual ~CGameScriptInterfaceBase() {}
    virtual CCharString GetActiveQuestName() const { return CCharString("V_LostTrader"); }
    virtual void SetQuestCardObjective(const CCharString& quest,const CCharString& objective,const CCharString& region,const CCharString& extra) const {
        int i=next(OBJECTIVE);
        check(!strcmp(quest.text,"V_LostTrader"));
        check(!strcmp(objective.text,i==1?"TEXT_QUEST_LOST_TRADER_OBJECTIVE_01":"TEXT_QUEST_LOST_TRADER_OBJECTIVE_02"));
        check(!strcmp(region.text,"BanditCampPath1")&&!strcmp(extra.text,""));
    }
    virtual bool IsLevelLoaded(const CCharString& name) const { check(!strcmp(name.text,"BanditCampPath_1"));return next(LEVEL)!=0; }
    virtual void NewScriptFrame() const { next(FRAME);on_frame(); }
    virtual CScriptThing& GetHero() { next(HERO);return hero; }
    virtual CScriptThing GetThingWithScriptName(const CCharString& name) const {
        int i=next(LOOKUP);check(!strcmp(name.text,i==2?"LostTraderBrother":"LostTraderTrader"));return CScriptThing(i);
    }
    virtual void RemoveThing(const CScriptThing& thing,bool a,bool b) const { next(REMOVE);check(thing.Identity==3&&!a&&!b); }
    virtual void MiniMapAddMarker(const CScriptThing& thing,const CCharString& graphic) const {
        next(MARKER);check(thing.Identity==2&&!strcmp(graphic.text,"HUD_ORB_QUEST_OPTIONAL"));
    }
    virtual void PostAddScriptedEntities() const { next(NOTIFY_GAME);events[event_count++]=2; }
    virtual void SetCreatureGeneratorsEnabled(const CCharString& region,bool on) const {
        check(!strcmp(region.text,"LostBay"));check(!on);events[event_count++]=3;
    }
};
class CScriptBase {
public:
    void AddEntityScriptBinding(CEntityScriptBindingBase*);
    void AddSpawnedFunction(CSpawnedFuncBase*,const CCharString&);
    bool IsActiveThreadTerminating() { return next(TERMINATING)!=0; }
    void PostAddScriptedEntities() { check(registration_index==3);next(NOTIFY_SCRIPT);events[event_count++]=1; }
};
namespace NScript {
class CV_LostTraderScript:public CScriptBase {
public:
    CV_LostTraderScript(CGameScriptInterfaceBase& game) : Game(game) {}
    CGameScriptInterfaceBase& Game;
    bool ReachedTrader;
    void WatchForQuestFailure() {}
    void Main();
    class CLostTraderTrader { public: static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CV_LostTraderScript&, CScriptDataBase*, const CScriptThing&) { return CCountedPointer<CEntityScriptBase>(); } };
    class CLostTraderBrother { public: static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CV_LostTraderScript&, CScriptDataBase*, const CScriptThing&) { return CCountedPointer<CEntityScriptBase>(); } };
    class CLostTraderBanditSpawn { public: static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CV_LostTraderScript&, CScriptDataBase*, const CScriptThing&) { return CCountedPointer<CEntityScriptBase>(); } };
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
    __forceinline CEntityScriptBinding(const CCharString& name,T* parent,TAllocFunc alloc,unsigned long flags)
        :EntityScriptName(name),PParentScript(parent),ClassAllocFunc(alloc),ClassAllocValid(true),Flags(flags) {}
    static void* operator new(size_t n) throw() {
        unsigned bit=1u<<allocation_index++;
        return (failure_mask&bit)?0:malloc(n);
    }
    static void operator delete(void* p) { free(p); }
};
typedef NScript::CV_LostTraderScript Script;
typedef CEntityScriptBinding<Script> Binding;
static Script* expected_parent;
static Binding* saved[3];
static const char* expected_names[3]={
"LostTraderTrader",
"LostTraderBrother",
"LostTraderBanditSpawn"};
static Binding::TAllocFunc expected_factory[3]={
&Script::CLostTraderTrader::Alloc,
&Script::CLostTraderBrother::Alloc,
&Script::CLostTraderBanditSpawn::Alloc};
void CScriptBase::AddEntityScriptBinding(CEntityScriptBindingBase* p) {
    next(REGISTER);int i=registration_index++;check(i<3);if(i>=3)return;
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

template<class T> class CSpawnedFunc:public CSpawnedFuncBase {
public:
    void (T::*Func)();T* PClass;
    __forceinline CSpawnedFunc(const CCharString& name,unsigned long id,T* pclass,void (T::*func)())
        :CSpawnedFuncBase(name,id),Func(func),PClass(pclass) {}
    static void* operator new(size_t n) throw() {
        unsigned bit=1u<<allocation_index++;return (failure_mask&bit)?0:malloc(n);
    }
    static void operator delete(void* p) { free(p); }
};
static CSpawnedFunc<Script>* saved_thread;
static void on_frame() {
    if(spawns&&++reach_frames>=reach_target)expected_parent->ReachedTrader=true;
}
void CScriptBase::AddSpawnedFunction(CSpawnedFuncBase* p,const CCharString& region) {
    next(SPAWN);++spawns;check(spawns==1);
    check(!strcmp(region.text,""));check((p==0)==((failure_mask&8u)!=0));
    CSpawnedFunc<Script>* f=static_cast<CSpawnedFunc<Script>*>(p);saved_thread=f;
    if(f) {
        check(!strcmp(f->Name.text,"WatchForQuestFailure"));
        check(f->ID==0&&f->PClass==expected_parent&&f->Func==&Script::WatchForQuestFailure);
    }
}
void NScript::CV_LostTraderScript::Main()
{
    Game.SetQuestCardObjective(Game.GetActiveQuestName(), "TEXT_QUEST_LOST_TRADER_OBJECTIVE_01", "BanditCampPath1", "");
    bool HeroEnteredTheLevelFromCorrectEntrance = false;
    while (!HeroEnteredTheLevelFromCorrectEntrance) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
        while (!Game.IsLevelLoaded("BanditCampPath_1")) {
            Game.NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
        }
        if (IsActiveThreadTerminating()) return;
        HeroEnteredTheLevelFromCorrectEntrance = IsDistanceBetweenThingsUnder(Game.GetHero(), Game.GetThingWithScriptName("LostTraderBrother"), 20.0f);
        if (!HeroEnteredTheLevelFromCorrectEntrance) {
            if (IsActiveThreadTerminating()) return;
            Game.RemoveThing(Game.GetThingWithScriptName("LostTraderTrader"), false, false);
            Game.MiniMapAddMarker(Game.GetThingWithScriptName("LostTraderBrother"), "HUD_ORB_QUEST_OPTIONAL");
            while (Game.IsLevelLoaded("BanditCampPath_1") != false) {
                Game.NewScriptFrame();
                if (IsActiveThreadTerminating()) return;
            }
            if (IsActiveThreadTerminating()) return;
        } else {
            if (IsActiveThreadTerminating()) return;
        }
    }
    if (IsActiveThreadTerminating()) return;
    AddEntityScriptBinding(new CEntityScriptBinding<CV_LostTraderScript>("LostTraderTrader", this, &CLostTraderTrader::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_LostTraderScript>("LostTraderBrother", this, &CLostTraderBrother::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_LostTraderScript>("LostTraderBanditSpawn", this, &CLostTraderBanditSpawn::Alloc, 0));
    PostAddScriptedEntities();
    Game.PostAddScriptedEntities();
    AddSpawnedFunction(new CSpawnedFunc<CV_LostTraderScript>("WatchForQuestFailure", 0, this, &CV_LostTraderScript::WatchForQuestFailure), CCharString(""));
    while (!ReachedTrader) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
    }
    if (IsActiveThreadTerminating()) return;
    Game.SetQuestCardObjective(Game.GetActiveQuestName(), "TEXT_QUEST_LOST_TRADER_OBJECTIVE_02", "BanditCampPath1", "");
}


// This call-trace oracle follows the independently read native blocks. It supplies
// external readiness and distance results, and checks every observable call in order.
static void make_plan(int wrong_entries,int loading,int unloading,int reaching) {
    plan_size=0;append(OBJECTIVE,1);
    for(int visit=0;visit<=wrong_entries;++visit) {
        append(FRAME);append(TERMINATING);
        for(int wait=0;wait<loading;++wait) { append(LEVEL,0);append(FRAME);append(TERMINATING); }
        append(LEVEL,1);append(TERMINATING);
        append(LOOKUP,2);append(HERO);append(DISTANCE,visit==wrong_entries);append(TERMINATING);
        if(visit<wrong_entries) {
            append(LOOKUP,3);append(REMOVE);append(LOOKUP,2);append(MARKER);
            for(int wait=0;wait<unloading;++wait) { append(LEVEL,1);append(FRAME);append(TERMINATING); }
            append(LEVEL,0);append(TERMINATING);
        }
    }
    append(TERMINATING);
    append(REGISTER);append(REGISTER);append(REGISTER);
    append(NOTIFY_SCRIPT);append(NOTIFY_GAME);append(SPAWN);
    for(int wait=0;wait<reaching;++wait) { append(FRAME);append(TERMINATING); }
    append(TERMINATING);append(OBJECTIVE,2);
}
int main() {
    CGameScriptInterfaceBase game;
    Script script(game);expected_parent=&script;
    for(int wrong=0;wrong<3;++wrong)for(int loading=0;loading<2;++loading)for(int unloading=0;unloading<2;++unloading)for(int reaching=0;reaching<3;++reaching) {
        make_plan(wrong,loading,unloading,reaching);
        int total_checks=0;
        for(int i=0;i<plan_size;++i)if(plan_kind[i]==TERMINATING)++total_checks;
        for(int ending=0;ending<=total_checks;++ending)for(int alloc_case=0;alloc_case<6;++alloc_case) {
            stop_at=ending;reach_target=reaching;script.ReachedTrader=reaching==0;
            failure_mask=alloc_case==0?0:alloc_case==5?15u:1u<<(alloc_case-1);
            cursor=checks=spawns=reach_frames=allocation_index=registration_index=retained=event_count=0;
            memset(saved,0,sizeof(saved));saved_thread=0;
            script.Main();
            int expected_end=plan_size;
            if(ending) {
                int guards=0;
                for(int i=0;i<plan_size;++i)if(plan_kind[i]==TERMINATING&&++guards==ending) { expected_end=i+1;break; }
            }
            check(cursor==expected_end);check(checks==(ending?ending:total_checks));
            int allocations=0;
            for(int j=0;j<expected_end;++j)if(plan_kind[j]==REGISTER||plan_kind[j]==SPAWN)++allocations;
            check(allocation_index==allocations);check(thing_live==1);
            for(int k=0;k<3;++k)delete saved[k];delete saved_thread;
            check(string_live==0);
        }
    }
    if(errors) { fprintf(stderr,"checks failed: %d\n",errors);return 1; }
    puts("00ea2e80_TEST PASS");return 0;
}
