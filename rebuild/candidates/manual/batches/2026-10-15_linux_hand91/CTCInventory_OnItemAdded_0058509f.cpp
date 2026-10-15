#pragma optimize("y", off)
#pragma optimize("s", on)
// CTCInventory::OnItemAdded(long item, bool assignQuickAccess, bool silent) -- base handling, then (when asked) puts the new item into the next free quick-access slot; flags the context-sensitive list for an update
// Leaf: OnItemAdded
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0058509F, length 64 bytes; size-optimised TU with a frame pointer
// (`optimize("y",off)` + `("s",on)`).
// Callees: NInventory::CTCInventoryBase::OnItemAdded(long, bool, bool) 0x005BE028,
//          CTCInventory::GetQuickAccessNextFreeSlot(long) 0x00582027 (same TU, defined above),
//          CTCInventory::SetThingAsQuickAccessItem(long, long, bool) 0x00584157.
// Ego_r PDB: ?OnItemAdded@CTCInventory@@UAEXJ_N0@Z (manifest: long, bool, bool).
// Retail layout: QuickAccessItems is the 12-byte {first,last,end} vector<long> at +0x148 and
// ContextSensitiveUpdate sits at +0x1d4 (PDB header +0x168 / +0x20c), so `this` is modelled locally.
// Shape: `ret 0Ch`; the base call keeps `this` in ecx; the slot search result -1 means "no free slot".
// Retail does NOT reload ecx before SetThingAsQuickAccessItem: GetQuickAccessNextFreeSlot never writes ecx,
// and VC7.1 knows that only when the callee's body was compiled EARLIER IN THE SAME TU (it sits at
// 0x00582027, before this function, in the retail CTCInventory object). So the slot search (the byte-exact
// body of 0x00582027, chunk row of its own) is defined here first; the landed symbol is OnItemAdded.
struct CLongVector {
    long* first;                                     // +0x00
    long* last;                                      // +0x04
    long* end_of_storage;                            // +0x08
    unsigned long size() const { return (unsigned long)(last - first); }
    long& operator[](unsigned long n) { return first[n]; }
};

struct CTCInventoryBase {
    unsigned char _pad_0x000[0x148];
    CLongVector   QuickAccessItems;                  // +0x148 (PDB +0x168; CTCInventory member, kept here for the layout)
    unsigned char _pad_0x154[0x1d4 - 0x154];
    void OnItemAdded(long item, bool assignQuickAccess, bool silent);   // 0x005BE028
};

struct CTCInventory : CTCInventoryBase {
    bool ContextSensitiveUpdate;                     // +0x1d4 (PDB +0x20c)
    long GetQuickAccessNextFreeSlot(long item);      // 0x00582027
    void SetThingAsQuickAccessItem(long item, long slot, bool silent);  // 0x00584157
    void OnItemAdded(long item, bool assignQuickAccess, bool silent);
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

void CTCInventory::OnItemAdded(long item, bool assignQuickAccess, bool silent)
{
    CTCInventoryBase::OnItemAdded(item, assignQuickAccess, silent);
    if (assignQuickAccess) {
        long slot = GetQuickAccessNextFreeSlot(item);
        if (slot != -1)
            SetThingAsQuickAccessItem(item, slot, false);
    }
    ContextSensitiveUpdate = true;
}
