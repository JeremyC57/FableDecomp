// NUISystem::CScrollBarOutside::~CScrollBarOutside() -- scroll-bar track dtor: delete the two owned action lists (clicked above, clicked under the thumb), then ~CClickable
// Leaf: ??1CScrollBarOutside@NUISystem@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00559520, 115 bytes, tail jump to the base destructor.
// Identity: the three vtable stores 0x0124B54C / 0x0124B528 / 0x0124B520 all carry RTTI `.?AVCScrollBarOutside@NUISystem@@`
//   (CUIInterface @0, CObserver @4, CActionParam @0x18); rtti_bases: CScrollBarOutside : CClickable : CHoverable :
//   CChangingStateComponent; Ego_r.pdb has `??1CScrollBarOutside@NUISystem@@UAE@XZ`. The catalog label
//   `CScrollableList::VectorDtor` is wrong twice (class and kind: there is no flag test, this is the plain `??1`).
// Callees: std::_List_base<CCountedPointer<NUISystem::CAction> >::clear 0x005356A0 (out of line), free 0x00BFEA14 (the list's
//   header node), operator delete 0x00BFE9BC (the list object), NUISystem::CClickable::~CClickable 0x0055B760 (tail jump;
//   bound so by the landed CKeyRedefiner deleting destructor 0x00558B30).
// Layout: Ego_r.pdb CScrollBarOutside (0x1D0): PScrollBar +0x1C4, PActionsOnLeftClickedAbove +0x1C8, PActionsOnLeftClickedUnder
//   +0x1CC; retail is 0x30 lower (the Dinkumware containers inside CComponent): +0x194 / +0x198 / +0x19C. No engine header
//   exists for the NUISystem component chain (header debt), so the polymorphic bases are modelled here.
// Shape: the landed CDraggable::~CDraggable 0x0055ABA0 shape: `delete PActionsOnLeftClickedAbove; delete PActionsOnLeftClickedUnder;`
//   -- the first member is loaded and null-tested around the vtable stores; each delete inlines the implicit ~list
//   (STLport ~_List_base: out-of-line clear, then free the header node under its null test) and operator delete.
extern "C" void __cdecl free(void* block);                  // 0x00BFEA14
void __cdecl operator delete(void* block);                  // 0x00BFE9BC

class CCPPointerInfo { public:                              // CCountedPointer count block
    unsigned long RefCount;                                 // +0x00
    void (__fastcall* DeleteFunc)(void* data);              // +0x04
    void*         Data;                                     // +0x08
};
template <class T> class CCountedPointer { public:
    T*              Data;                                   // +0x00
    CCPPointerInfo* Info;                                   // +0x04
};

namespace std {
template <class _Tp> class allocator { public:
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Tp, class _Alloc> class _STLP_alloc_proxy : public _Alloc { public:
    _Tp _M_data;
};
struct _List_node_base {
    _List_node_base* _M_next;                               // +0x0
    _List_node_base* _M_prev;                               // +0x4
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;                                            // +0x8
};
template <class _Tp, class _Alloc> class _List_base { public:
    typedef _List_node<_Tp> _Node;
    _STLP_alloc_proxy<_Node*, allocator<_Node> > _M_node;   // +0x0: the header node
    ~_List_base()
    {
        clear();
        _M_node.deallocate(_M_node._M_data, 1);
    }
    void clear();
};
template <class _Tp, class _Alloc = allocator<_Tp> > class list : public _List_base<_Tp, _Alloc> { public:
};
} // namespace std

namespace NUISystem {
class CAction;
typedef std::list<CCountedPointer<CAction> > CActionListType;   // descriptive typedef only (no symbol); PDB member type list<CCountedPointer<NUISystem::CAction> >

class CUIInterface { public: virtual ~CUIInterface(); };                                 // vptr +0x00
class CObserver { public: virtual ~CObserver(); unsigned char _body[0x14 - 4]; };       // +0x04 (EventsToObserve set +0x08, PreventObservation +0x14)
class CActionParam { public: virtual ~CActionParam(); long ActionParamType; };           // +0x18, type +0x1C
class CComponent : public CUIInterface, public CObserver, public CActionParam { public:
    virtual ~CComponent();
    unsigned char _body[0x15c - 0x20];                      // +0x20 .. +0x15C: CComponent / CChangingStateComponent state
};
class CChangingStateComponent : public CComponent { public:
    virtual ~CChangingStateComponent();                     // 0x0052CCF0
};
class CHoverable : public CChangingStateComponent { public:
    unsigned char _body[0x16c - 0x15c];                     // +0x15C .. +0x16C (Ego_r CHoverable members, 0x30 lower)
    virtual ~CHoverable();
};
class CClickable : public CHoverable { public:
    long             StateBeforeLeftClicked;                // +0x16C
    bool             WasLeftClicked;                        // +0x170
    CActionListType* PActionsOnLeftClicked;                 // +0x174
    CActionListType* PActionsOnLeftHeld;                    // +0x178
    CActionListType* PActionsOnLeftUnclicked;               // +0x17C
    long             StateBeforeRightClicked;               // +0x180
    bool             WasRightClicked;                       // +0x184
    CActionListType* PActionsOnRightClicked;                // +0x188
    float            InputDelay;                            // +0x18C
    float            LastInputTime;                         // +0x190
    virtual ~CClickable();                                  // 0x0055B760
};

class CScrollBarOutside : public CClickable { public:
    CComponent*      PScrollBar;                            // +0x194
    CActionListType* PActionsOnLeftClickedAbove;            // +0x198
    CActionListType* PActionsOnLeftClickedUnder;            // +0x19C
    virtual ~CScrollBarOutside();
};

CScrollBarOutside::~CScrollBarOutside()
{
    delete PActionsOnLeftClickedAbove;
    delete PActionsOnLeftClickedUnder;
}
} // namespace NUISystem