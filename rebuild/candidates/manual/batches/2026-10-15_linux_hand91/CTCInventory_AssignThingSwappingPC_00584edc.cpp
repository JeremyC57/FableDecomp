#pragma optimize("y", off)
#pragma optimize("s", on)
// CTCInventory::AssignThingSwappingPC(long item, unsigned long slot, long previousSlot, CComponent* component, CComponent* previousComponent) -- assigns an item to a quick-access slot and moves the slot's previous item to another slot
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00584EDC, length 57 bytes; size-optimised TU with a frame pointer
// (`optimize("y",off)` + `("s",on)`, `pop ebp`).
// Callees: CTCInventory::AssignThingPC(long, unsigned long, CComponent*) 0x00584B86 (twice).
// Ego_r PDB: ?AssignThingSwappingPC@CTCInventory@@QAEXJKJPAVCComponent@@0@Z (manifest: long, ulong, long,
// CComponent*, CComponent*).
// Retail layout: QuickAccessItems is the 12-byte {first,last,end} vector<long> at +0x148 (PDB header +0x168),
// so `this` is modelled locally.
// Shape: `ret 14h`; the slot's previous item is read into a named local (edi) between pushing the first call's
// arguments (VC interleaves the load with the right-to-left pushes); the second assignment only runs for a
// non-empty previous item.
struct CComponent;

struct CLongVector {
    long* first;                                     // +0x00
    long* last;                                      // +0x04
    long* end_of_storage;                            // +0x08
    long& operator[](unsigned long n) { return first[n]; }
};

struct CTCInventory {
    unsigned char _pad_0x000[0x148];
    CLongVector   QuickAccessItems;                  // +0x148 (PDB +0x168)
    void AssignThingPC(long item, unsigned long slot, CComponent* component);   // 0x00584B86
    void AssignThingSwappingPC(long item, unsigned long slot, long previousSlot, CComponent* component, CComponent* previousComponent);
};

void CTCInventory::AssignThingSwappingPC(long item, unsigned long slot, long previousSlot, CComponent* component, CComponent* previousComponent)
{
    long previousItem = QuickAccessItems[slot];
    AssignThingPC(item, slot, component);
    if (previousItem)
        AssignThingPC(previousItem, previousSlot, previousComponent);
}
