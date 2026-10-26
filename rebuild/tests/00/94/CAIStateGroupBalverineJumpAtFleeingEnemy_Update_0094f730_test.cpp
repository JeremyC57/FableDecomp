#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define FABLE_ENGINE_CAIBRAIN_H
#define FABLE_ENGINE_CAISTATEGROUP_BALVERINEJUMPATFLEEINGENEMY_H
class CThingAICreature;
class CAIBrain { public: CThingAICreature* Creature; };
struct CAIStateGroup_BalverineJumpAtFleeingEnemy {
    unsigned char _pad_0x04[sizeof(void*)], PTarget[2*sizeof(void*)];
    long WaitUntilWorldFrameForNextAttempt;
};
#endif
// CAIStateGroup_BalverineJumpAtFleeingEnemy::Update() -- schedule next attempt, lunge at a live target, then break off after successful contact.
// Retail0094F730,230 bytes. State/brain use shared headers; thing Dead is retail+91 vs donor+a1.
// Actions have the verified0x114-byte retail combat base, rather than donor0x12c.
#include "engine/CAIStateGroup_BalverineJumpAtFleeingEnemy.h"
#include "engine/CAIBrain.h"
class CThing;
class CThingCreatureBase;
struct CRetailThing { unsigned char _prefix[0x91]; unsigned char Dead:1; };
struct CBaseIntelligentPointer {
    void* __vftable; CThing* Object;
    CThing* GetPItem() const; // 00A01B50
};
inline bool IsAlive(const CBaseIntelligentPointer& p) {
    if(p.GetPItem()!=0) {if(!((CRetailThing*)p.GetPItem())->Dead) return true;}
    return false;
}
class CCombatActionBase { public:
    virtual void v00(); unsigned char _body[0x110];
    ~CCombatActionBase(); // 0062E900
};
class CCreatureAction_BalverineLungeAttack : public CCombatActionBase { public:
    CCreatureAction_BalverineLungeAttack(CThingCreatureBase*,CThing*); // 008CCC70
};
class CCreatureAction_BalverineBreakOffFromCombatLong : public CCombatActionBase { public:
    CCreatureAction_BalverineBreakOffFromCombatLong(CThingCreatureBase*,CThing*); // 008CCD30
};
extern long g_framesPerSecond; // 01375550
struct CWorldClock {static long GetCurrentFrame();}; // 0049D870
struct CJumpAtFleeingMethods : CAIStateGroup_BalverineJumpAtFleeingEnemy {
    CAIBrain* Brain() const {return *(CAIBrain* const*)_pad_0x04;}
    const CBaseIntelligentPointer& Target() const {return *(const CBaseIntelligentPointer*)PTarget;}
    long PerformActionAndWait(const CCombatActionBase*,bool); // 0097DA40
    void Terminate(); // 008FCF40
    void Update();
};
void CJumpAtFleeingMethods::Update() {
    long fps=g_framesPerSecond;
    WaitUntilWorldFrameForNextAttempt=(long)(fps*5.0f+CWorldClock::GetCurrentFrame());
    if(IsAlive(Target())) {
        CThingCreatureBase* creature=(CThingCreatureBase*)Brain()->Creature;
        long result=PerformActionAndWait(&CCreatureAction_BalverineLungeAttack(creature,Target().GetPItem()),false);
        if(result==2 && IsAlive(Target())) {
            CThingCreatureBase* creature=(CThingCreatureBase*)Brain()->Creature;
            long result=PerformActionAndWait(&CCreatureAction_BalverineBreakOffFromCombatLong(creature,Target().GetPItem()),false);
            if(result==2) {Terminate();return;}
        }
    }
    Terminate();
}

class CThing : public CRetailThing { public:};
class CThingAICreature : public CThing { public:};
long g_framesPerSecond;
static long frameValue,expectedDeadline,firstResult,secondResult;
static int events[32],eventCount,actionCalls,modeAfterFirst;
static bool wrong;
static CJumpAtFleeingMethods* expectedState;
static CBaseIntelligentPointer* targetPointer;
static CThing* originalTarget;
static CAIBrain* expectedBrain;
static CThingAICreature *initialCreature,*refreshedCreature;
static CCombatActionBase* currentAction;
static void record(int e){if(eventCount>=32){wrong=true;return;}events[eventCount++]=e;}
long CWorldClock::GetCurrentFrame(){record(0);g_framesPerSecond=99;return frameValue;}
CThing* CBaseIntelligentPointer::GetPItem() const {
    if(this!=targetPointer || expectedState->WaitUntilWorldFrameForNextAttempt!=expectedDeadline)wrong=true;
    record(1);return Object;
}
void CCombatActionBase::v00(){}
CCombatActionBase::~CCombatActionBase(){
    if(this!=currentAction || !eventCount || (events[eventCount-1]!=3 && events[eventCount-1]!=6))wrong=true;
    record(4);currentAction=0;
}
CCreatureAction_BalverineLungeAttack::CCreatureAction_BalverineLungeAttack(CThingCreatureBase* creature,CThing* target){
    if(creature!=(CThingCreatureBase*)initialCreature || target!=originalTarget)wrong=true;
    currentAction=this;record(2);
}
CCreatureAction_BalverineBreakOffFromCombatLong::CCreatureAction_BalverineBreakOffFromCombatLong(CThingCreatureBase* creature,CThing* target){
    if(creature!=(CThingCreatureBase*)refreshedCreature || target!=originalTarget)wrong=true;
    currentAction=this;record(5);
}
long CJumpAtFleeingMethods::PerformActionAndWait(const CCombatActionBase* action,bool wait){
    if(this!=expectedState || action!=currentAction || wait)wrong=true;
    ++actionCalls;
    if(actionCalls==1){
        record(3);expectedBrain->Creature=refreshedCreature;
        if(modeAfterFirst==1)targetPointer->Object=0;
        if(modeAfterFirst==2)originalTarget->Dead=1;
        return firstResult;
    }
    if(actionCalls!=2)wrong=true;
    record(6);return secondResult;
}
void CJumpAtFleeingMethods::Terminate(){if(this!=expectedState||currentAction)wrong=true;record(9);}
static bool run(int initialTarget,int after,long first,long second,long fps,long frame){
    CJumpAtFleeingMethods state;std::memset(&state,0,sizeof(state));
    CAIBrain brain;std::memset(&brain,0,sizeof(brain));
    CThingAICreature creature,refreshed;
    CThing target;std::memset(&target,0,sizeof(target));
    CAIBrain* brainPointer=&brain;std::memcpy(state._pad_0x04,&brainPointer,sizeof(brainPointer));
    brain.Creature=&creature;targetPointer=(CBaseIntelligentPointer*)state.PTarget;
    targetPointer->Object=initialTarget?&target:0;target.Dead=initialTarget==2;
    expectedState=&state;expectedBrain=&brain;originalTarget=&target;
    initialCreature=&creature;refreshedCreature=&refreshed;
    g_framesPerSecond=fps;frameValue=frame;expectedDeadline=frame+fps*5;
    firstResult=first;secondResult=second;modeAfterFirst=after;
    eventCount=actionCalls=0;wrong=false;currentAction=0;
    state.Update();
    if(wrong || state.WaitUntilWorldFrameForNextAttempt!=expectedDeadline || currentAction) return false;
    int expected[32];int n=0;expected[n++]=0;expected[n++]=1;
    if(initialTarget){expected[n++]=1;
        if(initialTarget==1){
            expected[n++]=1;expected[n++]=2;expected[n++]=3;expected[n++]=4;
            if(first==2){
                expected[n++]=1;
                if(after!=1){expected[n++]=1;
                    if(after==0){expected[n++]=1;expected[n++]=5;expected[n++]=6;expected[n++]=4;}
                }
            }
        }
    }
    expected[n++]=9;
    return eventCount==n && !std::memcmp(events,expected,n*sizeof(int));
}
int main(){
    if(!run(0,0,2,2,15,100)||!run(2,0,2,2,60,-20))return 1;
    for(int after=0;after<3;++after)for(int first=0;first<=4;first+=2)for(int second=0;second<=4;second+=2)
        if(!run(1,after,first,second,15,100))return 2;
    if(!run(1,0,2,2,0,42)||!run(1,0,2,0,2,-30))return 3;
    std::puts("0094f730_TEST PASS");return 0;
}