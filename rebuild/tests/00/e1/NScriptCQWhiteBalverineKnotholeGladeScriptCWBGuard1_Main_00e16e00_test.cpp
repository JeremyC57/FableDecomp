#include <stdio.h>
#include "rebuild_abi.h"
#include "fable_compiler.h"
#include <stdlib.h>
#include <string.h>
#define FABLE_RETAIL_PHASE1_ARCHER_PASS26
#include "engine/EScriptAIPriority.h"
#include "engine/EScriptEntityMoveType.h"
#include "engine/ECutsceneBehaviour.h"
#include "engine/TRetVoid.h"
static void check(bool x) { if (!x) { puts("assertion failed"); exit(1); } }
struct Scenario { int stop, probes, frame, failures, post, cycles, close, travel, busy, validity, starts, releases, asks, moves, removes, tasks, cutoff; bool predicate, paused, stopped; };
static Scenario s;
struct C3DVector { float X; };
static bool consciousAlive, consciousDown;
struct CScriptThing { int id; bool IsAlive()const{return consciousAlive;} bool IsUnconscious()const{return consciousDown;} C3DVector GetPos() const { C3DVector p={static_cast<float>(id)};return p; } };
namespace NScript {
struct CPredicateBase { virtual bool operator()()const=0; virtual CPredicateBase* Clone()const=0; virtual ~CPredicateBase(){} };
struct CIsThingConscious : CPredicateBase { explicit CIsThingConscious(const CScriptThing&); virtual ~CIsThingConscious(); virtual bool operator()()const; virtual CPredicateBase* Clone()const; private: CScriptThing Thing; };
}
struct CScriptGameResourceObjectScriptedThingBase {
 bool IsValid() const {return s.validity!=0;}
 void Release(){check(s.validity!=0);++s.releases;}
 CScriptThing GetScriptThing() const {CScriptThing t={1};return t;}
 void MoveToPosition(const C3DVector& p,float r,EScriptEntityMoveType m,bool f,bool g){
  check(s.paused && !s.stopped && !f && g && m==ENTITY_MOVE_RUN);
  if (s.moves==0) check(p.X==2.0f && r==3.0f);
  else {check(p.X==4.0f && r==0.0f && s.asks>0);s.tasks=s.busy;}
  ++s.moves;
 }
 bool IsPerformingScriptTask(){return s.tasks-->0;}
};
static bool IsDistanceBetweenThingsUnder(const CScriptThing& t,const CScriptThing& u,float r){
 check(t.id==1 && s.paused && !s.stopped);
 if(u.id==2) {check(r==5.0f);return s.post--<=0;}
 check(u.id==3 && r==30.0f);return s.close!=0;
}
static bool IsDistanceFromThingToPositionOver(const CScriptThing& t,const C3DVector& p,float r){
 check(t.id==1 && p.X==4.0f && r==2.0f && !s.stopped);return s.travel-->0;
}
struct CScriptDataBase {};
namespace NScript {struct CQ_SunnyvaleMasterData:CScriptDataBase { bool WhiteBalverineFinished; };}
typedef NScript::CQ_SunnyvaleMasterData Master;
struct GameStub {
 Master* master;
 void NewScriptFrame(){check(s.predicate && !s.stopped);++s.frame;if(s.asks>=s.cycles)master->WhiteBalverineFinished=true;check(s.frame<120);}
 bool StartScriptingEntity(const CScriptThing& t,CScriptGameResourceObjectScriptedThingBase&,EScriptAIPriority v){check(t.id==1 && v==SCRIPT_AI_PRIORITY_HIGH);return s.starts++>=s.failures;}
 void EntitySetCutsceneBehaviour(const CScriptThing& t,ECutsceneBehaviour v){check(t.id==1 && v==CUTSCENE_BEHAVIOUR_PAUSED);s.paused=true;}
 CScriptThing GetThingWithScriptName(const char* n){CScriptThing t;if(!strcmp(n,"WB_WhiteBalverine")){t.id=3;++s.asks;}else{check(!strcmp(n,"WB_Guard1Pos"));t.id=2;}return t;}
 CScriptThing GetHero(){check(s.asks>0 && s.close);CScriptThing t={4};return t;}
 void RemoveThing(const CScriptThing& t,bool a,bool b){check(t.id==1 && !a && b && !s.stopped && master->WhiteBalverineFinished);++s.removes;}
};
struct CScriptBase {};
struct CEntityScriptBase { GameStub& Game; CScriptThing Me; CEntityScriptBase(GameStub& g,const CScriptThing& t):Game(g),Me(t){} virtual ~CEntityScriptBase(){}virtual void Main()=0; virtual void Init()=0;virtual CScriptBase* GetParentScript()const=0;virtual void OnPredicateFail()=0;void SetPredicate(const NScript::CPredicateBase& p){check(!s.predicate);NScript::CPredicateBase* c=p.Clone();check((*c)()==(consciousAlive&&!consciousDown));delete c;s.predicate=true;} bool IsActiveThreadTerminating(){check(!s.stopped);s.stopped=++s.probes==s.stop;return s.stopped;} };
template <class T> struct CCountedPointer {T* p;int* refs;explicit CCountedPointer(T* q):p(q),refs(new int(1)){} CCountedPointer(const CCountedPointer& q):p(q.p),refs(q.refs){++*refs;}~CCountedPointer(){if(--*refs==0){delete p;delete refs;}}T* operator->()const{return p;}};
namespace NScript {
struct CQ_WhiteBalverineKnotholeGladeScript:CScriptBase {GameStub Game;class CWB_Guard1;};
class CQ_WhiteBalverineKnotholeGladeScript::CWB_Guard1:public CEntityScriptBase {public: CWB_Guard1(CQ_WhiteBalverineKnotholeGladeScript&,CScriptDataBase*,const CScriptThing&); static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_WhiteBalverineKnotholeGladeScript&,CScriptDataBase*,const CScriptThing&); CQ_SunnyvaleMasterData* GetMasterData() { return PMasterData; } virtual CScriptBase* GetParentScript()const; private:virtual void Main();virtual void Init();virtual void OnPredicateFail();protected:CQ_WhiteBalverineKnotholeGladeScript& ParentClass;CQ_SunnyvaleMasterData* PMasterData;};
}
inline NScript::CIsThingConscious::CIsThingConscious(const CScriptThing& thing) : Thing(thing) {}
// Inline-budget: the leaf expands the predicate destructor and calls the script thing destructor.
#ifdef _MSC_VER
#pragma inline_depth(2)
#endif
FABLE_FORCEINLINE NScript::CIsThingConscious::~CIsThingConscious() {}
#ifdef _MSC_VER
#pragma inline_depth()
#endif
inline bool NScript::CIsThingConscious::operator()() const { return Thing.IsAlive() && !Thing.IsUnconscious(); }
inline NScript::CPredicateBase* NScript::CIsThingConscious::Clone() const { return new CIsThingConscious(Thing); }

namespace NScript {
CCountedPointer<CEntityScriptBase> FABLE_FASTCALL CQ_WhiteBalverineKnotholeGladeScript::CWB_Guard1::Alloc(
    CQ_WhiteBalverineKnotholeGladeScript& parent, CScriptDataBase* data, const CScriptThing& me)
{
    return CCountedPointer<CEntityScriptBase>(new CWB_Guard1(parent, data, me));
}
inline CQ_WhiteBalverineKnotholeGladeScript::CWB_Guard1::CWB_Guard1(
    CQ_WhiteBalverineKnotholeGladeScript& parent, CScriptDataBase* data, const CScriptThing& me)
    : CEntityScriptBase(parent.Game, me), ParentClass(parent)
{
    PMasterData = static_cast<CQ_SunnyvaleMasterData*>(data);
}
CScriptBase* CQ_WhiteBalverineKnotholeGladeScript::CWB_Guard1::GetParentScript() const { return &ParentClass; }
void CQ_WhiteBalverineKnotholeGladeScript::CWB_Guard1::Init() {}
void CQ_WhiteBalverineKnotholeGladeScript::CWB_Guard1::OnPredicateFail() {}
}


inline void NScript::CQ_WhiteBalverineKnotholeGladeScript::CWB_Guard1::Main()
{
    TRetVoid _Fail_Val;
    SetPredicate(CIsThingConscious(Me));
    Game.NewScriptFrame();
    if (IsActiveThreadTerminating())
        return _Fail_Val.Return();
    CScriptGameResourceObjectScriptedThingBase seh_me;
    if (seh_me.IsValid())
        seh_me.Release();
    while (!Game.StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGH)) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating())
        return _Fail_Val.Return();
    Game.EntitySetCutsceneBehaviour(Me, CUTSCENE_BEHAVIOUR_PAUSED);
    CScriptThing G1 = Game.GetThingWithScriptName("WB_Guard1Pos");
    seh_me.MoveToPosition(G1.GetPos(), 3.0f, ENTITY_MOVE_RUN, false, true);
    while (!IsDistanceBetweenThingsUnder(Me, G1, 5.0f)) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating())
        return _Fail_Val.Return();
    while (!GetMasterData()->WhiteBalverineFinished) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
        CScriptThing Balv = Game.GetThingWithScriptName("WB_WhiteBalverine");
        if (IsDistanceBetweenThingsUnder(Me, Balv, 30.0f)) {
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
            EScriptEntityMoveType local_movetype = ENTITY_MOVE_RUN;
            float local_range = 2.0f;
            C3DVector local_position = Game.GetHero().GetPos();
            while (IsDistanceFromThingToPositionOver(seh_me.GetScriptThing(), local_position, local_range)) {
                Game.NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
                seh_me.MoveToPosition(local_position, 0.0f, local_movetype, false, true);
                while (seh_me.IsPerformingScriptTask()) {
                    Game.NewScriptFrame();
                    if (IsActiveThreadTerminating())
                        return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
        }
    }
    if (IsActiveThreadTerminating())
        return _Fail_Val.Return();
    Game.RemoveThing(Me, false, true);
}

int main(){ for(int mode=0;mode<256;++mode)for(int stop=0;stop<40;++stop){
 memset(&s,0,sizeof(s));s.failures=mode&3;s.post=(mode>>2)&3;s.cycles=1+((mode>>4)&1);s.close=(mode>>5)&1;s.travel=(mode>>6)&1;s.busy=(mode>>1)&3;s.validity=(mode>>7)&1;s.stop=stop;
 Master m;m.WhiteBalverineFinished=false;NScript::CQ_WhiteBalverineKnotholeGladeScript parent;parent.Game.master=&m;CScriptThing me={1};consciousAlive=(mode&1)!=0;consciousDown=(mode&2)!=0;CCountedPointer<CEntityScriptBase> g=NScript::CQ_WhiteBalverineKnotholeGladeScript::CWB_Guard1::Alloc(parent,&m,me);check(g->GetParentScript()==&parent);g->Init();g->OnPredicateFail();g->Main();
 check(s.predicate && s.removes==(s.stopped?0:1));check(s.releases==((!s.stopped||s.probes>1)?s.validity:0));
 if(!s.stopped)check(s.starts==s.failures+1 && s.moves==1+((mode>>6)&1)*s.close && s.asks>=s.cycles);
 } puts("00e16e00_TEST PASS");return 0;}