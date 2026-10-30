#pragma optimize("s", on)
// CTCInventory::GetQuickAccessItemButtonIndex(long item) -- slot index of the item among the quick-access items, -1 when absent
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00581FD1, length 86 bytes; size-optimised TU ("s").
// Callees: CTCBase::PeekPlayerManager 0x00686D50 (`return g_pGame->m_30;`), CPlayerManager::GetMainPlayer() const 0x00449960,
//          CItemList::GetCount 0x00445950 (`(m_7c8 - m_7c4) >> 3`) -- the same dropped range-check chain as
//          the landed CTCInventory::ValidateIndex (0x005819F2), whose model is reused here.
// Ego_r PDB: ?GetQuickAccessItemButtonIndex@CTCInventory@@QBEJJ@Z (const).
// Retail layout: engine/CTCInventory.h is the wider Ego_r.pdb layout (QuickAccessItems at PDB +0x168, 16-byte
// vectors); the shipped object keeps QuickAccessItems as a 12-byte {first,last,end} vector<long> at +0x148,
// so `this` is modelled locally.
// Shape: the count chain is a statement whose result is dropped (the callees are out of line); then a counted
// loop over `size()` strength-reduced to a walking element pointer, `or eax,-1` when not found.
struct CItemList {
    unsigned long GetCount();                        // 0x00445950
};

class CPlayer { public:
    unsigned char _pad_0x00[0x0c];
    CItemList*    m_pItems;                          // +0x0c
};

// CPlayerManager: the shared model engine/CPlayerManager.h (Ego_r.pdb CPlayerManager; the five accessors declared
//   there with their retail addresses: GetPlayer 0x004498C0 / 0x00449910, GetMainPlayer 0x00449970 / 0x00449960,
//   GetMainPlayerNumber 0x00449700).
#include "engine/CPlayerManager.h"

class CTCBase { protected:
    const CPlayerManager& PeekPlayerManager() const;   // 0x00686D50 (Ego_r.pdb CTCBase, protected)
};

struct CLongVector {
    long* first;                                     // +0x00
    long* last;                                      // +0x04
    long* end_of_storage;                            // +0x08
    unsigned long size() const { return (unsigned long)(last - first); }
    long& operator[](unsigned long n) const { return first[n]; }
};

class CTCInventory : public CTCBase { public:
    unsigned char _pad_0x000[0x148];
    CLongVector   QuickAccessItems;                  // +0x148 (PDB +0x168)
    long GetQuickAccessItemButtonIndex(long item) const;
};

long CTCInventory::GetQuickAccessItemButtonIndex(long item) const
{
    PeekPlayerManager().GetMainPlayer().m_pItems->GetCount();
    for (unsigned long i = 0; i < QuickAccessItems.size(); ++i) {
        if (QuickAccessItems[i] == item)
            return i;
    }
    return -1;
}