// Exact selected function with dependency doubles; native branch/call traces are planned independently.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
static int errors,scenarios,strings,wide_strings,handles;
static int plan[256],answer[256],count,cursor,checks,ending,iteration,iterations;
static int current_mode,current_variant,alive_mother,null_marker;
enum Event { REGION=1,FRAME,TERM,HERO,POSSESSION,TAKE,RESTORE,WILL,QUEST,OBJECTIVE,
    LOOKUP,SPAWN_GUARD,CONVERSATION,LINE,COURTYARD,CHAMBER,ALIVE,POSITION,CREATE,FAIL_QUEST };
enum Stage { INITIAL=1,GUARD,COURT_WAIT,OUTER,FINAL_WAIT };
static void check(bool v) {if(!v)++errors;}
static int next(int event) {
    if(cursor>=count) {++errors;return event==TERM||event==REGION||event==COURTYARD;}
    if(plan[cursor]!=event) {if(errors<5)fprintf(stderr,"event %d expected %d at %d plan %d/%d/%d end %d\n",event,plan[cursor],cursor,current_mode,iterations,current_variant,ending);++errors;}
    int value=answer[cursor++];if(event==TERM){++checks;return checks==ending;}return value;
}
static void append(int event,int value=0){check(count<256);plan[count]=event;answer[count++]=value;}
static void on_frame(int);
static void on_take();
class CCharString {
public:
    const char* text;
    CCharString(const char* t,long n=-1):text(t){check(n==-1);++strings;}
    CCharString(const CCharString& r):text(r.text){++strings;}
    ~CCharString(){--strings;}
};
class CWideString {
public:
    const wchar_t* text;
    CWideString(const wchar_t* t):text(t){++wide_strings;}
    CWideString(const CWideString& r):text(r.text){++wide_strings;}
    ~CWideString(){--wide_strings;}
};
class C3DVector {public:float X,Y,Z;C3DVector(float x,float y,float z):X(x),Y(y),Z(z){}};
class CScriptThing {
public:
    int id;
    CScriptThing(int n=0):id(n){++handles;}
    CScriptThing(const CScriptThing& r):id(r.id){++handles;}
    virtual ~CScriptThing(){--handles;}
    virtual bool IsAlive() const {next(ALIVE);check(id==3);return alive_mother!=0;}
    const C3DVector& GetPos() const {
        next(POSITION);check(id==4);static const C3DVector zero(0.0f,0.0f,0.0f),position(2.0f,4.0f,8.0f);
        return null_marker?zero:position;
    }
};
class CGameScriptInterfaceBase {
public:
    CScriptThing Hero;
    CGameScriptInterfaceBase():Hero(9){}
    bool IsRegionLoaded(const CCharString& s) const {check(!strcmp(s.text,"PrisonCourtyard"));return next(REGION)!=0;}
    void NewScriptFrame() {int stage=next(FRAME);check(strings==0&&wide_strings==0);check(handles==((stage==GUARD||stage==COURT_WAIT)?3:1));on_frame(stage);}
    CScriptThing& GetHero(){next(HERO);return Hero;}
    bool IsObjectInThingsPossession(const CCharString& n,const CScriptThing& t) const {
        check(!strcmp(n.text,"OBJECT_HERO_EQUIPMENT_BAG")&&t.id==9);return next(POSSESSION)!=0;
    }
    void TakeObjectFromHero(const CCharString& n) const {next(TAKE);check(!strcmp(n.text,"OBJECT_HERO_EQUIPMENT_BAG"));on_take();}
    void ReturnAllConfiscatedItemsToHero() const {next(RESTORE);}
    void SetHeroWillAsUsable(bool v) const {next(WILL);check(v);}
    CCharString GetActiveQuestName() const {next(QUEST);return CCharString("prison_escape");}
    void SetQuestCardObjective(const CCharString& q,const CCharString& objective,const CCharString& place,const CCharString& last) const {
        next(OBJECTIVE);check(!strcmp(q.text,"prison_escape")&&!strcmp(objective.text,"TEXT_QUEST_PRISON_ESCAPE_OBJECTIVE_04")&&!strcmp(place.text,"PrisonTortureChamber")&&!last.text[0]);
    }
    CScriptThing GetThingWithScriptName(const CCharString& n) const {
        int id=next(LOOKUP);const char* name=id==1?"M_JumpGuard":id==3?"MotherTorture":"Q_MOTHER_MARKER_ONE";
        check((id==1||id==3||id==4)&&!strcmp(n.text,name));return CScriptThing(id);
    }
    CScriptThing CreateCreatureOnEntity(const CCharString& n,const CScriptThing& pos,const CCharString& script) const {
        next(SPAWN_GUARD);check(!strcmp(n.text,"CREATURE_PRISON_GUARD")&&pos.id==1&&!strcmp(script.text,"PrisonEscapeGuard"));return CScriptThing(2);
    }
    long AddNewConversation(const CScriptThing& t,bool first,bool second) const {next(CONVERSATION);check(t.id==2&&!first&&!second);return 37;}
    void AddLineToConversation(long index,const CCharString& text,bool first,const CScriptThing& guard,const CScriptThing& hero) const {
        next(LINE);check(index==37&&!strcmp(text.text,"TEXT_QST_055_GUARD_SPOTS_HERO")&&!first&&guard.id==2&&hero.id==9);
    }
    bool IsLevelLoaded(const CCharString& n) const {
        if(!strcmp(n.text,"PrisonCourtyard"))return next(COURTYARD)!=0;
        check(!strcmp(n.text,"PrisonTortureChamber"));return next(CHAMBER)!=0;
    }
    CScriptThing CreateCreature(const CCharString& name,const C3DVector& pos,const CCharString& script,bool first) const {
        next(CREATE);check(!strcmp(name.text,"CREATURE_SCARLET_ROBE")&&!strcmp(script.text,"MotherTorture")&&!first);
        check(pos.X==(null_marker?0.0f:2.0f)&&pos.Y==(null_marker?0.0f:4.0f)&&pos.Z==(null_marker?0.0f:8.0f));return CScriptThing(5);
    }
    void SetQuestAsFailed(const CCharString& quest,bool first,const CWideString& text,bool second) const {
        next(FAIL_QUEST);check(!strcmp(quest.text,"prison_escape")&&first&&!second&&!wcscmp(text.text,L"TEXT_QST_055_QUEST_FAILED_MOTHER_DIED"));
    }
};
namespace NScript {
class CQ_SunnyvaleMasterData {public:bool PrisonKeyStolenByHero;};
class CQ_PrisonEscapeRescueMotherScript {
public:
    CQ_PrisonEscapeRescueMotherScript(CGameScriptInterfaceBase& game,CQ_SunnyvaleMasterData& data) : Game(game),MasterScriptData(data) {}
    CGameScriptInterfaceBase& Game;CQ_SunnyvaleMasterData& MasterScriptData;
    bool MotherRescuedFromPrisonAutoSaveCheckPoint,HeroHasFoundWeapons,MotherDied;
    bool IsActiveThreadTerminating(){return next(TERM)!=0;}
    void WatchHeroEscapeStatus();
};
}
static NScript::CQ_PrisonEscapeRescueMotherScript* owner;
static void on_take(){check(owner->HeroHasFoundWeapons);}
static void on_frame(int stage) {
    if(stage==INITIAL)owner->MasterScriptData.PrisonKeyStolenByHero=true;
    if(stage==OUTER){++iteration;owner->MotherDied=iteration==iterations-1;}
}

void NScript::CQ_PrisonEscapeRescueMotherScript::WatchHeroEscapeStatus()
{
    bool TortureChamberReset=false;
    while (!((MasterScriptData.PrisonKeyStolenByHero && Game.IsRegionLoaded("PrisonCourtyard")) || MotherRescuedFromPrisonAutoSaveCheckPoint)) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
    }
    if (IsActiveThreadTerminating()) return;
    for (;;) {
        if (IsActiveThreadTerminating()) return;
        if (!HeroHasFoundWeapons && Game.IsObjectInThingsPossession("OBJECT_HERO_EQUIPMENT_BAG",Game.GetHero())) {
            if (IsActiveThreadTerminating()) return;
            HeroHasFoundWeapons=true;
            Game.TakeObjectFromHero("OBJECT_HERO_EQUIPMENT_BAG");
            Game.ReturnAllConfiscatedItemsToHero();
            Game.SetHeroWillAsUsable(true);
            Game.SetQuestCardObjective(Game.GetActiveQuestName(),"TEXT_QUEST_PRISON_ESCAPE_OBJECTIVE_04","PrisonTortureChamber","");
            CScriptThing jump_guard_pos=Game.GetThingWithScriptName("M_JumpGuard");
            CScriptThing jump_guard=Game.CreateCreatureOnEntity("CREATURE_PRISON_GUARD",jump_guard_pos,"PrisonEscapeGuard");
            Game.NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
            long HLStyleConversationIndex=Game.AddNewConversation(jump_guard,false,false);
            Game.AddLineToConversation(HLStyleConversationIndex,"TEXT_QST_055_GUARD_SPOTS_HERO",false,jump_guard,Game.GetHero());
            while (!Game.IsLevelLoaded("PrisonCourtyard")) {
                Game.NewScriptFrame();
                if (IsActiveThreadTerminating()) return;
            }
            if (IsActiveThreadTerminating()) return;
        }
        if (Game.IsLevelLoaded("PrisonTortureChamber") && !TortureChamberReset) {
            if (IsActiveThreadTerminating()) return;
            const CScriptThing& MotherInCell=Game.GetThingWithScriptName("MotherTorture");
            CScriptThing MotherSpawnPos=Game.GetThingWithScriptName("Q_MOTHER_MARKER_ONE");
            if (!MotherInCell.IsAlive()) {
                if (IsActiveThreadTerminating()) return;
                Game.CreateCreature("CREATURE_SCARLET_ROBE",MotherSpawnPos.GetPos(),"MotherTorture",false);
            }
            TortureChamberReset=true;
        }
        if (MotherDied) break;
        Game.NewScriptFrame();
    }
    if (IsActiveThreadTerminating()) return;
    Game.SetQuestAsFailed(Game.GetActiveQuestName(),true,L"TEXT_QST_055_QUEST_FAILED_MOTHER_DIED",false);
    for (;;) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
    }
}


static void make_plan(int mode,int loops,int variant) {
    count=0;
    if(mode==0)append(REGION,1);
    if(mode==1){append(FRAME,INITIAL);append(TERM);append(REGION,1);}
    if(mode==2){append(REGION,0);append(FRAME,INITIAL);append(TERM);append(REGION,1);}
    if(mode==4)append(REGION,0);
    append(TERM);
    bool weapons=(variant&2)!=0,reset=false;int equipment_due=variant&1,chamber_due=(variant+loops)%3;
    for(int i=0;i<loops;++i) {
        append(TERM);
        if(!weapons){append(HERO);append(POSSESSION,i>=equipment_due);
            if(i>=equipment_due) {
                append(TERM);append(TAKE);append(RESTORE);append(WILL);append(QUEST);append(OBJECTIVE);
                append(LOOKUP,1);append(SPAWN_GUARD);append(FRAME,GUARD);append(TERM);
                append(CONVERSATION);append(HERO);append(LINE);
                if(variant&1){append(COURTYARD,0);append(FRAME,COURT_WAIT);append(TERM);}
                append(COURTYARD,1);append(TERM);weapons=true;
            }
        }
        bool loaded=chamber_due!=2&&i>=chamber_due;
        append(CHAMBER,loaded);
        if(loaded&&!reset){append(TERM);append(LOOKUP,3);append(LOOKUP,4);append(ALIVE);
            if(!(variant&4)){append(TERM);append(POSITION);append(CREATE);}reset=true;
        }
        if(i+1<loops)append(FRAME,OUTER);
    }
    append(TERM);append(QUEST);append(FAIL_QUEST);
    for(int i=0;i<1+(variant&1);++i){append(FRAME,FINAL_WAIT);append(TERM);}
}
int main() {
    CGameScriptInterfaceBase game;NScript::CQ_SunnyvaleMasterData master;NScript::CQ_PrisonEscapeRescueMotherScript script(game,master);
    owner=&script;
    for(current_mode=0;current_mode<5;++current_mode)for(iterations=1;iterations<=3;++iterations)for(current_variant=0;current_variant<16;++current_variant) {
        make_plan(current_mode,iterations,current_variant);int guards=0;for(int i=0;i<count;++i)if(plan[i]==TERM)++guards;
        for(ending=1;ending<=guards;++ending) {
            ++scenarios;cursor=checks=iteration=0;alive_mother=(current_variant&4)!=0;null_marker=(current_variant&8)!=0;
            master.PrisonKeyStolenByHero=current_mode!=1&&current_mode!=3;
            script.MotherRescuedFromPrisonAutoSaveCheckPoint=current_mode>=3;
            script.HeroHasFoundWeapons=(current_variant&2)!=0;script.MotherDied=iterations==1;
            script.WatchHeroEscapeStatus();
            int expected_end=0,guard=0;bool gained=false;
            for(int i=0;i<count;++i){if(plan[i]==TAKE)gained=true;if(plan[i]==TERM&&++guard==ending){expected_end=i+1;break;}}
            check(cursor==expected_end&&checks==ending);
            check(script.HeroHasFoundWeapons==(((current_variant&2)!=0)||gained));
            check(script.MotherRescuedFromPrisonAutoSaveCheckPoint==(current_mode>=3));
            check(script.MotherDied==(iteration==iterations-1));
            check(strings==0&&wide_strings==0&&handles==1);
        }
    }
    check(scenarios==1951);
    if(errors){fprintf(stderr,"checks failed: %d in %d scenarios\n",errors,scenarios);return 1;}
    puts("00dd35d0_TEST PASS");return 0;
}