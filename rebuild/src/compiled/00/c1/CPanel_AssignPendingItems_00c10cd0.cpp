// CPanel::AssignPendingItems() -- hand each pending entry of the list at +0x164 a free slot of the panel's item set until either runs out, then place the entries of the list at +0x194 until one refuses (negative result)
// Leaf: AssignPendingItems
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C10CD0, 112 bytes (`ret`). Descriptive names (no PDB covers this library): the
//   manifest's "NUISystem_CSpellContainerList_PopulateSpells" is propagated. Callers 0x00C1137C, 0x00C11FD0, 0x00C128FA;
//   the item set at +0x2c is the one of OnKey 0x00C11F00 / UpdateItems 0x00C11CB0.
// Callees: the typed lists' GetNext (virtual slot 12, +0x30), CPanelItemSet::GetFreeSlot @ 0x00C21E50 (forwards to
//   the set's +0x1f8 object, virtual slot 1), CPendingEntry::Assign @ 0x00C21750, CPlacedEntry::Place @ 0x00C219F0.
// Shape: the first loop is rotated (entry in esi, list in edi, set in ebp pushed only around it); the second keeps the
//   entry in eax and jumps back to one shared `test eax,eax`: a plain `while (placed)`, `for (;;)` or `for` with a
//   `return` all rotate it; the top-tested form needs the `__forceinline` early-return condition helper IsEntry.
struct CPanel;

struct CPendingEntry {
    void Assign(CPanel* panel, void* slot);     // 0x00C21750
};

struct CPlacedEntry {
    long Place(CPanel* panel);                  // 0x00C219F0
};

struct CPendingList {                           // typed list over an implementation object
    virtual void Slot0(); virtual void Slot1(); virtual void Slot2(); virtual void Slot3(); virtual void Slot4();
    virtual void Slot5(); virtual void Slot6(); virtual void Slot7(); virtual void Slot8(); virtual void Slot9();
    virtual void Slot10(); virtual void Slot11();
    virtual CPendingEntry* GetNext();           // slot 12 (+0x30)
    void* m_pImpl;                              // +4
    unsigned char m_impl[8];                    // +8
};

struct CPlacedList {                            // typed list over an implementation object
    virtual void Slot0(); virtual void Slot1(); virtual void Slot2(); virtual void Slot3(); virtual void Slot4();
    virtual void Slot5(); virtual void Slot6(); virtual void Slot7(); virtual void Slot8(); virtual void Slot9();
    virtual void Slot10(); virtual void Slot11();
    virtual CPlacedEntry* GetNext();            // slot 12 (+0x30)
    void* m_pImpl;                              // +4
    unsigned char m_impl[8];                    // +8
};

static __forceinline bool IsEntry(const CPlacedEntry* entry)
{
    if (entry == 0)
        return false;
    return true;
}

struct CPanelItemSet {                          // +0x2c of the panel
    void* GetFreeSlot();                        // 0x00C21E50
};

struct CPanel {
    unsigned char _pad00[0x2c];
    CPanelItemSet m_itemSet;                    // +0x2c
    unsigned char _pad2d[0x164 - 0x2d];
    CPendingList m_pending;                     // +0x164
    unsigned char _pad174[0x194 - 0x174];
    CPlacedList m_placed;                       // +0x194
    void AssignPendingItems();
};

void CPanel::AssignPendingItems()
{
    CPendingEntry* pending = m_pending.GetNext();
    while (pending) {
        void* slot = m_itemSet.GetFreeSlot();
        if (!slot)
            break;
        pending->Assign(this, slot);
        pending = m_pending.GetNext();
    }
    CPlacedEntry* placed = m_placed.GetNext();
    while (IsEntry(placed)) {
        if (placed->Place(this) < 0)
            break;
        placed = m_placed.GetNext();
    }
}