// 00ec54e0 behavior fixture. The exact production Main body is pasted below.
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
static int stage, frames[3], targets[3], checks, stop_at, activations, completion, deactivations;
static void on_frame();
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
    virtual void NewScriptFrame() const { on_frame(); }
    virtual bool IsQuestCompleted(const CCharString& name) const {
        check(!strcmp(name.text,"QS_GuardianSisterInfo"));return frames[0]>=targets[0];
    }
    virtual void ActivateQuest(const CCharString& name) const {
        check(!strcmp(name.text,activations==0?"V_SickChild_Activate":"V_SickChildBarrowFields"));
        ++activations;stage=1;
    }
    virtual CCharString GetActiveQuestName() const { return CCharString("V_SickChild"); }
    virtual void SetQuestAsCompleted(const CCharString& name,bool a,bool b,bool c) const {
        check(!strcmp(name.text,"V_SickChild"));check(!a&&!b&&!c);++completion;stage=2;
    }
    virtual bool IsRegionLoaded(const CCharString& name) const {
        check(!strcmp(name.text,"BowerstoneSlums"));return frames[2]<targets[2];
    }
    virtual void DeactivateQuestLater(const CCharString& name,unsigned long delay) const {
        const char* expected=deactivations==0?"V_SickChildBarrowFields":deactivations==1?"V_SickChild_Activate":"V_SickChild";
        check(!strcmp(name.text,expected));check(delay==0);++deactivations;
    }
    virtual void PostAddScriptedEntities() const { events[event_count++]=2; }
    virtual void SetCreatureGeneratorsEnabled(const CCharString& region,bool on) const {
        check(!strcmp(region.text,"LostBay"));check(!on);events[event_count++]=3;
    }
};
class CScriptBase {
public:
    void AddEntityScriptBinding(CEntityScriptBindingBase*);
    bool IsActiveThreadTerminating() { ++checks;return stop_at!=0 && checks>=stop_at; }
    void PostAddScriptedEntities() { check(registration_index==11);events[event_count++]=1; }
};
namespace NScript {
class CV_SickChildScript:public CScriptBase {
public:
    CV_SickChildScript(CGameScriptInterfaceBase& game) : Game(game) {}
    CGameScriptInterfaceBase& Game;
    bool FinishedQuest;
    void Main();
    class CSickChildsMother { public: static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CV_SickChildScript&, CScriptDataBase*, const CScriptThing&) { return CCountedPointer<CEntityScriptBase>(); } };
    class CSickChild { public: static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CV_SickChildScript&, CScriptDataBase*, const CScriptThing&) { return CCountedPointer<CEntityScriptBase>(); } };
    class CSickChildsSister { public: static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CV_SickChildScript&, CScriptDataBase*, const CScriptThing&) { return CCountedPointer<CEntityScriptBase>(); } };
    class CWitch { public: static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CV_SickChildScript&, CScriptDataBase*, const CScriptThing&) { return CCountedPointer<CEntityScriptBase>(); } };
    class CManInLove { public: static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CV_SickChildScript&, CScriptDataBase*, const CScriptThing&) { return CCountedPointer<CEntityScriptBase>(); } };
    class CMansLover { public: static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CV_SickChildScript&, CScriptDataBase*, const CScriptThing&) { return CCountedPointer<CEntityScriptBase>(); } };
    class CIngredientOwner { public: static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CV_SickChildScript&, CScriptDataBase*, const CScriptThing&) { return CCountedPointer<CEntityScriptBase>(); } };
    class CTalkingTrader1 { public: static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CV_SickChildScript&, CScriptDataBase*, const CScriptThing&) { return CCountedPointer<CEntityScriptBase>(); } };
    class CTalkingTrader2 { public: static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CV_SickChildScript&, CScriptDataBase*, const CScriptThing&) { return CCountedPointer<CEntityScriptBase>(); } };
    class CWomanToAttract { public: static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CV_SickChildScript&, CScriptDataBase*, const CScriptThing&) { return CCountedPointer<CEntityScriptBase>(); } };
    class CSickChildFishingSpot { public: static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CV_SickChildScript&, CScriptDataBase*, const CScriptThing&) { return CCountedPointer<CEntityScriptBase>(); } };
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
typedef NScript::CV_SickChildScript Script;
typedef CEntityScriptBinding<Script> Binding;
static Script* expected_parent;
static Binding* saved[11];
static void on_frame() {
    check(stage>=0&&stage<3);++frames[stage];
    if(stage==1 && frames[1]>=targets[1])expected_parent->FinishedQuest=true;
}
static const char* expected_names[11]={
"SickChildsMother",
"SickChild",
"SickChildsSister",
"Witch",
"ManInLove",
"MansLover",
"IngredientOwner",
"TalkingTrader1",
"TalkingTrader2",
"WomanToAttract",
"SickChildFishingSpot"};
static Binding::TAllocFunc expected_factory[11]={
&Script::CSickChildsMother::Alloc,
&Script::CSickChild::Alloc,
&Script::CSickChildsSister::Alloc,
&Script::CWitch::Alloc,
&Script::CManInLove::Alloc,
&Script::CMansLover::Alloc,
&Script::CIngredientOwner::Alloc,
&Script::CTalkingTrader1::Alloc,
&Script::CTalkingTrader2::Alloc,
&Script::CWomanToAttract::Alloc,
&Script::CSickChildFishingSpot::Alloc};
void CScriptBase::AddEntityScriptBinding(CEntityScriptBindingBase* p) {
    int i=registration_index++;check(i<11);if(i>=11)return;
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

void NScript::CV_SickChildScript::Main()
{
    AddEntityScriptBinding(new CEntityScriptBinding<CV_SickChildScript>("SickChildsMother", this, &CSickChildsMother::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_SickChildScript>("SickChild", this, &CSickChild::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_SickChildScript>("SickChildsSister", this, &CSickChildsSister::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_SickChildScript>("Witch", this, &CWitch::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_SickChildScript>("ManInLove", this, &CManInLove::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_SickChildScript>("MansLover", this, &CMansLover::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_SickChildScript>("IngredientOwner", this, &CIngredientOwner::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_SickChildScript>("TalkingTrader1", this, &CTalkingTrader1::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_SickChildScript>("TalkingTrader2", this, &CTalkingTrader2::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_SickChildScript>("WomanToAttract", this, &CWomanToAttract::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_SickChildScript>("SickChildFishingSpot", this, &CSickChildFishingSpot::Alloc, 0));
    PostAddScriptedEntities();
    Game.PostAddScriptedEntities();
    while (!Game.IsQuestCompleted("QS_GuardianSisterInfo")) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
    }
    if (IsActiveThreadTerminating()) return;
    Game.ActivateQuest("V_SickChild_Activate");
    Game.ActivateQuest("V_SickChildBarrowFields");
    while (!FinishedQuest) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
    }
    if (IsActiveThreadTerminating()) return;
    Game.SetQuestAsCompleted(Game.GetActiveQuestName(), false, false, false);
    while (Game.IsRegionLoaded("BowerstoneSlums") != false) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
    }
    if (IsActiveThreadTerminating()) return;
    Game.DeactivateQuestLater("V_SickChildBarrowFields", 0);
    Game.DeactivateQuestLater("V_SickChild_Activate", 0);
    Game.DeactivateQuestLater(Game.GetActiveQuestName(), 0);
}

int main() {
    CGameScriptInterfaceBase game;
    Script script(game);expected_parent=&script;
    // Independent query-count oracle: each WAIT adds its frame checks and one final guard.
    for(int q=0;q<3;++q)for(int f=0;f<3;++f)for(int r=0;r<3;++r) {
        targets[0]=q;targets[1]=f;targets[2]=r;
        int total_checks=q+f+r+3;
        for(int ending=0;ending<=total_checks;++ending)for(int alloc_case=0;alloc_case<13;++alloc_case) {
            stop_at=ending;
            failure_mask=alloc_case==0?0:alloc_case==12?2047u:1u<<(alloc_case-1);
            allocation_index=registration_index=retained=event_count=0;
            stage=checks=activations=completion=deactivations=0;
            memset(frames,0,sizeof(frames));memset(saved,0,sizeof(saved));
            script.FinishedQuest=f==0;
            script.Main();
            check(allocation_index==11);check(registration_index==11);
            check(event_count==2);check(events[0]==1&&events[1]==2);
            int effective=ending==0?total_checks+1:ending;
            check(checks==(ending==0?total_checks:ending));
            check(activations==(effective>q+1?2:0));
            check(completion==(effective>q+f+2?1:0));
            check(deactivations==(ending==0?3:0));
            int budget=effective;
            for(int phase=0;phase<3;++phase) {
                int expected=budget<=0?0:budget<targets[phase]?budget:targets[phase];
                check(frames[phase]==expected);budget-=targets[phase]+1;
            }
            check(string_live==retained);
            for(int i=0;i<11;++i)delete saved[i];
            check(string_live==0);
        }
    }
    if(errors) { fprintf(stderr,"checks failed: %d\n",errors);return 1; }
    puts("00ec54e0_TEST PASS");return 0;
}