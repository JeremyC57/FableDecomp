// NUISystem::CClickable::~CClickable() -- delete the four owned action lists (left-clicked, right-clicked, left-held, left-unclicked: each an STLport list<CCountedPointer<CAction> >), then ~CHoverable
// Leaf: ??1CClickable
// Retail: Fable.exe (TLC, VC7.1) @ 0x0055B760, 197 bytes. Callers: the ??_G 0x0055B850 (slot 0 of the CClickable vtable
//   0x0124BD2C) and the derived clickables' destructors (four calls, four tail jumps: 0x0053E739, 0x0054DF47, 0x00558697 ..).
// Identity: the stores are the three CClickable vtables 0x0124BD2C (+0), 0x0124BD08 (+4, the CObserver subobject) and
//   0x0124BD00 (+0x18, CActionParam) -- RTTI .?AVCClickable@NUISystem@@ : CHoverable : CChangingStateComponent : CComponent :
//   CUIInterface, CObserver (mdisp 4), CActionParam (mdisp 0x18); Ego_r.pdb ??1CClickable@NUISystem@@UAE@XZ (user-declared:
//   the stores stay). The tail jump is ~CHoverable 0x0055BC30 (landed, ??1CHoverable@NUISystem@@UAE@XZ).
// Callees: std::_List_base<CCountedPointer<NUISystem::CAction> >::clear 0x005356A0 (kept out of line by VC7.1 from the _list.c
//   body in this TU, as retail; bound spelling ?clear@?$_List_base@V?$CCountedPointer@VCAction@NUISystem@@@@..@std@@QAEXXZ);
//   free 0x00BFEA14 (the list sentinel); operator delete 0x00BFE9BC (the list object); ~CHoverable 0x0055BC30.
// Layout (Ego_r.pdb less the retail container growth: CHoverable 0x19C -> 0x16C): StateBeforeLeftClicked +0x16C,
//   WasLeftClicked +0x170, PActionsOnLeftClicked +0x174, PActionsOnLeftHeld +0x178, PActionsOnLeftUnclicked +0x17C,
//   StateBeforeRightClicked +0x180, WasRightClicked +0x184, PActionsOnRightClicked +0x188, InputDelay, LastInputTime.
// Shape: the body deletes in retail's order LeftClicked, RightClicked, LeftHeld, LeftUnclicked; each `delete` is the null
//   test, the inline ~list (= ~_List_base: clear() + free of the sentinel), then operator delete.
// Label: manifest "_global::CHoverable_destructor" named the base; this is the derived CClickable destructor.
// Header debt: engine/CHoverable.h / CObserver.h are flattened (vptr words + pads); the NUISystem chain is a local
//   polymorphic model with the RTTI base order (only the subobject offsets +4 / +0x18 and the CClickable members matter here).
extern "C" void __cdecl free(void* block);                                       // 0x00BFEA14
#include "engine/CCountedPointer.h"

namespace std {
template <class _Tp> class allocator {
public:
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;
};
template <class _Tp, class _Alloc> class _List_base {
public:
    typedef _List_node<_Tp> _Node;
    _STLP_alloc_proxy<_List_node_base*, _Node, allocator<_Node> > _M_node;
    ~_List_base()
    {
        clear();
        _M_node.deallocate((_Node*)_M_node._M_data, 1);
    }
    void clear();
};
template <class _Tp, class _Alloc>
void _List_base<_Tp, _Alloc>::clear()
{
    _List_node<_Tp>* __cur = (_List_node<_Tp>*)this->_M_node._M_data->_M_next;
    while (__cur != this->_M_node._M_data) {
        _List_node<_Tp>* __tmp = __cur;
        __cur = (_List_node<_Tp>*)__cur->_M_next;
        _Destroy(&__tmp->_M_data);
        this->_M_node.deallocate(__tmp, 1);
    }
    this->_M_node._M_data->_M_next = this->_M_node._M_data;
    this->_M_node._M_data->_M_prev = this->_M_node._M_data;
}
template <class _Tp, class _Alloc = allocator<_Tp> > class list : public _List_base<_Tp, _Alloc> {
};
} // namespace std

namespace NUISystem {
class CAction;
class CUIInterface {
public:
    virtual ~CUIInterface();
};
class CObserver {
public:
    virtual void Notify();
    unsigned char EventsToObserve[0xc];                                  // +0x04 set<EEvent>
    bool PreventObservation;                                             // +0x10
};
class CActionParam {
public:
    virtual void ActionParamSlot0();
};
class CComponent : public CUIInterface, public CObserver, public CActionParam {
public:
    virtual ~CComponent();
};
class CChangingStateComponent : public CComponent {
public:
    virtual ~CChangingStateComponent();                                  // 0x0052CCF0
};
class CHoverable : public CChangingStateComponent {
public:
    virtual ~CHoverable();                                               // 0x0055BC30
    unsigned char _members_0x1c[0x16c - 0x1c];                           // CComponent .. CHoverable state (not touched)
};
class CClickable : public CHoverable {
public:
    virtual ~CClickable();
    long StateBeforeLeftClicked;                                         // +0x16c
    bool WasLeftClicked;                                                 // +0x170
    std::list<CCountedPointer<CAction> >* PActionsOnLeftClicked;         // +0x174
    std::list<CCountedPointer<CAction> >* PActionsOnLeftHeld;            // +0x178
    std::list<CCountedPointer<CAction> >* PActionsOnLeftUnclicked;       // +0x17c
    long StateBeforeRightClicked;                                        // +0x180
    bool WasRightClicked;                                                // +0x184
    std::list<CCountedPointer<CAction> >* PActionsOnRightClicked;        // +0x188
    float InputDelay;                                                    // +0x18c
    float LastInputTime;                                                 // +0x190
};
CClickable::~CClickable()
{
    delete PActionsOnLeftClicked;
    delete PActionsOnRightClicked;
    delete PActionsOnLeftHeld;
    delete PActionsOnLeftUnclicked;
}
} // namespace NUISystem