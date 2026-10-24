// std::sort_heap(SAnimationEntryPair* first, SAnimationEntryPair* last, CKeyPairCompareLess comp) -- SGI sort_heap over the (CRC, CAnimationEntry) vector map: pop the root to the back until one element remains
// Leaf: sort_heap
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005E2FB0, length 137 bytes (+ int3 padding), `ret 4`.
// Identity: the catalog label `_Sort_heap<pair<unsigned long,C3DHelperPoint>*,...>` is propagated over the
// byte-identical skeleton; the only callee is the landed SGI `__pop_heap<pair<unsigned long, CAnimationEntry>>`
// @ 0x005E21F0, so this is sort_heap over the same 0x14-byte element (stride 20: the 0x66666667 / sar 3 divide).
// Callees: __pop_heap @ 0x005E21F0 (out of line, __fastcall: first/last in ecx/edx, then result, the 0x14-byte
// value, the comparator and the Distance* on the stack).
// Shape: the SGI algorithm TUs are /Gr -- the free templates are __fastcall (first in ecx, last in edx, the
// empty comparator by value on the stack). sort_heap is `while (last - first > 1) pop_heap(first, last--, comp)`;
// pop_heap and __pop_heap_aux are inline, so the loop pushes `(int*)0`, the comparator dword, builds the value
// copy `T(*(last - 1))` in the argument area through the pair's implicit copy constructor (four dword moves, then
// the counted pointer's inline copy with `inc [ecx+4]`) and passes `last - 1` both as last and as result.
// Element model from the landed 0x005E21F0 source (Ego_r.pdb CAnimationEntry members).
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

void __fastcall __pop_heap(SAnimationEntryPair* first, SAnimationEntryPair* last, SAnimationEntryPair* result,
                           SAnimationEntryPair value, CKeyPairCompareLess comp, int*);   // 0x005E21F0

inline void __pop_heap_aux(SAnimationEntryPair* first, SAnimationEntryPair* last, SAnimationEntryPair*,
                           CKeyPairCompareLess comp)
{
    __pop_heap(first, last - 1, last - 1, SAnimationEntryPair(*(last - 1)), comp, (int*)0);
}

inline void pop_heap(SAnimationEntryPair* first, SAnimationEntryPair* last, CKeyPairCompareLess comp)
{
    __pop_heap_aux(first, last, (SAnimationEntryPair*)0, comp);
}

void __fastcall sort_heap(SAnimationEntryPair* first, SAnimationEntryPair* last, CKeyPairCompareLess comp)
{
    while (last - first > 1)
        pop_heap(first, last--, comp);
}