#pragma optimize("s", on)
// CTCInventory::GetQuickAccessItem(unsigned long index) -- item of a combined slot index: the context-sensitive items first, then the quick-access items
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00581E33, length 60 bytes; size-optimised TU ("s").
// Callees: none.
// Ego_r PDB: ?GetQuickAccessItem@CTCInventory@@QAEJK@Z.
// Retail layout: engine/CTCInventory.h is the wider Ego_r.pdb layout (QuickAccessItems +0x168,
// ContextSensitiveItems +0x178, 16-byte vectors); the shipped object keeps both as 12-byte {first,last,end}
// vector<long> at +0x148 / +0x154, so `this` is modelled locally.
// Shape: unsigned `index < size()` (`jae`); the two element reads share one tail (`mov eax,[ecx+eax*4]`), the
// context-sensitive branch only selecting the base; size() is recomputed for the subtraction.
struct CLongVector {
    long* first;                                     // +0x00
    long* last;                                      // +0x04
    long* end_of_storage;                            // +0x08
    unsigned long size() const { return (unsigned long)(last - first); }
    long& operator[](unsigned long n) { return first[n]; }
};

struct CTCInventory {
    unsigned char _pad_0x000[0x148];
    CLongVector   QuickAccessItems;                  // +0x148 (PDB +0x168)
    CLongVector   ContextSensitiveItems;             // +0x154 (PDB +0x178)
    long GetQuickAccessItem(unsigned long index);
};

long CTCInventory::GetQuickAccessItem(unsigned long index)
{
    if (index < ContextSensitiveItems.size())
        return ContextSensitiveItems[index];
    return QuickAccessItems[index - ContextSensitiveItems.size()];
}
