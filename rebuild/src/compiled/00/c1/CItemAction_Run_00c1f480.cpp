// CItemAction::Run() -- for an item of kind 3 only: rebuilds it as its target's type with bit 1 set (the old item detached, replaced through the factory, deleted, the new one attached to the dispatcher)
// Leaf: Run
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C1F480, 89 bytes (`ret`). Descriptive name (no PDB covers this library): the
//   manifest's "ActionHandler_ReplaceWithUnsheatheWeapons" is propagated. Caller: CItemDispatcher::RunAllItems
//   0x00C10400 (chunk 431: `CItemAction action(this, item); action.Run();`).
// Callees: the item's GetKind slot 4 (+0x10), GetTarget slot 5 (+0x14) and the target's GetType slot 0, the item's
//   Detach slot 7 (+0x1C), scalar deleting destructor slot 8 (+0x20) and Attach slot 6 (+0x18); CFactory::Create
//   @ 0x00C1EFC0 (`__fastcall(type, source)`, landed).
// Shape: the same inline ReplaceItem tail as SetTypeBits 0x00C1F320; edi is pushed only inside the kind-3 arm.
struct CItemDispatcher;

struct CItemTarget {
    virtual unsigned long GetType();                    // slot 0
};

struct CItem {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual unsigned long GetKind();                    // slot 4 (+0x10)
    virtual CItemTarget* GetTarget();                   // slot 5 (+0x14)
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
    void Run();
};

void CItemAction::Run()
{
    if (m_pItem->GetKind() == 3) {
        unsigned long type = m_pItem->GetTarget()->GetType() | 2;
        ReplaceItem(type);
    }
}