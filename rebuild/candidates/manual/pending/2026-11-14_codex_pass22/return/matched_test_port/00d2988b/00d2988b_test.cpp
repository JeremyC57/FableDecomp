// Full production Main and timer member bodies are copied verbatim below.
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <string>
#include <map>
#include <new>
#define FABLE_FASTCALL
struct Event { int kind; unsigned value; };
struct TestState {
    int stop,checks,frames,finishFrame,failStarts,starts,registered,deregistered,setCalls,getCalls,updates;
    int messages,pauses,removed,markers,movies,liveHandles,allocated,freed,pauseStage,cutscenes,fades,quests,teleports,stories;
    long timer;
    float amount,upper,interval,initial,frameBar;
    float* bar;
    bool initialEnough,gotZero,resetAfterZero,finished;
    Event events[2048];int eventCount;
};
static TestState state;
static unsigned Bits(float f){unsigned x;assert(sizeof(x)==4&&sizeof(f)==4);memcpy(&x,&f,4);return x;}
static void EventAt(int k,unsigned v=0){assert(state.eventCount<2048);state.events[state.eventCount].kind=k;state.events[state.eventCount++].value=v;}
class CCharString { public:std::string Text;CCharString(const char* s,long=-1):Text(s){}bool operator<(const CCharString& o)const{return Text<o.Text;} };
class CScriptThing { public:int Identity;CScriptThing(int i=0):Identity(i){} };
static CScriptThing Hero(1);
struct Token {int kind;Token(int k):kind(k){++state.allocated;}~Token(){++state.freed;}};
struct CCPPointerInfo {unsigned long RefCount;void(*DeleteFunc)(void*);void* Data;CCPPointerInfo(unsigned long n,void(*f)(void*),void* p):RefCount(n),DeleteFunc(f),Data(p){}};
static void DeleteToken(void* p){assert(p);delete static_cast<Token*>(p);}
template<class T>class CCountedPointer {
public:T* Data;CCPPointerInfo* Info;
    CCountedPointer():Data(0),Info(0){}
    CCountedPointer(const CCountedPointer& o):Data(o.Data),Info(o.Info){if(Info)++Info->RefCount;}
    ~CCountedPointer(){ReleaseData();}
    void ReleaseData(){if(Info){--Info->RefCount;if(Info->RefCount==0){Info->DeleteFunc(Info->Data);delete Info;}}Data=0;Info=0;}
    CCountedPointer& operator=(const CCountedPointer& o){if(Info!=o.Info){ReleaseData();Data=o.Data;Info=o.Info;if(Info)++Info->RefCount;}return *this;}
    void Make(int kind){assert(!Data&&!Info);Data=new T(kind);Info=new CCPPointerInfo(1,DeleteToken,Data);}
};
class CScriptGameResourceObjectBase {public:virtual ~CScriptGameResourceObjectBase(){}};
class CScriptGameResourceObjectScriptedThingBase:public CScriptGameResourceObjectBase {
public:CCountedPointer<Token> PImp;
    CScriptGameResourceObjectScriptedThingBase(){++state.liveHandles;}
    CScriptGameResourceObjectScriptedThingBase(const CScriptGameResourceObjectScriptedThingBase& o):PImp(o.PImp){++state.liveHandles;}
    ~CScriptGameResourceObjectScriptedThingBase(){--state.liveHandles;}
    CScriptGameResourceObjectScriptedThingBase& operator=(const CScriptGameResourceObjectScriptedThingBase& o){PImp=o.PImp;return *this;}
    bool IsValid()const{return PImp.Data!=0;}
    void Release(){assert(false);}
};
class CScriptGameResourceObjectMovieBase:public CScriptGameResourceObjectBase {public:CCountedPointer<Token>PImp;~CScriptGameResourceObjectMovieBase(){assert(state.pauseStage==2);--state.movies;}};
// The selected shared STLport constructor is exercised directly as well.
// Namespace isolation and ABI assertions are fixture adaptations only.
#define std NativeSTL
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_MAP_INDEX_DECL_ONLY
#include "stlport/fable_map.h"
#undef std
static void CheckRealMapConstructor(){
    typedef NativeSTL::map<CCharString,CScriptGameResourceObjectScriptedThingBase> Map;
    for(int k=0;k<4096;++k){
        Map map;assert(map.size()==0&&map.begin()==map.end());
        NativeSTL::_Rb_tree_node_base* node=map.end()._M_node;
        assert(node&&!node->_M_color&&!node->_M_parent&&node->_M_left==node&&node->_M_right==node);
    }
}
enum EScriptAIPriority { PRIORITY_ZERO=0 };
class CRGBColour {public:unsigned char B,G,R,A;CRGBColour(unsigned char r,unsigned char g,unsigned char b):B(b),G(g),R(r),A(255){}};
class CGameScriptInterfaceBase {
public:
    void NewScriptFrame(){++state.frames;if(state.timer>0)--state.timer;if(state.frames==state.finishFrame)*state.bar=state.upper;state.frameBar=*state.bar;state.gotZero=false;state.resetAfterZero=false;EventAt(1,Bits(*state.bar));}
    long RegisterTimer(){assert(!state.registered);++state.registered;EventAt(2);return 19;}
    void DeregisterTimer(long i){assert(i==19&&state.registered==1&&!state.deregistered);++state.deregistered;}
    void SetTimer(long i,long t){assert(i==19);if(!state.setCalls)assert(t==1);else{assert(state.gotZero&&t==(long)state.interval);state.resetAfterZero=true;}++state.setCalls;state.timer=t;EventAt(3,(unsigned)t);}
    long GetTimer(long i){assert(i==19);++state.getCalls;state.gotZero=state.timer==0;return state.timer;}
    void HeroReceiveMessageFromGuildMaster(const CCharString& s,const CCharString& empty,bool a,bool b){
        assert(state.initial<state.amount&&empty.Text.empty()&&a&&b);
        const char* names[]={"TEXT_QST_025_MOTHER_INSTRUCTIONS_10","TEXT_QST_025_MOTHER_INSTRUCTIONS_20"};
        assert(state.messages<2&&s.Text==names[state.messages]);assert(state.pauses==state.messages);++state.messages;EventAt(4,state.messages);
    }
    void Pause(float s){assert(state.messages==state.pauses+1);assert(s==(state.pauses?4.0f:3.0f));++state.pauses;EventAt(5,Bits(s));}
    void UpdateQuestInfoBar(long i,float v,float a,float b){
        assert(i==71&&a==-1.0f&&b==-1.0f);float expected=state.frameBar;
        if(state.gotZero){expected=expected<state.amount?0.0f:expected-state.amount;assert(state.resetAfterZero);}else assert(!state.resetAfterZero);
        assert(Bits(v)==Bits(expected)&&Bits(*state.bar)==Bits(expected));if(!(v<state.upper))state.finished=true;++state.updates;EventAt(6,Bits(v));
    }
    void RemoveQuestInfoElement(long i){assert(i==71&&!(*state.bar<state.upper)&&!state.removed);++state.removed;EventAt(7);}
    void MiniMapRemoveMarker(const CScriptThing& x){assert(x.Identity==2&&state.removed==1&&!state.markers);++state.markers;EventAt(8);}
    CScriptThing& GetHero(){return Hero;}
    bool StartScriptingEntity(const CScriptThing& h,CScriptGameResourceObjectScriptedThingBase& x,EScriptAIPriority p){
        assert(!state.initialEnough&&state.markers==1&&h.Identity==1&&p==4&&!x.IsValid());++state.starts;EventAt(9,state.starts);
        if(state.starts<=state.failStarts)return false;x.PImp.Make(7);return true;
    }
    void StartMovieSequence(const CCharString& n,CScriptGameResourceObjectMovieBase& m){assert(n.Text.empty()&&!state.movies&&state.liveHandles>=2);m.PImp.Make(8);++state.movies;EventAt(10);}
    void PauseAllNonScriptedEntities(bool p){if(p){assert(state.pauseStage==0&&state.movies==1);state.pauseStage=1;}else{assert(state.pauseStage==1&&state.cutscenes==1);state.pauseStage=2;}EventAt(11,p?1:0);}
    bool FadeScreenOut(float a,float b,CRGBColour c){assert(a==0.5f&&b==0.0f&&c.R==0&&c.G==0&&c.B==0&&c.A==255);assert(!state.movies&&!state.liveHandles&&state.allocated==state.freed&&state.cutscenes==1);++state.fades;EventAt(13);return true;}
    void ActivateQuestWithoutLoadingResources(const CCharString& n){assert(n.Text=="Q_EndGame_Barriers"&&state.fades==1);++state.quests;EventAt(14);}
    CScriptThing GetThingWithScriptName(const CCharString& n){assert(n.Text=="MG_EndGameTeleport"&&state.quests==1);return CScriptThing(3);}
    void EntityTeleportToThing(const CScriptThing& a,const CScriptThing& b,bool p){assert(a.Identity==1&&b.Identity==3&&!p&&state.quests==1);++state.teleports;EventAt(15);}
};
static CGameScriptInterfaceBase Interface;
class CWideScreenMagicPauseEntities {CGameScriptInterfaceBase* Game;public:CWideScreenMagicPauseEntities(CGameScriptInterfaceBase& g):Game(&g){Game->PauseAllNonScriptedEntities(true);}~CWideScreenMagicPauseEntities(){Game->PauseAllNonScriptedEntities(false);}};
class CTimer {long TimerIndex;public:CTimer();~CTimer();CTimer& operator=(long);long GetTime()const;bool operator==(long)const;};
class TRetVoid {public:void Return(){}};
class CScriptDef {public:float EndGame_UpperBarLimit,EndGame_BarDecrementAmount,EndGame_BarDecrementInterval;};
template<class T>class CDefPointer {public:T* Object;T* operator->()const{return Object;}};
static CScriptDef Definition;
static CDefPointer<const CScriptDef> pGlobalScriptDef;
CGameScriptInterfaceBase* pDSTGame=&Interface;
void AddLogbookStoryEntry(long n){assert(n==240&&state.teleports==1);++state.stories;EventAt(16);}
void RunCutsceneMacro(const CCharString& n,std::map<CCharString,CScriptGameResourceObjectScriptedThingBase>& cast,std::map<CCharString,bool>* a,std::map<CCharString,CCharString>* b,bool c,bool d){
    assert(n.Text=="CS_GATEWAY_OPEN"&&!a&&!b&&!c&&d&&state.pauseStage==1&&state.movies==1&&cast.size()==1);
    std::map<CCharString,CScriptGameResourceObjectScriptedThingBase>::iterator i=cast.find(CCharString("HERO"));assert(i!=cast.end()&&i->second.PImp.Data&&i->second.PImp.Data->kind==7&&i->second.PImp.Info->RefCount==2);
    ++state.cutscenes;EventAt(12);
}
namespace NScript {class CQ_EndGameScript {public:float UndeadBar;long GuiCounter;bool ShowUndeadBar;
class CHookCoastTeleporter {public:CQ_EndGameScript& ParentClass;CGameScriptInterfaceBase& Game;CScriptThing Me;
    CHookCoastTeleporter(CQ_EndGameScript& p):ParentClass(p),Game(Interface),Me(2){}
    bool IsActiveThreadTerminating()const{assert(state.checks<state.stop);return ++state.checks==state.stop;}
    void Main();
};};}

extern CGameScriptInterfaceBase* pDSTGame;
void FABLE_FASTCALL AddLogbookStoryEntry(long);
inline CTimer::~CTimer() { pDSTGame->DeregisterTimer(TimerIndex); }
inline CTimer::CTimer() { TimerIndex=pDSTGame->RegisterTimer(); }
inline CTimer& CTimer::operator=(long time) { pDSTGame->SetTimer(TimerIndex,time);return *this; }
inline long CTimer::GetTime() const { return pDSTGame->GetTimer(TimerIndex); }
inline bool CTimer::operator==(long time)const { return GetTime()==time; }
void NScript::CQ_EndGameScript::CHookCoastTeleporter::Main()
{
    TRetVoid _Fail_Val;
    bool PreviouslyKilledEnoughUndead=ParentClass.UndeadBar>=pGlobalScriptDef->EndGame_UpperBarLimit;
    if(ParentClass.UndeadBar<pGlobalScriptDef->EndGame_BarDecrementAmount) {
        if(IsActiveThreadTerminating())return _Fail_Val.Return();
        Game.HeroReceiveMessageFromGuildMaster("TEXT_QST_025_MOTHER_INSTRUCTIONS_10","",true,true);
        Game.Pause(3.0f);
        Game.HeroReceiveMessageFromGuildMaster("TEXT_QST_025_MOTHER_INSTRUCTIONS_20","",true,true);
        Game.Pause(4.0f);
        ParentClass.ShowUndeadBar=true;
    }
    {
        CTimer DecrementTimer;
        DecrementTimer=1;
        for(;ParentClass.UndeadBar<pGlobalScriptDef->EndGame_UpperBarLimit;) {
            Game.NewScriptFrame();
            if(IsActiveThreadTerminating())return _Fail_Val.Return();
            if(DecrementTimer==0) {
                if(IsActiveThreadTerminating())return _Fail_Val.Return();
                if(ParentClass.UndeadBar<pGlobalScriptDef->EndGame_BarDecrementAmount) {
                    if(IsActiveThreadTerminating())return _Fail_Val.Return();
                    ParentClass.UndeadBar=0;
                } else {
                    if(IsActiveThreadTerminating())return _Fail_Val.Return();
                    ParentClass.UndeadBar-=pGlobalScriptDef->EndGame_BarDecrementAmount;
                }
                DecrementTimer=pGlobalScriptDef->EndGame_BarDecrementInterval;
            }
            Game.UpdateQuestInfoBar(ParentClass.GuiCounter,ParentClass.UndeadBar,-1.0f,-1.0f);
        }
        if(IsActiveThreadTerminating())goto Terminating;
        Game.RemoveQuestInfoElement(ParentClass.GuiCounter);
        Game.MiniMapRemoveMarker(Me);
        if(!PreviouslyKilledEnoughUndead) {
            if(IsActiveThreadTerminating()) {
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
                return _Fail_Val.Return();
#ifdef _MSC_VER
#pragma inline_depth()
#endif
            }
            {
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
                CScriptGameResourceObjectScriptedThingBase seh_hero;
#ifdef _MSC_VER
#pragma inline_depth()
#endif
                if(seh_hero.IsValid())seh_hero.Release();
                while(!Game.StartScriptingEntity(Game.GetHero(),seh_hero,(EScriptAIPriority)4)) {
                    Game.NewScriptFrame();
                    if(IsActiveThreadTerminating()) {
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
                        return _Fail_Val.Return();
#ifdef _MSC_VER
#pragma inline_depth()
#endif
                    }
                }
                if(IsActiveThreadTerminating()) {
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
                    return _Fail_Val.Return();
#ifdef _MSC_VER
#pragma inline_depth()
#endif
                }
                std::map<CCharString,CScriptGameResourceObjectScriptedThingBase> mymap;
                mymap["HERO"]=seh_hero;
                {
                    CScriptGameResourceObjectMovieBase speech_movie;
                    Game.StartMovieSequence("",speech_movie);
                    CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
                    RunCutsceneMacro("CS_GATEWAY_OPEN",mymap,0,0,false,true);
                }
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
            }
#ifdef _MSC_VER
#pragma inline_depth()
#endif
            Game.FadeScreenOut(0.5f,0.0f,CRGBColour(0,0,0));
            Game.ActivateQuestWithoutLoadingResources("Q_EndGame_Barriers");
            Game.EntityTeleportToThing(Game.GetHero(),Game.GetThingWithScriptName("MG_EndGameTeleport"),false);
            AddLogbookStoryEntry(240);
        }
        while(true) {
            Game.NewScriptFrame();
            if(IsActiveThreadTerminating()) {
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
                return _Fail_Val.Return();
#ifdef _MSC_VER
#pragma inline_depth()
#endif
            }
        }
        if(IsActiveThreadTerminating()) {
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
            return _Fail_Val.Return();
#ifdef _MSC_VER
#pragma inline_depth()
#endif
        }
    }
    Terminating:
    return _Fail_Val.Return();
}

static float FloatBits(unsigned u){float f;memcpy(&f,&u,4);return f;}
int main(){
    assert(sizeof(float)==4&&sizeof(unsigned)==4);CheckRealMapConstructor();
    float inputs[]={-4.0f,0.0f,1.0f,2.0f,4.0f,7.0f,8.0f,12.0f,16.0f,FloatBits(0x7fc12345u),FloatBits(0x7f800000u),FloatBits(0xff800000u)};
    float amounts[]={2.0f,4.0f},uppers[]={8.0f,12.0f},intervals[]={-2.0f,0.0f,1.75f,3.5f};
    int finishes[]={0,1,3,6},failures[]={0,1,3};
    unsigned cases=0;
    for(int initial=0;initial<12;++initial)for(int amount=0;amount<2;++amount)for(int upper=0;upper<2;++upper)for(int interval=0;interval<4;++interval)for(int finish=0;finish<4;++finish)for(int fail=0;fail<3;++fail){
        Event previous[2048];int previousCount=0;
        for(int stop=1;stop<=48;++stop){
            memset(&state,0,sizeof(state));state.stop=stop;state.initial=inputs[initial];state.amount=amounts[amount];state.upper=uppers[upper];state.interval=intervals[interval];state.finishFrame=finishes[finish];state.failStarts=failures[fail];state.initialEnough=state.initial>=state.upper;
            Definition.EndGame_UpperBarLimit=state.upper;Definition.EndGame_BarDecrementAmount=state.amount;Definition.EndGame_BarDecrementInterval=state.interval;pGlobalScriptDef.Object=&Definition;
            NScript::CQ_EndGameScript parent;parent.UndeadBar=state.initial;parent.GuiCounter=71;parent.ShowUndeadBar=false;state.bar=&parent.UndeadBar;
            NScript::CQ_EndGameScript::CHookCoastTeleporter script(parent);script.Main();
            assert(state.checks==stop&&state.registered==state.deregistered&&!state.liveHandles&&!state.movies&&state.allocated==state.freed);
            assert(state.messages==0||state.messages==2);assert(state.messages==state.pauses);assert(parent.ShowUndeadBar==(state.messages==2));
            assert(state.registered==((state.initial<state.amount&&stop==1)?0:1));
            assert(state.removed==state.markers&&state.removed<=1&&state.cutscenes<=1&&state.fades==state.quests&&state.quests==state.teleports&&state.teleports==state.stories);
            assert(state.pauseStage==0||state.pauseStage==2);assert(state.cutscenes==state.stories);
            if(state.initialEnough)assert(!state.messages&&!state.updates&&!state.starts&&!state.cutscenes&&state.removed==(stop>1));
            if(stop==48&&!state.initialEnough&&(state.finished||!(state.initial<state.upper)))assert(state.stories==1);
            assert(previousCount<=state.eventCount);for(int n=0;n<previousCount;++n)assert(previous[n].kind==state.events[n].kind&&previous[n].value==state.events[n].value);
            previousCount=state.eventCount;memcpy(previous,state.events,previousCount*sizeof(Event));++cases;
        }
    }
    assert(cases==110592u);puts("00d2988b_TEST PASS");return 0;
}
