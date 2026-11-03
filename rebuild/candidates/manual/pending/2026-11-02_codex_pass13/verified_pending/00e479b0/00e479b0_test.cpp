#ifndef _MSC_VER
#define __forceinline inline
#endif
// 00e479b0 behavior fixture. The exact production Main body is pasted below.
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
static int frames, ready_after, checks, stop_at, lookups, creations, spawns, thing_live;
static unsigned null_mask;
static int created[2];
static int events[32], event_count;
static void check(bool value) { if(!value) ++errors; }
class CCharString {
public:
    const char* text;
    CCharString(const char* p, long n=-1):text(p) { check(n==-1);++string_live; }
    CCharString(const CCharString& other):text(other.text) { ++string_live; }
    ~CCharString() { --string_live; }
};
class C3DVector {
public: float X,Y,Z;
    C3DVector(float x=0,float y=0,float z=0):X(x),Y(y),Z(z) {}
};
class CScriptThingImp {
public: C3DVector Position;
    CScriptThingImp(float x,float y,float z):Position(x,y,z) {}
    const C3DVector& GetPos() const { return Position; }
};
class CScriptThing {
public:
    struct { CScriptThingImp* Data; } PImp;
    static C3DVector NullCoord;
    CScriptThing(CScriptThingImp* p=0) { PImp.Data=p;++thing_live; }
    CScriptThing(const CScriptThing& other) { PImp.Data=other.PImp.Data;++thing_live; }
    ~CScriptThing() { --thing_live; }
    const C3DVector& GetPos() const;
};
C3DVector CScriptThing::NullCoord;
static CScriptThingImp points[2]={CScriptThingImp(2,4,8),CScriptThingImp(16,32,64)};
class CSpawnedFuncBase { public: CCharString Name;unsigned long ID;CSpawnedFuncBase(const CCharString& name,unsigned long id):Name(name),ID(id) {} virtual ~CSpawnedFuncBase() {} };
class CScriptDataBase {};
class CEntityScriptBase {};
template<class T> class CCountedPointer { public: T* Data; CCountedPointer(T* p=0):Data(p) {} };
class CEntityScriptBindingBase { public: virtual ~CEntityScriptBindingBase() {} };
class CGameScriptInterfaceBase {
public:
    virtual ~CGameScriptInterfaceBase() {}
    virtual bool IsRegionLoaded(const CCharString& name) const {
        check(!strcmp(name.text,"GreatwoodBanditToll"));return frames>=ready_after;
    }
    virtual void NewScriptFrame() const { ++frames; }
    virtual CScriptThing GetThingWithScriptName(const CCharString& name) const {
        int i=lookups++;check(i<2);
        check(!strcmp(name.text,i==0?"M_ReluctantTraderSpawnPoint":"M_SafeTraderSpawnPoint"));
        return CScriptThing((null_mask&(1u<<i))?0:&points[i]);
    }
    virtual CScriptThing CreateCreature(const CCharString& name,const C3DVector& pos,const CCharString& script,bool flag) const {
        check(!strcmp(name.text,"CREATURE_TRADER_01"));check(!flag);
        int i=!strcmp(script.text,"ReluctantTrader")?0:1;
        check(!strcmp(script.text,i==0?"ReluctantTrader":"SafeTrader"));
        const C3DVector& expected=(null_mask&(1u<<i))?CScriptThing::NullCoord:points[i].Position;
        check(pos.X==expected.X&&pos.Y==expected.Y&&pos.Z==expected.Z);
        ++creations;++created[i];return CScriptThing();
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
    void PostAddScriptedEntities() { check(registration_index==10);events[event_count++]=1; }
};
namespace NScript {
class CV_BanditTollScript:public CScriptBase {
public:
    CV_BanditTollScript(CGameScriptInterfaceBase& game) : Game(game) {}
    CGameScriptInterfaceBase& Game;
    bool TraderDead,SafeTraderAppeared;
    void CheckForLeaderDead() {}
    void ResetToll() {}
    void Main();
    class CM_TollBandit { public: static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CV_BanditTollScript&, CScriptDataBase*, const CScriptThing&) { return CCountedPointer<CEntityScriptBase>(); } };
    class CTollBandit { public: static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CV_BanditTollScript&, CScriptDataBase*, const CScriptThing&) { return CCountedPointer<CEntityScriptBase>(); } };
    class CM_TauntingBandit { public: static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CV_BanditTollScript&, CScriptDataBase*, const CScriptThing&) { return CCountedPointer<CEntityScriptBase>(); } };
    class CTauntingBandit { public: static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CV_BanditTollScript&, CScriptDataBase*, const CScriptThing&) { return CCountedPointer<CEntityScriptBase>(); } };
    class CBlockingBandit { public: static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CV_BanditTollScript&, CScriptDataBase*, const CScriptThing&) { return CCountedPointer<CEntityScriptBase>(); } };
    class CReluctantTrader { public: static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CV_BanditTollScript&, CScriptDataBase*, const CScriptThing&) { return CCountedPointer<CEntityScriptBase>(); } };
    class CSafeTrader { public: static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CV_BanditTollScript&, CScriptDataBase*, const CScriptThing&) { return CCountedPointer<CEntityScriptBase>(); } };
    class CM_BanditLeader { public: static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CV_BanditTollScript&, CScriptDataBase*, const CScriptThing&) { return CCountedPointer<CEntityScriptBase>(); } };
    class CBanditLeader { public: static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CV_BanditTollScript&, CScriptDataBase*, const CScriptThing&) { return CCountedPointer<CEntityScriptBase>(); } };
    class CGuildSealWarning { public: static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CV_BanditTollScript&, CScriptDataBase*, const CScriptThing&) { return CCountedPointer<CEntityScriptBase>(); } };
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
typedef NScript::CV_BanditTollScript Script;
typedef CEntityScriptBinding<Script> Binding;
static Script* expected_parent;
static Binding* saved[10];
static const char* expected_names[10]={
"M_TollBandit",
"TollBandit",
"M_TauntingBandit",
"TauntingBandit",
"BlockingBandit",
"ReluctantTrader",
"SafeTrader",
"M_BanditLeader",
"BanditLeader",
"GuildSealWarning"};
static Binding::TAllocFunc expected_factory[10]={
&Script::CM_TollBandit::Alloc,
&Script::CTollBandit::Alloc,
&Script::CM_TauntingBandit::Alloc,
&Script::CTauntingBandit::Alloc,
&Script::CBlockingBandit::Alloc,
&Script::CReluctantTrader::Alloc,
&Script::CSafeTrader::Alloc,
&Script::CM_BanditLeader::Alloc,
&Script::CBanditLeader::Alloc,
&Script::CGuildSealWarning::Alloc};
void CScriptBase::AddEntityScriptBinding(CEntityScriptBindingBase* p) {
    int i=registration_index++;check(i<10);if(i>=10)return;
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
static CSpawnedFunc<Script>* saved_threads[2];
void CScriptBase::AddSpawnedFunction(CSpawnedFuncBase* p,const CCharString& region) {
    int i=spawns++;check(i<2);if(i>=2)return;
    check(!strcmp(region.text,""));
    check((p==0)==((failure_mask&(1u<<(i+10)))!=0));
    CSpawnedFunc<Script>* f=static_cast<CSpawnedFunc<Script>*>(p);saved_threads[i]=f;
    if(f) {
        check(!strcmp(f->Name.text,i==0?"CheckForLeaderDead":"ResetToll"));
        check(f->ID==0&&f->PClass==expected_parent);
        check(f->Func==(i==0?&Script::CheckForLeaderDead:&Script::ResetToll));
    }
}
inline const C3DVector& CScriptThing::GetPos() const
{
    if (!PImp.Data) return NullCoord;
    return PImp.Data->GetPos();
}

void NScript::CV_BanditTollScript::Main()
{
    while (!Game.IsRegionLoaded("GreatwoodBanditToll")) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
    }
    if (IsActiveThreadTerminating()) return;
    AddEntityScriptBinding(new CEntityScriptBinding<CV_BanditTollScript>("M_TollBandit", this, &CM_TollBandit::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_BanditTollScript>("TollBandit", this, &CTollBandit::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_BanditTollScript>("M_TauntingBandit", this, &CM_TauntingBandit::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_BanditTollScript>("TauntingBandit", this, &CTauntingBandit::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_BanditTollScript>("BlockingBandit", this, &CBlockingBandit::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_BanditTollScript>("ReluctantTrader", this, &CReluctantTrader::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_BanditTollScript>("SafeTrader", this, &CSafeTrader::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_BanditTollScript>("M_BanditLeader", this, &CM_BanditLeader::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_BanditTollScript>("BanditLeader", this, &CBanditLeader::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_BanditTollScript>("GuildSealWarning", this, &CGuildSealWarning::Alloc, 0));
    PostAddScriptedEntities();
    Game.PostAddScriptedEntities();
    CScriptThing ReluctantTraderSpawn = Game.GetThingWithScriptName("M_ReluctantTraderSpawnPoint");
    if (!TraderDead) {
        if (IsActiveThreadTerminating()) return;
        Game.CreateCreature("CREATURE_TRADER_01", ReluctantTraderSpawn.GetPos(), "ReluctantTrader", false);
    }
    CScriptThing SafeTraderSpawn = Game.GetThingWithScriptName("M_SafeTraderSpawnPoint");
    if (!SafeTraderAppeared) {
        if (IsActiveThreadTerminating()) return;
        Game.CreateCreature("CREATURE_TRADER_01", SafeTraderSpawn.GetPos(), "SafeTrader", false);
    }
    AddSpawnedFunction(new CSpawnedFunc<CV_BanditTollScript>("CheckForLeaderDead", 0, this, &CV_BanditTollScript::CheckForLeaderDead), CCharString(""));
    AddSpawnedFunction(new CSpawnedFunc<CV_BanditTollScript>("ResetToll", 0, this, &CV_BanditTollScript::ResetToll), CCharString(""));
}

int main() {
    CGameScriptInterfaceBase game;
    Script script(game);expected_parent=&script;
    for(int delay=0;delay<3;++delay)for(int flags=0;flags<4;++flags)for(unsigned missing=0;missing<4;++missing) {
        script.TraderDead=(flags&1)!=0;script.SafeTraderAppeared=(flags&2)!=0;
        int first_guard=delay+1;
        int second_guard=first_guard+!script.TraderDead;
        int final_guard=second_guard+!script.SafeTraderAppeared;
        for(int ending=0;ending<=final_guard;++ending)for(int alloc_case=0;alloc_case<14;++alloc_case) {
            ready_after=delay;null_mask=missing;stop_at=ending;
            failure_mask=alloc_case==0?0:alloc_case==13?4095u:1u<<(alloc_case-1);
            frames=checks=lookups=creations=spawns=allocation_index=registration_index=retained=event_count=0;
            memset(saved,0,sizeof(saved));memset(saved_threads,0,sizeof(saved_threads));memset(created,0,sizeof(created));
            script.Main();
            int effective=ending==0?final_guard+1:ending;
            bool registered=effective>first_guard;
            bool reached_safe=effective>second_guard;
            check(frames==(ending!=0&&ending<=delay?ending:delay));
            check(checks==(ending==0?final_guard:ending));
            check(registration_index==(registered?10:0));
            check(event_count==(registered?2:0));
            if(registered)check(events[0]==1&&events[1]==2);
            check(lookups==(registered?1:0)+(reached_safe?1:0));
            check(created[0]==(!script.TraderDead&&reached_safe?1:0));
            check(created[1]==(!script.SafeTraderAppeared&&ending==0?1:0));
            check(creations==created[0]+created[1]);
            check(spawns==(ending==0?2:0));
            check(allocation_index==(registered?10:0)+(ending==0?2:0));
            check(thing_live==0);
            for(int i=0;i<10;++i)delete saved[i];
            delete saved_threads[0];delete saved_threads[1];
            check(string_live==0);
        }
    }
    if(errors) { fprintf(stderr,"checks failed: %d\n",errors);return 1; }
    puts("00e479b0_TEST PASS");return 0;
}
