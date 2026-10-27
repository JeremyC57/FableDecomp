// CTCInventoryExperience::SetInventorySelectionState(NInventory::CTCInventoryBase::EInventorySelectionState state) -- store the new selection state, then run the base's (empty in this build) follow-up hook
// Leaf: ?SetInventorySelectionState@CTCInventoryExperience@@UAEXW4EInventorySelectionState@CTCInventoryBase@NInventory@@@Z
// Retail: Fable.exe @ 0x0056E59B, 15 bytes, thiscall `ret 4`; /Os TU (packed unaligned address).
// Identity (proven): CTCInventoryExperience vtable 0x0124F11C slot 46 (rtti-vtables.tsv). The base's slot 46 is the
// shared `ret 4` 0x005BC586 that ten inventory vtables inherit, among them NInventory::CTCInventoryBase's own
// 0x01253DBC; Ego_r.pdb names exactly one one-argument override of a base virtual for this class that a state store
// fits: ?SetInventorySelectionState@CTCInventoryExperience@@UAEXW4EInventorySelectionState@CTCInventoryBase@NInventory@@@Z,
// and the store goes to CTCInventoryBase::InventorySelectionState (Ego_r.pdb +0x20; retail +0x1C behind the 12-byte
// STLport Components vector).
// Callees: 0x005BC58B, an empty `ret` member (descriptive NInventory::CTCInventoryBase::EmptyHook_005bc58b; it sits with
// the inventory base's other empty bodies 0x005BC586 / 0x005BC589 / 0x005BC58A, called only from here).
// Shape: /Os keeps `call ; ret 4` for the void forwarder where /O2 tail-jumps.
// Layout: CTCBase (vptr, ParentThing +4, ToBeRemoved +8, IsInGlobalUpdateTCs +9; engine/CTCBase.h), then
// CTCInventoryBase's Components +0x0C, FirstRenderFrame +0x18, InventorySelectionState +0x1C. engine/CTCInventoryExperience.h
// flattens the base region into padding (no name at +0x1C), so the base chain is a local partial view (header debt).
#pragma optimize("s", on)
class CThing;
namespace NUISystem {
class CComponent;
}
namespace std {
template <class _Tp> class allocator {
public:
    allocator() { }
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {    // 12 bytes in retail (STLport 4.6)
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
};
}

class CTCBase {
public:
    virtual ~CTCBase();
    CThing* ParentThing;                                    // +0x04
    bool ToBeRemoved;                                       // +0x08
    bool IsInGlobalUpdateTCs;                               // +0x09
};

namespace NInventory {
class CTCInventoryBase : public CTCBase {                   // partial view
public:
    enum EInventorySelectionState { };
    std::vector<NUISystem::CComponent*> Components;         // +0x0C
    bool FirstRenderFrame;                                  // +0x18
    EInventorySelectionState InventorySelectionState;       // +0x1C
    virtual void SetInventorySelectionState(EInventorySelectionState state);
    void EmptyHook_005bc58b();                              // 0x005BC58B (descriptive)
};
}

class CTCInventoryExperience : public NInventory::CTCInventoryBase {   // partial view
public:
    virtual void SetInventorySelectionState(NInventory::CTCInventoryBase::EInventorySelectionState state);
};

void CTCInventoryExperience::SetInventorySelectionState(NInventory::CTCInventoryBase::EInventorySelectionState state)
{
    InventorySelectionState = state;
    EmptyHook_005bc58b();
}