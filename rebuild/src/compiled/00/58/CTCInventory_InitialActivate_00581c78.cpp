#pragma optimize("s", on)
// CTCInventory::InitialActivate() -- base activation, then clears every quick-access slot whose item type is no longer held
// Leaf: InitialActivate
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00581C78, length 81 bytes; size-optimised TU ("s").
// Callees: NInventory::CTCInventoryBase::InitialActivate() 0x005BF9E3,
//          CTCInventoryBase::GetNumberOfItemsOfTypeInInventory(long) 0x005BDF08.
// Vtable slot 5 of CTCInventory (catalog label "vfunc_5"). Ego_r PDB: ?InitialActivate@CTCInventory@@UAEXXZ.
// Retail layout: engine/CTCInventory.h is the wider Ego_r.pdb layout (QuickAccessItems at +0x168, 16-byte
// vectors); the shipped object keeps QuickAccessItems as a 12-byte {first,last,end} vector<long> at +0x148,
// so `this` is modelled locally.
// Shape: `size()` is re-evaluated every iteration (`sub ; sar 2 ; cmp ; jb`) and the element is re-indexed
// through `first` after the call (`and [eax+edi*4],0` is the /Os form of `= 0`).
struct CLongVector {
    long* first;                                     // +0x00
    long* last;                                      // +0x04
    long* end_of_storage;                            // +0x08
    unsigned long size() const { return (unsigned long)(last - first); }
    long& operator[](unsigned long n) { return first[n]; }
};

struct CTCInventoryBase {
    unsigned char _pad_0x000[0x148];
    void InitialActivate();                          // 0x005BF9E3
    long GetNumberOfItemsOfTypeInInventory(long type);   // 0x005BDF08
};

class CTCInventory : public CTCInventoryBase { public:
    CLongVector QuickAccessItems;                    // +0x148 (PDB +0x168)
    void InitialActivate();
};

void CTCInventory::InitialActivate()
{
    CTCInventoryBase::InitialActivate();
    for (unsigned long i = 0; i < QuickAccessItems.size(); ++i) {
        if (GetNumberOfItemsOfTypeInInventory(QuickAccessItems[i]) == 0)
            QuickAccessItems[i] = 0;
    }
}