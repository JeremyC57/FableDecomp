// NInventory::CTCInventoryBase::GetInitialCategoryRepeatDelay() const -- the inventory def's initial category-movement repeat delay
// Leaf: ?GetInitialCategoryRepeatDelay@CTCInventoryBase@NInventory@@UBEMXZ
// Retail: Fable.exe @ 0x005BD404, 35 bytes. Catalog label CTCWeapon::GetAnimationSpeedValue is wrong (ICF propagation):
// the function is slot 42 (+0xa8) of NInventory::CTCInventoryBase (vtable 0x01253DBC) and of all nine derived inventory
// TCs (Clothing 0x01243614, Experience, Trade, Weapons, Stats, Abilities, Map, Magic, CTCInventory), and it reads
// this+0x68, not CTCWeapon's WeaponDef +0x0c. Slots 42..45 (0x005BD404/27/4A/6D, 35 bytes apart) read def +0x3c/+0x40/
// +0x44/+0x48, matching Ego_r.pdb's four contiguous 35-byte getters GetInitialCategoryRepeatDelay,
// GetContinuedCategoryRepeatDelay, GetInitialItemRepeatDelay, GetContinuedItemRepeatDelay (0x006202AD..0x00620316) and
// CInventoryDef's four repeat-delay floats (PDB +0x40..+0x4c; retail 4 lower, the same shift as CInventoryCategoryDef in 0x005BD1B3).
// Landed 0x0061A100 (CTCInventoryQuests slot 43) calls the +0x40 sibling as the base implementation.
// Layout: Ego_r.pdb CTCInventoryBase PDef (CDefPointer<const CInventoryDef>) at +0x70; retail +0x68 (8 bytes lower,
// two 12-byte STLport containers before it). Local model: no engine/ header for CTCInventoryBase.
// Opt: size TU (ebp frame, in-place `dec dword ptr [ecx+4]`, the float result spilled across the release call).
#pragma optimize("s", on)
struct CDefPointeeBase {
    virtual ~CDefPointeeBase();
    virtual void Delete() const;                         // slot 1 (+4)
    mutable unsigned long IVRefCount;                    // +0x04
    __forceinline void Release() const { if (--IVRefCount == 0) Delete(); }
};
template <class T> struct CDefPointer {
    T* Object;
    CDefPointer(const CDefPointer& p) : Object(p.Object) { if (Object) ++Object->IVRefCount; }
    __forceinline ~CDefPointer() { if (Object) Object->Release(); }
    T* operator->() const { return Object; }
};
struct CInventoryDef : CDefPointeeBase {
    unsigned char _pad08[0x34];                          // +0x08
    float         InitialCategoryMovementRepeatDelay;    // +0x3c
    float         ContinuedCategoryMovementRepeatDelay;  // +0x40
    float         InitialItemMovementRepeatDelay;        // +0x44
    float         ContinuedItemMovementRepeatDelay;      // +0x48
};
namespace NInventory {
class CTCInventoryBase {
public:
    virtual float GetInitialCategoryRepeatDelay() const;
    unsigned char                     _pad04[0x64];      // +0x04
    CDefPointer<const CInventoryDef>  PDef;              // +0x68
};
float CTCInventoryBase::GetInitialCategoryRepeatDelay() const
{
    CDefPointer<const CInventoryDef> def(PDef);
    return def->InitialCategoryMovementRepeatDelay;
}
}