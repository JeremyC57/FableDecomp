// NUISystem::CDraggable::CDraggable(const CDraggable& other) -- copy constructor: copy the clickable base, start a fresh draggable (zero pre-drag position, not dragged, no drop action lists, snap back if dropped nowhere, no layer change) and reload its data from the def
// Leaf: ??0CDraggable@NUISystem@@QAE@ABV01@@Z
// Retail: Fable.exe @ 0x0055AA20, 91 bytes, thiscall `ret 4`.
// Identity (proven): stores CDraggable's three vtables 0x0124BA94 / 0x0124BA70 / 0x0124BA68 (RTTI NUISystem::CDraggable,
// direct base CClickable) after CClickable's copy constructor 0x0055B4C0; Ego_r.pdb holds exactly two CDraggable
// constructors, (long) = 0x0055A9C0 (landed, over CClickable(long) 0x0055B460) and this one.
// Callees: CClickable(const CClickable&) 0x0055B4C0, CDraggable::GetDataFromDef() 0x0055A890 (Ego_r.pdb
// ?GetDataFromDef@CDraggable@NUISystem@@QAEXXZ: called only by the two constructors, reads the def through virtual slot
// 0x1B0; reloc_bindings' RegisterDragging@CDraggable_Retail and the manifest's "SetupRumbleAction_Weak" are stand-ins).
// Shape: the base copy, the three vptrs, then the members in declaration order from one zero register:
// PositionBeforeDrag = C2DVector(0, 0) (two integer zero stores), WasDragged, the two list pointers,
// SnapBackIfDroppedNowhere = true (`mov byte ptr [esi+1A8h],1`), ChangeLayer = false, then GetDataFromDef().
// Layout: CDraggable members at Ego_r.pdb offset - 0x30: PositionBeforeDrag +0x194, WasDragged +0x19C,
// PActionsOnDropped +0x1A0, PActionsOnDroppedNowhere +0x1A4, SnapBackIfDroppedNowhere +0x1A8, ChangeLayer +0x1A9 (size 0x1AC).
class C2DVector {
public:
    float X;
    float Y;
    C2DVector() { }
    C2DVector(float x, float y) : X(x), Y(y) { }
};

template <class T> class CCountedPointer;
namespace std {
template <class _Tp> class allocator;
template <class _Tp, class _Alloc> class list;
}

namespace NUISystem {
class CUIInterface {                                        // RTTI base @0 (primary vtable)
public:
    virtual ~CUIInterface();
};
class CObserver {                                           // RTTI base @4, 0x14 bytes
public:
    virtual ~CObserver();
    unsigned char EventsToObserve[0xc];                     // +0x04 set<NUISystem::EEvent> (STLport, 12 bytes)
    bool PreventObservation;                                // +0x10
};
class CActionParam {                                        // RTTI base @0x18, 8 bytes
public:
    virtual ~CActionParam();
    long ActionParamType;                                   // +0x04 EActionParamType
};
class CComponent : public CUIInterface, public CObserver, public CActionParam {   // partial view
public:
    CComponent(const CComponent& other);
    unsigned char ComponentBody[0x15c - 0x20];              // CComponent / CChangingStateComponent members (not modelled)
};
class CChangingStateComponent : public CComponent {         // partial view, 0x15C bytes
public:
    CChangingStateComponent(const CChangingStateComponent& other);   // 0x0052CCA0
};
class CAction;
class CHoverable : public CChangingStateComponent {         // partial view, 0x16C bytes
public:
    long StateBeforeHovered;                                // +0x15C
    bool WasHovered;                                        // +0x160
    std::list<CCountedPointer<CAction>, std::allocator<CCountedPointer<CAction> > >* PActionsOnHovered;     // +0x164
    std::list<CCountedPointer<CAction>, std::allocator<CCountedPointer<CAction> > >* PActionsOnUnhovered;   // +0x168
    CHoverable(const CHoverable& other);                    // 0x0055BA80
};
class CClickable : public CHoverable {                       // partial view, 0x194 bytes
public:
    long StateBeforeLeftClicked;                            // +0x16C
    bool WasLeftClicked;                                    // +0x170
    std::list<CCountedPointer<CAction>, std::allocator<CCountedPointer<CAction> > >* PActionsOnLeftClicked;    // +0x174
    std::list<CCountedPointer<CAction>, std::allocator<CCountedPointer<CAction> > >* PActionsOnLeftHeld;       // +0x178
    std::list<CCountedPointer<CAction>, std::allocator<CCountedPointer<CAction> > >* PActionsOnLeftUnclicked;  // +0x17C
    long StateBeforeRightClicked;                           // +0x180
    bool WasRightClicked;                                   // +0x184
    std::list<CCountedPointer<CAction>, std::allocator<CCountedPointer<CAction> > >* PActionsOnRightClicked;   // +0x188
    float InputDelay;                                       // +0x18C
    float LastInputTime;                                    // +0x190
    CClickable(const CClickable& other);                    // 0x0055B4C0
};
class CDraggable : public CClickable {
public:
    C2DVector PositionBeforeDrag;                           // +0x194
    bool WasDragged;                                        // +0x19C
    std::list<CCountedPointer<CAction>, std::allocator<CCountedPointer<CAction> > >* PActionsOnDropped;          // +0x1A0
    std::list<CCountedPointer<CAction>, std::allocator<CCountedPointer<CAction> > >* PActionsOnDroppedNowhere;   // +0x1A4
    bool SnapBackIfDroppedNowhere;                          // +0x1A8
    bool ChangeLayer;                                       // +0x1A9
    CDraggable(const CDraggable& other);
    virtual ~CDraggable();
    void GetDataFromDef();                                  // 0x0055A890
};

CDraggable::CDraggable(const CDraggable& other)
    : CClickable(other),
      PositionBeforeDrag(0.0f, 0.0f),
      WasDragged(false),
      PActionsOnDropped(0),
      PActionsOnDroppedNowhere(0),
      SnapBackIfDroppedNowhere(true),
      ChangeLayer(false)
{
    GetDataFromDef();
}
}