// Grade: RELOCATION_MATCH (Fable landing 2026-10-19 after the Codex review hold: wrong callee: ::operator new where retail calls the malloc import 0xBFEA0E)
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
class CDeletion { public:
    unsigned long Method;
    CDeletionParents AssociatedParents;
    CDeletion() { Method=2; }
    CDeletion(const CDeletion&); //0042CD84
};
struct CUIInterface { virtual ~CUIInterface() {} };
class CObserver { public: virtual ~CObserver() {} unsigned char EventsToObserve[0xc]; bool PreventObservation; unsigned char ObserverPadding[3]; };
class CActionParam { public: virtual ~CActionParam() {} };
class CComponent : public CUIInterface,public CObserver,public CActionParam { public:
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
class CChangingStateComponent : public CComponent { public:
    unsigned char ChangingState[0x15c-0x134];
    ~CChangingStateComponent(); //0052CCF0
};
class CScrollableList : public CChangingStateComponent { public:
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