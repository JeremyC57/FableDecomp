// NInventory::CTCInventoryBase::GetContinuedCategoryRepeatDelay() const -- the inventory def's ContinuedCategoryMovementRepeatDelay
// Leaf: ?GetContinuedCategoryRepeatDelay@CTCInventoryBase@NInventory@@UBEMXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x005BD427, 35 bytes, thiscall, plain `ret`, ebp frame.
// Identity (proven): slot 43 of vtable 0x01253DBC (RTTI .?AVCTCInventoryBase@NInventory@@, inherited by all nine inventory
//   TCs); Ego_r.pdb ?GetContinuedCategoryRepeatDelay@CTCInventoryBase@NInventory@@UBEMXZ -- slots 42..45 are its four contiguous 35-byte getters
//   (the landed slot 42 GetInitialCategoryRepeatDelay 0x005BD404 reads +0x3C; this one reads +0x40 = PDB +0x44).
//   The manifest's "CTCWeapon::GetAnimationSpeedValue" is an ICF-style propagation.
// Shape (family M153, representative 0x005BD427): the landed 0x005BD404 body -- `CDefPointer<const CInventoryDef> def(PDef);
//   return def->ContinuedCategoryMovementRepeatDelay;` (null-tested add-ref, unchecked release, the float spilled across the release). `optimize("s", on)`.
#pragma optimize("s", on)
class CDefPointeeBase {
public:
    virtual ~CDefPointeeBase();
    virtual void Delete() const;                         // slot 1 (+4)
    mutable unsigned long IVRefCount;                    // +0x04
    __forceinline void Release() const { if (--IVRefCount == 0) Delete(); }
};
template <class T> class CDefPointer {
public:
    T* Object;
    CDefPointer(const CDefPointer& p) : Object(p.Object) { if (Object) ++Object->IVRefCount; }
    __forceinline ~CDefPointer() { if (Object) Object->Release(); }
    T* operator->() const { return Object; }
};
class CInventoryDef : public CDefPointeeBase {
public:
    unsigned char _pad08[0x34];                          // +0x08
    float         InitialCategoryMovementRepeatDelay;    // +0x3c (PDB +0x40)
    float         ContinuedCategoryMovementRepeatDelay;  // +0x40 (PDB +0x44)
    float         InitialItemMovementRepeatDelay;        // +0x44 (PDB +0x48)
    float         ContinuedItemMovementRepeatDelay;      // +0x48 (PDB +0x4c)
};
namespace NInventory {
class CTCInventoryBase {
public:
    virtual float GetContinuedCategoryRepeatDelay() const;
    unsigned char                     _pad04[0x64];      // +0x04
    CDefPointer<const CInventoryDef>  PDef;              // +0x68 (PDB +0x70)
};
float CTCInventoryBase::GetContinuedCategoryRepeatDelay() const
{
    CDefPointer<const CInventoryDef> def(PDef);
    return def->ContinuedCategoryMovementRepeatDelay;
}
}