// 00eded10 behavior fixture. The exact production Main body is pasted below.
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
    void PostAddScriptedEntities() { check(registration_index==13);events[event_count++]=1; }
};
namespace NScript {
class CV_TravellingHeroesScript:public CScriptBase {
public:
    CV_TravellingHeroesScript(CGameScriptInterfaceBase& game) : Game(game) {}
    CGameScriptInterfaceBase& Game;
    void Main();
    class CTH_ThunderBoy { public: static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CV_TravellingHeroesScript&, CScriptDataBase*, const CScriptThing&) { return CCountedPointer<CEntityScriptBase>(); } };
    class CTH_ThunderGirl { public: static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CV_TravellingHeroesScript&, CScriptDataBase*, const CScriptThing&) { return CCountedPointer<CEntityScriptBase>(); } };
    class CTH_Maze { public: static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CV_TravellingHeroesScript&, CScriptDataBase*, const CScriptThing&) { return CCountedPointer<CEntityScriptBase>(); } };
    class CTH_Thunder { public: static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CV_TravellingHeroesScript&, CScriptDataBase*, const CScriptThing&) { return CCountedPointer<CEntityScriptBase>(); } };
    class CTH_Whisper { public: static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CV_TravellingHeroesScript&, CScriptDataBase*, const CScriptThing&) { return CCountedPointer<CEntityScriptBase>(); } };
    class CTH_SnowTroll { public: static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CV_TravellingHeroesScript&, CScriptDataBase*, const CScriptThing&) { return CCountedPointer<CEntityScriptBase>(); } };
    class CTH_Trigger_Bowerstone { public: static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CV_TravellingHeroesScript&, CScriptDataBase*, const CScriptThing&) { return CCountedPointer<CEntityScriptBase>(); } };
    class CTH_Trigger_Guild_Maze { public: static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CV_TravellingHeroesScript&, CScriptDataBase*, const CScriptThing&) { return CCountedPointer<CEntityScriptBase>(); } };
    class CTH_Trigger_Guild_Rose { public: static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CV_TravellingHeroesScript&, CScriptDataBase*, const CScriptThing&) { return CCountedPointer<CEntityScriptBase>(); } };
    class CTH_Trigger_Oakvale { public: static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CV_TravellingHeroesScript&, CScriptDataBase*, const CScriptThing&) { return CCountedPointer<CEntityScriptBase>(); } };
    class CTH_Trigger_Oakvale_Gossip { public: static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CV_TravellingHeroesScript&, CScriptDataBase*, const CScriptThing&) { return CCountedPointer<CEntityScriptBase>(); } };
    class CTH_MagicBarrier { public: static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CV_TravellingHeroesScript&, CScriptDataBase*, const CScriptThing&) { return CCountedPointer<CEntityScriptBase>(); } };
    class CTH_Trigger_NorthernWastes { public: static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CV_TravellingHeroesScript&, CScriptDataBase*, const CScriptThing&) { return CCountedPointer<CEntityScriptBase>(); } };
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
typedef NScript::CV_TravellingHeroesScript Script;
typedef CEntityScriptBinding<Script> Binding;
static Script* expected_parent;
static Binding* saved[13];
static const char* expected_names[13]={
"TH_ThunderBoy",
"TH_ThunderGirl",
"TH_Maze",
"TH_Thunder",
"TH_Whisper",
"TH_SnowTroll",
"TH_Trigger_Bowerstone",
"TH_Trigger_Guild_Maze",
"TH_Trigger_Guild_Rose",
"TH_Trigger_Oakvale",
"TH_Trigger_Oakvale_Gossip",
"TH_MagicBarrier",
"TH_Trigger_NorthernWastes"};
static Binding::TAllocFunc expected_factory[13]={
&Script::CTH_ThunderBoy::Alloc,
&Script::CTH_ThunderGirl::Alloc,
&Script::CTH_Maze::Alloc,
&Script::CTH_Thunder::Alloc,
&Script::CTH_Whisper::Alloc,
&Script::CTH_SnowTroll::Alloc,
&Script::CTH_Trigger_Bowerstone::Alloc,
&Script::CTH_Trigger_Guild_Maze::Alloc,
&Script::CTH_Trigger_Guild_Rose::Alloc,
&Script::CTH_Trigger_Oakvale::Alloc,
&Script::CTH_Trigger_Oakvale_Gossip::Alloc,
&Script::CTH_MagicBarrier::Alloc,
&Script::CTH_Trigger_NorthernWastes::Alloc};
void CScriptBase::AddEntityScriptBinding(CEntityScriptBindingBase* p) {
    int i=registration_index++;check(i<13);if(i>=13)return;
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

void NScript::CV_TravellingHeroesScript::Main()
{
    AddEntityScriptBinding(new CEntityScriptBinding<CV_TravellingHeroesScript>("TH_ThunderBoy", this, &CTH_ThunderBoy::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_TravellingHeroesScript>("TH_ThunderGirl", this, &CTH_ThunderGirl::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_TravellingHeroesScript>("TH_Maze", this, &CTH_Maze::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_TravellingHeroesScript>("TH_Thunder", this, &CTH_Thunder::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_TravellingHeroesScript>("TH_Whisper", this, &CTH_Whisper::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_TravellingHeroesScript>("TH_SnowTroll", this, &CTH_SnowTroll::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_TravellingHeroesScript>("TH_Trigger_Bowerstone", this, &CTH_Trigger_Bowerstone::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_TravellingHeroesScript>("TH_Trigger_Guild_Maze", this, &CTH_Trigger_Guild_Maze::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_TravellingHeroesScript>("TH_Trigger_Guild_Rose", this, &CTH_Trigger_Guild_Rose::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_TravellingHeroesScript>("TH_Trigger_Oakvale", this, &CTH_Trigger_Oakvale::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_TravellingHeroesScript>("TH_Trigger_Oakvale_Gossip", this, &CTH_Trigger_Oakvale_Gossip::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_TravellingHeroesScript>("TH_MagicBarrier", this, &CTH_MagicBarrier::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_TravellingHeroesScript>("TH_Trigger_NorthernWastes", this, &CTH_Trigger_NorthernWastes::Alloc, 0));
    PostAddScriptedEntities();
    Game.PostAddScriptedEntities();
    Game.SetCreatureGeneratorsEnabled("LostBay", false);
}

int main() {
    CGameScriptInterfaceBase game;
    Script script(game); expected_parent=&script;
    for(int scenario=0;scenario<15;++scenario) {
        failure_mask=scenario==0?0:scenario==14?8191u:1u<<(scenario-1);
        allocation_index=registration_index=retained=event_count=0;
        memset(saved,0,sizeof(saved));
        script.Main();
        check(allocation_index==13);check(registration_index==13);
        check(event_count==3);check(events[0]==1&&events[1]==2&&events[2]==3);
        check(string_live==retained);
        for(int i=0;i<13;++i)delete saved[i];
        check(string_live==0);
    }
    if(errors) { fprintf(stderr,"checks failed: %d\n",errors);return 1; }
    puts("00eded10_TEST PASS");return 0;
}