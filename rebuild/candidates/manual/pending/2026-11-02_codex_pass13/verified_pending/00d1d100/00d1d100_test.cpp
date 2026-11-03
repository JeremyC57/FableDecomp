// 00d1d100 behavior fixture. The exact production Main body is pasted below.
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
static int frames, ready_after, checks, stop_at, spawns, thing_live, objectives, gate_queries, closes, persists;
static int events[32], event_count;
static void check(bool value) { if(!value) ++errors; }
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
class CSpawnedFuncBase { public: virtual ~CSpawnedFuncBase() {} };
class CScriptDataBase {};
class CEntityScriptBase {};
template<class T> class CCountedPointer { public: T* Data; CCountedPointer(T* p=0):Data(p) {} };
class CEntityScriptBindingBase { public: virtual ~CEntityScriptBindingBase() {} };
class CGameScriptInterfaceBase {
public:
    virtual ~CGameScriptInterfaceBase() {}
    virtual CCharString GetActiveQuestName() const { return CCharString("Q_BreakSiege"); }
    virtual void SetQuestCardObjective(const CCharString& quest,const CCharString& objective,const CCharString& first,const CCharString& second) const {
        check(!strcmp(quest.text,"Q_BreakSiege"));
        check(!strcmp(objective.text,"TEXT_QUEST_BREAK_SIEGE_OBJECTIVE_01"));
        check(!strcmp(first.text,"KnotholeGlade")&&!strcmp(second.text,"KnotholeGlade"));
        check(registration_index==0&&event_count==0);++objectives;
    }
    virtual bool IsRegionLoaded(const CCharString& name) const {
        check(!strcmp(name.text,"KnotholeGlade"));check(event_count==2);return frames>=ready_after;
    }
    virtual void NewScriptFrame() const { ++frames; }
    virtual CScriptThing GetThingWithScriptName(const CCharString& name) const {
        check(!strcmp(name.text,"KG_Gate"));check(spawns==3);++gate_queries;return CScriptThing(71);
    }
    virtual void CloseDoor(const CScriptThing& gate) const { check(gate.Identity==71);++closes; }
    virtual void SetThingPersistent(const CScriptThing& gate,bool value) const {
        check(gate.Identity==71&&value&&closes==1);++persists;
    }
    virtual void PostAddScriptedEntities() const { events[event_count++]=2; }
    virtual void SetCreatureGeneratorsEnabled(const CCharString& region,bool on) const {
        check(!strcmp(region.text,"LostBay"));check(!on);events[event_count++]=3;
    }
};
class CScriptBase {
public:
    void AddEntityScriptBinding(CEntityScriptBindingBase*);
    void AddSpawnedFunction(CSpawnedFuncBase*,const CCharString&);
    bool IsActiveThreadTerminating() { ++checks;return stop_at!=0 && checks>=stop_at; }
    void PostAddScriptedEntities() { check(registration_index==9);events[event_count++]=1; }
};
namespace NScript {
class CQ_BreakSiegeScript:public CScriptBase {
public:
    CQ_BreakSiegeScript(CGameScriptInterfaceBase& game) : Game(game) {}
    CGameScriptInterfaceBase& Game;
    bool BanditSiegeStart;
    void WatchForOpenGates() {}
    void WatchForWinLoseConditions() {}
    void WatchForTermination() {}
    void Main();
    class CImprisonedBanditLeader { public: static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CQ_BreakSiegeScript&, CScriptDataBase*, const CScriptThing&) { return CCountedPointer<CEntityScriptBase>(); } };
    class CKG_ChiefBreakSiege { public: static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CQ_BreakSiegeScript&, CScriptDataBase*, const CScriptThing&) { return CCountedPointer<CEntityScriptBase>(); } };
    class CGuardToHelpDefend { public: static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CQ_BreakSiegeScript&, CScriptDataBase*, const CScriptThing&) { return CCountedPointer<CEntityScriptBase>(); } };
    class CBanditAttacker { public: static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CQ_BreakSiegeScript&, CScriptDataBase*, const CScriptThing&) { return CCountedPointer<CEntityScriptBase>(); } };
    class CBanditSiegeLeader { public: static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CQ_BreakSiegeScript&, CScriptDataBase*, const CScriptThing&) { return CCountedPointer<CEntityScriptBase>(); } };
    class CKG_Sheriff { public: static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CQ_BreakSiegeScript&, CScriptDataBase*, const CScriptThing&) { return CCountedPointer<CEntityScriptBase>(); } };
    class CGuardHoldingBandit { public: static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CQ_BreakSiegeScript&, CScriptDataBase*, const CScriptThing&) { return CCountedPointer<CEntityScriptBase>(); } };
    class CBanditFirstWave { public: static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CQ_BreakSiegeScript&, CScriptDataBase*, const CScriptThing&) { return CCountedPointer<CEntityScriptBase>(); } };
    class CDisplacedTrader { public: static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CQ_BreakSiegeScript&, CScriptDataBase*, const CScriptThing&) { return CCountedPointer<CEntityScriptBase>(); } };
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
typedef NScript::CQ_BreakSiegeScript Script;
typedef CEntityScriptBinding<Script> Binding;
static Script* expected_parent;
static Binding* saved[9];
static const char* expected_names[9]={
"ImprisonedBanditLeader",
"KG_ChiefBreakSiege",
"GuardToHelpDefend",
"BanditAttacker",
"BanditSiegeLeader",
"KG_Sheriff",
"GuardHoldingBandit",
"BanditFirstWave",
"DisplacedTrader"};
static Binding::TAllocFunc expected_factory[9]={
&Script::CImprisonedBanditLeader::Alloc,
&Script::CKG_ChiefBreakSiege::Alloc,
&Script::CGuardToHelpDefend::Alloc,
&Script::CBanditAttacker::Alloc,
&Script::CBanditSiegeLeader::Alloc,
&Script::CKG_Sheriff::Alloc,
&Script::CGuardHoldingBandit::Alloc,
&Script::CBanditFirstWave::Alloc,
&Script::CDisplacedTrader::Alloc};
void CScriptBase::AddEntityScriptBinding(CEntityScriptBindingBase* p) {
    int i=registration_index++;check(i<9);if(i>=9)return;
    check((p==0)==((failure_mask&(1u<<i))!=0));
    Binding* b=static_cast<Binding*>(p);saved[i]=b;
    if(b) {
        check(!strcmp(b->EntityScriptName.text,expected_names[i]));
        check(b->PParentScript==expected_parent);
        check(b->ClassAllocFunc==expected_factory[i]);
        check(b->ClassAllocValid);check(b->Flags==1);
        check(string_live==retained+2);++retained;
    } else check(string_live==retained);
}

template<class T> class CSpawnedFunc:public CSpawnedFuncBase {
public:
    CCharString Name;unsigned long ID;T* PClass;void (T::*Func)();
    CSpawnedFunc(const CCharString& n,unsigned long id,T* parent,void (T::*f)())
        :Name(n),ID(id),PClass(parent),Func(f) {}
    static void* operator new(size_t n) throw() {
        unsigned bit=1u<<allocation_index++;return (failure_mask&bit)?0:malloc(n);
    }
    static void operator delete(void* p) { free(p); }
};
static CSpawnedFunc<Script>* saved_threads[3];
void CScriptBase::AddSpawnedFunction(CSpawnedFuncBase* p,const CCharString& region) {
    int i=spawns++;check(i<3);if(i>=3)return;
    check(!strcmp(region.text,i==2?"":"KnotholeGlade"));
    check((p==0)==((failure_mask&(1u<<(i+9)))!=0));
    CSpawnedFunc<Script>* f=static_cast<CSpawnedFunc<Script>*>(p);saved_threads[i]=f;
    if(f) {
        const char* name=i==0?"WatchForOpenGates":i==1?"WatchForWinLoseConditions":"WatchForTermination";
        void (Script::*callback)()=i==0?&Script::WatchForOpenGates:i==1?&Script::WatchForWinLoseConditions:&Script::WatchForTermination;
        check(!strcmp(f->Name.text,name));check(f->ID==0&&f->PClass==expected_parent);check(f->Func==callback);
    }
}
void NScript::CQ_BreakSiegeScript::Main()
{
    Game.SetQuestCardObjective(Game.GetActiveQuestName(), "TEXT_QUEST_BREAK_SIEGE_OBJECTIVE_01", "KnotholeGlade", "KnotholeGlade");
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_BreakSiegeScript>("ImprisonedBanditLeader", this, &CImprisonedBanditLeader::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_BreakSiegeScript>("KG_ChiefBreakSiege", this, &CKG_ChiefBreakSiege::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_BreakSiegeScript>("GuardToHelpDefend", this, &CGuardToHelpDefend::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_BreakSiegeScript>("BanditAttacker", this, &CBanditAttacker::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_BreakSiegeScript>("BanditSiegeLeader", this, &CBanditSiegeLeader::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_BreakSiegeScript>("KG_Sheriff", this, &CKG_Sheriff::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_BreakSiegeScript>("GuardHoldingBandit", this, &CGuardHoldingBandit::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_BreakSiegeScript>("BanditFirstWave", this, &CBanditFirstWave::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_BreakSiegeScript>("DisplacedTrader", this, &CDisplacedTrader::Alloc, 1));
    PostAddScriptedEntities();
    Game.PostAddScriptedEntities();
    while (!Game.IsRegionLoaded("KnotholeGlade")) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
    }
    if (IsActiveThreadTerminating()) return;
    AddSpawnedFunction(new CSpawnedFunc<CQ_BreakSiegeScript>("WatchForOpenGates", 0, this, &CQ_BreakSiegeScript::WatchForOpenGates), "KnotholeGlade");
    AddSpawnedFunction(new CSpawnedFunc<CQ_BreakSiegeScript>("WatchForWinLoseConditions", 0, this, &CQ_BreakSiegeScript::WatchForWinLoseConditions), "KnotholeGlade");
    AddSpawnedFunction(new CSpawnedFunc<CQ_BreakSiegeScript>("WatchForTermination", 0, this, &CQ_BreakSiegeScript::WatchForTermination), CCharString(""));
    if (!BanditSiegeStart) {
        if (IsActiveThreadTerminating()) return;
        CScriptThing Gate = Game.GetThingWithScriptName("KG_Gate");
        Game.CloseDoor(Gate);
        Game.SetThingPersistent(Gate, true);
    }
}


int main() {
    CGameScriptInterfaceBase game;
    Script script(game);expected_parent=&script;
    for(int delay=0;delay<3;++delay)for(int started=0;started<2;++started) {
        script.BanditSiegeStart=started!=0;
        int first_guard=delay+1;
        int final_guard=first_guard+!script.BanditSiegeStart;
        for(int ending=0;ending<=final_guard;++ending)for(int alloc_case=0;alloc_case<14;++alloc_case) {
            ready_after=delay;stop_at=ending;
            failure_mask=alloc_case==0?0:alloc_case==13?4095u:1u<<(alloc_case-1);
            frames=checks=spawns=allocation_index=registration_index=retained=event_count=0;
            objectives=gate_queries=closes=persists=0;
            memset(saved,0,sizeof(saved));memset(saved_threads,0,sizeof(saved_threads));
            script.Main();
            int effective=ending==0?final_guard+1:ending;
            check(objectives==1&&registration_index==9);
            check(event_count==2&&events[0]==1&&events[1]==2);
            check(frames==(ending!=0&&ending<=delay?ending:delay));
            check(checks==(ending==0?final_guard:ending));
            check(spawns==(effective>first_guard?3:0));
            check(allocation_index==9+spawns);
            check(gate_queries==(!started&&ending==0?1:0));
            check(closes==gate_queries&&persists==gate_queries);
            check(thing_live==0);
            for(int i=0;i<9;++i)delete saved[i];
            for(int j=0;j<3;++j)delete saved_threads[j];
            check(string_live==0);
        }
    }
    if(errors) { fprintf(stderr,"checks failed: %d\n",errors);return 1; }
    puts("00d1d100_TEST PASS");return 0;
}
