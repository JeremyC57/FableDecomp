// std::__unguarded_linear_insert(pair<ETCInterfaceType, CTCBase*>* last, pair<ETCInterfaceType, CTCBase*> val, CKeyPairCompareLess comp) -- STLport insertion-sort inner step over a thing's component map: shift entries with a larger interface type up one slot until val's place, then store it
// Leaf: ??$__unguarded_linear_insert@
// Retail: Fable.exe (TLC, VC7.1) @ 0x004CB5E0, 51 bytes (+ int3 padding), `ret 0Ch` (/Gr algorithm TU, explicit
//   __fastcall here: last in ecx, the 8-byte value and the empty comparator slot on the stack). No callees.
// Caller: the matching __linear_insert 0x004CD0C0 (this chunk).
// Identity: CThing::Components is CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<...> > (Ego_r.pdb names the
//   whole `pair<ETCInterfaceType, CTCBase*>` algorithm family, e.g. _Adjust_heap / _Insertion_sort1 with
//   CKeyPairCompareLess<ETCInterfaceType, CTCBase*>); the key compare is signed (`jge` / `jl`). The catalog's
//   "std::_Insertion_sort_BinaryInsert" is descriptive.
// Shape: STLport 4.x `next = last; --next; while (comp(val, *next)) { *last = *next; last = next; --next; } *last = val;`
//   -- the value's key stays in edx, the next key is pre-loaded at the loop bottom (`mov esi,[eax-8]`).
enum ETCInterfaceType { TC_NONE = 0 };                  // the thing-component interface type (a signed key)
class CTCBase;

namespace std {
template <class _T1, class _T2> struct pair {          // STLport pair: user constructors, implicit operator=
    _T1 first;                                          // +0x00
    _T2 second;                                         // +0x04
    pair() : first(_T1()), second(_T2()) { }
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }
};
}

template <class K, class V> class CKeyPairCompareLess { // Ego_r.pdb: empty, passed by value (4-byte slot)
public:
    bool operator()(const std::pair<K, V>& a, const std::pair<K, V>& b) const { return a.first < b.first; }
};

typedef std::pair<ETCInterfaceType, CTCBase*> TComponentPair;          // CThing::Components entry, 8 bytes
typedef CKeyPairCompareLess<ETCInterfaceType, CTCBase*> TComponentLess;

namespace std {
template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __unguarded_linear_insert(_RandomAccessIter __last, _Tp __val, _Compare __comp)
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
template void __fastcall __unguarded_linear_insert<TComponentPair*, TComponentPair, TComponentLess>(TComponentPair*, TComponentPair, TComponentLess);
}