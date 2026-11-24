#include <stdio.h>
#include <string.h>
#include <string>
#include <vector>

#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CBASICSTRING_COMPARE_BODY
#define FABLE_RETAIL_CBASICSTRING_TEXT_EQ_COMPARE_ZERO
#define FABLE_RETAIL_BEARDY_ORIGINAL_TEXT_COMPARISON_PASS27
#include "fable_string.h"

class BasicTextFixture : public CBasicString<char> {
public:
    explicit BasicTextFixture(char* value) { PData=value; }
};

namespace BeardyScenario {
struct CCharString {
    std::string text;
    CCharString(const char* x="") : text(x) {}
    bool operator==(const char* x) const { return text==x; }
};
struct TRetVoid { void Return() const {} };
struct CScriptThing {
    int id;
    bool alive;
    CScriptThing(int n,bool value) : id(n),alive(value) {}
    bool IsAlive() const {return alive;}
};
template<class T> struct CEntityScriptBinding {
    const char* name; T* owner; void (*factory)(); unsigned flags;
    CEntityScriptBinding(const char* n,T* s,void (*f)(),unsigned value) : name(n),owner(s),factory(f),flags(value) {}
};
template<class T> struct CSpawnedFunc {
    const char* name; T* owner; void (T::*func)(); unsigned id;
    CSpawnedFunc(const char* n,unsigned i,T* o,void (T::*f)()) : name(n),owner(o),func(f),id(i) {}
};
static const char* watchers[]={"WatchForQuestFinished","WatchForQuestCardConditions","WatchForBarberLadyDeath","WatchForBeardyBaldyDeath","WatchForNewHairdo","WatchForNewBeard","WatchForNewTash"};
static const char* region="BowerstoneSlumsWarehouses";
namespace NScript { struct CV_BeardyBaldyScript; }
struct CGame {
    std::vector<std::string> events;
    NScript::CV_BeardyBaldyScript* script;
    bool barber,beardy,loaded,valid;
    int frames,finishAt;
    CGame():script(0),barber(false),beardy(false),loaded(false),valid(true),frames(0),finishAt(6){}
    void PostAddScriptedEntities(){events.push_back("post game");}
    void NewScriptFrame();
    bool MsgOnRegionLoaded(CCharString& name) {
        if(frames==0){name=CCharString("Lookout");return true;}
        if(frames==2){name=CCharString(region);return true;}
        return false;
    }
    bool IsRegionLoaded(const char* name){if(strcmp(name,region))valid=false;return loaded;}
    CScriptThing GetThingWithScriptName(const char* s){if(strcmp(s,"BB_BeardyBaldyMan"))valid=false;return CScriptThing(1,beardy);}
    CScriptThing GetHero(){return CScriptThing(3,true);}
    CScriptThing GetNearestWithScriptName(CScriptThing hero,const char* s){if(hero.id!=3||strcmp(s,"CREATURE_BOWERSTONE_SLUMS_VILLAGER_FEMALE_BARBER"))valid=false;return CScriptThing(2,barber);}
    void MiniMapRemoveMarker(CScriptThing x){events.push_back(x.id==1?"marker man":"marker barber");}
    void ClearThingHasInformation(CScriptThing x){events.push_back(x.id==1?"clear man":"clear barber");}
    CCharString GetActiveQuestName(){return CCharString("V_BeardyBaldy");}
    void SetQuestAsCompleted(CCharString q,bool x,bool y,bool z){if(!(q=="V_BeardyBaldy")||x||y||z)valid=false;events.push_back("complete");}
    void RemoveQuestCardFromHero(CCharString q){if(!(q=="V_BeardyBaldy"))valid=false;events.push_back("remove card");}
    void DeactivateQuestLater(CCharString q,long n){if(!(q=="V_BeardyBaldy")||n)valid=false;events.push_back("deactivate");}
};
namespace NScript {
struct CV_BeardyBaldyScript {
    CGame* Game;
    bool QuestActivated,QuestComplete,QuestFinished;
    int checks,stopAt,spawned;
    CV_BeardyBaldyScript():Game(0),QuestActivated(false),QuestComplete(false),QuestFinished(false),checks(0),stopAt(0),spawned(0){}
    struct CBB_BeardyBaldyMan{static void Alloc(){}};
    void Main();
    bool IsActiveThreadTerminating(){return ++checks==stopAt;}
    void WatchForQuestFinished(){} void WatchForQuestCardConditions(){} void WatchForBarberLadyDeath(){}
    void WatchForBeardyBaldyDeath(){} void WatchForNewHairdo(){} void WatchForNewBeard(){} void WatchForNewTash(){}
    void AddEntityScriptBinding(CEntityScriptBinding<CV_BeardyBaldyScript>* b){
        if(strcmp(b->name,"BB_BeardyBaldyMan")||b->factory!=&CBB_BeardyBaldyMan::Alloc||b->owner!=this||b->flags)Game->valid=false;
        Game->events.push_back("bind"); delete b;
    }
    void PostAddScriptedEntities(){Game->events.push_back("post script");}
    void AddSpawnedFunction(CSpawnedFunc<CV_BeardyBaldyScript>* f,const CCharString& text){
        typedef void (CV_BeardyBaldyScript::*Callback)();
        const Callback expected[]={&CV_BeardyBaldyScript::WatchForQuestFinished,&CV_BeardyBaldyScript::WatchForQuestCardConditions,
            &CV_BeardyBaldyScript::WatchForBarberLadyDeath,&CV_BeardyBaldyScript::WatchForBeardyBaldyDeath,
            &CV_BeardyBaldyScript::WatchForNewHairdo,&CV_BeardyBaldyScript::WatchForNewBeard,&CV_BeardyBaldyScript::WatchForNewTash};
        if(spawned>=7||strcmp(f->name,watchers[spawned])||f->func!=expected[spawned]||f->id||f->owner!=this||!(text==""))Game->valid=false;
        ++spawned;Game->events.push_back(f->name);delete f;
    }
    void GoTalkToBeardyBaldy(){Game->events.push_back("talk");}
};
}
void CGame::NewScriptFrame(){++frames;if(frames==finishAt)script->QuestFinished=true;}
void NScript::CV_BeardyBaldyScript::Main()
{
    TRetVoid _Fail_Val;
    AddEntityScriptBinding(new CEntityScriptBinding<CV_BeardyBaldyScript>("BB_BeardyBaldyMan", this, &CBB_BeardyBaldyMan::Alloc, 0));
    PostAddScriptedEntities();
    Game->PostAddScriptedEntities();
    CCharString regionName;
    for (;; Game->NewScriptFrame()) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        while (!Game->MsgOnRegionLoaded(regionName)) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        if (regionName == "BowerstoneSlumsWarehouses") {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            break;
        }
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    AddSpawnedFunction(new CSpawnedFunc<CV_BeardyBaldyScript>("WatchForQuestFinished", 0, this, &CV_BeardyBaldyScript::WatchForQuestFinished), CCharString(""));
    AddSpawnedFunction(new CSpawnedFunc<CV_BeardyBaldyScript>("WatchForQuestCardConditions", 0, this, &CV_BeardyBaldyScript::WatchForQuestCardConditions), CCharString(""));
    AddSpawnedFunction(new CSpawnedFunc<CV_BeardyBaldyScript>("WatchForBarberLadyDeath", 0, this, &CV_BeardyBaldyScript::WatchForBarberLadyDeath), CCharString(""));
    AddSpawnedFunction(new CSpawnedFunc<CV_BeardyBaldyScript>("WatchForBeardyBaldyDeath", 0, this, &CV_BeardyBaldyScript::WatchForBeardyBaldyDeath), CCharString(""));
    AddSpawnedFunction(new CSpawnedFunc<CV_BeardyBaldyScript>("WatchForNewHairdo", 0, this, &CV_BeardyBaldyScript::WatchForNewHairdo), CCharString(""));
    AddSpawnedFunction(new CSpawnedFunc<CV_BeardyBaldyScript>("WatchForNewBeard", 0, this, &CV_BeardyBaldyScript::WatchForNewBeard), CCharString(""));
    AddSpawnedFunction(new CSpawnedFunc<CV_BeardyBaldyScript>("WatchForNewTash", 0, this, &CV_BeardyBaldyScript::WatchForNewTash), CCharString(""));
    if (!QuestActivated) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        QuestActivated = true;
    }
    GoTalkToBeardyBaldy();
    while (!QuestFinished && (!QuestComplete || Game->IsRegionLoaded("BowerstoneSlumsWarehouses"))) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    CScriptThing beardyBaldyMan = Game->GetThingWithScriptName("BB_BeardyBaldyMan");
    CScriptThing barberLady = Game->GetNearestWithScriptName(Game->GetHero(), "CREATURE_BOWERSTONE_SLUMS_VILLAGER_FEMALE_BARBER");
    if (barberLady.IsAlive()) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        Game->MiniMapRemoveMarker(barberLady);
        Game->ClearThingHasInformation(barberLady);
    }
    // Inline-budget: the second IsAlive is a call at 00E50030.
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
    if (beardyBaldyMan.IsAlive()) {
#ifdef _MSC_VER
#pragma inline_depth()
#endif
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        Game->MiniMapRemoveMarker(beardyBaldyMan);
        Game->ClearThingHasInformation(beardyBaldyMan);
    }
    Game->SetQuestAsCompleted(Game->GetActiveQuestName(), false, false, false);
    Game->RemoveQuestCardFromHero(Game->GetActiveQuestName());
    Game->DeactivateQuestLater(Game->GetActiveQuestName(), 0);
}

int exerciseScript(){
    for(int mask=0;mask<64;++mask){
        std::vector<std::string> completeEvents;
        int maxChecks=1;
        for(int stop=0;stop<=maxChecks;++stop){
            NScript::CV_BeardyBaldyScript s;CGame g;g.script=&s;s.Game=&g;s.stopAt=stop;
            s.QuestActivated=(mask&1)!=0;s.QuestComplete=(mask&2)!=0;s.QuestFinished=(mask&4)!=0;
            g.loaded=(mask&8)!=0;g.barber=(mask&16)!=0;g.beardy=(mask&32)!=0;
            s.Main();
            if(!g.valid)return 1;
            if(!stop){
                std::vector<std::string> expected;expected.push_back("bind");expected.push_back("post script");expected.push_back("post game");
                for(int i=0;i<7;++i)expected.push_back(watchers[i]);expected.push_back("talk");
                if(g.barber){expected.push_back("marker barber");expected.push_back("clear barber");}
                if(g.beardy){expected.push_back("marker man");expected.push_back("clear man");}
                expected.push_back("complete");expected.push_back("remove card");expected.push_back("deactivate");
                const int expectedFrames=((mask&4)||((mask&2)&&!(mask&8)))?2:6;
                if(g.events!=expected||!s.QuestActivated||g.frames!=expectedFrames)return 2;
                completeEvents=g.events;maxChecks=s.checks;
            }else{
                if(s.checks!=stop||g.events.size()>=completeEvents.size())return 3;
                for(unsigned k=0;k<g.events.size();++k)if(g.events[k]!=completeEvents[k])return 4;
            }
        }
    }

    return 0;
}

}
int main() {
    char empty[]="";
    char prefix[]="Bowerstone";
    char name[]="BowerstoneSlumsWarehouses";
    BasicTextFixture noText(empty), shortText(prefix), region(name);
    if (!(noText=="") || noText=="a" || !(shortText=="Bowerstone") || shortText==name ||
        !(region=="BowerstoneSlumsWarehouses") || region==prefix ||
        region=="BowerstoneSlumsWarehouseq" || region=="BowerstoneSlumsWarehouser")
        return 5;
    int status=BeardyScenario::exerciseScript();
    if (status) return status;
    puts("00e4fa20_TEST PASS");
    return 0;
}