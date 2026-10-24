// CPanel::UpdateItems() -- walk the panel's item array (+0x5c, count +0x60): every active item that does not report itself handled is activated for the panel
// Leaf: UpdateItems
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C11CB0, 72 bytes (`ret`). Descriptive names (no PDB covers this library): the
//   manifest's "CTCInventory_ProcessKeyPress" is propagated. Callers: OnKey 0x00C11F00 (in this chunk) and the two
//   lock-guarded refreshers 0x00C12720 / 0x00C127C0, which then walk the same array themselves.
// Callees: CPanelItemArray::operator[] @ 0x00C13620 (bounds-checked; element size 0xB4), CPanelItem::IsActive
//   @ 0x00C20840, CPanelItem::IsHandled @ 0x00C20C10, CPanelItem::Activate @ 0x00C21620.
// Shape: the count is re-read every pass (`mov eax,[edi+60h] ; inc ebx ; cmp ebx,eax ; jb`), the array address kept
//   in ebp, the item in esi.
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
    void UpdateItems();
};

void CPanel::UpdateItems()
{
    for (unsigned long i = 0; i < m_itemSet.m_items.m_count; ++i) {
        CPanelItem& item = m_itemSet.m_items[i];
        if (item.IsActive()) {
            if (!item.IsHandled(this))
                item.Activate(this);
        }
    }
}