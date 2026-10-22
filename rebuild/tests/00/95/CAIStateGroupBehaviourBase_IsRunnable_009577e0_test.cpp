#include <cstdio>
#ifndef _MSC_VER
// Only the parent declaration is needed for pointer-based host behavior tests.
#define FABLE_ENGINE_CAISTATEGROUPBEHAVIOURBASE_H
#define FABLE_ENGINE_CTCPERCEIVETHING_H
struct CTCPerceiveThing {};
#endif
// CAIStateGroupBehaviourBase::IsRunnable() -- reset target, check behavior predicate, and acquire perceived target.
// Grade: RELOCATION_MATCH -- explicit reset temporary lifetime preserves retail target-address ordering.
#include "engine/CAIStateGroupBehaviourBase.h"
#include "engine/CTCPerceiveThing.h"
struct CThing;

struct CBaseIntelligentPointer {
    virtual void v00();
    void* m_pItem;                          // +4
    CBaseIntelligentPointer();              // 0x00A01B10
    CThing* GetPItem() const;               // 0x00A01B50
    void SetPItem(CThing* item);            // 0x00A01B90
    ~CBaseIntelligentPointer();             // 0x00A01C10
};

struct CIntelligentPointer : CBaseIntelligentPointer {    // vtable 0x01237F1C
    CIntelligentPointer(CThing* item=0) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }
};


struct CPerceptionMethods : CTCPerceiveThing { CThing* GetThing(); }; // 007446A0
struct CBehaviourVirtualView {
 virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
 virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
 virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
 virtual void v12(); virtual void v13(); virtual void v14(); virtual bool CanRun();
};
struct CThingRetailLife { unsigned char _beforeDead[0x91]; unsigned char Dead:1; };
// Typed view of the engine header's opaque pointer member; offsets remain unchanged.
struct CBehaviourMethods : CBehaviourVirtualView {
 unsigned char _base[0x10];
 CIntelligentPointer PTargetThing;
 CTCPerceiveThing* PTCPerception;
 inline CIntelligentPointer& Target() { return PTargetThing; }
 bool IsRunnable();
};
bool CBehaviourMethods::IsRunnable() {
 {
 CIntelligentPointer empty(0);
 CThing* item=empty.GetPItem();
 Target().SetPItem(item);
 }
 if (Target().GetPItem() && !((CThingRetailLife*)Target().GetPItem())->Dead &&
     ((CBehaviourVirtualView*)this)->CanRun()) {
  Target()=((CPerceptionMethods*)PTCPerception)->GetThing();
  return true;
 }
 return false;
}

static int constructed, destroyed, getCalls, setCalls, canRunCalls, perceptionCalls, errors;
static bool canRun;
static CBehaviourMethods* expectedState;
static CPerceptionMethods* expectedPerception;
static CThing* queriedTarget;
static CThing* perceivedTarget;
void CBaseIntelligentPointer::v00() {}
CBaseIntelligentPointer::CBaseIntelligentPointer():m_pItem(0) { ++constructed; }
CBaseIntelligentPointer::~CBaseIntelligentPointer() { ++destroyed; }
void CBaseIntelligentPointer::SetPItem(CThing* p) { ++setCalls; m_pItem=p; }
CThing* CBaseIntelligentPointer::GetPItem() const {
    ++getCalls;
    // Script the external query result independently of the reset operation so
    // every retail branch can be exercised without implementing engine services.
    if (this==&expectedState->PTargetThing) return queriedTarget;
    return (CThing*)m_pItem;
}
void CBehaviourVirtualView::v00() {} void CBehaviourVirtualView::v01() {}
void CBehaviourVirtualView::v02() {} void CBehaviourVirtualView::v03() {}
void CBehaviourVirtualView::v04() {} void CBehaviourVirtualView::v05() {}
void CBehaviourVirtualView::v06() {} void CBehaviourVirtualView::v07() {}
void CBehaviourVirtualView::v08() {} void CBehaviourVirtualView::v09() {}
void CBehaviourVirtualView::v10() {} void CBehaviourVirtualView::v11() {}
void CBehaviourVirtualView::v12() {} void CBehaviourVirtualView::v13() {}
void CBehaviourVirtualView::v14() {}
bool CBehaviourVirtualView::CanRun() {
    ++canRunCalls;
    if ((void*)this!=(void*)expectedState || destroyed!=1 || expectedState->PTargetThing.m_pItem!=0) ++errors;
    return canRun;
}
CThing* CPerceptionMethods::GetThing() {
    ++perceptionCalls;
    if (this!=expectedPerception || canRunCalls!=1 || destroyed!=1) ++errors;
    return perceivedTarget;
}
static int run(bool targetPresent,bool dead,bool allowed,bool perceptionPresent) {
    CBehaviourMethods state;
    CPerceptionMethods perception;
    CThingRetailLife oldThing = {}, newThing = {};
    oldThing.Dead=dead;
    state.PTCPerception=&perception;
    expectedState=&state; expectedPerception=&perception;
    queriedTarget=targetPresent?(CThing*)&oldThing:0;
    perceivedTarget=perceptionPresent?(CThing*)&newThing:0;
    canRun=allowed;
    constructed=destroyed=getCalls=setCalls=canRunCalls=perceptionCalls=errors=0;
    bool result=state.IsRunnable();
    bool accepted=targetPresent && !dead && allowed;
    if (result!=accepted || constructed!=(accepted?2:1) || destroyed!=(accepted?2:1) ||
        setCalls!=(accepted?4:2) || getCalls!=(accepted?4:(targetPresent?3:2)) ||
        canRunCalls!=(targetPresent && !dead?1:0) || perceptionCalls!=(accepted?1:0) ||
        state.PTargetThing.m_pItem!=(accepted?perceivedTarget:0)) ++errors;
    return errors;
}
int main() {
    if (run(false,false,true,true) || run(true,true,true,true) || run(true,false,false,true) ||
        run(true,false,true,true) || run(true,false,true,false)) {
        std::printf("009577e0_TEST FAIL\n"); return 1;
    }
    std::printf("009577e0_TEST PASS\n"); return 0;
}