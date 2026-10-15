#pragma optimize("s", on)
// CTCInventory::Copy(const CTCBase* other) -- copies the base inventory state, then the other inventory's quick-access assignments
// Leaf: Copy
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00583A6D, length 37 bytes; size-optimised TU ("s").
// Callees: NInventory::CTCInventoryBase::Copy(const CTCBase*) 0x005BFB96,
//          std::vector<long>::operator=(const vector&) 0x00454886 (out of line; manifest label
//          "CActiveFile::operator=" is a byte-identical propagation).
// Vtable slot 12 of CTCInventory (catalog label "vfunc_12"). Ego_r PDB: ?Copy@CTCInventory@@UAEXPBVCTCBase@@@Z
// (the manifest gives the base overload as (CTCInventoryBase*, CTCBase*)).
// Retail layout: QuickAccessItems is the 12-byte {first,last,end} vector<long> at +0x148 (PDB header +0x168),
// so `this` is modelled locally.
// Shape: the argument is downcast to CTCInventory with no adjustment (the inventory component is the primary
// base), so `&other->QuickAccessItems` is `add edi,148h`.
struct CLongVector {
    long* first;                                     // +0x00
    long* last;                                      // +0x04
    long* end_of_storage;                            // +0x08
    CLongVector& operator=(const CLongVector& other);    // 0x00454886
};

struct CTCBase {
    unsigned char _pad_0x000[0x148];
};

struct CTCInventoryBase : CTCBase {
    void Copy(const CTCBase* other);                 // 0x005BFB96
};

struct CTCInventory : CTCInventoryBase {
    CLongVector QuickAccessItems;                    // +0x148 (PDB +0x168)
    void Copy(const CTCBase* other);
};

void CTCInventory::Copy(const CTCBase* other)
{
    CTCInventoryBase::Copy(other);
    QuickAccessItems = static_cast<const CTCInventory*>(other)->QuickAccessItems;
}