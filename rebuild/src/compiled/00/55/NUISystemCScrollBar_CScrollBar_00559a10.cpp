// NUISystem::CScrollBar::~CScrollBar() -- scroll-bar thumb dtor: delete the two owned action lists (dragged down, then dragged up), then ~CDraggable
// Leaf: ??1CScrollBar@NUISystem@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00559A10, 115 bytes, tail jump to the base destructor.
// Identity: the vtable stores 0x0124B7E4 / 0x0124B7BC / 0x0124B7B4 carry RTTI `.?AVCScrollBar@NUISystem@@` (CUIInterface @0,
//   CObserver @4, CActionParam @0x18); rtti_bases: CScrollBar : CDraggable : CClickable : CHoverable : CChangingStateComponent;
//   Ego_r.pdb has `??1CScrollBar@NUISystem@@UAE@XZ`. The catalog label `CScrollableListWithButton::VectorDtor` is wrong (class and
//   kind: no flag test, this is the plain `??1`).
// Callees: std::_List_base<CCountedPointer<NUISystem::CAction> >::clear 0x005356A0 (out of line), free 0x00BFEA14, operator delete
//   0x00BFE9BC, NUISystem::CDraggable::~CDraggable 0x0055ABA0 (tail jump; landed, stores the CDraggable vtables).
// Layout: Ego_r.pdb CScrollBar (0x20C): PActionsOnDraggedUp +0x1DC, PActionsOnDraggedDown +0x1E0, DeltaBeforeAction +0x1E4,
//   DeltaForActionUp +0x1EC, DeltaForActionDown +0x1F4, AmountActionsUp/Down +0x1FC/+0x200, MaxAmountActionsUp/Down
//   +0x204/+0x208; retail 0x30 lower (+0x1AC ...). CDraggable (landed 0x0055ABA0): PositionBeforeDrag +0x194, WasDragged +0x19C,
//   PActionsOnDropped +0x1A0, PActionsOnDroppedNowhere +0x1A4, SnapBackIfDroppedNowhere +0x1A8, ChangeLayer +0x1A9.
// Shape: `delete PActionsOnDraggedDown; delete PActionsOnDraggedUp;` (retail frees +0x1B0 first): each delete inlines the
//   implicit ~list (STLport ~_List_base: out-of-line clear, free the header node) and operator delete.
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

class C2DVector { public:
    float X;                                                // +0x0
    float Y;                                                // +0x4
};
class CDraggable : public CClickable { public:
    C2DVector        PositionBeforeDrag;                    // +0x194
    bool             WasDragged;                            // +0x19C
    CActionListType* PActionsOnDropped;                     // +0x1A0
    CActionListType* PActionsOnDroppedNowhere;              // +0x1A4
    bool             SnapBackIfDroppedNowhere;              // +0x1A8
    bool             ChangeLayer;                           // +0x1A9
    virtual ~CDraggable();                                  // 0x0055ABA0
};
class CScrollBar : public CDraggable { public:
    CActionListType* PActionsOnDraggedUp;                   // +0x1AC
    CActionListType* PActionsOnDraggedDown;                 // +0x1B0
    C2DVector        DeltaBeforeAction;                     // +0x1B4
    C2DVector        DeltaForActionUp;                      // +0x1BC
    C2DVector        DeltaForActionDown;                    // +0x1C4
    unsigned long    AmountActionsUp;                       // +0x1CC
    unsigned long    AmountActionsDown;                     // +0x1D0
    unsigned long    MaxAmountActionsUp;                    // +0x1D4
    unsigned long    MaxAmountActionsDown;                  // +0x1D8
    virtual ~CScrollBar();
};

CScrollBar::~CScrollBar()
{
    delete PActionsOnDraggedDown;
    delete PActionsOnDraggedUp;
}
} // namespace NUISystem