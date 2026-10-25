// std::__unguarded_linear_insert(SKeyedPair* last, SKeyedPair val, SKeyGreater comp) -- shift the greater-keyed pairs before last up one place and drop val into the gap (insertion-sort step without a lower bound)
// Leaf: ??$__unguarded_linear_insert@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0088C780, length 52 bytes (+ int3 padding), `ret 0Ch`.
// Identity: one caller, 0x0088CD88 in the __linear_insert 0x0088CD30 of the same 8-byte sort family as the landed
//   __push_heap 0x0088C7C0, _Adjust_heap 0x0088CA20, __insertion_sort 0x0088CE60 and sort 0x0088D130 (CVectorMap Sort
//   0x0088D1B0). The catalog label "RBTree_InsertBefore" is wrong. Element and comparator follow the landed 0x0088C7C0
//   model (descriptive names): a { long key, dword value } pair under a SIGNED greater-key comparator.
// Shape: STLport 4.x `__unguarded_linear_insert(last, val, comp)` in a /Gr TU (`__fastcall`: last in ecx, the 8-byte
//   value and the empty comparator by value on the stack, `ret 0Ch`); val.key stays in edx (`cmp edx,[next] ; jle`),
//   each shifted pair goes through esi, val.m_value is read from its slot for the final store.
struct SKeyedPair {                             // 8 bytes: a signed key and its payload (descriptive name)
    long          m_key;                        // +0x00
    unsigned long m_value;                      // +0x04
    SKeyedPair(const SKeyedPair& other) : m_key(other.m_key), m_value(other.m_value) { }
};

struct SKeyGreater {                            // empty comparator passed by value (one stack slot)
    char m_tag;
    bool operator()(const SKeyedPair& a, const SKeyedPair& b) const { return a.m_key > b.m_key; }
};

namespace std {
template <class _RandomAccessIter, class _Tp, class _Compare>
inline void __fastcall __unguarded_linear_insert(_RandomAccessIter __last, _Tp __val, _Compare __comp)
{
    _RandomAccessIter __next = __last;
    --__next;
    while (__comp(__val, *__next)) {
        *__last = *__next;
        __last = __next;
        --__next;
    }
    *__last = __val;
}
template void __fastcall __unguarded_linear_insert<SKeyedPair*, SKeyedPair, SKeyGreater>(SKeyedPair*, SKeyedPair, SKeyGreater);
}