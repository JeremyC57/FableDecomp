// _global::__pop_heap(SAnimationEntryPair* first, SAnimationEntryPair* last, SAnimationEntryPair* result, SAnimationEntryPair value, CKeyPairCompareLess comp, int*) -- SGI pop_heap core for the (CRC, CAnimationEntry) vector map: *result = *first, then sift `value` down from the root
// Leaf: __pop_heap
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005E21F0, length 119 bytes (`ret 20h`: result, the 0x14-byte value, the comparator
// and the distance pointer on the stack; first/last in ecx/edx -- the SGI algorithm TUs are /Gr).
// Label correction: the catalog "CParticleEffect::GetDataFromDef" is propagated and wrong; the callees are the pair's operator=
// @ 0x005E0E20 (manifest "CAnimationEntry::operator=") and 0x005E1CB0 (manifest `_Adjust_heap<std::pair<unsigned long,
// CAnimationEntry>*,int,...,CKeyPairCompareLess<...>>`), and the only callers (0x005E23D5 ...) are the __pop_heap wrappers that
// copy the last element into the argument area -- this is the matching __pop_heap.
// Callees: pair<unsigned long, CAnimationEntry>::operator= @ 0x005E0E20 (out of line), __adjust_heap @ 0x005E1CB0.
// Element: pair<unsigned long, CAnimationEntry>, 0x14 bytes (Ego_r.pdb CAnimationEntry: BankIndex +0, AnimName +4, GroupName +8
// (CDefString = one table-position word each), Components +0xc = CIVCountedPointer<CAnimComponentMap>, an intrusive count at +4
// of the object, destroyed through its vtable slot 1).
// Shape: the by-value `value` is copied into __adjust_heap's argument area by the pair's implicit copy ctor (four dword moves,
// then the counted pointer's inline copy with `inc [edi+4]`), the comparator dword is pushed first, the count is the /20
// reciprocal multiply; this callee destroys its own `value` parameter at the end (the inline counted release).
struct CAnimComponentMap {                  // intrusively counted object: vptr, count at +4
    virtual void v00();
    virtual void Destroy();                 // slot 1
    long m_count;                           // +4
};
struct CIVCountedPointer {
    CAnimComponentMap* m_p;
    CIVCountedPointer(const CIVCountedPointer& o) : m_p(o.m_p) { if (m_p) ++m_p->m_count; }
    ~CIVCountedPointer() { if (m_p && --m_p->m_count == 0) m_p->Destroy(); }
};
struct CAnimationEntry {                    // 0x10 bytes
    long BankIndex;                         // +0x00
    long AnimName;                          // +0x04 CDefString (table position)
    long GroupName;                         // +0x08 CDefString (table position)
    CIVCountedPointer Components;           // +0x0c
};
struct SAnimationEntryPair {                // std::pair<unsigned long, CAnimationEntry> (0x14 bytes)
    unsigned long   first;                  // +0x00
    CAnimationEntry second;                 // +0x04
    SAnimationEntryPair& operator=(const SAnimationEntryPair& other);   // 0x005E0E20
};
struct CKeyPairCompareLess {                // empty, passed by value
    bool operator()(const SAnimationEntryPair& a, const SAnimationEntryPair& b) const { return a.first < b.first; }
};
void __fastcall __adjust_heap(SAnimationEntryPair* first, int holeIndex, int len, SAnimationEntryPair value,
                              CKeyPairCompareLess comp);   // 0x005E1CB0
void __fastcall __pop_heap(SAnimationEntryPair* first, SAnimationEntryPair* last, SAnimationEntryPair* result,
                           SAnimationEntryPair value, CKeyPairCompareLess comp, int*)
{
    *result = *first;
    __adjust_heap(first, 0, int(last - first), value, comp);
}