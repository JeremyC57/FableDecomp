// std::sort_heap(SAnimComponentPair* first, SAnimComponentPair* last, CKeyPairCompareLess comp) -- SGI sort_heap over the (EAnimComponent, CIVCountedPointer<CAnimComponentBase>) vector map: pop the root to the back until one element remains
// Leaf: sort_heap
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005E2860, length 88 bytes, `ret 4`.
// Identity: the catalog label `_Sort_heap<std::pair<long,unsigned long>*>` is propagated over the byte-identical
// skeleton. The callee 0x005E1B60 (manifest "pair<long,CDefPointer<...>>::operator=", also propagated) is the SGI
// `__pop_heap` of this element: it assigns the 8-byte pair inline and forwards to the manifest's
// `_Adjust_heap<pair<EAnimComponent,CIVCountedPointer<CAnimComponentBase>>*,...>` @ 0x005E1210, `ret 14h`
// (result, the 8-byte value, the comparator, the Distance*).
// Callees: __pop_heap @ 0x005E1B60 (out of line, __fastcall).
// Shape: /Gr SGI algorithm TU -- sort_heap is `while (last - first > 1) pop_heap(first, last--, comp)` with
// pop_heap / __pop_heap_aux inline; the 8-byte stride turns the count test into `and eax,-8 ; cmp eax,8`.
// The value copy is built in the argument area by the pair's implicit copy constructor: the key dword, then
// the intrusive counted pointer's inline copy (`test ; je ; inc [eax+4]`, the count sits at +4 of the pointee).
struct CAnimComponentBase {                 // intrusively counted (CIVCountedPointeeBase): vptr, count at +4
    virtual void v00();
    virtual void Destroy();                 // slot 1
    long m_count;                           // +4
};
struct CIVCountedPointer {
    CAnimComponentBase* m_p;
    CIVCountedPointer(const CIVCountedPointer& o) : m_p(o.m_p) { if (m_p) ++m_p->m_count; }
    ~CIVCountedPointer() { if (m_p && --m_p->m_count == 0) m_p->Destroy(); }
};
struct SAnimComponentPair {                 // std::pair<EAnimComponent, CIVCountedPointer<CAnimComponentBase>> (8 bytes)
    long              first;                // +0x00 EAnimComponent
    CIVCountedPointer second;               // +0x04
};
struct CKeyPairCompareLess {                // empty, passed by value
    bool operator()(const SAnimComponentPair& a, const SAnimComponentPair& b) const { return a.first < b.first; }
};

void __fastcall __pop_heap(SAnimComponentPair* first, SAnimComponentPair* last, SAnimComponentPair* result,
                           SAnimComponentPair value, CKeyPairCompareLess comp, int*);   // 0x005E1B60

inline void __pop_heap_aux(SAnimComponentPair* first, SAnimComponentPair* last, SAnimComponentPair*,
                           CKeyPairCompareLess comp)
{
    __pop_heap(first, last - 1, last - 1, SAnimComponentPair(*(last - 1)), comp, (int*)0);
}

inline void pop_heap(SAnimComponentPair* first, SAnimComponentPair* last, CKeyPairCompareLess comp)
{
    __pop_heap_aux(first, last, (SAnimComponentPair*)0, comp);
}

void __fastcall sort_heap(SAnimComponentPair* first, SAnimComponentPair* last, CKeyPairCompareLess comp)
{
    while (last - first > 1)
        pop_heap(first, last--, comp);
}