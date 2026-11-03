// 00d51520: exact selected Main, dependency doubles and independently transcribed native event plans.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <map>
static int errors,scenarios,strings,handles,resource_count,movie_count,timer_count;
static int plan[200],answers[200],count,cursor,checks,ending,allocations,failmask;
static int made,cleanup[16],cleanup_count,phase,active_bindings,timer_started;
enum Event { BIND=1,POST_SCRIPT,POST_GAME,LOOKUP,TITLE,RESOURCE,VALID,START,FRAME,TERM,HERO,
    MOVIE,PAUSE,CAMERA,CUTSCENE,HEALTH,RESET,REGISTER_TIMER,SET_TIMER,GET_TIMER,EXPRESSION,QUEST_NAME,DEACTIVATE };
static void check(bool v) { if(!v) ++errors; }
static int next(int kind) {
    if(cursor>=count) {++errors;return kind==TERM||kind==START;}
    check(plan[cursor]==kind);int result=answers[cursor++];
    if(kind==TERM) {++checks;return ending&&checks==ending;}
    return result;
}
static void append(int kind,int value=0) {check(count<200);plan[count]=kind;answers[count++]=value;}
static void destroyed(int id) {check(cleanup_count<16);cleanup[cleanup_count++]=id;}
static void on_frame(int);
static void check_after_cutscene();
class CCharString {
public:
    const char* text;
    CCharString(const char* t,long length=-1):text(t) {check(length==-1);++strings;}
    CCharString(const CCharString& r):text(r.text) {++strings;}
    ~CCharString() {--strings;}
    bool operator<(const CCharString& r) const {return strcmp(text,r.text)<0;}
};
class CScriptThing {
public:
    int id;
    CScriptThing(int n=0):id(n) {++handles;}
    CScriptThing(const CScriptThing& r):id(r.id) {++handles;}
    ~CScriptThing() {--handles;}
};
class CScriptGameResourceObjectScriptedThingBase {
public:
    int id;bool primary,valid;
    CScriptGameResourceObjectScriptedThingBase():id(0),primary(false),valid(false) {
        ++resource_count;
        if(made<4) {id=++made;primary=true;check(next(RESOURCE)==id);}
    }
    CScriptGameResourceObjectScriptedThingBase(const CScriptGameResourceObjectScriptedThingBase& r)
        :id(r.id),primary(false),valid(r.valid) {++resource_count;}
    CScriptGameResourceObjectScriptedThingBase& operator=(const CScriptGameResourceObjectScriptedThingBase& r) {
        id=r.id;valid=r.valid;return *this;
    }
    ~CScriptGameResourceObjectScriptedThingBase() {if(primary)destroyed(id);--resource_count;}
    bool IsValid() const {check(next(VALID)==id);return valid;}
    void Release() {++errors;valid=false;} // Native default construction makes this branch unreachable here.
};
class CScriptGameResourceObjectMovieBase {
public:
    CScriptGameResourceObjectMovieBase() {++movie_count;}
    ~CScriptGameResourceObjectMovieBase() {--movie_count;destroyed(200);}
};
enum EScriptAIPriority {SCRIPT_AI_PRIORITY_HIGHEST=4};
class CGameScriptInterfaceBase {
public:
    CScriptThing Hero;
    CGameScriptInterfaceBase():Hero(4) {}
    void PostAddScriptedEntities() {next(POST_GAME);}
    CScriptThing GetThingWithScriptName(const CCharString& name) const {
        int id=next(LOOKUP);const char* names[3]={"PreMeleeMaze","PreMeleeWhisper","TheRealGuildmaster"};
        check(id>=1&&id<=3);if(id>=1&&id<=3)check(!strcmp(name.text,names[id-1]));return CScriptThing(id);
    }
    void GiveHeroTitle(const CCharString& name) const {next(TITLE);check(!strcmp(name.text,"OBJECT_HERO_TITLE_CHICKEN_CHASER"));}
    CScriptThing& GetHero() {next(HERO);return Hero;}
    bool StartScriptingEntity(const CScriptThing& thing,CScriptGameResourceObjectScriptedThingBase& out,EScriptAIPriority priority) {
        int v=next(START);int id=v>>1;phase=id;
        check(thing.id==id&&out.id==id&&priority==SCRIPT_AI_PRIORITY_HIGHEST);out.valid=(v&1)!=0;return out.valid;
    }
    void NewScriptFrame() {int p=next(FRAME);phase=p;on_frame(p);}
    void StartMovieSequence(const CCharString& name,CScriptGameResourceObjectMovieBase&) {next(MOVIE);check(!name.text[0]&&movie_count==1);}
    void PauseAllNonScriptedEntities(bool value) {check(next(PAUSE)==int(value));}
    void FixMovieSequenceCamera(bool value) const {check(next(CAMERA)==int(value));}
    void ChangeHeroHealthBy(float amount,bool first,bool second) const {next(HEALTH);check(amount==1000.0f&&first&&!second);}
    void ResetPlayerCreatureCombatMultiplier() const {next(RESET);}
    long RegisterTimer() {next(REGISTER_TIMER);++timer_count;timer_started=1;check_after_cutscene();return 19;}
    void DeregisterTimer(long index) {check(index==19);--timer_count;destroyed(201);}
    void SetTimer(long index,long n) {next(SET_TIMER);check(index==19&&n==5);}
    long GetTimer(long index) const {check(index==19);return next(GET_TIMER);}
    void GiveHeroExpression(const CCharString& name,long mode,bool allowed) const {
        int id=next(EXPRESSION);const char* names[3]={"EXPRESSION_FART","EXPRESSION_BELCH","EXPRESSION_GIGGLE"};
        check(id>=0&&id<3);if(id>=0&&id<3)check(!strcmp(name.text,names[id]));check(mode==-1&&allowed);
    }
    CCharString GetActiveQuestName() const {next(QUEST_NAME);return CCharString("guild_training_premelee");}
    void DeactivateQuestLater(const CCharString& name,unsigned long delay) const {
        next(DEACTIVATE);check(!strcmp(name.text,"guild_training_premelee")&&delay==0);
    }
};
static CGameScriptInterfaceBase* pDSTGame;
class CWideScreenMagicPauseEntities {
    CGameScriptInterfaceBase* Game;
public:
    CWideScreenMagicPauseEntities(CGameScriptInterfaceBase& game):Game(&game) {Game->PauseAllNonScriptedEntities(true);}
    ~CWideScreenMagicPauseEntities() {Game->PauseAllNonScriptedEntities(false);destroyed(199);}
};
class CTimer {
    long TimerIndex;
public:
    CTimer():TimerIndex(pDSTGame->RegisterTimer()) {}
    ~CTimer() {pDSTGame->DeregisterTimer(TimerIndex);}
    CTimer& operator=(long n) {pDSTGame->SetTimer(TimerIndex,n);return *this;}
    long GetTime() const {return pDSTGame->GetTimer(TimerIndex);}
    bool operator>(long n) const {return GetTime()>n;}
};
class CEntityScriptBindingBase {public: virtual ~CEntityScriptBindingBase() {}};
class CEntityScriptBase {};
class CScriptDataBase {};
template<class T> class CCountedPointer {public:T* Data;CCountedPointer():Data(0) {}};
template<class T> class CEntityScriptBinding:public CEntityScriptBindingBase {
public:
    typedef CCountedPointer<CEntityScriptBase> (*TAllocFunc)(T&,CScriptDataBase*,const CScriptThing&);
    CCharString EntityScriptName;T* PParentScript;TAllocFunc ClassAllocFunc;unsigned long Flags;
    CEntityScriptBinding(const CCharString& name,T* parent,TAllocFunc f,unsigned long flags)
        :EntityScriptName(name),PParentScript(parent),ClassAllocFunc(f),Flags(flags) {++active_bindings;}
    ~CEntityScriptBinding() {--active_bindings;}
    static void* operator new(size_t n) throw() {int i=allocations++;return (failmask&(1<<i))?0:malloc(n);}
    static void operator delete(void* p) {free(p);}
};
static CEntityScriptBindingBase* bindings[3];
class CScriptBase {
public:
    void AddEntityScriptBinding(CEntityScriptBindingBase*);
    void PostAddScriptedEntities() {next(POST_SCRIPT);}
    bool IsActiveThreadTerminating() {return next(TERM)!=0;}
};
namespace NScript {
class CQ_SunnyvaleMasterData {public:bool GuildWarningOccuring;};
class CQ_GuildTrainingPreMeleeScript:public CScriptBase {
public:
    CQ_GuildTrainingPreMeleeScript(CGameScriptInterfaceBase& game,CQ_SunnyvaleMasterData& data) : Game(game),MasterScriptData(data) {}
    CGameScriptInterfaceBase& Game;CQ_SunnyvaleMasterData& MasterScriptData;
    bool MazeArrived,HeroSleeps,WhisperCutsceneFinished,GuildmasterTeleport,WhisperStopFollowing;
    long PreMeleeMode,DummyHits;
    void Main();
    class CTheRealGuildmaster {public:static CCountedPointer<CEntityScriptBase> Alloc(CQ_GuildTrainingPreMeleeScript&,CScriptDataBase*,const CScriptThing&){return CCountedPointer<CEntityScriptBase>();}};
    class CPreMeleeDummy {public:static CCountedPointer<CEntityScriptBase> Alloc(CQ_GuildTrainingPreMeleeScript&,CScriptDataBase*,const CScriptThing&){return CCountedPointer<CEntityScriptBase>();}};
    class CPreMeleeWhisper {public:static CCountedPointer<CEntityScriptBase> Alloc(CQ_GuildTrainingPreMeleeScript&,CScriptDataBase*,const CScriptThing&){return CCountedPointer<CEntityScriptBase>();}};
};
}
typedef NScript::CQ_GuildTrainingPreMeleeScript Script;
static Script* owner;
void CScriptBase::AddEntityScriptBinding(CEntityScriptBindingBase* p) {
    int id=next(BIND);check(id>=0&&id<3);if(id<0||id>2)return;bindings[id]=p;
    check((p==0)==((failmask&(1<<id))!=0));
    if(p) {
        CEntityScriptBinding<Script>* b=static_cast<CEntityScriptBinding<Script>*>(p);
        const char* names[3]={"TheRealGuildmaster","PreMeleeDummy","PreMeleeWhisper"};
        CEntityScriptBinding<Script>::TAllocFunc f[3]={&Script::CTheRealGuildmaster::Alloc,&Script::CPreMeleeDummy::Alloc,&Script::CPreMeleeWhisper::Alloc};
        check(!strcmp(b->EntityScriptName.text,names[id])&&b->PParentScript==owner&&b->ClassAllocFunc==f[id]&&b->Flags==1);
    }
}
static void on_frame(int p) {if(p==6)owner->HeroSleeps=true;}
static void check_after_cutscene() {check(owner->WhisperCutsceneFinished&&owner->GuildmasterTeleport&&!owner->MasterScriptData.GuildWarningOccuring);}
static void RunCutsceneMacro(const CCharString& name,std::map<CCharString,CScriptGameResourceObjectScriptedThingBase>& m,
    std::map<CCharString,bool>* flags,std::map<CCharString,CCharString>* arguments,bool first,bool second) {
    int id=next(CUTSCENE);const char* names[3]={"CS_GUILD_PREMELEE_INTRO","CS_GUILD_PREMELEE_BOOHOO","CS_GUILD_PREMELEE_WAKEUP"};
    check(id>=0&&id<3);if(id>=0&&id<3)check(!strcmp(name.text,names[id]));
    check(!flags&&!arguments&&!first&&second&&m.size()==4);
    const char* keys[4]={"MAZE","WHISPER","MASTER","HERO"};
    for(int k=0;k<4;++k) {std::map<CCharString,CScriptGameResourceObjectScriptedThingBase>::iterator i=m.find(keys[k]);check(i!=m.end());if(i!=m.end())check(i->second.id==k+1&&i->second.valid);}
    check(owner->MasterScriptData.GuildWarningOccuring&&!owner->WhisperCutsceneFinished&&!owner->GuildmasterTeleport&&owner->PreMeleeMode==0);
}

void NScript::CQ_GuildTrainingPreMeleeScript::Main()
{
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_GuildTrainingPreMeleeScript>("TheRealGuildmaster",this,&CTheRealGuildmaster::Alloc,1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_GuildTrainingPreMeleeScript>("PreMeleeDummy",this,&CPreMeleeDummy::Alloc,1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_GuildTrainingPreMeleeScript>("PreMeleeWhisper",this,&CPreMeleeWhisper::Alloc,1));
    PostAddScriptedEntities();
    Game.PostAddScriptedEntities();
    MasterScriptData.GuildWarningOccuring=true;
    CScriptThing Maze=Game.GetThingWithScriptName("PreMeleeMaze");
    CScriptThing Whisper=Game.GetThingWithScriptName("PreMeleeWhisper");
    CScriptThing GuildMaster=Game.GetThingWithScriptName("TheRealGuildmaster");
    Game.GiveHeroTitle("OBJECT_HERO_TITLE_CHICKEN_CHASER");
    PreMeleeMode=0;
    {
        CScriptGameResourceObjectScriptedThingBase seh_Maze;
        if (seh_Maze.IsValid()) seh_Maze.Release();
        while (!Game.StartScriptingEntity(Maze,seh_Maze,SCRIPT_AI_PRIORITY_HIGHEST)) {
            Game.NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
        }
        if (IsActiveThreadTerminating()) return;
        CScriptGameResourceObjectScriptedThingBase seh_Whisper;
        if (seh_Whisper.IsValid()) seh_Whisper.Release();
        while (!Game.StartScriptingEntity(Whisper,seh_Whisper,SCRIPT_AI_PRIORITY_HIGHEST)) {
            Game.NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
        }
        if (IsActiveThreadTerminating()) return;
        CScriptGameResourceObjectScriptedThingBase seh_GuildMaster;
        if (seh_GuildMaster.IsValid()) seh_GuildMaster.Release();
        while (!Game.StartScriptingEntity(GuildMaster,seh_GuildMaster,SCRIPT_AI_PRIORITY_HIGHEST)) {
            Game.NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
        }
        if (IsActiveThreadTerminating()) return;
        CScriptGameResourceObjectScriptedThingBase seh_Hero;
        if (seh_Hero.IsValid()) seh_Hero.Release();
        while (!Game.StartScriptingEntity(Game.GetHero(),seh_Hero,SCRIPT_AI_PRIORITY_HIGHEST)) {
            Game.NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
        }
        if (IsActiveThreadTerminating()) return;
        std::map<CCharString,CScriptGameResourceObjectScriptedThingBase> mymap;
        mymap["HERO"]=seh_Hero;
        mymap["MAZE"]=seh_Maze;
        mymap["WHISPER"]=seh_Whisper;
        mymap["MASTER"]=seh_GuildMaster;
        CScriptGameResourceObjectMovieBase speech_movie;
        Game.StartMovieSequence("",speech_movie);
        CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
        Game.FixMovieSequenceCamera(true);
        RunCutsceneMacro("CS_GUILD_PREMELEE_INTRO",mymap,0,0,false,true);
        RunCutsceneMacro("CS_GUILD_PREMELEE_BOOHOO",mymap,0,0,false,true);
        RunCutsceneMacro("CS_GUILD_PREMELEE_WAKEUP",mymap,0,0,false,true);
        Game.FixMovieSequenceCamera(false);
        Game.ChangeHeroHealthBy(1000.0f,true,false);
        Game.ResetPlayerCreatureCombatMultiplier();
    }
    WhisperCutsceneFinished=true;
    GuildmasterTeleport=true;
    MasterScriptData.GuildWarningOccuring=false;
    {
    CTimer ExpressionTimer;
    ExpressionTimer=5;
    while (ExpressionTimer.GetTime()>0) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
    }
    if (IsActiveThreadTerminating()) return;
    Game.GiveHeroExpression("EXPRESSION_FART",-1,true);
    Game.GiveHeroExpression("EXPRESSION_BELCH",-1,true);
    Game.GiveHeroExpression("EXPRESSION_GIGGLE",-1,true);
    while (!HeroSleeps) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
    }
    if (IsActiveThreadTerminating()) return;
    Game.DeactivateQuestLater(Game.GetActiveQuestName(),0);
    }
}


static void make_plan(int delays) {
    count=0;for(int i=0;i<3;++i)append(BIND,i);append(POST_SCRIPT);append(POST_GAME);
    for(int i=1;i<=3;++i)append(LOOKUP,i);append(TITLE);
    for(int i=1;i<=4;++i) {
        append(RESOURCE,i);append(VALID,i);
        if(delays&(1<<(i-1))) {if(i==4)append(HERO);append(START,i*2);append(FRAME,i);append(TERM);}
        if(i==4)append(HERO);append(START,i*2+1);append(TERM);
    }
    append(MOVIE);append(PAUSE,1);append(CAMERA,1);
    for(int i=0;i<3;++i)append(CUTSCENE,i);append(CAMERA,0);append(HEALTH);append(RESET);append(PAUSE,0);
    append(REGISTER_TIMER);append(SET_TIMER);
    if(delays&16) {append(GET_TIMER,5);append(FRAME,5);append(TERM);}
    append(GET_TIMER,0);append(TERM);for(int i=0;i<3;++i)append(EXPRESSION,i);
    if(delays&32) {append(FRAME,6);append(TERM);}
    append(TERM);append(QUEST_NAME);append(DEACTIVATE);
}
int main() {
    CGameScriptInterfaceBase game;NScript::CQ_SunnyvaleMasterData master;Script script(game,master);
    owner=&script;pDSTGame=&game;
    for(int delays=0;delays<64;++delays)for(failmask=0;failmask<8;++failmask) {
        make_plan(delays);int guards=0;for(int i=0;i<count;++i)if(plan[i]==TERM)++guards;
        for(ending=0;ending<=guards;++ending) {
            ++scenarios;cursor=checks=allocations=made=cleanup_count=phase=timer_started=0;
            script.HeroSleeps=(delays&32)==0;script.WhisperCutsceneFinished=script.GuildmasterTeleport=false;
            script.MazeArrived=true;script.WhisperStopFollowing=true;script.DummyHits=71;script.PreMeleeMode=9;master.GuildWarningOccuring=false;
            for(int i=0;i<3;++i)bindings[i]=0;
            script.Main();int expected_end=count;
            if(ending) {int g=0;for(int i=0;i<count;++i)if(plan[i]==TERM&&++g==ending){expected_end=i+1;break;}}
            check(cursor==expected_end&&checks==(ending?ending:guards));check(allocations==3);
            check(script.PreMeleeMode==0&&script.MazeArrived&&script.WhisperStopFollowing&&script.DummyHits==71);
            check(script.WhisperCutsceneFinished==(timer_started!=0)&&script.GuildmasterTeleport==(timer_started!=0));
            check(master.GuildWarningOccuring==(timer_started==0));
            check(handles==1&&resource_count==0&&movie_count==0&&timer_count==0&&strings==active_bindings);
            if(timer_started) {int expected[7]={199,200,4,3,2,1,201};check(cleanup_count==7);for(int i=0;i<7&&i<cleanup_count;++i)check(cleanup[i]==expected[i]);}
            else {check(cleanup_count==made);for(int i=0;i<made&&i<cleanup_count;++i)check(cleanup[i]==made-i);}
            for(int i=0;i<3;++i)delete bindings[i];check(strings==0&&active_bindings==0);
        }
    }
    check(scenarios==5120);
    if(errors){fprintf(stderr,"checks failed: %d\n",errors);return 1;}
    puts("00d51520_TEST PASS");return 0;
}
