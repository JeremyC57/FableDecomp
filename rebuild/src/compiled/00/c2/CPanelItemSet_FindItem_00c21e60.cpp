// CPanelItemSet::FindItem(unsigned long key) -- peek the first item of the set's pending queue and return it only when its +0x14 value is below the key (else null)
// Leaf: FindItem
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C21E60, 33 bytes (`ret 4`). Descriptive names (no PDB covers this middleware
//   library): the manifest's "CTCInventoryItem_GetFirstAvailableIndex" is propagated. Its only caller is
//   CPanel::OnKey 0x00C11F00 (chunk 431: `item = m_itemSet.FindItem(key); if (item) item->Activate(this);`), so
//   `this` is the panel's item set at +0x2C and the result is a CPanelItem.
// Callees: the pending queue member object at +0x198 (panel +0x1C4), virtual slot 11 (+0x2C): its first item or null.
// Shape: the queue is a member OBJECT (`mov eax,[ecx+198h] ; add ecx,198h ; call [eax+2Ch]`); the unsigned compare
//   `item->m_key < key` jumps straight to the shared `ret 4` with the item still in eax, the null and the
//   not-below cases share the `xor eax,eax`.

struct CPanelItem {                             // 0xB4-byte state-machine item
    unsigned char _pad00[0x14];
    unsigned long m_key;                        // +0x14
};

struct CPanelItemQueue {                        // polymorphic queue member of the item set
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10();
    virtual CPanelItem* GetFirst();             // slot 11 (+0x2C)
};

struct CPanelItemSet {                          // +0x2C of the panel
    unsigned char   _pad00[0x198];
    CPanelItemQueue m_pending;                  // +0x198
    CPanelItem* FindItem(unsigned long key);
};

CPanelItem* CPanelItemSet::FindItem(unsigned long key)
{
    CPanelItem* item = m_pending.GetFirst();
    if (item && item->m_key < key)
        return item;
    return 0;
}