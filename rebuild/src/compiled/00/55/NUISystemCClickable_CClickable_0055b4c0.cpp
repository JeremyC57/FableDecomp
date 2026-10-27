// NUISystem::CClickable::CClickable(const CClickable& other) -- copy constructor: copy the hoverable base, start a fresh clickable (click states and flags cleared, no click action lists) and reload its data from the def
// Leaf: ??0CClickable@NUISystem@@QAE@ABV01@@Z
// Retail: Fable.exe @ 0x0055B4C0, 96 bytes, thiscall `ret 4`.
// Identity (proven): stores CClickable's three vtables 0x0124BD2C / 0x0124BD08 / 0x0124BD00 (RTTI NUISystem::CClickable,
// direct base CHoverable) after CHoverable's copy constructor 0x0055BA80; Ego_r.pdb holds exactly two CClickable
// constructors, (long) = 0x0055B460 (landed, over CHoverable(long) 0x0055BA20) and this one; the CDraggable copy
// constructor 0x0055AA20 calls it (bound ??0CClickable@NUISystem@@QAE@ABV01@@Z).
// Callees: CHoverable(const CHoverable&) 0x0055BA80, CClickable::GetDataFromDef() 0x0055B040 (Ego_r.pdb
// ?GetDataFromDef@CClickable@NUISystem@@QAEXXZ: called only by the two constructors, reads the def through virtual
// slot 0x1B0 and copies its input delay into InputDelay +0x18C; reloc_bindings' InitialiseInputDelay@CClickable_Retail and
// the manifest's "SetupRumbleAction_Strong" are stand-ins).
// Shape: the base copy, the three vptrs, the eight members zeroed from one register in declaration order (the bools
// through al), GetDataFromDef(). InputDelay / LastInputTime are left to the def load.
// Layout: CClickable members at Ego_r.pdb offset - 0x30: StateBeforeLeftClicked +0x16C, WasLeftClicked +0x170,
// PActionsOnLeftClicked +0x174, PActionsOnLeftHeld +0x178, PActionsOnLeftUnclicked +0x17C, StateBeforeRightClicked +0x180,
// WasRightClicked +0x184, PActionsOnRightClicked +0x188, InputDelay +0x18C, LastInputTime +0x190 (size 0x194).
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
class CClickable : public CHoverable {
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
    CClickable(const CClickable& other);
    virtual ~CClickable();
    void GetDataFromDef();                                  // 0x0055B040
};

CClickable::CClickable(const CClickable& other)
    : CHoverable(other),
      StateBeforeLeftClicked(0),
      WasLeftClicked(false),
      PActionsOnLeftClicked(0),
      PActionsOnLeftHeld(0),
      PActionsOnLeftUnclicked(0),
      StateBeforeRightClicked(0),
      WasRightClicked(false),
      PActionsOnRightClicked(0)
{
    GetDataFromDef();
}
}