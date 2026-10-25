// CItemAction::SetTypeBits(unsigned long typeBits) -- rebuilds the action's item as the factory type formed from the new variant/family bits (bits 8..31) and the item's own kind byte: the old item is detached, replaced through the factory, deleted, and the new one attached to the dispatcher
// Leaf: SetTypeBits
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C1F320, 81 bytes (`ret 4`). Descriptive name (no PDB covers this library): the
//   manifest's "Component_UpdateState" is propagated. Callers: the four type setters 0x00C1F380 / 0x00C1F3C0 /
//   0x00C1F400 / 0x00C1F440 (each changes one byte of the item's current type and calls this when it differs).
//   The {dispatcher, item} layout is chunk 431's CItemAction (constructor 0x00C1EE00, Run 0x00C1F480).
// Callees: the item's GetKind slot 4 (+0x10), Detach slot 7 (+0x1C), the scalar deleting destructor slot 8 (+0x20) and
//   Attach slot 6 (+0x18); CFactory::Create @ 0x00C1EFC0 (`__fastcall(type, source)`, landed).
// Shape: `(typeBits & 0xFFFFFF00) | kind` (`and ecx,0FFFFFF00h ; or edi,ecx`); `delete m_pItem` is the null test plus
//   the slot-8 call with 1; the new item is attached through the register it was stored from.
struct CItemDispatcher;

struct CItem {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual unsigned long GetKind();                    // slot 4 (+0x10)
    virtual void* GetTarget();                          // slot 5 (+0x14)
    virtual void Attach(CItemDispatcher* dispatcher);   // slot 6 (+0x18)
    virtual void Detach(CItemDispatcher* dispatcher);   // slot 7 (+0x1C)
    virtual ~CItem();                                   // slot 8 (+0x20)
};

struct CFactory {
    static CItem* __fastcall Create(unsigned long type, CItem* source);    // 0x00C1EFC0
};

struct CItemAction {                        // 8-byte {dispatcher, item}
    CItemDispatcher* m_pDispatcher;         // +0
    CItem* m_pItem;                         // +4
    void ReplaceItem(unsigned long type)
    {
        m_pItem->Detach(m_pDispatcher);
        CItem* item = CFactory::Create(type, m_pItem);
        delete m_pItem;
        m_pItem = item;
        m_pItem->Attach(m_pDispatcher);
    }
    void SetTypeBits(unsigned long typeBits);
};

void CItemAction::SetTypeBits(unsigned long typeBits)
{
    unsigned long type = m_pItem->GetKind() | (typeBits & 0xFFFFFF00);
    ReplaceItem(type);
}