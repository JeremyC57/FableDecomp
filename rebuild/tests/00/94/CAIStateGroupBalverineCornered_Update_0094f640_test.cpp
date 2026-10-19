#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __forceinline inline
#define FABLE_ENGINE_CAIBRAIN_H
#define FABLE_ENGINE_CAISTATEGROUP_BALVERINECORNERED_H
struct CThingAICreature;
struct CAIBrain { CThingAICreature* Creature; };
struct CAIStateGroup_BalverineCornered {
    unsigned char _pad_0x04[sizeof(void*)];
    unsigned char PTarget[2*sizeof(void*)];
    float LandPos_X,LandPos_Y,LandPos_Z;
};
#endif
// CAIStateGroup_BalverineCornered::Update() -- choose a lunge or sky jump for a live target, then terminate.
// Retail 0094F640, 236 bytes. State/brain use shared headers; retail CThing map is +44 (donor +48).
// Combat action base is 0x114 retail versus donor 0x12c; sky-jump EndPos is +114.
// The shared result is compared after both temporary actions are destroyed, then every path terminates.
// Address-of-temporary syntax follows VC7.1; g++ fixtures compile the identical body with -fpermissive.
#include "engine/CAIStateGroup_BalverineCornered.h"
#include "engine/CAIBrain.h"
struct CThing;
struct CThingCreatureBase;
struct CInterfaceMap {
    struct Entry { long Type; void* Component; };
    Entry *First,*Last;
    Entry* LowerBound(const long*); // 0040F020
};
struct CRetailThing {
    unsigned char _prefix[0x20];
    unsigned long UsingComponents[9];
    CInterfaceMap Components;
    unsigned char _beforeDead[0x91-0x4c];
    unsigned char Dead:1;
    template<class T> __forceinline bool GetInterface(T** out,long type) {
        if (UsingComponents[type>>5] & (1UL<<(type&31))) {
            long key=type;
            CInterfaceMap* map=&Components;
            CInterfaceMap::Entry* it=map->LowerBound(&key);
            if(it==map->Last || type<it->Type) it=map->Last;
            *out=(T*)it->Component;
            return true;
        }
        return false;
    }
};
struct CBaseIntelligentPointer {
    void* __vftable; CThing* Object;
    CThing* GetPItem() const; // 00A01B50
};
inline bool IsAlive(const CBaseIntelligentPointer& p) {
    if(p.GetPItem()!=0) { if(!((CRetailThing*)p.GetPItem())->Dead) return true; }
    return false;
}
struct CTCVolumeContainmentTracker {
    unsigned char _base[0xc];
    CBaseIntelligentPointer PThingInside;
    void Update(); // 00785620
};
struct C3DVector { float X,Y,Z; };
struct CCombatActionBase {
    virtual void v00();
    unsigned char _body[0x110];
    ~CCombatActionBase(); // 0062E900
};
struct CCreatureAction_BalverineLungeAttack : CCombatActionBase {
    CCreatureAction_BalverineLungeAttack(CThingCreatureBase*,CThing*); // 008CCC70
};
struct CCreatureAction_BalverineCorneredJumpIntoSky : CCombatActionBase {
    C3DVector* EndPos;
    CCreatureAction_BalverineCorneredJumpIntoSky(CThingCreatureBase*,CThing*,C3DVector*); // 008CCB80
};
struct CCorneredMethods : CAIStateGroup_BalverineCornered {
    CAIBrain* Brain() const {return *(CAIBrain* const*)_pad_0x04;}
    const CBaseIntelligentPointer& Target() const {return *(const CBaseIntelligentPointer*)PTarget;}
    long PerformActionAndWait(const CCombatActionBase*,bool); // 0097DA40
    void Terminate(); // 008FCF40
    void Update();
};
void CCorneredMethods::Update() {
    if(IsAlive(Target())) {
        // The owning state requires interface0x75; retail leaves the miss out-parameter unwritten.
        CTCVolumeContainmentTracker* tracker;
        ((CRetailThing*)Brain()->Creature)->GetInterface(&tracker,0x75);
        tracker->Update();
        long result;
        if(tracker->PThingInside.GetPItem()) {
            CThingCreatureBase* creature=(CThingCreatureBase*)Brain()->Creature;
            result=PerformActionAndWait(&CCreatureAction_BalverineLungeAttack(creature,Target().GetPItem()),false);
        } else {
            CThingCreatureBase* creature=(CThingCreatureBase*)Brain()->Creature;
            result=PerformActionAndWait(&CCreatureAction_BalverineCorneredJumpIntoSky(creature,Target().GetPItem(),(C3DVector*)&LandPos_X),false);
        }
        if(result==2) { Terminate(); return; }
    }
    Terminate();
}

struct CThing : CRetailThing {};
struct CThingAICreature : CThing {};
static int events[32], eventCount, targetGets, insideGets, actionCalls, terminateCalls;
static bool wrong, wantInside;
static long actionResult;
static CCorneredMethods* expectedState;
static CBaseIntelligentPointer* targetPointer;
static CTCVolumeContainmentTracker* expectedTracker;
static CInterfaceMap* expectedMap;
static CInterfaceMap::Entry* foundEntry;
static CThing* expectedTarget;
static CThingCreatureBase* expectedOwner;
static CCombatActionBase* currentAction;
static CAIBrain* expectedBrain;
static CThingAICreature* refreshedCreature;
static CThing insideThing;
static void record(int e) { if(eventCount>=32) {wrong=true;return;} events[eventCount++]=e; }
CThing* CBaseIntelligentPointer::GetPItem() const {
    if(this==targetPointer) {++targetGets;record(1);}
    else if(this==&expectedTracker->PThingInside) {++insideGets;record(4);}
    else wrong=true;
    return Object;
}
CInterfaceMap::Entry* CInterfaceMap::LowerBound(const long* key) {
    if(this!=expectedMap || *key!=0x75) wrong=true;
    record(2);return foundEntry;
}
void CTCVolumeContainmentTracker::Update() {
    if(this!=expectedTracker) wrong=true;
    record(3);PThingInside.Object=wantInside?&insideThing:0;
    // The state must re-read the owning creature after this callback.
    expectedBrain->Creature=refreshedCreature;
}
void CCombatActionBase::v00() {}
CCombatActionBase::~CCombatActionBase() {
    if(this!=currentAction || eventCount==0 || events[eventCount-1]!=7) wrong=true;
    record(8);currentAction=0;
}
CCreatureAction_BalverineLungeAttack::CCreatureAction_BalverineLungeAttack(CThingCreatureBase* owner,CThing* target) {
    if(!wantInside || owner!=expectedOwner || target!=expectedTarget) wrong=true;
    currentAction=this;record(5);
}
CCreatureAction_BalverineCorneredJumpIntoSky::CCreatureAction_BalverineCorneredJumpIntoSky(CThingCreatureBase* owner,CThing* target,C3DVector* end) {
    if(wantInside || owner!=expectedOwner || target!=expectedTarget || end!=(C3DVector*)&expectedState->LandPos_X) wrong=true;
    if(end->X!=3 || end->Y!=-4 || end->Z!=5) wrong=true;
    EndPos=end;currentAction=this;record(6);
}
long CCorneredMethods::PerformActionAndWait(const CCombatActionBase* action,bool wait) {
    if(this!=expectedState || action!=currentAction || wait) wrong=true;
    ++actionCalls;record(7);return actionResult;
}
void CCorneredMethods::Terminate() {
    if(this!=expectedState || currentAction) wrong=true;
    ++terminateCalls;record(9);
}
static bool run(int targetState,bool inside,long result,bool useSentinel) {
    CCorneredMethods state;std::memset(&state,0,sizeof(state));
    CAIBrain brain;std::memset(&brain,0,sizeof(brain));
    CThingAICreature creature, refreshed;std::memset(&creature,0,sizeof(creature));std::memset(&refreshed,0,sizeof(refreshed));
    CThing target;std::memset(&target,0,sizeof(target));
    CTCVolumeContainmentTracker tracker;std::memset(&tracker,0,sizeof(tracker));
    CInterfaceMap::Entry mapEntries[2];
    mapEntries[0].Type=useSentinel?0x76:0x75; mapEntries[0].Component=useSentinel?0:&tracker;
    mapEntries[1].Type=0;mapEntries[1].Component=&tracker;
    creature.UsingComponents[0x75>>5]=1UL<<(0x75&31);
    creature.Components.First=&mapEntries[0];creature.Components.Last=&mapEntries[1];
    brain.Creature=&creature;
    CAIBrain* brainPointer=&brain;std::memcpy(state._pad_0x04,&brainPointer,sizeof(brainPointer));
    targetPointer=(CBaseIntelligentPointer*)state.PTarget;
    targetPointer->Object=targetState?&target:0; target.Dead=targetState==2;
    state.LandPos_X=3;state.LandPos_Y=-4;state.LandPos_Z=5;
    expectedState=&state;expectedBrain=&brain;refreshedCreature=&refreshed;
    expectedTracker=&tracker;expectedMap=&creature.Components;foundEntry=&mapEntries[0];
    expectedTarget=&target;expectedOwner=(CThingCreatureBase*)&refreshed;
    wantInside=inside;actionResult=result;currentAction=0;
    eventCount=targetGets=insideGets=actionCalls=terminateCalls=0;wrong=false;
    state.Update();
    if(wrong || terminateCalls!=1 || currentAction) return false;
    if(targetState==0) return eventCount==2 && events[0]==1 && events[1]==9 && !actionCalls && !insideGets;
    if(targetState==2) return eventCount==3 && events[0]==1 && events[1]==1 && events[2]==9 && !actionCalls && !insideGets;
    const int expected[]={1,1,2,3,4,1,inside?5:6,7,8,9};
    return eventCount==10 && !std::memcmp(events,expected,sizeof(expected)) && targetGets==3 && insideGets==1 && actionCalls==1;
}
int main() {
    if(!run(0,false,0,false) || !run(2,true,2,false)) return 1;
    for(int mode=0;mode<2;++mode) for(int result=0;result<=4;result+=2) for(int sentinel=0;sentinel<2;++sentinel)
        if(!run(1,mode!=0,result,sentinel!=0)) return 2;
    std::puts("0094f640_TEST PASS");return 0;
}