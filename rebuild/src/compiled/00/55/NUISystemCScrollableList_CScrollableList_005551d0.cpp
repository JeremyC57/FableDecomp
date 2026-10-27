// NUISystem::CScrollableList::CScrollableList(const CScrollableList& other) -- copy constructor: copy the changing-state component base, then start a fresh list (scroll timer 0, no arrow components, both arrows inactive)
// Leaf: ??0CScrollableList@NUISystem@@QAE@ABV01@@Z
// Retail: Fable.exe @ 0x005551D0, 71 bytes, thiscall `ret 4`.
// Identity (proven): stores CScrollableList's three vtables 0x0124A72C / 0x0124A708 / 0x0124A700 (RTTI
// NUISystem::CScrollableList, direct base CChangingStateComponent) after calling CChangingStateComponent's copy
// constructor 0x0052CCA0; Ego_r.pdb holds exactly two CScrollableList constructors, (long) and this
// ??0CScrollableList@NUISystem@@QAE@ABV01@@Z (the copy-style constructors of this hierarchy pass `other` only to the base).
// Callees: CChangingStateComponent(const CChangingStateComponent&) 0x0052CCA0.
// Shape: the base copy, then ScrollTimer / PUpArrow / PDownArrow zeroed through eax (the float shares the integer zero),
// the two arrow flags, and the three vptrs scheduled last; TopOfList / VisibleListItems / PositionOffset are left to
// Initialise.
// Layout: retail CChangingStateComponent is 0x15C bytes; CScrollableList members at Ego_r.pdb offset - 0x30: TopOfList
// +0x15C, VisibleListItems +0x160, PositionOffset +0x164, ScrollTimer +0x16C, PUpArrow +0x170, PDownArrow +0x174,
// UpArrowActive +0x178, DownArrowActive +0x179. No engine header (local polymorphic partial view).
class C2DVector {
public:
    float X;
    float Y;
};

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
class CScrollableList : public CChangingStateComponent {
public:
    unsigned long TopOfList;                                // +0x15C
    unsigned long VisibleListItems;                         // +0x160
    C2DVector PositionOffset;                               // +0x164
    float ScrollTimer;                                      // +0x16C
    CComponent* PUpArrow;                                   // +0x170
    CComponent* PDownArrow;                                 // +0x174
    bool UpArrowActive;                                     // +0x178
    bool DownArrowActive;                                   // +0x179
    CScrollableList(const CScrollableList& other);
    virtual ~CScrollableList();
};

CScrollableList::CScrollableList(const CScrollableList& other)
    : CChangingStateComponent(other),
      ScrollTimer(0.0f),
      PUpArrow(0),
      PDownArrow(0),
      UpArrowActive(false),
      DownArrowActive(false)
{
}
}