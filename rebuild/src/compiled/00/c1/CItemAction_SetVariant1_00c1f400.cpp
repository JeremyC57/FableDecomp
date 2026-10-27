// CItemAction::SetVariant1() -- set byte 1 of the item target's packed type to 1 and apply it when it changed
// Leaf: ?SetVariant1@CItemAction@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00c1f400, 61 bytes, thiscall, plain `ret`.
// Identity (descriptive): unbound, no direct callers; it sets type byte 1 to 1 -- the value-1 sibling of SetVariant2 0x00C1F380, named in that descriptive scheme. libpk: no PDB, no RTTI; the names are the landed Handle's descriptive ones.
// Callees: the item's GetTarget (slot 5, +0x14) and the target's packed-type getter (slot 0), CItemAction::SetTypeBits
//   0x00C1F320 (thiscall, the new type).
// Shape (family L124): the type is fetched twice (once to build the new value, once to compare); VC7.1 folds the
//   byte replacement `(type & 0xffff00ff) | 0x100` into `and` with the value's bits kept, then `or`.
struct CItemTarget {
    virtual unsigned long GetType();                    // slot 0
};

struct CItem {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual CItemTarget* GetTarget();                   // slot 5 (+0x14)
};

struct CItemDispatcher;

struct CItemAction {                        // 8-byte {dispatcher, item}
    CItemDispatcher* m_pDispatcher;         // +0
    CItem* m_pItem;                         // +4
    void SetTypeBits(unsigned long type);   // 0x00C1F320
    void SetVariant1();
};

void CItemAction::SetVariant1()
{
    unsigned long type = (m_pItem->GetTarget()->GetType() & 0xffff00ff) | 0x100;
    if (m_pItem->GetTarget()->GetType() != type)
        SetTypeBits(type);
}