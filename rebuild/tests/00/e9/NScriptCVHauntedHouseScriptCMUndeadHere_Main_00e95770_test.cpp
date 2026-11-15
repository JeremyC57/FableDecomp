#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <new>
#include "engine/CCountedPointer.h"
struct Event {int kind,a,b;};
struct TestState {
    int stop,checks,frames,startFrame,doorFrame,life,pauseMode,farMode,randomPolicy,failCreation;
    int starts,pauses,creates,resets,sets,randCalls,distanceCalls,aliveReads,deaths,allocated,freed;
    int fastSword,fastAxe,slowSword,slowAxe,delay,frameAlive,initialAlive,initialCounter;
    bool terminated,far,paused;
    long* alive;long* counter;long* maximum;bool* inHouse;bool* door;
    Event events[2048];int count;
};
static TestState state;
struct Token;
static Token* Current;
static void EventAt(int k,int a=0,int b=0){assert(!state.terminated&&state.count<2048);state.events[state.count].kind=k;state.events[state.count].a=a;state.events[state.count++].b=b;}
struct Token {int frames;bool notified;Token(int n):frames(n),notified(false){++state.allocated;}~Token(){if(Current==this)Current=0;++state.freed;}};
struct C3DVector {float X,Y,Z;};
class CCharString {public:const char* Text;CCharString(const char* s,long n=-1):Text(s){assert(n==-1);}~CCharString(){}};
class CBaseClass {public:CBaseClass(){}virtual ~CBaseClass(){}};
class CScriptThing:public CBaseClass {
public:CCountedPointer<Token> PImp;int Identity;C3DVector Pos;
    CScriptThing();virtual ~CScriptThing();
    CScriptThing(const CScriptThing& o):PImp(o.PImp),Identity(o.Identity),Pos(o.Pos){}
    CScriptThing& operator=(const CScriptThing& o){PImp=o.PImp;Identity=o.Identity;Pos=o.Pos;return *this;}
    virtual const C3DVector& GetPos()const{assert(Identity==2);return Pos;}
    virtual bool IsAlive()const{++state.aliveReads;return PImp.Data&&PImp.Data->frames>0;}
    virtual bool IsDead()const;
};
static CScriptThing Hero;
class TRetVoid {public:void Return(){}};
class CGameScriptInterfaceBase {
public:
    void NewScriptFrame(){
        assert(!state.delay);++state.frames;
        if(state.startFrame&&state.frames>=state.startFrame)*state.inHouse=true;
        if(state.doorFrame&&state.frames>=state.doorFrame)*state.door=true;
        if(Current){if(Current->frames>0)--Current->frames;if(Current->frames==0&&!Current->notified){Current->notified=true;--*state.alive;++state.deaths;}}
        state.frameAlive=(int)*state.alive;state.paused=false;
        state.far=state.farMode==1||(state.farMode==2&&(state.frames%2)!=0);
        EventAt(1,state.frames,(int)*state.alive);
    }
    CScriptThing& GetHero(){return Hero;}
    void SetCreatureCreationDelayFrames(long n){assert(n==1&&!state.delay);state.delay=1;++state.sets;EventAt(2);}
    void ResetCreatureCreationDelayFrames(){assert(state.delay==1&&state.creates==state.sets);state.delay=0;++state.resets;EventAt(4);}
    CScriptThing CreateCreature(const CCharString& def,const C3DVector& p,const CCharString& name,bool flag){
        assert(state.delay==1&&name.Text&&strcmp(name.Text,"HHUndead")==0&&flag&&p.X==12.0f&&p.Y==13.0f&&p.Z==14.0f);
        assert(*state.counter==state.initialCounter+state.creates&&*state.alive==state.frameAlive+1&&*state.alive<=*state.maximum);
        bool axe=strcmp(def.Text,"CREATURE_OK_UNDEAD_SOLDIER_AXE_HH")==0;
        assert(axe||strcmp(def.Text,"CREATURE_OK_UNDEAD_SOLDIER_SWORD_HH")==0);
        int expected=state.randomPolicy==2?(state.randCalls-1)%2:state.randomPolicy;assert(axe==(expected!=0));
        if(state.paused){assert(!*state.door&&state.far);if(axe)++state.slowAxe;else ++state.slowSword;}else{assert(*state.counter<10&&state.far);if(axe)++state.fastAxe;else ++state.fastSword;}
        ++state.creates;EventAt(3,axe?1:0,(int)*state.counter);
        CScriptThing value;value.Identity=3;value.Pos=p;
        if(!state.failCreation){value.PImp=CCountedPointer<Token>(new Token(state.life));Current=value.PImp.Data;}else Current=0;
        return value;
    }
    void Pause(float n){assert(n==1.0f&&!state.delay);++state.pauses;state.paused=true;if(state.pauseMode==1)*state.door=true;else if(state.pauseMode==2)*state.maximum=0;else if(state.pauseMode==3)state.far=true;EventAt(5,state.pauseMode);}
};
static CGameScriptInterfaceBase Interface;
bool IsDistanceBetweenThingsOver(const CScriptThing& a,const CScriptThing& b,float d){assert(a.Identity==2&&b.Identity==1&&d==3.0f);++state.distanceCalls;EventAt(6,state.far?1:0);return state.far;}
extern "C" int rand(void){assert(state.delay==0);int v=state.randomPolicy==2?state.randCalls%2:state.randomPolicy;++state.randCalls;EventAt(7,v);return v;}
namespace NScript {class CV_HauntedHouseScript {public:bool InHouse,CellarDoorOpen;long UndeadAlive,MaxUndead;
    class CM_UndeadHere {public:CV_HauntedHouseScript& ParentClass;CGameScriptInterfaceBase& Game;CScriptThing Me;long Counter;
        CM_UndeadHere(CV_HauntedHouseScript& p):ParentClass(p),Game(Interface){}
        bool IsActiveThreadTerminating()const{assert(!state.terminated&&state.checks<state.stop);state.terminated=++state.checks==state.stop;return state.terminated;}
        void Main();
    };
};}

inline bool CScriptThing::IsDead() const { return !IsAlive(); }
inline CScriptThing::CScriptThing() { }
inline CScriptThing::~CScriptThing() { }

void NScript::CV_HauntedHouseScript::CM_UndeadHere::Main()
{
    TRetVoid _Fail_Val;
    while (!ParentClass.InHouse) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    CScriptThing MyCreature;
    while (!ParentClass.CellarDoorOpen) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        if (ParentClass.UndeadAlive < ParentClass.MaxUndead) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (Counter < 10 && IsDistanceBetweenThingsOver(Me, Game.GetHero(), 3.0f)) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                ParentClass.UndeadAlive++;
                if (rand() % 2 == 0) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    Game.SetCreatureCreationDelayFrames(1);
                    MyCreature = Game.CreateCreature("CREATURE_OK_UNDEAD_SOLDIER_SWORD_HH", Me.GetPos(), "HHUndead", true);
                    Game.ResetCreatureCreationDelayFrames();
                } else {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    Game.SetCreatureCreationDelayFrames(1);
                    MyCreature = Game.CreateCreature("CREATURE_OK_UNDEAD_SOLDIER_AXE_HH", Me.GetPos(), "HHUndead", true);
                    Game.ResetCreatureCreationDelayFrames();
                }
                Counter++;
            } else {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                Game.Pause(1.0f);
                if (ParentClass.UndeadAlive < ParentClass.MaxUndead && !ParentClass.CellarDoorOpen
                    && IsDistanceBetweenThingsOver(Me, Game.GetHero(), 3.0f)) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    ParentClass.UndeadAlive++;
                    if (rand() % 2 == 0) {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        Game.SetCreatureCreationDelayFrames(1);
                        MyCreature = Game.CreateCreature("CREATURE_OK_UNDEAD_SOLDIER_SWORD_HH", Me.GetPos(), "HHUndead", true);
                        Game.ResetCreatureCreationDelayFrames();
                    } else {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        Game.SetCreatureCreationDelayFrames(1);
                        MyCreature = Game.CreateCreature("CREATURE_OK_UNDEAD_SOLDIER_AXE_HH", Me.GetPos(), "HHUndead", true);
                        Game.ResetCreatureCreationDelayFrames();
                    }
                    Counter++;
                }
            }
            while (!MyCreature.IsDead()) {
                Game.NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
}

int main(){
    static const int counters[]={0,9,10,12},starts[]={0,1,3,5},doors[]={0,2,8,20},maximums[]={0,1,3},alive[]={0,1,2,4},lives[]={0,1,3};
    int covered[4]={0,0,0,0};unsigned cases=0;Hero.Identity=1;
    for(int seed=0;seed<1024;++seed){Event previous[2048];int previousCount=0;
        for(int stop=1;stop<=48;++stop){
            memset(&state,0,sizeof(state));Current=0;state.stop=stop;state.startFrame=starts[seed/8%4];state.doorFrame=doors[seed/32%4];state.life=lives[seed/7%3];state.pauseMode=seed/11%4;state.farMode=seed/13%3;state.randomPolicy=seed/17%3;state.failCreation=seed/19%2;
            NScript::CV_HauntedHouseScript parent;parent.InHouse=seed/4%2!=0;parent.CellarDoorOpen=seed/128%2!=0;parent.UndeadAlive=alive[seed/5%4];parent.MaxUndead=maximums[seed/3%3];
            NScript::CV_HauntedHouseScript::CM_UndeadHere script(parent);script.Counter=counters[seed%4];script.Me.Identity=2;script.Me.Pos.X=12.0f;script.Me.Pos.Y=13.0f;script.Me.Pos.Z=14.0f;
            state.initialAlive=(int)parent.UndeadAlive;state.initialCounter=(int)script.Counter;state.alive=&parent.UndeadAlive;state.counter=&script.Counter;state.maximum=&parent.MaxUndead;state.inHouse=&parent.InHouse;state.door=&parent.CellarDoorOpen;
            script.Main();assert(state.checks<=stop);if(!state.terminated)assert(parent.InHouse&&parent.CellarDoorOpen);
            assert(!state.delay&&state.sets==state.creates&&state.creates==state.resets&&(state.randCalls==state.creates||(state.terminated&&state.randCalls==state.creates+1)));
            assert(script.Counter==state.initialCounter+state.creates&&parent.UndeadAlive==state.initialAlive+state.randCalls-state.deaths);
            assert(!Current&&state.allocated==state.freed);if(state.failCreation)assert(!state.allocated);else assert(state.allocated==state.creates);
            assert(previousCount<=state.count);for(int n=0;n<previousCount;++n)assert(previous[n].kind==state.events[n].kind&&previous[n].a==state.events[n].a&&previous[n].b==state.events[n].b);
            previousCount=state.count;memcpy(previous,state.events,previousCount*sizeof(Event));covered[0]+=state.fastSword;covered[1]+=state.fastAxe;covered[2]+=state.slowSword;covered[3]+=state.slowAxe;++cases;
        }
    }
    assert(cases==49152u);for(int k=0;k<4;++k)assert(covered[k]>0);puts("00e95770_TEST PASS");return 0;
}