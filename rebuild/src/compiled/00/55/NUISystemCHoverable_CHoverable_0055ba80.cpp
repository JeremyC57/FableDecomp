// NUISystem::CHoverable::CHoverable(const CHoverable& other) -- copy constructor: copy the changing-state component base, start a fresh hoverable (not hovered, no hover/unhover action lists) and register its observer with the UI manager
// Leaf: ??0CHoverable@NUISystem@@QAE@ABV01@@Z
// Retail: Fable.exe @ 0x0055BA80, 82 bytes, thiscall `ret 4`.
// Identity (proven): stores CHoverable's three vtables 0x0124BFB4 / 0x0124BF90 / 0x0124BF88 (RTTI NUISystem::CHoverable,
// direct base CChangingStateComponent) after CChangingStateComponent's copy constructor 0x0052CCA0; Ego_r.pdb holds
// exactly two CHoverable constructors, (long) = 0x0055BA20 (landed, same body over the (long) base 0x0052CC50) and this
// one; the CClickable copy constructor 0x0055B4C0 calls it.
// Callees: CChangingStateComponent(const CChangingStateComponent&) 0x0052CCA0, NUISystem::CManager::GetInstance() 0x0041E5F2
// (Ego_r.pdb ?GetInstance@CManager@NUISystem@@SIPAV12@XZ: it lazily news the 0xD0-byte object whose ctor 0x0041E3F6
// stores the CManager vtable 0x01230134), then CObservable vtable slot 2 = AddObserver (0x0055CA40, landed).
// Shape: the CObserver subobject address (`lea edi,[esi+4]`) is formed before the vptr stores; the members are zeroed in
// declaration order; the manager call ends the body.
// Layout: retail CChangingStateComponent is 0x15C bytes; CHoverable members at Ego_r.pdb offset - 0x30:
// StateBeforeHovered +0x15C, WasHovered +0x160, PActionsOnHovered +0x164, PActionsOnUnhovered +0x168 (size 0x16C).
// engine/CHoverable.h is a flattened all-padding header (no vptrs, no names): local polymorphic partial view.
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
enum EEvent { EVENT_NONE };
class CAction;
class CObservable {                                         // vtable 0x01230044
public:
    virtual void SetEvent(EEvent event);                     // slot 0
    virtual void SetOwnedEvent(CObserver* observer, EEvent event);   // slot 1
    virtual void AddObserver(CObserver* observer);          // slot 2, 0x0055CA40
};
class CManager : public CObservable {                       // vtable 0x01230134
public:
    static CManager* __fastcall GetInstance();              // 0x0041E5F2
};
class CHoverable : public CChangingStateComponent {
public:
    long StateBeforeHovered;                                // +0x15C
    bool WasHovered;                                        // +0x160
    std::list<CCountedPointer<CAction>, std::allocator<CCountedPointer<CAction> > >* PActionsOnHovered;     // +0x164
    std::list<CCountedPointer<CAction>, std::allocator<CCountedPointer<CAction> > >* PActionsOnUnhovered;   // +0x168
    CHoverable(const CHoverable& other);
    virtual ~CHoverable();
};

CHoverable::CHoverable(const CHoverable& other)
    : CChangingStateComponent(other),
      StateBeforeHovered(0),
      WasHovered(false),
      PActionsOnHovered(0),
      PActionsOnUnhovered(0)
{
    CManager::GetInstance()->AddObserver(this);
}
}