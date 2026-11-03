// 00e6e380 behavior fixture. The exact production Main body is pasted below.
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
    void PostAddScriptedEntities() { check(registration_index==14);events[event_count++]=1; }
};
namespace NScript {
class CV_DemonDoorsScript:public CScriptBase {
public:
    CV_DemonDoorsScript(CGameScriptInterfaceBase& game) : Game(game) {}
    CGameScriptInterfaceBase& Game;
    void Main();
    class CDF_ShineLight { public: static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CV_DemonDoorsScript&, CScriptDataBase*, const CScriptThing&) { return CCountedPointer<CEntityScriptBase>(); } };
    class CDF_ThunderHelmet { public: static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CV_DemonDoorsScript&, CScriptDataBase*, const CScriptThing&) { return CCountedPointer<CEntityScriptBase>(); } };
    class CDF_CombatMult { public: static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CV_DemonDoorsScript&, CScriptDataBase*, const CScriptThing&) { return CCountedPointer<CEntityScriptBase>(); } };
    class CDF_Dragon { public: static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CV_DemonDoorsScript&, CScriptDataBase*, const CScriptThing&) { return CCountedPointer<CEntityScriptBase>(); } };
    class CDF_SummonsCreatures { public: static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CV_DemonDoorsScript&, CScriptDataBase*, const CScriptThing&) { return CCountedPointer<CEntityScriptBase>(); } };
    class CDF_BeEvil { public: static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CV_DemonDoorsScript&, CScriptDataBase*, const CScriptThing&) { return CCountedPointer<CEntityScriptBase>(); } };
    class CDF_DoDamage { public: static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CV_DemonDoorsScript&, CScriptDataBase*, const CScriptThing&) { return CCountedPointer<CEntityScriptBase>(); } };
    class CDF_EatPies { public: static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CV_DemonDoorsScript&, CScriptDataBase*, const CScriptThing&) { return CCountedPointer<CEntityScriptBase>(); } };
    class CDF_GiveGift { public: static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CV_DemonDoorsScript&, CScriptDataBase*, const CScriptThing&) { return CCountedPointer<CEntityScriptBase>(); } };
    class CDF_DressAsBandit { public: static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CV_DemonDoorsScript&, CScriptDataBase*, const CScriptThing&) { return CCountedPointer<CEntityScriptBase>(); } };
    class CDF_ThreeSuits { public: static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CV_DemonDoorsScript&, CScriptDataBase*, const CScriptThing&) { return CCountedPointer<CEntityScriptBase>(); } };
    class CDF_MarryLadyGrey { public: static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CV_DemonDoorsScript&, CScriptDataBase*, const CScriptThing&) { return CCountedPointer<CEntityScriptBase>(); } };
    class CDF_SexMad { public: static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CV_DemonDoorsScript&, CScriptDataBase*, const CScriptThing&) { return CCountedPointer<CEntityScriptBase>(); } };
    class CDF_KeyHoarder { public: static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CV_DemonDoorsScript&, CScriptDataBase*, const CScriptThing&) { return CCountedPointer<CEntityScriptBase>(); } };
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
typedef NScript::CV_DemonDoorsScript Script;
typedef CEntityScriptBinding<Script> Binding;
static Script* expected_parent;
static Binding* saved[14];
static const char* expected_names[14]={
"DF_ShineLight",
"DF_ThunderHelmet",
"DF_CombatMult",
"DF_Dragon",
"DF_SummonsCreatures",
"DF_BeEvil",
"DF_DoDamage",
"DF_EatPies",
"DF_GiveGift",
"DF_DressAsBandit",
"DF_ThreeSuits",
"DF_MarryLadyGrey",
"DF_SexMad",
"DF_KeyHoarder"};
static Binding::TAllocFunc expected_factory[14]={
&Script::CDF_ShineLight::Alloc,
&Script::CDF_ThunderHelmet::Alloc,
&Script::CDF_CombatMult::Alloc,
&Script::CDF_Dragon::Alloc,
&Script::CDF_SummonsCreatures::Alloc,
&Script::CDF_BeEvil::Alloc,
&Script::CDF_DoDamage::Alloc,
&Script::CDF_EatPies::Alloc,
&Script::CDF_GiveGift::Alloc,
&Script::CDF_DressAsBandit::Alloc,
&Script::CDF_ThreeSuits::Alloc,
&Script::CDF_MarryLadyGrey::Alloc,
&Script::CDF_SexMad::Alloc,
&Script::CDF_KeyHoarder::Alloc};
void CScriptBase::AddEntityScriptBinding(CEntityScriptBindingBase* p) {
    int i=registration_index++;check(i<14);if(i>=14)return;
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

void NScript::CV_DemonDoorsScript::Main()
{
    AddEntityScriptBinding(new CEntityScriptBinding<CV_DemonDoorsScript>("DF_ShineLight", this, &CDF_ShineLight::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_DemonDoorsScript>("DF_ThunderHelmet", this, &CDF_ThunderHelmet::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_DemonDoorsScript>("DF_CombatMult", this, &CDF_CombatMult::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_DemonDoorsScript>("DF_Dragon", this, &CDF_Dragon::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_DemonDoorsScript>("DF_SummonsCreatures", this, &CDF_SummonsCreatures::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_DemonDoorsScript>("DF_BeEvil", this, &CDF_BeEvil::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_DemonDoorsScript>("DF_DoDamage", this, &CDF_DoDamage::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_DemonDoorsScript>("DF_EatPies", this, &CDF_EatPies::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_DemonDoorsScript>("DF_GiveGift", this, &CDF_GiveGift::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_DemonDoorsScript>("DF_DressAsBandit", this, &CDF_DressAsBandit::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_DemonDoorsScript>("DF_ThreeSuits", this, &CDF_ThreeSuits::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_DemonDoorsScript>("DF_MarryLadyGrey", this, &CDF_MarryLadyGrey::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_DemonDoorsScript>("DF_SexMad", this, &CDF_SexMad::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_DemonDoorsScript>("DF_KeyHoarder", this, &CDF_KeyHoarder::Alloc, 0));
    PostAddScriptedEntities();
    Game.PostAddScriptedEntities();
}

int main() {
    CGameScriptInterfaceBase game;
    Script script(game); expected_parent=&script;
    for(int scenario=0;scenario<16;++scenario) {
        failure_mask=scenario==0?0:scenario==15?16383u:1u<<(scenario-1);
        allocation_index=registration_index=retained=event_count=0;
        memset(saved,0,sizeof(saved));
        script.Main();
        check(allocation_index==14);check(registration_index==14);
        check(event_count==2);check(events[0]==1&&events[1]==2);
        check(string_live==retained);
        for(int i=0;i<14;++i)delete saved[i];
        check(string_live==0);
    }
    if(errors) { fprintf(stderr,"checks failed: %d\n",errors);return 1; }
    puts("00e6e380_TEST PASS");return 0;
}
