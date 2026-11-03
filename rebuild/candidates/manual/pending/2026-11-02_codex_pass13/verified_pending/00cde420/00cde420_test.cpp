// 00cde420 behavior fixture. The exact production Main body is pasted below.
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
static int events[32], event_count;
static void check(bool value) { if(!value) ++errors; }
class CCharString {
public:
    const char* text;
    CCharString(const char* p, long n=-1):text(p) { check(n==-1);++string_live; }
    CCharString(const CCharString& other):text(other.text) { ++string_live; }
    ~CCharString() { --string_live; }
};
class CScriptThing {};
class CScriptDataBase {};
class CEntityScriptBase {};
template<class T> class CCountedPointer { public: T* Data; CCountedPointer(T* p=0):Data(p) {} };
class CEntityScriptBindingBase { public: virtual ~CEntityScriptBindingBase() {} };
class CGameScriptInterfaceBase {
public:
    virtual ~CGameScriptInterfaceBase() {}
    virtual void PostAddScriptedEntities() const { events[event_count++]=2; }
    virtual void SetCreatureGeneratorsEnabled(const CCharString& region,bool on) const {
        check(!strcmp(region.text,"LostBay"));check(!on);events[event_count++]=3;
    }
};
class CScriptBase {
public:
    void AddEntityScriptBinding(CEntityScriptBindingBase*);
    void PostAddScriptedEntities() { check(registration_index==17);events[event_count++]=1; }
};
namespace NScript {
class CPersonalScript_GlobalThingsScript:public CScriptBase {
public:
    CPersonalScript_GlobalThingsScript(CGameScriptInterfaceBase& game) : Game(game) {}
    CGameScriptInterfaceBase& Game;
    void Main();
    class CClickableDoor { public: static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CPersonalScript_GlobalThingsScript&, CScriptDataBase*, const CScriptThing&) { return CCountedPointer<CEntityScriptBase>(); } };
    class CTeleportPoint { public: static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CPersonalScript_GlobalThingsScript&, CScriptDataBase*, const CScriptThing&) { return CCountedPointer<CEntityScriptBase>(); } };
    class CHERO_GUILD_TELEPORT_MARKER { public: static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CPersonalScript_GlobalThingsScript&, CScriptDataBase*, const CScriptThing&) { return CCountedPointer<CEntityScriptBase>(); } };
    class CBowerstoneTeleportPointMarker { public: static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CPersonalScript_GlobalThingsScript&, CScriptDataBase*, const CScriptThing&) { return CCountedPointer<CEntityScriptBase>(); } };
    class COF_MainGates { public: static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CPersonalScript_GlobalThingsScript&, CScriptDataBase*, const CScriptThing&) { return CCountedPointer<CEntityScriptBase>(); } };
    class CKG_Gate { public: static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CPersonalScript_GlobalThingsScript&, CScriptDataBase*, const CScriptThing&) { return CCountedPointer<CEntityScriptBase>(); } };
    class CDragonGate { public: static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CPersonalScript_GlobalThingsScript&, CScriptDataBase*, const CScriptThing&) { return CCountedPointer<CEntityScriptBase>(); } };
    class CTestGateSwitch { public: static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CPersonalScript_GlobalThingsScript&, CScriptDataBase*, const CScriptThing&) { return CCountedPointer<CEntityScriptBase>(); } };
    class CInvisibleBarrier { public: static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CPersonalScript_GlobalThingsScript&, CScriptDataBase*, const CScriptThing&) { return CCountedPointer<CEntityScriptBase>(); } };
    class CBSGateGuard { public: static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CPersonalScript_GlobalThingsScript&, CScriptDataBase*, const CScriptThing&) { return CCountedPointer<CEntityScriptBase>(); } };
    class CTargetForKey { public: static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CPersonalScript_GlobalThingsScript&, CScriptDataBase*, const CScriptThing&) { return CCountedPointer<CEntityScriptBase>(); } };
    class CEndGameBarrier { public: static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CPersonalScript_GlobalThingsScript&, CScriptDataBase*, const CScriptThing&) { return CCountedPointer<CEntityScriptBase>(); } };
    class CLadyGreysBed { public: static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CPersonalScript_GlobalThingsScript&, CScriptDataBase*, const CScriptThing&) { return CCountedPointer<CEntityScriptBase>(); } };
    class CArcheryCompetitionSignpost { public: static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CPersonalScript_GlobalThingsScript&, CScriptDataBase*, const CScriptThing&) { return CCountedPointer<CEntityScriptBase>(); } };
    class CMK_HS2_SOUL { public: static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CPersonalScript_GlobalThingsScript&, CScriptDataBase*, const CScriptThing&) { return CCountedPointer<CEntityScriptBase>(); } };
    class CMK_HS3_SOUL { public: static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CPersonalScript_GlobalThingsScript&, CScriptDataBase*, const CScriptThing&) { return CCountedPointer<CEntityScriptBase>(); } };
    class CMK_HSE_SOUL { public: static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CPersonalScript_GlobalThingsScript&, CScriptDataBase*, const CScriptThing&) { return CCountedPointer<CEntityScriptBase>(); } };
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
typedef NScript::CPersonalScript_GlobalThingsScript Script;
typedef CEntityScriptBinding<Script> Binding;
static Script* expected_parent;
static Binding* saved[17];
static const char* expected_names[17]={
"ClickableDoor",
"TeleportPoint",
"HERO_GUILD_TELEPORT_MARKER",
"BowerstoneTeleportPointMarker",
"OF_MainGates",
"KG_Gate",
"DragonGate",
"TestGateSwitch",
"InvisibleBarrier",
"BSGateGuard",
"TargetForKey",
"EndGameBarrier",
"LadyGreysBed",
"ArcheryCompetitionSignpost",
"MK_HS2_SOUL",
"MK_HS3_SOUL",
"MK_HSE_SOUL"};
static Binding::TAllocFunc expected_factory[17]={
&Script::CClickableDoor::Alloc,
&Script::CTeleportPoint::Alloc,
&Script::CHERO_GUILD_TELEPORT_MARKER::Alloc,
&Script::CBowerstoneTeleportPointMarker::Alloc,
&Script::COF_MainGates::Alloc,
&Script::CKG_Gate::Alloc,
&Script::CDragonGate::Alloc,
&Script::CTestGateSwitch::Alloc,
&Script::CInvisibleBarrier::Alloc,
&Script::CBSGateGuard::Alloc,
&Script::CTargetForKey::Alloc,
&Script::CEndGameBarrier::Alloc,
&Script::CLadyGreysBed::Alloc,
&Script::CArcheryCompetitionSignpost::Alloc,
&Script::CMK_HS2_SOUL::Alloc,
&Script::CMK_HS3_SOUL::Alloc,
&Script::CMK_HSE_SOUL::Alloc};
void CScriptBase::AddEntityScriptBinding(CEntityScriptBindingBase* p) {
    int i=registration_index++;check(i<17);if(i>=17)return;
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

void NScript::CPersonalScript_GlobalThingsScript::Main()
{
    AddEntityScriptBinding(new CEntityScriptBinding<CPersonalScript_GlobalThingsScript>("ClickableDoor", this, &CClickableDoor::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CPersonalScript_GlobalThingsScript>("TeleportPoint", this, &CTeleportPoint::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CPersonalScript_GlobalThingsScript>("HERO_GUILD_TELEPORT_MARKER", this, &CHERO_GUILD_TELEPORT_MARKER::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CPersonalScript_GlobalThingsScript>("BowerstoneTeleportPointMarker", this, &CBowerstoneTeleportPointMarker::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CPersonalScript_GlobalThingsScript>("OF_MainGates", this, &COF_MainGates::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CPersonalScript_GlobalThingsScript>("KG_Gate", this, &CKG_Gate::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CPersonalScript_GlobalThingsScript>("DragonGate", this, &CDragonGate::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CPersonalScript_GlobalThingsScript>("TestGateSwitch", this, &CTestGateSwitch::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CPersonalScript_GlobalThingsScript>("InvisibleBarrier", this, &CInvisibleBarrier::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CPersonalScript_GlobalThingsScript>("BSGateGuard", this, &CBSGateGuard::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CPersonalScript_GlobalThingsScript>("TargetForKey", this, &CTargetForKey::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CPersonalScript_GlobalThingsScript>("EndGameBarrier", this, &CEndGameBarrier::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CPersonalScript_GlobalThingsScript>("LadyGreysBed", this, &CLadyGreysBed::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CPersonalScript_GlobalThingsScript>("ArcheryCompetitionSignpost", this, &CArcheryCompetitionSignpost::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CPersonalScript_GlobalThingsScript>("MK_HS2_SOUL", this, &CMK_HS2_SOUL::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CPersonalScript_GlobalThingsScript>("MK_HS3_SOUL", this, &CMK_HS3_SOUL::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CPersonalScript_GlobalThingsScript>("MK_HSE_SOUL", this, &CMK_HSE_SOUL::Alloc, 0));
    PostAddScriptedEntities();
    Game.PostAddScriptedEntities();
}

int main() {
    CGameScriptInterfaceBase game;
    Script script(game); expected_parent=&script;
    for(int scenario=0;scenario<19;++scenario) {
        failure_mask=scenario==0?0:scenario==18?131071u:1u<<(scenario-1);
        allocation_index=registration_index=retained=event_count=0;
        memset(saved,0,sizeof(saved));
        script.Main();
        check(allocation_index==17);check(registration_index==17);
        check(event_count==2);check(events[0]==1&&events[1]==2);
        check(string_live==retained);
        for(int i=0;i<17;++i)delete saved[i];
        check(string_live==0);
    }
    if(errors) { fprintf(stderr,"checks failed: %d\n",errors);return 1; }
    puts("00cde420_TEST PASS");return 0;
}
