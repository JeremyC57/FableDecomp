// CVisitedItem::ContainsValue(long value) -- 1 when any entry of any group of the item's claim source equals the value, else 0
// Leaf: ContainsValue
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C1ECF0, 133 bytes (`ret 4`). Descriptive name (no PDB covers this library): the
//   manifest's "Container_SearchByValue" is a placeholder. No references in the image (no call, jump or vtable slot).
//   Same item class as MatchesKey 0x00C1EC70 (slot 5 = the claim source).
// Callees: the item's slot 5 (+0x14), re-fetched before every source call; the source's GetGroupCount slot 5 (+0x14),
//   GetCount slot 4 (+0x10, group) and GetAt slot 6 (+0x18, group, index).
// Shape: two rotated unsigned `for` loops (`jbe` / `jb`), group in ebx, index in edi, the value in ebp; the two returns
//   are full-register 1 / 0 (a long, not a bool); `value == GetAt(...)` with the value on the left gives `cmp ebp,eax`.
struct CClaimSource {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual unsigned long GetCount(unsigned long group);            // slot 4 (+0x10)
    virtual unsigned long GetGroupCount();                          // slot 5 (+0x14)
    virtual long GetAt(unsigned long group, unsigned long index);   // slot 6 (+0x18)
};

struct CVisitedItem {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual CClaimSource* GetClaimSource();                         // slot 5 (+0x14)
    long ContainsValue(long value);
};

long CVisitedItem::ContainsValue(long value)
{
    for (unsigned long group = 0; group < GetClaimSource()->GetGroupCount(); ++group)
        for (unsigned long index = 0; index < GetClaimSource()->GetCount(group); ++index)
            if (value == GetClaimSource()->GetAt(group, index))
                return 1;
    return 0;
}