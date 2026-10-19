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