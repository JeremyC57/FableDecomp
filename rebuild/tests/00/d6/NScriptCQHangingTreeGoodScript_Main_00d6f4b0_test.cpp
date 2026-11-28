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
    if (!value) { fprintf(stderr,"00d6f4b0_TEST FAIL %s\n",label);exit(1); }
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
struct CScriptDef { float HTGoodCompletionMorality; long HTGoodHeroKilledPrisonerGoldReward; long HTGoodHeroKilledPrisonerRenownReward; };
static const CScriptDef* pGlobalScriptDef;
struct Controls {
    explicit Controls(int outcomeFlags,bool killed,int jail,int outcome,int stop,int failAlloc):
        outcomeFlags(outcomeFlags),killed(killed),jailAt(jail),outcomeAt(outcome),stopAt(stop),failAt(failAlloc),
        polls(0),frames(0),news(0),bindings(0),threads(0),generators(0),quests(0),length(0) { trace[0]=0; }
    void event(char e) {
        check(length+1<sizeof(trace),"trace capacity");
        trace[length++]=e;trace[length]=0;
    }
    int outcomeFlags;
    bool killed;
    int jailAt,outcomeAt,stopAt,failAt,polls,frames,news,bindings,threads,generators,quests;
    size_t length;
    char trace[256];
};
static Controls* active;
static const char* const entityNames[]={
    "HTG_Prisoner", "HTG_Guard", "HTG_AmbushPoint", "HTG_AmbushPointRear", "HTG_AmbushPointCond",
    "HTG_AmbushPointBackup", "HTG_Executioner", "HTG_Audience", "HTG_Boss", "HTG_FreedomFighter"
};
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
namespace NScript {
class CQ_HangingTreeGoodScript;
class Services {
public:
    Services() : script(0) {}
    void bind(CQ_HangingTreeGoodScript& owner) { script=&owner; }
    CCharString GetActiveQuestName() const {
        ++active->quests;active->event('q');
        return CCharString(active->quests%2==0 ? "EVEN_QUEST" : "ODD_QUEST");
    }
    void validateQuest(const CCharString& name) const {
        check(strcmp(name.text,active->quests%2==0 ? "EVEN_QUEST" : "ODD_QUEST")==0,"current quest name forwarded");
    }
    void SetQuestCardGoldReward(const CCharString& quest,long gold) const {
        validateQuest(quest);
        check(gold==pGlobalScriptDef->HTGoodHeroKilledPrisonerGoldReward,"definition gold reward");
        active->event('G');
    }
    void SetQuestCardRenownReward(const CCharString& quest,long renown) const {
        validateQuest(quest);
        check(renown==pGlobalScriptDef->HTGoodHeroKilledPrisonerRenownReward,"definition renown reward");
        active->event('R');
    }
    bool IsLevelLoaded(const CCharString& area) const {
        check(strcmp(area.text,"BowerstoneJail")==0,"level wait");active->event('L');
        return active->frames>=active->jailAt;
    }
    void NewScriptFrame();
    void PostAddScriptedEntities() const { active->event('P'); }
    void SetQuestAsCompleted(const CCharString& quest,bool a,bool b,bool c) const {
        validateQuest(quest);check(a && b && !c,"quest completion flags");active->event('C');
    }
    void GiveHeroMorality(float value) const { check(value==pGlobalScriptDef->HTGoodCompletionMorality,"definition morality");active->event('m'); }
    void RemoveQuestCardFromGuild(const CCharString& other) const {
        check(strcmp(other.text,"Q_HangingTreeEvil")==0,"opposite quest card");active->event('r');
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
    CQ_HangingTreeGoodScript* script;
};
class CQ_HangingTreeGoodScript {
public:
    CQ_HangingTreeGoodScript():MissionSucceeded(false),MissionFailed(false),PartyDefeated(false),HeroKilledPrisoner(active->killed),ReasonForFailing("TEST_FAILURE_REASON") { Game.bind(*this); }
    void Main();
    void DoMission() {}
    void RemoveParty(bool value) { check(!value,"remove party argument");active->event('x'); }
    void RemoveDregs() { active->event('d'); }
    void CleanUp() { active->event('c'); }
    bool IsActiveThreadTerminating() { active->event('|');return ++active->polls==active->stopAt; }
    void PostAddScriptedEntities() { active->event('R'); }
    void AddEntityScriptBinding(CEntityScriptBinding<CQ_HangingTreeGoodScript>* binding) {
        int index=active->bindings++;
        check(index<10,"ten entity bindings");
        if (active->failAt==index+1) check(binding==0,"null binding forwarded");
        else {
            check(binding && binding->quest==this && binding->flags==1,"binding parent and flags");
            check(strcmp(binding->name.text,entityNames[index])==0 && binding->factory()==index,"named entity and matching allocator");
        }
        delete binding;active->event('b');
    }
    void AddSpawnedFunction(CSpawnedFunc<CQ_HangingTreeGoodScript>* thread,CCharString region) {
        int index=active->threads++;
        check(index==0 && region.text[0]==0,"thread count and region");
        if(active->failAt==11) check(thread==0,"null thread forwarded");
        else {
            check(thread && thread->quest==this && thread->id==0,"thread parent and id");
            check(strcmp(thread->name.text,"DoMission")==0 && thread->method==&CQ_HangingTreeGoodScript::DoMission,
                  "named thread and matching member");
        }
        delete thread;active->event('t');
    }
    struct CHTG_Prisoner { static int Alloc() {return 0;} };
    struct CHTG_Guard { static int Alloc() {return 1;} };
    struct CHTG_AmbushPoint { static int Alloc() {return 2;} };
    struct CHTG_AmbushPointRear { static int Alloc() {return 3;} };
    struct CHTG_AmbushPointCond { static int Alloc() {return 4;} };
    struct CHTG_AmbushPointBackup { static int Alloc() {return 5;} };
    struct CHTG_Executioner { static int Alloc() {return 6;} };
    struct CHTG_Audience { static int Alloc() {return 7;} };
    struct CHTG_Boss { static int Alloc() {return 8;} };
    struct CHTG_FreedomFighter { static int Alloc() {return 9;} };
    Services Game;
    bool MissionSucceeded,MissionFailed,PartyDefeated,HeroKilledPrisoner;
    CCharString ReasonForFailing;
    void setOutcome() {
        MissionSucceeded=(active->outcomeFlags&1)!=0;
        MissionFailed=(active->outcomeFlags&2)!=0;
        PartyDefeated=(active->outcomeFlags&4)!=0;
    }
};
void Services::NewScriptFrame() {
    active->event('N');
    ++active->frames;
    if (active->frames>=active->outcomeAt) script->setOutcome();
}
}

void NScript::CQ_HangingTreeGoodScript::Main()
{
    TRetVoid _Fail_Val;
    while (!Game.IsLevelLoaded("BowerstoneJail")) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating())
        return _Fail_Val.Return();
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_HangingTreeGoodScript>("HTG_Prisoner", this, &CHTG_Prisoner::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_HangingTreeGoodScript>("HTG_Guard", this, &CHTG_Guard::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_HangingTreeGoodScript>("HTG_AmbushPoint", this, &CHTG_AmbushPoint::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_HangingTreeGoodScript>("HTG_AmbushPointRear", this, &CHTG_AmbushPointRear::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_HangingTreeGoodScript>("HTG_AmbushPointCond", this, &CHTG_AmbushPointCond::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_HangingTreeGoodScript>("HTG_AmbushPointBackup", this, &CHTG_AmbushPointBackup::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_HangingTreeGoodScript>("HTG_Executioner", this, &CHTG_Executioner::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_HangingTreeGoodScript>("HTG_Audience", this, &CHTG_Audience::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_HangingTreeGoodScript>("HTG_Boss", this, &CHTG_Boss::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_HangingTreeGoodScript>("HTG_FreedomFighter", this, &CHTG_FreedomFighter::Alloc, 1));
    PostAddScriptedEntities();
    Game.PostAddScriptedEntities();
    AddSpawnedFunction(new CSpawnedFunc<CQ_HangingTreeGoodScript>("DoMission", 0, this, &CQ_HangingTreeGoodScript::DoMission), CCharString(""));
    while (!MissionSucceeded && !MissionFailed && !PartyDefeated) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating())
        return _Fail_Val.Return();
    if (MissionSucceeded) {
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
        Game.FadeScreenOut(2.0f, 0.5f, CRGBColour(0, 0, 0));
        if (HeroKilledPrisoner) {
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
            Game.SetQuestCardGoldReward(Game.GetActiveQuestName(), pGlobalScriptDef->HTGoodHeroKilledPrisonerGoldReward);
            Game.SetQuestCardRenownReward(Game.GetActiveQuestName(), pGlobalScriptDef->HTGoodHeroKilledPrisonerRenownReward);
            Game.NewScriptFrame();
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
        }
        Game.GiveHeroMorality(pGlobalScriptDef->HTGoodCompletionMorality);
        Game.SetQuestAsCompleted(Game.GetActiveQuestName(), true, true, false);
        Game.RemoveQuestCardFromGuild("Q_HangingTreeEvil");
        RemoveParty(false);
        CleanUp();
        Game.FadeScreenIn();
    } else {
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
        Game.FadeScreenOut(2.0f, 0.5f, CRGBColour(0, 0, 0));
        Game.SetQuestAsFailed(Game.GetActiveQuestName(), true, ReasonForFailing.ToWideString(), true);
        RemoveParty(false);
        RemoveDregs();
        Game.FadeScreenIn();
    }
    Game.SetCreatureGeneratorsEnabled("BowerstoneJail", true);
    Game.SetCreatureGeneratorsEnabled("GibbetWoods", true);
    Game.SetCreatureGeneratorsEnabled("DesertedFarm", true);
    Game.SetCreatureGeneratorsEnabled("ExecutionTree", true);
    Game.DeactivateQuestLater(Game.GetActiveQuestName(), 0);
}

static void exercise(int outcomeFlags,bool killed,int jail,int outcome,int stop,int failAlloc,const char* expected) {
    Controls controls(outcomeFlags,killed,jail,outcome,stop,failAlloc);
    CScriptDef def={82.375f+static_cast<float>(stop+failAlloc), 427+stop, 91+failAlloc};
    active=&controls;pGlobalScriptDef=&def;
    NScript::CQ_HangingTreeGoodScript script;
    if(outcome==0) script.setOutcome();
    script.Main();
    if(strcmp(expected,controls.trace)!=0) {
        fprintf(stderr,"expected %s\nreceived %s\n",expected,controls.trace);
        check(false,"ordered quest events");
    }
    active=0;pGlobalScriptDef=0;
}
static void testStops(int outcomeFlags,bool killed,const char* complete) {
    exercise(outcomeFlags,killed,2,4,0,0,complete);
    int stop=0;
    for(const char* p=complete;*p;++p) {
        if(*p!='|') continue;
        char prefix[256];size_t length=static_cast<size_t>(p-complete)+1;
        memcpy(prefix,complete,length);prefix[length]=0;
        exercise(outcomeFlags,killed,2,4,++stop,0,prefix);
    }
}
int main() {
    const char* rewarded="LN|LN|L|bbbbbbbbbbRPtN|N|||f|qGqRN|mqCrxciggggqD";
    const char* ordinary="LN|LN|L|bbbbbbbbbbRPtN|N|||fmqCrxciggggqD";
    const char* failed="LN|LN|L|bbbbbbbbbbRPtN|N|||fqFxdiggggqD";
    testStops(1,true,rewarded);
    testStops(1,false,ordinary);
    testStops(2,false,failed);
    testStops(4,true,failed);
    const char* immediate="L|bbbbbbbbbbRPt||fmqCrxciggggqD";
    exercise(7,false,0,0,0,0,immediate);
    exercise(6,true,0,0,0,0,"L|bbbbbbbbbbRPt||fqFxdiggggqD");
    for(int alloc=1;alloc<=11;++alloc)
        exercise(1,false,0,0,0,alloc,immediate);
    puts("00d6f4b0_TEST PASS");
    return 0;
}