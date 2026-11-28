// Execute the production Main body against script services that record the order and arguments.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <new>
#ifndef _MSC_VER
#define FABLE_RETAIL_PASS23_HOST_TEST_ABI
#endif
#define FABLE_RETAIL_CRGBCOLOUR_ORIGINAL_COMPLETE_DECLARATIONS_PASS22
#include "engine/TRetVoid.h"
#include "engine/CRGBColour.h"

static void check(bool value,const char* label) {
    if (!value) { fprintf(stderr,"00d69480_TEST FAIL %s\n",label);exit(1); }
}
class CWideString {
public:
    explicit CWideString(const char* text): text(text) {}
    const char* text;
};
class CCharString {
public:
    CCharString(const char* text=""): text(text) {}
    CWideString ToWideString() const { return CWideString(text); }
    const char* text;
};
struct CScriptDef { float HTEvilCompletionMorality; };
static const CScriptDef* pGlobalScriptDef;
struct Controls {
    explicit Controls(bool success,bool both,int jail,int outcome,int stop,int failAlloc):
        success(success),both(both),jailAt(jail),outcomeAt(outcome),stopAt(stop),failAt(failAlloc),
        polls(0),frames(0),news(0),bindings(0),threads(0),generators(0),quests(0),length(0) { trace[0]=0; }
    void event(char e) {
        check(length+1<sizeof(trace),"trace capacity");
        trace[length++]=e;trace[length]=0;
    }
    bool success,both;
    int jailAt,outcomeAt,stopAt,failAt,polls,frames,news,bindings,threads,generators,quests;
    size_t length;
    char trace[256];
};
static Controls* active;
static const char* const entityNames[]={"HTE_AmbushPoint","HTE_AssaultGuard","HTE_FreedomFighter","HTE_Audience",
    "HTE_Prisoner","HTE_Executioner","GUARD1","GUARD2"};
static const char* const generatorNames[]={"BowerstoneJail","GibbetWoods","DesertedFarm","ExecutionTree"};

class Allocation {
public:
    static void* operator new(size_t bytes) throw() {
        ++active->news;
        return active->news==active->failAt ? 0 : malloc(bytes);
    }
    static void operator delete(void* data) { free(data); }
};
template <class Quest> class CEntityScriptBinding : public Allocation {
public:
    typedef int (*Factory)();
    CEntityScriptBinding(CCharString name,Quest* quest,Factory factory,unsigned long flags):
        name(name),quest(quest),factory(factory),flags(flags) {}
    CCharString name;
    Quest* quest;
    Factory factory;
    unsigned long flags;
};
template <class Quest> class CSpawnedFunc : public Allocation {
public:
    typedef void (Quest::*Method)();
    CSpawnedFunc(CCharString name,unsigned long id,Quest* quest,Method method):
        name(name),id(id),quest(quest),method(method) {}
    CCharString name;
    unsigned long id;
    Quest* quest;
    Method method;
};
class CTimer {
public:
    CTimer& operator=(long time) { check(time==1000,"mission timer");active->event('T');return *this; }
};
namespace NScript {
class CQ_HangingTreeEvilScript;
class Services {
public:
    Services() : script(0) {}
    void bind(CQ_HangingTreeEvilScript& owner) { script=&owner; }
    CCharString GetActiveQuestName() const {
        ++active->quests;active->event('q');
        return CCharString(active->quests%2==0 ? "EVEN_QUEST" : "ODD_QUEST");
    }
    void validateQuest(const CCharString& name) const {
        check(strcmp(name.text,active->quests%2==0 ? "EVEN_QUEST" : "ODD_QUEST")==0,"current quest name forwarded");
    }
    void SetQuestCardObjective(const CCharString& quest,const CCharString& objective,const CCharString& target,const CCharString& area) const {
        validateQuest(quest);
        check(strcmp(objective.text,"TEXT_QUEST_HANGING_TREE_EVIL_OBJECTIVE_01")==0 && strcmp(target.text,"ExecutionTree")==0 &&
              strcmp(area.text,"BowerstoneSlums")==0,"quest objective arguments");
        active->event('O');
    }
    bool IsLevelLoaded(const CCharString& area) const {
        check(strcmp(area.text,"BowerstoneJail")==0,"level wait");active->event('L');
        return active->frames>=active->jailAt;
    }
    void NewScriptFrame();
    void PostAddScriptedEntities() const { active->event('P'); }
    void Pause(float sec) const { check(sec==(active->success?1.0f:2.0f),"pause length");active->event('p'); }
    void SetQuestAsCompleted(const CCharString& quest,bool a,bool b,bool c) const {
        validateQuest(quest);check(a && b && !c,"quest completion flags");active->event('C');
    }
    void GiveHeroMorality(float value) const { check(value==pGlobalScriptDef->HTEvilCompletionMorality,"definition morality");active->event('m'); }
    void RemoveQuestCardFromGuild(const CCharString& other) const {
        check(strcmp(other.text,"Q_HangingTreeGood")==0,"opposite quest card");active->event('r');
    }
    bool FadeScreenOut(float duration,float offset,CRGBColour colour) {
        check(duration==2.0f && offset==0.5f && colour.R==0 && colour.G==0 && colour.B==0 && colour.A==255,"fade parameters");
        active->event('f');return true;
    }
    void SetQuestAsFailed(const CCharString& quest,bool a,const CWideString& reason,bool b) const {
        validateQuest(quest);check(a && b && strcmp(reason.text,"TEST_FAILURE_REASON")==0,"reason and failure flags");active->event('F');
    }
    void FadeScreenIn() { active->event('i'); }
    void SetCreatureGeneratorsEnabled(const CCharString& region,bool enable) const {
        check(active->generators<4 && strcmp(region.text,generatorNames[active->generators])==0 && enable,"generator region and state");
        ++active->generators;active->event('g');
    }
    void DeactivateQuestLater(const CCharString& quest,unsigned long delay) const { validateQuest(quest);check(delay==0,"deactivation delay");active->event('D'); }
private:
    CQ_HangingTreeEvilScript* script;
};
class CQ_HangingTreeEvilScript {
public:
    CQ_HangingTreeEvilScript():MissionSucceeded(false),MissionFailed(false),ReasonForFailing("TEST_FAILURE_REASON") { Game.bind(*this); }
    void Main();
    void DoMission() {}
    void BanditYells() {}
    void CleanUp() { active->event('c'); }
    bool IsActiveThreadTerminating() { active->event('|');return ++active->polls==active->stopAt; }
    void PostAddScriptedEntities() { active->event('R'); }
    void AddEntityScriptBinding(CEntityScriptBinding<CQ_HangingTreeEvilScript>* binding) {
        int index=active->bindings++;
        check(index<8,"eight entity bindings");
        if (active->failAt==index+1) check(binding==0,"null binding forwarded");
        else {
            check(binding && binding->quest==this && binding->flags==1,"binding parent and flags");
            check(strcmp(binding->name.text,entityNames[index])==0 && binding->factory()==index,"named entity and matching allocator");
        }
        delete binding;active->event('b');
    }
    void AddSpawnedFunction(CSpawnedFunc<CQ_HangingTreeEvilScript>* thread,CCharString region) {
        int index=active->threads++;
        check(index<2 && region.text[0]==0,"thread count and region");
        if(active->failAt==9+index) check(thread==0,"null thread forwarded");
        else {
            check(thread && thread->quest==this && thread->id==0,"thread parent and id");
            check(index==0 ? (strcmp(thread->name.text,"DoMission")==0 && thread->method==&CQ_HangingTreeEvilScript::DoMission)
                           : (strcmp(thread->name.text,"BanditYells")==0 && thread->method==&CQ_HangingTreeEvilScript::BanditYells),
                  "named thread and matching member");
        }
        delete thread;active->event('t');
    }
    struct CHTE_AmbushPoint { static int Alloc() {return 0;} };
    struct CHTE_AssaultGuard { static int Alloc() {return 1;} };
    struct CHTE_FreedomFighter { static int Alloc() {return 2;} };
    struct CHTE_Audience { static int Alloc() {return 3;} };
    struct CHTE_Prisoner { static int Alloc() {return 4;} };
    struct CHTE_Executioner { static int Alloc() {return 5;} };
    struct CGUARD1 { static int Alloc() {return 6;} };
    struct CGUARD2 { static int Alloc() {return 7;} };
    Services Game;
    bool MissionSucceeded,MissionFailed;
    CCharString ReasonForFailing;
    CTimer MissionTimer;
    void setOutcome() {
        MissionSucceeded=active->success;
        MissionFailed=!active->success || active->both;
    }
};
void Services::NewScriptFrame() {
    active->event('N');
    ++active->frames;
    if (active->frames>=active->outcomeAt) script->setOutcome();
}
}

void NScript::CQ_HangingTreeEvilScript::Main()
{
    TRetVoid _Fail_Val;
    Game.SetQuestCardObjective(Game.GetActiveQuestName(), "TEXT_QUEST_HANGING_TREE_EVIL_OBJECTIVE_01", "ExecutionTree", "BowerstoneSlums");
    while (!Game.IsLevelLoaded("BowerstoneJail")) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating())
        return _Fail_Val.Return();
    MissionTimer = 1000;
    Game.SetQuestCardObjective(Game.GetActiveQuestName(), "TEXT_QUEST_HANGING_TREE_EVIL_OBJECTIVE_01", "ExecutionTree", "BowerstoneSlums");
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_HangingTreeEvilScript>("HTE_AmbushPoint", this, &CHTE_AmbushPoint::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_HangingTreeEvilScript>("HTE_AssaultGuard", this, &CHTE_AssaultGuard::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_HangingTreeEvilScript>("HTE_FreedomFighter", this, &CHTE_FreedomFighter::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_HangingTreeEvilScript>("HTE_Audience", this, &CHTE_Audience::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_HangingTreeEvilScript>("HTE_Prisoner", this, &CHTE_Prisoner::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_HangingTreeEvilScript>("HTE_Executioner", this, &CHTE_Executioner::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_HangingTreeEvilScript>("GUARD1", this, &CGUARD1::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_HangingTreeEvilScript>("GUARD2", this, &CGUARD2::Alloc, 1));
    PostAddScriptedEntities();
    Game.PostAddScriptedEntities();
    AddSpawnedFunction(new CSpawnedFunc<CQ_HangingTreeEvilScript>("DoMission", 0, this, &CQ_HangingTreeEvilScript::DoMission), CCharString(""));
    AddSpawnedFunction(new CSpawnedFunc<CQ_HangingTreeEvilScript>("BanditYells", 0, this, &CQ_HangingTreeEvilScript::BanditYells), CCharString(""));
    while (!MissionSucceeded && !MissionFailed) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating())
        return _Fail_Val.Return();
    if (MissionSucceeded) {
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
        CleanUp();
        Game.Pause(1.0f);
        Game.SetQuestAsCompleted(Game.GetActiveQuestName(), true, true, false);
        Game.GiveHeroMorality(pGlobalScriptDef->HTEvilCompletionMorality);
        Game.RemoveQuestCardFromGuild("Q_HangingTreeGood");
        Game.FadeScreenIn();
    } else {
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
        Game.FadeScreenOut(2.0f, 0.5f, CRGBColour(0, 0, 0));
        CleanUp();
        Game.Pause(2.0f);
        Game.SetQuestAsFailed(Game.GetActiveQuestName(), true, ReasonForFailing.ToWideString(), true);
        Game.FadeScreenIn();
    }
    Game.SetCreatureGeneratorsEnabled("BowerstoneJail", true);
    Game.SetCreatureGeneratorsEnabled("GibbetWoods", true);
    Game.SetCreatureGeneratorsEnabled("DesertedFarm", true);
    Game.SetCreatureGeneratorsEnabled("ExecutionTree", true);
    Game.DeactivateQuestLater(Game.GetActiveQuestName(), 0);
}

static void exercise(bool success,bool both,int jail,int outcome,int stop,int failAlloc,const char* expected) {
    Controls controls(success,both,jail,outcome,stop,failAlloc);
    CScriptDef def={ -(71.375f+static_cast<float>(stop+failAlloc)) };
    active=&controls;pGlobalScriptDef=&def;
    NScript::CQ_HangingTreeEvilScript script;
    if(outcome==0) script.setOutcome();
    script.Main();
    if(strcmp(expected,controls.trace)!=0) {
        fprintf(stderr,"expected %s\nreceived %s\n",expected,controls.trace);
        check(false,"ordered quest events");
    }
    active=0;pGlobalScriptDef=0;
}
static void testStops(bool success,const char* complete) {
    exercise(success,false,2,4,0,0,complete);
    int stop=0;
    for(const char* p=complete;*p;++p) {
        if(*p!='|') continue;
        char prefix[256];size_t length=static_cast<size_t>(p-complete)+1;
        memcpy(prefix,complete,length);prefix[length]=0;
        exercise(success,false,2,4,++stop,0,prefix);
    }
}
int main() {
    const char* success="qOLN|LN|L|TqObbbbbbbbRPttN|N|||cpqCmriggggqD";
    const char* failure="qOLN|LN|L|TqObbbbbbbbRPttN|N|||fcpqFiggggqD";
    testStops(true,success);
    testStops(false,failure);
    const char* immediate="qOL|TqObbbbbbbbRPtt||cpqCmriggggqD";
    exercise(true,true,0,0,0,0,immediate);
    exercise(false,false,0,0,0,0,"qOL|TqObbbbbbbbRPtt||fcpqFiggggqD");
    for(int alloc=1;alloc<=10;++alloc)
        exercise(true,false,0,0,0,alloc,immediate);
    puts("00d69480_TEST PASS");
    return 0;
}