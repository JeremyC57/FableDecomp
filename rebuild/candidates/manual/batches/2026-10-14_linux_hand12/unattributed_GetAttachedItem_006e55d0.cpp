// _unattributed::GetAttachedItem() -- the thing held in the current slot: slot index -> item index -> intelligent pointer -> item
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006E55D0, length 37 bytes (+ int3 padding).
// Callees: CBaseIntelligentPointer::GetPItem() @ 0x00A01B50 (tail-called on the 8-byte pointer element).
// Layout (from the bytes; the manifest's doubled "CAnimateThing::CAnimateThing::GetAttachedItem" label does not
// fit the CAnimateThing layout): an array of 8-byte CIntelligentPointers at +0x08, an array of 16-byte slot
// records at +0x14 (item index at +4) and the current slot index at +0x20. Both indices are "none" when -1
// (`cmp eax,-1 ; jle`), giving the shared `xor eax,eax` tail.
struct CBaseObject;

struct CBaseIntelligentPointer {               // 8 bytes: vptr + item
    virtual void v0();
    void* m_pItem;                              // +0x04
    CBaseObject* GetPItem() const;              // 0x00A01B50
};

struct SAttachmentSlot {                        // 16 bytes
    long m_type;                                // +0x00
    long m_itemIndex;                           // +0x04 index into the pointer array, -1 = empty
    long m_reserved8;                           // +0x08
    long m_reservedC;                           // +0x0c
};

struct CAttachmentHolder_006e55d0 {
    unsigned char _pad0[8];
    CBaseIntelligentPointer* m_pItems;          // +0x08
    unsigned char _pad1[8];
    SAttachmentSlot* m_pSlots;                  // +0x14
    unsigned char _pad2[8];
    long m_currentSlot;                         // +0x20 -1 = none
    CBaseObject* GetAttachedItem() const;
};

CBaseObject* CAttachmentHolder_006e55d0::GetAttachedItem() const
{
    long slot = m_currentSlot;
    if (slot > -1) {
        long item = m_pSlots[slot].m_itemIndex;
        if (item > -1)
            return m_pItems[item].GetPItem();
    }
    return 0;
}
