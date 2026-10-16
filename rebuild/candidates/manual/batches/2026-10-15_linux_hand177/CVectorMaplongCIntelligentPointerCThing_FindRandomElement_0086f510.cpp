// CVectorMap<long, CIntelligentPointer<CThing> >::FindRandomElement(long key, CRandomGenerator& random) -- pick one of the things stored under `key` at random: the [LowerBound, UpperBound) range is sized and indexed with one step of the caller's generator; 0 when the key is absent
// Leaf: FindRandomElement
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0086F510, length 122 bytes (padded to 128), `ret 8`.
// Callees: CVectorMap::LowerBound(const long&) @ 0x0086F180, CVectorMap::UpperBound(const long&) @ 0x0086F1E0 (catalog label
// "CVectorMap<EGameAction,...>::UpperBound" is a byte-twin propagation; the element here is pair<long, CIntelligentPointer<CThing> >,
// 12 bytes: key +0, the intelligent pointer's vptr +4 and item +8), GFROR13 @ 0x00497890 (`__fastcall`).
// Shape: the element count is a named local computed before the generator step (`imul 2AAAAAABh ; sar 1` = /12 before
// `seed = GFROR13(seed * 0x24A1 + 0x24DF)`), then the landed 0x008EE720 generator `n ? r % n : 0` (unsigned `div`); the hit arm is the
// `if (first != last) { ... }` body so the `return 0` sits at the end.
long __fastcall GFROR13(unsigned long value);      // 0x00497890
struct CRandomGenerator {
    unsigned long Seed;
    __forceinline unsigned long Next() { Seed = Seed * 0x24a1 + 0x24df; Seed = GFROR13(Seed); return Seed; }
    __forceinline unsigned long Random(unsigned long n) { unsigned long r = Next(); return n ? r % n : 0; }
};
struct CThing;
struct CIntelligentPointer_CThing {                // 8 bytes: vptr + item
    virtual void v00();
    CThing* PItem;
    CThing* GetPItem() const { return PItem; }
};
struct CThingEntry {                               // pair<long, CIntelligentPointer<CThing> >
    long                       first;
    CIntelligentPointer_CThing second;
};
struct CThingVectorMap {                           // CVectorMap<long, CIntelligentPointer<CThing>, CKeyPairCompareLess<...> >
    CThingEntry* m_first;
    CThingEntry* m_last;
    CThingEntry* m_end;
    const CThingEntry* LowerBound(const long& key) const;   // 0x0086F180
    const CThingEntry* UpperBound(const long& key) const;   // 0x0086F1E0
    CThing* FindRandomElement(long key, CRandomGenerator& random) const;
};
CThing* CThingVectorMap::FindRandomElement(long key, CRandomGenerator& random) const
{
    const CThingEntry* first = LowerBound(key);
    const CThingEntry* last = UpperBound(key);
    if (first != last) {
        long count = last - first;
        return first[random.Random(count)].second.GetPItem();
    }
    return 0;
}
