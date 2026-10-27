// CTCInventory::GetInitialItems() const -- the inventory def's InitialInventoryStandardItems list (the items a new inventory starts with)
// Leaf: ?GetInitialItems@CTCInventory@@UBEABV?$vector@JV?$allocator@J@std@@@std@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00581CC9, 35 bytes, thiscall, plain `ret`.
// Identity (proven): slot 86 of vtable 0x01250DE4 (RTTI .?AVCTCInventory@@); Ego_r.pdb ?GetInitialItems@CTCInventory@@UBE...
//   (the only const virtual of the class returning a vector<long> reference); the manifest's vfunc_86 label is a placeholder.
//   The member is fixed by its offset: Ego_r.pdb CInventoryDef holds InitialInventoryClothingItems / Weapons / Stats /
//   Standard at +0x60/+0x70/+0x80/+0x90 (16-byte vectors); retail's def base is 4 bytes narrower (landed
//   PlayInventoryMovementSound 0x005BD33C: MovementSound +0x4C for PDB +0x50) and its vectors 12 bytes, giving
//   +0x5C/+0x68/+0x74/+0x80 -- this body reads +0x80.
// Shape (family M144, representative 0x00581CC9): `CDefPointer<const CInventoryDef> def = PDef; return def->InitialInventoryStandardItems;` --
//   the copy's add-ref and its release are both null-tested; the member address is formed before the release (CDefPointer copy/dtor __forceinline: /Os leaves them out of line otherwise).
//   `optimize("s", on)` (the add-ref / release are read-modify-write).
#pragma optimize("s", on)
namespace std {
template <class _Tp> class allocator { public: allocator() { } };
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
};
}
class CInventoryDef {
public:
    virtual void v000();
    virtual void Destroy() const;                         // slot 1 (+0x04)
    mutable long m_refCount;                              // +0x04
    unsigned char _pad08[0x5C - 0x08];
    std::vector<long> InitialInventoryClothingItems;      // +0x5C (PDB +0x60)
    std::vector<long> InitialInventoryWeaponsItems;       // +0x68 (PDB +0x70)
    std::vector<long> InitialInventoryStatsItems;         // +0x74 (PDB +0x80)
    std::vector<long> InitialInventoryStandardItems;      // +0x80 (PDB +0x90)
};
template <class T> class CDefPointer {
public:
    T* Object;
    __forceinline CDefPointer(const CDefPointer& other) : Object(other.Object)
    {
        if (Object)
            ++Object->m_refCount;
    }
    __forceinline ~CDefPointer()
    {
        if (Object && --Object->m_refCount == 0)
            Object->Destroy();
    }
    T* operator->() const { return Object; }
};
class CTCInventory {
public:
    virtual const std::vector<long>& GetInitialItems() const;
    unsigned char _pad04[0x68 - 0x04];
    CDefPointer<const CInventoryDef> PDef;                // +0x68 (CTCInventoryBase; PDB +0x70)
};
const std::vector<long>& CTCInventory::GetInitialItems() const
{
    CDefPointer<const CInventoryDef> def = PDef;
    return def->InitialInventoryStandardItems;
}