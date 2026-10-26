#pragma optimize("s", on)
// CTCInventory::GetContextSensitiveItemButtonIndex(long item) -- slot index of the item among the context-sensitive items, -1 when absent
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00581D4E, length 64 bytes; size-optimised TU ("s").
// Callees: none.
// Ego_r PDB: ?GetContextSensitiveItemButtonIndex@CTCInventory@@QAEJJ@Z.
// Retail layout: engine/CTCInventory.h is the wider Ego_r.pdb layout (ContextSensitiveItems at PDB +0x178,
// 16-byte vectors); the shipped object keeps ContextSensitiveItems as a 12-byte {first,last,end} vector<long>
// at +0x154, so `this` is modelled locally.
// Shape: a counted loop over `size()` (re-evaluated each pass) that the compiler strength-reduces to a
// walking element pointer (`add edx,4`); the not-found exit is `or eax,-1`.
struct CLongVector {
    long* first;                                     // +0x00
    long* last;                                      // +0x04
    long* end_of_storage;                            // +0x08
    unsigned long size() const { return (unsigned long)(last - first); }
    long& operator[](unsigned long n) { return first[n]; }
};

class CTCInventory { public:
    unsigned char _pad_0x000[0x154];
    CLongVector   ContextSensitiveItems;             // +0x154 (PDB +0x178)
    long GetContextSensitiveItemButtonIndex(long item);
};

long CTCInventory::GetContextSensitiveItemButtonIndex(long item)
{
    for (unsigned long i = 0; i < ContextSensitiveItems.size(); ++i) {
        if (ContextSensitiveItems[i] == item)
            return i;
    }
    return -1;
}