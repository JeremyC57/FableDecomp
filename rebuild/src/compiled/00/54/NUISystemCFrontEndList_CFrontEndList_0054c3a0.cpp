// NUISystem::CFrontEndList::CFrontEndList(long defIndex) -- construct from a def index: the base constructor with it, then the class's own three vptrs
// Leaf: ??0CFrontEndList@NUISystem@@QAE@J@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0054C3A0, 39 bytes (+ int3 padding), thiscall, `ret 4`.
// Identity (proven): NUISystem::CFrontEndList::CFrontEndList(long defIndex); evidence: CManager::CreateComponent(long) 0x0041D21B news 0x1FC bytes and constructs them with its def index; the stored vtables 0x01249224 / 0x012491FC / 0x012491F4 are CFrontEndList's (RTTI: CFrontEndList -> CList -> CChangingStateComponent -> CComponent), the base call is CList(long) 0x0053B63E (which stores CList's vtables 0x012462E4..).
// Label: the manifest's "CComponentContainer" is propagated (family J097: five 39-byte forwarding constructors of different
//   component classes; retail has no identical-code folding).
// Callees: NUISystem::CList::CList(long) 0x0053B63E.
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
class CList : public CChangingStateComponent {                  // partial view
public:
    CList(long defIndex);
    CList(const CList& other);   // (long) 0x0053B63E
};
class CFrontEndList : public CList {                           // adds no members
public:
    CFrontEndList(long defIndex);
};

CFrontEndList::CFrontEndList(long defIndex)
    : CList(defIndex)
{
}
}