// CItemAction::Handle(long a, long b) -- passes (a, b) to the item's target; on a negative result it resets the item's type (-1: the second setter, else the first) and asks the target again, returning that result
// Leaf: Handle
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C1F4E0, 76 bytes (`ret 8`). Descriptive name (no PDB covers this library): the
//   manifest's "CQueueProcessor_HandleProcessError" is propagated. Callers 0x00C2E57B / 0x00C2E75E / 0x00C2E8B2 (the
//   event-trigger drivers, which also build the actions 0x00C1EE60 / 0x00C1EEE0).
// Callees: the item's GetTarget slot 5 (+0x14) and the target's Handle slot 3 (+0xC); the type setters @ 0x00C1F3C0
//   (sets type byte 2 to 2) and @ 0x00C1F380 (sets type byte 1 to 2), both thiscall without arguments.
// Shape: a and b stay in ebx/edi for both target calls; the setter calls share one `mov ecx,esi`.
struct CItemTarget {
    virtual void v00(); virtual void v01(); virtual void v02();
    virtual long Handle(long a, long b);                // slot 3 (+0xC)
};

struct CItem {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual CItemTarget* GetTarget();                   // slot 5 (+0x14)
};

struct CItemDispatcher;

struct CItemAction {                        // 8-byte {dispatcher, item}
    CItemDispatcher* m_pDispatcher;         // +0
    CItem* m_pItem;                         // +4
    void SetVariant2();                     // 0x00C1F380
    void SetFamily2();                      // 0x00C1F3C0
    long Handle(long a, long b);
};

long CItemAction::Handle(long a, long b)
{
    long result = m_pItem->GetTarget()->Handle(a, b);
    if (result < 0) {
        if (result == -1)
            SetFamily2();
        else
            SetVariant2();
        result = m_pItem->GetTarget()->Handle(a, b);
    }
    return result;
}