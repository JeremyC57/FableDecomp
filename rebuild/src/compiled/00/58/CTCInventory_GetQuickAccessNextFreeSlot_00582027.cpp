#pragma optimize("s", on)
// CTCInventory::GetQuickAccessNextFreeSlot(long item) -- first empty quick-access slot, or -1 when none is free or the item is already assigned before it
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00582027, length 76 bytes; size-optimised TU ("s").
// Callees: none.
// Ego_r PDB: ?GetQuickAccessNextFreeSlot@CTCInventory@@QAEJJ@Z.
// Retail layout: engine/CTCInventory.h is the wider Ego_r.pdb layout (QuickAccessItems at PDB +0x168, 16-byte
// vectors); the shipped object keeps QuickAccessItems as a 12-byte {first,last,end} vector<long> at +0x148,
// so `this` is modelled locally.
// Shape: the result starts at -1 (`or eax,-1` before the loop); a zero slot stores the index and breaks, a
// positive slot equal to the item breaks with -1; the loop walks an element pointer (`add esi,4`).
struct CLongVector {
    long* first;                                     // +0x00
    long* last;                                      // +0x04
    long* end_of_storage;                            // +0x08
    unsigned long size() const { return (unsigned long)(last - first); }
    long& operator[](unsigned long n) { return first[n]; }
};

class CTCInventory { public:
    unsigned char _pad_0x000[0x148];
    CLongVector   QuickAccessItems;                  // +0x148 (PDB +0x168)
    long GetQuickAccessNextFreeSlot(long item);
};

long CTCInventory::GetQuickAccessNextFreeSlot(long item)
{
    long result = -1;
    for (unsigned long i = 0; i < QuickAccessItems.size(); ++i) {
        if (QuickAccessItems[i] == 0) {
            result = i;
            break;
        }
        if (QuickAccessItems[i] > 0 && QuickAccessItems[i] == item)
            break;
    }
    return result;
}