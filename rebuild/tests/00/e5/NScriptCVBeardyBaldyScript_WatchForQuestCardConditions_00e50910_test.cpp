
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
class TRetVoid { public: void Return() {} };
static void ck(bool a){if(!a)exit(1);}
struct CCharString {
    const char* text; CCharString(const char* s="") : text(s) {}
    bool operator==(const char* s) const {return strcmp(text,s)==0;}
};
struct Scenario {
    int stop,checks,frames,card,objectives,hair,regions,alive,phase3,quest,barber,beardy,talks,ticks,delay;
};
static Scenario state;
struct CScriptThing {
    bool IsAlive()const{return state.alive!=0;}
    bool MsgIsTalkedToBy(const char* s)const{ck(!strcmp(s,"SCRIPT_NAME_HERO"));return ++state.talks>state.delay;}
};
struct ScriptGame {
    void NewScriptFrame(){++state.frames;ck(state.frames<100);}
    const char* GetActiveQuestName()const{++state.quest;return "quest";}
    void GiveHeroQuestCardDirectly(const char* c,const char* q,bool show){
        ck(!strcmp(c,"OBJECT_QUEST_CARD_BEARDY_BALDY")&&!strcmp(q,"quest")&&show);++state.card;
    }
    void SetQuestCardObjective(const char* q,const char* o,const char* a,const char* b){
        ck(!strcmp(q,"quest") && !*a && !*b && state.card==1);
        const char* expected=(state.objectives==1)?"TEXT_QUEST_BEARDY_BALDY_OBJECTIVE_03":"TEXT_QUEST_BEARDY_BALDY_OBJECTIVE_02";
        ck(!strcmp(o,expected));++state.objectives;
    }
    bool MsgOnRegionLoaded(CCharString& region){
        ++state.regions; if(state.regions%2==0){region=state.regions<4?"another":"BowerstoneSlumsWarehouses";return true;}return false;
    }
    CScriptThing GetThingWithScriptName(const char* n){ck(!strcmp(n,"BB_BeardyBaldyMan"));return CScriptThing();}
};
namespace NScript {
class CV_BeardyBaldyScript {
public:
    ScriptGame Game;
    bool Phase1RequirementsComplete,Phase3RequirementsComplete,AcquiredNewHairdo,AcquiredNewBeard,AcquiredNewTash;
    CV_BeardyBaldyScript():Phase1RequirementsComplete(false),Phase3RequirementsComplete(state.phase3!=0),
        AcquiredNewHairdo(false),AcquiredNewBeard(false),AcquiredNewTash(false){}
    void ResetHairWatch();
    void WatchForQuestCardConditions();
    void GoTalkToBarber(){++state.barber;}
    void GoTalkToBeardyBaldy(){++state.beardy;}
    bool IsActiveThreadTerminating() {
        ++state.checks;
        if(state.frames>=state.delay)Phase1RequirementsComplete=true;
        if(state.card && state.objectives==1 && ++state.ticks>state.delay){
            AcquiredNewHairdo=state.hair==0;AcquiredNewBeard=state.hair==1;AcquiredNewTash=state.hair==2;
        }
        return state.checks==state.stop || state.barber==2;
    }
};
}
inline void NScript::CV_BeardyBaldyScript::ResetHairWatch()
{
    AcquiredNewHairdo = false;
    AcquiredNewBeard = false;
    AcquiredNewTash = false;
}
void NScript::CV_BeardyBaldyScript::WatchForQuestCardConditions()
{
    TRetVoid _Fail_Val;
    CCharString defName;
    CCharString regionName;
    CScriptThing beardyBaldyMan;
    while (!Phase1RequirementsComplete) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating())
        return _Fail_Val.Return();
    Game.GiveHeroQuestCardDirectly("OBJECT_QUEST_CARD_BEARDY_BALDY", Game.GetActiveQuestName(), true);
    Game.SetQuestCardObjective(Game.GetActiveQuestName(), "TEXT_QUEST_BEARDY_BALDY_OBJECTIVE_02", "", "");
    GoTalkToBarber();
    if (false) { } else for (;;) {
        do { if (IsActiveThreadTerminating()) return _Fail_Val.Return(); } while (0);
        while (!AcquiredNewHairdo && !AcquiredNewBeard && !AcquiredNewTash) {
            Game.NewScriptFrame();
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
        }
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
        Game.SetQuestCardObjective(Game.GetActiveQuestName(), "TEXT_QUEST_BEARDY_BALDY_OBJECTIVE_03", "", "");
        GoTalkToBeardyBaldy();
        ResetHairWatch();
        if (false) { } else for (;;) {
            do { if (IsActiveThreadTerminating()) return _Fail_Val.Return(); } while (0);
            while (!Game.MsgOnRegionLoaded(regionName)) {
                Game.NewScriptFrame();
                if (IsActiveThreadTerminating()) {
                    // Inline-budget: retail 00E50CF9 calls the 66-byte CScriptThing destructor on this exit.
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
                    return _Fail_Val.Return();
#ifdef _MSC_VER
#pragma inline_depth()
#endif
                }
            }
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
            if (regionName == "BowerstoneSlumsWarehouses")
                break;
            Game.NewScriptFrame();
        }
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
        beardyBaldyMan = Game.GetThingWithScriptName("BB_BeardyBaldyMan");
        if (beardyBaldyMan.IsAlive()) {
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
            while (!beardyBaldyMan.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
                Game.NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
            if (!Phase3RequirementsComplete) {
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
                Game.SetQuestCardObjective(Game.GetActiveQuestName(), "TEXT_QUEST_BEARDY_BALDY_OBJECTIVE_02", "", "");
            }
        }
        GoTalkToBarber();
        Game.NewScriptFrame();
    }
}

static int run(int hair,int alive,int phase3,int delay,int stop) {
    memset(&state,0,sizeof(state));state.stop=stop;state.hair=hair;state.alive=alive;state.phase3=phase3;state.delay=delay;
    NScript::CV_BeardyBaldyScript script; script.WatchForQuestCardConditions();
    ck(state.card<=1 && state.beardy<=1 && state.barber<=2 && state.quest==state.card+state.objectives);
    if(!stop) {ck(state.card==1&&state.beardy==1&&state.barber==2&&state.regions==4);
      ck(state.objectives==2+(alive&&!phase3));ck(state.talks==(alive?delay+1:0));}
    else ck(state.checks==stop);
    return state.checks;
}
int main(){
 for(int h=0;h<3;++h)for(int a=0;a<2;++a)for(int p=0;p<2;++p)for(int d=0;d<4;++d){
  int n=run(h,a,p,d,0);for(int stop=1;stop<=n;++stop)run(h,a,p,d,stop);
 }
 puts("00e50910_TEST PASS");return 0;
}