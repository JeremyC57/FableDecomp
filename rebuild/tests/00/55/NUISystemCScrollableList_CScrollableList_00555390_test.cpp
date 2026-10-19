// Behaviour test for retail 0x00555390: the candidate source below is embedded verbatim (from src/00555390.cpp),
// followed by dependency stubs/spies and a main that asserts on the observable effects.
// ===== candidate source =====
// NUISystem::CScrollableList::~CScrollableList() -- mark both scroll arrows for deletion using method 2 and an empty associated-parent list
// Leaf: ??1CScrollableList
// Retail00555390,174 bytes through ret0055543d; two following int3 bytes excluded.
// RTTI COL0133AC88/CHD0133AC78 confirms CUIInterface/CObserver/CActionParam bases.
// Secondary slot0 adjustors005562D0/E0 target deleting destructor005562F0.
// PDB CDeletion: Method+0,AssociatedParents+4; method2 value retained without inferred enum name.
// No shared CScrollableList/CChangingStateComponent header exists. Retail MI bases are +0,+4,+18;
// PDB arrow pointers +1a0/+1a4 shift to +170/+174 in shipped layout.
// Deletion value copy0042CD84, list destructor0042ABCA, changing-state destructor0052CCF0.
// The STLport list header node comes from the CRT malloc import thunk 0x00BFEA0E (not ::operator new).
// Local CObserver: engine/CObserver.h is the Ego_r donor (16-byte set, 0x18 bytes); the retail STLport set is 12 bytes,
// so the native CObserver is 0x14 bytes and the header does not fit.
#include <stddef.h>
extern "C" void* __cdecl malloc(size_t);                 // 0x00BFEA0E
namespace NUISystem {
struct CDeletionParents {
    struct Node { Node* Next; Node* Previous; void* Parent; };
    Node* Head;
    CDeletionParents() { Node* head=(Node*)malloc(sizeof(Node)); head->Next=head; head->Previous=head; Head=head; }
    CDeletionParents(const CDeletionParents&); //0042BF06
    ~CDeletionParents(); //0042ABCA
};
struct CDeletion {
    unsigned long Method;
    CDeletionParents AssociatedParents;
    CDeletion() { Method=2; }
    CDeletion(const CDeletion&); //0042CD84
};
struct CUIInterface { virtual ~CUIInterface() {} };
struct CObserver { virtual ~CObserver() {} unsigned char EventsToObserve[0xc]; bool PreventObservation; unsigned char ObserverPadding[3]; };
struct CActionParam { virtual ~CActionParam() {} };
struct CComponent : CUIInterface,CObserver,CActionParam {
    unsigned char ComponentState[0x134-0x1c];
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09();
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual void v13();
    virtual void v14();
    virtual void v15();
    virtual void v16();
    virtual void v17();
    virtual void v18();
    virtual void v19();
    virtual void v20();
    virtual void v21();
    virtual void v22();
    virtual void v23();
    virtual void v24();
    virtual void v25();
    virtual void v26();
    virtual void v27();
    virtual void v28();
    virtual void v29();
    virtual void v30();
    virtual void v31();
    virtual void v32();
    virtual void v33();
    virtual void v34();
    virtual void v35();
    virtual void v36();
    virtual void v37();
    virtual void v38();
    virtual void v39();
    virtual void v40();
    virtual void v41();
    virtual void v42();
    virtual void v43();
    virtual void v44();
    virtual void v45();
    virtual void v46();
    virtual void v47();
    virtual void v48();
    virtual void v49();
    virtual void v50();
    virtual void v51();
    virtual void v52();
    virtual void v53();
    virtual void v54();
    virtual void v55();
    virtual void v56();
    virtual void v57();
    virtual void v58();
    virtual void v59();
    virtual void v60();
    virtual void v61();
    virtual void SetDeletion(CDeletion); //slot62/00531E90
};
struct CChangingStateComponent : CComponent {
    unsigned char ChangingState[0x15c-0x134];
    ~CChangingStateComponent(); //0052CCF0
};
struct CScrollableList : CChangingStateComponent {
    unsigned char ListLayout[0x10];
    float ScrollTimer;
    CComponent* PUpArrow;
    CComponent* PDownArrow;
    bool ScrollUp,ScrollDown;
    virtual ~CScrollableList();
};
CScrollableList::~CScrollableList() {
    CDeletion deletion;
    if (PUpArrow) { PUpArrow->SetDeletion(deletion); PUpArrow=0; }
    if (PDownArrow) { PDownArrow->SetDeletion(deletion); PDownArrow=0; }
}
}
// ===== harness =====
#include <stdio.h>
#include <new>
namespace NUISystem {
static CScrollableList* active;
static int bad,calls,copies,destroyed,baseCalls,mask;
static CDeletionParents::Node* original;
CDeletionParents::CDeletionParents(const CDeletionParents& other) {
    ++copies;
    if (other.Head->Next!=other.Head || other.Head->Previous!=other.Head) bad=1;
    if (!original) original=other.Head;
    else if (original!=other.Head) bad=1;
    Head=(Node*)::operator new(sizeof(Node));Head->Next=Head;Head->Previous=Head;
}
CDeletionParents::~CDeletionParents() {
    if (Head->Next!=Head || Head->Previous!=Head) bad=1;
    ++destroyed;::operator delete(Head);
}
CDeletion::CDeletion(const CDeletion& other) : Method(other.Method),AssociatedParents(other.AssociatedParents) {}
CChangingStateComponent::~CChangingStateComponent() {
    ++baseCalls;
    if (active->PUpArrow || active->PDownArrow || destroyed!=copies+1) bad=1;
}
void CComponent::SetDeletion(CDeletion) { bad=1; }
void CComponent::v01() {}
void CComponent::v02() {}
void CComponent::v03() {}
void CComponent::v04() {}
void CComponent::v05() {}
void CComponent::v06() {}
void CComponent::v07() {}
void CComponent::v08() {}
void CComponent::v09() {}
void CComponent::v10() {}
void CComponent::v11() {}
void CComponent::v12() {}
void CComponent::v13() {}
void CComponent::v14() {}
void CComponent::v15() {}
void CComponent::v16() {}
void CComponent::v17() {}
void CComponent::v18() {}
void CComponent::v19() {}
void CComponent::v20() {}
void CComponent::v21() {}
void CComponent::v22() {}
void CComponent::v23() {}
void CComponent::v24() {}
void CComponent::v25() {}
void CComponent::v26() {}
void CComponent::v27() {}
void CComponent::v28() {}
void CComponent::v29() {}
void CComponent::v30() {}
void CComponent::v31() {}
void CComponent::v32() {}
void CComponent::v33() {}
void CComponent::v34() {}
void CComponent::v35() {}
void CComponent::v36() {}
void CComponent::v37() {}
void CComponent::v38() {}
void CComponent::v39() {}
void CComponent::v40() {}
void CComponent::v41() {}
void CComponent::v42() {}
void CComponent::v43() {}
void CComponent::v44() {}
void CComponent::v45() {}
void CComponent::v46() {}
void CComponent::v47() {}
void CComponent::v48() {}
void CComponent::v49() {}
void CComponent::v50() {}
void CComponent::v51() {}
void CComponent::v52() {}
void CComponent::v53() {}
void CComponent::v54() {}
void CComponent::v55() {}
void CComponent::v56() {}
void CComponent::v57() {}
void CComponent::v58() {}
void CComponent::v59() {}
void CComponent::v60() {}
void CComponent::v61() {}

struct Arrow : CComponent {
    int ID;
    Arrow(int id):ID(id) {}
    virtual void SetDeletion(CDeletion event) {
        ++calls;
        if(event.Method!=2 || event.AssociatedParents.Head==original ||
           event.AssociatedParents.Head->Next!=event.AssociatedParents.Head ||
           event.AssociatedParents.Head->Previous!=event.AssociatedParents.Head || baseCalls) bad=1;
        if (ID==1) {
            if (calls!=1 || active->PUpArrow!=this || ((mask&2)!=0)!=(active->PDownArrow!=0)) bad=1;
        } else if (active->PUpArrow || active->PDownArrow!=this || calls!=((mask&1)?2:1)) bad=1;
    }
};
static int run(int m) {
    Arrow up(1),down(2);
    CScrollableList* list=new CScrollableList;
    active=list;mask=m;bad=calls=copies=destroyed=baseCalls=0;original=0;
    list->PUpArrow=(m&1)?&up:0;list->PDownArrow=(m&2)?&down:0;
    list->ScrollTimer=7.25f;list->ScrollUp=true;list->ScrollDown=false;
    list->ListLayout[0]=37;list->ListLayout[15]=91;
    list->~CScrollableList();
    int expected=((m&1)!=0)+((m&2)!=0);
    int ok=!bad && calls==expected && copies==expected && destroyed==expected+1 && baseCalls==1;
    ::operator delete(list);active=0;return ok;
}
}
int main() {
    for(int m=0;m<4;++m) if(!NUISystem::run(m)) {printf("00555390_TEST FAIL\n");return 1;}
    printf("00555390_TEST PASS\n");return 0;
}