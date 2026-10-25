// CVisitedItem::MatchesKey(long key) -- walks the key's matches through the item's claim source (FindNext fills two words), hands each pair back to the source and returns how many there were
// Leaf: MatchesKey
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C1EC70, 113 bytes (`ret 4`). Descriptive name (no PDB covers this library): the
//   manifest's "Container_IterateAndCount" is a placeholder; the name follows the landed callers, the visitors
//   CCollectFreeMatchesA/B::Visit 0x00C10150 / 0x00C10190 (`if (item->MatchesKey(m_key)) ...`).
// Callees: the item's slot 5 (+0x14, the claim source, re-fetched for every call), the source's FindNext slot 10
//   (+0x28: key, &first, &second) and Process slot 8 (+0x20: first, second).
// Shape: a rotated `while (FindNext(...))` loop with the count in edi (`inc edi` scheduled before the re-fetch) and the
//   key in ebx; the two out-words live in the 8-byte frame, `first` above `second`.
struct CClaimSource {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07();
    virtual void Process(long first, long second);                  // slot 8 (+0x20)
    virtual void v09();
    virtual long FindNext(long key, long* first, long* second);     // slot 10 (+0x28)
};

struct CVisitedItem {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual CClaimSource* GetClaimSource();                         // slot 5 (+0x14)
    long MatchesKey(long key);
};

long CVisitedItem::MatchesKey(long key)
{
    long count = 0;
    long first, second;
    while (GetClaimSource()->FindNext(key, &first, &second)) {
        GetClaimSource()->Process(first, second);
        ++count;
    }
    return count;
}