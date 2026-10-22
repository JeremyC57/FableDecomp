// NUISystem::CMenuEntry::~CMenuEntry() -- unregister from the front-end manager, hand every UI action parameter and thing action parameter back to the manager, delete the owned UI action parameters and both parameter lists, delete the six action lists, then run the CClickable base destructor
// Leaf: ??1CMenuEntry@NUISystem
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0053E4E0, length 606 bytes (+ int3 padding); ends in a tail jump to the base dtor.
// Callees: CFrontEndManager::GetInstance 0x0041E5F2 (__fastcall) and its virtual slots 5 (+0x14, remove the input
//   observer), 25 (+0x64, release a thing action parameter) and 28 (+0x70, release a UI action parameter); free 0x00BFEA14
//   (STLport list nodes and header), operator delete 0x00BFE9BC, list<CCountedPointer<CAction> >::clear 0x005356A0,
//   CClickable::~CClickable 0x0055B760 (manifest "CHoverable_destructor").
// Vtables 0x0124662C / 0x01246604 / 0x012465FC at +0 / +4 / +0x18 (as the landed ctor 0x0053DFE0).
// Layout: the landed ctor 0x0053DFE0 model (retail = PDB - 0x30): PUIActionParams +0x19c (list<CActionParam_*>*),
//   PThingActionParams +0x1a0 (list<long>*), PActions +0x1a4, PActionsOnSelected +0x1a8, PActionsOnBack +0x1ac,
//   PPreActions +0x1b0, PActionsOnUnselected +0x1b4, PActionsOnVisible +0x1b8 (list<CCountedPointer<CAction> >*).
// Shape: the POD lists' clear() is inline (node walk, free, header relink); the counted-pointer list's clear is the
//   out-of-line 0x005356A0 as in the landed CSlider::~CSlider 0x0054B360. Only the thing-parameter list is zeroed after
//   its delete, so that delete sits in its own if. The owned-parameter delete is written `delete *it` after `(*it)->Type`
//   (a named local makes VC7.1 drop the delete null test after the dereference; retail keeps it).
extern "C" void __cdecl free(void* block);       // 0x00BFEA14
void operator delete(void* p);                   // 0x00BFE9BC

template <class T> struct _List_node {
    _List_node* _M_next;                         // +0
    _List_node* _M_prev;                         // +4
    T           _M_data;                         // +8
};
template <class T> struct _List_iterator {
    _List_node<T>* _M_node;
    _List_iterator(_List_node<T>* n) : _M_node(n) {}
    T& operator*() const { return _M_node->_M_data; }
    _List_iterator& operator++() { _M_node = _M_node->_M_next; return *this; }
    bool operator!=(const _List_iterator& x) const { return _M_node != x._M_node; }
};
template <class T> struct CPodList {             // STLport list<T> with a trivially destructible T
    _List_node<T>* _M_node;
    _List_iterator<T> begin() const { return _List_iterator<T>(_M_node->_M_next); }
    _List_iterator<T> end() const { return _List_iterator<T>(_M_node); }
    void clear()
    {
        _List_node<T>* cur = _M_node->_M_next;
        while (cur != _M_node) {
            _List_node<T>* tmp = cur;
            cur = cur->_M_next;
            free(tmp);
        }
        _M_node->_M_next = _M_node;
        _M_node->_M_prev = _M_node;
    }
    ~CPodList()
    {
        clear();
        if (_M_node)
            free(_M_node);
    }
};
template <class T> struct CRetailList {          // STLport list<CCountedPointer<T> >: the header-node pointer
    void* _M_node;
    void clear();                                // 0x005356A0
    ~CRetailList()
    {
        clear();
        if (_M_node)
            free(_M_node);
    }
};

namespace NUISystem {
struct CAction;
struct CActionParam_ {
    virtual ~CActionParam_();                    // slot 0: scalar deleting destructor
    long Type;                                   // +0x04 (1: not owned by the entry)
};
}

struct CObserver;
struct CFrontEndManager {
    static CFrontEndManager* __fastcall GetInstance();   // 0x0041E5F2
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void RemoveObserver(CObserver* observer);                          // slot 5 (+0x14)
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10();
    virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19(); virtual void v20();
    virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24();
    virtual void ReleaseThingActionParam(long param);                          // slot 25 (+0x64)
    virtual void v26(); virtual void v27();
    virtual void ReleaseUIActionParam(NUISystem::CActionParam_* param);        // slot 28 (+0x70)
};

namespace NUISystem {

struct CClickableBase0 { virtual void v00(); };
struct CClickableBase2 { virtual void v20(); };
}
struct CObserver { virtual void v10(); unsigned char _body[0x10]; };
namespace NUISystem {

struct CClickable : CClickableBase0, CObserver, CClickableBase2 {
    unsigned char _body[0x198 - 0x1c];
    ~CClickable();                                  // 0x0055B760
};

struct CMenuEntry : CClickable {
    bool                               IsAvailable;            // +0x198
    CPodList<CActionParam_*>*          PUIActionParams;        // +0x19c
    CPodList<long>*                    PThingActionParams;     // +0x1a0
    CRetailList<CAction>*              PActions;               // +0x1a4
    CRetailList<CAction>*              PActionsOnSelected;     // +0x1a8
    CRetailList<CAction>*              PActionsOnBack;         // +0x1ac
    CRetailList<CAction>*              PPreActions;            // +0x1b0
    CRetailList<CAction>*              PActionsOnUnselected;   // +0x1b4
    CRetailList<CAction>*              PActionsOnVisible;      // +0x1b8
    virtual void v00();
    virtual void v10();
    virtual void v20();
    ~CMenuEntry();
};

CMenuEntry::~CMenuEntry()
{
    CFrontEndManager::GetInstance()->RemoveObserver(this);
    if (PUIActionParams) {
        for (_List_iterator<CActionParam_*> it = PUIActionParams->begin(); it != PUIActionParams->end(); ++it)
            CFrontEndManager::GetInstance()->ReleaseUIActionParam(*it);
    }
    if (PThingActionParams) {
        for (_List_iterator<long> it = PThingActionParams->begin(); it != PThingActionParams->end(); ++it)
            CFrontEndManager::GetInstance()->ReleaseThingActionParam(*it);
    }
    if (PUIActionParams) {
        for (_List_iterator<CActionParam_*> it = PUIActionParams->begin(); it != PUIActionParams->end(); ++it) {
            if ((*it)->Type != 1)
                delete *it;
        }
        delete PUIActionParams;
    }
    if (PThingActionParams) {
        delete PThingActionParams;
        PThingActionParams = 0;
    }
    delete PActions;
    delete PActionsOnBack;
    delete PPreActions;
    delete PActionsOnSelected;
    delete PActionsOnUnselected;
    delete PActionsOnVisible;
}

}