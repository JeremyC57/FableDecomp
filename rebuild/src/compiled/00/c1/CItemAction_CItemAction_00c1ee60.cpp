// CItemAction::CItemAction(CItemDispatcher* dispatcher, long key) -- binds the action to the dispatcher's item for the key; when there is none, a new keyed item (factory type 0x10101) is created for the key and attached to the dispatcher
// Flags: /EHsc
// Leaf: ??0CItemAction
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C1EE60, 128 bytes (`ret 8`). Built under /EHsc. Descriptive name (no PDB covers
//   this library): the manifest's "CCreatureAction_WriteUnsheatheWeapon" is propagated. The {dispatcher, item} action of
//   chunk 431 (plain constructor 0x00C1EE00, Run 0x00C1F480); caller 0x00C2E567 (an event-trigger driver).
// Callees: the dispatcher's FindItem @ 0x00C10140 (thiscall, key; slot-17 keyed lookup), scalar operator new
//   @ 0x00BFEA1A, the keyed item's constructor @ 0x00C1F720 (landed CVtable_012b3498: key at +4), the new item's Attach
//   slot 6 (+0x18).
// Shape: both members are init-list stores (the dispatcher from the argument register, the lookup result tested as it
//   is stored); EH state 0 covers only the constructor call of the `new`, and the attach runs after state -1.
struct CItemDispatcher;

struct CItem {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05();
    virtual void Attach(CItemDispatcher* dispatcher);   // slot 6 (+0x18)
};

struct CKeyedItem : CItem {                 // 0x10 bytes, vtable 0x012B3498 (factory type 0x10101)
    long m_key;                             // +4
    unsigned char m_member[8];              // +8
    CKeyedItem(long key);                   // 0x00C1F720
};

struct CItemDispatcher {
    CItem* FindItem(long key);              // 0x00C10140
};

struct CItemAction {                        // 8-byte {dispatcher, item}
    CItemDispatcher* m_pDispatcher;         // +0
    CItem* m_pItem;                         // +4
    CItemAction(CItemDispatcher* dispatcher, long key);
};

CItemAction::CItemAction(CItemDispatcher* dispatcher, long key)
    : m_pDispatcher(dispatcher), m_pItem(dispatcher->FindItem(key))
{
    if (!m_pItem) {
        m_pItem = new CKeyedItem(key);
        m_pItem->Attach(m_pDispatcher);
    }
}