// _STL::__unguarded_linear_insert(SUniqueIDOpen* last, SUniqueIDOpen val, CUniqueIDOpenLess comp) -- insertion-sort step over the 16-byte (unsigned __int64 unique ID, bool open) entries: shift every larger entry up one slot, then drop the value into the hole
// Leaf: ??$__unguarded_linear_insert@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A7D570, 83 bytes, `ret 14h` (/Gr: last in ecx; the 16-byte value and the empty
//   comparator's slot on the stack).
// Callers: __copy-style insert loop 0x00A7EFE0 and the insertion-sort step 0x00A81C50 (this chunk), both copying the
//   value as two dwords plus ONE byte -- the entry is pair<unsigned __int64, bool>, the CNavSwitchableLeafNode
//   UniqueIDsOpen map (CVectorMap<unsigned __int64, bool>, landed LowerBound 0x00A7DF60). Byte-identical twin:
//   0x00A7D610 (the 16-byte pair<unsigned __int64, CANavQuadTreeNode*> instantiation). Catalog
//   "SortedArray_InsertWithBackwardShift" is descriptive.
// Shape: STLport verbatim -- `next = last; --next; while (comp(val, *next)) { *last = *next; last = next; --next; } *last
//   = val;` with the 64-bit key compare inline (high word `ja`/`jb`, then the low word) and the pair's trivial
//   assignment copying all 16 bytes (padding included) as four dwords.
namespace _STL {
template <class _T1, class _T2> class pair {
public:
    _T1 first;
    _T2 second;
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }
};
}

template <class K, class V> class CKeyPairCompareLess {
public:
    bool operator()(const _STL::pair<K, V>& a, const _STL::pair<K, V>& b) const { return a.first < b.first; }
};

typedef _STL::pair<unsigned __int64, bool>          SUniqueIDOpen;      // 16 bytes (7 bytes of tail padding)
typedef CKeyPairCompareLess<unsigned __int64, bool> CUniqueIDOpenLess;

namespace _STL {
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
template void __fastcall __unguarded_linear_insert(SUniqueIDOpen*, SUniqueIDOpen, CUniqueIDOpenLess);
}