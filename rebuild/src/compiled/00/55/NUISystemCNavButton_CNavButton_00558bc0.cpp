// NUISystem::CNavButton::CNavButton(const NUISystem::CNavButton& other) -- copy construction: the base copy constructor (the CComponent level copies DefIndex), then the class's own three vptrs
// Leaf: ??0CNavButton@NUISystem@@QAE@ABV01@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00558BC0, 39 bytes (+ int3 padding), thiscall, `ret 4`.
// Identity (proven): NUISystem::CNavButton::CNavButton(const NUISystem::CNavButton& other); evidence: the stored vtables 0x0124B04C / 0x0124B024 / 0x0124B01C are CNavButton's (RTTI: CNavButton -> CClickable -> CHoverable -> CChangingStateComponent -> CComponent); its base call 0x0055B4C0 is the CClickable constructor of the copy chain 0x0055BA80 -> 0x0052CCA0 -> 0x00533720, whose CComponent level copies other.DefIndex (+0x130) where the (long) chain's 0x005334A0 stores its argument; the (long) twin is the landed CNavButton::CNavButton(long) 0x00558B90 right before it (base CClickable(long) 0x0055B460). Unreferenced in retail; both PDB name sets hold ??0CNavButton@NUISystem@@QAE@ABV01@@Z.
// Label: the manifest's "sub_00558bc0" is propagated (family J097: five 39-byte forwarding constructors of different
//   component classes; retail has no identical-code folding).
// Callees: NUISystem::CClickable::CClickable(const NUISystem::CClickable&) 0x0055B4C0.
// Shape: the base constructor, then the class's three vptrs (primary +0, CObserver +4, CActionParam +0x18); the class adds
//   no members.
// Layout: retail RTTI bases CUIInterface @0, CObserver @4 (vptr, the 12-byte STLport EventsToObserve set, PreventObservation:
//   engine/CObserver.h's 0x14 bytes), CActionParam @0x18 (vptr, ActionParamType). Local polymorphic partial views (the engine
//   headers for this chain are flattened data-only structs); the component bodies past the bases are not modelled.
namespace NUISystem {
class CUIInterface {                                        // RTTI base @0 (primary vtable)
public:
    virtual ~CUIInterface();
};
class CObserver {                                           // RTTI base @4, 0x14 bytes
public:
    virtual ~CObserver();
    unsigned char EventsToObserve[0xc];                    // +0x04 set<NUISystem::EEvent> (STLport, 12 bytes)
    bool PreventObservation;                               // +0x10
};
class CActionParam {                                        // RTTI base @0x18, 8 bytes (Ego_r.pdb)
public:
    virtual ~CActionParam();
    long ActionParamType;                                  // +0x04 EActionParamType
};
class CComponent : public CUIInterface, public CObserver, public CActionParam {   // partial view
public:
    CComponent(long defIndex);
    CComponent(const CComponent& other);
};
class CChangingStateComponent : public CComponent {                   // partial view
public:
    CChangingStateComponent(long defIndex);                                  // 0x0052CC50
    CChangingStateComponent(const CChangingStateComponent& other);           // 0x0052CCA0
};
class CHoverable : public CChangingStateComponent {             // partial view
public:
    CHoverable(long defIndex);
    CHoverable(const CHoverable& other);   // copy 0x0055BA80
};
class CClickable : public CHoverable {                          // partial view
public:
    CClickable(long defIndex);
    CClickable(const CClickable& other);   // copy 0x0055B4C0
};
class CNavButton : public CClickable {                           // adds no members
public:
    CNavButton(const CNavButton& other);
};

CNavButton::CNavButton(const CNavButton& other)
    : CClickable(other)
{
}
}