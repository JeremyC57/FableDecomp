// CPanel::OnKey(unsigned long key) -- unless the panel's item set is busy: update the items, and if it is still not busy, activate the item the set finds for the key, then poll the set again
// Leaf: OnKey
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C11F00, 72 bytes (`ret 4`). Descriptive names (no PDB covers this library):
//   the manifest's "CTCHero_OnInventoryKeyPressed" is propagated. The only caller is 0x00C12A84.
// Callees: CPanelItemSet::IsBusy @ 0x00C21DF0 (three times; the last result is dropped), CPanel::UpdateItems
//   @ 0x00C11CB0, CPanelItemSet::FindItem @ 0x00C21E60, CPanelItem::Activate @ 0x00C21620.
// Shape: the item set address is kept in esi, `this` in edi; both busy exits share the epilogue.
struct CPanel;

struct CPanelItem {                             // 0xb4-byte element (state word at +0xac)
    long IsActive() const;                      // 0x00C20840 (`state != 0`, returned as a full register)
    bool IsHandled(CPanel* panel);              // 0x00C20C10
    void Activate(CPanel* panel);               // 0x00C21620
};

struct CPanelItemArray {                        // polymorphic array: count at +4, elements at +0x10
    void* m_vtable;                             // +0x00
    unsigned long m_count;                      // +0x04
    unsigned char _pad08[0x10 - 0x08];
    CPanelItem* m_pItems;                       // +0x10
    CPanelItem& operator[](unsigned long index);   // 0x00C13620 (bounds-checked, EH-framed)
};

struct CPanelItemSet {                          // +0x2c of the panel
    unsigned char _pad00[0x30];
    CPanelItemArray m_items;                    // +0x30 (panel +0x5c)
    long IsBusy();                              // 0x00C21DF0 (forwards to m_items virtual slot 1)
    CPanelItem* FindItem(unsigned long key);    // 0x00C21E60
};

struct CPanel {
    unsigned char _pad00[0x2c];
    CPanelItemSet m_itemSet;                    // +0x2c
    void UpdateItems();                         // 0x00C11CB0
    void OnKey(unsigned long key);
};

void CPanel::OnKey(unsigned long key)
{
    if (!m_itemSet.IsBusy()) {
        UpdateItems();
        if (!m_itemSet.IsBusy()) {
            CPanelItem* item = m_itemSet.FindItem(key);
            if (item)
                item->Activate(this);
            m_itemSet.IsBusy();
        }
    }
}